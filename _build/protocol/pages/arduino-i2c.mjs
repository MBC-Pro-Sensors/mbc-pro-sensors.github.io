// sensors/<p>/arduino-i2c.md：I2C v2 值表＋命令表＋接線＋單檔小幫手
import { AI_NOTE, genNote, backLink, chips, figureFile, assumptions, pageFile, commonRef, flagsTable, fence, hex, L, T, table, pageHref, siteKey, valueName, valueIdRange } from './common.mjs';
import * as i2c from '../snippets/i2c.mjs';

export function render(product, common, ctx, lang) {
  const name = L(product.name, lang);
  const key = siteKey(product);
  const routes = ctx.routes(key);
  const ref = pageHref(lang, pageFile(product, 'reference'));
  const proto = pageHref(lang, '/i2c-protocol.md');
  const out = [AI_NOTE, genNote(product, 'arduino-i2c')];
  out.push(`# ${T(lang, `${name} × Arduino I2C（通用 I2C 版）`, `${name} × Arduino I2C (universal I2C edition)`)}`, '', backLink(product, lang), '');
  out.push(T(lang,
    `${name}通用 I2C 版用 **MBC I2C 協議 v2**：寫一個編號、讀 2 byte 就是一個值（int16 小端），連號的值可以一次讀完。本頁是這款產品的值表、命令表與接線；共用規則（F0~F5 版本與狀態、F8 換位址、EF 重開機）在 [通用 I2C 協議](${proto})。`,
    `The universal I2C edition of the ${name} speaks **MBC I2C protocol v2**: write one id, read 2 bytes = one int16 little-endian value; consecutive ids can be read in one go. This page has the value table, commands and wiring for this product; the shared rules (F0~F5 version & status, F8 re-address, EF reboot) are on the [universal I2C protocol](${proto}) page.`));
  out.push('', chips(product, lang, routes, 'arduino'), '');
  out.push(`> [!TIP]\n> ${T(lang, `**不太會寫程式？** 直接看 [Arduino 範例](${pageHref(lang, `/sensors/${key}/arduino-examples.md`)})，每支都能複製上傳。每個值的六平台讀法在 [指令參考](${ref})。`, `**New to coding?** Start with the [Arduino examples](${pageHref(lang, `/sensors/${key}/arduino-examples.md`)}); every sketch is copy-and-upload. The six-host reading of every value is in the [reference](${ref}).`)}`);

  // 基本資訊
  out.push('', `## ${T(lang, '基本資訊', 'Basics')}`, '');
  out.push(table([T(lang, '項目', 'Item'), T(lang, '內容', 'Detail')], [
    [T(lang, 'I2C 位址', 'I2C address'), `\`${product.addr}\`（${T(lang, '可用 F8 換', 'change with F8')}）`],
    [`F0 ${T(lang, '裝置 ID', 'device ID')}`, `${T(lang, '低 byte', 'low byte')} \`${product.addr}\`、${T(lang, '高 byte', 'high byte')} \`${product.hw0}\``],
    [T(lang, '型號', 'Model'), `\`${product.model}\``],
    [T(lang, '協議版本 F1', 'Protocol F1'), `\`${common.protoVer}\``],
    [T(lang, '速度', 'Speed'), T(lang, '100 kHz（建議）／400 kHz；寫完編號等 2 ms 再讀', '100 kHz (recommended) / 400 kHz; wait 2 ms after writing the id')],
    [T(lang, '讀法', 'Read'), common.i2c.read],
    [T(lang, '快照', 'Snapshot'), T(lang, `每 ${common.i2c.snapshotMs} ms 更新，一次讀多個值保證同一瞬間`, `updated every ${common.i2c.snapshotMs} ms; a multi-value read is atomic`)],
  ]));

  // 接線
  out.push('', `## ${T(lang, '接線', 'Wiring')}`, '');
  out.push(figureFile(ctx, `images/figures/${key}/wiring-uno.svg`, `${name} × Arduino UNO`));
  out.push('', table([T(lang, '感應器', 'Sensor'), 'Arduino UNO / Nano', 'Arduino R4', 'ESP32'], [
    ['SDA', 'A4', 'SDA', 'GPIO 21'], ['SCL', 'A5', 'SCL', 'GPIO 22'], ['5V', '5V', '5V', '5V'], ['GND', 'GND', 'GND', 'GND'],
  ]));
  out.push('', T(lang, '5 V 主機可直接接，不需準位轉換；同一組 SDA／SCL 上位址相同的產品不能同時接（用 F8 先換位址）。', 'Works directly with 5 V hosts, no level shifter; two products with the same address cannot share a bus (re-address one with F8 first).'));

  // 值表
  out.push('', `## ${T(lang, '值表', 'Value table')}`, '');
  out.push(T(lang, `每個編號讀回一個 int16；點編號看六平台讀法與示意圖。`, 'Each id returns one int16; click an id for the six-host reading and figure.'), '');
  out.push(table([T(lang, '編號', 'Id'), T(lang, '值', 'Value'), T(lang, '範圍', 'Range'), T(lang, '說明', 'Meaning')],
    product.values.filter((v) => v.i2c).map((v) => [`[\`${valueIdRange(v)}\`](${ref}#v${v.id.toLowerCase()})`, `${valueName(v, lang)} \`${v.key}\``, `${v.range}${v.unit ? ' ' + v.unit : ''}`, L(v.desc, lang)])));

  // 命令
  if (product.commands && product.commands.length) {
    out.push('', `## ${T(lang, '寫入命令', 'Write commands')}`, '');
    out.push(table([T(lang, '編號', 'Id'), T(lang, '命令', 'Command'), T(lang, '參數', 'Parameters'), T(lang, '說明', 'Meaning')],
      product.commands.filter((c) => c.i2c).map((c) => [`[\`${c.id}\`](${ref}#c${c.id.toLowerCase()})`, `${L(c.name, lang)} \`${c.key}\``, (c.params || []).map((p) => `${L(p.name, lang)} ${p.range || ''}`).join(T(lang, '、', ', ')) || '—', L(c.desc, lang) + (c.busyFlag != null ? `（${T(lang, `執行中 F3 bit${c.busyFlag} = 1`, `F3 bit${c.busyFlag} = 1 while running`)}）` : '')])));
  }

  // 狀態旗標
  out.push('', `## ${T(lang, '狀態旗標 F3', 'Status flags F3')}`, '', flagsTable(product, lang));

  // 共用
  out.push('', `## ${T(lang, '共用暫存器', 'Shared registers')}`, '');
  out.push(table([T(lang, '編號', 'Id'), T(lang, '名稱', 'Name'), T(lang, '說明', 'Meaning')],
    [...common.values, ...common.commands].map((cv) => [`[\`${cv.id}\`](${commonRef(ctx, lang, cv.id)})`, L(cv.name, lang), L(cv.desc, lang)])));

  // 小幫手
  out.push('', `## ${T(lang, '單檔小幫手函式', 'Single-file helper functions')}`, '');
  out.push(T(lang, '每支範例底部都貼這一段，不需要安裝函式庫：', 'Paste this at the bottom of every sketch; no library to install:'), '');
  out.push(fence(i2c.helpers(product, lang), 'cpp'));

  // 範例：整張值表
  out.push('', `## ${T(lang, '範例：整張值表逐行印', 'Example: print the whole value table')}`, '');
  const lines = product.values.filter((v) => v.i2c).map((v) => v.count
    ? `  { int16_t a[${v.count}]; mbcReadMany(ADDR, ${hex(v.id)}, ${v.count}, a); Serial.print("${v.key}: "); for (uint8_t i = 0; i < ${v.count}; i++) { Serial.print(a[i]); Serial.print(' '); } Serial.println(); }`
    : `  Serial.print("${v.key}: "); Serial.println(mbcRead(ADDR, ${hex(v.id)}));   // ${valueName(v, lang)}`);
  out.push(fence(`const uint8_t ADDR = ${product.addr};

void setup() {
  Serial.begin(115200);
  Wire.begin();
#if defined(WIRE_HAS_TIMEOUT)
  Wire.setWireTimeout(25000, true);   // ${T(lang, 'AVR：模組沒回應時不會卡死', 'AVR: never hang when the module is silent')}
#endif
  if (!mbcWaitReady(ADDR, 2000)) Serial.println("${T(lang, '找不到模組，請檢查接線與位址', 'module not found: check wiring and address')}");
  Serial.print("ID 0x"); Serial.println(mbcRead(ADDR, 0xF0), HEX);   // ${T(lang, '低 byte 位址、高 byte hw[0]', 'low byte address, high byte hw[0]')}
  Serial.print("FW ");    Serial.println(mbcRead(ADDR, 0xF2));
}

void loop() {
${lines.join('\n')}
  Serial.println();
  delay(200);
}

// ${T(lang, '下面貼上「單檔小幫手函式」', 'paste the helper functions below')}`, 'cpp'));

  out.push('', assumptions(product, lang, [T(lang, '接線表的腳位以各主機板常見預設為準（ESP32 可用 `Wire.begin(sda, scl)` 改）。', 'Pin table uses each board\'s common defaults (ESP32: change with `Wire.begin(sda, scl)`).')]));
  return out.join('\n') + '\n';
}
