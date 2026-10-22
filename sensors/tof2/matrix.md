<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-reference.mjs 從 _build/protocol/tof2.json 產生（matrix），請勿手改；改內容請改 JSON 後重跑 node _build/gen-reference.mjs -->
# 測距者 2 路 × MATRIX Mini R4

[🔙 回到測距者 2 路產品頁](/sensors/tof2/index.md)

測距者 2 路 MATRIX 版接在 Mini R4 的 I2C 孔，位址 `0x22`，有兩種用法：**A. 原廠顏色感應器積木**（不用換函式庫，R~K 七個 0~255 的數）與 **B. LauMo 大通道**（換成大通道函式庫，一包 30 byte 什麼都有）。兩種要裝不同的函式庫檔，不能同時用。

<p class="chips"><a class="chip chip-spike" href="/sensors/tof2/index.md">SPIKE</a><a class="chip chip-ev3" href="/sensors/tof2/index.md">EV3</a><a class="chip chip-arduino" href="/sensors/tof2/arduino-i2c.md">Arduino I2C</a><a class="chip chip-mblock" href="/sensors/tof2/mblock.md">mBlock</a><a class="chip chip-matrix current" href="/sensors/tof2/matrix.md">MATRIX</a><a class="chip chip-microbit" href="/sensors/tof2/microbit.md">micro:bit</a></p>

## 接線

<figure class="ref-fig"><img src="/images/figures/tof2/wiring-matrix.svg" alt="測距者 2 路 × MATRIX Mini R4" loading="lazy"></figure>

用 MATRIX 原廠 4 pin 感應器線接 I2C1 或 I2C2，積木裡選對應的孔。

## A. 原廠積木（0x22）

積木「顏色感應器 → R／G／B／C／M／Y／K」各讀 1 個 0~255 的數。⚠️ 不要用原廠的 H／S／V、顏色辨識積木（那是主機拿 R、G、B 自己算的，對本產品沒有意義）；灰階、顏色編號積木已被 MATRIX 拿掉。

| 積木 | 內容 | 解法 |
| :--- | :--- | :--- |
| R | [左距離 cm](/sensors/tof2/reference.md#v01) |  |
| G | [右距離 cm](/sensors/tof2/reference.md#v02) |  |
| B | [較近距離 cm](/sensors/tof2/reference.md#v03) | B（命令 12 也回較近，schema 不收 12） |

```cpp
// MATRIXblock → Arduino：原廠函式庫
int r = MiniR4.I2C1.MXColor.getColor(R);   // 左距離 cm
```

## B. 大通道（LauMo 函式庫）

函式庫 `MiniR4ColorSensorExt大通道版`（[下載](/downloads.md)）換掉原廠檔後，同一組積木變成「指標讀法」：ColorID 把指標歸零並回傳狀態、Blue 讀 1 byte、Green 讀 1 byte 有正負、Saturation 讀 2 byte、Hue 讀 2 byte 有正負、Value 讀 4 byte 小數、Red 把指標往後移「上一個讀取的大小」。

<figure class="ref-fig"><img src="/images/figures/common/packet-big30.svg" alt="LauMo 30-byte packet" loading="lazy"></figure>

| byte | 積木 | 內容 | 備註 |
| :--- | :--- | :--- | :--- |
| 0 | Blue | [左距離 cm](/sensors/tof2/reference.md#v01) |  |
| 1 | Blue | [右距離 cm](/sensors/tof2/reference.md#v02) |  |
| 2 | Blue | [較近距離 cm](/sensors/tof2/reference.md#v03) |  |
| 5~6 | Saturation | [左距離 mm](/sensors/tof2/reference.md#v04) | #67 追加（MATRIX 定案 §4：[5..6] 小端） |
| 7~8 | Saturation | [右距離 mm](/sensors/tof2/reference.md#v05) | #67 追加（[7..8] 小端） |
| 3 | Blue | [初始化位元圖](/sensors/tof2/reference.md#v06) |  |
| 4 | Blue | [故障位元圖](/sensors/tof2/reference.md#v07) |  |
| 24 | ColorID | [F3 狀態旗標](/sensors/tof2/reference.md#flags) |  |
| 25 | Cyan | F1 動作完成次數 | 0 |
| 26 | Magenta | F2 動作完成次數 | 0 |
| 27 | Yellow | 裝置 ID | 0x29 |
| 28 | Key | hw[0] | 2 |
| 29 | — | CRC-8 | 多項式 0x07，涵蓋 0~28；函式庫自己檢查 |

**動作**：無（Cyan／Magenta／Yellow／Key 只回傳 byte 25~28，不做事）

### Arduino 寫法（MATRIXblock 的 Arduino 模式）

```cpp
// 大通道：讀第 n 個 byte（uint8）／第 n 個起的 2 byte（uint16 小端）
uint8_t bigU8(uint8_t n) {
  MiniR4.I2C1.MXColor.getColorID();                 // 指標回到 byte 0
  uint8_t v = MiniR4.I2C1.MXColor.getColor(B);      // 讀 byte 0
  for (uint8_t i = 0; i < n; i++) { MiniR4.I2C1.MXColor.getColor(R); v = MiniR4.I2C1.MXColor.getColor(B); }   // Red = 前進 1
  return v;
}
uint16_t bigU16(uint8_t n) {
  MiniR4.I2C1.MXColor.getColorID();
  MiniR4.I2C1.MXColor.getColor(B);
  for (uint8_t i = 0; i < n; i++) MiniR4.I2C1.MXColor.getColor(R);
  return (uint16_t)MiniR4.I2C1.MXColor.getS();      // Saturation = 讀 2 byte
}
int16_t bigI16(uint8_t n) {
  MiniR4.I2C1.MXColor.getColorID();
  MiniR4.I2C1.MXColor.getColor(B);
  for (uint8_t i = 0; i < n; i++) MiniR4.I2C1.MXColor.getColor(R);
  return (int16_t)MiniR4.I2C1.MXColor.getH();       // Hue = 讀 2 byte 有正負
}
```

```cpp
int leftCm = bigU8(0);   // 左距離 cm
```

## 本頁依據與假設

本頁由 `_build/gen-reference.mjs` 從 `_build/protocol/tof2.json`（單一真相來源，轉錄自 2026-10-21 協定定案）產生。

- 共識 #8：08 左右平均 mm 補進 I2C／mBlock 值表（定案 §5 已列）。
- B-7：大通道 [24] 由 sensorInitOk 改成 F3 狀態旗標（統一格式），[25..26] = 0（沒有動作）、[27] = 0x29、[28] = 2；[0..4] 不動、追加 [5..6] 左 mm、[7..8] 右 mm（u16 小端）。
- 0x22 命令 4~12 不動（4 左、5 右、6 最近、7~11 = 0、12 = 最近）；schema 不收 11/12，所以 12 只寫在 03 的 note。
- EV3：模式 0、2、4 鎖住（照盤點）；模式 1 改 [左,右]×4（index 0/1 各代表左右）、模式 3 改平均 mm、模式 5 改 [左 mm, 右 mm]；模式 1 描述表宣告 int8 −100~100，cm×100/120 在範圍內。
- SPIKE：0/5/7/9 鎖住，另加模式 3 鎖住（網站 sensors/tof2/index.md read(3) = 左／右 cm，共識 #11）；模式 6 改 [左 mm, 右 mm, 平均 mm]；模式 1、2、4、8 維持不動；模式 8 v[0] 改 ID|hw[0]<<8（統一後共用函式）。
- 「左」= 站在車後往前看的左（第 0 顆，XSHUT PA6）；韌體只有 L/R 巨集名，盤點檔標為推測。
- 沒有寫入命令；F8 換位址／EF 重開機用共用的。
- 函式庫規則（命令 1 回 0x43、CRC-8、最快 5 ms 讀一次）以 `80_上位機研究\MatrixMiniR4\MiniR4ColorSensorExt大通道版` 為準。
