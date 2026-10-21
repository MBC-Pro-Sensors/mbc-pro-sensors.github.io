# ============================================================
# TOF8 八方位雷射測距感應器 EV3 新手範例 01：量量看正前方有多遠
# 軟體：Pybricks v4（用 Chrome 打開 https://code.pybricks.com ，EV3 版目前是 Beta）
# 接法：TOF8 八方位雷射插 1 號孔
# 這個範例會做什麼：八方位雷射的模式 0 會給你正前方（中間兩顆）的平均距離，最簡單好用。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port, Color
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = LUMPDevice(Port.S1)       # TOF8 八方位雷射插 1 號孔


def read_front():
    # 正前方（第 4、5 顆平均）距離：0 ~ 100（1 格＝1.2 公分，100＝前面沒東西）
    return sensor.read(0)[0]


# ---------- 程式從這裡開始 ----------
# 清除螢幕
ev3.screen.clear()
# 一直重複
while True:
    # 第 1 行顯示正前方距離（0~100）
    ev3.screen.draw_text(0, 0, "FRONT " + str(read_front()) + "      ", Color.BLACK, Color.WHITE)
    # 第 2 行換算成公分：× 1.2
    ev3.screen.draw_text(0, 16, "CM " + str(read_front() * 1.2) + "      ", Color.BLACK, Color.WHITE)
    # 等 0.1 秒
    wait(100)
