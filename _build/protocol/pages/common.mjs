// 頁面樣板共用：頁首註解、六平台「怎麼讀」<details>、程式框、示意圖、頁尾假設
import { PLATFORMS, L, T, hex, pageHref, siteKey, pageFile, valueName, valueIdRange, table } from '../../gen-lib.mjs';
import * as i2c from '../snippets/i2c.mjs';
import * as mblock from '../snippets/mblock.mjs';
import * as ev3 from '../snippets/ev3.mjs';
import * as spike from '../snippets/spike.mjs';
import * as matrix from '../snippets/matrix.mjs';
import * as microbit from '../snippets/microbit.mjs';

export const SNIPPET = { spike, ev3, arduino: i2c, mblock, matrix, microbit };

export const AI_NOTE = '<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->';
export const genNote = (product, page) => `<!-- 本頁由 _build/gen-reference.mjs 從 _build/protocol/${product.id}.json 產生（${page}），請勿手改；改內容請改 JSON 後重跑 node _build/gen-reference.mjs -->`;

export const fence = (code, lang = 'text') => `\`\`\`${lang}\n${code.replace(/\s+$/, '')}\n\`\`\``;

export function backLink(product, lang) {
  return `[🔙 ${T(lang, `回到${L(product.name, lang)}產品頁`, `Back to ${L(product.name, lang)}`)}](${pageHref(lang, `/sensors/${siteKey(product)}/index.md`)})`;
}

// 平台 chip 列（連到各平台頁；沒有的平台灰掉）
export function chips(product, lang, routes, current) {
  const items = PLATFORMS.map((p) => {
    const on = product.hosts && product.hosts[p.hostKey];
    const r = routes && routes[p.id];
    // SPIKE／EV3 連到主機入口頁（hub），其他平台連到產生的說明頁
    const target = r && ((p.id === 'spike' || p.id === 'ev3') ? (r.start || r.ref) : (r.ref || r.start));
    const href = on && target ? pageHref(lang, target) : null;
    const cls = `chip chip-${p.id}${on ? '' : ' off'}${current === p.id ? ' current' : ''}`;
    return href ? `<a class="${cls}" href="${href}">${p.name[lang]}</a>` : `<span class="${cls}">${p.name[lang]}</span>`;
  });
  return `<p class="chips">${items.join('')}</p>`;
}

// 示意圖
export function figure(ctx, product, v, lang) {
  if (!v.figure || !v.figure.tpl) return '';
  const rel = `images/figures/${siteKey(product)}/value-${v.key}.svg`;
  if (!ctx.hasFile(rel)) return '';
  return `<figure class="ref-fig"><img src="/${rel}" alt="${valueName(v, lang)}" loading="lazy"></figure>`;
}
export function figureFile(ctx, rel, alt) {
  return ctx.hasFile(rel) ? `<figure class="ref-fig"><img src="/${rel}" alt="${alt}" loading="lazy"></figure>` : '';
}

// 六平台 <details>：kind = value | command | status；第一個有內容的平台預設展開
export function howTo(product, item, lang, kind) {
  const blocks = [];
  let opened = false;
  for (const p of PLATFORMS) {
    if (!(product.hosts && product.hosts[p.hostKey])) continue;
    const mod = SNIPPET[p.id];
    let r = null;
    try { r = kind === 'value' ? mod.value(product, item, lang) : kind === 'command' ? mod.command(product, item, lang) : mod.status(product, lang); }
    catch (e) { throw new Error(`${p.id} ${kind} ${item && item.id}: ${e.message}`); }
    if (!r) continue;
    const parts = [];
    if (r.text) parts.push(r.text);
    if (r.note) parts.push(r.note);
    if (r.html) parts.push(r.html);
    if (r.code) parts.push(fence(r.code, r.codeLang || 'text'));
    for (const x of r.extra || []) { if (x.title) parts.push(`**${x.title}**`); parts.push(fence(x.code, x.codeLang || 'text')); }
    const open = !opened && !!(r.code || r.html);
    if (open) opened = true;
    blocks.push(`<details class="plat plat-${p.id}" data-host="${p.id}"${open ? ' open' : ''}>\n<summary><span class="plat-dot"></span>${p.name[lang]}</summary>\n\n${parts.join('\n\n')}\n\n</details>`);
  }
  return blocks.join('\n\n');
}

// 值的資訊表
export function valueInfo(product, v, lang) {
  const rows = [
    [T(lang, '用途', 'Meaning'), L(v.desc, lang)],
    [T(lang, '範圍', 'Range'), `\`${v.range}\`${v.unit ? ` ${v.unit}` : ''}`],
    [T(lang, 'I2C／mBlock 編號', 'I2C / mBlock id'), `\`${valueIdRange(v)}\``],
  ];
  if (v.locked) rows.push([T(lang, '鎖住', 'Locked'), T(lang, '🔒 已鎖住的樂高模式排法不再改', '🔒 LEGO mode layout will not change')]);
  return table([T(lang, '項目', 'Item'), T(lang, '內容', 'Detail')], rows);
}

export function examplesLine(ctx, product, v, lang) {
  if (!v.examples || !v.examples.length) return '';
  const key = siteKey(product);
  const PAGE = { arduino: 'arduino-examples', ev3: 'ev3-examples', spike: 'spike-examples', mblock: 'mblock-examples', matrix: 'matrix-examples', microbit: 'microbit-examples' };
  const NAME = { arduino: 'Arduino', ev3: 'EV3', spike: 'SPIKE', mblock: 'mBlock', matrix: 'MATRIX', microbit: 'micro:bit' };
  const links = v.examples.map((e) => {
    const [plat, n] = e.split(':');
    const md = `/sensors/${key}/${PAGE[plat] || plat + '-examples'}.md`;
    const label = `${NAME[plat] || plat} ${T(lang, '範例', 'example')} ${n}`;
    // 該語言的範例頁還沒有就退回中文頁；中文頁也沒有就只列文字（不出斷鏈）
    if (lang === 'en' && ctx.hasFile('en' + md)) return `[${label}](/en${md})`;
    if (ctx.hasFile(md.slice(1))) return `[${label}](${md})`;
    return label;
  });
  return `**${T(lang, '相關範例', 'Related examples')}**：${links.join('、')}`;
}

// 共用暫存器連結：i2c-protocol.md 有 <!-- gen:i2c-common --> 標記（錨點 id 由它產生）才帶 #rXX
export function commonRef(ctx, lang, id) {
  const page = pageHref(lang, '/i2c-protocol.md');
  return ctx.hasMarker(`${lang === 'en' ? 'en/' : ''}i2c-protocol.md`, 'i2c-common') ? `${page}#r${id.toLowerCase()}` : page;
}
// 根目錄頁（downloads.md、microbit.md…）存在才給連結，否則回退到 fallback
export function rootLink(ctx, lang, md, fallback) {
  const rel = md.replace(/^\//, '');
  return ctx.hasFile(`${lang === 'en' ? 'en/' : ''}${rel}`) || ctx.hasFile(rel) ? pageHref(lang, md) : fallback;
}

export function assumptions(product, lang, extra = []) {
  const items = [...(product.assumptions || []), ...extra];
  const head = T(lang, '## 本頁依據與假設', '## Sources and assumptions');
  const src = T(lang,
    `本頁由 \`_build/gen-reference.mjs\` 從 \`_build/protocol/${product.id}.json\`（單一真相來源，轉錄自 2026-10-21 協定定案）產生。`,
    `Generated by \`_build/gen-reference.mjs\` from \`_build/protocol/${product.id}.json\` (single source of truth, transcribed from the 2026-10-21 protocol decisions).`);
  return `${head}\n\n${src}\n\n${items.map((a) => `- ${a}`).join('\n')}`;
}

export const flagsTable = (product, lang) => table(
  ['bit', T(lang, '名稱', 'Name'), T(lang, '說明', 'Meaning')],
  (product.flags || []).map((f) => [`b${f.bit}`, `${L(f.name, lang)} (\`${f.key}\`)`, L(f.desc, lang)]));

export { hex, L, T, table, pageHref, siteKey, pageFile, valueName, valueIdRange };
