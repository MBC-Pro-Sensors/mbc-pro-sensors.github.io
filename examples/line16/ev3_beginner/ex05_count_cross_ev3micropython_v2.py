#!/usr/bin/env pybricks-micropython
# ============================================================
# 循行者 16 路循線感應器 EV3 新手範例 05：數路口：第 3 個路口停車
# 軟體：樂高官方 EV3 MicroPython v2.0（microSD 開機卡＋VS Code「LEGO MINDSTORMS EV3 MicroPython」擴充套件）
# 接法：循行者 16 路插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：比賽常常要「走到第幾個路口」。這個範例用一個變數 count 數路口，用另一個變數 onCross 記住「現在是不是正壓在路口上」，避免同一個路口被數很多次。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.iodevices import Ev3devSensor
from pybricks.parameters import Port, Color
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = Ev3devSensor(Port.S1)     # 循行者 16 路插 1 號孔
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def read_pos100():
    # 線的位置（細）：-100 ~ +100，0＝線在正中間
    return sensor.read('IR-REMOTE')[0]


def read_width():
    # 線的寬度（壓到黑線的格數）：0 ~ 16，0＝看不到線
    return sensor.read('IR-REMOTE')[1]


# ---------- 程式從這裡開始 ----------
# 路口數 count 從 0 開始
count = 0
# onCross＝0 代表「現在不在路口上」
onCross = 0
# 一直循線，直到數滿 3 個路口
while not (count >= 3):
    # 比例循線：turn ＝ 線的位置 × 0.3
    turn = read_pos100() * 0.3
    # 左輪＝25＋turn、右輪＝25－turn
    left_motor.dc(25 + turn)
    right_motor.dc(25 - turn)
    # 如果線寬大於等於 10（壓在路口上）
    if read_width() >= 10:
        # 而且剛剛還不在路口上
        if onCross == 0:
            # 剛壓到新路口：count 加 1
            count = count + 1
            # 記住「正在路口上」，同一個路口不要再數
            onCross = 1
            # 嗶一聲
            ev3.speaker.beep()
    else:
        # 離開路口了：onCross 改回 0，準備數下一個
        onCross = 0
    # 螢幕顯示已經數到第幾個路口
    ev3.screen.draw_text(0, 0, "COUNT " + str(count) + "      ", Color.BLACK, Color.WHITE)
# 停車
left_motor.brake()
right_motor.brake()
