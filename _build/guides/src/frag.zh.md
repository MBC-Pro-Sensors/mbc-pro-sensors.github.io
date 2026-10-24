@@@ cal
### 先校準（只要做一次）

循行者要先「認識」你場地上的黑色和白色，數值才會準。校準完會記在感應器裡，重新開機也還在（除非換場地）。

1. 感應器插好、主機開機（感應器的燈亮起來）。
2. 按住感應器上的**校準按鈕約 0.5 秒**再放開（不要一直按著，按著不放會一直重新校準）。
3. **綠色校準指示燈**（靠近校準按鈕）亮起後，**5 秒內**把感應器在黑線和白色地面之間來回掃幾次，%%N%% 個感應點都要掃過**最黑的黑線**和**最白的白色區域**。
4. 掃的時候感應器離地 **0.5～1.2 公分**，太高會讀不準。
5. 綠燈熄滅＝校準完成，數值自動存進感應器。

**怎麼知道校準好了？** 放在白色地面時「線寬」是 0，壓在一般黑線上是 %%CHK%%。

<iframe src="https://www.youtube.com/embed/TKi7b20x8UY" title="循行者校準教學" frameborder="0" allowfullscreen loading="lazy" style="width:100%;aspect-ratio:16/9;border:none;border-radius:12px;"></iframe>

@@@ cal-arduino
> [!TIP]
> 也可以用程式校準：送命令 `E7`（5 秒動態校準，期間 F3 bit2 = 1），或 `E2`／`E3` 把目前讀值記為黑／白（約 50 ms，存 EEPROM）。本頁的範例 4 就是校準範例。

@@@ cal-mblock
> [!TIP]
> 也可以用積木校準：「循行者 連接埠 ［埠］ 5 秒動態校準」做的事跟按按鈕一樣。

@@@ cal-microbit
> [!TIP]
> 也可以用積木校準：「循行者 校準 …」選「記為黑」（E2）、「記為白」（E3）或「5 秒動態掃描」（E7），結果存進感應器。

@@@ cal-matrixbig
> [!TIP]
> 換成大通道函式庫後也能用積木校準：**Cyan = 黑色校準**、**Magenta = 白色校準**（循行者 8 路的 byte 25／26 會記下完成次數）。

@@@ mblock-legacy
### 舊韌體與舊積木

舊版擴展 v1.0.0（15 個積木）搭配 2026-10 以前出貨的韌體。新韌體要用舊積木時，循行者 8／16 路可以燒 `MBC_MBLOCK_LEGACY=1` 版本的韌體（其他產品沒有舊版）。

@@@ line8-spike-official
### 數據通道對照表

官方 App 裡循行者 8 路被當成 **SPIKE 顏色感應器**，8 路的資料封裝在顏色感應器的各個通道裡。**教育版和家用版的對照不同**，請看你那一版。

**教育版（LEGO Education SPIKE App，SPIKE 3）**

| 官方積木 | 資料 | 範圍與解碼 |
| :--- | :--- | :--- |
| 顏色（Color） | 特徵線寬 | `0 ~ 8`：踩在線上的感應器數量。正常黑線一般是 `2 ~ 3`；十字路口或大色塊會大幅增加，可用來判斷路口 |
| 反射光（Reflected Light） | 標準位置 | `0 ~ 16`：`8` 正中央、`0` 最左、`16` 最右。讀到後 **−8** 就是循線位置（負＝線在左、正＝線在右），適合 PID 循線 |
| 紅色數值（Red） | 高解析平滑位置 | `0 ~ 200`：`100` 正中央。讀到後 **−100** 就是高精度循線位置（負＝線在左）；這個值要確實校準 |
| 綠色數值（Green） | 前 4 顆光電值 | `0 ~ 65535`：每 4 bit 一顆，每顆 `0 ~ 15` |
| 藍色數值（Blue） | 後 4 顆光電值 | `0 ~ 65535`：每 4 bit 一顆，每顆 `0 ~ 15` |

**家用版（LEGO MINDSTORMS Robot Inventor App）**

家用版 App 有些單一通道積木被官方限制在 `0 ~ 126`，所以位置與反射光資料改放在 RGB 與反射光通道，繞過 126 的限制：

| 官方積木 | 資料 | 範圍與解碼 |
| :--- | :--- | :--- |
| 顏色（Color） | 特徵線寬 | `0 ~ 8`（家用版顏色最大只到 10），常用來判斷十字路口 |
| 反射光（Reflected Light） | 標準位置 | `0 ~ 16`：讀到後 **−8** 就是循線位置，不受 126 限制 |
| 紅色通道（Red） | 高解析平滑位置 | `0 ~ 200`：讀到後 **−100** 就是高精度循線位置，家用版完全沒有縮水 |
| 綠色通道（Green） | 前 4 顆光電值 | `0 ~ 255`：每 2 bit 一顆，每顆 `0 ~ 3` |
| 藍色通道（Blue） | 後 4 顆光電值 | `0 ~ 255`：每 2 bit 一顆，每顆 `0 ~ 3` |

### 範例程式

![循行者 8 路 SPIKE App 的 PID 循線積木](/images/sensors/line8/spike-education-example.webp)

- [📥 教育版範例（.llsp3）](/examples/line8/spike/line8-demo.llsp3)：用 LEGO Education SPIKE App 開啟。
- [📥 家用版範例（.lms）](/examples/line8/spike/line8-ri-demo.lms)：用 LEGO MINDSTORMS Robot Inventor App 開啟。

![循行者 8 路 Robot Inventor App 的 PID 循線積木](/images/sensors/line8/spike-ri-example.webp)

@@@ line16-spike-official
### 數據通道對照表

官方 App 裡循行者 16 路被當成 **SPIKE 顏色感應器**，16 路的資料封裝在顏色感應器的各個通道裡。**教育版和家用版的對照不同**，請看你那一版。

**教育版（LEGO Education SPIKE App，SPIKE 3）**

| 官方積木 | 資料 | 範圍與解碼 |
| :--- | :--- | :--- |
| 顏色（Color） | 特徵線寬 | `0 ~ 16`：踩在線上的感應器數量。正常黑線一般是 `3 ~ 5`；十字路口或大色塊會大幅增加，可用來判斷路口 |
| 反射光（Reflected Light） | 標準位置 | `0 ~ 32`：`16` 正中央、`0` 最左、`32` 最右。讀到後 **−16** 就是循線位置（負＝線在左、正＝線在右），適合 PID 循線 |
| 紅色數值（Red） | 高解析平滑位置 | `0 ~ 200`：`100` 正中央。讀到後 **−100** 就是高精度循線位置（負＝線在左）；這個值要確實校準 |
| 綠色數值（Green） | 前 8 顆光電值 | `0 ~ 65535`：每 2 bit 一顆，每顆 `0 ~ 3` |
| 藍色數值（Blue） | 後 8 顆光電值 | `0 ~ 65535`：每 2 bit 一顆，每顆 `0 ~ 3` |

**家用版（LEGO MINDSTORMS Robot Inventor App）**

家用版 App 有些單一通道積木被官方限制在 `0 ~ 126`，所以位置與反射光資料改放在 RGB 與反射光通道，繞過 126 的限制：

| 官方積木 | 資料 | 範圍與解碼 |
| :--- | :--- | :--- |
| 顏色（Color） | 特徵線寬 | `0 ~ 10`：家用版顏色最大只到 10，線寬被限制在 `0 ~ 10`，常用來判斷十字路口 |
| 反射光（Reflected Light） | 標準位置 | `0 ~ 32`：讀到後 **−16** 就是循線位置，不受 126 限制 |
| 紅色通道（Red） | 高解析平滑位置 | `0 ~ 200`：讀到後 **−100** 就是高精度循線位置，家用版完全沒有縮水 |
| 綠色通道（Green） | 前 8 顆光電值 | `0 ~ 255`：每 1 bit 一顆，每顆 `0 ~ 1` |
| 藍色通道（Blue） | 後 8 顆光電值 | `0 ~ 255`：每 1 bit 一顆，每顆 `0 ~ 1` |

### 範例程式

![循行者 16 路 SPIKE App 的 PID 循線積木](/images/sensors/line16/spike-education-example.webp)

- [📥 教育版範例（.llsp3）](/examples/line16/spike/line16-demo.llsp3)：用 LEGO Education SPIKE App 開啟。
- [📥 家用版範例（.lms）](/examples/line16/spike/line16-demo.lms)：用 LEGO MINDSTORMS Robot Inventor App 開啟。

![循行者 16 路 Robot Inventor App 的 PID 循線積木](/images/sensors/line16/spike-ri-example.webp)

@@@ coach
### 想學更厲害的循線方法？

可以找這幾位有豐富比賽與教學經驗的教練：[LegoLauXiao 教練](https://www.youtube.com/@LegoLauXiao)（[示範影片](https://www.youtube.com/watch?v=WgacdWLatbk)）、[legolaumo 教練](https://www.youtube.com/@legolaumo)（[示範影片](https://www.youtube.com/watch?v=T9bcndBNQvQ)）。

@@@ line-spike-pybricks
### 函式庫與範例下載

依主機的 Pybricks 韌體版本選一包（zip 裡有函式庫＋4 支範例）：

- [📥 打包下載（韌體 4.0.0 版）](/examples/%%P%%/%%P%%_pybricks_spike_v4.0.0.zip)
- [📥 打包下載（韌體 3.6.1 版）](/examples/%%P%%/%%P%%_pybricks_spike_v3.6.1.zip)

單獨下載函式庫：[4.0.0 版 MBC_%%P%%_obj_Lib_v400.py](/examples/%%P%%/pybricks/For%20firmware%204.0.0/MBC_%%P%%_obj_Lib_v400.py)・[3.6.1 版 MBC_%%P%%_Lib_v361.py](/examples/%%P%%/pybricks/For%20firmware%203.6.1/MBC_%%P%%_Lib_v361.py)

> [!IMPORTANT]
> **檔名一定要改**：範例程式寫的是 `import MBC_%%P%%_obj_Lib`（3.6.1 版是 `MBC_%%P%%_Lib`），下載後要把檔名改成 `MBC_%%P%%_obj_Lib.py`（去掉 `_v400`）或 `MBC_%%P%%_Lib.py`（去掉 `_v361`），否則會出現 `ImportError`。函式庫要跟主程式放在**同一個專案列表**裡。

![把函式庫匯入 Pybricks 專案](/images/sensors/%%P%%/pybricks-upload-lib.webp)

### 方式一：內建感應器指令（不用函式庫）

直接用 Pybricks 內建的 `ColorSensor` 指令讀，不用載入額外檔案。下面的值是用內建指令算出來的：

| 組合 | 函數 | 範圍 | 說明 |
| :--- | :--- | :--- | :--- |
| 基礎（比較不挑校準品質，但還是要先校準一次） | `getLinePos%%N%%()` | `-%%N%% ~ +%%N%%` | 循線位置（整數），0 正中央，負偏左、正偏右 |
| | `getLineWidth%%N%%()` | `0 ~ %%N%%` | 特徵線寬（觸發的感測器數量），偵測十字、T 字路口 |
| 高精度（建議先確實校準） | `getLinePos100()` | `-100 ~ +100` | 高解析插值位置，適合高階 PID |
| | `getLineWidth100()` | `0 ~ %%N%%` | 高精度模式的線寬，跟 `getLinePos100()` 同步 |

> [!TIP]
> 位置和線寬要用**同一組**：`getLinePos%%N%%()` 配 `getLineWidth%%N%%()`，`getLinePos100()` 配 `getLineWidth100()`，讀到的才是同一刻的資料。

範例：積木版 [4.0.0](/examples/%%P%%/pybricks/For%20firmware%204.0.0/%%P%%_block_native_v400.py)・[3.6.1](/examples/%%P%%/pybricks/For%20firmware%203.6.1/%%P%%_block_native_v361.py)；Python 版 [4.0.0](/examples/%%P%%/pybricks/For%20firmware%204.0.0/%%P%%_python_native_v400.py)・[3.6.1](/examples/%%P%%/pybricks/For%20firmware%203.6.1/%%P%%_python_native_v361.py)

### 方式二：匯入專屬函式庫

| 函數 | 範圍 | 說明 |
| :--- | :--- | :--- |
| `line_init_port_multitask(port, multitask)` | — | 初始化。`port` = 1~6 對應 A~F；`multitask=True` 啟用非同步模式（搭配 `await`） |
| `line_pos%%N%%()` | `-%%N%% ~ +%%N%%` | 整數循線位置，比較不挑校準品質（沒校準時每一路都會被當成黑色） |
| `line_pos100()` | `-100 ~ +100` | 高解析插值位置，最佳 PID 選擇，要確實校準 |
| `line_width()` | `0 ~ %%N%%` | 特徵線寬，兩種位置函數都適用 |
| `line_ir_calibration()` | `list[%%N%%]` | %%N%% 路校正後光值陣列（每值 `0 ~ %%IRMAX%%`）。從車子後面往前看：index 0 = 最左（ch1） |
| `line_ir_ch(ch)` | `0 ~ %%IRMAX%%` | 單一通道校正光值，`ch` = 1~%%N%%，可連續呼叫（共用快取） |
| `line_junction()` | `0, 1, 2+` | 線群數量：0 無線、1 單線、2 以上路口分叉 |
| `line_junction_name()` | `str` | `"none"`、`"line"`、`"junction"` |
| `line_bin_raw()` | `0 ~ %%BINMAX%%` | %%N%% 位元觸發遮罩，bit0 = ch1，1 = 偵測到線 |
| `line_on_sensors()` | `list[int]` | 偵測到線的通道編號，例如 `%%EXON%%` |
| `line_read_all()` | `dict` | 一次讀全部：`ir_calibration`、`line_pos%%N%%`、`line_pos100`、`line_width`、`bin_raw`、`on_sensors`、`junction`、`junction_name` |

範例：積木版 [4.0.0](/examples/%%P%%/pybricks/For%20firmware%204.0.0/%%P%%_block_with_lib_v400.py)・[3.6.1](/examples/%%P%%/pybricks/For%20firmware%203.6.1/%%P%%_block_with_lib_v361.py)；Python 版 [4.0.0](/examples/%%P%%/pybricks/For%20firmware%204.0.0/%%P%%_python_with_lib_v400.py)・[3.6.1](/examples/%%P%%/pybricks/For%20firmware%203.6.1/%%P%%_python_with_lib_v361.py)

@@@ ev3-prox
### Proximity 模式：線位置

| 讀到的值 | 範圍 | 說明 |
| :--- | :--- | :--- |
| 線位置 | `-%%N%% ~ +%%N%%` | 0 = 黑線在正中央，負數偏左、正數偏右；±%%N%% 表示線在感應器最邊緣 |

@@@ ev3-beacon-line8
### Beacon 模式：8 路光電反射值

讀「數值列表」（8 bytes），每一路都是校準後的反射值：0＝最暗（黑線）、100＝最亮（白底）。第 1 路在最左邊（站在車子後面、面向車頭看）。

| Byte | 內容 | 範圍 | EV3-G／Classroom 怎麼讀 | clev3r |
| :---: | :--- | :--- | :--- | :--- |
| 0 | 第 1 路 | 0 ~ 100 | 信標頻道 1 的「方向」 | `raw[0]` |
| 1 | 第 2 路 | 0 ~ 100 | 信標頻道 1 的「近接（距離）」 | `raw[1]` |
| 2 | 第 3 路 | 0 ~ 100 | 信標頻道 2 的「方向」 | `raw[2]` |
| 3 | 第 4 路 | 0 ~ 100 | 信標頻道 2 的「近接（距離）」 | `raw[3]` |
| 4 | 第 5 路 | 0 ~ 100 | 信標頻道 3 的「方向」 | `raw[4]` |
| 5 | 第 6 路 | 0 ~ 100 | 信標頻道 3 的「近接（距離）」 | `raw[5]` |
| 6 | 第 7 路 | 0 ~ 100 | 信標頻道 4 的「方向」 | `raw[6]` |
| 7 | 第 8 路 | 0 ~ 100 | 信標頻道 4 的「近接（距離）」 | `raw[7]` |

@@@ ev3-beacon-line16
### Beacon 模式：16 路光電反射值

讀「數值列表」（8 bytes）。每路光值量化成 1～9（1＝最暗、9＝最亮），兩路合併成一個兩位數（11～99），避開 EV3 的 100 數值限制。

| Byte | 內容 | 範圍 | 解碼 |
| :---: | :--- | :--- | :--- |
| 0 | 通道 0＋1 | 11 ~ 99 | 十位數＝通道 0、個位數＝通道 1 |
| 1 | 通道 2＋3 | 11 ~ 99 | 十位數＝通道 2、個位數＝通道 3 |
| 2 | 通道 4＋5 | 11 ~ 99 | 十位數＝通道 4、個位數＝通道 5 |
| 3 | 通道 6＋7 | 11 ~ 99 | 十位數＝通道 6、個位數＝通道 7 |
| 4 | 通道 8＋9 | 11 ~ 99 | 十位數＝通道 8、個位數＝通道 9 |
| 5 | 通道 10＋11 | 11 ~ 99 | 十位數＝通道 10、個位數＝通道 11 |
| 6 | 通道 12＋13 | 11 ~ 99 | 十位數＝通道 12、個位數＝通道 13 |
| 7 | 通道 14＋15 | 11 ~ 99 | 十位數＝通道 14、個位數＝通道 15 |

例：讀到 `83`，十位數 8 是偶數通道（例如通道 0）的光值，個位數 3 是奇數通道（例如通道 1）的光值。

@@@ ev3-remote
### Remote 模式：位置＋線寬＋左右邊界（比賽推薦）

讀「數值列表」（4 bytes）：

| Byte | 內容 | 範圍 | 說明 |
| :---: | :--- | :--- | :--- |
| 0 | 高解析位置 | `-100 ~ +100` | 0 正中央，負偏左、正偏右，PID 誤差輸入首選 |
| 1 | 特徵線寬 | `0 ~ %%N%%` | 觸發的感測器數量，偵測十字或 T 字路口 |
| 2 | 左邊界光值 | `0 ~ 100` | 第 1 路（最左）的反射光，0 純黑、100 純白 |
| 3 | 右邊界光值 | `0 ~ 100` | 第 %%N%% 路（最右）的反射光，0 純黑、100 純白 |

> [!TIP]
> Byte 2 和 Byte 3 同時小於 20，表示車頭整個進入十字路口的橫線；其中一邊突然變化，表示快到分岔，可以用來防掉線。

@@@ ev3-myblocks-ev3g
### MBC 自訂積木（My Blocks）

範例專案裡有三個自訂積木，不用另外裝擴充包，呼叫後會在背景解析資料、存進專屬變數。**放進無限迴圈（Loop）裡**就能隨時拿到最新資料：

1. `LinePos%%N%%_dataUpdate`：基礎循線，更新變數 `0_LinePos%%N%%`（`-%%N%% ~ +%%N%%`，左負右正），適合新手。
2. `LinePos100_dataUpdate`：高精度循線（要確實校準），更新 `0_LinePos100`（`-100 ~ +100`）、`0_LineWidth%%N%%`（`0 ~ %%N%%`）與最左／最右邊界光值。
3. `LineCh%%N%%_dataUpdate`：指定通道解碼，輸入通道編號 `1 ~ %%N%%`，輸出該通道的量化光值 `1 ~ 9`（1 最暗、9 最亮）。

@@@ ev3-myblocks-classroom
### MBC 自訂積木（My Blocks）

範例專案裡有三個自訂積木，不用另外裝擴充包，呼叫後會在背景解析資料、存進專屬變數。**放進「重複無限次」迴圈裡**就能隨時拿到最新資料：

1. `LinePos%%N%%_dataUpdate`：基礎循線，更新變數 `0_LinePos%%N%%`（`-%%N%% ~ +%%N%%`，左負右正），適合新手。
2. `LinePos100_dataUpdate`：高精度循線（要確實校準），更新 `0_LinePos100`（`-100 ~ +100`）、`0_LineWidth%%N%%`（`0 ~ %%N%%`）與最左／最右邊界光值。
3. `LineCh%%N%%_dataUpdate`：全陣列展開，更新 `s1` 到 `s%%N%%` 共 %%N%% 個變數，由左至右對應每一顆的讀數。

@@@ ev3-myblocks-clev3r
### MBC 專屬副程式（Subroutines）

範例檔裡有三個副程式，不用另外裝擴充包。**放進無限迴圈裡**就能隨時拿到最新資料：

1. `LinePos%%N%%_dataUpdate`：輸入連接埠 `port`，回傳 `pos%%N%%`（`-%%N%% ~ +%%N%%`，左負右正）。
2. `LinePos100_dataUpdate`：輸入 `port`，回傳 `pos100`（`-100 ~ +100`，要確實校準）、`width`（`0 ~ %%N%%`）、`ch1`／`ch%%N%%`（最左／最右光值 `0 ~ 100`）。
3. `LineCh%%N%%_dataUpdate`：輸入 `port`，回傳陣列 `light%%N%%`，用 `light%%N%%[0]` ~ `light%%N%%[%%NM1%%]` 取每一顆的量化光值 `1 ~ 9`（1 最暗、9 最亮）。

@@@ ev3-ex-ev3g
### 範例程式

- [📥 EV3-G 範例（.ev3）](/examples/%%P%%/ev3/%%P%%_ev3_official.ev3)：用 EV3 官方軟體開啟。

![%%NAME%%在 EV3-G 的 PID 循線程式](/images/sensors/%%P%%/ev3-official-example.webp)

@@@ ev3-ex-classroom
### 範例程式

- [📥 Classroom 範例（.lmsp）](/examples/%%P%%/ev3/%%P%%_ev3_classroom.lmsp)：用 EV3 Classroom 開啟。

![%%NAME%%在 EV3 Classroom 的 PID 循線程式](/images/sensors/%%P%%/ev3-classroom-example.webp)

@@@ ev3-ex-clev3r
### 範例程式

- [📥 clev3r 範例（.bp）](/examples/%%P%%/ev3/%%P%%_ev3_clev3r.bp)：用 clev3r 開啟。

![%%NAME%%在 clev3r 的 PID 循線程式](/images/sensors/%%P%%/ev3-clev3r-example.webp)

@@@ line-ev3-pybricks
### 函式庫與範例下載

- [📥 打包下載（範例＋函式庫）](/examples/%%P%%/%%P%%_pybricks_ev3.zip)
- 函式庫：[MBC_%%P%%_EV3_Lib.py](/examples/%%P%%/ev3/MBC_%%P%%_EV3_Lib.py)。要直接匯入你的 Pybricks 專案，**檔名必須完全一致**，`import` 才載得進來。

![把函式庫匯入 EV3 Pybricks 專案](/images/sensors/%%P%%/ev3-pybricks-upload-lib.webp)

### 方式一：原生指令讀取

用 Pybricks 底層的 `device.mode(mode)` 直接拿陣列資料：

| 指令 | 回傳 | 內容 |
| :--- | :--- | :--- |
| `device.mode(0)` | Tuple（1 byte） | 新手模式：位置 `-%%N%% ~ %%N%%`，0 為中央 |
| `device.mode(1)` | Tuple（8 bytes） | 全覽除錯模式：%%MODE1%% |
| `device.mode(2)` | Tuple（4 bytes） | 競技模式：高精度位置 `-100 ~ 100`、線寬 `0 ~ %%N%%`、左邊界光值、右邊界光值 |

> [!NOTE]
> Pybricks 官方還沒把對應的圖形與底層指令更新好，原生範例程式之後補齊；目前建議直接用方式二。

### 方式二：匯入專屬函式庫

`line_init_port_multitask(port, multitask)` 初始化：`port` 傳 EV3 埠物件（例如 `hub.port.S1`）。**非多工**：`multitask` 填 `False`，呼叫時前面選 `call`；**多工**：填 `True`，呼叫時前面選 `await`。

![非多工設定](/images/sensors/%%P%%/ev3-pybricks-non-multitask.webp)

![多工設定（await）](/images/sensors/%%P%%/ev3-pybricks-multitask.webp)

| 模式 | 函數 | 範圍 | 說明 |
| :--- | :--- | :--- | :--- |
| 模式 0（基礎） | `line_pos%%N%%()` | `-%%N%% ~ +%%N%%` | 整數循線位置，0 正中央 |
| 模式 1（解碼） | `line_ir_ch(ch)` | `1 ~ 9` | 單一通道量化光值，`ch` = 1~%%N%%，1 最暗、9 最亮 |
| 模式 2（競技） | `line_pos100()` | `-100 ~ +100` | 高解析插值位置，PID／PD 專用 |
| 模式 2（競技） | `line_width()` | `0 ~ %%N%%` | 特徵線寬，偵測路口與寬線 |
| 模式 2（競技） | `line_edge_l()` | `0 ~ 100` | 左邊界光值（0 黑、100 白），防掉線 |
| 模式 2（競技） | `line_edge_r()` | `0 ~ 100` | 右邊界光值（0 黑、100 白），防掉線 |

> [!IMPORTANT]
> 不同模式的函數交叉呼叫會有切換延遲。一般使用者與比賽選手**全程用模式 2** 的函數；只有要讀所有感測器原始值、自己寫特殊演算法時，才單獨用模式 1。

範例：[積木版（.py）](/examples/%%P%%/ev3/%%P%%_ev3_block_with_lib.py)・[Python 版（.py）](/examples/%%P%%/ev3/%%P%%_ev3_python_with_lib.py)

@@@ pair-ps4
### 配對手把（第一次用或要換手把）

1. 接收器接好、主機開機。**插上去的時候不要按住接收器上的按鍵**（會進入燒錄模式，接收器就不會動作；放開按鍵、拔掉重插就好）。
2. **第一次使用**：接收器還沒記住任何手把時，一開機就會自動開放 **30 秒**配對（綠燈快速閃）。**要換手把、或 30 秒已經過了**：按一下接收器上的按鍵（Reset），會清掉舊的手把、重新開放 30 秒。
3. 在這 30 秒內讓手把進入配對模式，直到手把的燈條快速閃爍：**PS4 手把**同時長按 **SHARE＋PS**；**PS5 手把**同時長按 **建立（Create）＋PS**。
4. 連上之後接收器會**記住這支手把**，也不會再連別人的手把。之後開機，只要按手把的 **PS 鍵**就會自動連上。

| 接收器的綠燈 | 意思 |
| :--- | :--- |
| 快速閃 | 正在等手把配對 |
| 一直亮 | 沒有連上手把（按手把 PS 鍵重新連線） |
| 熄掉 | 已經連上手把 |

> [!TIP]
> 比賽現場請在自己的位置完成配對，避免 30 秒內有別隊手把也在配對模式。綁錯手把就再按一次 Reset 重新配對。

@@@ pair-ps2
### 讓手把連上接收器

PS2 的 2.4G 手把和它的接收頭是**原廠成對**的，不用按任何配對鍵。

1. 接收器接好、主機開機。
2. **打開手把電源**，手把會自己連上。
3. 看接收器板子上的**綠燈**：**亮著＝還沒接到手把**；**熄掉＝已經接到手把**。
4. 手把上的 **ANALOG（或 MODE）燈要亮**，搖桿才有數值。接收器一接上手把會自動切到這個模式；燈沒亮就按一下手把中間的 ANALOG 鍵。

> [!CAUTION]
> 2.4GHz 是公共頻段，跟 Wi-Fi、微波爐共用。同一場地有 **12 套以上**同時使用時可能互相干擾，這種場合建議改用藍牙版（PS4／PS5）。

@@@ ps4-spike-official
### 範例程式

- [📥 教育版範例（.llsp3，SPIKE App 3.4.3）](/examples/ps4/spike/ps4_demo_spike_prime_v3.4.3.llsp3)
- [📥 家用版範例（.lms，Robot Inventor App 10.5.0）](/examples/ps4/spike/ps4_demo_robot_inventor_v10.5.0.lms)

> [!NOTE]
> 本頁的新手範例用的是教育版（SPIKE 3）。家用版（Robot Inventor）的「顏色」積木讀法不一樣，新手範例不適用，請用上面的家用版範例。

@@@ ps2-spike-official
> [!NOTE]
> 本頁的新手範例用的是教育版（SPIKE 3）。家用版（Robot Inventor）的「顏色」積木讀法不一樣，新手範例不適用，請看 [指令說明（PDF）](/examples/ps2/PS2_Controller_Command_Guide_ZH.pdf)。

@@@ pad-spike-pybricks
### 範例下載（Pybricks 3.6.1）

- [📥 全部下載（.zip）](/examples/%%P%%/%%P%%_pybricks_spike_v3.6.1.zip)
- [完整按鍵版（mode 9）](/examples/%%P%%/pybricks/For%20firmware%203.6.1/%%P%%_python_mode9_v361.py)・[積木版](/examples/%%P%%/pybricks/For%20firmware%203.6.1/%%P%%_block_native_v361.py)・[Python 版](/examples/%%P%%/pybricks/For%20firmware%203.6.1/%%P%%_python_native_v361.py)

@@@ ps2-pdf
> [!TIP]
> 掌控者 PS2 的 EV3／SPIKE App／Pybricks 積木用法也整理在 [指令說明（PDF）](/examples/ps2/PS2_Controller_Command_Guide_ZH.pdf)。

@@@ ps4-matrixbig
> [!NOTE]
> PS4 接收器是 ESP32，I2C 從機要 ≥ 2 ms 才能回應；韌體會預先載好最新一包，大通道函式庫 v2 的 `begin()` 等 5 ms 就是為了它。**上機待測**。

@@@ power5v-microbit
> [!IMPORTANT]
> **%%NAME%% 一定要外部 5 V 供電**（%%WHY%%），不能吃 micro:bit 的 3V 腳。外部電源的 GND 要和 micro:bit 共地。

@@@ tof8-warm
### 開機要等 2～4 秒

測距者 8 路不用校準，插上去就能用；**開機後要等 2～4 秒**讓 8 顆雷射準備好，這段時間全部讀到 120（公分）。程式開頭先等幾秒再讀距離。

| 第幾路 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |
| :--- | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| 朝哪裡 | 正左邊 | 左前 60° | 左前 30° | 正前方 | 正前方 | 右前 30° | 右前 60° | 正右邊 |

上表是感應器正面朝前、元件面朝上的裝法。手放在機器人左邊，範例 1 應該顯示**負的**方向；顯示正的代表裝反了，範例裡的「左轉」「右轉」也要對調。

@@@ tof8-spike-official
> [!WARNING]
> 官方 App 的讀法是依照感應器韌體推算的，**還沒有用實機逐一確認**。家用版（Robot Inventor）讀到的格式不一樣，本頁的官方 App 範例不適用。如果官方 App 版範例 1 不管手放哪裡都顯示 -7 或怪怪的數字，請先改用 MBC 積木網站或 Pybricks，並跟我們回報。

@@@ imu-ev3
> [!NOTE]
> 陀螺儀 IMU 在 EV3 上會顯示成「**陀螺儀**」（不是紅外線感應器），直接用原廠陀螺儀的積木讀。

@@@ imu-ev3-pybricks
> [!NOTE]
> 陀螺儀 IMU 在 EV3 上會被當成原廠陀螺儀，範例程式用 `GyroSensor` 讀。

@@@ cam-setup
### 先在感應器上設定顏色

神攝手的設定都在**感應器背面的彩色螢幕與按鍵**上完成，不用電腦：

1. 感應器要接上**已經開機**的主機才會啟動（螢幕先顯示「LEGO Sync...」），出現鏡頭畫面就可以使用。
2. **短按側鍵**進入設定選單，用左／右鍵選擇、確認鍵進入；在選單中長按側鍵 1 秒清除該組顏色。
3. 設定好的顏色與鏡頭參數（亮度、對比、飽和度、曝光、白平衡、翻轉、鏡像）會自動保存，下次開機直接使用。

@@@ cam-setup-i2c
### 先在感應器上設定顏色

神攝手的設定都在**感應器背面的彩色螢幕與按鍵**上完成，不用電腦：**短按側鍵**進入設定選單，用左／右鍵選擇、確認鍵進入；設定好的顏色與鏡頭參數會自動保存，下次開機直接使用。

@@@ cam-spike-official
> [!NOTE]
> - 教育版官方 App 的積木對應：「顏色」= 色相、「反射光」= 動態偵測值、「原始紅／綠／藍」= 色塊 X／Y／面積。
> - 家用版（Robot Inventor）的「顏色」只有 **0～10**（是教育版的十分之一），範例 2 的數字要自己改。
> - 本頁的數字是**目前出貨的韌體**（色相 0～100）。怎麼確認：開機後看感應器螢幕上方 `X` 的數字，**最大只到 98 左右**就是新版；如果會跑到 100 以上（最多 159），是舊版韌體，請先聯絡我們更新。

### WRO 版（HUE12）

樂高官方 App **沒有 HUE12 可以用的積木**（HUE12 不是樂高的感應器），官方 App 的範例是**教育版 SPIKE 3 App 的 Python 程式**。想用積木的話，請用 MBC 積木網站。

@@@ cam-spike-pyb
### WRO 版（HUE12）

主機要換成 Pybricks 韌體 **v4.0.0b5 以上**。MBC 積木網站上方選單「SPIKE 主機 → 安裝／更新 Pybricks 韌體」燒的是 v4.1.0，可以直接用。%%EXTRA%%

@@@ cam-ev3-two
### 兩種 EV3 韌體

| 韌體 | EV3 上顯示成 | 能用的軟體 |
| :--- | :--- | :--- |
| 學校老師創意賽版（取色＋色塊追蹤＋動態偵測） | 紅外線感應器 | EV3-G、EV3 Classroom、Pybricks、clev3r |
| WRO 2026 版（HUE12，12 個色點） | 第三方感應器（type 95） | EV3-G（MBC 自訂積木）、clev3r、Pybricks；**EV3 Classroom 讀不到** |

@@@ cam-ev3g-wro
### WRO 版（HUE12）：先匯入 MBC 自訂積木

用 EV3-G 的話，先下載 MBC 自訂積木 [MBC_Hue12Sensor.ev3b](/examples/esp32cam/ev3_beginner_wro/MBC_Hue12Sensor.ev3b)，在 EV3-G 選「**工具 → 積木匯入**」匯入一次（每台電腦只要做一次），WRO 版範例程式才打得開。

@@@ cam-classroom-wro
> [!WARNING]
> **WRO 版（HUE12）在 EV3 Classroom 讀不到**：它在 EV3 上是第三方感應器（type 95）。請改用 EV3-G（MBC 自訂積木）、clev3r 或 Pybricks。

@@@ cam-mvision-wro
### WRO 版（HUE12）

WRO 版的 mVision 資料**維持原本的 HUE12 打包**（12 色點顏色碼＋鮮豔度），照 `HUE12_AS_mVisionSensor_v1.0.8.mbr4` 範例解碼。WRO 2026 隊伍用的 HUE12 範例不受影響。學校版的資料 0~9 數值範圍跟 EV3／SPIKE 版一致，程式邏輯可以直接搬。

@@@ cam-microbit
> [!NOTE]
> 神攝手的 I2C 用 **SDA = IO3、SCL = IO1**（2026-10-21 定案），接上 I2C 後 USB 序列監看就沒了；硬體已確認可做，**上機待測**。

@@@ cam-arduino
> [!NOTE]
> 神攝手的 I2C（位址 `0x4C`）用 **SDA = IO3、SCL = IO1**，接上 I2C 後 USB 序列監看就沒了；硬體已確認可做，**上機待測**。神攝手本身是 ESP32，不能時脈延展，讀值前一定要等 2 ms（範例小幫手已經有 `delay(2)`）。

@@@ cam-mblock
> [!NOTE]
> 神攝手的 MakeBlock 版是 2026 年 10 月新增，訂購前請先 [LINE 詢問](https://line.me/R/ti/p/@692vcvuk) 確認出貨版本。神攝手接 mBot 走 IO1／IO3 串口，**上機驗證中**。

@@@ imu-mblock
> [!NOTE]
> 陀螺儀 IMU 的 MakeBlock 版是 2026 年 10 月新增，訂購前請先 [LINE 詢問](https://line.me/R/ti/p/@692vcvuk) 確認出貨版本。

@@@ exp6-power
### 接線與供電

1. **主機通訊線**：把擴充器的 **Host 通訊線**插進 SPIKE 主機任一孔（Port A~F）。
2. **獨立電源**：擴充器一定要接額外的獨立電源（電池盒或變壓器），專門供電給馬達；馬達電流不會從主機抽，主機訊號線也經過隔離晶片。
3. **接裝置**：樂高感應器（顏色、超音波、力量）與馬達，任意接在擴充器的 **1～6 號**孔。

> [!WARNING]
> EXP6 **只支援 SPIKE Prime**（不支援 Robot Inventor 與 EV3）。**換另一種寫法之前，先讓 EXP6 斷電一次**（關主機或拔掉 Host 線）：EXP6 一旦跑過 Pybricks 程式，就會一直用 Pybricks 的方式溝通，直到重新開機，這時候官方 App 會連不上。

@@@ exp6-official
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

@@@ exp6-pybricks
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

@@@ exp6-arduino
### EXP6 走 UART，不是 I2C

EXP6 跟 Arduino 用 **UART 序列埠**（TX／RX 兩條線）溝通，**不用上面的 I2C 接線**。出貨的 EXP6 就能直接用，不用重燒韌體：一開機是「樂高模式」，收到 Arduino 送來的第一包指令就自動切到「UART 模式」（跟 Pybricks 同一套指令）。

| 板子 | 可以用嗎 | 跟 EXP6 講話用 | 說明 |
| :--- | :---: | :---: | :--- |
| **Arduino UNO R4**（Minima／WiFi） | ⭐ 推薦 | D0、D1（`Serial1`） | D0／D1 跟 USB 是分開的兩組序列埠，上傳、看序列埠監控窗都不受影響 |
| Arduino Mega 2560 | ✅ | 18、19（`Serial1`） | 有 4 組序列埠 |
| ESP32 開發板 | ✅ | GPIO16、17（`Serial2`） | 訊號是 3.3V，第 4 腳要接 3V3；要先安裝 ESP32 板子套件 |
| Arduino UNO R3、Nano | ❌ | — | 唯一的硬體序列埠被 USB 用掉；軟體序列埠在 115200 收不穩 |

用「樂高 6 針轉杜邦線」（或樂高接頭轉接板）把 EXP6 的 **Host 孔**接到 Arduino：

| EXP6 Host 孔 | UNO R4 | Mega 2560 | ESP32 |
| :---: | :---: | :---: | :---: |
| 第 3 腳 GND | GND | GND | GND |
| 第 4 腳 VCC | **5V** | **5V** | **3V3** |
| 第 5 腳（EXP6 收） | D1（TX） | 18（TX1） | GPIO17（TX2） |
| 第 6 腳（EXP6 送） | D0（RX） | 19（RX1） | GPIO16（RX2） |
| 第 1、2 腳 | 不接 | 不接 | 不接 |

> [!IMPORTANT]
> - EXP6 的 XT30 電源座一定要接 **2～3S 鋰電池**，馬達和感應器都靠它供電；EXP6 **不會供電給 Arduino**。
> - **TX 接 EXP6 收、RX 接 EXP6 送**，兩條線是交叉的。
> - **第 4 腳的電壓要跟板子的訊號一樣**（UNO R4、Mega 接 5V，ESP32 接 3V3），這一腳也是 EXP6 的「開機鑰匙」：沒電 EXP6 不會開機。
> - 接線前先拔掉 USB 線和電池。

> [!WARNING]
> **看門狗**：EXP6 超過 **0.5 秒**沒收到 Arduino 的任何指令，就自動把馬達停下來。等待一律用 `exp6Wait(毫秒)`，不要用 `delay()`。

@@@ exp6-mbcblocks
### 接線與供電

1. 把 EXP6 的 **Host 通訊線**插進 SPIKE Prime 任一孔（Port A~F）。
2. EXP6 一定要接額外的獨立電源（電池盒或變壓器）供電給馬達。
3. 樂高感應器與馬達接在擴充器的 1～6 號孔。

> [!WARNING]
> EXP6 **只支援 SPIKE Prime**。**換另一種寫法之前，先讓 EXP6 斷電一次**（關主機或拔掉 Host 線）：EXP6 一旦跑過 Pybricks 程式，就會一直用 Pybricks 的方式溝通，直到重新開機。

@@@ servo6-common
> [!NOTE]
> Servo6 **即將推出**：已完成電路與韌體，正在做最後的主機實測，規格以正式上市版本為準。只支援 **SPIKE Prime**（不支援 Robot Inventor 與 EV3）。

### 接線與供電

1. **主機通訊線**：把擴充器的 SPIKE 連接線插進主機任一孔（Port A~F）。
2. **接舵機**：舵機 3 線插頭依板上標示（訊號／VCC／GND）插到 **CN1~CN6**，可接 SG90、MG996R 這類 500~2500 µs 訊號的舵機。
3. **不用接電池**：主機握手完成後會自動把電池電壓送到擴充器，綠燈由快閃轉為恆亮。舵機電源**總電流上限約 0.75 A**，大扭力舵機請先確認工作電流。

> [!TIP]
> 剛上電時擴充器不會輸出任何脈衝，舵機不會亂跳；收到第一個角度指令時會直接到位（不套速度斜坡），之後的指令才會用你設定的速度慢慢轉。

@@@ servo6-official
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

@@@ servo6-pybricks
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
