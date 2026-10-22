#!/usr/bin/env pybricks-micropython
# ============================================================
# IMU 陀螺儀感應器 EV3 新手範例 06：俯仰角：上坡就減速
# 軟體：樂高官方 EV3 MicroPython v2.0（microSD 開機卡＋VS Code「LEGO MINDSTORMS EV3 MicroPython」擴充套件）
# 接法：IMU 陀螺儀插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：模式 6 是「俯仰角」：車頭上抬或下壓幾度，平放是 0。車子在平地用 50 的速度跑，一上坡（或下坡）俯仰角超過 8 度就放慢到 20，比較不會翻車或衝太快。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.iodevices import Ev3devSensor
from pybricks.parameters import Port, Color
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = Ev3devSensor(Port.S1)     # IMU 陀螺儀插 1 號孔（當一般 UART 裝置讀原始值）
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def read_tilt():
    # 俯仰角（車頭上抬或下壓幾度）：-90 ~ +90 度，平放＝0（板子絲印 X 軸）
    return sensor.read('TILT-ANG')[0]


# ---------- 程式從這裡開始 ----------
# 清除螢幕
ev3.screen.clear()
# 一直重複
while True:
    # 第 1 行顯示俯仰角（度）
    ev3.screen.draw_text(0, 0, "TILT " + str(read_tilt()) + "      ", Color.BLACK, Color.WHITE)
    # 如果俯仰角的絕對值大於 8 度
    if abs(read_tilt()) > 8:
        # 在坡上（俯仰角超過 8 度）：放慢到 20
        left_motor.dc(20)
        right_motor.dc(20)
        # 第 2 行顯示 SLOPE
        ev3.screen.draw_text(0, 16, "SLOPE      ", Color.BLACK, Color.WHITE)
    else:
        # 平地：用 50 往前開
        left_motor.dc(50)
        right_motor.dc(50)
        # 第 2 行顯示 FLAT
        ev3.screen.draw_text(0, 16, "FLAT      ", Color.BLACK, Color.WHITE)
