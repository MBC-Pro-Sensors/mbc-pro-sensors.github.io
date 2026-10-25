> [!NOTE]
> Servo6 **即將推出**：已完成電路與韌體，正在做最後的主機實測，規格以正式上市版本為準。只支援 **SPIKE Prime**（不支援 Robot Inventor 與 EV3）。

### 接線與供電

1. **主機通訊線**：把擴充器的 SPIKE 連接線插進主機任一孔（Port A~F）。
2. **接舵機**：舵機 3 線插頭依板上標示（訊號／VCC／GND）插到 **CN1~CN6**，可接 SG90、MG996R 這類 500~2500 µs 訊號的舵機。
3. **不用接電池**：主機握手完成後會自動把電池電壓送到擴充器，綠燈由快閃轉為恆亮。舵機電源**總電流上限約 0.75 A**，大扭力舵機請先確認工作電流。

> [!TIP]
> 剛上電時擴充器不會輸出任何脈衝，舵機不會亂跳；收到第一個角度指令時會直接到位（不套速度斜坡），之後的指令才會用你設定的速度慢慢轉。

### 放進函式庫

1. 主機要是 Pybricks 韌體 **v4.0 以上**。
2. 下載 [📥 Pybricks 函式庫＋範例（.zip）](/downloads/MBC_Servo6_Pybricks_Lib.zip)，把 `MBC_servo6_Lib.py` 放進 Pybricks 專案（跟主程式同一層），就能 `import`。

### 第一次執行要按 Y

函式庫用 `UARTDevice(port, 115200, power_pin=1)` 主動要求主機把電池電壓送到擴充器。**第一次執行**主控台會出現：

```
Custom electronics may damage your LEGO hub ... Press Y to proceed.
```

按 **Y**（只需一次，主機會記住）。按 N 的話舵機不會有電。

```python
from pybricks.tools import wait
from MBC_servo6_Lib import MBC_SERVO6
sv = MBC_SERVO6(1, False)      # 擴充器插在 Port A（1=A … 6=F），False = 一般模式
sv.servo_angle(1, 90)          # CN1 轉到 90°
wait(1000)
sv.stop_all()                  # 程式結束前放鬆所有舵機
```

> [!TIP]
> 擴充器**沒有看門狗**，程式結束後舵機會維持最後位置；要放鬆請在程式最後呼叫 `stop_all()`（多工寫法用函式庫版 `run_task` 會自動做）。堵轉自動放鬆後，再送**同一個角度**不會重新上力，請先送不同角度或先 `servo_release()`。
