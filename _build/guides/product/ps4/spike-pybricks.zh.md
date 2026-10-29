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

### 函式庫與範例下載（Pybricks 4.1）

- [📥 全部下載（.zip）](/examples/ps4/ps4_pybricks_spike_v4.x.zip)：`MBC_sensors.py`、`MBC_gamepad_Lib.py` 和 6 個新手範例
- 單獨下載函式庫：[MBC_sensors.py](/examples/ps4/pybricks/MBC_sensors.py)（新專案用這個，一個檔適用全部 MBC 感應器，寫 `from MBC_sensors import MBC_GAMEPAD`）・[MBC_gamepad_Lib.py](/examples/ps4/pybricks/MBC_gamepad_Lib.py)（本頁範例與舊程式用）

> [!NOTE]
> Pybricks 3.6.1 版範例已不再更新，舊檔放在[下載專區](/downloads.md)的「舊版（不再更新）」。

### 全部按鍵合成一個數字：buttons() 和 buttons_psx()

函式庫有兩種「16 個鍵合成一個數字」的寫法。按鍵是同一組，只是**每個鍵代表的數字不一樣**：

| 按鍵 | `pad.buttons()`（MBC 排列） | `pad.buttons_psx()`（PSX 排列） |
| :--- | :--- | :--- |
| 上／右／下／左 | 1／2／4／8 | 16／32／64／128 |
| ▲／●／✕／■ | 16／32／64／128 | 4096／8192／16384／32768 |
| L2／R2／L1／R1 | 256／512／1024／2048 | 256／512／1024／2048 |
| SHARE（PS5：建立）／L3／R3／OPTIONS | 4096／8192／16384／32768 | 1／2／4／8 |
| 判斷某一個鍵 | `pad.pressed('CROSS')` | `pad.pressed_psx('CROSS')` 或 `pad.buttons_psx() & PSX_CROSS` |

- **新手用 `buttons()`／`pressed()`**：方向鍵、圖形鍵的數字跟官方 App「反射光」一樣，本頁範例都用這個。
- **`buttons_psx()` 用 PS2 手把原本的按鍵順序**，PS2 和 PS4 接收器送出的數字相同；Arduino（I2C）讀到的十六鍵也是這個排列，程式要跟 Arduino 共用同一套按鍵數字時用它。只有 SPIKE 版能用（讀模式 9 的第 7 個數字）。
- 沒連上手把時兩個都是 0。

```python
from pybricks.parameters import Port
from pybricks.tools import wait
from MBC_gamepad_Lib import MBC_GAMEPAD, PSX_CROSS

pad = MBC_GAMEPAD(Port.C)
while True:
    pad.update()                 # 每圈先更新一次
    n = pad.buttons_psx()        # PSX 排列的十六鍵
    if n & PSX_CROSS:            # ✕ 有按
        print('CROSS')
    if pad.pressed_psx('L1'):    # 用名稱判斷也可以
        print('L1')
    wait(50)
```
