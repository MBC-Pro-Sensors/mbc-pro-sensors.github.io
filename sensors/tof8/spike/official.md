<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（tof8 × spike × official），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: tof8/spike/official -->
# 測距者 8 路 (TOF8) × SPIKE × 樂高官方 App

這一頁只講 **樂高官方 App（SPIKE 3、Robot Inventor）**：在 SPIKE Prime／Robot Inventor 上用它讀測距者 8 路 (TOF8)的安裝步驟、每個值怎麼讀、能用的命令與範例。要換產品、主機或軟體，用上方的切換列。

## 安裝與設定

<!-- setup: _build/guides/software/spike-official.zh.md + _build/guides/product/tof8/spike-official.zh.md {"product":"測距者 8 路 (TOF8)","edition":"SPIKE 版","port":"C","lib":"MBC_laser8_obj_Lib.py","libUrl":"/examples/tof8/spike_beginner/pybricks/MBC_laser8_obj_Lib.py","addr":"0x29"} -->
<!-- include-fallback -->
> [!NOTE]
> 這個軟體的安裝步驟正在搬到這一頁。先看原本的說明：[測距者 8 路 (TOF8)：樂高官方 App（SPIKE 3、Robot Inventor） 說明](/sensors/tof8/spike-examples.md)。
<!-- /include-fallback -->

## 讀值

官方 App 會把感應器顯示成「顏色感應器」。用「更多感應器」分類裡的顏色、反射光、原始紅／綠／藍積木讀，再照表換算。

<section id="v10" class="ref-item">

### 第 1~8 顆距離 cm <small>distCm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-distCm.svg" alt="第 1~8 顆距離 cm" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~120` cm | 第 1~8 顆各自的距離 0~120 cm（第 1 顆正左、第 4、5 顆正前、第 8 顆正右，每顆隔 30°）；120 = 那個方向沒東西／那顆失效；編號 10~17 |

| 主機 | 值 | 積木（更多感應器 → 顏色感應器） | 換算 |
| :--- | :--- | :--- | :--- |
| SPIKE Prime（教育版） | 第 1 組 | 「顏色」 | 讀到的數 |
| SPIKE Prime（教育版） | 第 2 組 | 「反射光」 | 讀到的數 |
| SPIKE Prime（教育版） | 第 3 組 | 「原始紅」 | 讀到的數 ÷ 256 取餘數 |
| SPIKE Prime（教育版） | 第 4 組 | 「原始紅」 | 讀到的數 ÷ 256 取整數（無條件捨去） |
| SPIKE Prime（教育版） | 第 5 組 | 「原始綠」 | 讀到的數 ÷ 256 取餘數 |
| SPIKE Prime（教育版） | 第 6 組 | 「原始綠」 | 讀到的數 ÷ 256 取整數（無條件捨去） |
| SPIKE Prime（教育版） | 第 7 組 | 「原始藍」 | 讀到的數 ÷ 256 取餘數 |
| SPIKE Prime（教育版） | 第 8 組 | 「原始藍」 | 讀到的數 ÷ 256 取整數（無條件捨去） |
| Robot Inventor（玩具版） | 第 1 組 | 「反射光」 | 讀到的數 |
| Robot Inventor（玩具版） | 第 2 組 | 「顏色」 | 讀到的數 |

<p class="ref-note">韌體備註：16 B = [第 1~8 顆, 0×8]：App 顏色 = 第 1 顆、反射光 = 第 2 顆、原始紅 = 第 3+256×第 4 …；byte0/1 = 第 1、2 顆 ×10/120（0~10）；byte2~7 = 第 3~8 顆兩顆一組 4-bit 再查表編碼（spikeEncodeU8，INO:259-284）</p>

</section>

<p class="ref-missing"><b>這個軟體讀不到</b>：最近距離 cm、最近方向、最空曠距離 cm、最空曠方向、正前方 cm、最近距離 mm、初始化位元圖、故障位元圖、正前方距離 mm、第 1~8 顆距離 mm。要用這些值請改用 <a href="/sensors/tof8/spike/mbcblocks.md">MBC 積木網站</a>／<a href="/sensors/tof8/spike/pybricks.md">Pybricks</a>。</p>

## 範例

<!-- examples: sensors/tof8/spike-examples.md host=spike sw=official -->

## 其他問題

- **主機認不到感應器？** 拔掉重插一次，等 2 秒；換一個孔試試。官方 App 會把它顯示成「顏色感應器」，這是正常的。
- **可以同時用官方 App 和 Pybricks 嗎？** 一次只能裝一種韌體：Pybricks 與 MBC 積木網站用 Pybricks 韌體，官方 App 用樂高原廠韌體；隨時可以用 SPIKE App 換回原廠韌體。

還有問題？到 [聯絡我們](/contact.md) 附上你用的軟體和程式截圖。

## 下載

- [下載中心（全部產品的函式庫、韌體、型錄）](/downloads.md)

## 本頁依據

- 主機與軟體清單：`_build/guides.json`
- 值與命令：`_build/protocol/tof8.json`（單一真相來源）
- 安裝與設定：`_build/guides/software/spike-official.zh.md`、`_build/guides/product/tof8/spike-official.zh.md`
- 範例：`sensors/tof8/spike-examples.md`
