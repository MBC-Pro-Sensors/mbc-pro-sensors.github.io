// micro:bit 片段：MakeCode 擴充 pxt-mbc-sensors v2 積木文字＋ JavaScript（D5；積木名以發布版為準）
import { L, T, hex, valueName } from '../../gen-lib.mjs';

export function available(product) { return !!(product.hosts && product.hosts.microbit); }

const TS = 'typescript';
export const EXT_URL = 'https://github.com/contactmbcrobot/pxt-mbc-sensors';

export function blockText(product, v, lang) {
  if (!v.microbit || !v.microbit.block) return null;
  return `MBC ${L(v.microbit.block, lang).replace('[N]', T(lang, '［1~' + (v.count || 1) + '］', '[1~' + (v.count || 1) + ']'))}`;
}
export function commandText(product, c, lang) {
  if (!c.microbit || !c.microbit.block) return null;
  return `MBC ${L(c.microbit.block, lang)}`;
}

const blocks = (txt) => `<pre class="blocks">${txt}</pre>`;

export function value(product, v, lang) {
  if (!v.microbit || !v.microbit.block) return { text: T(lang, 'micro:bit 擴充沒有這顆積木（可用 `MBC.readValue(位址, 編號)` 通用積木讀）。', 'No dedicated block (use the generic `MBC.readValue(address, id)` block).') };
  const label = L(v.microbit.block, lang).replace('[N]', '[1 v]');
  const html = blocks(`(MBC ${label} :: #E8711E)`);
  const code = v.count
    ? `let ${v.key} = MBC.readValues(${product.addr}, ${hex(v.id)}, ${v.count})   // ${valueName(v, lang)}`
    : `let ${v.key} = MBC.readValue(${product.addr}, ${hex(v.id)})   // ${valueName(v, lang)}：${v.range}${v.unit ? ' ' + v.unit : ''}`;
  return { html, code, codeLang: TS, note: T(lang, '擴充內部用 `Int16LE` 讀 2 byte；寫完編號 `basic.pause(2)` 再讀。', 'The extension reads 2 bytes as `Int16LE` and pauses 2 ms after writing the id.') };
}

export function command(product, c, lang) {
  if (!c.microbit || !c.microbit.block) return { text: T(lang, '擴充沒有這顆積木（可用 `MBC.writeCmd` 通用積木）。', 'No dedicated block (use the generic `MBC.writeCmd` block).') };
  const label = L(c.microbit.block, lang).replace(/\[n\]/, '(50)');
  const args = (c.params || []).map(() => '50').join(', ');
  return { html: blocks(`MBC ${label} :: #E8711E`), code: `MBC.writeCmd(${product.addr}, ${hex(c.id)}${args ? `, [${args}]` : ''})   // ${L(c.name, lang)}`, codeLang: TS, note: '' };
}

export function status(product, lang) {
  return { html: blocks(`<MBC ${T(lang, '裝置就緒', 'device ready')} :: #E8711E>`), code: `let st = MBC.status(${product.addr})   // F3\nlet ready = (st & 1) != 0`, codeLang: TS, note: '' };
}
