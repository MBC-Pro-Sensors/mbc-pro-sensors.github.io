> [!NOTE]
> Servo6 **即將推出**：已完成電路與韌體，正在做最後的主機實測，規格以正式上市版本為準。只支援 **SPIKE Prime**（不支援 Robot Inventor 與 EV3）。

### 接線與供電

1. **主機通訊線**：把擴充器的 SPIKE 連接線插進主機任一孔（Port A~F）。
2. **接舵機**：舵機 3 線插頭依板上標示（訊號／VCC／GND）插到 **CN1~CN6**，可接 SG90、MG996R 這類 500~2500 µs 訊號的舵機。
3. **不用接電池**：主機握手完成後會自動把電池電壓送到擴充器，綠燈由快閃轉為恆亮。舵機電源**總電流上限約 0.75 A**，大扭力舵機請先確認工作電流。

> [!TIP]
> 剛上電時擴充器不會輸出任何脈衝，舵機不會亂跳；收到第一個角度指令時會直接到位（不套速度斜坡），之後的指令才會用你設定的速度慢慢轉。

### 放進函式庫

官方 App **只支援 Python 文字模式**，不支援圖控積木。

1. 下載 [📥 官方 SPIKE App 函式庫＋範例（.zip）](/downloads/MBC_Servo6_Official_App_Lib.zip)。
2. 官方 App 的 Python 專案不能另外放檔案，所以把 `MBC_servo6_SPIKE_App_Lib.py` 的**整份內容貼在你程式的最上方**，再接著寫自己的程式。

```python
from hub import port
import time
# ↑ 上面先貼 MBC_servo6_SPIKE_App_Lib.py 的內容
sv = MBC_SERVO6(port.A)        # 擴充器插在 Port A
sv.servo_angle(1, 90)          # CN1 轉到 90°
time.sleep_ms(1000)
sv.stop_all()
```

擴充器對主機偽裝成 **SPIKE 超音波感應器**：指令透過超音波的「燈光 4 格數值」送出，狀態從 `device.data()` 讀回。每個指令會等一個系統輪詢週期（約 20~50 ms），連續控制 6 顆舵機大約需要 6 個週期。

> [!TIP]
> 官方 App 程式結束時不會自動放鬆舵機（舵機維持最後位置，不會暴衝）；想放鬆請在程式最後呼叫 `stop_all()`。
