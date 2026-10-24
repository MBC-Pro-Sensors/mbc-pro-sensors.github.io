<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（ps4 × spike × official），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: ps4/spike/official -->
# 掌控者 PS4/PS5 × SPIKE × 樂高官方 App

這一頁只講 **樂高官方 App（SPIKE 3、Robot Inventor）**：在 SPIKE Prime／Robot Inventor 上用它讀掌控者 PS4/PS5的安裝步驟、每個值怎麼讀、能用的命令與範例。要換產品、主機或軟體，用上方的切換列。

## 安裝與設定

<!-- setup: _build/guides/software/spike-official.zh.md + _build/guides/product/ps4/spike-official.zh.md {"product":"掌控者 PS4/PS5","edition":"SPIKE 版","port":"C","lib":"MBC_gamepad_Lib.py","libUrl":"/examples/ps4/spike_beginner/pybricks/MBC_gamepad_Lib.py","addr":"0x42"} -->
<!-- include-fallback -->
> [!NOTE]
> 這個軟體的安裝步驟正在搬到這一頁。先看原本的說明：[掌控者 PS4/PS5：樂高官方 App（SPIKE 3、Robot Inventor） 說明](/sensors/ps4/spike-examples.md)。
<!-- /include-fallback -->

## 讀值

官方 App 會把感應器顯示成「顏色感應器」。用「更多感應器」分類裡的顏色、反射光、原始紅／綠／藍積木讀，再照表換算。

<section id="v01" class="ref-item">

### 十六鍵 <small>buttons16</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-buttons16.svg" alt="十六鍵" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~65535` bit | 全部按鍵一個 16 bit（PSX 排列）：bit0 SELECT(SHARE)、1 L3、2 R3、3 START(OPTIONS)、4 上、5 右、6 下、7 左、8 L2、9 R2、10 L1、11 R1、12 ▲、13 ●、14 ✕、15 ■；1 = 按下；未連線 = 0 |

| 主機 | 值 | 積木（更多感應器 → 顏色感應器） | 換算 |
| :--- | :--- | :--- | :--- |
| SPIKE Prime（教育版） | 十六鍵 | 「原始藍」 | 讀到的數 |

<p class="ref-note">韌體備註：PSX 排列（#65 已改，原本是 Bluepad32 排列）</p>

</section>

<section id="v02" class="ref-item">

### 方向鍵 <small>dpad</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-dpad.svg" alt="方向鍵" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~15` bit | 上 1、右 2、下 4、左 8，同時按相加（0~15） |

| 主機 | 值 | 積木（更多感應器 → 顏色感應器） | 換算 |
| :--- | :--- | :--- | :--- |
| SPIKE Prime（教育版） | 方向鍵 | 「反射光」 | 讀到的數 |
| Robot Inventor（玩具版） | 方向鍵 | 「反射光」 | 讀到的數 |

<p class="ref-note">韌體備註：低 nibble；高 nibble = 形狀鍵；低 nibble；高 nibble = 形狀鍵</p>

</section>

<section id="v03" class="ref-item">

### 形狀鍵 <small>shapeKeys</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-shapeKeys.svg" alt="形狀鍵" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~15` bit | ▲ 1、● 2、✕ 4、■ 8，同時按相加（0~15） |

| 主機 | 值 | 積木（更多感應器 → 顏色感應器） | 換算 |
| :--- | :--- | :--- | :--- |
| SPIKE Prime（教育版） | 形狀鍵 | 「反射光」 | 讀到的數 |
| Robot Inventor（玩具版） | 形狀鍵 | 「反射光」 | 讀到的數 |

<p class="ref-note">韌體備註：高 nibble；高 nibble</p>

</section>

<section id="v04" class="ref-item">

### 肩鍵 <small>shoulderKeys</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-shoulderKeys.svg" alt="肩鍵" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~15` bit | L2 1、R2 2、L1 4、R1 8，同時按相加（0~15） |

| 主機 | 值 | 積木（更多感應器 → 顏色感應器） | 換算 |
| :--- | :--- | :--- | :--- |
| SPIKE Prime（教育版） | 肩鍵 | 「顏色」 | 讀到的數 |

<p class="ref-note">韌體備註：低 nibble；高 nibble = 功能鍵</p>

</section>

<section id="v05" class="ref-item">

### 功能鍵 <small>funcKeys</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-funcKeys.svg" alt="功能鍵" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~15` bit | SELECT(SHARE) 1、L3 2、R3 4、START(OPTIONS) 8，同時按相加（0~15） |

| 主機 | 值 | 積木（更多感應器 → 顏色感應器） | 換算 |
| :--- | :--- | :--- | :--- |
| SPIKE Prime（教育版） | 功能鍵 | 「顏色」 | 讀到的數 |
| Robot Inventor（玩具版） | 功能鍵 | 「顏色」 | 讀到的數 |

<p class="ref-note">韌體備註：高 nibble；單鍵編號 1~8（掃 功能鍵<<4|肩鍵 的 bit0→7：1 L2 2 R2 3 L1 4 R1 5 SELECT 6 L3 7 R3 8 START）</p>

</section>

<section id="v10" class="ref-item">

### 搖桿 1~4 軸 <small>stick</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-stick.svg" alt="搖桿 1~4 軸" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-100~+100` | 10 LX、11 LY、12 RX、13 RY：−100 ~ +100，0 = 中間，右／上為正；未連線 0 |

| 主機 | 值 | 積木（更多感應器 → 顏色感應器） | 換算 |
| :--- | :--- | :--- | :--- |
| SPIKE Prime（教育版） | 搖桿 1~4 軸 | 「原始紅」 | 讀到的數 |
| Robot Inventor（玩具版） | 搖桿 1~4 軸 | 「原始紅」 | 讀到的數 |

<p class="ref-note">韌體備註：LX×10+LY（各 1~9，5 = 中間）；byte 4~5 = RX×10+RY；LX×10+LY 查表編碼；byte 4~5 = RX×10+RY；byte 6~7 = 肩鍵 byte 查表</p>

</section>

<p class="ref-missing"><b>這個軟體讀不到</b>：已連線、電量、L2 類比、R2 類比、系統鍵、手把類型。要用這些值請改用 <a href="/sensors/ps4/spike/pybricks.md">Pybricks</a>。</p>

## 命令

SPIKE 接頭是唯讀的，這個軟體不能送命令：校準用感應器上的按鈕，切黑白線、門檻等設定改用 Arduino I2C、mBlock 或 micro:bit。

## 範例

<!-- examples: sensors/ps4/spike-examples.md host=spike sw=official -->

## 其他問題

- **主機認不到感應器？** 拔掉重插一次，等 2 秒；換一個孔試試。官方 App 會把它顯示成「顏色感應器」，這是正常的。
- **可以同時用官方 App 和 Pybricks 嗎？** 一次只能裝一種韌體：Pybricks 與 MBC 積木網站用 Pybricks 韌體，官方 App 用樂高原廠韌體；隨時可以用 SPIKE App 換回原廠韌體。

還有問題？到 [聯絡我們](/contact.md) 附上你用的軟體和程式截圖。

## 下載

- [ps4_demo_robot_inventor_v10.5.0.lms](/examples/ps4/spike/ps4_demo_robot_inventor_v10.5.0.lms)
- [ps4_demo_spike_prime_v3.4.3.llsp3](/examples/ps4/spike/ps4_demo_spike_prime_v3.4.3.llsp3)
- [下載中心（全部產品的函式庫、韌體、型錄）](/downloads.md)

## 本頁依據

- 主機與軟體清單：`_build/guides.json`
- 值與命令：`_build/protocol/ps4.json`（單一真相來源）
- 安裝與設定：`_build/guides/software/spike-official.zh.md`、`_build/guides/product/ps4/spike-official.zh.md`
- 範例：`sensors/ps4/spike-examples.md`
