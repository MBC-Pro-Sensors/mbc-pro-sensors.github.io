<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# 🧭 選購指南

不知道該買哪一款、哪個版本？兩個步驟就能決定：**先選你的主機，再依需求挑產品**。還是不確定，直接 [LINE 問我們](https://line.me/R/ti/p/@692vcvuk)，工程師幫你確認。

> [!IMPORTANT]
> MBC 每款產品都依主機分成不同版本（接頭與通訊方式不同），**不同版本不能互換**。下單前請確認你的主機型號。

## 1️⃣ 你用的是哪一種主機？

<div class="guide-hosts">
<a class="guide-host" href="#spike">🧱 SPIKE Prime<small>含 Robot Inventor</small></a>
<a class="guide-host" href="#ev3">🧱 EV3<small>MINDSTORMS EV3</small></a>
<a class="guide-host" href="#arduino">∿ Arduino<small>ESP32 / 樹莓派</small></a>
<a class="guide-host" href="#matrix">🤖 MATRIX<small>Mini R4</small></a>
<a class="guide-host" href="#makeblock">🤖 MakeBlock</a>
</div>

<h3 id="spike">🧱 LEGO SPIKE Prime / Robot Inventor</h3>

| 產品 | 用途 | 備註 |
| :--- | :--- | :--- |
| [循行者 8 路](/sensors/line8/index.md) | 循線 | 官方 App 積木、Pybricks |
| [循行者 16 路](/sensors/line16/index.md) | 高速循線、十字路口 | 官方 App 積木、Pybricks |
| [測距者 2 路](/sensors/tof2/index.md) | 左右測距、物體方向 | |
| [測距者 8 路](/sensors/tof8/index.md) | 180° 避障、相撲 | |
| [SPIKE 6 路擴充器](/sensors/exp6/index.md) | 孔位不夠時擴充 | **僅限 SPIKE Prime**；官方 App 需用 Python 文字模式 |
| [掌控者 PS4/PS5](/sensors/ps4/index.md) | 藍牙手把遙控 | 大型比賽場地推薦 |
| [掌控者 PS2](/sensors/ps2/index.md) | 2.4G 手把遙控 | 教學、專題 |
| [神攝手 ESP32CAM](/sensors/esp32cam/index.md) | 顏色追蹤、視覺 | |
| [陀螺儀 IMU](/sensors/imu/index.md) | 航向、轉角度 | 即將推出 |

<h3 id="ev3">🧱 LEGO MINDSTORMS EV3</h3>

| 產品 | 用途 | 備註 |
| :--- | :--- | :--- |
| [循行者 8 路](/sensors/line8/index.md) | 循線 | EV3 官方軟體、EV3 Classroom、clev3r、Pybricks |
| [循行者 16 路](/sensors/line16/index.md) | 高速循線、十字路口 | 同上 |
| [測距者 2 路](/sensors/tof2/index.md) | 左右測距、物體方向 | |
| [測距者 8 路](/sensors/tof8/index.md) | 180° 避障、相撲 | |
| [掌控者 PS4/PS5](/sensors/ps4/index.md) | 藍牙手把遙控 | |
| [掌控者 PS2](/sensors/ps2/index.md) | 2.4G 手把遙控 | |
| [神攝手 ESP32CAM](/sensors/esp32cam/index.md) | 顏色追蹤、視覺 | |
| [陀螺儀 IMU](/sensors/imu/index.md) | 航向、轉角度 | 直接用原廠陀螺儀積木；即將推出 |

<h3 id="arduino">∿ Arduino / ESP32 / 樹莓派（通用 I2C 版）</h3>

全系列使用同一套 [通用 I2C 協議](/i2c-protocol.md)，學會一款就會全部。

| 產品 | 用途 | I2C 位址 |
| :--- | :--- | :---: |
| [循行者 8 路](/sensors/line8/arduino-i2c.md) | 循線 | `0x16` |
| [循行者 16 路](/sensors/line16/arduino-i2c.md) | 高速循線、十字路口 | `0x16` |
| [測距者 8 路](/sensors/tof8/index.md) | 180° 避障 | `0x29` |
| [SPIKE 6 路擴充器](/sensors/exp6/arduino-i2c.md) | 用 Arduino 驅動 LEGO 馬達與感應器 | `0x6E` |
| [掌控者 PS4/PS5](/sensors/ps4/index.md) | 藍牙手把遙控 | `0x42` |
| [掌控者 PS2](/sensors/ps2/index.md) | 2.4G 手把遙控 | `0x42` |
| [陀螺儀 IMU](/sensors/imu/index.md) | 航向、九軸資料 | `0x6A` |

<h3 id="matrix">🤖 MATRIX Mini R4</h3>

| 產品 | 用途 |
| :--- | :--- |
| [循行者 8 路](/sensors/line8/index.md) | 循線 |
| [循行者 16 路](/sensors/line16/index.md) | 高速循線、十字路口 |
| [測距者 8 路](/sensors/tof8/index.md) | 180° 避障 |
| [神攝手 ESP32CAM](/sensors/esp32cam/index.md) | 顏色追蹤、視覺 |
| [陀螺儀 IMU](/sensors/imu/index.md) | 航向、轉角度（即將推出） |

<h3 id="makeblock">🤖 MakeBlock</h3>

| 產品 | 用途 |
| :--- | :--- |
| [循行者 8 路](/sensors/line8/index.md) | 循線 |
| [循行者 16 路](/sensors/line16/index.md) | 高速循線、十字路口 |
| [測距者 2 路](/sensors/tof2/index.md) | 左右測距 |
| [測距者 8 路](/sensors/tof8/index.md) | 180° 避障 |
| [掌控者 PS4/PS5](/sensors/ps4/index.md) | 藍牙手把遙控 |
| [掌控者 PS2](/sensors/ps2/index.md) | 2.4G 手把遙控 |

## 2️⃣ 依需求挑選

| 你想要… | 推薦 | 為什麼 |
| :--- | :--- | :--- |
| 循線更穩、適合一般地圖 | [循行者 8 路](/sensors/line8/index.md) | 8 通道、體積精巧，最常用的競賽循線選擇 |
| 高速過彎、十字路口多 | [循行者 16 路](/sensors/line16/index.md) | 16 通道超寬視野，彎道與路口一眼判斷 |
| 判斷左右有沒有障礙物 | [測距者 2 路](/sensors/tof2/index.md) | 雙點雷射，輕巧、反應快 |
| 相撲、全方位避障 | [測距者 8 路](/sensors/tof8/index.md) | 前方 180° 一次看完，直接回報最近目標方向 |
| SPIKE 孔位不夠用 | [SPIKE 6 路擴充器](/sensors/exp6/index.md) | 一個孔接出 6 路，獨立供電 |
| 比賽用遙控，現場隊伍多 | [掌控者 PS4/PS5](/sensors/ps4/index.md) | 只綁定自己的手把，斷訊自動停車 |
| 教學或專題用遙控，預算有限 | [掌控者 PS2](/sensors/ps2/index.md) | 2.4G 手把，最划算 |
| 追顏色、找物體位置 | [神攝手 ESP32CAM](/sensors/esp32cam/index.md) | 機身螢幕直接設定，色塊位置一次輸出 |
| 走直線、轉精準角度 | [陀螺儀 IMU](/sensors/imu/index.md) | 任意角度安裝，磁力計融合不飄移（即將推出） |

<div class="lp-final">
<h2>還是不確定？</h2>
<p>告訴我們你的主機型號和比賽項目，工程師幫你挑最適合的組合。</p>
<div class="lp-btns">
<a class="lp-btn line" href="https://line.me/R/ti/p/@692vcvuk" target="_blank" rel="noopener">💬 LINE 立即詢價</a>
<a class="lp-btn ghost" href="/contact.md">📬 其他聯絡方式</a>
</div>
</div>
