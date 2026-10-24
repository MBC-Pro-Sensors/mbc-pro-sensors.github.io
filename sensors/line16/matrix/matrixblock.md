<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（line16 × matrix × matrixblock），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: line16/matrix/matrixblock -->
# 循行者 16 路 (Line16) × MATRIX × MATRIXblock 原廠積木

這一頁只講 **MATRIXblock 原廠顏色感應器積木（0x22）**：在 MATRIX Mini R4 上用它讀循行者 16 路 (Line16)的安裝步驟、每個值怎麼讀、能用的命令與範例。要換產品、主機或軟體，用上方的切換列。

## 安裝與設定

<!-- setup: _build/guides/software/matrix-matrixblock.zh.md + _build/guides/product/line16/matrix-matrixblock.zh.md {"product":"循行者 16 路 (Line16)","edition":"MATRIX 版","port":"I2C1","lib":"","libUrl":"","addr":"0x16"} -->
<!-- include-fallback -->
> [!NOTE]
> 這個軟體的安裝步驟正在搬到這一頁。先看原本的說明：[循行者 16 路 (Line16)：MATRIXblock 原廠顏色感應器積木（0x22） 說明](/sensors/line16/matrix.md)。
<!-- /include-fallback -->

## 讀值

用 MATRIXblock 原廠「顏色感應器」積木的 R／G／B／C／M／Y／K 讀（不要用 H／S／V 與顏色辨識積木）；Arduino 模式寫法一併列出。

<section id="v01" class="ref-item">

### 線位置 <small>linePos</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-linePos.svg" alt="線位置" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-16~+16` | 線在哪一路：−16（最左）~+16（最右），0 = 正中間；站在車後往前看，負 = 線在左（平常 ±15，±16 只在最外側掉線後 200 ms 內） |

積木「顏色感應器 → R」，讀到的數減 16

```cpp
int linePos = MiniR4.I2C1.MXColor.getColor(R) - 16;   // 線位置
```

</section>

<section id="v02" class="ref-item">

### 細位置 <small>linePosFine</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-linePosFine.svg" alt="細位置" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-100~+100` | 線位置的細分：−100 ~ +100，0 = 正中間，負 = 線在左（與線位置同號） |

積木「顏色感應器 → B」，讀到的數減 100

```cpp
int linePosFine = MiniR4.I2C1.MXColor.getColor(B) - 100;   // 細位置
```

</section>

<section id="v03" class="ref-item">

### 線寬 <small>lineWidth</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-lineWidth.svg" alt="線寬" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~16` 路 | 壓到線的路數 0~16，0 = 找不到線 |

積木「顏色感應器 → G」

```cpp
int lineWidth = MiniR4.I2C1.MXColor.getColor(G);   // 線寬
```

</section>

<section id="v10" class="ref-item">

### 第 1~16 路光值 <small>light</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-light.svg" alt="第 1~16 路光值" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~100` | 第 1~16 路校準後光值 0~100（第 1 路最左）；編號 10~1F |

積木「顏色感應器 → C~K」：C~K 四路一組，每路 2 bit（convert3：0~24→0、25~49→1、50~74→2、75~100→3）：第 1 路在 bit7-6 … 第 4 路在 bit1-0；C = 第 1~4 路、M = 5~8、Y = 9~12、K = 13~16

```cpp
int raw = MiniR4.I2C1.MXColor.getColor(C);   // 第 1 組
```

</section>

<p class="ref-missing"><b>這個軟體讀不到</b>：線段數、壓線位元圖、最後離線方向、二值化門檻、校準完成次數。要用這些值請改用 <a href="/sensors/line16/matrix/bigchannel.md">大通道函式庫</a>。</p>

## 命令

這個軟體不能送命令；需要時改用 Arduino I2C、mBlock 或 micro:bit。

## 其他問題

- **原廠積木和大通道可以一起用嗎？** 不行：兩種要裝不同的函式庫檔，一個程式只能選一種。

還有問題？到 [聯絡我們](/contact.md) 附上你用的軟體和程式截圖。

## 下載

- [下載中心（全部產品的函式庫、韌體、型錄）](/downloads.md)

## 本頁依據

- 主機與軟體清單：`_build/guides.json`
- 值與命令：`_build/protocol/line16.json`（單一真相來源）
- 安裝與設定：`_build/guides/software/matrix-matrixblock.zh.md`、`_build/guides/product/line16/matrix-matrixblock.zh.md`
