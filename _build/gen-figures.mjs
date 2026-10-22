// 示意圖產生器：讀 _build/protocol/*.json 每個值的 figure.tpl/params → images/figures/<產品>/value-<key>.svg
//   另產接線圖 images/figures/<產品>/wiring-<主機>.svg 與封包圖 images/figures/common/packet-<名稱>.svg
// 用法：node _build/gen-figures.mjs            產生（有差異才覆寫）
//       node _build/gen-figures.mjs --check    重產比對，不一致時結束碼 1
//       node _build/gen-figures.mjs --only line8
import { loadProducts, loadSiteProducts, makeWriter, siteKey, hostOn, L } from './gen-lib.mjs';
import * as values from './figures/templates/values.svg.mjs';
import * as wiring from './figures/templates/wiring.svg.mjs';
import * as packet from './figures/templates/packet.svg.mjs';

const argv = process.argv.slice(2);
const CHECK = argv.includes('--check');
const onlyIdx = argv.indexOf('--only');
const ONLY = onlyIdx > -1 ? argv[onlyIdx + 1].split(',') : null;

const w = makeWriter({ check: CHECK, label: 'gen-figures' });
const products = loadProducts(ONLY);
const siteProducts = loadSiteProducts();
const generic = [];

for (const p of products) {
  const key = siteKey(p);
  // 值示意圖
  for (const v of p.values) {
    if (!v.figure || !v.figure.tpl) continue;
    if (!values.TEMPLATES[v.figure.tpl]) generic.push(`${p.id}.${v.key}: ${v.figure.tpl}`);
    let params = v.figure.params || {};
    // pad-buttons 沒給 group 時依值的 key 推（ps2.json 的 02~05 只寫 {}）
    if (v.figure.tpl === 'pad-buttons' && !params.group && !params.layout) {
      const g = /^dpad/.test(v.key) ? 'dpad' : /^shape/.test(v.key) ? 'shape' : /^shoulder/.test(v.key) ? 'shoulder' : /^func/.test(v.key) ? 'func' : /^sys/.test(v.key) ? 'sys' : null;
      if (g) params = { ...params, group: g };
    }
    w.write(`images/figures/${key}/value-${v.key}.svg`, values.render(v.figure.tpl, params));
  }
  // F3 旗標條（每款一張：images/figures/<產品>/flags.svg；神攝手兩版各自檔名）
  if (Array.isArray(p.flags) && p.flags.length) w.write(`images/figures/${key}/flags${p.id === 'cam_wro' ? '-wro' : ''}.svg`, values.render('flags', { bits: p.flags.map((f) => ({ bit: f.bit, key: f.key })) }));
  // 接線圖：依支援的平台
  const hosts = [];
  if (hostOn(p, 'i2c')) hosts.push('uno', 'esp32', 'r4');
  if (hostOn(p, 'microbit')) hosts.push('microbit');
  if (hostOn(p, 'mblock')) hosts.push('mbot');
  if (hostOn(p, 'matrix')) hosts.push('matrix');
  if (hostOn(p, 'spike')) hosts.push('spike');
  if (hostOn(p, 'ev3')) hosts.push('ev3');
  for (const h of hosts) w.write(`images/figures/${key}/wiring-${h}.svg`, wiring.render({ host: h, product: siteProducts[key] && siteProducts[key].name ? L(siteProducts[key].name, 'en').replace(/\s*\(.*\)$/, '') : L(p.name, 'en'), addr: ['uno', 'esp32', 'r4', 'microbit'].includes(h) ? `I2C ${p.addr}` : h === 'matrix' ? 'I2C 0x22' : '' }));
}
// 封包圖（共用）
for (const name of Object.keys(packet.PACKETS)) w.write(`images/figures/common/packet-${name}.svg`, packet.render(name));

if (generic.length && !CHECK) { console.log(`以下樣板還沒畫，先用通用佔位（${generic.length}）：`); generic.forEach((g) => console.log('  ' + g)); }
w.done();
