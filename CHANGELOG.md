# 更新日誌 (Changelog)

所有針對本專案文件與網站架構的重大變更，都會記錄於此檔案中。

---

## [2026-07-28] - EXP6 模組完整上線與全站穩定性升級

### 🎉 新增 (Added)
- **EXP6 模組官方與 Pybricks 雙生態教學**：
  - 完整新增 EXP6 六路擴充板的「官方教學」與「Pybricks 專業環境」說明。
  - 加入 HTML 版面配置、展示圖片、與 ZIP 下載包（含安裝/解除安裝腳本、積木/Python 範例程式）。
  - 中文版與英文版同步上線，並在英文版 (`en/sensors/exp6`) 完成所有翻譯。
- **環境設定檔 (`.agents/AGENTS.md`)**：
  - 新增 AI 協作開發規則，強制記錄 GitHub Pages 的嚴格大小寫規範以及使用絕對路徑以防破圖的教訓。

### 🔄 變更 (Changed)
- **重要限制說明提醒**：
  - 於中文版與英文版 `exp6/index.md` 首頁加入「⚠️ 重要限制說明」警告區塊，明確標示僅支援 SPIKE Prime 主機及 Python 文字模式（不支援圖控積木）。

### 🛠️ 修復 (Fixed)
- **全站資源路徑升級（防破圖修復）**：
  - 執行全站（包含 Line8, Line16, TOF, PS4 等）的資源路徑升級，將可能因為 Docsify 路由深度不同而導致 404 破圖的相對路徑（如 `../images/`）全面轉換為絕對根目錄路徑（`/images/`、`/downloads/`、`/examples/`）。
- **網址大小寫相容性修復**：
  - 修正 `README.md`、`en/README.md` 及首頁選單中的路徑，將大寫的 `EXP6` 網址路徑修正為小寫 `exp6`，解決 GitHub Pages（Linux 環境）嚴格區分大小寫所導致的 404 找不到網頁的問題。
- **清除重複亂碼片段**：
  - 修復 `sensors/exp6/spike-pybricks.md` 中因為指令重疊覆寫所殘留的重複 HTML 標籤與 `get_ul般模式` 亂碼問題。
