# 循行者 16 路 PD 循線示範（速度 50）／Pathfinder 16 PD line following demo (speed 50)
# 讀法：mode 7：內建 ColorSensor 的 ambient() 讀「細位置 −100～100」和「線寬」（小數第 1 位；16 路最多顯示 9；要 2026-10-30 以後的韌體，舊韌體線寬會是 0）
# Reading: mode 7: built-in ColorSensor ambient() gives fine position −100~100 and line width (1st decimal, up to 9; needs firmware from 2026-10-30, older firmware reads width 0)
# 接線：循行者 → C 孔；左馬達 → E 孔；右馬達 → F 孔。左右兩顆馬達裝的方向相反，所以左馬達設 COUNTERCLOCKWISE（反轉），兩輪給正值車子都往前。
# Wiring: sensor → port C, left motor → E, right motor → F. The motors face opposite ways, so the left one is COUNTERCLOCKWISE and positive power drives both forward.
# 方向：站在車後往前看，線在左＝負 → 左輪慢、右輪快 → 往左轉／line on the left = negative → left wheel slower → turns left
# KP、KD 是建議值，請在你的車上調整／KP and KD are starting values, tune them on your robot
# 積木版：同名的 _block 檔（Pybricks Code 打開就是積木畫面）／Block version: the matching _block file
from pybricks.hubs import PrimeHub
from pybricks.parameters import Direction, Port
from pybricks.pupdevices import ColorSensor, Motor
from pybricks.tools import wait

# Set up.
prime_hub = PrimeHub()
Line16 = ColorSensor(Port.C)
motorL = Motor(Port.E, Direction.COUNTERCLOCKWISE)
motorR = Motor(Port.F, Direction.CLOCKWISE)
SPEED = 50     # 前進速度（%）／forward speed (%)
KP = 0.4     # 轉彎靈敏度：車子左右甩就調小／steering gain: lower it if the robot wobbles
KD = 1.5     # 抑制甩動：轉彎太慢就調小／damping: lower it if turns are too slow
error = 0
lastError = 0
PD = 0

def getLinePos100():
    return round(Line16.ambient() // 1 - 100)

# 環境光＝細位置+100 . 線寬：Pybricks 4.1.2 起 ambient() 只有小數 1 位，線寬放在小數第 1 位（16 路 9 代表 9 以上）
# ambient() = fine position + 100 . width (1st decimal; on 16-way 9 means 9 or more)
def getLineWidth100():
    return round(Line16.ambient() * 10) % 10


# The main program starts here.
while True:
    if getLineWidth100() > 8:     # 線寬太寬＝十字路口（整排踩到線）：停車／too wide = crossing: stop
        motorL.dc(0)
        motorR.dc(0)
    else:
        error = getLinePos100()
        PD = error * KP + (error - lastError) * KD     # P：偏多少轉多少；D：偏移變快時多轉一點／P: how far off; D: how fast it drifts
        motorL.dc(SPEED + PD)
        motorR.dc(SPEED - PD)
        lastError = error
    wait(10)
