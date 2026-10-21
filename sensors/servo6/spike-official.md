<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# 官方 SPIKE App 專區 (Servo6)

[🔙 回到 Servo6 首頁](/sensors/servo6/index.md)

> [!WARNING]
> **您目前觀看的是 【官方 SPIKE App】 教學。** 適用於 SPIKE Prime 官方軟體（SPIKE 3）的 **Python 文字模式**；不支援圖控積木。如果您的主機刷了 Pybricks，請點擊 [這裡切換至 Pybricks 教學](/sensors/servo6/spike-pybricks.md)。

## 📥 下載函式庫與範例

[📥 官方 SPIKE App 函式庫＋範例（.zip）](/downloads/MBC_Servo6_Official_App_Lib.zip)

官方 App 的 Python 專案不能另外放檔案，所以把 `MBC_servo6_SPIKE_App_Lib.py` 的**整份內容貼在你程式的最上方**，再接著寫自己的程式。

## 🧠 原理（為什麼不用改主機）

擴充器對主機偽裝成 **SPIKE 超音波感應器**：指令透過超音波的「燈光 4 格數值」送出，狀態從 `device.data()` 讀回 8 筆數值。官方系統會在背景一直重送最後一包燈光值，所有指令都是「設定成某個狀態」，重送沒有副作用。每個指令會等一個系統輪詢週期（約 20~50 ms），連續控制 6 顆舵機大約需要 6 個週期。

## 📚 範例

```python
from hub import port
import time
# ↑ 上面先貼 MBC_servo6_SPIKE_App_Lib.py 的內容，或照下行匯入
from MBC_servo6_SPIKE_App_Lib import MBC_SERVO6

sv = MBC_SERVO6(port.A)        # 擴充器插在 Port A

sv.servo_angle(1, 90)          # CN1 轉到 90°
time.sleep_ms(1000)
sv.servo_angle(1, 0, 10)       # 以 60°/s 慢慢轉到 0°
sv.servo_wait(1)
sv.servo_angle(1, 180, 10)
sv.servo_wait(1)
print("CN1 電流 mA:", sv.get_current(1), " 總電流 mA:", sv.get_total_current())

sv.servo_run(2, 50)            # CN2 是 360° 舵機：半速正轉
time.sleep_ms(2000)
sv.servo_run(2, -50)
time.sleep_ms(2000)
sv.servo_run(2, 0)

print("CN1 堵轉:", sv.is_stalled(1), " eFuse 故障:", sv.is_power_fault())
sv.stop_all()
```

## 📐 函式一覽

| 函式 | 功能 |
| :--- | :--- |
| `MBC_SERVO6(port.X)` | 建立物件，`port.A`~`port.F` |
| `servo_angle(ch, angle, speed=100)` | 轉到角度；`speed` 1~99 = 每秒 `speed×6` 度，100 = 立即到位 |
| `servo_wait(ch, timeout_ms=10000)` | 等轉到位 |
| `servo_run(ch, speed)` | 360° 舵機：−100~100，0 停 |
| `servo_speed_config(ch, stop_us=1500, span_us=1000)` | 360° 舵機的停止脈寬與滿速偏移 |
| `servo_pulse(ch, pulse_us)` | 直接下脈寬 500~2500 µs |
| `servo_release(ch)` / `stop_all()` | 放鬆該孔／全部放鬆 |
| `servo_calibrate(ch, min_us=500, max_us=2500, range_deg=180)` | 校正脈寬與角度範圍 |
| `servo_stall_config(ch, current_ma=400, time_ms=300, auto_release=True)` | 堵轉門檻（0 = 關閉） |
| `get_current(ch)` / `get_total_current()` | 電流 mA |
| `is_moving(ch)` / `is_stalled(ch)` / `is_active(ch)` / `is_power_fault()` | 狀態 |

> [!TIP]
> 官方 App 程式結束時不會自動放鬆舵機（舵機維持最後位置，不會暴衝）；想放鬆請在程式最後呼叫 `stop_all()`。
