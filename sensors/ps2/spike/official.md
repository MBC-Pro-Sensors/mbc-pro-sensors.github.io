<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（ps2 × spike × official），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: ps2/spike/official -->
# 掌控者 PS2 (PS2) × SPIKE × 樂高官方 App

這一頁只講 **樂高官方 App（SPIKE 3、Robot Inventor）**：在 SPIKE Prime／Robot Inventor 上用它讀掌控者 PS2 (PS2)的安裝步驟、每個值怎麼讀、能用的命令與範例。要換產品、主機或軟體，用上方的切換列。

## 安裝與設定

<!-- setup: _build/guides/software/spike-official.zh.md + _build/guides/product/ps2/spike-official.zh.md {"product":"掌控者 PS2 (PS2)","edition":"SPIKE 版","port":"C","lib":"MBC_gamepad_Lib.py","libUrl":"/examples/ps2/spike_beginner/pybricks/MBC_gamepad_Lib.py","addr":"0x42"} -->
<!-- include-fallback -->
> [!NOTE]
> 這個軟體的安裝步驟正在搬到這一頁。先看原本的說明：[掌控者 PS2 (PS2)：樂高官方 App（SPIKE 3、Robot Inventor） 說明](/sensors/ps2/spike-examples.md)。
<!-- /include-fallback -->

## 讀值

官方 App 會把感應器顯示成「顏色感應器」。用「更多感應器」分類裡的顏色、反射光、原始紅／綠／藍積木讀，再照表換算。

<section id="v01" class="ref-item">

### 十六鍵 <small>buttons16</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-buttons16.svg" alt="十六鍵" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~65535` bit | 16 個按鍵合成一個數（PSX 排列，1 = 按下）：bit0 SELECT、1 L3、2 R3、3 START、4 上、5 右、6 下、7 左、8 L2、9 R2、10 L1、11 R1、12 ▲、13 ●、14 ✕、15 ■；未連線 = 0 |

| 主機 | 值 | 積木（更多感應器 → 顏色感應器） | 換算 |
| :--- | :--- | :--- | :--- |
| SPIKE Prime（教育版） | 十六鍵 | 「原始藍」 | 讀到的數 |

<p class="ref-note">韌體備註：App 原始藍 = button16（.ino:201-211）</p>

</section>

<section id="v02" class="ref-item">

### 方向鍵 <small>dpad</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-dpad.svg" alt="方向鍵" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~15` bit | 上 1、右 2、下 4、左 8（同時按相加），0~15；未連線 = 0 |

| 主機 | 值 | 積木（更多感應器 → 顏色感應器） | 換算 |
| :--- | :--- | :--- | :--- |
| SPIKE Prime（教育版） | 方向鍵 | 「反射光」 | 讀到的數 |
| Robot Inventor（玩具版） | 方向鍵 | 「反射光」 | 讀到的數 |

<p class="ref-note">韌體備註：byte1 = 方向 | 形狀<<4（App 反射光）：低 4 bit；byte0 = 方向 | 形狀<<4</p>

</section>

<section id="v03" class="ref-item">

### 形狀鍵 <small>shape</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-shape.svg" alt="形狀鍵" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~15` bit | ▲ 1、● 2、✕ 4、■ 8（同時按相加），0~15；未連線 = 0；0x22 的 R 高 4 bit |

| 主機 | 值 | 積木（更多感應器 → 顏色感應器） | 換算 |
| :--- | :--- | :--- | :--- |
| SPIKE Prime（教育版） | 形狀鍵 | 「反射光」 | 讀到的數 |
| Robot Inventor（玩具版） | 形狀鍵 | 「反射光」 | 讀到的數 |

<p class="ref-note">韌體備註：高 4 bit；高 4 bit</p>

</section>

<section id="v04" class="ref-item">

### 肩鍵 <small>shoulder</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-shoulder.svg" alt="肩鍵" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~15` bit | L2 1、R2 2、L1 4、R1 8（同時按相加），0~15；未連線 = 0 |

| 主機 | 值 | 積木（更多感應器 → 顏色感應器） | 換算 |
| :--- | :--- | :--- | :--- |
| SPIKE Prime（教育版） | 肩鍵 | 「顏色」 | 讀到的數 |
| Robot Inventor（玩具版） | 肩鍵 | 「原始藍」 | 讀到的數 ÷ 256 取餘數（玩具版查表編碼，解法見上方安裝與設定） |

<p class="ref-note">韌體備註：byte0 = 肩 | 功能<<4（App 顏色，START = −128）：低 4 bit；byte6~7 = (肩 | 功能<<4) 經玩具版查表編碼；byte1 = 單鍵編號（L2=1 R2=2 L1=3 R1=4 SELECT=5 L3=6 R3=7 START=8，沒按 0）</p>

</section>

<section id="v05" class="ref-item">

### 功能鍵 <small>func</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-func.svg" alt="功能鍵" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~15` bit | SELECT 1、L3 2、R3 4、START 8（同時按相加），0~15；未連線 = 0；0x22 的 G 高 4 bit |

| 主機 | 值 | 積木（更多感應器 → 顏色感應器） | 換算 |
| :--- | :--- | :--- | :--- |
| SPIKE Prime（教育版） | 功能鍵 | 「顏色」 | 讀到的數 |

<p class="ref-note">韌體備註：高 4 bit</p>

</section>

<section id="v10" class="ref-item">

### 搖桿 LX／LY／RX／RY <small>stick</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-stick.svg" alt="搖桿 LX／LY／RX／RY" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-100~+100` | 10 LX、11 LY、12 RX、13 RY：−100 ~ +100，0 = 中間，右／上為正（原始 0~255 死區 ±7 吸成中心）；未連線 = 0。EV3／SPIKE／MATRIX 送的是 0~99（50 = 中間）或 1~9（5 = 中間），見各平台欄 |

| 主機 | 值 | 積木（更多感應器 → 顏色感應器） | 換算 |
| :--- | :--- | :--- | :--- |
| SPIKE Prime（教育版） | 第 1 組 | 「原始紅」 | 讀到的數 |
| SPIKE Prime（教育版） | 第 2 組 | 「原始綠」 | 讀到的數 |
| Robot Inventor（玩具版） | 第 1 組 | 「原始紅」 | 讀到的數 ÷ 256 取餘數（玩具版查表編碼，解法見上方安裝與設定） |
| Robot Inventor（玩具版） | 第 2 組 | 「原始紅」 | 讀到的數 ÷ 256 取整數（無條件捨去）（玩具版查表編碼，解法見上方安裝與設定） |

<p class="ref-note">韌體備註：byte2~3 = LX×10+LY、byte4~5 = RX×10+RY（11~99，中心 55；App 原始紅／綠）；LX×10+LY、RX×10+RY 經玩具版查表編碼成 byte2~3、byte4~5</p>

</section>

<p class="ref-missing"><b>這個軟體讀不到</b>：已連線、電量、L2 類比、R2 類比、系統鍵、手把類型。要用這些值請改用 <a href="/sensors/ps2/spike/pybricks.md">Pybricks</a>。</p>

## 命令

SPIKE 接頭是唯讀的，這個軟體不能送命令：校準用感應器上的按鈕，切黑白線、門檻等設定改用 Arduino I2C、mBlock 或 micro:bit。

## 範例

<!-- examples: sensors/ps2/spike-examples.md host=spike sw=official -->

## 其他問題

- **主機認不到感應器？** 拔掉重插一次，等 2 秒；換一個孔試試。官方 App 會把它顯示成「顏色感應器」，這是正常的。
- **可以同時用官方 App 和 Pybricks 嗎？** 一次只能裝一種韌體：Pybricks 與 MBC 積木網站用 Pybricks 韌體，官方 App 用樂高原廠韌體；隨時可以用 SPIKE App 換回原廠韌體。

還有問題？到 [聯絡我們](/contact.md) 附上你用的軟體和程式截圖。

## 下載

- [下載中心（全部產品的函式庫、韌體、型錄）](/downloads.md)

## 本頁依據

- 主機與軟體清單：`_build/guides.json`
- 值與命令：`_build/protocol/ps2.json`（單一真相來源）
- 安裝與設定：`_build/guides/software/spike-official.zh.md`、`_build/guides/product/ps2/spike-official.zh.md`
- 範例：`sensors/ps2/spike-examples.md`
