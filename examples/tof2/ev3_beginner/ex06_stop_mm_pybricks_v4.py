# ============================================================
# TOF2 雙眼雷射測距感應器 EV3 新手範例 06：用公釐精準停車：20 公分前停
# 軟體：Pybricks v4（用 Chrome 打開 https://code.pybricks.com ，EV3 版目前是 Beta）
# 接法：TOF2 雙眼雷射插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：模式 0 的距離 1 格＝1.2 公分，比賽要停在「剛好 20 公分」不夠準。模式 3 直接給公釐（1 格＝1 公釐），這個範例一邊往前開一邊顯示公釐，小於 200 公釐就停。
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


def read_avgMm():
    # 左右兩眼的平均距離（公釐）：0 ~ 1200 mm，1200＝前面沒東西
    return sensor.read(3)[0]


# ---------- 程式從這裡開始 ----------
# 清除螢幕
ev3.screen.clear()
# 兩輪用 30 的力量往前開
left_motor.dc(30)
right_motor.dc(30)
# 一直往前，直到距離小於 200 公釐（20 公分）
while not (read_avgMm() < 200):
    # 第 1 行顯示目前距離（公釐）
    ev3.screen.draw_text(0, 0, "MM " + str(read_avgMm()) + "      ", Color.BLACK, Color.WHITE)
# 停車
left_motor.brake()
right_motor.brake()
# 嗶一聲
ev3.speaker.beep()
# 停下來後再顯示一次距離，看停得準不準
ev3.screen.draw_text(0, 0, "MM " + str(read_avgMm()) + "      ", Color.BLACK, Color.WHITE)
