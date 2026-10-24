### 先校準（只要做一次）

循行者要先「認識」你場地上的黑色和白色，數值才會準。校準完會記在感應器裡，重新開機也還在（除非換場地）。

1. 感應器插好、主機開機（感應器的燈亮起來）。
2. 按住感應器上的**校準按鈕約 0.5 秒**再放開（不要一直按著，按著不放會一直重新校準）。
3. **綠色校準指示燈**（靠近校準按鈕）亮起後，**5 秒內**把感應器在黑線和白色地面之間來回掃幾次，16 個感應點都要掃過**最黑的黑線**和**最白的白色區域**。
4. 掃的時候感應器離地 **0.5～1.2 公分**，太高會讀不準。
5. 綠燈熄滅＝校準完成，數值自動存進感應器。

**怎麼知道校準好了？** 放在白色地面時「線寬」是 0，壓在一般黑線上是 3～5。

<iframe src="https://www.youtube.com/embed/TKi7b20x8UY" title="循行者校準教學" frameborder="0" allowfullscreen loading="lazy" style="width:100%;aspect-ratio:16/9;border:none;border-radius:12px;"></iframe>

### 函式庫與範例下載

依主機的 Pybricks 韌體版本選一包（zip 裡有函式庫＋4 支範例）：

- [📥 打包下載（韌體 4.0.0 版）](/examples/line16/line16_pybricks_spike_v4.0.0.zip)
- [📥 打包下載（韌體 3.6.1 版）](/examples/line16/line16_pybricks_spike_v3.6.1.zip)

單獨下載函式庫：[4.0.0 版 MBC_line16_obj_Lib_v400.py](/examples/line16/pybricks/For%20firmware%204.0.0/MBC_line16_obj_Lib_v400.py)・[3.6.1 版 MBC_line16_Lib_v361.py](/examples/line16/pybricks/For%20firmware%203.6.1/MBC_line16_Lib_v361.py)

> [!IMPORTANT]
> **檔名一定要改**：範例程式寫的是 `import MBC_line16_obj_Lib`（3.6.1 版是 `MBC_line16_Lib`），下載後要把檔名改成 `MBC_line16_obj_Lib.py`（去掉 `_v400`）或 `MBC_line16_Lib.py`（去掉 `_v361`），否則會出現 `ImportError`。函式庫要跟主程式放在**同一個專案列表**裡。

![把函式庫匯入 Pybricks 專案](/images/sensors/line16/pybricks-upload-lib.webp)

### 方式一：內建感應器指令（不用函式庫）

直接用 Pybricks 內建的 `ColorSensor` 指令讀，不用載入額外檔案。下面的值是用內建指令算出來的：

| 組合 | 函數 | 範圍 | 說明 |
| :--- | :--- | :--- | :--- |
| 基礎（比較不挑校準品質，但還是要先校準一次） | `getLinePos16()` | `-16 ~ +16` | 循線位置（整數），0 正中央，負偏左、正偏右 |
| | `getLineWidth16()` | `0 ~ 16` | 特徵線寬（觸發的感測器數量），偵測十字、T 字路口 |
| 高精度（建議先確實校準） | `getLinePos100()` | `-100 ~ +100` | 高解析插值位置，適合高階 PID |
| | `getLineWidth100()` | `0 ~ 16` | 高精度模式的線寬，跟 `getLinePos100()` 同步 |

> [!TIP]
> 位置和線寬要用**同一組**：`getLinePos16()` 配 `getLineWidth16()`，`getLinePos100()` 配 `getLineWidth100()`，讀到的才是同一刻的資料。

範例：積木版 [4.0.0](/examples/line16/pybricks/For%20firmware%204.0.0/line16_block_native_v400.py)・[3.6.1](/examples/line16/pybricks/For%20firmware%203.6.1/line16_block_native_v361.py)；Python 版 [4.0.0](/examples/line16/pybricks/For%20firmware%204.0.0/line16_python_native_v400.py)・[3.6.1](/examples/line16/pybricks/For%20firmware%203.6.1/line16_python_native_v361.py)

### 方式二：匯入專屬函式庫

| 函數 | 範圍 | 說明 |
| :--- | :--- | :--- |
| `line_init_port_multitask(port, multitask)` | — | 初始化。`port` = 1~6 對應 A~F；`multitask=True` 啟用非同步模式（搭配 `await`） |
| `line_pos16()` | `-16 ~ +16` | 整數循線位置，比較不挑校準品質（沒校準時每一路都會被當成黑色） |
| `line_pos100()` | `-100 ~ +100` | 高解析插值位置，最佳 PID 選擇，要確實校準 |
| `line_width()` | `0 ~ 16` | 特徵線寬，兩種位置函數都適用 |
| `line_ir_calibration()` | `list[16]` | 16 路校正後光值陣列（每值 `0 ~ 15`）。從車子後面往前看：index 0 = 最左（ch1） |
| `line_ir_ch(ch)` | `0 ~ 15` | 單一通道校正光值，`ch` = 1~16，可連續呼叫（共用快取） |
| `line_junction()` | `0, 1, 2+` | 線群數量：0 無線、1 單線、2 以上路口分叉 |
| `line_junction_name()` | `str` | `"none"`、`"line"`、`"junction"` |
| `line_bin_raw()` | `0 ~ 65535` | 16 位元觸發遮罩，bit0 = ch1，1 = 偵測到線 |
| `line_on_sensors()` | `list[int]` | 偵測到線的通道編號，例如 `[7, 8, 9]` |
| `line_read_all()` | `dict` | 一次讀全部：`ir_calibration`、`line_pos16`、`line_pos100`、`line_width`、`bin_raw`、`on_sensors`、`junction`、`junction_name` |

範例：積木版 [4.0.0](/examples/line16/pybricks/For%20firmware%204.0.0/line16_block_with_lib_v400.py)・[3.6.1](/examples/line16/pybricks/For%20firmware%203.6.1/line16_block_with_lib_v361.py)；Python 版 [4.0.0](/examples/line16/pybricks/For%20firmware%204.0.0/line16_python_with_lib_v400.py)・[3.6.1](/examples/line16/pybricks/For%20firmware%203.6.1/line16_python_with_lib_v361.py)

### 想學更厲害的循線方法？

可以找這幾位有豐富比賽與教學經驗的教練：[LegoLauXiao 教練](https://www.youtube.com/@LegoLauXiao)（[示範影片](https://www.youtube.com/watch?v=WgacdWLatbk)）、[legolaumo 教練](https://www.youtube.com/@legolaumo)（[示範影片](https://www.youtube.com/watch?v=T9bcndBNQvQ)）。
