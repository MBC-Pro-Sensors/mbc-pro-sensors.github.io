# 說明頁三層導覽：產品 → 主機平台 → 軟體（2026-10-22 使用者要求）

## 問題
現在每款產品的說明散在十幾頁（index、reference、spike-hub、spike-examples、ev3-examples、arduino-i2c、arduino-examples、mblock、mblock-examples、matrix、microbit、microbit-examples…），每頁又把 6 個平台、10 幾種軟體全部攤開，使用者每到一頁都要重新往下找自己的平台和軟體。

## 目標
使用者只選三次，之後整頁都是「他的產品 × 他的平台 × 他的軟體」：

1. **產品頁** `/sensors/<p>/`：介紹、規格、校準、指示燈 → 大格子「① 選你的主機」（只列這款有的平台）。
2. **平台頁** `/sensors/<p>/<host>/`：這個平台要買哪個版本、怎麼接（接線圖）→ 大格子「② 選你用的軟體」。平台只有一種軟體時，產品頁的格子直接連到第 3 層，不經過平台頁。
3. **軟體頁** `/sensors/<p>/<host>/<sw>.html`：一頁到底，只講這個軟體：
   - 安裝與設定（裝軟體、裝函式庫／擴充、插哪個孔、選哪個模式）
   - 讀值：每個值一節（示意圖＋範圍單位＋「在這個軟體怎麼讀」，只有這個軟體的積木或程式碼）
   - 命令（校準、切黑白線、震動…，這個軟體能用的才列）
   - 5 個範例（只留這個軟體的積木圖或程式碼，預設展開）
   - 常見問題、下載（這個軟體用的檔案）

每個第 2、3 層頁面頂端固定一條**切換列**：`循行者 8 路 ▾ › SPIKE ▾ › Pybricks ▾`。每一段是下拉選單，換其中一段時另外兩段盡量保留（換產品時保留同平台同軟體，沒有就退到平台頁）。選過的平台與軟體記在 localStorage（`mbc.host`、`mbc.sw`），之後打開任何產品頁，第一格出現「繼續用 SPIKE › Pybricks →」。

首頁選擇器改成同樣三步（產品 → 主機 → 軟體），直接進第 3 層。

## 主機與軟體清單（`_build/guides.json`，單一來源）
| host | 名稱 | 軟體 sw | 名稱 | 範例來源（拆 details） | 讀值寫法來源 |
|---|---|---|---|---|---|
| spike | SPIKE Prime／Robot Inventor | `official` | 樂高官方 App（SPIKE 3、Robot Inventor） | spike-examples：summary 含「樂高官方 App」／"official LEGO app" | JSON `spike` 模式 0（教育版／玩具版 byte）＋現有 spike-education／spike-ri 頁 |
| | | `mbcblocks` | MBC 積木網站 | summary 含「MBC 積木網站」／"MBC block" | 模式 9 ＋ 積木名稱 |
| | | `pybricks` | Pybricks | summary 含「Pybricks」 | snippets/spike.mjs（原生 read＋MBC 函式庫） |
| ev3 | MINDSTORMS EV3 | `ev3g` | EV3-G 官方軟體 | ev3-examples：summary 含「EV3-G」 | JSON `ev3` 模式 0~2（紅外線 PROX／SEEK／REMOTE 積木） |
| | | `classroom` | EV3 Classroom | summary 含「Classroom」 | 同上 |
| | | `pybricks` | Pybricks（v4 與 EV3 MicroPython v2） | summary 含「Pybricks v4」或「MicroPython」 | snippets/ev3.mjs |
| | | `clev3r` | clev3r（BASIC） | summary 含「clev3r」 | snippets/ev3.mjs clev3r 部分 |
| arduino | Arduino／ESP32／R4（I2C） | `ide` | Arduino IDE | arduino-examples 整頁內容 | snippets/i2c.mjs |
| mblock | mBot（mBlock 5） | `mblock5` | mBlock 5 | mblock-examples 整頁內容 | snippets/mblock.mjs |
| matrix | MATRIX Mini R4 | `matrixblock` | MATRIXblock 原廠積木（0x22） | matrix-examples 若有，否則無 | snippets/matrix.mjs ch22 |
| | | `bigchannel` | 大通道函式庫（MATRIXblock／Arduino） | 同上 | snippets/matrix.mjs big |
| | （神攝手） | `mvision` | mVision 串口積木 | — | JSON `mvision` |
| microbit | micro:bit | `makecode` | MakeCode（積木／JavaScript） | microbit-examples 整頁內容 | snippets/microbit.mjs |

產品有沒有某個平台／軟體：看 `products.json routes` 與 `protocol\<id>.json hosts`；EXP6／Servo6 只有 SPIKE（`official`、`pybricks`）與 EXP6 的 Arduino（UART，用現有 `arduino-examples.md`）；OV3660 沒有。神攝手 WRO 版用同一個產品頁，軟體頁加 `-wro` 字尾或在軟體頁內分兩節（產生器決定，維持一致）。

## 檔案
- `_build/guides.json`：上表（host、sw、名稱 zh/en、圖示、顏色 token、範例 summary 比對規則、哪些產品有）。
- `_build/guides/software/<host>-<sw>.<zh|en>.md`：通用的「安裝與設定」內文（裝軟體、裝韌體、加擴充、讀值概念），可用 `{{product}}`、`{{port}}`、`{{lib}}`、`{{libUrl}}`、`{{addr}}` 佔位。
- `_build/guides/product/<p>/<host>-<sw>.<zh|en>.md`（選用）：這款產品在這個軟體的額外說明（例如循行者 8 路官方 App 的數據通道對照表、校準影片）。現有手寫頁（line8／line16 的 spike-education、spike-ri、spike-pybricks、ev3-official、ev3-classroom、ev3-clev3r、ev3-pybricks，exp6／servo6 的 spike-official、spike-pybricks）的內容搬到這裡。
- `_build/gen-guides.mjs`：產生 `sensors/<p>/<host>/index.md`（平台頁）與 `sensors/<p>/<host>/<sw>.md`（軟體頁），中英同出；`--check`。
- 舊網址（spike-hub、ev3-hub、spike-education、spike-ri、spike-pybricks、ev3-official、ev3-classroom、ev3-clev3r、ev3-pybricks、reference、arduino-i2c、arduino-examples、mblock、mblock-examples、matrix、microbit、microbit-examples、ev3-examples、spike-examples）：**保留檔案但不再出現在導覽**，頁首加一行「這頁已整合到 → 新頁」；`build.mjs` 支援 `<!-- moved: /sensors/<p>/<host>/<sw>.md -->` 標記，產出含 `rel=canonical` 指向新頁的版本＋頁首提示（不做自動跳轉，避免 GSC 軟 404）。
- 側欄：每款產品只有一行（產品名），點進去在頁面裡選；全站區保留選購指南、I2C 協議、下載中心、MATRIX、micro:bit、mBlock 安裝。
- `build.mjs`：切換列（第 2、3 層頁面自動加）、產品頁 `<!-- platform-grid -->` 改成「① 選你的主機」大格子＋「繼續用…」、平台頁「② 選你用的軟體」大格子、麵包屑、首頁三步選擇器。

## 版型
- 第 1、2 層：大格子（主機圖／軟體圖示＋一句話「適合誰」），手機一欄、桌機 2～3 欄，觸控 ≥44px。
- 第 3 層：文件版型（左側欄＋內容＋右側目錄），頂端切換列 sticky。
- 平台顏色沿用 SPIKE 黃、EV3 紅、Arduino 青、mBlock 藍、MATRIX 綠、micro:bit 橘。
