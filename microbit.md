<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# 🟠 micro:bit（MakeCode 擴充）

MBC 通用 I2C 版的循行者、測距者、掌控者、IMU 陀螺儀與神攝手，都能接 BBC micro:bit V1／V2：在 MakeCode 加一個擴充 **pxt-mbc-sensors v2**，積木區就多出「MBC 感應器」分類，每個數值一顆積木，內部走 [MBC 通用 I2C 協議 v2](/i2c-protocol.md)。

> [!IMPORTANT]
> **上機待測**：擴充 v2.0.2 已在 MakeCode 編譯通過（V1＋V2），但還沒有接實機逐顆積木核對數值。積木名稱以 GitHub 發布版為準：<https://github.com/contactmbcrobot/pxt-mbc-sensors>。實測後本頁會更新。

---

## 📥 加入擴充

1. 打開 <https://makecode.microbit.org/> → **新專案**。
2. 點積木區下方的 **進階 → 擴充**（齒輪選單裡也有「擴充」）。
3. 在搜尋框貼上網址 `https://github.com/contactmbcrobot/pxt-mbc-sensors`，按 Enter。
4. 點出現的 **mbc-sensors** 卡片，積木區會多出「MBC 感應器」分類（循行者、測距者、掌控者、陀螺儀、神攝手、裝置 六組）。
5. 編輯器語言切到 **繁體中文**（右上齒輪 → Language），積木就會顯示中文。

> [!TIP]
> 不需要送審、不需要下載檔案：老師把上面這個網址給學生貼就能用。要固定版本可貼 `https://github.com/contactmbcrobot/pxt-mbc-sensors#v2.0.2`。

## 🔌 接線

micro:bit 的 I2C 腳位是固定的：**P19 = SCL、P20 = SDA**（金手指邊緣接頭，要用擴充板或鱷魚夾轉接板才接得到）。MBC 4P 感應器線的 **TX 接 SCL、RX 接 SDA**，跟接 Arduino 時是同一條線。

| 感應器 | micro:bit | 說明 |
| :--- | :--- | :--- |
| SCL（TX） | **P19** | I2C 時脈 |
| SDA（RX） | **P20** | I2C 資料 |
| VCC | **3V** 或外部 5 V | 看下表，耗電大的產品不能吃 micro:bit 的 3V |
| GND | GND | 外部供電時 GND 要和 micro:bit 共地 |

| 產品 | 供電 | 原因 |
| :--- | :--- | :--- |
| 循行者 8／16 路、測距者 2 路、掌控者 PS2、IMU 陀螺儀 | micro:bit 3V 可用（電池供電時建議外部 5 V） | STM32 產品，耗電小 |
| **測距者 8 路** | **一定要外部 5 V** | 8 顆雷射同時量測，電流超過 3V 腳能給的 |
| **掌控者 PS4／PS5** | **一定要外部 5 V** | ESP32 藍牙接收，瞬間電流大 |
| **神攝手 ESP32CAM** | **一定要外部 5 V** | ESP32 相機＋螢幕＋補光燈 |

多個產品可以串在同一組 P19／P20 上，只要 I2C 位址不同（出廠位址：循行者 `0x16`、測距者 `0x29`、掌控者 `0x42`、陀螺儀 `0x6A`、神攝手 `0x4C`）。

## ⚠️ 硬體關卡

| 項目 | 狀況 |
| :--- | :--- |
| **電壓準位** | MBC 產品的穩壓器吃 3.3 V 或 5 V 都可以，I2C 線是 3.3 V 準位，跟 micro:bit 相容，不需要準位轉換。 |
| **3V 腳電流** | V1 約 **90 mA**、V2 約 **190 mA**；用電池時電壓可能掉到 2 V 多。超過的產品（上表）一律外部供電。 |
| **V1 與 V2 差異** | V1 的 I2C 線和板上加速度計共用，感應器卡住 SDA 會連帶讓指南針、加速度計失效；V2 把外部 I2C 分開了。兩版都用 100 kHz。 |
| **時脈延展（clock stretching）** | STM32 產品（循行者、測距者、PS2、陀螺儀）準備資料時會拉住 SCL；micro:bit V2 的 nRF52 對時脈延展有已知問題（errata 219），**V1／V2 各要實測**，不穩就改韌體不延展。ESP32 產品（PS4、神攝手）不延展，擴充已內建「寫完編號等 2 ms 再讀」。 |
| **上拉電阻** | micro:bit 板上已有 4.7 kΩ 上拉；產品板也有時並聯會變強，正常情況仍可用。 |
| **同位址不能同接** | 循行者 8 路和 16 路都是 `0x16`、測距者 2 路和 8 路都是 `0x29`、PS2 和 PS4 都是 `0x42`，兩個同類產品要一起接時，先用「裝置 → 把位址 … 改成位址 …」積木把其中一個改掉（存在感應器裡，重開機生效），再用「循行者 改用位址」這類積木告訴擴充新位址。 |
| **裝置沒接時** | 讀值積木回 **0**（內部的 −32768 已過濾），不會報錯；先用「裝置 → 等位址 … 的裝置就緒」確認接線。 |

## 🧩 積木清單（83 顆）

每顆一行：積木文字（繁中 ｜ 英文）、功能、範圍。標 **進階** 的在分類的「…更多」裡。

### 裝置（共用，進階）

| 積木 | 功能 | 範圍 |
| :--- | :--- | :--- |
| 位址 … 的裝置就緒？ ｜ device at … is ready | 裝置有回應且 F3 bit0 = 1 | 是／否 |
| 等位址 … 的裝置就緒，最多 … 毫秒 ｜ wait for device at … ready, timeout … ms | 等到就緒或逾時 | 0 ~ 60000 ms，預設 5000 |
| 位址 … 的裝置 出廠 ID ｜ device at … factory id | F0 低 byte：出廠位址（改位址後不變） | 0x16／0x29／0x42／0x6A／0x4C |
| 位址 … 的裝置 類別 ｜ device at … kind | F0 高 byte：循行者路數 8／16、測距者顆數 2／8、掌控者 1＝PS2／2＝PS4、陀螺儀 1、神攝手 1＝學校版／2＝WRO 版 | 0 ~ 255 |
| 位址 … 的裝置 協議版本 ｜ device at … protocol version | F1，這個擴充要 2 | 2 |
| 位址 … 的裝置 韌體版本 ｜ device at … firmware version | F2 換成「主.次」字串 | 例 "4.0" |
| 位址 … 的裝置 狀態旗標 ｜ device at … status flags | F3 整個 byte | 0 ~ 255 |
| 位址 … 的裝置 狀態 bit … ｜ device at … status bit … | F3 的某一個 bit | bit 0 ~ 7 |
| 位址 … 的裝置 更新序號 ｜ device at … update counter | F4，每 10 ms +1，不動＝裝置忙碌 | 0 ~ 255 |
| 位址 … 的裝置 開機秒數 ｜ device at … uptime (s) | F5 | 0 ~ 32767 |
| 把位址 … 的裝置 改成位址 … ｜ change device at … to address … | F8 換位址，存進 EEPROM，重開機生效 | 0x08 ~ 0x77 |
| 位址 … 的裝置 回出廠位址 ｜ restore factory address of device at … | F8 `[00][FF]`，重開機生效 | — |
| 位址 … 的裝置 重新開機 ｜ reboot device at … | EF | — |

### 循行者（8 路／16 路，`0x16`）

| 積木 | 功能 | 範圍 |
| :--- | :--- | :--- |
| 循行者 線位置 ｜ line position | 線在哪一路，站在車後往前看 **負＝左** | −8 ~ +8（8 路）／−16 ~ +16（16 路），0 = 正中 |
| 循行者 細位置 ｜ fine line position | 線位置的細分，與線位置同號 | −100 ~ +100 |
| 循行者 看到線？ ｜ line is seen | 線段數 > 0 | 是／否 |
| 循行者 線寬 ｜ line width | 壓到線的路數 | 0 ~ 8／16，0 = 沒線 |
| 循行者 線段數 ｜ line segments | 0 丟線、1 一條線、≥2 分岔／十字 | 0 ~ 4（8 路）／0 ~ 8（16 路） |
| 循行者 壓線位元圖 ｜ line bitmap | bit0 = 第 1 路（最左）… | 0 ~ 255／0 ~ 65535 |
| 循行者 第 … 路在線上？ ｜ channel … is on the line | 位元圖的某一路 | 路 1 ~ 16 |
| 循行者 第 … 路光值 ｜ channel … reflect | 校準後光值，0 黑 ~ 100 白 | 0 ~ 100 |
| 循行者 最後離線方向 ｜ last exit direction | 丟線前線在哪一側，負＝左 | −8 ~ +8／−16 ~ +16 |
| 循行者 門檻 ｜ line threshold | 目前「在線上」的光值門檻 | 1 ~ 99，預設 50 |
| 循行者 校準次數 ｜ line calibration count（進階） | 開機後完成的校準次數 | 0 ~ 255 |
| 循行者 校準中？ ｜ line follower is calibrating（進階） | F3 bit2 | 是／否 |
| 循行者 循 … 線 ｜ line follower follow … line | 黑線（E0，預設）／白線（E1） | 黑／白 |
| 循行者 校準 … ｜ line follower calibrate … | 目前讀值記為黑（E2）／記為白（E3）／5 秒動態掃描（E7），存進 EEPROM | 3 種 |
| 循行者 門檻設為 … ｜ line follower set threshold … | E4，只存 RAM，重開機回 50 | 1 ~ 99 |
| 當循行者 … ｜ on line … | 事件：丟線／找回線／分岔 | 3 種 |
| 循行者 改用位址 … ｜ line follower use address …（進階） | 位址改過時告訴擴充 | 0x08 ~ 0x77 |

### 測距者 2 路（`0x29`）

| 積木 | 功能 | 範圍 |
| :--- | :--- | :--- |
| 測距者 2 路 … 距離（…） ｜ distance 2-ch … (…) | 左／右那一顆的距離，公分或公釐 | 0 ~ 120 cm（120 = 沒東西）／0 ~ 1200 mm |
| 測距者 2 路 較近距離（公分） ｜ distance 2-ch nearest (cm) | 左右取較近 | 0 ~ 120 |
| 測距者 2 路 平均距離（公釐） ｜ distance 2-ch average (mm) | 左右平均（＝EV3 模式 3／SPIKE 模式 6） | 0 ~ 1200 |
| 測距者 2 路 … 感應器正常？ ｜ distance 2-ch … sensor is OK（進階） | 該顆初始化成功且沒故障 | 是／否 |
| 當測距者 2 路 距離小於 … 公分 ｜ on distance 2-ch nearer than … cm | 較近距離跌破門檻時觸發一次 | 1 ~ 120，預設 10 |
| 測距者 2 路 改用位址 … ｜ distance 2-ch use address …（進階） | 位址改過時告訴擴充 | 0x08 ~ 0x77 |

### 測距者 8 路（`0x29`）

| 積木 | 功能 | 範圍 |
| :--- | :--- | :--- |
| 測距者 8 路 最近距離（…） ｜ distance 8-ch nearest (…) | 8 顆裡最近的，公分或公釐 | 0 ~ 120／0 ~ 1200 |
| 測距者 8 路 最近方向 ｜ distance 8-ch nearest direction | 最近目標在哪個方向，負＝左 | −8 ~ +8，0 = 正前方，±8 = 沒目標 |
| 測距者 8 路 最空曠距離（公分） ｜ distance 8-ch most open (cm) | 最遠那個方向的距離 | 0 ~ 120 |
| 測距者 8 路 最空曠方向 ｜ distance 8-ch most open direction | 最遠的方向，負＝左 | −7 ~ +7 |
| 測距者 8 路 正前方距離（…） ｜ distance 8-ch front (…) | 第 4、5 顆平均 | 0 ~ 120／0 ~ 1200 |
| 測距者 8 路 第 … 顆距離（…） ｜ distance 8-ch sensor … (…) | 單顆，1 = 最左、8 = 最右 | 顆 1 ~ 8 |
| 測距者 8 路 第 … 顆正常？ ｜ distance 8-ch sensor … is OK（進階） | 該顆初始化成功且沒故障 | 是／否 |
| 當測距者 8 路 距離小於 … 公分 ｜ on distance 8-ch nearer than … cm | 最近距離跌破門檻時觸發一次 | 1 ~ 120，預設 10 |
| 測距者 8 路 改用位址 … ｜ distance 8-ch use address …（進階） | 位址改過時告訴擴充 | 0x08 ~ 0x77 |

開機後約 2 ~ 4 秒 8 顆才初始化完成，先用「裝置 → 等位址 0x29 的裝置就緒」再讀距離。

### 掌控者（PS2／PS4／PS5，`0x42`）

| 積木 | 功能 | 範圍 |
| :--- | :--- | :--- |
| 掌控者 按鈕 … 被按住？ ｜ gamepad button … is pressed | 16 鍵任一顆 | select/share、L3、R3、start/options、上右下左、L2、R2、L1、R1、▲●✕■ |
| 掌控者 十六鍵位元圖 ｜ gamepad buttons bit mask | PSX 排列：bit0 select、1 L3、2 R3、3 start、4 上、5 右、6 下、7 左、8 L2、9 R2、10 L1、11 R1、12 ▲、13 ●、14 ✕、15 ■ | 0 ~ 65535 |
| 掌控者 …（0~15） ｜ gamepad … buttons (0-15) | 四鍵一組：方向鍵（上 1 右 2 下 4 左 8）／形狀鍵（▲1 ●2 ✕4 ■8）／肩鍵（L2 1 R2 2 L1 4 R1 8）／功能鍵（select 1 L3 2 R3 4 start 8） | 0 ~ 15 |
| 掌控者 搖桿 … ｜ gamepad joystick … | 左 X／左 Y／右 X／右 Y，右、上為正 | −100 ~ +100，0 = 中間 |
| 掌控者 扳機 … 類比值 ｜ gamepad trigger … analog | L2／R2 壓力（PS2 數位手把按下 = 255） | 0 ~ 255 |
| 掌控者 手把已連線？ ｜ gamepad is connected | F3 bit1 | 是／否 |
| 掌控者 電量（百分比） ｜ gamepad battery percent | PS4／PS5 手把電量；PS2 恆 0 | 0 ~ 100 |
| 掌控者 系統鍵 … 被按住？ ｜ gamepad system button … is pressed | PS 鍵／擷取鍵（PS4／PS5 才有） | 是／否 |
| 掌控者 手把類型 ｜ gamepad type | 0 無、1 PS2 數位、2 PS2 類比、3 PS4、4 PS5 | 0 ~ 4 |
| 掌控者 震動 強度 … ｜ gamepad rumble strength … | E5，PS4／PS5 手把震動 200 ms；PS2 忽略 | 0 ~ 255，0 = 停 |
| 掌控者 燈條顏色 … ｜ gamepad light bar colour … | E6，PS4／PS5 燈條 RGB；PS2 忽略 | 顏色 |
| 當掌控者 按鈕 … … ｜ on gamepad button … … | 事件：某鍵按下／放開 | 16 鍵 × 2 |
| 掌控者 改用位址 … ｜ gamepad use address …（進階） | 位址改過時告訴擴充 | 0x08 ~ 0x77 |

### 陀螺儀 IMU（`0x6A`）

| 積木 | 功能 | 範圍 |
| :--- | :--- | :--- |
| 陀螺儀 … 航向（°） ｜ IMU heading … (°) | 融合／陀螺 3D／陀螺 1D／磁力 四種航向，超過 360 繼續累加，左轉為負 | −32768 ~ 32767 |
| 陀螺儀 轉向角速度（0.1 °/s） ｜ IMU turn rate (0.1 °/s) | 垂直軸角速度，123 = 12.3 °/s | −32768 ~ 32767 |
| 陀螺儀 俯仰角（°） ｜ IMU pitch (°) | 抬頭／低頭角度 | ° |
| 陀螺儀 俯仰角速度（0.1 °/s） ｜ IMU pitch rate (0.1 °/s) | 俯仰方向角速度 | −32768 ~ 32767 |
| 陀螺儀 … … 軸 ｜ IMU … axis … | 原始值：角速度（0.1 °/s）／加速度（mg）／磁力（mG）的 X／Y／Z | −32768 ~ 32767 |
| 陀螺儀 朝上的面 ｜ IMU face up | 0 +X、1 +Y、2 +Z、3 −X、4 −Y、5 −Z | 0 ~ 5 |
| 陀螺儀 朝上的面是 …？ ｜ IMU face up is … | 判斷某一面朝上 | 6 面 |
| 陀螺儀 磁力信任度 ｜ IMU magnetic trust | 磁力計被採信的程度 | 0 ~ 255 |
| 陀螺儀 磁場干擾度 ｜ IMU magnetic disturbance | 越高干擾越大 | 0 ~ 255 |
| 陀螺儀 …？ ｜ IMU is … | 狀態旗標：靜止／已擷取安裝軸／安裝過歪／加速度計校準中／磁力計校準中／磁力融合已初始化／磁力自動校準已收斂 | 是／否 |
| 陀螺儀 航向歸零 ｜ IMU zero heading | E0，所有航向歸 0 | — |
| 陀螺儀 校準 … ｜ IMU calibration … | E1 ~ E4：加速度計開始／結束、磁力計開始／結束，結果存在感應器 | 4 步 |
| 當陀螺儀 轉了 … ° ｜ on IMU turned by … ° | 融合航向每變化指定角度觸發一次 | 例 90 |
| 陀螺儀 改用位址 … ｜ IMU use address …（進階） | 位址改過時告訴擴充 | 0x08 ~ 0x77 |

### 神攝手（學校版／WRO 版，`0x4C`）

| 積木 | 功能 | 範圍 |
| :--- | :--- | :--- |
| 神攝手 有找到色塊？ ｜ camera blob is found | 學校版：有找到目標顏色的色塊 | 是／否 |
| 神攝手 色塊 … ｜ camera blob … | 學校版：X／Y／寬／高／面積 | X 0 ~ 100、Y 0 ~ 75（下→上）、寬 0 ~ 100、高 0 ~ 75、面積 = 寬×高 |
| 神攝手 顏色 … ｜ camera colour … | 學校版取色點：色相／飽和度／明度 | 色相 0 ~ 100（−1 = 黑白灰）、飽和 0 ~ 100、明度 0 ~ 100 |
| 神攝手 顏色是黑白灰？ ｜ camera colour is black / white / grey | 取色點沒有色相 | 是／否 |
| 神攝手 動態 ｜ camera motion | 學校版畫面變化量 | 0 ~ 100 |
| 神攝手 色點 … 顏色碼 ｜ camera spot … colour code | WRO 版（HUE12）：0 ~ 239 彩色、240 ~ 245 白、246 ~ 250 黑、255 = 色點沒開 | 色點 1 ~ 12 |
| 神攝手 色點 … 鮮豔度 ｜ camera spot … vividness | WRO 版該色點飽和度 | 0 ~ 255 |
| 神攝手 色點 … 是 …？ ｜ camera spot … is … | WRO 版：彩色／白／黑／沒開 | 是／否 |
| 當神攝手 色塊 … ｜ on camera blob … | 學校版事件：找到／丟失 | 2 種 |
| 神攝手 改用位址 … ｜ camera use address …（進階） | 位址改過時告訴擴充 | 0x08 ~ 0x77 |

神攝手的 I2C 用 **SDA = IO3、SCL = IO1**（2026-10-21 定案），接上 I2C 後 USB 序列監看就沒了；硬體已確認可做，**上機待測**。

## 📖 各產品的 micro:bit 頁與範例

每款產品一頁，列出它用到的積木、對應的 I2C 值編號與 JavaScript 寫法；範例頁每款 5 支（.ts，可直接貼進 MakeCode 的 JavaScript 模式）。

| 產品 | 積木說明 | 範例（5 支） |
| :--- | :--- | :--- |
| 循行者 8 路 | [line8 × micro:bit](/sensors/line8/microbit.md) | [範例](/sensors/line8/microbit-examples.md) |
| 循行者 16 路 | [line16 × micro:bit](/sensors/line16/microbit.md) | [範例](/sensors/line16/microbit-examples.md) |
| 測距者 2 路 | [tof2 × micro:bit](/sensors/tof2/microbit.md) | [範例](/sensors/tof2/microbit-examples.md) |
| 測距者 8 路 | [tof8 × micro:bit](/sensors/tof8/microbit.md) | [範例](/sensors/tof8/microbit-examples.md) |
| 掌控者 PS2 | [ps2 × micro:bit](/sensors/ps2/microbit.md) | [範例](/sensors/ps2/microbit-examples.md) |
| 掌控者 PS4／PS5 | [ps4 × micro:bit](/sensors/ps4/microbit.md) | [範例](/sensors/ps4/microbit-examples.md) |
| IMU 陀螺儀 | [imu × micro:bit](/sensors/imu/microbit.md) | [範例](/sensors/imu/microbit-examples.md) |
| 神攝手 ESP32CAM | [esp32cam × micro:bit](/sensors/esp32cam/microbit.md) | [範例](/sensors/esp32cam/microbit-examples.md) |

> [!NOTE]
> SPIKE 6 路擴充器（EXP6）走 UART，不在這個擴充裡；Servo6 沒有 micro:bit 版。

## 💻 JavaScript 寫法

積木都對應 `mbc` 命名空間的函式，切到 JavaScript 模式可以直接寫：

```typescript
basic.forever(function () {
    if (mbc.waitReady(mbc.ADDR_TOF, 5000)) {
        basic.showNumber(mbc.tof8Nearest(MbcDistUnit.Cm))
    }
    basic.pause(100)
})
```

進階：`mbc.readValue(位址, 編號)` 直接讀任一個 I2C v2 值（int16 小端，裝置不在回 −32768），`mbc.writeCmd(位址, 編號, [參數])` 送命令，編號看各產品的[指令參考](/i2c-protocol.md)。

## 📦 下載與原始碼

- 擴充 repo（MIT）：<https://github.com/contactmbcrobot/pxt-mbc-sensors>（v2.0.2，2026-10）
- 其他平台的函式庫與範例：[下載專區](/downloads.md)
