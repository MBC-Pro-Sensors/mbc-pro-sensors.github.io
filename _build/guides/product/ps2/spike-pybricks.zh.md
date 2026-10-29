### 讓手把連上接收器

PS2 的 2.4G 手把和它的接收頭是**原廠成對**的，不用按任何配對鍵。

1. 接收器接好、主機開機。
2. **打開手把電源**，手把會自己連上。
3. 看接收器板子上的**綠燈**：**亮著＝還沒接到手把**；**熄掉＝已經接到手把**。
4. 手把上的 **ANALOG（或 MODE）燈要亮**，搖桿才有數值。接收器一接上手把會自動切到這個模式；燈沒亮就按一下手把中間的 ANALOG 鍵。

> [!CAUTION]
> 2.4GHz 是公共頻段，跟 Wi-Fi、微波爐共用。同一場地有 **12 套以上**同時使用時可能互相干擾，這種場合建議改用藍牙版（PS4／PS5）。

### 函式庫與範例下載（Pybricks 4.1）

- [📥 全部下載（.zip）](/examples/ps2/ps2_pybricks_spike_v4.x.zip)：`MBC_sensors.py`、`MBC_gamepad_Lib.py` 和 6 個新手範例
- 單獨下載函式庫：[MBC_sensors.py](/examples/ps2/pybricks/MBC_sensors.py)（新專案用這個，一個檔適用全部 MBC 感應器，寫 `from MBC_sensors import MBC_GAMEPAD`）・[MBC_gamepad_Lib.py](/examples/ps2/pybricks/MBC_gamepad_Lib.py)（本頁範例與舊程式用）

> [!NOTE]
> Pybricks 3.6.1 版範例已不再更新，舊檔放在[下載專區](/downloads.md)的「舊版（不再更新）」。

### 全部按鍵合成一個數字：buttons() 和 buttons_psx()

函式庫有兩種「16 個鍵合成一個數字」的寫法。按鍵是同一組，只是**每個鍵代表的數字不一樣**：

| 按鍵 | `pad.buttons()`（MBC 排列） | `pad.buttons_psx()`（PSX 排列） |
| :--- | :--- | :--- |
| 上／右／下／左 | 1／2／4／8 | 16／32／64／128 |
| ▲／●／✕／■ | 16／32／64／128 | 4096／8192／16384／32768 |
| L2／R2／L1／R1 | 256／512／1024／2048 | 256／512／1024／2048 |
| SELECT／L3／R3／START | 4096／8192／16384／32768 | 1／2／4／8 |
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

> [!TIP]
> 掌控者 PS2 的 EV3／SPIKE App／Pybricks 積木用法也整理在 [指令說明（PDF）](/examples/ps2/PS2_Controller_Command_Guide_ZH.pdf)。
