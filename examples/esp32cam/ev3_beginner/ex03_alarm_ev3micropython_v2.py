#!/usr/bin/env pybricks-micropython
# ============================================================
# 神攝手 ESP32CAM 視覺感應器（學校老師創意賽版） EV3 新手範例 03：防盜警報：有東西動就叫
# 軟體：樂高官方 EV3 MicroPython v2.0（microSD 開機卡＋VS Code「LEGO MINDSTORMS EV3 MicroPython」擴充套件）
# 接法：神攝手（學校版）插 1 號孔
# 這個範例會做什麼：模式 1 的第 4 個數字是「畫面動了多少」。相機對著門口，有人經過數字就會變大，超過 20 就嗶嗶叫。這個範例相機不用做任何設定。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.iodevices import Ev3devSensor
from pybricks.parameters import Port, Color
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = Ev3devSensor(Port.S1)     # 神攝手（學校版）插 1 號孔


def read_motion():
    # 畫面動了多少：0 ~ 100
    return sensor.read('IR-SEEK')[3]


# ---------- 程式從這裡開始 ----------
# 一直重複
while True:
    # 螢幕顯示動態值
    ev3.screen.draw_text(0, 0, "MOTION " + str(read_motion()) + "      ", Color.BLACK, Color.WHITE)
    # 如果動態值大於 20
    if read_motion() > 20:
        # 有東西在動：嗶！
        ev3.speaker.beep()
