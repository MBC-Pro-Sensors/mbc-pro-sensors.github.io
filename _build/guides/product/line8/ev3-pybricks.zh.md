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

- [📥 打包下載（範例＋函式庫）](/examples/line8/line8_pybricks_ev3.zip)
- 函式庫：[MBC_line8_EV3_Lib.py](/examples/line8/ev3/MBC_line8_EV3_Lib.py)。要直接匯入你的 Pybricks 專案，**檔名必須完全一致**，`import` 才載得進來。

![把函式庫匯入 EV3 Pybricks 專案](/images/sensors/line8/ev3-pybricks-upload-lib.webp)

### 方式一：原生指令讀取

用 Pybricks 底層的 `device.mode(mode)` 直接拿陣列資料：

| 指令 | 回傳 | 內容 |
| :--- | :--- | :--- |
| `device.mode(0)` | Tuple（1 byte） | 新手模式：位置 `-8 ~ 8`，0 為中央 |
| `device.mode(1)` | Tuple（8 bytes） | 全覽除錯模式：8 路校準後的反射值，每路 `0 ~ 100`（0＝最暗、100＝最亮），第 1 路在最左邊 |
| `device.mode(2)` | Tuple（4 bytes） | 競技模式：高精度位置 `-100 ~ 100`、線寬 `0 ~ 8`、左邊界光值、右邊界光值 |

> [!NOTE]
> Pybricks 官方還沒把對應的圖形與底層指令更新好，原生範例程式之後補齊；目前建議直接用方式二。

### 方式二：匯入專屬函式庫

`line_init_port_multitask(port, multitask)` 初始化：`port` 傳 EV3 埠物件（例如 `hub.port.S1`）。**非多工**：`multitask` 填 `False`，呼叫時前面選 `call`；**多工**：填 `True`，呼叫時前面選 `await`。

![非多工設定](/images/sensors/line8/ev3-pybricks-non-multitask.webp)

![多工設定（await）](/images/sensors/line8/ev3-pybricks-multitask.webp)

| 模式 | 函數 | 範圍 | 說明 |
| :--- | :--- | :--- | :--- |
| 模式 0（基礎） | `line_pos8()` | `-8 ~ +8` | 整數循線位置，0 正中央 |
| 模式 1（解碼） | `line_ir_ch(ch)` | `1 ~ 9` | 單一通道量化光值，`ch` = 1~8，1 最暗、9 最亮 |
| 模式 2（競技） | `line_pos100()` | `-100 ~ +100` | 高解析插值位置，PID／PD 專用 |
| 模式 2（競技） | `line_width()` | `0 ~ 8` | 特徵線寬，偵測路口與寬線 |
| 模式 2（競技） | `line_edge_l()` | `0 ~ 100` | 左邊界光值（0 黑、100 白），防掉線 |
| 模式 2（競技） | `line_edge_r()` | `0 ~ 100` | 右邊界光值（0 黑、100 白），防掉線 |

> [!IMPORTANT]
> 不同模式的函數交叉呼叫會有切換延遲。一般使用者與比賽選手**全程用模式 2** 的函數；只有要讀所有感測器原始值、自己寫特殊演算法時，才單獨用模式 1。

範例：[積木版（.py）](/examples/line8/ev3/line8_ev3_block_with_lib.py)・[Python 版（.py）](/examples/line8/ev3/line8_ev3_python_with_lib.py)

### 想學更厲害的循線方法？

可以找這幾位有豐富比賽與教學經驗的教練：[LegoLauXiao 教練](https://www.youtube.com/@LegoLauXiao)（[示範影片](https://www.youtube.com/watch?v=WgacdWLatbk)）、[legolaumo 教練](https://www.youtube.com/@legolaumo)（[示範影片](https://www.youtube.com/watch?v=T9bcndBNQvQ)）。
