<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-reference.mjs 從 _build/protocol/line8.json 產生（matrix），請勿手改；改內容請改 JSON 後重跑 node _build/gen-reference.mjs -->
# Line Follower 8-ch × MATRIX Mini R4

[🔙 Back to Line Follower 8-ch](/en/sensors/line8/index.md)

The MATRIX edition of the Line Follower 8-ch plugs into a Mini R4 I2C port at address `0x22` and can be read two ways: **A. stock colour-sensor blocks** (no library change, R~K = seven 0~255 numbers) or **B. the LauMo big channel** (swap in the big-channel library, one 30-byte packet with everything). They need different library files and cannot be mixed.

<p class="chips"><a class="chip chip-spike" href="/en/sensors/line8/spike-hub.md">SPIKE</a><a class="chip chip-ev3" href="/en/sensors/line8/ev3-hub.md">EV3</a><a class="chip chip-arduino" href="/en/sensors/line8/arduino-i2c.md">Arduino I2C</a><a class="chip chip-mblock" href="/en/sensors/line8/mblock.md">mBlock</a><a class="chip chip-matrix current" href="/en/sensors/line8/matrix.md">MATRIX</a><a class="chip chip-microbit" href="/en/sensors/line8/microbit.md">micro:bit</a></p>

## Wiring

<figure class="ref-fig"><img src="/images/figures/line8/wiring-matrix.svg" alt="Line Follower 8-ch × MATRIX Mini R4" loading="lazy"></figure>

Use the stock MATRIX 4-pin sensor cable on I2C1 or I2C2 and pick that port in the block.

## A. Stock blocks (0x22)

The "colour sensor → R / G / B / C / M / Y / K" blocks each return one 0~255 number. ⚠️ Do not use the stock H / S / V or colour-ID blocks (the host computes them from R, G, B; meaningless here); the grayscale and colour-number blocks were removed by MATRIX.

| Block | Content | Decode |
| :--- | :--- | :--- |
| R | [line position](/en/sensors/line8/reference.md#v01) + 8 | subtract 8 |
| B | [fine position](/en/sensors/line8/reference.md#v02) + 100 | subtract 100 |
| G | [line width](/en/sensors/line8/reference.md#v03) |  |
| C | [channel 1~8 reflect](/en/sensors/line8/reference.md#v10)  1~2  | C~K 兩路一組 convert9×10+convert9 |
| M | [channel 1~8 reflect](/en/sensors/line8/reference.md#v10)  3~4  | C~K 兩路一組 convert9×10+convert9 |
| Y | [channel 1~8 reflect](/en/sensors/line8/reference.md#v10)  5~6  | C~K 兩路一組 convert9×10+convert9 |
| K | [channel 1~8 reflect](/en/sensors/line8/reference.md#v10)  7~8  | C~K 兩路一組 convert9×10+convert9 |

```cpp
// MATRIXblock → Arduino：stock library
int r = MiniR4.I2C1.MXColor.getColor(R);   // line position + 8
```

## B. Big channel (LauMo library)

After swapping in the `MiniR4ColorSensorExt` big-channel library ([download](/en/downloads.md)) the same blocks become pointer reads: ColorID resets the pointer and returns the status byte, Blue reads 1 byte, Green 1 signed byte, Saturation 2 bytes, Hue 2 signed bytes, Value a 4-byte float, Red advances the pointer by the size of the last read.

<figure class="ref-fig"><img src="/images/figures/common/packet-big30.svg" alt="LauMo 30-byte packet" loading="lazy"></figure>

| byte | Block | Content | Note |
| :--- | :--- | :--- | :--- |
| 0 | Blue | [line position](/en/sensors/line8/reference.md#v01) + 8 | subtract 8 |
| 2 | Blue | [fine position](/en/sensors/line8/reference.md#v02) + 100 | subtract 100 |
| 1 | Blue | [line width](/en/sensors/line8/reference.md#v03) |  |
| 12 | Blue | [line segments](/en/sensors/line8/reference.md#v04) |  |
| 11 | Blue | [line bitmap](/en/sensors/line8/reference.md#v05) |  |
| 25 | Blue | [calibrations done](/en/sensors/line8/reference.md#v08) | 尾段 F1/F2 完成次數 |
| 3~10 | Blue | [channel 1~8 reflect](/en/sensors/line8/reference.md#v10)（1 byte each） |  |
| 24 | ColorID | [F3 status flags](/en/sensors/line8/reference.md#flags) |  |
| 25 | Cyan | F1 completion count | calibrate black |
| 26 | Magenta | F2 completion count | calibrate white |
| 27 | Yellow | device ID | 0x16 |
| 28 | Key | hw[0] | 8 |
| 29 | — | CRC-8 | poly 0x07 over bytes 0~28; checked by the library |

**Actions**：Cyan = calibrate black, Magenta = calibrate white

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
int linePos = bigU8(0) - 8;   // line position
```

## Sources and assumptions

Generated by `_build/gen-reference.mjs` from `_build/protocol/line8.json` (single source of truth, transcribed from the 2026-10-21 protocol decisions).

- B-11：細位置 02 與線位置 01 同號（負 = 左）；v1 韌體註解寫反，程式碼同號。
- B-12：E7 五秒動態校準補回（v1 的 0x20=0x01），定案表沒列。
- SPIKE 教育版模式 0 的 byte 8~15 補 0（#65 已改，使用者保留）。
- EV3 模式 4 與模式 2 相同內容；模式 3 改壓線位元圖（#67）。
- mBlock 舊版：MBC_MBLOCK_LEGACY=1 時逐 byte 同 2026-10-21 統一後韌體，0x0B 回 1。
- Library rules (command 1 returns 0x43, CRC-8, 5 ms minimum interval) follow the LauMo big-channel library source.
