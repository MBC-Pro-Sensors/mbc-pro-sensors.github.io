#!/usr/bin/env pybricks-micropython
# ============================================================
# 循行者 16 路循線感應器 EV3 新手範例 03：比例循線：偏越多、轉越多
# 軟體：樂高官方 EV3 MicroPython v2.0（microSD 開機卡＋VS Code「LEGO MINDSTORMS EV3 MicroPython」擴充套件）
# 接法：循行者 16 路插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：三段式循線會一直搖擺。這個範例把「線的位置 × 一個數字」當成轉彎的力量：偏一點就轉一點、偏很多就轉很多，車子會走得更順。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.iodevices import Ev3devSensor
from pybricks.parameters import Port
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = Ev3devSensor(Port.S1)     # 循行者 16 路插 1 號孔
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def read_pos():
    # 線的位置：-16 ~ +16，0＝線在正中間
    return sensor.read('IR-PROX')[0]


# ---------- 程式從這裡開始 ----------
# 一直重複
while True:
    # 轉彎力量 turn ＝ 線的位置 × 2
    turn = read_pos() * 2
    # 左輪＝30＋turn、右輪＝30－turn：線偏哪邊，車子就往哪邊修
    left_motor.dc(30 + turn)
    right_motor.dc(30 - turn)
