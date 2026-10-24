<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（line16 × matrix × bigchannel），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: line16/matrix/bigchannel -->
# 循行者 16 路 (Line16) × MATRIX × 大通道函式庫

這一頁只講 **LauMo 大通道函式庫（MATRIXblock／Arduino）**：在 MATRIX Mini R4 上用它讀循行者 16 路 (Line16)的安裝步驟、每個值怎麼讀、能用的命令與範例。要換產品、主機或軟體，用上方的切換列。

## 安裝與設定

<!-- setup: _build/guides/software/matrix-bigchannel.zh.md + _build/guides/product/line16/matrix-bigchannel.zh.md {"product":"循行者 16 路 (Line16)","edition":"MATRIX 版","port":"I2C1","lib":"","libUrl":"","addr":"0x16"} -->
<!-- include-fallback -->
> [!NOTE]
> 這個軟體的安裝步驟正在搬到這一頁。先看原本的說明：[循行者 16 路 (Line16)：LauMo 大通道函式庫（MATRIXblock／Arduino） 說明](/sensors/line16/matrix.md)。
<!-- /include-fallback -->

## 讀值

換上大通道函式庫後，用下面的 `bigU8()`／`bigU16()` 小幫手讀第幾個 byte（展開「小幫手程式」）。

<details class="helper">
<summary>小幫手程式</summary>

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

</details>

<section id="v01" class="ref-item">

### 線位置 <small>linePos</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-linePos.svg" alt="線位置" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-16~+16` | 線在哪一路：−16（最左）~+16（最右），0 = 正中間；站在車後往前看，負 = 線在左（平常 ±15，±16 只在最外側掉線後 200 ms 內） |

```cpp
int linePos = bigU8(0) - 16;   // 線位置
```

<p class="ref-note">大通道 byte 0（u8）</p>

</section>

<section id="v02" class="ref-item">

### 細位置 <small>linePosFine</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-linePosFine.svg" alt="細位置" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-100~+100` | 線位置的細分：−100 ~ +100，0 = 正中間，負 = 線在左（與線位置同號） |

```cpp
int linePosFine = bigU8(2) - 100;   // 細位置
```

<p class="ref-note">大通道 byte 2（u8）</p>

</section>

<section id="v03" class="ref-item">

### 線寬 <small>lineWidth</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-lineWidth.svg" alt="線寬" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~16` 路 | 壓到線的路數 0~16，0 = 找不到線 |

```cpp
int lineWidth = bigU8(1);   // 線寬
```

<p class="ref-note">大通道 byte 1（u8）</p>

</section>

<section id="v04" class="ref-item">

### 線段數 <small>groupCount</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-groupCount.svg" alt="線段數" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~8` | 看到幾段線：0 丟線、1 一條線、2 以上 = 分岔／十字 |

```cpp
int groupCount = bigU8(21);   // 線段數
```

<p class="ref-note">大通道 byte 21（u8）；#67 追加（MATRIX 定案 §4：[21] 線段數）</p>

</section>

<section id="v05" class="ref-item">

### 壓線位元圖 <small>bitmap</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-bitmap.svg" alt="壓線位元圖" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~65535` bit | bit0 = 第 1 路（最左）… bit15 = 第 16 路，1 = 那一路在線上 |

```cpp
int bitmap = bigU16(19);   // 壓線位元圖
```

<p class="ref-note">大通道 byte 19~20（u16）；#67 追加（MATRIX 定案 §4：[19..20] 小端）</p>

</section>

<section id="v10" class="ref-item">

### 第 1~16 路光值 <small>light</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-light.svg" alt="第 1~16 路光值" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~100` | 第 1~16 路校準後光值 0~100（第 1 路最左）；編號 10~1F |

```cpp
int light[16];
for (uint8_t i = 0; i < 16; i++) light[i] = bigU8(3 + i);   // 第 1~16 路光值
```

<p class="ref-note">大通道 byte 3~18（u8）</p>

</section>

<p class="ref-missing"><b>這個軟體讀不到</b>：最後離線方向、二值化門檻、校準完成次數。要用這些值請改用 <a href="/sensors/line16/arduino/ide.md">Arduino</a>／<a href="/sensors/line16/mblock/mblock5.md">mBlock</a>／<a href="/sensors/line16/microbit/makecode.md">micro:bit</a>。</p>

## 命令

<section id="ce2" class="ref-item ref-cmd">

### 黑色校準 <small>calibBlack</small>

把目前讀值記為黑（感應器壓在黑線上），約 50 ms；寫入 EEPROM

```cpp
MiniR4.I2C1.MXColor.getColor(C);   // 大通道 F1（Cyan）= 黑色校準，回傳上一包的完成次數
```

<p class="ref-note">只有大通道函式庫有這個動作；0x22 原廠函式庫沒有。</p>

</section>

<section id="ce3" class="ref-item ref-cmd">

### 白色校準 <small>calibWhite</small>

把目前讀值記為白（感應器壓在白底上），約 50 ms；寫入 EEPROM

```cpp
MiniR4.I2C1.MXColor.getColor(M);   // 大通道 F2（Magenta）= 白色校準，回傳上一包的完成次數
```

<p class="ref-note">只有大通道函式庫有這個動作；0x22 原廠函式庫沒有。</p>

</section>

<p class="ref-missing"><b>這個軟體送不了</b>：切黑線、切白線、設定門檻、5 秒動態校準</p>

## 其他問題

- **原廠積木和大通道可以一起用嗎？** 不行：兩種要裝不同的函式庫檔，一個程式只能選一種。

還有問題？到 [聯絡我們](/contact.md) 附上你用的軟體和程式截圖。

## 下載

- [MiniR4ColorSensorExt_LauMo_v2.zip](/downloads/MiniR4ColorSensorExt_LauMo_v2.zip)
- [下載中心（全部產品的函式庫、韌體、型錄）](/downloads.md)

## 本頁依據

- 主機與軟體清單：`_build/guides.json`
- 值與命令：`_build/protocol/line16.json`（單一真相來源）
- 安裝與設定：`_build/guides/software/matrix-bigchannel.zh.md`、`_build/guides/product/line16/matrix-bigchannel.zh.md`
