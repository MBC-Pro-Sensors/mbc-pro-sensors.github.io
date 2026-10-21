# ============================================================
# 掌控者 PS4/PS5 藍牙遙控接收器 EV3 新手範例 04：雙搖桿坦克車
# 軟體：Pybricks v4（用 Chrome 打開 https://code.pybricks.com ，EV3 版目前是 Beta）
# 接法：掌控者 PS4插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：左搖桿管左輪、右搖桿管右輪，跟開坦克一樣：兩個都往前推就前進，一前一後就原地轉。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = LUMPDevice(Port.S1)       # 掌控者 PS4插 1 號孔
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def read_ly():
    # 左搖桿上下：0 ~ 99，50＝中間，往上變大
    return sensor.read(1)[3]


def read_ry():
    # 右搖桿上下：0 ~ 99，50＝中間，往上變大
    return sensor.read(1)[7]


# ---------- 程式從這裡開始 ----------
# 一直重複
while True:
    # 左輪力量 ＝（左搖桿上下 − 50）× 2
    left = (read_ly() - 50) * 2
    # 右輪力量 ＝（右搖桿上下 − 50）× 2
    right = (read_ry() - 50) * 2
    # 兩輪各自用自己的力量轉
    left_motor.dc(left)
    right_motor.dc(right)
