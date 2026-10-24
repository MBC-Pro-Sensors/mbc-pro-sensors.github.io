<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（servo6 × spike × official），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: servo6/spike/official -->
# SPIKE 6 路舵機擴充器 (Servo6) × SPIKE × 樂高官方 App

這一頁只講 **樂高官方 App（SPIKE 3、Robot Inventor）**：在 SPIKE Prime／Robot Inventor 上用它讀SPIKE 6 路舵機擴充器 (Servo6)的安裝步驟、每個值怎麼讀與範例。要換產品、主機或軟體，用上方的切換列。

## 安裝與設定

<!-- setup: _build/guides/software/spike-official.zh.md + _build/guides/product/servo6/spike-official.zh.md {"product":"SPIKE 6 路舵機擴充器 (Servo6)","edition":"SPIKE 版","port":"A","lib":"","libUrl":"","addr":""} -->
<!-- include-fallback -->
> [!NOTE]
> 這個軟體的安裝步驟正在搬到這一頁。先看原本的說明：[SPIKE 6 路舵機擴充器 (Servo6)：樂高官方 App（SPIKE 3、Robot Inventor） 說明](/sensors/servo6/spike-official.md)。
<!-- /include-fallback -->

## 範例

<p class="no-examples">這個軟體的範例程式還在準備中，會跟產品一起上線；需要的話請先 <a href="/contact.html">聯絡我們</a>。</p>

## 其他問題

- **主機認不到感應器？** 拔掉重插一次，等 2 秒；換一個孔試試。官方 App 會把它顯示成「顏色感應器」，這是正常的。
- **可以同時用官方 App 和 Pybricks 嗎？** 一次只能裝一種韌體：Pybricks 與 MBC 積木網站用 Pybricks 韌體，官方 App 用樂高原廠韌體；隨時可以用 SPIKE App 換回原廠韌體。

還有問題？到 [聯絡我們](/contact.md) 附上你用的軟體和程式截圖。

## 下載

- [MBC_Servo6_Official_App_Lib.zip](/downloads/MBC_Servo6_Official_App_Lib.zip)
- [下載中心（全部產品的函式庫、韌體、型錄）](/downloads.md)

## 本頁依據

- 主機與軟體清單：`_build/guides.json`
- 安裝與設定：`_build/guides/software/spike-official.zh.md`、`_build/guides/product/servo6/spike-official.zh.md`
- 範例：`sensors/servo6/spike-examples.md`
