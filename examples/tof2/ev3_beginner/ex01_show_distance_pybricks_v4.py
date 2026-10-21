# ============================================================
# TOF2 雙眼雷射測距感應器 EV3 新手範例 01：量量看前面有多遠
# 軟體：Pybricks v4（用 Chrome 打開 https://code.pybricks.com ，EV3 版目前是 Beta）
# 接法：TOF2 雙眼雷射插 1 號孔
# 這個範例會做什麼：用手在感應器前面前後移動，EV3 螢幕會顯示距離。這個範例同時顯示「感應器原始數字」和「換算成公分」。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port, Color
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = LUMPDevice(Port.S1)       # TOF2 雙眼雷射插 1 號孔


def read_dist():
    # 左右兩眼的平均距離：0 ~ 100（1 格＝1.2 公分，100＝前面沒東西）
    return sensor.read(0)[0]


# ---------- 程式從這裡開始 ----------
# 先清除螢幕
ev3.screen.clear()
# 一直重複
while True:
    # 第 1 行顯示感應器讀到的原始數字（0~100）
    ev3.screen.draw_text(0, 0, "RAW " + str(read_dist()) + "      ", Color.BLACK, Color.WHITE)
    # 第 2 行顯示換算後的公分：數字 × 1.2
    ev3.screen.draw_text(0, 16, "CM " + str(read_dist() * 1.2) + "      ", Color.BLACK, Color.WHITE)
    # 等 0.1 秒
    wait(100)
