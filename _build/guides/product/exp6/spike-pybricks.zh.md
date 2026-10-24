### 接線與供電

1. **主機通訊線**：把擴充器的 **Host 通訊線**插進 SPIKE 主機任一孔（Port A~F）。
2. **獨立電源**：擴充器一定要接額外的獨立電源（電池盒或變壓器），專門供電給馬達；馬達電流不會從主機抽，主機訊號線也經過隔離晶片。
3. **接裝置**：樂高感應器（顏色、超音波、力量）與馬達，任意接在擴充器的 **1～6 號**孔。

> [!WARNING]
> EXP6 **只支援 SPIKE Prime**（不支援 Robot Inventor 與 EV3）。**換另一種寫法之前，先讓 EXP6 斷電一次**（關主機或拔掉 Host 線）：EXP6 一旦跑過 Pybricks 程式，就會一直用 Pybricks 的方式溝通，直到重新開機，這時候官方 App 會連不上。

### 匯入 EXP6 函式庫

1. 下載 [📥 Pybricks 函式庫 v2＋範例（.zip）](/downloads/MBC_EXP6_Pybricks_Lib_v2.zip)（2026-10 以前出貨的 v1 韌體用 [舊版 zip](/downloads/MBC_EXP6_Pybricks_Lib.zip)）。說明文件針對 **Pybricks 4.0.0 以上**。
2. 把函式庫放進 Pybricks 專案。範例寫的是 `from MBC_exp6_obj_Lib import MBC_EXP6`，所以檔名要是 `MBC_exp6_obj_Lib.py`（v2 包裡的 `MBC_exp6_obj_Lib_v2.py` 要改名）。

![EXP6 Pybricks 範例](/images/sensors/exp6/exp6_pybricks_example.webp)

### 一般模式與多工模式

假設擴充器接在主機的 **Port C（孔 3）**：

```python
# 一般模式：一行一行執行
from MBC_exp6_obj_Lib import MBC_EXP6
exp6 = MBC_EXP6(3, False)
exp6.motor_power(6, 100)
exp6.keep_alive_wait(5000)
```

```python
# 多工模式：每個指令前加 await，要用函式庫的 run_task
from pybricks.tools import multitask, run_task, wait
from MBC_exp6_obj_Lib import MBC_EXP6, run_task
exp6 = MBC_EXP6(3, True)

async def main():
    await exp6.motor_power(6, 100)
    await exp6.keep_alive_wait(5000)

run_task(main())
```

> [!IMPORTANT]
> **Pybricks 有看門狗**：擴充器約 **0.5 秒**沒收到主機訊號（斷線或程式當掉）就讓馬達滑行停止。多工模式的 `run_task` 會在背景自動送心跳；一般模式等待時要用 `exp6.keep_alive_wait(毫秒)` 取代 `wait`，耗時的迴圈裡要呼叫 `exp6.keep_alive()`。
