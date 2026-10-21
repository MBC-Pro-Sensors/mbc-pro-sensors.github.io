#!/usr/bin/env pybricks-micropython
# ============================================================
# 掌控者 PS4/PS5 藍牙遙控接收器 EV3 新手範例 03：左搖桿控制一顆馬達的快慢
# 軟體：樂高官方 EV3 MicroPython v2.0（microSD 開機卡＋VS Code「LEGO MINDSTORMS EV3 MicroPython」擴充套件）
# 接法：掌控者 PS4插 1 號孔；馬達接 B
# 這個範例會做什麼：搖桿的數字是 0~99、中間 50。把它減 50 再乘 2，就變成馬達要的 -100~+100：推越多轉越快，往下推就倒轉。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.iodevices import Ev3devSensor
from pybricks.parameters import Port, Color
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = Ev3devSensor(Port.S1)     # 掌控者 PS4插 1 號孔
motor_b = Motor(Port.B)             # B 埠馬達


def read_ly():
    # 左搖桿上下：0 ~ 99，50＝中間，往上變大
    return sensor.read('IR-SEEK')[3]


# ---------- 程式從這裡開始 ----------
# 一直重複
while True:
    # 力量 power ＝（左搖桿上下 − 50）× 2
    power = (read_ly() - 50) * 2
    # B 馬達用 power 的力量轉
    motor_b.dc(power)
    # 螢幕顯示現在的力量
    ev3.screen.draw_text(0, 0, "POWER " + str(power) + "      ", Color.BLACK, Color.WHITE)
