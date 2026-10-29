// 下載區打包：照 _build/downloads.json 用 tar -a -c -f 打 zip（Windows 內建 bsdtar），並輸出 _build/generated/downloads.json 供產生 downloads.md
// 用法：node _build/pack-downloads.mjs            重新打包（legacy 項目不動）
//       node _build/pack-downloads.mjs --check    檢查 zip 存在、檔案清單與每個檔的內容都跟正本一致、generated/downloads.json 一致；不一致結束碼 1
import fs from 'node:fs';
import path from 'node:path';
import os from 'node:os';
import { execFileSync } from 'node:child_process';
import { fileURLToPath } from 'node:url';
import { FLAGS } from './site-flags.mjs';

const BUILD = path.dirname(fileURLToPath(import.meta.url));
const ROOT = path.resolve(BUILD, '..');
const CHECK = process.argv.includes('--check');
const MBC = 'D:\\AI_Agent_AI助理\\02_MBC產品開發';
// Git Bash 的 GNU tar 不會依副檔名出 zip；固定用 Windows 內建的 bsdtar
const TAR = process.platform === 'win32' && fs.existsSync('C:\\Windows\\System32\\tar.exe') ? 'C:\\Windows\\System32\\tar.exe' : 'tar';
const EXCLUDE = /(\.bak|\.before_|__pycache__|\.pyc$|~$|\.DS_Store)/;   // .before_* = 改正本前的備份檔

const spec = JSON.parse(fs.readFileSync(path.join(BUILD, 'downloads.json'), 'utf8'));
const resolveSrc = (s) => s && s.replace('${MBC}', MBC).replace(/\//g, path.sep);

// zip 裡的檔名：bsdtar 在 Windows 用 ANSI 碼頁寫中文檔名，別的系統解壓會變亂碼 → 用 rename 對照表改成 ASCII；沒對照到的非 ASCII 名稱會警告
function zipName(rel, rename) {
  const out = rel.split('/').map((seg) => (rename && rename[seg]) || seg).join('/');
  if (/[^\x20-\x7e]/.test(out)) console.log(`  警告：zip 內非 ASCII 檔名（請在 downloads.json 的 rename 加對照）：${rel}`);
  return out;
}
// 遞迴列出要包的檔案（相對 src，正斜線），排除備份／快取
function listFiles(src, include) {
  const out = [];
  const walk = (rel) => {
    const abs = path.join(src, rel);
    if (EXCLUDE.test(rel)) return;
    if (fs.statSync(abs).isDirectory()) { for (const f of fs.readdirSync(abs).sort()) walk(rel ? `${rel}/${f}` : f); }
    else out.push(rel);
  };
  for (const inc of include && include.length ? include : fs.readdirSync(src).sort()) {
    if (!fs.existsSync(path.join(src, inc))) throw new Error(`${src} 裡找不到 ${inc}`);
    walk(inc);
  }
  return out.sort();
}
const zipList = (zip) => execFileSync(TAR, ['-t', '-f', zip], { encoding: 'utf8' }).split(/\r?\n/).filter((l) => l && !l.endsWith('/')).map((l) => l.replace(/\\/g, '/')).sort();
// zip 裡跟正本內容不同的檔（只比檔名清單會漏掉「正本改了內容」）；回傳不同的檔名，清單不同時回傳 null
function zipDiff(zip, files, src, srcOf) {
  if (JSON.stringify(zipList(zip)) !== JSON.stringify(files)) return null;
  return files.filter((f) => !execFileSync(TAR, ['-x', '-O', '-f', zip, f], { maxBuffer: 64 << 20 }).equals(fs.readFileSync(path.join(src, srcOf[f]))));
}

const generated = [];
let bad = 0;
for (const it of spec.items) {
  const outAbs = path.join(ROOT, it.out);
  // site-flags.json 隱藏的項目：不打包、不列進下載表，已存在的 zip 拿掉（正本在 src，要恢復把旗標改回 false 重跑）
  if (it.hideWhen && FLAGS[it.hideWhen]) {
    if (fs.existsSync(outAbs)) { if (CHECK) { console.log(`應隱藏但還在：${it.out}`); bad++; } else { fs.rmSync(outAbs); console.log(`已移除（隱藏）：${it.out}`); } }
    continue;
  }
  const entry = { out: '/' + it.out, label: it.label, version: it.version, firmware: it.firmware, legacy: !!it.legacy, files: null, size: null };
  if (it.legacy || !it.src) {
    if (!fs.existsSync(outAbs)) { console.log(`缺既有檔：${it.out}`); bad++; }
    else entry.size = fs.statSync(outAbs).size;
    generated.push(entry);
    continue;
  }
  const src = resolveSrc(it.src);
  if (!fs.existsSync(src)) { console.log(`找不到正本：${src}`); bad++; generated.push(entry); continue; }
  const srcFiles = listFiles(src, it.include);
  const files = srcFiles.map((f) => zipName(f, it.rename)).sort();
  const srcOf = Object.fromEntries(srcFiles.map((f) => [zipName(f, it.rename), f]));
  entry.files = files;
  if (CHECK) {
    if (!fs.existsSync(outAbs)) { console.log(`未打包：${it.out}`); bad++; }
    else {
      const diff = zipDiff(outAbs, files, src, srcOf);
      if (!diff) { console.log(`內容不一致：${it.out}\n    zip：${zipList(outAbs).join(', ')}\n    正本：${files.join(', ')}`); bad++; }
      else if (diff.length) { console.log(`檔案內容跟正本不同：${it.out}\n    ${diff.join(', ')}`); bad++; }
      entry.size = fs.statSync(outAbs).size;
    }
  } else if (fs.existsSync(outAbs) && zipDiff(outAbs, files, src, srcOf)?.length === 0) {
    // 內容跟正本完全一樣就不重打（重打只會改時間戳，git 會多一筆無意義的變更）
    entry.size = fs.statSync(outAbs).size;
  } else {
    // 先複製到暫存（排除備份檔），再打包，zip 裡只有乾淨的檔案
    const tmp = fs.mkdtempSync(path.join(os.tmpdir(), 'mbc-pack-'));
    for (const f of files) { const d = path.join(tmp, f); fs.mkdirSync(path.dirname(d), { recursive: true }); fs.copyFileSync(path.join(src, srcOf[f]), d); }
    fs.mkdirSync(path.dirname(outAbs), { recursive: true });
    fs.rmSync(outAbs, { force: true });
    const top = [...new Set(files.map((f) => f.split('/')[0]))];
    execFileSync(TAR, ['-a', '-c', '-f', outAbs, ...top], { cwd: tmp });
    fs.rmSync(tmp, { recursive: true, force: true });
    entry.size = fs.statSync(outAbs).size;
    console.log(`已打包：${it.out}（${files.length} 個檔）`);
  }
  generated.push(entry);
}

const genPath = path.join(BUILD, 'generated', 'downloads.json');
const genText = JSON.stringify({ generatedBy: '_build/pack-downloads.mjs', items: generated }, null, 2) + '\n';
if (CHECK) {
  const cur = fs.existsSync(genPath) ? fs.readFileSync(genPath, 'utf8') : '';
  // 大小會隨 zip 時間戳微變，比對時忽略 size
  const strip = (s) => s.replace(/"size": \d+/g, '"size": 0');
  if (strip(cur) !== strip(genText)) { console.log('generated/downloads.json 與清單不一致'); bad++; }
} else {
  fs.mkdirSync(path.dirname(genPath), { recursive: true });
  fs.writeFileSync(genPath, genText);
  console.log(`已寫 _build/generated/downloads.json（${generated.length} 項）`);
}
if (bad) { console.log(`問題 ${bad} 項`); process.exit(1); }
console.log(CHECK ? '下載區全部一致' : '完成');
