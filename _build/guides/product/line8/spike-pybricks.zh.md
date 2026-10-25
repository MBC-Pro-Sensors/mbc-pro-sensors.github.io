> [!TIP]
> **🧩 第一次用？跟著互動教學一步一步做：**[MicroPython 版](/learn/line8/micropython.html)・[Pybricks 積木版](/learn/line8/pybricks-blocks.html)。左邊點選、右邊程式跟著長出來，每個功能都有範例，還有模擬感應器可以拖線看數值。

### 先校準（只要做一次）

循行者要先「認識」你場地上的黑色和白色，數值才會準。校準完會記在感應器裡，重新開機也還在（除非換場地）。

1. 感應器插好、主機開機（感應器的燈亮起來）。
2. 按住感應器上的**校準按鈕約 0.5 秒**再放開（不要一直按著，按著不放會一直重新校準）。
3. **綠色校準指示燈**（靠近校準按鈕）亮起後，**5 秒內**把感應器在黑線和白色地面之間來回掃幾次，8 個感應點都要掃過**最黑的黑線**和**最白的白色區域**。
4. 掃的時候感應器離地 **0.5～1.2 公分**，太高會讀不準。
5. 綠燈熄滅＝校準完成，數值自動存進感應器。

**怎麼知道校準好了？** 放在白色地面時「線寬」是 0，壓在一般黑線上是 2～3。

<iframe src="https://www.youtube.com/embed/TKi7b20x8UY" title="循行者校準教學" frameborder="0" allowfullscreen loading="lazy" style="width:100%;aspect-ratio:16/9;border:none;border-radius:12px;"></iframe>

### 函式庫與範例下載

主機要是 **Pybricks 韌體 v4.1**。zip 裡有 `MBC_sensors.py`、`MBC_line8_obj_Lib.py` 和 4 支範例：

- [📥 打包下載（Pybricks 4.1）](/examples/line8/line8_pybricks_spike_v4.x.zip)

單獨下載函式庫：[MBC_sensors.py](/examples/line8/pybricks/For%20firmware%204.x/MBC_sensors.py)（新專案用這個，一個檔適用全部 MBC 感應器）・[MBC_line8.py](/examples/line8/pybricks/For%20firmware%204.x/MBC_line8.py)（只接循行者 8 路時用，檔案小、上傳快：`from MBC_line8 import MBC_LINE8`）・[MBC_line8_obj_Lib.py](/examples/line8/pybricks/For%20firmware%204.x/MBC_line8_obj_Lib.py)（本頁範例與舊程式用）

> [!IMPORTANT]
> 範例程式寫的是 `from MBC_line8_obj_Lib import MBC_LINE8`，所以要匯入 `MBC_line8_obj_Lib.py`（檔名不用改）。自己寫新程式改成 `from MBC_sensors import MBC_LINE8`，只匯入 `MBC_sensors.py`。函式庫要跟主程式放在**同一個專案列表**裡。

> [!NOTE]
> Pybricks 3.6.1 版已不再更新，舊檔放在[下載專區](/downloads.md)的「舊版（不再更新）」。

![把函式庫匯入 Pybricks 專案](/images/sensors/line8/pybricks-upload-lib.webp)

### 方式一：內建感應器指令（不用函式庫）

直接用 Pybricks 內建的 `ColorSensor` 指令讀，不用載入額外檔案。下面的值是用內建指令算出來的：

| 組合 | 函數 | 範圍 | 說明 |
| :--- | :--- | :--- | :--- |
| 基礎（比較不挑校準品質，但還是要先校準一次） | `getLinePos8()` | `-8 ~ +8` | 循線位置（整數），0 正中央，負偏左、正偏右 |
| | `getLineWidth8()` | `0 ~ 8` | 特徵線寬（觸發的感測器數量），偵測十字、T 字路口 |
| 高精度（建議先確實校準） | `getLinePos100()` | `-100 ~ +100` | 高解析插值位置，適合高階 PID |
| | `getLineWidth100()` | `0 ~ 8` | 高精度模式的線寬，跟 `getLinePos100()` 同步 |

> [!TIP]
> 位置和線寬要用**同一組**：`getLinePos8()` 配 `getLineWidth8()`，`getLinePos100()` 配 `getLineWidth100()`，讀到的才是同一刻的資料。

範例：積木版 [line8_block_native.py](/examples/line8/pybricks/For%20firmware%204.x/line8_block_native.py)；Python 版 [line8_python_native.py](/examples/line8/pybricks/For%20firmware%204.x/line8_python_native.py)

### 方式二：匯入專屬函式庫

| 函數 | 範圍 | 說明 |
| :--- | :--- | :--- |
| `line_init_port_multitask(port, multitask)` | — | 初始化。`port` = 1~6 對應 A~F；`multitask=True` 啟用非同步模式（搭配 `await`） |
| `line_pos8()` | `-8 ~ +8` | 整數循線位置，比較不挑校準品質（沒校準時每一路都會被當成黑色） |
| `line_pos100()` | `-100 ~ +100` | 高解析插值位置，最佳 PID 選擇，要確實校準 |
| `line_width()` | `0 ~ 8` | 特徵線寬，兩種位置函數都適用 |
| `line_ir_calibration()` | `list[8]` | 8 路校正後光值陣列（每值 `0 ~ 100`）。從車子後面往前看：index 0 = 最左（ch1） |
| `line_ir_ch(ch)` | `0 ~ 100` | 單一通道校正光值，`ch` = 1~8，可連續呼叫（共用快取） |
| `line_junction()` | `0, 1, 2+` | 線群數量：0 無線、1 單線、2 以上路口分叉 |
| `line_junction_name()` | `str` | `"none"`、`"line"`、`"junction"` |
| `line_bin_raw()` | `0 ~ 255` | 8 位元觸發遮罩，bit0 = ch1，1 = 偵測到線 |
| `line_on_sensors()` | `list[int]` | 偵測到線的通道編號，例如 `[4, 5]` |
| `line_read_all()` | `dict` | 一次讀全部：`ir_calibration`、`line_pos8`、`line_pos100`、`line_width`、`bin_raw`、`on_sensors`、`junction`、`junction_name` |

範例：積木版 [line8_block_with_lib.py](/examples/line8/pybricks/For%20firmware%204.x/line8_block_with_lib.py)；Python 版 [line8_python_with_lib.py](/examples/line8/pybricks/For%20firmware%204.x/line8_python_with_lib.py)

### 想學更厲害的循線方法？

可以找這幾位有豐富比賽與教學經驗的教練：[LegoLauXiao 教練](https://www.youtube.com/@LegoLauXiao)（[示範影片](https://www.youtube.com/watch?v=WgacdWLatbk)）、[legolaumo 教練](https://www.youtube.com/@legolaumo)（[示範影片](https://www.youtube.com/watch?v=T9bcndBNQvQ)）。
