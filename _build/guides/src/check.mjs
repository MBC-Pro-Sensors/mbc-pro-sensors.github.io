// 檢查 _build/guides/{software,product} 的 md：``` 偶數、沒有 h1/h2、站內連結存在、佔位名稱合法、HTML 不縮排
import fs from 'fs';
import path from 'path';

const ROOT = process.argv[2];
const ALLOWED = new Set(['product', 'port', 'lib', 'libUrl', 'addr', 'edition']);
const files = [];
for (const sub of ['software', 'product']) {
  const walk = d => { for (const e of fs.readdirSync(d, { withFileTypes: true })) {
    const f = path.join(d, e.name);
    if (e.isDirectory()) walk(f); else if (f.endsWith('.md')) files.push(f);
  } };
  walk(path.join(ROOT, '_build', 'guides', sub));
}
let bad = 0, links = 0;
const err = (f, m) => { bad++; console.log('✗', path.relative(ROOT, f), m); };
for (const f of files) {
  const s = fs.readFileSync(f, 'utf8');
  const fences = (s.match(/^```/gm) || []).length;
  if (fences % 2) err(f, 'odd ``` count ' + fences);
  let inCode = false;
  for (const line of s.split('\n')) {
    if (line.startsWith('```')) { inCode = !inCode; continue; }
    if (inCode) continue;
    if (/^#{1,2} /.test(line)) err(f, 'h1/h2: ' + line);
    if (/^\s+</.test(line)) err(f, 'indented HTML: ' + line);
  }
  for (const m of s.matchAll(/\{\{(\w+)\}\}/g)) if (!ALLOWED.has(m[1])) err(f, 'bad placeholder ' + m[0]);
  if (/%%[A-Z0-9]+%%/.test(s)) err(f, 'leftover %%var%%');
  if (f.includes(path.sep + 'product' + path.sep) && /\{\{/.test(s)) { /* product files may use placeholders too */ }
  const targets = [...s.matchAll(/\]\((\/[^)\s]*)\)/g), ...s.matchAll(/(?:src|href)="(\/[^"]*)"/g)].map(m => m[1]);
  for (let t of targets) {
    links++;
    t = decodeURIComponent(t.split('#')[0].split('?')[0]);
    let p = path.join(ROOT, t);
    let ok = fs.existsSync(p);
    if (!ok && t.endsWith('.html')) ok = fs.existsSync(p.replace(/\.html$/, '.md'));
    if (!ok) err(f, 'missing link target ' + t);
  }
}
console.log(`${files.length} files, ${links} internal links, ${bad} problems`);
