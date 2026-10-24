<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（tof2 × matrix × bigchannel），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: tof2/matrix/bigchannel -->
# 測距者 2 路 (TOF2) × MATRIX × 大通道函式庫

這一頁只講 **LauMo 大通道函式庫（MATRIXblock／Arduino）**：在 MATRIX Mini R4 上用它讀測距者 2 路 (TOF2)的安裝步驟、每個值怎麼讀、能用的命令與範例。要換產品、主機或軟體，用上方的切換列。

## 安裝與設定

<!-- setup: _build/guides/software/matrix-bigchannel.zh.md + _build/guides/product/tof2/matrix-bigchannel.zh.md {"product":"測距者 2 路 (TOF2)","edition":"MATRIX 版","port":"I2C1","lib":"","libUrl":"","addr":"0x29"} -->
<!-- include-fallback -->
> [!NOTE]
> 這個軟體的安裝步驟正在搬到這一頁。先看原本的說明：[測距者 2 路 (TOF2)：LauMo 大通道函式庫（MATRIXblock／Arduino） 說明](/sensors/tof2/matrix.md)。
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

### 左距離 cm <small>leftCm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-leftCm.svg" alt="左距離 cm" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~120` cm | 左邊那顆的距離 0~120 cm（站在車後往前看，左 = 第 0 顆），120 = 前面沒東西／那顆失效 |

```cpp
int leftCm = bigU8(0);   // 左距離 cm
```

<p class="ref-note">大通道 byte 0（u8）</p>

</section>

<section id="v02" class="ref-item">

### 右距離 cm <small>rightCm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-rightCm.svg" alt="右距離 cm" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~120` cm | 右邊那顆的距離 0~120 cm（右 = 第 1 顆），120 = 前面沒東西／那顆失效 |

```cpp
int rightCm = bigU8(1);   // 右距離 cm
```

<p class="ref-note">大通道 byte 1（u8）</p>

</section>

<section id="v03" class="ref-item">

### 較近距離 cm <small>nearCm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-nearCm.svg" alt="較近距離 cm" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~120` cm | 左右兩顆取較近的 0~120 cm |

```cpp
int nearCm = bigU8(2);   // 較近距離 cm
```

<p class="ref-note">大通道 byte 2（u8）</p>

</section>

<section id="v04" class="ref-item">

### 左距離 mm <small>leftMm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-leftMm.svg" alt="左距離 mm" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~1200` mm | 左邊那顆的距離 0~1200 mm，1200 = 前面沒東西／那顆失效 |

```cpp
int leftMm = bigU16(5);   // 左距離 mm
```

<p class="ref-note">大通道 byte 5~6（u16）；#67 追加（MATRIX 定案 §4：[5..6] 小端）</p>

</section>

<section id="v05" class="ref-item">

### 右距離 mm <small>rightMm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-rightMm.svg" alt="右距離 mm" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~1200` mm | 右邊那顆的距離 0~1200 mm，1200 = 前面沒東西／那顆失效 |

```cpp
int rightMm = bigU16(7);   // 右距離 mm
```

<p class="ref-note">大通道 byte 7~8（u16）；#67 追加（[7..8] 小端）</p>

</section>

<section id="v06" class="ref-item">

### 初始化位元圖 <small>initOk</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-initOk.svg" alt="初始化位元圖" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~3` bit | bit0 = 左、bit1 = 右，1 = 那顆開機初始化成功（失敗不重試）；3 = 兩顆都好 |

```cpp
int initOk = bigU8(3);   // 初始化位元圖
```

<p class="ref-note">大通道 byte 3（u8）</p>

</section>

<section id="v07" class="ref-item">

### 故障位元圖 <small>fault</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-fault.svg" alt="故障位元圖" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~3` bit | bit0 = 左、bit1 = 右，1 = 那顆目前失效（I2C 讀失敗或 200 ms 沒更新）；失效那顆距離回 120／1200 |

```cpp
int fault = bigU8(4);   // 故障位元圖
```

<p class="ref-note">大通道 byte 4（u8）</p>

</section>

<p class="ref-missing"><b>這個軟體讀不到</b>：左右平均 mm。要用這些值請改用 <a href="/sensors/tof2/arduino/ide.md">Arduino</a>／<a href="/sensors/tof2/mblock/mblock5.md">mBlock</a>／<a href="/sensors/tof2/microbit/makecode.md">micro:bit</a>。</p>

## 其他問題

- **原廠積木和大通道可以一起用嗎？** 不行：兩種要裝不同的函式庫檔，一個程式只能選一種。

還有問題？到 [聯絡我們](/contact.md) 附上你用的軟體和程式截圖。

## 下載

- [MiniR4ColorSensorExt_LauMo_v2.zip](/downloads/MiniR4ColorSensorExt_LauMo_v2.zip)
- [下載中心（全部產品的函式庫、韌體、型錄）](/downloads.md)

## 本頁依據

- 主機與軟體清單：`_build/guides.json`
- 值與命令：`_build/protocol/tof2.json`（單一真相來源）
- 安裝與設定：`_build/guides/software/matrix-bigchannel.zh.md`、`_build/guides/product/tof2/matrix-bigchannel.zh.md`
