<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-reference.mjs 從 _build/protocol/ps2.json 產生（matrix），請勿手改；改內容請改 JSON 後重跑 node _build/gen-reference.mjs -->
<!-- moved: /en/sensors/ps2/matrix/index.md -->
<p class="moved-note">This page has moved into the new guide: <a href="/en/sensors/ps2/matrix/index.md">Controller 2.4G › MATRIX</a></p>

# PS2 Gamepad Receiver × MATRIX Mini R4

[🔙 Back to PS2 Gamepad Receiver](/en/sensors/ps2/index.md)

The MATRIX edition of the PS2 Gamepad Receiver plugs into a Mini R4 I2C port at address `0x22` and can be read two ways: **A. stock colour-sensor blocks** (no library change, R~K = seven 0~255 numbers) or **B. the LauMo big channel** (swap in the big-channel library, one 30-byte packet with everything). They need different library files and cannot be mixed.

<p class="chips"><a class="chip chip-spike" href="/en/sensors/ps2/index.md">SPIKE</a><a class="chip chip-ev3" href="/en/sensors/ps2/index.md">EV3</a><a class="chip chip-arduino" href="/en/sensors/ps2/arduino-i2c.md">Arduino I2C</a><a class="chip chip-mblock" href="/en/sensors/ps2/mblock.md">mBlock</a><a class="chip chip-matrix current" href="/en/sensors/ps2/matrix.md">MATRIX</a><a class="chip chip-microbit" href="/en/sensors/ps2/microbit.md">micro:bit</a></p>

## Wiring

<figure class="ref-fig"><img src="/images/figures/ps2/wiring-matrix.svg" alt="PS2 Gamepad Receiver × MATRIX Mini R4" loading="lazy"></figure>

Use the stock MATRIX 4-pin sensor cable on I2C1 or I2C2 and pick that port in the block.

## A. Stock blocks (0x22)

The "colour sensor → R / G / B / C / M / Y / K" blocks each return one 0~255 number. ⚠️ Do not use the stock H / S / V or colour-ID blocks (the host computes them from R, G, B; meaningless here); the grayscale and colour-number blocks were removed by MATRIX.

| Block | Content | Decode |
| :--- | :--- | :--- |
| R | [d-pad](/en/sensors/ps2/reference.md#v02) | #67 新增：R = 方向鍵 \| 形狀鍵<<4（上 1、右 2、下 4、左 8、▲16、●32、✕64、■128） |
| G | [shoulder buttons](/en/sensors/ps2/reference.md#v04) | #67 新增：G = 肩鍵 \| 功能鍵<<4（L2 1、R2 2、L1 4、R1 8、SELECT 16、L3 32、R3 64、START 128） |
| K | [connected](/en/sensors/ps2/reference.md#v06) | #67 新增：K = 已連線 1／0 |
| B | [stick LX / LY / RX / RY](/en/sensors/ps2/reference.md#v10)  1~2  | #67 新增：B、C、M、Y = LX、LY、RX、RY |
| C | [stick LX / LY / RX / RY](/en/sensors/ps2/reference.md#v10)  3~4  | #67 新增：B、C、M、Y = LX、LY、RX、RY |
| M | [stick LX / LY / RX / RY](/en/sensors/ps2/reference.md#v10)  5~6  | #67 新增：B、C、M、Y = LX、LY、RX、RY |
| Y | [stick LX / LY / RX / RY](/en/sensors/ps2/reference.md#v10)  7~8  | #67 新增：B、C、M、Y = LX、LY、RX、RY |

```cpp
// MATRIXblock → Arduino：stock library
int r = MiniR4.I2C1.MXColor.getColor(R);   // d-pad
```

## B. Big channel (LauMo library)

After swapping in the `MiniR4ColorSensorExt` big-channel library ([download](/en/downloads.md)) the same blocks become pointer reads: ColorID resets the pointer and returns the status byte, Blue reads 1 byte, Green 1 signed byte, Saturation 2 bytes, Hue 2 signed bytes, Value a 4-byte float, Red advances the pointer by the size of the last read.

<figure class="ref-fig"><img src="/images/figures/common/packet-big30.svg" alt="LauMo 30-byte packet" loading="lazy"></figure>

| byte | Block | Content | Note |
| :--- | :--- | :--- | :--- |
| 8~9 | Saturation | [16 buttons](/en/sensors/ps2/reference.md#v01) | #67 新增（[8..9] 小端） |
| 0 | Blue | [d-pad](/en/sensors/ps2/reference.md#v02) | #67 新增 |
| 1 | Blue | [shape buttons](/en/sensors/ps2/reference.md#v03) | #67 新增 |
| 2 | Blue | [shoulder buttons](/en/sensors/ps2/reference.md#v04) | #67 新增 |
| 3 | Blue | [function buttons](/en/sensors/ps2/reference.md#v05) | #67 新增 |
| 10 | Blue | [connected](/en/sensors/ps2/reference.md#v06) | #67 新增 |
| 11 | Blue | [battery](/en/sensors/ps2/reference.md#v07) | #67 新增；PS2 = 0 |
| 4~7 | Blue | [stick LX / LY / RX / RY](/en/sensors/ps2/reference.md#v10)（1 byte each） | #67 新增：[4..7] = LX、LY、RX、RY |
| 24 | ColorID | [F3 status flags](/en/sensors/ps2/reference.md#flags) |  |
| 25 | Cyan | F1 completion count | 0 |
| 26 | Magenta | F2 completion count | 0 |
| 27 | Yellow | device ID | 0x42 |
| 28 | Key | hw[0] | 1 |
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

Generated by `_build/gen-reference.mjs` from `_build/protocol/ps2.json` (single source of truth, transcribed from the 2026-10-21 protocol decisions).

- hw[0] = 1（PS2）、電量 07 恆 0、系統鍵 0A 恆 0、手把類型 0B 只會出現 0／1／2（I2C 定案 §2、§4）。
- 08／09 L2／R2 類比照定案寫 0~255：DS2 類比模式有壓力值；沒有壓力值時按下 = 255、放開 = 0（沿用 v1 0x14 的做法，盤點 .ino:598-603）。
- E5 震動、E6 燈條照 I2C 定案 §3 列在 PS2／PS4 共用命令表；PS2 收到回 ACK 後忽略（跟 v1 0x10／0x23 一樣）。
- 0x22 新增（MATRIX 定案 §6）：1 = 0x43、4 = 方向|形狀<<4、5 = 肩|功能<<4、6~9 = LX LY RX RY 0~99、10 = 已連線、11~12 = 0；大通道新增 [0..3] 四組鍵、[4..7] 搖桿 0~99、[8..9] button16、[10] 已連線、[11] 電量，尾段 [24] F3、[27] 0x42、[28] 1；F1~F4 無動作。ch22 命令 4／5 各掛在 02／04 底下（03／05 在 note 說明高 4 bit），避免同一命令重複登記。
- 搖桿 10~13 在 I2C／mBlock 是 ±100（由 raw (v−128)×100/128、Y 上為正）；EV3／大通道／0x22 維持 0~99（50 中間）、SPIKE 維持 1~9（5 中間）照盤點，不改鎖住模式。
- EV3：模式 0、1、2、4 鎖住（照盤點）；模式 3 改 方向|形狀<<4|已連線<<8（原本沒有 bit8）；模式 5 改成 ID 格式 B（原本 = 按鍵／搖桿合併版）。
- SPIKE：0/5/7/9 鎖住，1、2、4、6 維持不動；模式 3 改 [已連線, 電量(0), 0]（原本 = 4 組按鍵）；模式 8 改 v[0] = ID|hw[0]<<8（原本 = 搖桿）。模式 9 v[6] button16 = PSX 排列（D3）。
- SPIKE 模式 5 只容得下 11 鍵（L2、R2、L3、R3、START 讀不到），鎖住不改。
- Library rules (command 1 returns 0x43, CRC-8, 5 ms minimum interval) follow the LauMo big-channel library source.
