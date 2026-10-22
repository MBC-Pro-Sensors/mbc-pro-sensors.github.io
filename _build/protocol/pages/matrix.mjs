// sensors/<p>/matrix.md：A 原廠 0x22 對照、B 大通道 byte 表＋指標讀法；神攝手產 mVision 表
import { AI_NOTE, genNote, backLink, chips, figureFile, assumptions, pageFile, rootLink, fence, L, T, table, pageHref, siteKey, valueName, valueIdRange } from './common.mjs';
import * as mx from '../snippets/matrix.mjs';

export function render(product, common, ctx, lang) {
  const name = L(product.name, lang);
  const key = siteKey(product);
  const routes = ctx.routes(key);
  const ref = pageHref(lang, pageFile(product, 'reference'));
  const out = [AI_NOTE, genNote(product, 'matrix')];
  out.push(`# ${T(lang, `${name} × MATRIX Mini R4`, `${name} × MATRIX Mini R4`)}`, '', backLink(product, lang), '');

  if (product.hosts && product.hosts.mvision) {
    out.push(T(lang, `${name}在 MATRIX 走串口，用 MATRIXblock 內建的 **mVision** 攝影機積木讀（不走 I2C）。`, `On MATRIX the ${name} talks over serial and is read with the built-in **mVision** camera blocks (not I2C).`));
    out.push('', chips(product, lang, routes, 'matrix'), '');
    out.push(`## ${T(lang, 'mVision 資料格', 'mVision data slots')}`, '');
    out.push(figureFile(ctx, 'images/figures/common/packet-mvision.svg', 'mVision packet'), '');
    out.push(table([T(lang, '資料', 'Slot'), T(lang, '值', 'Value'), T(lang, '範圍', 'Range'), T(lang, '說明', 'Meaning')],
      product.values.filter((v) => v.mvision).map((v) => [`${v.mvision.idx}`, `[${valueName(v, lang)}](${ref}#v${v.id.toLowerCase()})`, `${v.range}${v.unit ? ' ' + v.unit : ''}`, (v.mvision.note ? v.mvision.note + '；' : '') + L(v.desc, lang)])));
    out.push('', T(lang, common.mvision.format, common.mvision.format));
    out.push('', assumptions(product, lang));
    return out.join('\n') + '\n';
  }

  const hasCh22 = product.hosts.matrix && product.hosts.matrix.ch22;
  const hasBig = product.hosts.matrix && product.hosts.matrix.big;
  out.push(T(lang,
    `${name} MATRIX 版接在 Mini R4 的 I2C 孔，位址 \`0x22\`，有兩種用法：**A. 原廠顏色感應器積木**（不用換函式庫，R~K 七個 0~255 的數）與 **B. LauMo 大通道**（換成大通道函式庫，一包 30 byte 什麼都有）。兩種要裝不同的函式庫檔，不能同時用。`,
    `The MATRIX edition of the ${name} plugs into a Mini R4 I2C port at address \`0x22\` and can be read two ways: **A. stock colour-sensor blocks** (no library change, R~K = seven 0~255 numbers) or **B. the LauMo big channel** (swap in the big-channel library, one 30-byte packet with everything). They need different library files and cannot be mixed.`));
  out.push('', chips(product, lang, routes, 'matrix'), '');
  out.push(`## ${T(lang, '接線', 'Wiring')}`, '');
  out.push(figureFile(ctx, `images/figures/${key}/wiring-matrix.svg`, `${name} × MATRIX Mini R4`));
  out.push('', T(lang, '用 MATRIX 原廠 4 pin 感應器線接 I2C1 或 I2C2，積木裡選對應的孔。', 'Use the stock MATRIX 4-pin sensor cable on I2C1 or I2C2 and pick that port in the block.'));

  if (hasCh22) {
    out.push('', `## ${T(lang, 'A. 原廠積木（0x22）', 'A. Stock blocks (0x22)')}`, '');
    out.push(T(lang, '積木「顏色感應器 → R／G／B／C／M／Y／K」各讀 1 個 0~255 的數。⚠️ 不要用原廠的 H／S／V、顏色辨識積木（那是主機拿 R、G、B 自己算的，對本產品沒有意義）；灰階、顏色編號積木已被 MATRIX 拿掉。', 'The "colour sensor → R / G / B / C / M / Y / K" blocks each return one 0~255 number. ⚠️ Do not use the stock H / S / V or colour-ID blocks (the host computes them from R, G, B; meaningless here); the grayscale and colour-number blocks were removed by MATRIX.'), '');
    const rows = [];
    for (const v of product.values) {
      const c = v.matrix && v.matrix.ch22;
      if (!c) continue;
      if (c.count) for (let i = 0; i < c.count; i++) rows.push([mx.chName(c.cmd + i), `[${valueName(v, lang)}](${ref}#v${v.id.toLowerCase()}) ${T(lang, '第', '')} ${i * 2 + 1}~${i * 2 + 2} ${T(lang, '路', '')}`, c.note || '']);
      else rows.push([mx.chName(c.cmd), `[${valueName(v, lang)}](${ref}#v${v.id.toLowerCase()})${c.offset ? ` + ${c.offset}` : ''}`, c.offset ? T(lang, `讀到的數減 ${c.offset}`, `subtract ${c.offset}`) : (c.note && c.note !== mx.chName(c.cmd) ? c.note : '')]);
    }
    out.push(table([T(lang, '積木', 'Block'), T(lang, '內容', 'Content'), T(lang, '解法', 'Decode')], rows));
    out.push('', fence(`// MATRIXblock → Arduino：${T(lang, '原廠函式庫', 'stock library')}\nint r = MiniR4.I2C1.MXColor.getColor(R);   // ${rows[0] ? rows[0][1].replace(/\[|\]\([^)]*\)/g, '') : ''}`, 'cpp'));
  }

  if (hasBig) {
    out.push('', `## ${T(lang, 'B. 大通道（LauMo 函式庫）', 'B. Big channel (LauMo library)')}`, '');
    out.push(T(lang, `函式庫 \`MiniR4ColorSensorExt大通道版\`（[下載](${rootLink(ctx, lang, '/downloads.md', 'https://github.com/contactmbcrobot')})）換掉原廠檔後，同一組積木變成「指標讀法」：ColorID 把指標歸零並回傳狀態、Blue 讀 1 byte、Green 讀 1 byte 有正負、Saturation 讀 2 byte、Hue 讀 2 byte 有正負、Value 讀 4 byte 小數、Red 把指標往後移「上一個讀取的大小」。`, `After swapping in the \`MiniR4ColorSensorExt\` big-channel library ([download](${rootLink(ctx, lang, '/downloads.md', 'https://github.com/contactmbcrobot')})) the same blocks become pointer reads: ColorID resets the pointer and returns the status byte, Blue reads 1 byte, Green 1 signed byte, Saturation 2 bytes, Hue 2 signed bytes, Value a 4-byte float, Red advances the pointer by the size of the last read.`), '');
    out.push(figureFile(ctx, 'images/figures/common/packet-big30.svg', 'LauMo 30-byte packet'), '');
    const rows = [];
    for (const v of product.values) {
      const b = v.matrix && v.matrix.big;
      if (!b) continue;
      const size = b.type.endsWith('16') ? 2 : 1;
      const blk = { u8: 'Blue', i8: 'Green', u16: 'Saturation', i16: 'Hue' }[b.type] || 'Blue';
      const n = b.count || 1;
      const span = n * size === 1 ? `${b.byte}` : `${b.byte}~${b.byte + n * size - 1}`;
      rows.push([span, blk, `[${valueName(v, lang)}](${ref}#v${v.id.toLowerCase()})${b.offset ? ` + ${b.offset}` : ''}${n > 1 ? `（${T(lang, `每路 ${size} byte`, `${size} byte each`)}）` : ''}`, b.note || (b.offset ? T(lang, `讀到的數減 ${b.offset}`, `subtract ${b.offset}`) : '')]);
    }
    const tail0 = product.id === 'line16' || product.id === 'imu';
    if (!tail0) {
      rows.push(['24', 'ColorID', `[F3 ${T(lang, '狀態旗標', 'status flags')}](${ref}#flags)`, '']);
      rows.push(['25', 'Cyan', T(lang, 'F1 動作完成次數', 'F1 completion count'), (product.commands || []).filter((c) => c.matrix && c.matrix.f === 'F1').map((c) => L(c.name, lang)).join('') || '0']);
      rows.push(['26', 'Magenta', T(lang, 'F2 動作完成次數', 'F2 completion count'), (product.commands || []).filter((c) => c.matrix && c.matrix.f === 'F2').map((c) => L(c.name, lang)).join('') || '0']);
      rows.push(['27', 'Yellow', T(lang, '裝置 ID', 'device ID'), product.addr]);
      rows.push(['28', 'Key', 'hw[0]', String(product.hw0)]);
    } else rows.push(['24~28', 'ColorID / C / M / Y / K', T(lang, '維持 0（已流出格式鎖住）', 'stay 0 (shipped layout locked)'), '']);
    rows.push(['29', '—', 'CRC-8', T(lang, '多項式 0x07，涵蓋 0~28；函式庫自己檢查', 'poly 0x07 over bytes 0~28; checked by the library')]);
    out.push(table(['byte', T(lang, '積木', 'Block'), T(lang, '內容', 'Content'), T(lang, '備註', 'Note')], rows));
    const fcmds = (product.commands || []).filter((c) => c.matrix && c.matrix.f);
    out.push('', `**${T(lang, '動作', 'Actions')}**：` + (fcmds.length ? fcmds.map((c) => `${{ F1: 'Cyan', F2: 'Magenta', F3: 'Yellow', F4: 'Key' }[c.matrix.f]} = ${L(c.name, lang)}`).join(T(lang, '、', ', ')) : T(lang, '無（Cyan／Magenta／Yellow／Key 只回傳 byte 25~28，不做事）', 'none (Cyan / Magenta / Yellow / Key only return bytes 25~28)')));
    out.push('', `### ${T(lang, 'Arduino 寫法（MATRIXblock 的 Arduino 模式）', 'Arduino code (MATRIXblock Arduino mode)')}`, '');
    out.push(fence(mx.bigHelpers(lang), 'cpp'));
    const first = product.values.find((v) => v.matrix && v.matrix.big);
    if (first) out.push('', fence(`int ${first.key} = bigU8(${first.matrix.big.byte})${first.matrix.big.offset ? ` - ${first.matrix.big.offset}` : ''};   // ${valueName(first, lang)}`, 'cpp'));
  }

  out.push('', assumptions(product, lang, [T(lang, '函式庫規則（命令 1 回 0x43、CRC-8、最快 5 ms 讀一次）以 `80_上位機研究\\MatrixMiniR4\\MiniR4ColorSensorExt大通道版` 為準。', 'Library rules (command 1 returns 0x43, CRC-8, 5 ms minimum interval) follow the LauMo big-channel library source.')]));
  return out.join('\n') + '\n';
}
