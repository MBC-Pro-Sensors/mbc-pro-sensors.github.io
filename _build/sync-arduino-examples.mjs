// Arduino 範例同步：把 examples/<產品>/arduino/*/*.ino 的內容填進範例頁，並打包 ZIP
//
// 範例頁（sensors/<產品>/arduino-examples.md）裡寫：
//   <!-- ino: examples/line8/arduino/line8_01_hello/line8_01_hello.ino -->
//   ```cpp
//   （這裡的內容會被整段換成 .ino 原文）
//   ```
// 用法：node _build/sync-arduino-examples.mjs          同步程式碼＋重新打包 ZIP
//       node _build/sync-arduino-examples.mjs --check  只檢查是否已同步（不一致時結束碼 1）
import fs from 'node:fs';
import path from 'node:path';
import { execFileSync } from 'node:child_process';
import { fileURLToPath } from 'node:url';

const ROOT = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '..');
const CHECK = process.argv.includes('--check');
// 結尾的 ``` 必須自成一行；空的佔位框（```cpp 下一行就是 ```）也能正確配對，不會吃到下一個程式框
const MARK = /(<!-- ino: ([^ ]+\.ino) -->\r?\n)```cpp\r?\n[\s\S]*?^```[ \t]*$/gm;

const pages = [];
for (const lang of ['', 'en/']) {
  const dir = path.join(ROOT, lang, 'sensors');
  if (!fs.existsSync(dir)) continue;
  for (const p of fs.readdirSync(dir)) {
    const f = path.join(dir, p, 'arduino-examples.md');
    if (fs.existsSync(f)) pages.push(f);
  }
}

let stale = 0;
for (const page of pages) {
  const src = fs.readFileSync(page, 'utf8');
  const out = src.replace(MARK, (_, head, ino) => {
    const code = fs.readFileSync(path.join(ROOT, ino), 'utf8').replace(/\r\n/g, '\n').trimEnd();
    return `${head}\`\`\`cpp\n${code}\n\`\`\``;
  });
  if (out !== src) {
    stale++;
    if (CHECK) console.log('未同步：' + path.relative(ROOT, page));
    else { fs.writeFileSync(page, out); console.log('已同步：' + path.relative(ROOT, page)); }
  }
}

if (CHECK) process.exit(stale ? 1 : 0);

// 每個產品打包一個 ZIP（內含各範例資料夾，Arduino IDE 可直接開啟）
// arduino/ = 中文版、arduino-en/ = 英文版（ZIP 檔名加 _en）
for (const p of fs.readdirSync(path.join(ROOT, 'examples'))) for (const [sub, suffix] of [['arduino', ''], ['arduino-en', '_en']]) {
  const dir = path.join(ROOT, 'examples', p, sub);
  if (!fs.existsSync(dir)) continue;
  // 只打包「產品_兩位數編號_名稱」格式的範例資料夾
  const sketches = fs.readdirSync(dir)
    .filter((d) => d.startsWith(p + '_') && /^\d\d_/.test(d.slice(p.length + 1)) && fs.existsSync(path.join(dir, d, d + '.ino')))
    .sort();
  if (!sketches.length) continue;
  const zip = `${p}_arduino_examples${suffix}.zip`;
  fs.rmSync(path.join(dir, zip), { force: true });
  execFileSync('tar', ['-a', '-c', '-f', zip, ...sketches], { cwd: dir });
  console.log(`已打包：examples/${p}/${sub}/${zip}（${sketches.length} 個範例）`);
}
