<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（ps2 × matrix × matrixblock），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: ps2/matrix/matrixblock -->
# 掌控者 PS2 (PS2) × MATRIX × MATRIXblock 原廠積木

這一頁只講 **MATRIXblock 原廠顏色感應器積木（0x22）**：在 MATRIX Mini R4 上用它讀掌控者 PS2 (PS2)的安裝步驟、每個值怎麼讀、能用的命令與範例。要換產品、主機或軟體，用上方的切換列。

## 要買哪個版本、怎麼接

買 **MATRIX 版**，用 MATRIX 原廠 4 pin 感應器線接 Mini R4 的 I2C1 或 I2C2（位址 `0x22`），積木裡選對應的孔。

<figure class="ref-fig"><img src="/images/figures/ps2/wiring-matrix.svg" alt="掌控者 PS2 (PS2) × MATRIX Mini R4" loading="lazy"></figure>

## 安裝與設定

<!-- setup: _build/guides/software/matrix-matrixblock.zh.md + _build/guides/product/ps2/matrix-matrixblock.zh.md {"product":"掌控者 PS2 (PS2)","edition":"MATRIX 版","port":"I2C1","lib":"","libUrl":"","addr":"0x42"} -->
<!-- include-fallback -->
> [!NOTE]
> 這個軟體的安裝步驟正在搬到這一頁。先看原本的說明：[掌控者 PS2 (PS2)：MATRIXblock 原廠顏色感應器積木（0x22） 說明](/sensors/ps2/matrix.md)。
<!-- /include-fallback -->

## 讀值

用 MATRIXblock 原廠「顏色感應器」積木的 R／G／B／C／M／Y／K 讀（不要用 H／S／V 與顏色辨識積木）；Arduino 模式寫法一併列出。

<section id="v02" class="ref-item">

### 方向鍵 <small>dpad</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-dpad.svg" alt="方向鍵" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~15` bit | 上 1、右 2、下 4、左 8（同時按相加），0~15；未連線 = 0 |

積木「顏色感應器 → R」

```cpp
int dpad = MiniR4.I2C1.MXColor.getColor(R);   // 方向鍵
```

</section>

<section id="v04" class="ref-item">

### 肩鍵 <small>shoulder</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-shoulder.svg" alt="肩鍵" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~15` bit | L2 1、R2 2、L1 4、R1 8（同時按相加），0~15；未連線 = 0 |

積木「顏色感應器 → G」

```cpp
int shoulder = MiniR4.I2C1.MXColor.getColor(G);   // 肩鍵
```

</section>

<section id="v06" class="ref-item">

### 已連線 <small>connected</small>

| 範圍 | 意思 |
| :--- | :--- |
| `0~1` | 1 = 手把已連線（psx.read() 成功），0 = 沒有；未連線時所有按鍵 0、搖桿回中 |

積木「顏色感應器 → K」

```cpp
int connected = MiniR4.I2C1.MXColor.getColor(K);   // 已連線
```

</section>

<section id="v10" class="ref-item">

### 搖桿 LX／LY／RX／RY <small>stick</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-stick.svg" alt="搖桿 LX／LY／RX／RY" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-100~+100` | 10 LX、11 LY、12 RX、13 RY：−100 ~ +100，0 = 中間，右／上為正（原始 0~255 死區 ±7 吸成中心）；未連線 = 0。EV3／SPIKE／MATRIX 送的是 0~99（50 = 中間）或 1~9（5 = 中間），見各平台欄 |

積木「顏色感應器 → B~Y」：#67 新增：B、C、M、Y = LX、LY、RX、RY

```cpp
int raw = MiniR4.I2C1.MXColor.getColor(B);   // 第 1 組
```

</section>

<p class="ref-missing"><b>這個軟體讀不到</b>：十六鍵、形狀鍵、功能鍵、電量、L2 類比、R2 類比、系統鍵、手把類型。要用這些值請改用 <a href="/sensors/ps2/arduino/ide.md">Arduino</a>／<a href="/sensors/ps2/mblock/mblock5.md">mBlock</a>／<a href="/sensors/ps2/microbit/makecode.md">micro:bit</a>。</p>

## 命令

這個軟體不能送命令；需要時改用 Arduino I2C、mBlock 或 micro:bit。

## 範例

<p class="no-examples">這個軟體還沒有整支的範例程式。上面「讀值」和「命令」每一項都附了可以直接照著做的寫法，照順序組起來就能用；需要整支範例請 <a href="/contact.html">聯絡我們</a>。</p>

## 其他問題



還有問題？到 [聯絡我們](/contact.md) 附上你用的軟體和程式截圖。

## 下載

- [下載中心（全部產品的函式庫、韌體、型錄）](/downloads.md)

## 本頁依據

- 主機與軟體清單：`_build/guides.json`
- 值與命令：`_build/protocol/ps2.json`（單一真相來源）
- 安裝與設定：`_build/guides/software/matrix-matrixblock.zh.md`、`_build/guides/product/ps2/matrix-matrixblock.zh.md`
