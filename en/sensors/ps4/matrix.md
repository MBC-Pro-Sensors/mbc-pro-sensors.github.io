<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-reference.mjs 從 _build/protocol/ps4.json 產生（matrix），請勿手改；改內容請改 JSON 後重跑 node _build/gen-reference.mjs -->
# Gamepad Receiver PS4/PS5 × MATRIX Mini R4

[🔙 Back to Gamepad Receiver PS4/PS5](/en/sensors/ps4/index.md)

The MATRIX edition of the Gamepad Receiver PS4/PS5 plugs into a Mini R4 I2C port at address `0x22` and can be read two ways: **A. stock colour-sensor blocks** (no library change, R~K = seven 0~255 numbers) or **B. the LauMo big channel** (swap in the big-channel library, one 30-byte packet with everything). They need different library files and cannot be mixed.

<p class="chips"><a class="chip chip-spike" href="/en/sensors/ps4/index.md">SPIKE</a><a class="chip chip-ev3" href="/en/sensors/ps4/index.md">EV3</a><a class="chip chip-arduino" href="/en/sensors/ps4/arduino-i2c.md">Arduino I2C</a><a class="chip chip-mblock" href="/en/sensors/ps4/mblock.md">mBlock</a><a class="chip chip-matrix current" href="/en/sensors/ps4/matrix.md">MATRIX</a><a class="chip chip-microbit" href="/en/sensors/ps4/microbit.md">micro:bit</a></p>

## Wiring

<figure class="ref-fig"><img src="/images/figures/ps4/wiring-matrix.svg" alt="Gamepad Receiver PS4/PS5 × MATRIX Mini R4" loading="lazy"></figure>

Use the stock MATRIX 4-pin sensor cable on I2C1 or I2C2 and pick that port in the block.

## A. Stock blocks (0x22)

The "colour sensor → R / G / B / C / M / Y / K" blocks each return one 0~255 number. ⚠️ Do not use the stock H / S / V or colour-ID blocks (the host computes them from R, G, B; meaningless here); the grayscale and colour-number blocks were removed by MATRIX.

| Block | Content | Decode |
| :--- | :--- | :--- |
| R | [D-pad](/en/sensors/ps4/reference.md#v02) | R = 方向鍵 \| 形狀鍵<<4（上 1 右 2 下 4 左 8、▲16 ●32 ✕64 ■128） |
| G | [shoulder buttons](/en/sensors/ps4/reference.md#v04) | G = 肩鍵 \| 功能鍵<<4（L2 1 R2 2 L1 4 R1 8、SELECT 16 L3 32 R3 64 START 128） |
| K | [connected](/en/sensors/ps4/reference.md#v06) | K = 1／0 |
| B | [stick axis 1~4](/en/sensors/ps4/reference.md#v10)  1~2  | B C M Y = LX LY RX RY，0~99，50 = 中間 |
| C | [stick axis 1~4](/en/sensors/ps4/reference.md#v10)  3~4  | B C M Y = LX LY RX RY，0~99，50 = 中間 |
| M | [stick axis 1~4](/en/sensors/ps4/reference.md#v10)  5~6  | B C M Y = LX LY RX RY，0~99，50 = 中間 |
| Y | [stick axis 1~4](/en/sensors/ps4/reference.md#v10)  7~8  | B C M Y = LX LY RX RY，0~99，50 = 中間 |

```cpp
// MATRIXblock → Arduino：stock library
int r = MiniR4.I2C1.MXColor.getColor(R);   // D-pad
```

## B. Big channel (LauMo library)

After swapping in the `MiniR4ColorSensorExt` big-channel library ([download](/en/downloads.md)) the same blocks become pointer reads: ColorID resets the pointer and returns the status byte, Blue reads 1 byte, Green 1 signed byte, Saturation 2 bytes, Hue 2 signed bytes, Value a 4-byte float, Red advances the pointer by the size of the last read.

<figure class="ref-fig"><img src="/images/figures/common/packet-big30.svg" alt="LauMo 30-byte packet" loading="lazy"></figure>

| byte | Block | Content | Note |
| :--- | :--- | :--- | :--- |
| 8~9 | Saturation | [16 buttons](/en/sensors/ps4/reference.md#v01) | PSX 排列（Saturation 積木讀 2 byte） |
| 0 | Blue | [D-pad](/en/sensors/ps4/reference.md#v02) |  |
| 1 | Blue | [shape buttons](/en/sensors/ps4/reference.md#v03) |  |
| 2 | Blue | [shoulder buttons](/en/sensors/ps4/reference.md#v04) |  |
| 3 | Blue | [function buttons](/en/sensors/ps4/reference.md#v05) |  |
| 10 | Blue | [connected](/en/sensors/ps4/reference.md#v06) |  |
| 11 | Blue | [battery](/en/sensors/ps4/reference.md#v07) |  |
| 4~7 | Blue | [stick axis 1~4](/en/sensors/ps4/reference.md#v10)（1 byte each） | 0~99，50 = 中間 |
| 24 | ColorID | [F3 status flags](/en/sensors/ps4/reference.md#flags) |  |
| 25 | Cyan | F1 completion count | 0 |
| 26 | Magenta | F2 completion count | 0 |
| 27 | Yellow | device ID | 0x42 |
| 28 | Key | hw[0] | 2 |
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
int buttons16 = bigU8(8);   // 16 buttons
```

## Sources and assumptions

Generated by `_build/gen-reference.mjs` from `_build/protocol/ps4.json` (single source of truth, transcribed from the 2026-10-21 protocol decisions).

- 值表、命令、旗標與 PS2 完全相同（mBlock 定案「兩款相同」＋I2C 定案 §3／§4）；只有 hw0 = 2、model = PS4、E5／E6 有作用。
- D3：SPIKE 模式 0 教育版 byte 6~7、模式 6 v[2]、模式 9 v[6] 的 button16 都是 PSX 排列（#65 已把 PS4 改成 PSX，盤點檔寫的 Bluepad32 排列已過時）。
- 0x22 命令 4 同時裝方向鍵（低 nibble）與形狀鍵（高 nibble）、命令 5 裝肩鍵（低）與功能鍵（高）；JSON 只能把一個命令掛在一個值上，所以掛在 02／04，03／05 的 ch22 = null（內容寫在 02／04 的 note）。SPIKE 模式 0／1／2／9 的 nibble 也用同樣方式在 note 說明。
- 搖桿：I2C／mBlock 送 ±100；EV3／0x22／大通道送 0~99（50 = 中間）、SPIKE 送 1~9（5 = 中間），不是線性 offset，主機端各自換算（網站範例已是這樣）。
- EV3 模式 3 = 方向鍵 | 形狀鍵<<4 | 已連線<<8（#67 新增 bit8）；模式 4 內容不變（[方向鍵, LX, 形狀鍵, LY]）；模式 5 改送裝置 ID 格式 B [0x42 | 2<<8, 韌體版]。
- SPIKE 模式 3 改 [已連線, 電量, 0]（3×int8）、模式 8 改 [0x42 | 2<<8, 協議版 2, 韌體版, 更新序號]；其餘空模式 1、2、4、6 維持現狀。
- 大通道尾段：byte 24 = F3、25~26 = 0（無 F1／F2 動作）、27 = 0x42、28 = 2；F1~F4 收到不做任何動作。
- ESP32 從機不能時脈延展：I2C v2 在 onReceive 就預載從該編號起 32 B；0x22／大通道照 MATRIX 定案 §5 #2 預載最新一包，待上機。
- I2C 0A 系統鍵 b1 = 擷取鍵（v1 0x15 放在 b3，v2 改 b1，照 I2C 定案 §4）。
- Library rules (command 1 returns 0x43, CRC-8, 5 ms minimum interval) follow the LauMo big-channel library source.
