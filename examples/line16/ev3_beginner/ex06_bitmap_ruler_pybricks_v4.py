# ============================================================
# 循行者 16 路循線感應器 EV3 新手範例 06：壓線位元圖：在螢幕上畫一把 LED 尺
# 軟體：Pybricks v4（用 Chrome 打開 https://code.pybricks.com ，EV3 版目前是 Beta）
# 接法：循行者 16 路插 1 號孔
# 這個範例會做什麼：模式 3 送出「壓線位元圖」：16 個位元，每一路壓到線就是 1。這個範例把它畫成一把尺（# ＝壓到線、. ＝沒壓到），跟感應器上的 LED 一樣，一眼就看出線在哪幾路、有沒有分岔。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port, Color
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = LUMPDevice(Port.S1)       # 循行者 16 路插 1 號孔


def read_bitmap():
    # 壓線位元圖：0 ~ 65535：bit0＝第 1 路（最左）… bit15＝第 16 路，1＝那一路壓到線
    return sensor.read(3)[0] & 0xFFFF


# ---------- 程式從這裡開始 ----------
# 清除螢幕
ev3.screen.clear()
# 一直重複
while True:
    # 第 1 行畫尺：16 格，最左是第 1 路，# ＝壓到線、. ＝沒壓到
    bits_value = read_bitmap()
    bits_text = ""
    for b in range(16):
        if (bits_value // (2 ** b)) % 2 == 1:   # 第 b 個位元是 1
            bits_text = bits_text + "#"
        else:
            bits_text = bits_text + "."
    ev3.screen.draw_text(0, 0, "L " + bits_text + "  ", Color.BLACK, Color.WHITE)
    # 第 2 行顯示位元圖的數字（第 1 路＝1、第 2 路＝2、第 3 路＝4…相加）
    ev3.screen.draw_text(0, 16, "BITS " + str(read_bitmap()) + "      ", Color.BLACK, Color.WHITE)
    # 等 0.1 秒
    wait(100)
