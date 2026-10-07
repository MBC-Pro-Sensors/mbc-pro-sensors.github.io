<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# SPIKE Prime 循線 PID 教學：用 Pybricks 寫出穩定的高速循線（附程式）

循線跑不快、一快就抖，多半是控制方法的問題。這篇用最白話的方式說明 PID 是什麼，並附上可以直接改來用的 **Pybricks** 程式：一份用原廠顏色感應器，一份用多通道循線感應器。

---

## PID 是什麼？用開車來想

想像你在開車，要讓車子保持在車道正中央：

| 項目 | 白話解釋 | 對循線的作用 |
| :--- | :--- | :--- |
| **P（比例）** | 偏越多，方向盤轉越多 | 主要的修正力量 |
| **I（積分）** | 一直偏同一邊，就慢慢加大修正 | 修正長時間的固定偏差；循線通常**不需要** |
| **D（微分）** | 偏差變化得很快，就先收一點方向盤 | **抑制抖動**、防止修正過頭 |

循線最常用的是 **PD 控制**：P 負責拉回中間，D 負責不要拉過頭。只有 P 的話，速度一快就會左右甩。

## 方法一：原廠顏色感應器（沿著黑白交界走）

一顆顏色感應器看不出線在左邊還是右邊，所以做法是**沿著黑線的邊緣走**：讀到的反射值比目標值大（太白）就往黑線那邊轉，比目標值小（太黑）就轉回來。

```python
from pybricks.hubs import PrimeHub
from pybricks.parameters import Direction, Port
from pybricks.pupdevices import ColorSensor, Motor
from pybricks.tools import wait

hub = PrimeHub()
sensor = ColorSensor(Port.C)
motorL = Motor(Port.E, Direction.COUNTERCLOCKWISE)
motorR = Motor(Port.F, Direction.CLOCKWISE)

target = 50      # 先量白色、黑色的反射值，取兩者中間
Kp = 0.8
Kd = 2.0
base = 40        # 基本速度（-100 ~ 100）
last_error = 0

while True:
    error = sensor.reflection() - target
    turn = Kp * error + Kd * (error - last_error)
    motorL.dc(base + turn)
    motorR.dc(base - turn)
    last_error = error
    wait(10)
```

> [!TIP]
> 機器人往反方向修正的話，把 `turn` 前面加上負號，或改成沿著線的另一邊走。

**這個方法的限制：** 只能跟著一條邊走，遇到十字路口、分岔或寬線很容易判斷錯，速度也很難再提高。

## 方法二：多通道循線感應器（直接知道線在哪）

[循行者 8 路](/sensors/line8/index.md) 一次看 8 個點，直接回傳線的位置 `pos100()`：**0 = 正中央，−100 = 線在最右邊，+100 = 線在最左邊**。程式不用再猜，PD 只要直接用這個數字算：

```python
from pybricks.hubs import PrimeHub
from pybricks.parameters import Direction, Port
from pybricks.pupdevices import Motor
from pybricks.tools import wait

from MBC_line8_obj_Lib import MBC_LINE8

hub = PrimeHub()
line8 = MBC_LINE8(3)          # 感應器接在 Port C（3）
motorL = Motor(Port.E, Direction.COUNTERCLOCKWISE)
motorR = Motor(Port.F, Direction.CLOCKWISE)

Kp = 1.2
Kd = 3.6
base = 75
last_pos = 0

while True:
    pos = line8.pos100()          # -100 ~ +100
    if line8.width() > 6:         # 很寬 = 碰到橫線或終點區，先停下
        motorL.dc(0)
        motorR.dc(0)
    else:
        turn = Kp * pos + Kd * (pos - last_pos)
        motorL.dc(base + turn)
        motorR.dc(base - turn)
        last_pos = pos
    wait(10)
```

這份程式的參數（Kp = 1.2、Kd = 3.6、速度 75）來自官方範例，可以直接當起點。使用前請依 [Line8 Pybricks 教學](/sensors/line8/spike-pybricks.md) 下載函式庫並加入專案；馬達的孔位和轉向請依你的機器人修改。

> [!TIP]
> 機器人往遠離線的方向修正，代表馬達安裝方向和範例不同：把 `turn` 改成 `-turn` 即可。

## 參數怎麼調？4 個步驟

1. **先把 Kd 設成 0、速度放慢**，只調 Kp：從小慢慢加，加到機器人能跟著彎道走、開始輕微左右擺為止。
2. **加入 Kd**：從 Kp 的 2～3 倍開始試，左右擺動應該會明顯變小。
3. **逐步提高速度**：每次加一點，過彎跟不上就稍微加大 Kp，開始抖就加大 Kd。
4. **到比賽場地重新校準並微調**：燈光和地圖不同，參數可能要小修。

> [!NOTE]
> 看到抖動，先加 Kd，而不是降低 Kp。Kp 太小會讓過彎跟不上，這是很多人調參數時繞遠路的原因。

## 延伸閱讀

- [循線一加速就抖、過彎就脫線？5 個原因與解法](/articles/spike-line-following-wobble.md)
- [循行者 8 路產品介紹](/sensors/line8/index.md)
- [循行者 16 路產品介紹](/sensors/line16/index.md)：16 通道，更適合高速過彎與十字路口多的地圖

有問題或想詢價，歡迎 [LINE 問我們](https://line.me/R/ti/p/@692vcvuk)。
