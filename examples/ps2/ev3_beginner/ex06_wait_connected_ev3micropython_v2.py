#!/usr/bin/env pybricks-micropython
# ============================================================
# 掌控者 PS2 2.4G 遙控接收器 EV3 新手範例 06：手把連上才出發
# 軟體：樂高官方 EV3 MicroPython v2.0（microSD 開機卡＋VS Code「LEGO MINDSTORMS EV3 MicroPython」擴充套件）
# 接法：掌控者 PS2插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：手把還沒連上時搖桿是中間、按鍵是 0，程式分不出「沒按」和「沒連上」。模式 3 的數字加了「已連線」：大於等於 256 就是手把連上了。這個範例等手把連上才嗶一聲出發，之後用方向鍵上（256＋1）前進、下（256＋4）後退，手把斷線就馬上停車。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.iodevices import Ev3devSensor
from pybricks.parameters import Port, Color
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = Ev3devSensor(Port.S1)     # 掌控者 PS2插 1 號孔
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def read_status3():
    # 方向鍵＋形狀鍵＋連線狀態：0 ~ 511：方向鍵（0~15）＋ 形狀鍵 × 16 ＋ 已連線 × 256，大於等於 256＝手把已連上
    return sensor.read('IR-REM-A')[0]


# ---------- 程式從這裡開始 ----------
# 清除螢幕
ev3.screen.clear()
# 螢幕顯示 WAIT PAD：等手把
ev3.screen.draw_text(0, 0, "WAIT PAD      ", Color.BLACK, Color.WHITE)
# 等到狀態值大於等於 256（手把已連上）
while not (read_status3() >= 256):
    wait(10)
# 連上了：嗶一聲
ev3.speaker.beep()
# 一直重複
while True:
    # 第 2 行顯示狀態值
    ev3.screen.draw_text(0, 16, "STATUS " + str(read_status3()) + "      ", Color.BLACK, Color.WHITE)
    # 如果狀態值小於 256（手把斷線了）
    if read_status3() < 256:
        # 手把斷線：馬上停車
        left_motor.brake()
        right_motor.brake()
        # 螢幕顯示 PAD LOST
        ev3.screen.draw_text(0, 0, "PAD LOST      ", Color.BLACK, Color.WHITE)
    else:
        # 螢幕顯示 PAD OK
        ev3.screen.draw_text(0, 0, "PAD OK      ", Color.BLACK, Color.WHITE)
        # 如果狀態值＝257（已連上＋方向鍵上）
        if read_status3() == 257:
            # 已連上＋方向鍵上（256＋1）：前進
            left_motor.dc(40)
            right_motor.dc(40)
        # 否則，如果狀態值＝260（已連上＋方向鍵下）
        elif read_status3() == 260:
            # 已連上＋方向鍵下（256＋4）：後退
            left_motor.dc(-40)
            right_motor.dc(-40)
        else:
            # 連上但沒按：停車
            left_motor.brake()
            right_motor.brake()
