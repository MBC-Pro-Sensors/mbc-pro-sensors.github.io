// 標記式片段（<!-- gen:name -->…<!-- /gen -->）與全站索引：i2c-protocol、makeblock、guide#start、llms、reference.md、llms-protocol.txt
import { PLATFORMS, L, T, hex, table, pageHref, siteKey, pageFile, valueName, valueIdRange } from '../../gen-lib.mjs';
import { AI_NOTE } from './common.mjs';
import { HIDE_BIG } from '../../site-flags.mjs';
import * as mb from '../snippets/mblock.mjs';
import { HOST_BY_ID, swMd } from '../../gen-guides.mjs';

// ---- i2c-protocol.md
export function i2cAddrTable(products, lang) {
  return table([T(lang, '產品', 'Product'), T(lang, 'I2C 位址', 'I2C address'), 'hw[0]', T(lang, '型號', 'Model'), T(lang, '值表', 'Value table'), T(lang, '指令參考', 'Reference'), T(lang, '新手範例', 'Examples')],
    products.filter((p) => p.hosts && p.hosts.i2c).map((p) => {
      const k = siteKey(p);
      return [L(p.name, lang), `\`${p.addr}\``, `\`${p.hw0}\``, `\`${p.model}\``, `[I2C](${pageHref(lang, pageFile(p, 'arduino-i2c'))})`, `[${T(lang, '參考', 'Reference')}](${pageHref(lang, pageFile(p, 'reference'))})`, `[Arduino](${pageHref(lang, `/sensors/${k}/arduino-examples.md`)})`];
    }));
}
export function i2cCommon(common, lang) {
  const rows = [
    ...common.values.map((v) => [`<span id="r${v.id.toLowerCase()}">\`${v.id}\`</span>`, 'R', L(v.name, lang), `${L(v.desc, lang)}${v.range ? `（${v.range}）` : ''}`]),
    ...common.commands.map((c) => [`<span id="r${c.id.toLowerCase()}">\`${c.id}\`</span>`, 'W', L(c.name, lang), `${L(c.desc, lang)}；${T(lang, '參數', 'params')}：${(c.params || []).map((p) => `${L(p.name, lang)} ${p.range || ''}`.trim()).join(T(lang, '、', ', ')) || '—'}`]),
  ];
  return table([T(lang, '編號', 'Id'), T(lang, '讀寫', 'R/W'), T(lang, '名稱', 'Name'), T(lang, '說明', 'Meaning')], rows, [':---', ':---:', ':---', ':---']);
}

// ---- makeblock.md
export function mblockBlocks(products, lang) {
  const rows = [];
  for (const p of products.filter((p) => p.hosts && p.hosts.mblock)) {
    const k = siteKey(p);
    for (const v of p.values.filter((v) => v.mblock && v.mblock.block)) rows.push([mb.blockText(p, v, lang), `${v.range}${v.unit ? ' ' + v.unit : ''}`, `[${L(p.name, lang)}](${pageHref(lang, pageFile(p, 'mblock'))})`, L(v.desc, lang)]);
    for (const c of (p.commands || []).filter((c) => c.mblock)) rows.push([mb.commandText(p, c, lang), '—', `[${L(p.name, lang)}](${pageHref(lang, pageFile(p, 'mblock'))})`, L(c.desc, lang)]);
  }
  return table([T(lang, '積木', 'Block'), T(lang, '傳回', 'Returns'), T(lang, '產品', 'Product'), T(lang, '說明', 'Meaning')], rows);
}
export function mblockProducts(products, lang) {
  return table([T(lang, '產品', 'Product'), T(lang, '積木分類', 'Category'), T(lang, '積木', 'Blocks'), T(lang, '舊版擴展', 'Legacy ext.')],
    products.filter((p) => p.hosts && p.hosts.mblock).map((p) => [`[${L(p.name, lang)}](${pageHref(lang, pageFile(p, 'mblock'))})`, mb.category(p, lang),
      p.values.filter((v) => v.mblock && v.mblock.block).map((v) => L(v.mblock.block, lang).replace('[N]', 'N')).join(T(lang, '、', ', ')),
      p.hosts.mblock.legacy ? T(lang, 'v1.0.0 可用（2026-10 前韌體）', 'v1.0.0 works (pre-2026-10 firmware)') : '—']));
}

// ---- guide.md#start：每格連到第 3 層軟體頁（平台只有一種軟體）或列出該平台的每個軟體（2026-10-22 三層導覽）
export function guideStart(siteProducts, lang, avail) {
  const rows = Object.entries(siteProducts).map(([k, sp]) => {
    const a = (avail && avail[k]) || { hosts: [] };
    return [`[${sp.name ? L(sp.name, lang) : k}](${pageHref(lang, `/sensors/${k}/index.md`)})`, ...PLATFORMS.map((p) => {
      const ah = a.hosts.find((x) => x.id === p.id);
      if (!ah) return '—';
      const H = HOST_BY_ID[p.id];
      return ah.sws.map((sw) => `[${L(H.software.find((s) => s.id === sw).name, lang)}](${swMd(lang, k, p.id, sw)})`).join('・');
    })];
  });
  return table([T(lang, '產品', 'Product'), ...PLATFORMS.map((p) => p.name[lang])], rows, [':---', ...PLATFORMS.map(() => ':---:')]);
}

// ---- llms.txt 支援平台節
export function llmsPlatforms(lang) {
  const S = 'https://mbc-pro-sensors.github.io';
  return [
    `- [${T(lang, '產品 × 平台指令參考索引', 'Product × host value reference index')}](${S}/reference.html): ${T(lang, '每款產品每個值在六個平台（SPIKE、EV3、Arduino I2C、mBlock、MATRIX、micro:bit）各怎麼讀。', 'how every value of every product is read on the six hosts (SPIKE, EV3, Arduino I2C, mBlock, MATRIX, micro:bit).')}`,
    `- [${T(lang, 'MBC 通用 I2C 協議 v2', 'MBC universal I2C protocol v2')}](${S}/i2c-protocol.html): ${T(lang, '寫編號讀 int16 小端；共用 F0~F5 版本／狀態、F8 換位址、EF 重開機。', 'write an id, read int16 little-endian; shared F0~F5 version / status, F8 re-address, EF reboot.')}`,
    `- [${T(lang, 'mBlock 5 擴展 v2', 'mBlock 5 extension v2')}](${S}/makeblock.html): ${T(lang, 'UART 38400，3 byte 回應帶檢查碼，整包 FD。', 'UART 38400, 3-byte replies with checksum, FD pack-all.')}`,
    `- [MATRIX Mini R4](${S}/matrix.html): ${(HIDE_BIG ? T(lang, '原廠 0x22 R~K 對照與神攝手 mVision。', 'stock 0x22 R~K mapping and the Sharpshooter mVision format.') : T(lang, '原廠 0x22 R~K 對照與 LauMo 大通道 30 byte。', 'stock 0x22 R~K mapping and the LauMo 30-byte big channel.'))}`,
    `- [micro:bit MakeCode](${S}/microbit.html): ${T(lang, 'pxt-mbc-sensors 擴充 v2。', 'pxt-mbc-sensors extension v2.')}`,
    `- [${T(lang, '協定純文字版', 'Protocol plain text')}](${S}/llms-protocol.txt): ${T(lang, '每款產品的值表、命令、狀態旗標。', 'value tables, commands and status flags of every product.')}`,
  ].join('\n');
}

// ---- 全站索引 reference.md
export function referenceIndex(products, siteProducts, ctx, lang) {
  const byKey = Object.fromEntries(products.map((p) => [siteKey(p), p]));
  const out = [AI_NOTE, '<!-- 本頁由 _build/gen-reference.mjs 產生，請勿手改 -->'];
  out.push(`# ${T(lang, '指令參考索引：產品 × 平台', 'Value reference index: product × host')}`, '');
  out.push(T(lang, '每款產品一頁「指令參考」，列出每個值在六個平台各怎麼讀；表格裡每一格連到那個平台的說明頁。', 'Each product has one reference page listing how every value is read on the six hosts; every cell links to that host\'s page.'), '');
  const rows = Object.entries(siteProducts).map(([k, sp]) => {
    const p = byKey[k];
    const r = sp.routes || {};
    const name = sp.name ? L(sp.name, lang) : k;
    const refMd = `/sensors/${k}/reference.md`;
    const first = p && ctx.hasFile(refMd.slice(1)) ? `[${name}](${pageHref(lang, refMd)})` : `[${name}](${pageHref(lang, `/sensors/${k}/index.md`)})`;
    // 平台格：優先 ref，沒有就 start；目標頁還沒產生就留 —（check-links 才不會有斷鏈）
    const cell = (pl) => {
      const rt = r[pl.id];
      if (!rt) return '—';
      const t = [rt.ref, rt.start].find((x) => x && (/^https?:/.test(x) || ctx.hasFile(x.replace(/^\//, ''))));
      return t ? `[${pl.name[lang]}](${pageHref(lang, t)})` : '—';
    };
    return [first, p ? `\`${p.addr}\`` : '—', ...PLATFORMS.map(cell)];
  });
  out.push(table([T(lang, '產品', 'Product'), 'I2C', ...PLATFORMS.map((p) => p.name[lang])], rows, [':---', ':---:', ...PLATFORMS.map(() => ':---:')]));
  out.push('', `## ${T(lang, '共用規則', 'Shared rules')}`, '');
  out.push(`- [${T(lang, '通用 I2C 協議 v2', 'Universal I2C protocol v2')}](${pageHref(lang, '/i2c-protocol.md')})`);
  out.push(`- [${T(lang, 'mBlock 擴展', 'mBlock extension')}](${pageHref(lang, '/makeblock.md')})`);
  out.push(`- [${T(lang, '協定純文字版（給 AI）', 'Plain-text protocol (for AI)')}](/llms-protocol.txt)`);
  out.push('', `## ${T(lang, '已產生指令參考的產品', 'Products with a generated reference')}`, '');
  for (const p of products.filter((p) => ctx.hasFile(pageFile(p, 'reference').slice(1)))) out.push(`- [${L(p.name, lang)}](${pageHref(lang, pageFile(p, 'reference'))})：${p.values.length} ${T(lang, '個值', 'values')}、${(p.commands || []).length} ${T(lang, '個命令', 'commands')}`);
  return out.join('\n') + '\n';
}

// ---- llms-protocol.txt
export function llmsProtocol(products, common) {
  const out = ['# MBC-Pro Sensors protocol v2 (plain text for AI assistants) — generated by _build/gen-reference.mjs', ''];
  out.push('Shared (all products): I2C write [id] then read 2*k bytes = k int16 little-endian; undefined id = -32768; snapshot every 10 ms.');
  out.push('mBlock: UART 38400; send [id] -> [lo][hi][id^lo^hi]; commands E0~EE ack [00][00][id]; FD pack-all [AA][N][N x int16 LE][xor].');
  for (const v of common.values) out.push(`  ${v.id} ${v.key}: ${v.desc.zh} / ${v.desc.en}`);
  for (const c of common.commands) out.push(`  ${c.id} ${c.key}: ${c.desc.zh} / ${c.desc.en}`);
  for (const p of products) {
    out.push('', `## ${p.name.zh} / ${p.name.en} (id ${p.id}, I2C ${p.addr}, hw0 ${p.hw0}, model ${p.model})`);
    out.push(`hosts: ${Object.entries(p.hosts || {}).filter(([, v]) => v).map(([k]) => k).join(', ')}`);
    out.push('values:');
    for (const v of p.values) {
      const where = [];
      if (v.ev3 && v.ev3.length) where.push('EV3 ' + v.ev3.map((e) => `mode${e.mode}[${e.index}]${e.offset ? '-' + e.offset : ''}`).join(','));
      if (v.spike && v.spike.length) where.push('SPIKE ' + v.spike.map((e) => `mode${e.mode}${e.v != null ? ` v${e.v}${e.part || ''}` : e.byte != null ? ` b${e.byte}` : ''}${e.offset ? '-' + e.offset : ''}`).join(','));
      if (v.matrix && v.matrix.ch22) where.push(`MATRIX 0x22 cmd${v.matrix.ch22.cmd}${v.matrix.ch22.offset ? '-' + v.matrix.ch22.offset : ''}`);
      if (v.matrix && v.matrix.big) where.push(`MATRIX big byte${v.matrix.big.byte} ${v.matrix.big.type}${v.matrix.big.offset ? '-' + v.matrix.big.offset : ''}`);
      if (v.mvision) where.push(`mVision slot${v.mvision.idx}`);
      out.push(`  ${valueIdRange(v)} ${v.key}: ${valueName(v, 'zh')} / ${valueName(v, 'en')}; range ${v.range}${v.unit ? ' ' + v.unit : ''}; ${v.desc.zh} / ${v.desc.en}${where.length ? '; ' + where.join('; ') : ''}`);
    }
    if (p.commands && p.commands.length) {
      out.push('commands:');
      for (const c of p.commands) out.push(`  ${c.id} ${c.key}: ${c.desc.zh} / ${c.desc.en}${(c.params || []).length ? '; params ' + c.params.map((q) => `${q.name.en} ${q.range}`).join(', ') : ''}${c.busyFlag != null ? `; busy F3 bit${c.busyFlag}` : ''}`);
    }
    if (p.flags && p.flags.length) {
      out.push('status F3:');
      for (const f of p.flags) out.push(`  bit${f.bit} ${f.key}: ${f.desc.zh} / ${f.desc.en}`);
    }
    if (p.assumptions && p.assumptions.length) { out.push('assumptions:'); for (const a of p.assumptions) out.push(`  - ${a}`); }
  }
  return out.join('\n') + '\n';
}
