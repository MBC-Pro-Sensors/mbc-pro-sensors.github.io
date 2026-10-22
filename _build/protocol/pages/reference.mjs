// sensors/<p>/reference.md：每個值一節（示意圖＋資訊表＋六平台怎麼讀）、命令、F3 旗標、共用暫存器連結、假設
import { PLATFORMS } from '../../gen-lib.mjs';
import { AI_NOTE, genNote, backLink, chips, figure, howTo, valueInfo, examplesLine, assumptions, flagsTable, commonRef, hex, L, T, table, pageHref, siteKey, valueName, valueIdRange } from './common.mjs';

export function render(product, common, ctx, lang) {
  const name = L(product.name, lang);
  const key = siteKey(product);
  const routes = ctx.routes(key);
  const out = [];
  out.push(AI_NOTE, genNote(product, 'reference'));
  out.push(`# ${T(lang, `${name} 指令參考`, `${name}: value & command reference`)}`);
  out.push('', backLink(product, lang), '');
  out.push(T(lang,
    `這一頁列出${name}送給每個上位機的**每一個值、每一個命令**：它是什麼意思、範圍多少、在 SPIKE／EV3／Arduino I2C／mBlock／MATRIX／micro:bit 各要怎麼讀。網址加 \`?host=spike\`（或 ev3、arduino、mblock、matrix、microbit）會直接展開那個平台。`,
    `Every value and command the ${name} reports to each host: what it means, its range, and how to read it on SPIKE / EV3 / Arduino I2C / mBlock / MATRIX / micro:bit. Append \`?host=spike\` (or ev3, arduino, mblock, matrix, microbit) to the URL to expand that host by default.`));
  out.push('', chips(product, lang, routes, 'reference'), '');

  // 基本資訊
  out.push(table([T(lang, '項目', 'Item'), T(lang, '內容', 'Detail')], [
    [T(lang, 'I2C 出廠位址', 'Factory I2C address'), `\`${product.addr}\``],
    ['hw[0]', `\`${product.hw0}\`（${T(lang, 'F0 高 byte、SPIKE 模式 8／EV3 模式 5 ID 高 byte、大通道 byte 28', 'F0 high byte; ID high byte in SPIKE mode 8 / EV3 mode 5; big-channel byte 28')}）`],
    [T(lang, '型號', 'Model'), `\`${product.model}\``],
    [T(lang, '協議版本', 'Protocol version'), `\`${common.protoVer}\``],
    [T(lang, '支援平台', 'Hosts'), PLATFORMS.filter((p) => product.hosts && product.hosts[p.hostKey]).map((p) => p.name[lang]).join(T(lang, '、', ', '))],
  ]));

  // 總覽
  out.push('', `## ${T(lang, '值一覽', 'Values at a glance')}`, '');
  const platHead = PLATFORMS.filter((p) => product.hosts && product.hosts[p.hostKey]);
  out.push(table(
    [T(lang, '編號', 'Id'), T(lang, '值', 'Value'), T(lang, '範圍', 'Range'), ...platHead.map((p) => p.name[lang])],
    product.values.map((v) => [`[\`${valueIdRange(v)}\`](#v${v.id.toLowerCase()})`, `[${valueName(v, lang)}](#v${v.id.toLowerCase()})`, `${v.range}${v.unit ? ' ' + v.unit : ''}`, ...platHead.map((p) => mark(product, v, p))]),
    [':---', ':---', ':---', ...platHead.map(() => ':---:')]));
  out.push('', T(lang, '✓ = 這個平台有這個值；— = 沒有（表格裡的平台欄位只列產品支援的平台）。', '✓ = available on that host; — = not sent.'));

  // 每個值
  out.push('', `## ${T(lang, '每個值怎麼讀', 'Reading each value')}`);
  for (const v of product.values) {
    out.push('', `<section id="v${v.id.toLowerCase()}" class="ref-item">`, '');
    out.push(`### \`${valueIdRange(v)}\` ${valueName(v, lang)} <small>${v.key}</small>`, '');
    const fig = figure(ctx, product, v, lang);
    if (fig) out.push(fig, '');
    out.push(valueInfo(product, v, lang), '');
    out.push(`**${T(lang, '怎麼讀', 'How to read it')}**`, '');
    out.push(howTo(product, v, lang, 'value'));
    const ex = examplesLine(ctx, product, v, lang);
    if (ex) out.push('', ex);
    out.push('', '</section>');
  }

  // 命令
  if (product.commands && product.commands.length) {
    out.push('', `## ${T(lang, '寫入命令', 'Write commands')}`, '');
    out.push(T(lang, '只有 Arduino I2C、mBlock、micro:bit（與 MATRIX 大通道的 F1／F2）能送命令；SPIKE／EV3 接頭是唯讀的。', 'Only Arduino I2C, mBlock, micro:bit (and MATRIX big-channel F1 / F2) can send commands; the SPIKE / EV3 ports are read-only.'));
    for (const c of product.commands) {
      out.push('', `<section id="c${c.id.toLowerCase()}" class="ref-item ref-cmd">`, '');
      out.push(`### \`${c.id}\` ${L(c.name, lang)} <small>${c.key}</small>`, '');
      const params = (c.params || []).length ? c.params.map((p) => `${L(p.name, lang)} \`${p.range || ''}\`（${p.bytes || 1} byte）`).join(T(lang, '、', ', ')) : T(lang, '無', 'none');
      out.push(table([T(lang, '項目', 'Item'), T(lang, '內容', 'Detail')], [
        [T(lang, '作用', 'Action'), L(c.desc, lang)],
        [T(lang, '參數', 'Parameters'), params],
        [T(lang, '忙碌旗標', 'Busy flag'), c.busyFlag != null ? `F3 bit${c.busyFlag}（${T(lang, '執行中 = 1', '1 while running')}）` : T(lang, '無（立即生效）', 'none (immediate)')],
      ]), '');
      out.push(`**${T(lang, '怎麼送', 'How to send it')}**`, '');
      out.push(howTo(product, c, lang, 'command'));
      out.push('', '</section>');
    }
  }

  // 狀態旗標
  out.push('', `<section id="flags" class="ref-item">`, '', `## ${T(lang, '狀態旗標 F3', 'Status flags F3')}`, '');
  out.push(T(lang, '`F3` 每個 bit 一個狀態；I2C、mBlock、micro:bit 直接讀，MATRIX 大通道是 byte 24。', 'One status per bit of `F3`; read directly over I2C, mBlock, micro:bit; MATRIX big-channel byte 24.'), '');
  out.push(flagsTable(product, lang), '');
  out.push(howTo(product, null, lang, 'status'));
  out.push('', '</section>');

  // 共用暫存器
  out.push('', `## ${T(lang, '共用暫存器與命令', 'Shared registers and commands')}`, '');
  const proto = pageHref(lang, '/i2c-protocol.md');
  out.push(T(lang, `所有產品相同，只在 [通用 I2C 協議](${proto}) 寫一次：`, `Identical on every product, documented once in the [universal I2C protocol](${proto}):`), '');
  out.push(table([T(lang, '編號', 'Id'), T(lang, '名稱', 'Name'), T(lang, '說明', 'Meaning')],
    [...common.values, ...common.commands].map((cv) => [`[\`${cv.id}\`](${commonRef(ctx, lang, cv.id)})`, L(cv.name, lang), L(cv.desc, lang)])));

  out.push('', assumptions(product, lang));
  return out.join('\n') + '\n';
}

function mark(product, v, p) {
  const on = {
    spike: v.spike && v.spike.length, ev3: v.ev3 && v.ev3.length, arduino: v.i2c, mblock: v.mblock && v.mblock.block,
    matrix: v.matrix && (v.matrix.ch22 || v.matrix.big), microbit: v.microbit && v.microbit.block,
  }[p.id];
  return on ? '✓' : '—';
}
