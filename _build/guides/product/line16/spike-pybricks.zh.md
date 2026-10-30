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

主機要是 **Pybricks 韌體 v4.1**。zip 裡有 `MBC_sensors.py`、`MBC_line16_obj_Lib.py` 和 4 支範例：

- [📥 打包下載（Pybricks 4.1）](/examples/line16/line16_pybricks_spike_v4.x.zip)

單獨下載函式庫：[MBC_sensors.py](/examples/line16/pybricks/For%20firmware%204.x/MBC_sensors.py)（新專案用這個，一個檔適用全部 MBC 感應器）・[MBC_line16_obj_Lib.py](/examples/line16/pybricks/For%20firmware%204.x/MBC_line16_obj_Lib.py)（本頁範例與舊程式用）

> [!IMPORTANT]
> 範例程式寫的是 `from MBC_line16_obj_Lib import MBC_LINE16`，所以要匯入 `MBC_line16_obj_Lib.py`（檔名不用改）。自己寫新程式改成 `from MBC_sensors import MBC_LINE16`，只匯入 `MBC_sensors.py`。函式庫要跟主程式放在**同一個專案列表**裡。

> [!NOTE]
> Pybricks 3.6.1 版已不再更新，舊檔放在[下載專區](/downloads.md)的「舊版（不再更新）」。

![把函式庫匯入 Pybricks 專案](/images/sensors/line16/pybricks-upload-lib.webp)

### 方式一：內建感應器指令（不用函式庫）

直接用 Pybricks 內建的 `ColorSensor` 指令讀，不用載入額外檔案。下面的值是用內建指令算出來的：

| 組合 | 函數 | 範圍 | 說明 |
| :--- | :--- | :--- | :--- |
| 基礎（比較不挑校準品質，但還是要先校準一次） | `getLinePos16()` | `-16 ~ +16` | 循線位置（整數），0 正中央，負偏左、正偏右 |
| | `getLineWidth16()` | `0 ~ 16` | 特徵線寬（觸發的感測器數量），偵測十字、T 字路口 |
| 高精度（建議先確實校準） | `getLinePos100()` | `-100 ~ +100` | 高解析插值位置，適合高階 PID |
| | `getLineWidth100()` | `0 ~ 9` | 高精度模式的線寬，跟 `getLinePos100()` 同步 |

> [!TIP]
> 位置和線寬要用**同一組**：`getLinePos16()` 配 `getLineWidth16()`，`getLinePos100()` 配 `getLineWidth100()`，讀到的才是同一刻的資料。

範例：積木版 [line16_block_native.py](/examples/line16/pybricks/For%20firmware%204.x/line16_block_native.py)；Python 版 [line16_python_native.py](/examples/line16/pybricks/For%20firmware%204.x/line16_python_native.py)

> [!IMPORTANT]
> **Pybricks 4.1 的拆法**（上面的範例已經照這樣寫）
> - `reflection()`（模式 5）＝粗位置＋線寬。Pybricks 4.1 起會帶 1 位小數，**要先取整數再拆**：`r = int(line.reflection())`；16 路：讀到負數先 +6399，位置＝`r // 100 - 16`（−16～+16）、線寬＝`r % 100`。積木版沒有「取整數」積木：≥ 0 用「無條件捨去」、負數用「無條件進位」，結果跟 `int()` 一樣。
> - `ambient()`（模式 7）＝「細位置＋100 . 線寬」，線寬在小數第 1 位：細位置＝整數部分 − 100，線寬＝`round(line.ambient() * 10) % 10`（16 路最多顯示 9＝9 以上）。**要 2026-10-30 以後的感應器韌體**；舊韌體線寬會是 0（細位置照常）。
> - 位置都是「加偏移量送出、讀了再減」，不會直接送負數。
> - 進階（只有 Python）：`line.hsv(surface=False).h`＝(細位置＋100)×100＋線寬，可以拿到完整的線寬。

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

範例：積木版 [line16_block_with_lib.py](/examples/line16/pybricks/For%20firmware%204.x/line16_block_with_lib.py)；Python 版 [line16_python_with_lib.py](/examples/line16/pybricks/For%20firmware%204.x/line16_python_with_lib.py)

### 下命令範例（校準、左右反向）

用 MBC 統一函式庫下命令（寫法和每個命令的說明見本頁下方「命令」段）。先把 [MBC_sensors.py](/examples/line16/pybricks/For%20firmware%204.x/MBC_sensors.py) 上傳到 Pybricks Code，跟範例放在同一個專案列表；感應器接 **C 埠**；照輸出區的提示按主機**左鍵**進下一步（中間鍵照常是停止程式）。要 2026-10 以後的感應器韌體。3 支都在主機上跑過。

| 範例 | 做什麼 | 怎麼確認成功 |
| :--- | :--- | :--- |
| [line16_cmd_calib_bw.py](/examples/line16/pybricks/For%20firmware%204.x/line16_cmd_calib_bw.py) | 黑白校準（E2 黑、E3 白）：整排放在黑色上按左鍵，再整排放在白紙上按左鍵（高度固定） | 黑色光值全 0；白紙光值（`ir_calibration()`，0～15）13 以上 |
| [line16_cmd_calib_auto.py](/examples/line16/pybricks/For%20firmware%204.x/line16_cmd_calib_auto.py) | E7 5 秒自動校準：5 秒內把感應器在黑線和白底之間來回掃（高度固定、不要傾斜） | 狀態從「執行中」變「完成」；白紙光值 11 以上（校準品質看手掃的高度） |
| [line16_cmd_reverse.py](/examples/line16/pybricks/For%20firmware%204.x/line16_cmd_reverse.py) | E9 左右反向，也示範送錯參數會回「失敗」 | 線位置正負號對調（例如 −15 → +15，再送 0 換回來）；送 5 回「失敗」 |

### PD 循線示範（速度 50）

同一套 PD 循線邏輯，4 種讀法各有積木版和 Python 版，共 8 支；看到十字路口（整排踩到線）會停車。8 支都在 Pybricks 主機上跑過。

- [📥 打包下載（Pybricks 8 支＋函式庫＋官方 App 版）](/examples/line16/line16_pd50_examples.zip)

| 讀法 | 讀什麼 | 建議 KP／KD | 十字路口停車 | 積木版 | Python 版 |
| :--- | :--- | :--- | :--- | :--- | :--- |
| 內建反射光（模式 5） | `ColorSensor.reflection()`：粗位置＋線寬 | 2.5／10 | 線寬 > 10 | [mode5_block](/examples/line16/pd50/pybricks/line16_pd50_mode5_block.py) | [mode5_python](/examples/line16/pd50/pybricks/line16_pd50_mode5_python.py) |
| 內建環境光（模式 7） | `ColorSensor.ambient()`：細位置＋線寬（要 2026-10-30 以後的韌體） | 0.4／1.5 | 線寬 > 8 | [mode7_block](/examples/line16/pd50/pybricks/line16_pd50_mode7_block.py) | [mode7_python](/examples/line16/pd50/pybricks/line16_pd50_mode7_python.py) |
| 專屬函式庫 | `MBC_line16_obj_Lib`：`pos100()`＋`width()`（模式 9） | 0.4／1.5 | 線寬 > 10 | [lib_block](/examples/line16/pd50/pybricks/line16_pd50_lib_block.py) | [lib_python](/examples/line16/pd50/pybricks/line16_pd50_lib_python.py) |
| 統一函式庫 | `MBC_sensors`：同上 | 0.4／1.5 | 線寬 > 10 | [sensors_block](/examples/line16/pd50/pybricks/line16_pd50_sensors_block.py) | [sensors_python](/examples/line16/pd50/pybricks/line16_pd50_sensors_python.py) |

- **接線**：感應器 C 埠、左馬達 E 埠（程式設成反轉 COUNTERCLOCKWISE）、右馬達 F 埠。
- 速度 50（功率 %）。KP、KD 是建議值，要在自己的車上調：車子左右甩就調小 KP，轉彎太慢就調小 KD。
- 用函式庫的兩種，要先把 `MBC_line16_obj_Lib.py` 或 `MBC_sensors.py` 上傳到 Pybricks Code，跟主程式放在同一個專案列表（zip 裡有附）。
- 積木版打開就是 Pybricks 積木畫面；第一行是積木資料，請整個檔案匯入，不要只複製下面的 Python。

### 想學更厲害的循線方法？

可以找這幾位有豐富比賽與教學經驗的教練：[LegoLauXiao 教練](https://www.youtube.com/@LegoLauXiao)（[示範影片](https://www.youtube.com/watch?v=WgacdWLatbk)）、[legolaumo 教練](https://www.youtube.com/@legolaumo)（[示範影片](https://www.youtube.com/watch?v=T9bcndBNQvQ)）。
