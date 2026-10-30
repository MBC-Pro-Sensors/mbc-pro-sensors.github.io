### 先校準（只要做一次）

循行者要先「認識」你場地上的黑色和白色，數值才會準。校準完會記在感應器裡，重新開機也還在（除非換場地）。

1. 感應器插好、主機開機（感應器的燈亮起來）。
2. 按住感應器上的**校準按鈕約 0.5 秒**再放開（不要一直按著，按著不放會一直重新校準）。
3. **綠色校準指示燈**（靠近校準按鈕）亮起後，**5 秒內**把感應器在黑線和白色地面之間來回掃幾次，8 個感應點都要掃過**最黑的黑線**和**最白的白色區域**。
4. 掃的時候感應器離地 **0.5～1.2 公分**，太高會讀不準。
5. 綠燈熄滅＝校準完成，數值自動存進感應器。

**怎麼知道校準好了？** 放在白色地面時「線寬」是 0，壓在一般黑線上是 2～3。

<iframe src="https://www.youtube.com/embed/TKi7b20x8UY" title="循行者校準教學" frameborder="0" allowfullscreen loading="lazy" style="width:100%;aspect-ratio:16/9;border:none;border-radius:12px;"></iframe>

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

### PD 循線示範（速度 50）

跟 Pybricks 版同一套 PD 循線邏輯，教育版和玩具版各有積木和 Python，共 4 支，都在主機上跑過。

- 教育版（SPIKE App 3）：[📥 積木 .llsp3](/examples/line8/pd50/spike_app/line8_pd50_edu_blocks.llsp3)・[📥 Python（小幫手已內嵌）](/examples/line8/pd50/spike_app/line8_pd50_edu_python.py)
- 玩具版（Robot Inventor App）：[📥 積木 .lms](/examples/line8/pd50/spike_app/line8_pd50_ri_blocks.lms)・[📥 Python（小幫手已內嵌）](/examples/line8/pd50/spike_app/line8_pd50_ri_python.py)
- [📥 全部打包（含 Pybricks 版）](/examples/line8/line8_pd50_examples.zip)

- **接線**：感應器 C 埠、左馬達 E 埠、右馬達 F 埠。左右馬達裝的方向相反，**左馬達要給負的功率才會往前**：左＝−(速度＋PD)、右＝速度−PD。
- **讀值**：細位置＝「原始紅」− 100（兩版一樣）。線寬：**積木版（教育版、玩具版都一樣）用「顏色」積木**；只有**玩具版 Python** 用反射光 `get_reflected_light()`。
- 速度 50（功率 %）、KP 0.4、KD 1.5 是建議值；十字路口停車：線寬 > 5。
- 舊韌體也能用：官方 App 讀的是模式 0，不受模式 7 改版影響。

### 玩具版（Robot Inventor App）Python

> [!WARNING]
> LEGO 已在 2026-10-01 停止支援 Robot Inventor App，這段只給手上還有玩具版主機的人用；新專案建議用 SPIKE App 3 或 Pybricks。

- 讀法（實機量過）：細位置＝`get_rgb_intensity()[0] − 100`、線寬＝`get_reflected_light()`。`get_color()` 只有少數位置有顏色名稱，不能當位置；粗位置和每一路的光值這個 App 讀不到。
- 小幫手：[MBC_line8_ri_App_helper.py](/examples/line8/spike_app_ri/MBC_line8_ri_App_helper.py)（類別 `Line8RI`：`pos100()`、`width()`），整份貼在程式最上面。
- 範例：[ex01_line_follow.py](/examples/line8/spike_app_ri/ex01_line_follow.py)（小幫手已貼好）：循線＋十字路口停車，接線 C／E／F，已在 51515（App 10.5.4）實機跑過。

### 想學更厲害的循線方法？

可以找這幾位有豐富比賽與教學經驗的教練：[LegoLauXiao 教練](https://www.youtube.com/@LegoLauXiao)（[示範影片](https://www.youtube.com/watch?v=WgacdWLatbk)）、[legolaumo 教練](https://www.youtube.com/@legolaumo)（[示範影片](https://www.youtube.com/watch?v=T9bcndBNQvQ)）。
