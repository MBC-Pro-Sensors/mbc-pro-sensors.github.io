# 掌控者PS2 範例 4：方向鍵開車（按住才走，放開就停）
# 接線：掌控者PS2 接收器 → C 埠；左輪馬達 → E 埠；右輪馬達 → F 埠
from pybricks.parameters import Direction, Port
from pybricks.pupdevices import Motor
from pybricks.tools import wait
from MBC_gamepad_Lib import MBC_GAMEPAD

pad = MBC_GAMEPAD(Port.C)                          # 接收器插在 C 埠
left = Motor(Port.E, Direction.COUNTERCLOCKWISE)   # 左輪：反過來裝，所以設成逆時針才是往前
right = Motor(Port.F)                              # 右輪

while True:
    pad.update()                    # 先讀一次手把
    if pad.pressed('UP'):           # 上 → 前進（功率 50%）
        left.dc(50)
        right.dc(50)
    elif pad.pressed('DOWN'):       # 下 → 後退
        left.dc(-50)
        right.dc(-50)
    elif pad.pressed('LEFT'):       # 左 → 原地左轉：左輪往後、右輪往前
        left.dc(-30)
        right.dc(30)
    elif pad.pressed('RIGHT'):      # 右 → 原地右轉
        left.dc(30)
        right.dc(-30)
    else:                           # 都沒按 → 停
        left.dc(0)
        right.dc(0)
    wait(50)
