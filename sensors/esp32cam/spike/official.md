<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（esp32cam × spike × official），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: esp32cam/spike/official -->
# 神攝手 ESP32CAM × SPIKE × 樂高官方 App

這一頁只講 **樂高官方 App（SPIKE 3、Robot Inventor）**：在 SPIKE Prime／Robot Inventor 上用它讀神攝手 ESP32CAM的安裝步驟、每個值怎麼讀、能用的命令與範例。要換產品、主機或軟體，用上方的切換列。

## 安裝與設定

<!-- setup: _build/guides/software/spike-official.zh.md + _build/guides/product/esp32cam/spike-official.zh.md {"product":"神攝手 ESP32CAM","edition":"SPIKE 版","port":"C","lib":"MBC_cam_school_Lib.py","libUrl":"/examples/esp32cam/spike_beginner/pybricks/MBC_cam_school_Lib.py","addr":"0x4C"} -->
<!-- include-fallback -->
> [!NOTE]
> 這個軟體的安裝步驟正在搬到這一頁。先看原本的說明：[神攝手 ESP32CAM：樂高官方 App（SPIKE 3、Robot Inventor） 說明](/sensors/esp32cam/spike-examples.md)。
<!-- /include-fallback -->

## 讀值（學校版）

官方 App 會把感應器顯示成「顏色感應器」。用「更多感應器」分類裡的顏色、反射光、原始紅／綠／藍積木讀，再照表換算。

<section id="v01" class="ref-item">

### 色相 <small>hue</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-hue.svg" alt="色相" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-1~100` | S1 取樣點色相 0~100（色相環 0~240 等比縮放）；−1 = 黑白灰（飽和度 < 10）；S1 沒設定 = 0 |

| 主機 | 值 | 積木（更多感應器 → 顏色感應器） | 換算 |
| :--- | :--- | :--- | :--- |
| SPIKE Prime（教育版） | 色相 | 「顏色」 | 讀到的數 |
| Robot Inventor（玩具版） | 色相 | 「顏色」 | 讀到的數 |

<p class="ref-note">韌體備註：−1 = 0xFF；0~10（÷10）</p>

</section>

<section id="v04" class="ref-item">

### 動態 <small>motion</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-motion.svg" alt="動態" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~100` | 全畫面動態量 0~100（畫面變化越多越大） |

| 主機 | 值 | 積木（更多感應器 → 顏色感應器） | 換算 |
| :--- | :--- | :--- | :--- |
| SPIKE Prime（教育版） | 動態 | 「反射光」 | 讀到的數 |
| Robot Inventor（玩具版） | 動態 | 「反射光」 | 讀到的數 |

</section>

<section id="v05" class="ref-item">

### 色塊 X <small>blobX</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobX.svg" alt="色塊 X" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~100` | 色塊中心 X 0~100，左 = 0；沒找到 = 0 |

| 主機 | 值 | 積木（更多感應器 → 顏色感應器） | 換算 |
| :--- | :--- | :--- | :--- |
| SPIKE Prime（教育版） | 色塊 X | 「原始紅」 | 讀到的數 ÷ 256 取餘數 |
| Robot Inventor（玩具版） | 色塊 X | 「原始紅」 | 讀到的數 |

<p class="ref-note">韌體備註：byte 3 = 0；×4 查表編碼（10-bit）</p>

</section>

<section id="v06" class="ref-item">

### 色塊 Y <small>blobY</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobY.svg" alt="色塊 Y" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~75` | 色塊中心 Y 0~75，下 = 0、上 = 75；沒找到 = 0 |

| 主機 | 值 | 積木（更多感應器 → 顏色感應器） | 換算 |
| :--- | :--- | :--- | :--- |
| SPIKE Prime（教育版） | 色塊 Y | 「原始綠」 | 讀到的數 ÷ 256 取餘數 |
| Robot Inventor（玩具版） | 色塊 Y | 「原始綠」 | 讀到的數 |

<p class="ref-note">韌體備註：byte 5 = 0；×4 查表編碼</p>

</section>

<section id="v09" class="ref-item">

### 色塊面積 <small>blobArea</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobArea.svg" alt="色塊面積" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~7500` | W×H（0~7500）；沒找到 = 0 |

| 主機 | 值 | 積木（更多感應器 → 顏色感應器） | 換算 |
| :--- | :--- | :--- | :--- |
| SPIKE Prime（教育版） | 色塊面積 | 「原始藍」 | 讀到的數 ÷ 256 取餘數 |
| Robot Inventor（玩具版） | 色塊面積 | 「原始藍」 | 讀到的數 |

<p class="ref-note">韌體備註：鎖住：這裡是 map(像素面積, 0~5000, 0~100)，不是 W×H；byte 7 = 0；同上 0~100 再 ×4 查表編碼</p>

</section>

<p class="ref-missing"><b>這個軟體讀不到</b>：飽和度、明度、色塊寬、色塊高、有找到色塊。要用這些值請改用 <a href="/sensors/esp32cam/spike/mbcblocks.md">MBC 積木網站</a>／<a href="/sensors/esp32cam/spike/pybricks.md">Pybricks</a>。</p>

## 讀值（WRO 版）

官方 App 會把感應器顯示成「顏色感應器」。用「更多感應器」分類裡的顏色、反射光、原始紅／綠／藍積木讀，再照表換算。

<p class="ref-missing"><b>這個軟體讀不到</b>：色點 1~12 顏色碼、色點 1~12 鮮豔度。要用這些值請改用 <a href="/sensors/esp32cam/spike/pybricks.md">Pybricks</a>。</p>

## 範例（學校版）

<!-- examples: sensors/esp32cam/spike-examples.md host=spike sw=official -->

## 範例（WRO 版）

<!-- examples: sensors/esp32cam/spike-examples-wro.md host=spike sw=official -->

## 其他問題

- **主機認不到感應器？** 拔掉重插一次，等 2 秒；換一個孔試試。官方 App 會把它顯示成「顏色感應器」，這是正常的。
- **可以同時用官方 App 和 Pybricks 嗎？** 一次只能裝一種韌體：Pybricks 與 MBC 積木網站用 Pybricks 韌體，官方 App 用樂高原廠韌體；隨時可以用 SPIKE App 換回原廠韌體。

還有問題？到 [聯絡我們](/contact.md) 附上你用的軟體和程式截圖。

## 下載

- [下載中心（全部產品的函式庫、韌體、型錄）](/downloads.md)

## 本頁依據

- 主機與軟體清單：`_build/guides.json`
- 值與命令：`_build/protocol/cam_school.json`、`_build/protocol/cam_wro.json`（單一真相來源）
- 安裝與設定：`_build/guides/software/spike-official.zh.md`、`_build/guides/product/esp32cam/spike-official.zh.md`
- 範例：`sensors/esp32cam/spike-examples(-wro).md`
