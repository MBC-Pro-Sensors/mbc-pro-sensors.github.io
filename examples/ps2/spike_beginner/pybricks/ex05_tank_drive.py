# 掌控者PS2 範例 5：雙搖桿坦克開車（左搖桿管左輪、右搖桿管右輪）
# 接線：掌控者PS2 接收器 → C 埠；左輪馬達 → E 埠；右輪馬達 → F 埠
from pybricks.hubs import PrimeHub
from pybricks.parameters import Color, Direction, Port
from pybricks.pupdevices import Motor
from pybricks.tools import wait
from MBC_gamepad_Lib import MBC_GAMEPAD

hub = PrimeHub()
pad = MBC_GAMEPAD(Port.C)                          # 接收器插在 C 埠
left = Motor(Port.E, Direction.COUNTERCLOCKWISE)   # 左輪：反過來裝，所以設成逆時針才是往前
right = Motor(Port.F)                              # 右輪

# 開始前的保護：等兩支搖桿都在中間（4~6）才開始，車子才不會一開機就衝出去
while True:
    pad.update()
    if 4 <= pad.stick('LY') <= 6 and 4 <= pad.stick('RY') <= 6:
        break
    wait(20)
hub.light.on(Color.GREEN)                          # 綠燈 = 可以開了

while True:
    pad.update()                                   # 先讀一次手把
    ly = pad.stick('LY')                           # 左搖桿上下 1~9
    ry = pad.stick('RY')                           # 右搖桿上下 1~9
    # 死區：搖桿在中間附近（4~6）就當成 0，放開手車子就停
    if 4 <= ly <= 6:
        left_power = 0
    else:
        left_power = (ly - 5) * 25                 # 7 → 50%、9 → 100%、3 → -50%、1 → -100%
    if 4 <= ry <= 6:
        right_power = 0
    else:
        right_power = (ry - 5) * 25
    left.dc(left_power)
    right.dc(right_power)
    wait(20)
