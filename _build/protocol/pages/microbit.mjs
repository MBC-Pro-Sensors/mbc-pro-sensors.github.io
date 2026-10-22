// sensors/<p>/microbit.md：MakeCode 擴充積木表＋接線
import { AI_NOTE, genNote, backLink, chips, figureFile, assumptions, pageFile, rootLink, flagsTable, fence, hex, L, T, table, pageHref, siteKey, valueName, valueIdRange } from './common.mjs';
import * as mb from '../snippets/microbit.mjs';

export function render(product, common, ctx, lang) {
  const name = L(product.name, lang);
  const key = siteKey(product);
  const routes = ctx.routes(key);
  const ref = pageHref(lang, pageFile(product, 'reference'));
  const out = [AI_NOTE, genNote(product, 'microbit')];
  out.push(`# ${T(lang, `${name} × micro:bit（MakeCode 擴充）`, `${name} × micro:bit (MakeCode extension)`)}`, '', backLink(product, lang), '');
  out.push(T(lang,
    `${name}通用 I2C 版可以接 BBC micro:bit V1／V2，用 MakeCode 擴充 **pxt-mbc-sensors v2** 的積木讀值（內部走 [MBC I2C 協議 v2](${pageHref(lang, '/i2c-protocol.md')})）。擴充安裝、硬體注意事項（3.3 V、供電）請看 [micro:bit 總覽](${rootLink(ctx, lang, '/microbit.md', mb.EXT_URL)})。`,
    `The universal I2C edition of the ${name} works with BBC micro:bit V1 / V2 through the MakeCode extension **pxt-mbc-sensors v2** (which speaks [MBC I2C protocol v2](${pageHref(lang, '/i2c-protocol.md')})). Installation and hardware notes (3.3 V, power): [micro:bit overview](${rootLink(ctx, lang, '/microbit.md', mb.EXT_URL)}).`));
  out.push('', chips(product, lang, routes, 'microbit'), '');
  out.push(`> [!IMPORTANT]\n> ${T(lang, '擴充 v2 尚未上機驗證；積木名稱以發布版為準：', 'Extension v2 is not yet verified on hardware; block names follow the published release:')} <${mb.EXT_URL}>`, '');

  out.push(`## ${T(lang, '接線', 'Wiring')}`, '');
  out.push(figureFile(ctx, `images/figures/${key}/wiring-microbit.svg`, `${name} × micro:bit`));
  out.push('', table([T(lang, '感應器', 'Sensor'), 'micro:bit'], [['SDA', 'P20'], ['SCL', 'P19'], ['5V', T(lang, '外接 5 V（或 V2 的 3V 腳，需確認電流）', 'external 5 V (or the V2 3V pin, check current)')], ['GND', 'GND']]));

  out.push('', `## ${T(lang, '讀值積木', 'Reporter blocks')}`, '');
  out.push(table([T(lang, '積木', 'Block'), T(lang, '傳回', 'Returns'), T(lang, '編號', 'Id'), T(lang, '說明', 'Meaning')],
    product.values.filter((v) => v.microbit && v.microbit.block).map((v) => [mb.blockText(product, v, lang), `${v.range}${v.unit ? ' ' + v.unit : ''}`, `[\`${valueIdRange(v)}\`](${ref}#v${v.id.toLowerCase()})`, L(v.desc, lang)])));
  const cmds = (product.commands || []).filter((c) => c.microbit && c.microbit.block);
  if (cmds.length) {
    out.push('', `## ${T(lang, '命令積木', 'Command blocks')}`, '');
    out.push(table([T(lang, '積木', 'Block'), T(lang, '編號', 'Id'), T(lang, '說明', 'Meaning')],
      cmds.map((c) => [mb.commandText(product, c, lang), `[\`${c.id}\`](${ref}#c${c.id.toLowerCase()})`, L(c.desc, lang)])));
  }
  out.push('', `## ${T(lang, '狀態', 'Status')}`, '', flagsTable(product, lang));

  out.push('', `## ${T(lang, 'JavaScript 寫法', 'JavaScript')}`, '');
  const first = product.values.find((v) => v.i2c && !v.count);
  out.push(fence(`basic.forever(function () {\n    let ${first ? first.key : 'v'} = MBC.readValue(${product.addr}, ${hex(first ? first.id : '01')})   // ${first ? valueName(first, lang) : ''}\n    basic.showNumber(${first ? first.key : 'v'})\n    basic.pause(100)\n})`, 'typescript'));
  out.push('', assumptions(product, lang, [T(lang, '擴充內部用 `Int16LE` 讀、寫完編號 `basic.pause(2)`（D5）。', 'The extension reads `Int16LE` and pauses 2 ms after writing the id (D5).')]));
  return out.join('\n') + '\n';
}
