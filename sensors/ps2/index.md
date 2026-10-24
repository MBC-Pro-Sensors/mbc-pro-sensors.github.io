<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# 掌控者 2.4G 遙控接收器 (PS2)

<!-- product-hero -->

<!-- platform-grid -->


> **高性價比入門首選：輕鬆擴充控制按鍵，教學與小型專題的無線利器。**

## 🚀 產品概述

LEGO 原廠提供的紅外線遙控器按鈕少得可憐，而使用兩台 EV3 主機進行藍牙連線的方案又極度耗費資源、容易斷線。

掌控者 2.4G 遙控接收器提供了一個**高性價比的解決方案**：讓您的 SPIKE 或 EV3 主機透過標準接口，直接讀取一隻擁有雙搖桿與豐富按鍵的無線控制器，釋放您對機器人的完整操控能力。

## 🎯 核心優勢

- 🎮 **豐富按鍵配置**：雙搖桿 + 方向鍵 + 多個功能鍵，控制選項遠多於原廠遙控
- 💰 **高性價比**：解決入門遙控需求的最經濟方案
- 🔌 **即插即用**：直接插上 SPIKE / EV3，官方積木直接讀取按鍵狀態

## ⚠️ 使用限制與建議場景

> [!CAUTION]
> **2.4G 頻段注意事項**
> 2.4GHz 為公共頻段，與 Wi-Fi、微波爐等設備共用。在同一場地有 **12 套以上**的設備同時使用時，可能因頻道擁擠而產生互相干擾，建議在此場景下改用藍牙版（PS4/PS5）。

| 使用場景 | 適合程度 |
| :--- | :--- |
| 個人練習、家用、小型專題 | ✅ 非常適合 |
| 學校社團（10 套以內） | ✅ 適合 |
| 大型比賽場地（12+ 套同時使用） | ⚠️ 建議改用 PS4/PS5 版 |

## 🧩 支援平台

| 平台 | 支援方式 |
| :--- | :--- |
| LEGO SPIKE Prime / Robot Inventor | 官方 App、Pybricks（👉 [SPIKE 新手範例：官方 App／MBC 積木網站／Pybricks 各 5 個](/sensors/ps2/spike-examples.md)） |
| LEGO MINDSTORMS EV3 | EV3 官方軟體、Pybricks（[EV3 新手範例](/sensors/ps2/ev3-examples.md)） |
| Arduino / ESP32 / 樹莓派 | 通用 I2C（位址 `0x42`），[Arduino 範例](/sensors/ps2/arduino-examples.md) |
| MakeBlock（mBot 等） | 專屬版本，[積木與接法](/makeblock.md) |

> [!IMPORTANT]
> 每個平台是**不同的出廠版本**，請依你的主機選購對應版本。

## 📥 範例下載

| 主機 | 檔案 |
| :--- | :--- |
| SPIKE（Pybricks 4.1） | [全部下載（.zip）](/examples/ps2/ps2_pybricks_spike_v4.x.zip)・[函式庫 MBC_gamepad_Lib.py](/examples/ps2/pybricks/MBC_gamepad_Lib.py)・[共用函式庫 MBC_sensors.py（新專案用）](/examples/ps2/pybricks/MBC_sensors.py)・[新手範例](/sensors/ps2/spike-examples.md) |
| SPIKE（Pybricks 3.6.1，不再更新） | 舊版檔案在[下載專區](/downloads.md)的「舊版（不再更新）」 |
| EV3／SPIKE App／Pybricks 積木用法 | [指令說明（PDF）](/examples/ps2/PS2_Controller_Command_Guide_ZH.pdf) |
| mBot 等（MakeBlock） | [積木擴展與手把遙控範例](/makeblock.md) |

## 📤 輸出資料

**搖桿數值：** EV3 為 `0` ~ `99`（`50` = 中心）；SPIKE（官方 App、Pybricks）為 `1` ~ `9`（`5` = 中心）。**往上推、往右推數值變大**。

| EV3 模式 | 內容 |
| :---: | :--- |
| 0 | 方向鍵狀態 |
| 1 | 方向鍵、左搖桿 X、形狀鍵（■ ✕ ● ▲）、左搖桿 Y、肩鍵（L1 R1 L2 R2）、右搖桿 X、功能鍵（START SELECT L3 R3）、右搖桿 Y |
| 2 | 左搖桿 X、左搖桿 Y、右搖桿 X、右搖桿 Y |

## ∿ 通用 I2C 暫存器（位址 `0x42`）

與 [PS4/PS5 接收器](/sensors/ps4/index.md) 使用同一張暫存器表，主機程式可以共用。共通規則請看 [MBC 通用 I2C 協議](/i2c-protocol.md)。

> [!TIP]
> **不會寫程式也能用：** 看 [🧪 Arduino 範例](/sensors/ps2/arduino-examples.md)，每個範例都能直接複製上傳，每一行都有中文說明。

| 暫存器 | 方向 | 長度 | 內容 |
| :---: | :---: | :---: | :--- |
| `0x11` | R | 9 | 手把合併包（= `0x12` ~ `0x15`） |
| `0x12` | R | 2 | 按鈕 uint16（b15 ■、b14 ✕、b13 ●、b12 ▲、b11 R1、b10 L1、b9 R2、b8 L2、b7 左、b6 下、b5 右、b4 上、b3 START、b2 R3、b1 L3、b0 SELECT） |
| `0x13` | R | 4 | 搖桿 LX, LY, RX, RY：int8 `-127` ~ `+127`，0 = 中心，右／上為正；中心 ±8 以內當作 0（死區）。手把在數位模式（ANALOG 燈沒亮）時搖桿都是 0 |
| `0x14` | R | 2 | 扳機 L2, R2：`0` ~ `255`（手把不支援壓力值時只有 0 / 255） |
| `0x22` | R | 4 | 簡易搖桿 LX, LY, RX, RY：`0` ~ `99`，50 = 中心 |
| `0x32` | R | 1 | bit0 = 就緒、bit1 = 手把已連線 |
| `0x34` | R | 1 | 手把類型：0 = 未連線、1 = 數位模式、2 = 類比模式 |

`0x06` 款式第 1 byte = `1`。PS2 版不支援震動與燈條（寫入 `0x10`、`0x23` 會被忽略）。

## 🔌 硬體接線

1. SPIKE／EV3 版：將接收器插入 SPIKE（Port A~F）或 EV3（Port 1~4）
   通用 I2C 版：接收器 TX → SCL（UNO 的 A5）、RX → SDA（UNO 的 A4）、VCC → 5V、GND → GND，詳見 [Arduino 範例](/sensors/ps2/arduino-examples.md)
2. 開啟控制器電源，等待配對完成（指示燈穩定亮起）
3. 在官方積木環境中，讀取感應器數值即可取得按鍵狀態
