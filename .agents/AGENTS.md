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

4. **Sidebar and Caching:**
   - `_sidebar.md` and page contents are cached by browsers and the GitHub Pages CDN.
   - If a user reports that a link is still pointing to an old location (like `ext6` instead of `exp6`) but you have verified via `git grep` that the old name no longer exists in the codebase, it is a caching issue. Instruct the user to wait 1-3 minutes for GitHub Actions to deploy and to perform a Hard Refresh (`Ctrl + F5`).

## Static Site Build (since 2026-10-07 — Docsify is no longer used in production)
- The site is deployed by `.github/workflows/deploy.yml`: on every push to `main`, `_build/build.mjs` renders each `.md` page into its own HTML page in `_site/` and GitHub Pages serves that. Never commit `_site/`.
- URL scheme: `README.md` -> `/`, `en/README.md` -> `/en/`, `sensors/x/index.md` -> `/sensors/x/`, `sensors/x/foo.md` -> `/sensors/x/foo.html`, `contact.md` -> `/contact.html`. Old Docsify links (`/#/sensors/...`) and old `.md` URLs auto-redirect.
- Keep writing pages exactly as before (Markdown + raw HTML, links like `/sensors/line8/index.md`); the build rewrites links to the static URLs.
- **Raw HTML in Markdown must be balanced** (every `<div>`/`<section>` closed). Docsify used to hide broken tags; static pages will show broken layout. The build prints `WARN ... <div> vs </div>` — fix any warning before pushing.
- New page: create the `.md` file and add it to `_sidebar.md` (and `en/_sidebar.md`). Add the English twin at `en/<same path>` so the language toggle and hreflang link them.
- Page `<title>` comes from the page's first H1; the meta description from the first paragraph after the H1. Home page metadata and JSON-LD live in `_build/home.json`. Shared CSS/JS: `_build/assets/site.css`, `_build/assets/site.js`.
- Product share images (1200x630): `images/brand/og/<product>.jpg`, used automatically for that product's pages.
- Local preview: `cd _build && npm ci && node build.mjs`, then `python -m http.server 8000 -d _site` and open http://localhost:8000/

## Sales Layer (since 2026-10-07)
- Home pages (`README.md`, `en/README.md`) start with `<!-- layout: landing -->`: full-width landing layout with a top nav and no sidebar. Content is raw HTML using `lp-*` classes (styles in `_build/assets/site.css`). Keep all raw HTML un-indented.
- Product pages: the line `<!-- product-hero -->` right after the H1 is replaced at build time by the sales block (badge, photo, video, pitch, 3 key numbers, edition buttons, LINE CTA). Edit that content in `_build/products.json`, not in the Markdown. The pitch is also the page's meta description.
- Every page gets a LINE inquiry button (bottom bar on mobile). Buying guide: `guide.md` / `en/guide.md`.
- Never publish internal chip/part numbers (MCU, sensor IC, camera sensor) on the site.
- Social-proof section placeholder is an HTML comment in both READMEs; add competition results / testimonials there only from real material supplied by the owner.

## Articles & Analytics
- Tutorial / SEO articles live in `articles/*.md` (index: `articles/index.md`), get `Article` JSON-LD and breadcrumbs automatically. Add new ones to `_sidebar.md` and the article index cards. Write for real search questions (parents, coaches, students); link to the relevant product page and the buying guide; no competitor-brand comparisons.
- Google Analytics: put the GA4 measurement ID (G-...) in `_build/site-config.json`. Empty = no tracking. `site.js` sends `line_inquiry`, `email_inquiry` and `file_download` events.
