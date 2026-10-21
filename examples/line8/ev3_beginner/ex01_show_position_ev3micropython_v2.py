#!/usr/bin/env pybricks-micropython
# ============================================================
# 循行者 8 路循線感應器 EV3 新手範例 01：看看線在哪裡
# 軟體：樂高官方 EV3 MicroPython v2.0（microSD 開機卡＋VS Code「LEGO MINDSTORMS EV3 MicroPython」擴充套件）
# 接法：循行者 8 路插 1 號孔
# 這個範例會做什麼：把感應器放在黑線上，EV3 螢幕會顯示一個數字告訴你線在哪裡。這是所有循線程式的第一步：先學會「讀數字」。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.iodevices import Ev3devSensor
from pybricks.parameters import Port, Color
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = Ev3devSensor(Port.S1)     # 循行者 8 路插 1 號孔


def read_pos():
    # 線的位置：-8 ~ +8，0＝線在正中間
    return sensor.read('IR-PROX')[0]


# ---------- 程式從這裡開始 ----------
# 先把螢幕清乾淨
ev3.screen.clear()
# 下面的動作一直重複
while True:
    # 讀取線的位置，顯示在螢幕第 1 行
    ev3.screen.draw_text(0, 0, "POS " + str(read_pos()) + "      ", Color.BLACK, Color.WHITE)
    # 等 0.1 秒再讀下一次，數字才不會閃太快
    wait(100)
