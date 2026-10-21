<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# Pybricks 專業環境專區 (Servo6)

[🔙 回到 Servo6 首頁](/sensors/servo6/index.md)

> [!WARNING]
> **您目前觀看的是 【Pybricks 專業環境】 教學。** 適用於刷入 Pybricks 韌體（**v4.0 以上**）的 SPIKE Prime。如果您使用的是官方 SPIKE App，請點擊 [這裡切換至官方教學](/sensors/servo6/spike-official.md)。

## 📥 下載函式庫與範例

[📥 Pybricks 函式庫＋範例（.zip）](/downloads/MBC_Servo6_Pybricks_Lib.zip)　→ 把 `MBC_servo6_Lib.py` 放進 Pybricks 專案（跟主程式同一層），即可 `import`。

## 🔑 第一次執行一定要知道的事

Pybricks 開 `UARTDevice` 時，主機不會把擴充器當成「需要供電的感測器」，所以函式庫用 `UARTDevice(port, 115200, power_pin=1)` 主動要求主機把電池電壓送到擴充器。**第一次執行**主控台會出現：

```
Custom electronics may damage your LEGO hub ... Press Y to proceed.
```

按 **Y**（只需一次，主機會記住）。按 N 的話舵機不會有電。

## 📚 一般寫法（一行一行執行）

```python
from pybricks.tools import wait
from MBC_servo6_Lib import MBC_SERVO6

sv = MBC_SERVO6(1, False)      # 擴充器插在 Port A（1=A … 6=F），False = 一般模式

sv.servo_angle(1, 90)          # CN1 轉到 90°（第一次指令直接到位）
wait(1000)
sv.servo_angle(1, 0, 10)       # 以 60°/s（10×6）慢慢轉到 0°
sv.servo_wait(1)               # 等它到位
sv.servo_angle(1, 180, 10)
while sv.is_moving(1):
    print("CN1 電流 mA:", sv.get_current(1), " 總電流 mA:", sv.get_total_current())
    wait(100)

sv.servo_run(2, 50)            # CN2 是 360° 舵機：半速正轉
wait(2000)
sv.servo_run(2, -50)           # 半速反轉
wait(2000)
sv.servo_run(2, 0)             # 停

print("CN1 堵轉:", sv.is_stalled(1), " eFuse 故障:", sv.is_power_fault())
sv.stop_all()                  # 程式結束前放鬆所有舵機
```

## ⚡ 多工寫法（async，可以同時做很多事）

```python
from pybricks.tools import wait, run_task
from MBC_servo6_Lib import MBC_SERVO6, run_task   # 用函式庫版 run_task：程式結束自動放鬆舵機

sv = MBC_SERVO6(1, True)       # True = 多工模式，每個呼叫前要加 await

async def gripper():
    await sv.servo_angle(1, 30, 20)
    await sv.servo_wait(1)
    await sv.servo_angle(1, 120, 20)

async def main():
    await gripper()

run_task(main())
```

## 📐 函式一覽

| 函式 | 功能 |
| :--- | :--- |
| `MBC_SERVO6(port, multitask)` | 建立物件；`port` 1~6 = Port.A~F，`multitask` True 時每個呼叫前加 `await` |
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
> 擴充器**沒有看門狗**，程式結束後舵機會維持最後位置；要放鬆請在程式最後呼叫 `stop_all()`（多工寫法用函式庫版 `run_task` 會自動做）。堵轉自動放鬆後，再送**同一個角度**不會重新上力（指令去重），請先送不同角度或先 `servo_release()`。
