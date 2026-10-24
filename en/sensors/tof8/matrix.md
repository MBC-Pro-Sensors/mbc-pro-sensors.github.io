<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-reference.mjs 從 _build/protocol/tof8.json 產生（matrix），請勿手改；改內容請改 JSON 後重跑 node _build/gen-reference.mjs -->
<!-- moved: /en/sensors/tof8/matrix/index.md -->
<p class="moved-note">This page has moved into the new guide: <a href="/en/sensors/tof8/matrix/index.md">Ranger 8-Way › MATRIX</a></p>

# Laser Ranger 8-ch × MATRIX Mini R4

[🔙 Back to Laser Ranger 8-ch](/en/sensors/tof8/index.md)

The MATRIX edition of the Laser Ranger 8-ch plugs into a Mini R4 I2C port at address `0x22` and can be read two ways: **A. stock colour-sensor blocks** (no library change, R~K = seven 0~255 numbers) or **B. the LauMo big channel** (swap in the big-channel library, one 30-byte packet with everything). They need different library files and cannot be mixed.

<p class="chips"><a class="chip chip-spike" href="/en/sensors/tof8/index.md">SPIKE</a><a class="chip chip-ev3" href="/en/sensors/tof8/index.md">EV3</a><a class="chip chip-arduino" href="/en/sensors/tof8/arduino-i2c.md">Arduino I2C</a><a class="chip chip-mblock" href="/en/sensors/tof8/mblock.md">mBlock</a><a class="chip chip-matrix current" href="/en/sensors/tof8/matrix.md">MATRIX</a><a class="chip chip-microbit" href="/en/sensors/tof8/microbit.md">micro:bit</a></p>

## Wiring

<figure class="ref-fig"><img src="/images/figures/tof8/wiring-matrix.svg" alt="Laser Ranger 8-ch × MATRIX Mini R4" loading="lazy"></figure>

Use the stock MATRIX 4-pin sensor cable on I2C1 or I2C2 and pick that port in the block.

## A. Stock blocks (0x22)

The "colour sensor → R / G / B / C / M / Y / K" blocks each return one 0~255 number. ⚠️ Do not use the stock H / S / V or colour-ID blocks (the host computes them from R, G, B; meaningless here); the grayscale and colour-number blocks were removed by MATRIX.

| Block | Content | Decode |
| :--- | :--- | :--- |
| R | [sensor 1~8 distance (cm)](/en/sensors/tof8/reference.md#v10)  1~2  | #67 改（使用者最終定案）：R、G、B = 第 1、2、3 顆；C = min(第 4, 第 5 顆)（正前方取較近）；M、Y、K = 第 6、7、8 顆；全部 cm 0~120；11、12 = 0 |
| G | [sensor 1~8 distance (cm)](/en/sensors/tof8/reference.md#v10)  3~4  | #67 改（使用者最終定案）：R、G、B = 第 1、2、3 顆；C = min(第 4, 第 5 顆)（正前方取較近）；M、Y、K = 第 6、7、8 顆；全部 cm 0~120；11、12 = 0 |
| B | [sensor 1~8 distance (cm)](/en/sensors/tof8/reference.md#v10)  5~6  | #67 改（使用者最終定案）：R、G、B = 第 1、2、3 顆；C = min(第 4, 第 5 顆)（正前方取較近）；M、Y、K = 第 6、7、8 顆；全部 cm 0~120；11、12 = 0 |
| C | [sensor 1~8 distance (cm)](/en/sensors/tof8/reference.md#v10)  7~8  | #67 改（使用者最終定案）：R、G、B = 第 1、2、3 顆；C = min(第 4, 第 5 顆)（正前方取較近）；M、Y、K = 第 6、7、8 顆；全部 cm 0~120；11、12 = 0 |
| M | [sensor 1~8 distance (cm)](/en/sensors/tof8/reference.md#v10)  9~10  | #67 改（使用者最終定案）：R、G、B = 第 1、2、3 顆；C = min(第 4, 第 5 顆)（正前方取較近）；M、Y、K = 第 6、7、8 顆；全部 cm 0~120；11、12 = 0 |
| Y | [sensor 1~8 distance (cm)](/en/sensors/tof8/reference.md#v10)  11~12  | #67 改（使用者最終定案）：R、G、B = 第 1、2、3 顆；C = min(第 4, 第 5 顆)（正前方取較近）；M、Y、K = 第 6、7、8 顆；全部 cm 0~120；11、12 = 0 |
| K | [sensor 1~8 distance (cm)](/en/sensors/tof8/reference.md#v10)  13~14  | #67 改（使用者最終定案）：R、G、B = 第 1、2、3 顆；C = min(第 4, 第 5 顆)（正前方取較近）；M、Y、K = 第 6、7、8 顆；全部 cm 0~120；11、12 = 0 |

```cpp
// MATRIXblock → Arduino：stock library
int r = MiniR4.I2C1.MXColor.getColor(R);   // sensor 1~8 distance (cm)  1~2
```

## B. Big channel (LauMo library)

After swapping in the `MiniR4ColorSensorExt` big-channel library ([download](/en/downloads.md)) the same blocks become pointer reads: ColorID resets the pointer and returns the status byte, Blue reads 1 byte, Green 1 signed byte, Saturation 2 bytes, Hue 2 signed bytes, Value a 4-byte float, Red advances the pointer by the size of the last read.

<figure class="ref-fig"><img src="/images/figures/common/packet-big30.svg" alt="LauMo 30-byte packet" loading="lazy"></figure>

| byte | Block | Content | Note |
| :--- | :--- | :--- | :--- |
| 9 | Blue | [nearest distance (cm)](/en/sensors/tof8/reference.md#v01) |  |
| 8 | Blue | [nearest direction](/en/sensors/tof8/reference.md#v02) + 8 | 原始方位 0~16 |
| 11 | Blue | [farthest distance (cm)](/en/sensors/tof8/reference.md#v03) |  |
| 10 | Blue | [farthest direction](/en/sensors/tof8/reference.md#v04) + 8 | 原始方位 1~15 |
| 14~15 | Saturation | [nearest distance (mm)](/en/sensors/tof8/reference.md#v06) | #67 追加（[14..15] 小端） |
| 12 | Blue | [init bitmap](/en/sensors/tof8/reference.md#v07) | #67 補填（註解有寫、原本沒填） |
| 13 | Blue | [fault bitmap](/en/sensors/tof8/reference.md#v08) | #67 補填 |
| 16~17 | Saturation | [front distance (mm)](/en/sensors/tof8/reference.md#v09) | #67 追加（MATRIX 定案 §4：[16..17] 正前方 mm 小端） |
| 0~7 | Blue | [sensor 1~8 distance (cm)](/en/sensors/tof8/reference.md#v10)（1 byte each） |  |
| 24 | ColorID | [F3 status flags](/en/sensors/tof8/reference.md#flags) |  |
| 25 | Cyan | F1 completion count | 0 |
| 26 | Magenta | F2 completion count | 0 |
| 27 | Yellow | device ID | 0x29 |
| 28 | Key | hw[0] | 8 |
| 29 | — | CRC-8 | poly 0x07 over bytes 0~28; checked by the library |

**Actions**：none (Cyan / Magenta / Yellow / Key only return bytes 25~28)

### Arduino code (MATRIXblock Arduino mode)

```cpp
// big channel: read byte n (uint8) / 2 bytes from n (uint16 little-endian)
uint8_t bigU8(uint8_t n) {
  MiniR4.I2C1.MXColor.getColorID();                 // pointer back to byte 0
  uint8_t v = MiniR4.I2C1.MXColor.getColor(B);      // read byte 0
  for (uint8_t i = 0; i < n; i++) { MiniR4.I2C1.MXColor.getColor(R); v = MiniR4.I2C1.MXColor.getColor(B); }   // Red = advance 1
  return v;
}
uint16_t bigU16(uint8_t n) {
  MiniR4.I2C1.MXColor.getColorID();
  MiniR4.I2C1.MXColor.getColor(B);
  for (uint8_t i = 0; i < n; i++) MiniR4.I2C1.MXColor.getColor(R);
  return (uint16_t)MiniR4.I2C1.MXColor.getS();      // Saturation = read 2 bytes
}
int16_t bigI16(uint8_t n) {
  MiniR4.I2C1.MXColor.getColorID();
  MiniR4.I2C1.MXColor.getColor(B);
  for (uint8_t i = 0; i < n; i++) MiniR4.I2C1.MXColor.getColor(R);
  return (int16_t)MiniR4.I2C1.MXColor.getH();       // Hue = read signed 2 bytes
}
```

```cpp
int nearCm = bigU8(9);   // nearest distance (cm)
```

## Sources and assumptions

Generated by `_build/gen-reference.mjs` from `_build/protocol/tof8.json` (single source of truth, transcribed from the 2026-10-21 protocol decisions).

- 正前方 mm（EV3 模式 3、SPIKE 模式 6 v[1]、大通道 [16..17]）在定案 §5 沒有獨立 I2C 編號 → 掛在 05 正前方 cm 底下，用 scale:mm 標示，不自己加編號。
- 0x22（使用者 2026-10-21 最終定案）：只用一個位址，R~K 由左到右 7 個 cm：第 1、2、3 顆、min(第 4, 第 5)、第 6、7、8 顆；11、12 不放資料；最近距離／方向由主機自己比較。此排法只用在 0x22，EV3／SPIKE／大通道不動。
- 大通道 [0..11] 不動（8 顆 cm、最近方向 0~16、最近 cm、最空曠方向、最空曠 cm）；補填 [12] 初始化、[13] 故障（原本註解有寫沒填）；追加 [14..15] 最近 mm、[16..17] 正前方 mm；尾段 [24] = F3、[27] = 0x29、[28] = 8（原本 [24] 沒填）。
- EV3：模式 0、1、2、4 鎖住（照盤點）；模式 3 改正前方 mm（原本 = 第 1 顆 0~100）、模式 5 改 [最近 mm, 最近方向 −8~+8]（原本 = 前 4 顆兩兩合併）。方向在 EV3／I2C／mBlock 一律 −8~+8 的 int8／int16，SPIKE 模式 1/3/4/9 與大通道維持原始 0~16（offset 8）。
- SPIKE：0/5/7/9 鎖住，1、2、3、4、8 維持不動；模式 6 改 [最近 mm, 正前方 mm, 最近方向 −8~+8]（原本 = 第 2~7 顆兩兩合併）；模式 8 v[0] 改 ID|hw[0]<<8。
- 最空曠方向 04 範圍 −7~+7（maxDirection 永遠不會是 0／16，盤點 INO:1086-1088）；最近方向 02 ±8 = 沒目標、沿用上次偏向側，開機預設 −8。
- 沒有寫入命令；F8 換位址／EF 重開機用共用的。
- 主對話 2026-10-21：補 09 正前方 mm（定案 §5 只有 05 正前方 cm，但 EV3 模式 3、SPIKE 模式 6、大通道 [16..17] 都送 mm，I2C／mBlock 應有同一個值）。
- Library rules (command 1 returns 0x43, CRC-8, 5 ms minimum interval) follow the LauMo big-channel library source.
