<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# 掌控者 PS4/PS5 藍牙遙控接收器

<div style="text-align: center;">
  <span style="display:inline-block; background:#F2C200; color:#0a0a0a; padding:3px 12px; border-radius:20px; font-family:monospace; margin-bottom:1rem; font-size: 0.85rem;">CMD-BT-PRO · PS4 / PS5</span>
  <br>
  <img src="/images/sensors/ps4/ps4-product.webp" alt="掌控者 PS4/PS5 藍牙遙控接收器" style="max-width: 280px; margin: 1rem auto; display: block; filter: drop-shadow(0 0 30px rgba(242,194,0,0.2));" />
  <p style="font-family: monospace; color: #aaa; letter-spacing: 1px;">
    <strong>PS4 / PS5 手把專用藍牙接收器</strong><br>
    只綁定你的手把 · 開機自動重連 · 斷訊自動停車
  </p>
</div>

---

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
| LEGO SPIKE Prime / Robot Inventor | 官方 App、Pybricks |
| LEGO MINDSTORMS EV3 | EV3 官方軟體、Pybricks |
| Arduino / ESP32 / 樹莓派 | 通用 I2C（位址 `0x42`） |
| MakeBlock | 專屬版本 |

> [!IMPORTANT]
> 每個平台是**不同的出廠版本**，請依你的主機選購對應版本。

## 🔗 配對方式

1. 將接收器插入 SPIKE（Port A~F）或 EV3（Port 1~4）並開機。
2. **首次使用或要換手把**：按接收器上的 **Reset** 鍵，接收器會開放 **30 秒**配對時間。
3. 在這 30 秒內讓手把進入配對模式：
   - **PS4 手把**：同時長按 **SHARE + PS** 鍵，直到燈條快速閃爍
   - **PS5 手把**：同時長按 **建立（Create）+ PS** 鍵，直到燈條快速閃爍
4. 第一支連上的手把會被記住，接收器隨即關閉搜尋。之後只要開啟手把就會自動重連。

> [!TIP]
> 比賽現場請在自己的位置完成配對，避免 30 秒內有別隊手把也在配對模式。綁錯手把就再按一次 Reset 重新配對。

## 📤 輸出資料

**搖桿數值（SPIKE / EV3）：** `0` ~ `99`，`50` = 中心，**往上推、往右推數值變大**。

### EV3 模式

| 模式 | 內容 |
| :---: | :--- |
| 0 | 方向鍵狀態 |
| 1 | 方向鍵、左搖桿 X、形狀鍵（■ ✕ ● ▲）、左搖桿 Y、肩鍵（L1 R1 L2 R2）、右搖桿 X、功能鍵（START SELECT L3 R3）、右搖桿 Y |
| 2 | 左搖桿 X、左搖桿 Y、右搖桿 X、右搖桿 Y |

## ∿ 通用 I2C 暫存器（位址 `0x42`）

共通規則與 Arduino 函式請看 [MBC 通用 I2C 協議](/i2c-protocol.md)。

> [!WARNING]
> PS4/PS5 接收器使用 ESP32：**I2C 腳位不耐 5 V**，接 Arduino UNO 等 5 V 主機請加準位轉換；寫完暫存器編號後要**等 2 ms 再讀**。

| 暫存器 | 方向 | 長度 | 內容 |
| :---: | :---: | :---: | :--- |
| `0x10` | W | 1 | 手把震動：強度 `0` ~ `255`，持續 200 ms；`0` = 停止 |
| `0x11` | R | 9 | 手把合併包（= `0x12` ~ `0x15`） |
| `0x12` | R | 2 | 按鈕 uint16（b15 ■、b14 ✕、b13 ●、b12 ▲、b11 R1、b10 L1、b9 R2、b8 L2、b7 左、b6 下、b5 右、b4 上、b3 START、b2 R3、b1 L3、b0 SELECT） |
| `0x13` | R | 4 | 搖桿 LX, LY, RX, RY：int8 `-127` ~ `+127`，0 = 中心，**右／上為正**，已套用死區 |
| `0x14` | R | 2 | 類比扳機 L2, R2：`0` ~ `255` |
| `0x15` | R | 1 | 系統鍵（PS 鍵等） |
| `0x22` | R | 4 | 簡易搖桿 LX, LY, RX, RY：`0` ~ `99`，50 = 中心（與 SPIKE / EV3 版相同） |
| `0x23` | RW | 3 | 燈條顏色 R, G, B（一次寫 3 bytes；未連線時先記住，連上後套用） |
| `0x32` | R | 1 | bit0 = 就緒、bit1 = 手把已連線 |
| `0x34` | R | 1 | 手把類型：0 = 未連線、3 = PS4 手把、4 = PS5 或其他藍牙手把 |
| `0x35` | R | 1 | 手把電量 `0` ~ `255` |

`0x06` 款式第 1 byte = `2`（PS2 接收器為 `1`）。未連線時按鈕、搖桿、扳機都讀回 0。

```cpp
// 讀按鈕與搖桿（記得 iicRead 內要保留 delay(2)）
uint8_t d[9];
if (iicRead(0x42, 0x11, d, 9)) {
  uint16_t btn = (d[0] << 8) | d[1];
  int8_t lx = (int8_t)d[2], ly = (int8_t)d[3];
  bool cross = btn & (1 << 14);              // ✕ 鍵
}

// 燈條改成紅色
Wire.beginTransmission(0x42);
Wire.write(0x23); Wire.write(255); Wire.write(0); Wire.write(0);
Wire.endTransmission();
```
