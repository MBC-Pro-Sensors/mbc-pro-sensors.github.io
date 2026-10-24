<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# SPIKE 6 路舵機擴充器 (Servo6)（即將推出）

<!-- product-hero -->

<!-- platform-grid -->

---

> [!NOTE]
> 本產品已完成電路與韌體，正在做最後的主機實測，規格以正式上市版本為準。想搶先試用或詢價，歡迎 [聯絡我們](/contact.md)。

> [!WARNING]
> **⚠️ 購買前請確認**
>
> | 項目 | 說明 |
> | :--- | :--- |
> | **相容主機** | SPIKE™ Prime 限定（不支援 Robot Inventor 與 EV3） |
> | **SPIKE 官方軟體** | 僅支援 Python 文字模式，不支援圖控積木模式 |
> | **Pybricks** | 需要 Pybricks 韌體 **v4.0 以上**；第一次執行要在主控台按 **Y** 同意供電 |
> | **舵機電源** | 由主機電池經擴充器供電，**總電流上限約 0.75 A**；大扭力舵機請先確認工作電流 |

---

## 🚀 產品概述：SPIKE 也能用舵機

樂高 SPIKE Prime 沒有舵機（伺服馬達）可以用，但夾爪、轉盤、擺臂這類「轉到一個角度就停住」的機構，用舵機最省空間、最便宜。

**SPIKE 6 路舵機擴充器 (Servo6)** 插在主機任一孔，就能接 **6 顆一般 3 線舵機**（SG90、MG996R 這類 500~2500 µs 訊號的舵機）。角度、速度、電流回報、堵轉保護全部在擴充器上運算，主機只要下指令。

## 🧠 核心硬體特徵

- ⚡ **免外接電池**：舵機電源直接取自主機電池（M1/M2 供電線），經整流與電子保險絲（eFuse）限流，不需要再帶一顆電池盒。
- 🛡️ **每孔獨立量電流**：6 路各有檢流電阻，隨時讀到每顆舵機的電流；電流超過門檻持續一段時間就判定堵轉，可自動放鬆該孔保護舵機與齒輪。
- 🔌 **隔離保護主機**：訊號線用數位隔離器、控制晶片用隔離式 DC-DC 供電，舵機端短路或突波不會打到主機。
- 🎯 **兩種舵機都能用**：定點舵機用「轉到角度」（可設定轉速，1~99 對應 6~594 °/s，100 = 立即到位）；360° 連續旋轉舵機用「速度」（−100~100）。
- 🔧 **可校正**：每孔可設定最小／最大脈寬與角度範圍（預設 500~2500 µs、180°），也可以直接下脈寬。
- 🧷 **不需要心跳**：舵機指令是「設定成某個狀態」，主機程式結束後舵機維持最後位置，不會暴衝。

## 🔌 硬體接線與啟動

1. **主機通訊線**：把擴充器的 SPIKE 連接線插進主機任一孔（Port A~F）。
2. **接舵機**：舵機 3 線插頭依板上標示（訊號／VCC／GND）插到 CN1~CN6。
3. **不用接電池**：主機握手完成後會自動把電池電壓送到擴充器，綠燈由快閃轉為恆亮。
4. **寫程式**：依你的環境看 [官方 SPIKE App](/sensors/servo6/spike-official.md) 或 [Pybricks](/sensors/servo6/spike-pybricks.md)。

> [!TIP]
> 剛上電時擴充器不會輸出任何脈衝，舵機不會亂跳；收到第一個角度指令時會直接到位（不套速度斜坡），之後的指令才會用你設定的速度慢慢轉。

## 📐 指令一覽（兩個環境函式名稱相同）

| 函式 | 功能 |
| :--- | :--- |
| `servo_angle(port, angle, speed=100)` | 轉到角度 0~180（依校正範圍），`speed` 1~99 = 每秒 `speed×6` 度，100 = 立即到位 |
| `servo_wait(port, timeout_ms=10000)` | 等該孔轉到位 |
| `servo_run(port, speed)` | 360° 連續旋轉舵機：−100~100，0 停止 |
| `servo_pulse(port, pulse_us)` | 直接下脈寬 500~2500 µs（校正或特殊舵機） |
| `servo_release(port)` / `stop_all()` | 放鬆該孔／全部放鬆（停止輸出脈衝） |
| `servo_calibrate(port, min_us, max_us, range_deg)` | 設定該孔的脈寬範圍與角度範圍 |
| `servo_stall_config(port, current_ma, time_ms, auto_release)` | 堵轉門檻（預設 400 mA 持續 300 ms 自動放鬆，正式值待實測定案） |
| `get_current(port)` / `get_total_current()` | 該孔電流 mA／總電流 mA |
| `is_moving(port)` / `is_stalled(port)` / `is_active(port)` | 轉動中／堵轉旗標／有輸出脈衝 |
| `is_power_fault()` | 電子保險絲是否跳脫 |

## 🧩 支援平台

| 平台 | 支援方式 |
| :--- | :--- |
| LEGO SPIKE Prime（官方 App） | Python 文字模式 ＋ [專屬函式庫](/sensors/servo6/spike-official.md) |
| LEGO SPIKE Prime（Pybricks v4.0+） | [專屬函式庫](/sensors/servo6/spike-pybricks.md)，支援一般與多工（async）兩種寫法 |
| Robot Inventor、EV3 | 不支援 |

## 📥 下載

- [📥 官方 SPIKE App 函式庫＋範例（.zip）](/downloads/MBC_Servo6_Official_App_Lib.zip)
- [📥 Pybricks 函式庫＋範例（.zip）](/downloads/MBC_Servo6_Pybricks_Lib.zip)
