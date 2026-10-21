# ============================================================
# TOF2 雙眼雷射測距感應器 EV3 新手範例 05：左眼右眼：閃開兩邊的障礙物
# 軟體：Pybricks v4（用 Chrome 打開 https://code.pybricks.com ，EV3 版目前是 Beta）
# 接法：TOF2 雙眼雷射插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：雙眼雷射可以分別讀左眼和右眼。左邊太近就往右閃，右邊太近就往左閃，都沒東西就直走。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port, Color
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = LUMPDevice(Port.S1)       # TOF2 雙眼雷射插 1 號孔
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def read_left():
    # 左眼距離：0 ~ 100（1 格＝1.2 公分）
    return sensor.read(1)[0]


def read_right():
    # 右眼距離：0 ~ 100（1 格＝1.2 公分）
    return sensor.read(1)[1]


# ---------- 程式從這裡開始 ----------
# 一直重複
while True:
    # 螢幕第 1 行顯示左眼距離
    ev3.screen.draw_text(0, 0, "LEFT " + str(read_left()) + "      ", Color.BLACK, Color.WHITE)
    # 螢幕第 2 行顯示右眼距離
    ev3.screen.draw_text(0, 16, "RIGHT " + str(read_right()) + "      ", Color.BLACK, Color.WHITE)
    # 如果左眼距離小於 25
    if read_left() < 25:
        # 左邊太近：左輪轉、右輪停，車子往右閃
        left_motor.dc(30)
        right_motor.dc(0)
    # 否則，如果右眼距離小於 25
    elif read_right() < 25:
        # 右邊太近：往左閃
        left_motor.dc(0)
        right_motor.dc(30)
    else:
        # 兩邊都沒東西：直走
        left_motor.dc(30)
        right_motor.dc(30)
