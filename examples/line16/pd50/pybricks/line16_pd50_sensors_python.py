# 循行者 16 路 PD 循線示範（速度 50）／Pathfinder 16 PD line following demo (speed 50)
# 讀法：統一函式庫 MBC_sensors.py（所有 MBC 感應器共用一個檔）：pos100() 細位置、width() 線寬（函式庫檔要先上傳到 Pybricks Code）
# Reading: shared library MBC_sensors.py (one file for all MBC sensors): pos100() fine position, width() line width (upload it to Pybricks Code first)
# 接線：循行者 → C 孔；左馬達 → E 孔；右馬達 → F 孔。左右兩顆馬達裝的方向相反，所以左馬達設 COUNTERCLOCKWISE（反轉），兩輪給正值車子都往前。
# Wiring: sensor → port C, left motor → E, right motor → F. The motors face opposite ways, so the left one is COUNTERCLOCKWISE and positive power drives both forward.
# 方向：站在車後往前看，線在左＝負 → 左輪慢、右輪快 → 往左轉／line on the left = negative → left wheel slower → turns left
# KP、KD 是建議值，請在你的車上調整／KP and KD are starting values, tune them on your robot
# 積木版：同名的 _block 檔（Pybricks Code 打開就是積木畫面）／Block version: the matching _block file
from pybricks.hubs import PrimeHub
from pybricks.parameters import Direction, Port
from pybricks.pupdevices import Motor
from pybricks.tools import wait

from MBC_sensors import MBC_LINE16

# Set up.
line16 = MBC_LINE16(3)
prime_hub = PrimeHub()
motorL = Motor(Port.E, Direction.COUNTERCLOCKWISE)
motorR = Motor(Port.F, Direction.CLOCKWISE)
SPEED = 50     # 前進速度（%）／forward speed (%)
KP = 0.4     # 轉彎靈敏度：車子左右甩就調小／steering gain: lower it if the robot wobbles
KD = 1.5     # 抑制甩動：轉彎太慢就調小／damping: lower it if turns are too slow
error = 0
lastError = 0
PD = 0


# The main program starts here.
while True:
    if line16.width() > 10:     # 線寬太寬＝十字路口（整排踩到線）：停車／too wide = crossing: stop
        motorL.dc(0)
        motorR.dc(0)
    else:
        error = line16.pos100()
        PD = error * KP + (error - lastError) * KD     # P：偏多少轉多少；D：偏移變快時多轉一點／P: how far off; D: how fast it drifts
        motorL.dc(SPEED + PD)
        motorR.dc(SPEED - PD)
        lastError = error
    wait(10)
