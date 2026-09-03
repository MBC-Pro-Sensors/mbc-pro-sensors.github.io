# 專案特定行為準則 (Project Rules)

## Antigravity Native Memory (專案自動記憶)
- 本專案根目錄下會有一個 `project_memory.md` 檔案，作為此專案的長期記憶日誌。
- **當你完成一個重大任務或階段性工作時**，你必須主動使用寫檔工具 (`write_to_file` 或 `multi_replace_file_content`) 將「本次修改的核心摘要（約 50 字內）」追加 (Append) 到該檔案的最末端。
- **絕對禁止** 在對話框中輸出 Markdown 讓使用者手動複製貼上。身為 Agent，你必須自己動手寫檔。
- 每次開始新任務前，必須優先讀取 `project_memory.md` 以了解專案歷史脈絡。

## GitHub Pages Deployment & Markdown Path Rules
When working on this repository (`mbc-pro-sensors.github.io`), always strictly follow these rules to prevent deployment errors and broken links on GitHub Pages:

1. **Case Sensitivity (Linux vs Windows):**
   - GitHub Pages runs on a Linux server, which means the filesystem is **strictly case-sensitive**.
   - If a folder is named `exp6`, all URLs, links, and image sources must be exactly `exp6` (e.g., `/sensors/exp6/index.md`).
   - Using uppercase (e.g., `EXP6`) in links will work locally on Windows but will result in a **404 Not Found** error when deployed to GitHub.

2. **Absolute Paths over Relative Paths for Assets:**
   - Docsify routes can sometimes cause relative paths like `../images/` or `../../images/` to break depending on the page depth or how the user arrived at the URL.
   - **ALWAYS use absolute paths from the root** for images, downloads, and examples.
   - ✅ Correct: `/images/sensors/exp6/exp6-product.webp`
   - ✅ Correct: `/downloads/MBC_EXP6_Official_App_Lib.zip`
   - ❌ Incorrect: `../images/...` or `../../downloads/...`

3. **Relative File Links:**
   - When linking between markdown pages, you may use absolute paths (e.g. `[link](/sensors/exp6/index.md)`).
   - If linking inside the same directory, simple relative links are acceptable (e.g. `[link](spike-pybricks.md)`) but always verify case sensitivity.

4. **Docsify Sidebar and Caching:**
   - `_sidebar.md` and page contents are heavily cached by Docsify and browsers.
   - If a user reports that a link is still pointing to an old location (like `ext6` instead of `exp6`) but you have verified via `git grep` that the old name no longer exists in the codebase, it is a caching issue. Instruct the user to wait 1-3 minutes for GitHub Actions to deploy and to perform a Hard Refresh (`Ctrl + F5`).
