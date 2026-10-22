// 指令參考產生器：由 _build/protocol/*.json（單一真相來源）＋ products.json 產出
//   sensors/<p>/reference.md、arduino-i2c.md、mblock.md、matrix.md、microbit.md（中英同出，英文到 en/）
//   reference.md、en/reference.md（全站索引）、llms-protocol.txt
//   標記式局部覆寫：i2c-protocol.md、makeblock.md、guide.md、_sidebar.md、en/_sidebar.md、llms.txt
//     （頁面裡要有 <!-- gen:名稱 -->…<!-- /gen -->；沒有標記時只寫到 _build/generated/fragments/ 供手動貼上）
//
// 用法：node _build/gen-reference.mjs                產生全部產品
//       node _build/gen-reference.mjs --only line8   只產某幾款（逗號分隔）
//       node _build/gen-reference.mjs --check        重產比對，不一致時結束碼 1
import fs from 'node:fs';
import path from 'node:path';
import { ROOT, BUILD, loadCommon, loadProducts, loadSiteProducts, makeWriter, siteKey, hostOn, pageFile } from './gen-lib.mjs';
import * as refPage from './protocol/pages/reference.mjs';
import * as i2cPage from './protocol/pages/arduino-i2c.mjs';
import * as mblockPage from './protocol/pages/mblock.mjs';
import * as matrixPage from './protocol/pages/matrix.mjs';
import * as microbitPage from './protocol/pages/microbit.mjs';
import * as frag from './protocol/pages/fragments.mjs';

const argv = process.argv.slice(2);
const CHECK = argv.includes('--check');
const onlyIdx = argv.indexOf('--only');
const ONLY = onlyIdx > -1 ? argv[onlyIdx + 1].split(',') : null;

const common = loadCommon();
const products = loadProducts(ONLY);
const siteProducts = loadSiteProducts();
if (!products.length) { console.error('沒有產品 JSON（先跑 node _build/sync-protocol.mjs）'); process.exit(2); }

const w = makeWriter({ check: CHECK, label: 'gen-reference' });
const ctx = {
  hasFile: (rel) => fs.existsSync(path.join(ROOT, rel)),
  routes: (key) => (siteProducts[key] && siteProducts[key].routes) || {},
  hasMarker: (rel, name) => { const f = path.join(ROOT, rel); return fs.existsSync(f) && fs.readFileSync(f, 'utf8').includes(`<!-- gen:${name} -->`); },
};
const LANGS = ['zh', 'en'];
// 輸出路徑（相對 repo 根）：sensors/line8/reference.md、en/sensors/line8/reference.md
const file = (lang, p, base) => (lang === 'en' ? 'en' : '') + pageFile(p, base).replace(/^\//, lang === 'en' ? '/' : '');

// 1. 各產品五頁
for (const p of products) {
  const key = siteKey(p);
  for (const lang of LANGS) {
    w.write(file(lang, p, 'reference'), refPage.render(p, common, ctx, lang));
    if (hostOn(p, 'i2c')) w.write(file(lang, p, 'arduino-i2c'), i2cPage.render(p, common, ctx, lang));
    if (hostOn(p, 'mblock')) w.write(file(lang, p, 'mblock'), mblockPage.render(p, common, ctx, lang));
    if (hostOn(p, 'matrix') || hostOn(p, 'mvision')) w.write(file(lang, p, 'matrix'), matrixPage.render(p, common, ctx, lang));
    if (hostOn(p, 'microbit')) w.write(file(lang, p, 'microbit'), microbitPage.render(p, common, ctx, lang));
  }
}

// 2. 全站索引與純文字協定（全部產品 JSON，不受 --only 影響）
const all = loadProducts(null);
for (const lang of LANGS) w.write(`${lang === 'en' ? 'en/' : ''}reference.md`, frag.referenceIndex(all, siteProducts, ctx, lang));
w.write('llms-protocol.txt', frag.llmsProtocol(all, common));

// 3. 標記式片段
const missing = [];
// 中英頁面用同一個標記名稱（檔案不同）；片段檔存成 <名稱>.<語言>.md
function fragment(file, name, lang, content) {
  w.write(`_build/generated/fragments/${name}.${lang}.md`, content + '\n');
  const r = w.patch(file, name, content);
  if (r === null) missing.push(`${file} 缺 <!-- gen:${name} -->`);
}
for (const lang of LANGS) {
  const pre = lang === 'en' ? 'en/' : '';
  fragment(`${pre}i2c-protocol.md`, 'i2c-addr-table', lang, frag.i2cAddrTable(all, lang));
  fragment(`${pre}i2c-protocol.md`, 'i2c-common', lang, frag.i2cCommon(common, lang));
  fragment(`${pre}makeblock.md`, 'mblock-blocks', lang, frag.mblockBlocks(all, lang));
  fragment(`${pre}makeblock.md`, 'mblock-products', lang, frag.mblockProducts(all, lang));
  fragment(`${pre}guide.md`, 'guide-start', lang, frag.guideStart(siteProducts, lang));
  fragment(`${pre}llms.txt`, 'llms-platforms', lang, frag.llmsPlatforms(lang));
  for (const p of all) if (!p.id.endsWith('_wro')) fragment(`${pre}_sidebar.md`, `sidebar-${siteKey(p)}`, lang, frag.sidebarTree(p, ctx.routes(siteKey(p)), ctx, lang));
}
if (missing.length && !CHECK) {
  console.log(`以下頁面還沒放標記，片段只寫到 _build/generated/fragments/（${missing.length}）：`);
  missing.forEach((m) => console.log('  ' + m));
}
w.done();
