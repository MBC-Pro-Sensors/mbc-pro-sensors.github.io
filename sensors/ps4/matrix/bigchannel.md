<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（ps4 × matrix × bigchannel），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: ps4/matrix/bigchannel -->
# 掌控者 PS4/PS5 × MATRIX × 大通道函式庫

這一頁只講 **LauMo 大通道函式庫（MATRIXblock／Arduino）**：在 MATRIX Mini R4 上用它讀掌控者 PS4/PS5的安裝步驟、每個值怎麼讀、能用的命令與範例。要換產品、主機或軟體，用上方的切換列。

## 安裝與設定

<!-- setup: _build/guides/software/matrix-bigchannel.zh.md + _build/guides/product/ps4/matrix-bigchannel.zh.md {"product":"掌控者 PS4/PS5","edition":"MATRIX 版","port":"I2C1","lib":"","libUrl":"","addr":"0x42"} -->
<!-- include-fallback -->
> [!NOTE]
> 這個軟體的安裝步驟正在搬到這一頁。先看原本的說明：[掌控者 PS4/PS5：LauMo 大通道函式庫（MATRIXblock／Arduino） 說明](/sensors/ps4/matrix.md)。
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

### 十六鍵 <small>buttons16</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-buttons16.svg" alt="十六鍵" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~65535` bit | 全部按鍵一個 16 bit（PSX 排列）：bit0 SELECT(SHARE)、1 L3、2 R3、3 START(OPTIONS)、4 上、5 右、6 下、7 左、8 L2、9 R2、10 L1、11 R1、12 ▲、13 ●、14 ✕、15 ■；1 = 按下；未連線 = 0 |

```cpp
int buttons16 = bigU16(8);   // 十六鍵
```

<p class="ref-note">大通道 byte 8~9（u16）；PSX 排列（Saturation 積木讀 2 byte）</p>

</section>

<section id="v02" class="ref-item">

### 方向鍵 <small>dpad</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-dpad.svg" alt="方向鍵" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~15` bit | 上 1、右 2、下 4、左 8，同時按相加（0~15） |

```cpp
int dpad = bigU8(0);   // 方向鍵
```

<p class="ref-note">大通道 byte 0（u8）</p>

</section>

<section id="v03" class="ref-item">

### 形狀鍵 <small>shapeKeys</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-shapeKeys.svg" alt="形狀鍵" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~15` bit | ▲ 1、● 2、✕ 4、■ 8，同時按相加（0~15） |

```cpp
int shapeKeys = bigU8(1);   // 形狀鍵
```

<p class="ref-note">大通道 byte 1（u8）</p>

</section>

<section id="v04" class="ref-item">

### 肩鍵 <small>shoulderKeys</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-shoulderKeys.svg" alt="肩鍵" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~15` bit | L2 1、R2 2、L1 4、R1 8，同時按相加（0~15） |

```cpp
int shoulderKeys = bigU8(2);   // 肩鍵
```

<p class="ref-note">大通道 byte 2（u8）</p>

</section>

<section id="v05" class="ref-item">

### 功能鍵 <small>funcKeys</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-funcKeys.svg" alt="功能鍵" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~15` bit | SELECT(SHARE) 1、L3 2、R3 4、START(OPTIONS) 8，同時按相加（0~15） |

```cpp
int funcKeys = bigU8(3);   // 功能鍵
```

<p class="ref-note">大通道 byte 3（u8）</p>

</section>

<section id="v06" class="ref-item">

### 已連線 <small>connected</small>

| 範圍 | 意思 |
| :--- | :--- |
| `0~1` | 1 = 手把已連線且 300 ms 內有資料，0 = 未連線（斷線時按鍵 0、搖桿中間） |

```cpp
int connected = bigU8(10);   // 已連線
```

<p class="ref-note">大通道 byte 10（u8）</p>

</section>

<section id="v07" class="ref-item">

### 電量 <small>battery</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-battery.svg" alt="電量" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~100` % | 手把電量 0~100（PS2 接收器恆 0） |

```cpp
int battery = bigU8(11);   // 電量
```

<p class="ref-note">大通道 byte 11（u8）</p>

</section>

<section id="v10" class="ref-item">

### 搖桿 1~4 軸 <small>stick</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-stick.svg" alt="搖桿 1~4 軸" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-100~+100` | 10 LX、11 LY、12 RX、13 RY：−100 ~ +100，0 = 中間，右／上為正；未連線 0 |

```cpp
int stick[4];
for (uint8_t i = 0; i < 4; i++) stick[i] = bigU8(4 + i);   // 搖桿 1~4 軸
```

<p class="ref-note">大通道 byte 4~7（u8）；0~99，50 = 中間</p>

</section>

<p class="ref-missing"><b>這個軟體讀不到</b>：L2 類比、R2 類比、系統鍵、手把類型。要用這些值請改用 <a href="/sensors/ps4/arduino/ide.md">Arduino</a>／<a href="/sensors/ps4/mblock/mblock5.md">mBlock</a>／<a href="/sensors/ps4/microbit/makecode.md">micro:bit</a>。</p>

## 命令

這個軟體不能送命令；需要時改用 Arduino I2C、mBlock 或 micro:bit。

## 其他問題

- **原廠積木和大通道可以一起用嗎？** 不行：兩種要裝不同的函式庫檔，一個程式只能選一種。

還有問題？到 [聯絡我們](/contact.md) 附上你用的軟體和程式截圖。

## 下載

- [MiniR4ColorSensorExt_LauMo_v2.zip](/downloads/MiniR4ColorSensorExt_LauMo_v2.zip)
- [下載中心（全部產品的函式庫、韌體、型錄）](/downloads.md)

## 本頁依據

- 主機與軟體清單：`_build/guides.json`
- 值與命令：`_build/protocol/ps4.json`（單一真相來源）
- 安裝與設定：`_build/guides/software/matrix-bigchannel.zh.md`、`_build/guides/product/ps4/matrix-bigchannel.zh.md`
