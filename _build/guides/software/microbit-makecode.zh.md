### 你需要

- **感應器**：{{product}}（{{edition}}）。SPIKE 版、EV3 版韌體不同，接 micro:bit 讀不到。
- **主板**：BBC micro:bit **V1 或 V2**，加上能接到 P19／P20 的**擴充板或鱷魚夾轉接板**。
- **軟體**：**MakeCode 網頁版**（[makecode.microbit.org](https://makecode.microbit.org/)，積木／JavaScript）；離線版（Windows／Mac App）也可以，第一次加擴充要連網。
- **擴充**：**pxt-mbc-sensors v2**（MIT，免下載，貼網址就能加）。

> [!IMPORTANT]
> **上機待測**：擴充 v2.0.5 已在 MakeCode 編譯通過（V1＋V2），還沒有接實機逐顆積木核對數值。積木名稱以 [GitHub 發布版](https://github.com/contactmbcrobot/pxt-mbc-sensors) 為準。

### 安裝軟體

1. 打開 [makecode.microbit.org](https://makecode.microbit.org/) → **新專案**。
2. 編輯器語言切到 **繁體中文**（右上齒輪 → Language），積木就會顯示中文。

### 讓軟體認得感應器

1. 點積木區下方的 **進階 → 擴充**（齒輪選單裡也有「擴充」）。
2. 在搜尋框貼上 `https://github.com/contactmbcrobot/pxt-mbc-sensors`，按 Enter。
3. 點出現的 **mbc-sensors** 卡片，積木區會多出「**MBC 感應器**」分類（循行者、測距者、掌控者、陀螺儀、神攝手、裝置六組）。
4. 用範例：右上切到 **JavaScript** → 貼上範例 → 下載 `.hex` 拖進 **MICROBIT** 磁碟。

> [!TIP]
> 不需要送審、不需要下載檔案：老師把網址給學生貼就能用。要固定版本可貼 `https://github.com/contactmbcrobot/pxt-mbc-sensors#v2.0.5`。

### 接線

micro:bit 的 I2C 腳位是固定的：**P19 = SCL、P20 = SDA**（金手指邊緣接頭，要用擴充板或鱷魚夾轉接板才接得到）。MBC 4P 感應器線的 **TX 接 SCL、RX 接 SDA**，跟接 Arduino 是同一條線。

| 感應器 | micro:bit | 說明 |
| :--- | :--- | :--- |
| SCL（TX） | **P19** | I2C 時脈 |
| SDA（RX） | **P20** | I2C 資料 |
| VCC | **3V** 或外部 5 V | 耗電大的產品不能吃 micro:bit 的 3V |
| GND | GND | 外部供電時 GND 要和 micro:bit 共地 |

- **3V 腳電流**：V1 約 **90 mA**、V2 約 **190 mA**；用電池時電壓可能掉到 2 V 多。測距者 8 路、掌控者 PS4／PS5、神攝手 **一定要外部 5 V**；其他產品可先試 3V，電池供電時建議外部 5 V。
- **電壓準位**：MBC 產品的 I2C 線是 3.3 V 準位，跟 micro:bit 相容，不需要準位轉換。
- 這個產品的出廠位址是 `{{addr}}`；位址不同的產品可以串在同一組 P19／P20 上。

### 第一次測試

1. 先用「**裝置 → 等位址 {{addr}} 的裝置就緒**」或「**位址 {{addr}} 的裝置就緒？**」積木確認接線。
2. 下載本頁範例 1 的 `.hex` 到 micro:bit。
3. LED 顯示的數字跟著感應器前方的東西變化，就是接好了。

### 常見問題

- **讀值一直是 0？** 裝置沒接時讀值積木回 **0**（內部的 −32768 已過濾）、是／否積木一律回「否」，不會報錯。依序檢查：接線（SDA＝P20、SCL＝P19）→ 外接電源有沒有開 →「位址 … 的裝置就緒？」是不是 true。
- **V1 的指南針、加速度計也壞了？** V1 的 I2C 線和板上加速度計共用，感應器卡住 SDA 會連帶讓它們失效；V2 把外部 I2C 分開了。
- **時好時壞？** STM32 產品（循行者、測距者、PS2、陀螺儀）會時脈延展，micro:bit V2 的 nRF52 對此有已知問題（errata 219），V1／V2 都要實測；ESP32 產品（PS4、神攝手）不延展，擴充已內建「寫完編號等 2 ms 再讀」。
- **兩個同類產品一起接？** 同類產品位址相同，先用「裝置 → 把位址 … 的裝置改成位址 …」把其中一個改掉（重開機生效），再用該產品的「改用位址」積木告訴擴充新位址。
