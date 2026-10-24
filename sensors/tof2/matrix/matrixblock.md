<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（tof2 × matrix × matrixblock），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: tof2/matrix/matrixblock -->
# 測距者 2 路 (TOF2) × MATRIX × MATRIXblock 原廠積木

這一頁只講 **MATRIXblock 原廠顏色感應器積木（0x22）**：在 MATRIX Mini R4 上用它讀測距者 2 路 (TOF2)的安裝步驟、每個值怎麼讀、能用的命令與範例。要換產品、主機或軟體，用上方的切換列。

## 要買哪個版本、怎麼接

買 **MATRIX 版**，用 MATRIX 原廠 4 pin 感應器線接 Mini R4 的 I2C1 或 I2C2（位址 `0x22`），積木裡選對應的孔。

<figure class="ref-fig"><img src="/images/figures/tof2/wiring-matrix.svg" alt="測距者 2 路 (TOF2) × MATRIX Mini R4" loading="lazy"></figure>

## 安裝與設定

<!-- setup: _build/guides/software/matrix-matrixblock.zh.md + _build/guides/product/tof2/matrix-matrixblock.zh.md {"product":"測距者 2 路 (TOF2)","edition":"MATRIX 版","port":"I2C1","lib":"","libUrl":"","addr":"0x29"} -->
<!-- include-fallback -->
> [!NOTE]
> 這個軟體的安裝步驟正在搬到這一頁。先看原本的說明：[測距者 2 路 (TOF2)：MATRIXblock 原廠顏色感應器積木（0x22） 說明](/sensors/tof2/matrix.md)。
<!-- /include-fallback -->

## 讀值

用 MATRIXblock 原廠「顏色感應器」積木的 R／G／B／C／M／Y／K 讀（不要用 H／S／V 與顏色辨識積木）；Arduino 模式寫法一併列出。

<section id="v01" class="ref-item">

### 左距離 cm <small>leftCm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-leftCm.svg" alt="左距離 cm" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~120` cm | 左邊那顆的距離 0~120 cm（站在車後往前看，左 = 第 0 顆），120 = 前面沒東西／那顆失效 |

積木「顏色感應器 → R」

```cpp
int leftCm = MiniR4.I2C1.MXColor.getColor(R);   // 左距離 cm
```

</section>

<section id="v02" class="ref-item">

### 右距離 cm <small>rightCm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-rightCm.svg" alt="右距離 cm" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~120` cm | 右邊那顆的距離 0~120 cm（右 = 第 1 顆），120 = 前面沒東西／那顆失效 |

積木「顏色感應器 → G」

```cpp
int rightCm = MiniR4.I2C1.MXColor.getColor(G);   // 右距離 cm
```

</section>

<section id="v03" class="ref-item">

### 較近距離 cm <small>nearCm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-nearCm.svg" alt="較近距離 cm" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~120` cm | 左右兩顆取較近的 0~120 cm |

積木「顏色感應器 → B」

```cpp
int nearCm = MiniR4.I2C1.MXColor.getColor(B);   // 較近距離 cm
```

</section>

<p class="ref-missing"><b>這個軟體讀不到</b>：左距離 mm、右距離 mm、初始化位元圖、故障位元圖、左右平均 mm。要用這些值請改用 <a href="/sensors/tof2/arduino/ide.md">Arduino</a>／<a href="/sensors/tof2/mblock/mblock5.md">mBlock</a>／<a href="/sensors/tof2/microbit/makecode.md">micro:bit</a>。</p>

## 範例

<p class="no-examples">這個軟體還沒有整支的範例程式。上面「讀值」每一項都附了可以直接照著做的寫法，照順序組起來就能用；需要整支範例請 <a href="/contact.html">聯絡我們</a>。</p>

## 其他問題



還有問題？到 [聯絡我們](/contact.md) 附上你用的軟體和程式截圖。

## 下載

- [下載中心（全部產品的函式庫、韌體、型錄）](/downloads.md)

## 本頁依據

- 主機與軟體清單：`_build/guides.json`
- 值與命令：`_build/protocol/tof2.json`（單一真相來源）
- 安裝與設定：`_build/guides/software/matrix-matrixblock.zh.md`、`_build/guides/product/tof2/matrix-matrixblock.zh.md`
