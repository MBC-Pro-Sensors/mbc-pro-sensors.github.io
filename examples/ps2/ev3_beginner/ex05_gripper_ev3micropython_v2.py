#!/usr/bin/env pybricks-micropython
# ============================================================
# 掌控者 PS2 2.4G 遙控接收器 EV3 新手範例 05：✕ 夾、● 放：控制夾爪
# 軟體：樂高官方 EV3 MicroPython v2.0（microSD 開機卡＋VS Code「LEGO MINDSTORMS EV3 MicroPython」擴充套件）
# 接法：掌控者 PS2插 1 號孔；馬達接 A
# 這個範例會做什麼：用形狀鍵控制接在 A 埠的中馬達（夾爪）：按 ✕ 夾緊、按 ● 放開，都不按就停。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.iodevices import Ev3devSensor
from pybricks.parameters import Port
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = Ev3devSensor(Port.S1)     # 掌控者 PS2插 1 號孔
motor_a = Motor(Port.A)             # A 埠馬達


def read_shape():
    # 形狀鍵（▲1、●2、✕4、■8）：0 ~ 15
    return sensor.read('IR-SEEK')[2]


# ---------- 程式從這裡開始 ----------
# 一直重複
while True:
    # 如果形狀鍵＝4（✕）
    if read_shape() == 4:
        # 按 ✕：夾爪往夾緊的方向轉
        motor_a.dc(40)
    # 否則，如果形狀鍵＝2（●）
    elif read_shape() == 2:
        # 按 ●：夾爪往放開的方向轉
        motor_a.dc(-40)
    else:
        # 都沒按：夾爪停住
        motor_a.brake()
