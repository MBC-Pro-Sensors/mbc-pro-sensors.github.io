# ============================================================
# 循行者 8 路循線感應器 EV3 新手範例 04：碰到十字路口就停車
# 軟體：Pybricks v4（用 Chrome 打開 https://code.pybricks.com ，EV3 版目前是 Beta）
# 接法：循行者 8 路插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：感應器還會告訴你「壓到黑線的格數」（線寬）。平常只有 2~3 格，到十字路口會一下子變很多。這個範例邊循線邊看線寬，大於等於 6 就停車並嗶一聲。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = LUMPDevice(Port.S1)       # 循行者 8 路插 1 號孔
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def read_width():
    # 線的寬度（壓到黑線的格數）：0 ~ 8，0＝看不到線
    return sensor.read(2)[1]


def read_pos100():
    # 線的位置（細）：-100 ~ +100，0＝線在正中間
    return sensor.read(2)[0]


# ---------- 程式從這裡開始 ----------
# 一直循線，直到線寬大於等於 6（到路口了）
while not (read_width() >= 6):
    # 轉彎力量 turn ＝ 線的位置（-100~100）× 0.3
    turn = read_pos100() * 0.3
    # 一樣用比例循線往前走，速度放慢到 25
    left_motor.dc(25 + turn)
    right_motor.dc(25 - turn)
# 停車
left_motor.brake()
right_motor.brake()
# 嗶一聲告訴你到了
ev3.speaker.beep()
