<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（line16 × spike × official），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: line16/spike/official -->
# 循行者 16 路 (Line16) × SPIKE × 樂高官方 App

這一頁只講 **樂高官方 App（SPIKE 3、Robot Inventor）**：在 SPIKE Prime／Robot Inventor 上用它讀循行者 16 路 (Line16)的安裝步驟、每個值怎麼讀、能用的命令與範例。要換產品、主機或軟體，用上方的切換列。

## 安裝與設定

<!-- setup: _build/guides/software/spike-official.zh.md + _build/guides/product/line16/spike-official.zh.md {"product":"循行者 16 路 (Line16)","edition":"SPIKE 版","port":"C","lib":"MBC_line16_obj_Lib.py","libUrl":"/examples/line16/spike_beginner/pybricks/MBC_line16_obj_Lib.py","addr":"0x16"} -->
<!-- include-fallback -->
> [!NOTE]
> 這個軟體的安裝步驟正在搬到這一頁。先看原本的說明：[循行者 16 路 (Line16)：樂高官方 App（SPIKE 3、Robot Inventor） 說明](/sensors/line16/spike-education.md)。
<!-- /include-fallback -->

## 讀值

官方 App 會把感應器顯示成「顏色感應器」。用「更多感應器」分類裡的顏色、反射光、原始紅／綠／藍積木讀，再照表換算。

<section id="v01" class="ref-item">

### 線位置 <small>linePos</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-linePos.svg" alt="線位置" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-16~+16` | 線在哪一路：−16（最左）~+16（最右），0 = 正中間；站在車後往前看，負 = 線在左（平常 ±15，±16 只在最外側掉線後 200 ms 內） |

| 主機 | 值 | 積木（更多感應器 → 顏色感應器） | 換算 |
| :--- | :--- | :--- | :--- |
| SPIKE Prime（教育版） | 線位置 | 「反射光」 | 讀到的數，再減 16 |
| Robot Inventor（玩具版） | 線位置 | 「反射光」 | 讀到的數，再減 16 |

</section>

<section id="v02" class="ref-item">

### 細位置 <small>linePosFine</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-linePosFine.svg" alt="細位置" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-100~+100` | 線位置的細分：−100 ~ +100，0 = 正中間，負 = 線在左（與線位置同號） |

| 主機 | 值 | 積木（更多感應器 → 顏色感應器） | 換算 |
| :--- | :--- | :--- | :--- |
| SPIKE Prime（教育版） | 細位置 | 「原始紅」 | 讀到的數，再減 100 |
| Robot Inventor（玩具版） | 細位置 | 「原始紅」 | 讀到的數 ÷ 256 取餘數，再減 100（玩具版查表編碼，解法見上方安裝與設定） |

<p class="ref-note">韌體備註：byte2 = 細位置+100（0~200）、byte3 = 0；App 原始紅讀成 int16；細位置+100 經玩具版查表編碼成 byte2~3</p>

</section>

<section id="v03" class="ref-item">

### 線寬 <small>lineWidth</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-lineWidth.svg" alt="線寬" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~16` 路 | 壓到線的路數 0~16，0 = 找不到線 |

| 主機 | 值 | 積木（更多感應器 → 顏色感應器） | 換算 |
| :--- | :--- | :--- | :--- |
| SPIKE Prime（教育版） | 線寬 | 「顏色」 | 讀到的數 |
| Robot Inventor（玩具版） | 線寬 | 「顏色」 | 讀到的數 |

<p class="ref-note">韌體備註：min(線寬, 10)</p>

</section>

<section id="v10" class="ref-item">

### 第 1~16 路光值 <small>light</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-light.svg" alt="第 1~16 路光值" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~100` | 第 1~16 路校準後光值 0~100（第 1 路最左）；編號 10~1F |

| 主機 | 值 | 積木（更多感應器 → 顏色感應器） | 換算 |
| :--- | :--- | :--- | :--- |
| SPIKE Prime（教育版） | 第 1~16 路光值 | 「原始綠」 | 讀到的數 |
| Robot Inventor（玩具版） | 第 1~16 路光值 | 「原始綠」 | 讀到的數 ÷ 256 取餘數（玩具版查表編碼，解法見上方安裝與設定） |

<p class="ref-note">韌體備註：每路 2 bit（(v×10)>>8：0~25→0、26~51→1、52~76→2、77~100→3）；byte4 = 第 5~8 路（第 5 路在 bit7-6）、byte5 = 第 1~4 路、byte6 = 第 13~16 路、byte7 = 第 9~12 路 → App 原始綠 = 第 1~8 路（bit15-14 = 第 1 路）、原始藍 = 第 9~16 路；byte 8~15 = 0；每路 1 bit（光值 > 50 = 1 亮）：第 1~8 路合成 1 byte（第 1 路在 bit7）經玩具版查表編碼成 byte4~5；第 9~16 路 → byte6~7</p>

</section>

<p class="ref-missing"><b>這個軟體讀不到</b>：線段數、壓線位元圖、最後離線方向、二值化門檻、校準完成次數。要用這些值請改用 <a href="/sensors/line16/spike/mbcblocks.md">MBC 積木網站</a>／<a href="/sensors/line16/spike/pybricks.md">Pybricks</a>。</p>

## 命令

這個軟體送不了命令（官方 SPIKE App 3、Pybricks 內建 ColorSensor 積木都不行）。要下命令請用 [Pybricks＋MBC 函式庫](/sensors/line16/spike/pybricks.md)；校準也可以用感應器上的按鈕。

## 範例

<!-- examples: sensors/line16/spike-examples.md host=spike sw=official -->

## 其他問題

- **主機認不到感應器？** 拔掉重插一次，等 2 秒；換一個孔試試。官方 App 會把它顯示成「顏色感應器」，這是正常的。
- **可以同時用官方 App 和 Pybricks 嗎？** 一次只能裝一種韌體：Pybricks 與 MBC 積木網站用 Pybricks 韌體，官方 App 用樂高原廠韌體；隨時可以用 SPIKE App 換回原廠韌體。

還有問題？到 [聯絡我們](/contact.md) 附上你用的軟體和程式截圖。

## 下載

- [line16-demo.llsp3](/examples/line16/spike/line16-demo.llsp3)
- [line16-demo.lms](/examples/line16/spike/line16-demo.lms)
- [下載中心（全部產品的函式庫、韌體、型錄）](/downloads.md)

## 本頁依據

- 主機與軟體清單：`_build/guides.json`
- 值與命令：`_build/protocol/line16.json`（單一真相來源）
- 安裝與設定：`_build/guides/software/spike-official.zh.md`、`_build/guides/product/line16/spike-official.zh.md`
- 範例：`sensors/line16/spike-examples.md`
