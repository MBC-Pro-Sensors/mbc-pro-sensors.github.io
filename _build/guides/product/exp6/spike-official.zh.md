### 接線與供電

1. **主機通訊線**：把擴充器的 **Host 通訊線**插進 SPIKE 主機任一孔（Port A~F）。
2. **獨立電源**：擴充器一定要接額外的獨立電源（電池盒或變壓器），專門供電給馬達；馬達電流不會從主機抽，主機訊號線也經過隔離晶片。
3. **接裝置**：樂高感應器（顏色、超音波、力量）與馬達，任意接在擴充器的 **1～6 號**孔。

> [!WARNING]
> EXP6 **只支援 SPIKE Prime**（不支援 Robot Inventor 與 EV3）。**換另一種寫法之前，先讓 EXP6 斷電一次**（關主機或拔掉 Host 線）：EXP6 一旦跑過 Pybricks 程式，就會一直用 Pybricks 的方式溝通，直到重新開機，這時候官方 App 會連不上。

### 安裝 EXP6 函式庫（只要做一次）

官方 App 沒有 EXP6 的積木，**只能用 Python 文字模式**（SPIKE 3；函式庫標示適用 3.4.3 版）。函式庫用純 Python 寫，不支援多工（async），所有指令都是**同步（Sync）**執行。

1. 下載 [📥 官方 App 函式庫 v2＋範例（.zip）](/downloads/MBC_EXP6_Official_App_Lib_v2.zip)（2026-10 以前出貨的 v1 韌體用 [舊版 zip](/downloads/MBC_EXP6_Official_App_Lib.zip)）。
2. 把下載包 `SPIKE_Education` 資料夾裡的**安裝腳本**（`Install_MBC_exp6_Lib…py`）內容貼進 SPIKE App 的 Python 專案，執行一次。腳本會把核心函式庫寫進主機。
3. 安裝成功後，任何新專案都能直接 `from MBC_exp6_SPIKE_App_Lib import MBC_EXP6`，不用再貼一長串原始碼。
4. 不再需要時，執行下載包裡的**解除安裝腳本**（`Uninstall_MBC_exp6_Lib…py`），就能把函式庫從主機乾淨移除。

![在 SPIKE App 執行安裝腳本](/images/sensors/exp6/exp6_official_app_install.webp)

### 建立連線物件

假設 EXP6 接在主機的 **Port F**：

```python
from hub import port
import time
from MBC_exp6_SPIKE_App_Lib import MBC_EXP6

exp6 = MBC_EXP6(port.F)   # 擴充器接在主機的 Port F
time.sleep(1)             # 等 1 秒讓擴充器就緒

exp6.motor_power(1, 50)
time.sleep(3)
exp6.stop_all(1)          # 程式結束前一定要讓所有馬達停止（1 = 滑行）
```

> [!IMPORTANT]
> **官方 App 沒有看門狗**：程式當掉或主機斷線時，馬達會一直維持最後一道指令。程式最後一定要寫 `exp6.stop_all(1)`；等待直接用 `time.sleep()`，不需要心跳指令。
