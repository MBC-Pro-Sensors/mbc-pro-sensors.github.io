#!/usr/bin/env pybricks-micropython
# ============================================================
# TOF2 雙眼雷射測距感應器 EV3 新手範例 06：用公釐精準停車：20 公分前停
# 軟體：樂高官方 EV3 MicroPython v2.0（microSD 開機卡＋VS Code「LEGO MINDSTORMS EV3 MicroPython」擴充套件）
# 接法：TOF2 雙眼雷射插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：模式 0 的距離 1 格＝1.2 公分，比賽要停在「剛好 20 公分」不夠準。模式 5 直接給左右兩眼的公釐（1 格＝1 公釐），這個範例把兩眼平均（跟模式 3 一樣），一邊往前開一邊顯示公釐，小於 200 公釐就停。用模式 5 是因為 EV3 原廠韌體切到模式 3 偶爾切不過去，模式 5 每次都切得過去。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.iodevices import Ev3devSensor
from pybricks.parameters import Port, Color
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = Ev3devSensor(Port.S1)     # TOF2 雙眼雷射插 1 號孔
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def read_leftMm():
    # 左眼距離（公釐）：0 ~ 1200 mm
    return sensor.read('IR-CAL')[0]


def read_rightMm():
    # 右眼距離（公釐）：0 ~ 1200 mm
    return sensor.read('IR-CAL')[1]


# ---------- 程式從這裡開始 ----------
# 清除螢幕
ev3.screen.clear()
# 兩輪用 30 的力量往前開
left_motor.dc(30)
right_motor.dc(30)
# mm ＝ 左右兩眼公釐的平均（跟模式 3 一樣）
mm = (read_leftMm() + read_rightMm()) / 2
# 一直往前，直到距離小於 200 公釐（20 公分）
while not (mm < 200):
    # 第 1 行顯示目前距離（公釐）
    ev3.screen.draw_text(0, 0, "MM " + str(mm) + "      ", Color.BLACK, Color.WHITE)
    # 再讀一次左右兩眼，重新算平均
    mm = (read_leftMm() + read_rightMm()) / 2
# 停車
left_motor.brake()
right_motor.brake()
# 嗶一聲
ev3.speaker.beep()
# 停下來後再讀一次
mm = (read_leftMm() + read_rightMm()) / 2
# 停下來後再顯示一次距離，看停得準不準
ev3.screen.draw_text(0, 0, "MM " + str(mm) + "      ", Color.BLACK, Color.WHITE)
