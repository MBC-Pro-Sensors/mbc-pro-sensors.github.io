<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# 掌控者 PS4/PS5 藍牙遙控接收器

<!-- product-hero -->

<!-- platform-grid -->


> **用 PS4 / PS5 手把遙控樂高機器人：只認你的手把、斷訊立刻停車，數十隊同場也不怕。**

## 🚀 產品概述

雙主機藍牙方案耗費資源又容易斷線；紅外線遙控器按鈕少且有方向限制；2.4G 方案在多設備場合容易互相干擾。

掌控者 PS4/PS5 藍牙遙控接收器讓 SPIKE、EV3 或 Arduino 直接讀取一支 **PS4（DualShock 4）或 PS5（DualSense）手把**的所有按鍵與搖桿。配對一次就記住，之後開機自動連線；比賽現場不會連到別隊的手把，訊號中斷時輸出立刻歸零，機器人不會失控暴衝。

## 🎯 核心優勢

- 🎮 **PS4 / PS5 手把都能用**：DualShock 4 與 DualSense 皆支援
- 🔒 **只綁定你的手把**：配對完成後自動關閉藍牙搜尋，不會連到別隊的手把，也不受旁邊正在配對的手把干擾，適合數十隊同場的比賽
- 🛑 **斷訊自動停車**：超過 0.3 秒沒收到手把資料，輸出立刻歸零，機器人不會暴衝
- 🧠 **配對記憶**：一次配對，之後開機自動重連，比賽前零設定時間
- 📶 **穩定連線**：一般比賽場地內穩定連線（空曠環境實測可達 100 公尺）
- 🕹️ **完整按鍵**：雙搖桿、類比扳機、方向鍵與所有功能鍵；I2C 版還能控制手把震動與燈條顏色

## 📊 掌控者系列比較

| 功能 | 2.4G 版 (PS2) | 藍牙版 (PS4/PS5) |
| :--- | :---: | :---: |
| 適用手把 | PS2 2.4G 無線手把 | PS4、PS5 手把 |
| 大型賽場多設備穩定性 | ⚠️ 可能干擾 | ✅ 只綁定自己的手把 |
| 配對記憶（開機即連） | ❌ | ✅ |
| 斷訊自動歸零 | — | ✅ 0.3 秒 |
| 連線距離 | 約 10-15 公尺 | 比賽場地內穩定（空曠實測 100 公尺） |
| 類比扳機支援 | ❌ | ✅ |

## 🧩 支援平台

| 平台 | 支援方式 |
| :--- | :--- |
| LEGO SPIKE Prime / Robot Inventor | 官方 App、Pybricks（👉 [SPIKE 新手範例：官方 App／MBC 積木網站／Pybricks 各 5 個](/sensors/ps4/spike-examples.md)） |
| LEGO SPIKE Essential | 不支援 |
| LEGO MINDSTORMS EV3 | EV3 官方軟體、Pybricks（[EV3 新手範例](/sensors/ps4/ev3-examples.md)） |
| Arduino / ESP32 / 樹莓派 | 通用 I2C（位址 `0x42`），[Arduino 範例](/sensors/ps4/arduino-examples.md) |
| MakeBlock（mBot 等） | 專屬版本，[積木與接法](/makeblock.md) |

> [!IMPORTANT]
> 每個平台是**不同的出廠版本**，請依你的主機選購對應版本。

## 🔗 配對方式

1. 將接收器插入 SPIKE（Port A~F）或 EV3（Port 1~4）並開機。通用 I2C 版：接收器 TX → SCL（UNO 的 A5）、RX → SDA（UNO 的 A4）、VCC → 5V、GND → GND，詳見 [Arduino 範例](/sensors/ps4/arduino-examples.md)。
2. **首次使用或要換手把**：按接收器上的 **Reset** 鍵，接收器會開放 **30 秒**配對時間。
3. 在這 30 秒內讓手把進入配對模式：
   - **PS4 手把**：同時長按 **SHARE + PS** 鍵，直到燈條快速閃爍
   - **PS5 手把**：同時長按 **建立（Create）+ PS** 鍵，直到燈條快速閃爍
4. 第一支連上的手把會被記住，接收器隨即關閉搜尋。之後只要開啟手把就會自動重連。

> [!TIP]
> 比賽現場請在自己的位置完成配對，避免 30 秒內有別隊手把也在配對模式。綁錯手把就再按一次 Reset 重新配對。

## 📥 範例下載

| 主機 | 檔案 |
| :--- | :--- |
| SPIKE App（教育版） | [ps4_demo_spike_prime_v3.4.3.llsp3](/examples/ps4/spike/ps4_demo_spike_prime_v3.4.3.llsp3) |
| Robot Inventor | [ps4_demo_robot_inventor_v10.5.0.lms](/examples/ps4/spike/ps4_demo_robot_inventor_v10.5.0.lms) |
| SPIKE（Pybricks 4.1） | [全部下載（.zip）](/examples/ps4/ps4_pybricks_spike_v4.x.zip)・[函式庫 MBC_gamepad_Lib.py](/examples/ps4/pybricks/MBC_gamepad_Lib.py)・[共用函式庫 MBC_sensors.py（新專案用）](/examples/ps4/pybricks/MBC_sensors.py)・[新手範例](/sensors/ps4/spike-examples.md) |
| SPIKE（Pybricks 3.6.1，不再更新） | 舊版檔案在[下載專區](/downloads.md)的「舊版（不再更新）」 |
| mBot 等（MakeBlock） | [積木擴展與手把遙控範例](/makeblock.md) |

## 📤 輸出資料

**搖桿數值：** EV3 為 `0` ~ `99`（`50` = 中心）；SPIKE（官方 App、Pybricks）為 `1` ~ `9`（`5` = 中心）。**往上推、往右推數值變大**。

### EV3 模式

| 模式 | 內容 |
| :---: | :--- |
| 0 | 方向鍵狀態 |
| 1 | 方向鍵、左搖桿 X、形狀鍵（▲ ● ✕ ■）、左搖桿 Y、肩鍵（L2 R2 L1 R1）、右搖桿 X、功能鍵（SHARE L3 R3 OPTIONS）、右搖桿 Y |
| 2 | 左搖桿 X、左搖桿 Y、右搖桿 X、右搖桿 Y |

## ∿ 通用 I2C 暫存器（位址 `0x42`）

共通規則與 Arduino 函式請看 [MBC 通用 I2C 協議](/i2c-protocol.md)。

> [!TIP]
> **不會寫程式也能用：** 看 [🧪 Arduino 範例](/sensors/ps4/arduino-examples.md)，每個範例都能直接複製上傳，每一行都有中文說明。

> [!NOTE]
> 接收器已內建電位轉換，可直接接 Arduino UNO 等 5 V 主機。寫完暫存器編號後要**等 2 ms 再讀**。

**讀法：** 寫 `[編號]` → 等 2 ms → 讀 2×k byte，就是從這個編號起連續 k 個值（每個值 int16、低位元組在前，k ≤ 16）；沒有的編號回 `-32768`。一次讀多個值保證是同一瞬間。

| 編號 | 值 | 內容 |
| :---: | :--- | :--- |
| `0x01` | 十六鍵 | 16 個鍵合成一個數（1 = 按下）：b0 SHARE、b1 L3、b2 R3、b3 OPTIONS、b4 上、b5 右、b6 下、b7 左、b8 L2、b9 R2、b10 L1、b11 R1、b12 ▲、b13 ●、b14 ✕、b15 ■；沒連線 = 0。⚠️ 當成 int16 讀時只按 ■ = `-32768`（跟「空號」一樣），請轉成 `uint16_t` 再判斷 |
| `0x02` | 方向鍵 | `0` ~ `15`：上 1、右 2、下 4、左 8（同時按相加） |
| `0x03` | 形狀鍵 | `0` ~ `15`：▲ 1、● 2、✕ 4、■ 8 |
| `0x04` | 肩鍵 | `0` ~ `15`：L2 1、R2 2、L1 4、R1 8 |
| `0x05` | 功能鍵 | `0` ~ `15`：SHARE 1、L3 2、R3 4、OPTIONS 8 |
| `0x06` | 已連線 | 1 = 手把已連線且 300 ms 內有資料、0 = 沒有（沒連線時按鍵、搖桿、扳機都是 0） |
| `0x07` | 電量 | `0` ~ `100` |
| `0x08`、`0x09` | L2、R2 類比扳機 | `0` ~ `255`（壓多深） |
| `0x0A` | 系統鍵 | b0 = PS 鍵、b1 = 擷取鍵（PS5：靜音鍵） |
| `0x0B` | 手把類型 | 0 = 沒有手把、3 = PS4 手把、4 = PS5 或其他藍牙手把 |
| `0x10` ~ `0x13` | 搖桿 LX、LY、RX、RY | `-100` ~ `+100`，0 = 中心，**右／上為正**，已套用死區 |

| 命令 | 參數 | 做什麼 |
| :---: | :--- | :--- |
| `E5` | [強度 `0` ~ `255`] | 手把震動 200 ms；`0` = 停 |
| `E6` | [R][G][B] 各 `0` ~ `255` | 燈條顏色，要剛好 3 byte；沒連線時先記住，連上再套用；預設藍 |

`F0` 裝置 ID：低 byte `0x42`、高 byte `2`（= PS4；PS2 接收器是 `1`）；`F1`～`F5`、`F8` 換位址、`EF` 重開機見 [MBC 通用 I2C 協議](/i2c-protocol.md)。Arduino 怎麼讀、每個值的完整說明在 [Arduino IDE 說明頁](/sensors/ps4/arduino/ide.md)。

```cpp
// 用 MBC 通用 I2C 協議頁的 mbcReadMany()／mbcWrite()（寫完編號會等 2 ms）
int16_t v[5];
if (mbcReadMany(0x42, 0x01, 5, v)) {   // 0x01 十六鍵、0x02 方向鍵、0x03 形狀鍵、0x04 肩鍵、0x05 功能鍵
  uint16_t btn = (uint16_t)v[0];
  bool cross = btn & (1 << 14);         // ✕ 鍵（b14）
  bool up = v[1] & 1;                   // 方向鍵：上 = 1
}
int16_t s[4];
mbcReadMany(0x42, 0x10, 4, s);          // 搖桿 LX, LY, RX, RY：-100 ~ +100

uint8_t red[3] = {255, 0, 0};
mbcWrite(0x42, 0xE6, red, 3);           // 燈條改成紅色（剛好 3 byte）
```
