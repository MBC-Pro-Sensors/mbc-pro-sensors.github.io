// sensors/<p>/mblock.md：mBlock 5 積木表＋接線＋封包
import { AI_NOTE, genNote, backLink, chips, figureFile, assumptions, pageFile, flagsTable, L, T, table, pageHref, siteKey, valueName, valueIdRange } from './common.mjs';
import * as mb from '../snippets/mblock.mjs';

export function render(product, common, ctx, lang) {
  const name = L(product.name, lang);
  const key = siteKey(product);
  const routes = ctx.routes(key);
  const ref = pageHref(lang, pageFile(product, 'reference'));
  const legacy = product.hosts && product.hosts.mblock && product.hosts.mblock.legacy;
  const out = [AI_NOTE, genNote(product, 'mblock')];
  out.push(`# ${T(lang, `${name} × mBlock 5（MakeBlock 版）`, `${name} × mBlock 5 (Makeblock edition)`)}`, '', backLink(product, lang), '');
  out.push(T(lang,
    `${name} MakeBlock 版接在 mBot、Ranger、Orion、MegaPi 的 RJ25 連接埠，用 **MBC Pro Sensors 擴展 v2** 的積木讀值。擴展安裝與各主板可用連接埠請看 [MakeBlock 積木擴展](${pageHref(lang, '/makeblock.md')})。`,
    `The Makeblock edition of the ${name} plugs into an RJ25 port on mBot, Ranger, Orion or MegaPi and is read with the **MBC Pro Sensors extension v2** blocks. Installation and usable ports per board: [Makeblock extension](${pageHref(lang, '/makeblock.md')}).`));
  out.push('', chips(product, lang, routes, 'mblock'), '');
  if (legacy) out.push(`> [!NOTE]\n> ${T(lang, '2026-10 以前出貨的 MakeBlock 版韌體用的是舊版擴展 v1.0.0（下載頁標「舊版」）；新舊積木不相容，請依感應器韌體版本選擇。', 'Units shipped before 2026-10 use the legacy extension v1.0.0 (marked "legacy" on the download page); old and new blocks are not interchangeable.')}`, '');

  // 接線
  out.push(`## ${T(lang, '接線', 'Wiring')}`, '');
  out.push(figureFile(ctx, `images/figures/${key}/wiring-mbot.svg`, `${name} × mBot`));
  out.push('', T(lang, '感應器 → mBot 插座片 → 主板 RJ25 連接埠（S1 = 主機送、S2 = 主機收）。UART 38400 8N1。', 'Sensor → mBot adapter → RJ25 port on the board (S1 = host TX, S2 = host RX). UART 38400 8N1.'));

  // 積木表
  out.push('', `## ${T(lang, '讀值積木', 'Reporter blocks')}`, '');
  out.push(table([T(lang, '積木', 'Block'), T(lang, '傳回', 'Returns'), T(lang, '編號', 'Id'), T(lang, '說明', 'Meaning')],
    product.values.filter((v) => v.mblock && v.mblock.block).map((v) => [mb.blockText(product, v, lang), `${v.range}${v.unit ? ' ' + v.unit : ''}`, `[\`${valueIdRange(v)}\`](${ref}#v${v.id.toLowerCase()})`, L(v.desc, lang)])));
  const cmds = (product.commands || []).filter((c) => c.mblock);
  if (cmds.length) {
    out.push('', `## ${T(lang, '命令積木', 'Command blocks')}`, '');
    out.push(table([T(lang, '積木', 'Block'), T(lang, '編號', 'Id'), T(lang, '說明', 'Meaning')],
      cmds.map((c) => [mb.commandText(product, c, lang), `[\`${c.id}\`](${ref}#c${c.id.toLowerCase()})`, L(c.desc, lang) + (c.busyFlag != null ? `（${T(lang, `執行中 F3 bit${c.busyFlag} = 1`, `F3 bit${c.busyFlag} = 1 while running`)}）` : '')])));
  }
  out.push('', `## ${T(lang, '狀態積木', 'Status blocks')}`, '');
  out.push(T(lang, '擴展把 `F3` 各 bit 拆成布林積木：', 'The extension exposes each `F3` bit as a boolean block:'), '', flagsTable(product, lang));

  // 封包
  out.push('', `## ${T(lang, '封包格式（給自己寫解碼器的人）', 'Packet format (for your own decoder)')}`, '');
  out.push(figureFile(ctx, 'images/figures/common/packet-mblock.svg', 'mBlock 3-byte packet'));
  out.push('', table([T(lang, '動作', 'Action'), T(lang, '格式', 'Format')], [
    [T(lang, '讀一個值', 'Read one value'), common.mblock.read],
    [T(lang, '寫入命令', 'Write command'), common.mblock.ack],
    [T(lang, '不認得的編號', 'Unknown id'), common.mblock.unknown],
    [T(lang, '整包 FD', 'Pack all (FD)'), mb.packAll(product, lang)],
    [T(lang, '共用編號', 'Shared ids'), common.values.filter((v) => v.mblock).map((v) => `\`${v.id}\` ${L(v.name, lang)}`).join(T(lang, '、', ', '))],
  ]));
  out.push('', figureFile(ctx, 'images/figures/common/packet-fd.svg', 'FD pack-all packet'));

  out.push('', assumptions(product, lang, [T(lang, '積木文字以擴展 v2.0.0 產生器 `build_mext.js` 的表為準；本頁由同一份 JSON 產生。', 'Block wording follows the v2.0.0 extension generator `build_mext.js`; this page is generated from the same JSON.')]));
  return out.join('\n') + '\n';
}
