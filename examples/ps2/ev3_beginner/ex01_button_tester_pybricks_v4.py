# ============================================================
# 掌控者 PS2 2.4G 遙控接收器 EV3 新手範例 01：按鍵測試器：看看每個按鍵的數字
# 軟體：Pybricks v4（用 Chrome 打開 https://code.pybricks.com ，EV3 版目前是 Beta）
# 接法：掌控者 PS2插 1 號孔
# 這個範例會做什麼：模式 1 一次送出 8 個數字：4 組按鍵和 4 個搖桿值。這個範例把它們全部顯示在螢幕上，按按看、推推看，就知道每個按鍵是多少。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port, Color
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = LUMPDevice(Port.S1)       # 掌控者 PS2插 1 號孔


def read_dpad1():
    # 方向鍵（上 1、右 2、下 4、左 8）：0 ~ 15
    return sensor.read(1)[0]


def read_shape():
    # 形狀鍵（▲1、●2、✕4、■8）：0 ~ 15
    return sensor.read(1)[2]


def read_shoulder():
    # 肩鍵（L2 1、R2 2、L1 4、R1 8）：0 ~ 15
    return sensor.read(1)[4]


def read_func():
    # 功能鍵（SELECT 1、L3 2、R3 4、START 8）：0 ~ 15
    return sensor.read(1)[6]


def read_lx():
    # 左搖桿左右：0 ~ 99，50＝中間，往右變大
    return sensor.read(1)[1]


def read_ly():
    # 左搖桿上下：0 ~ 99，50＝中間，往上變大
    return sensor.read(1)[3]


def read_rx():
    # 右搖桿左右：0 ~ 99，50＝中間，往右變大
    return sensor.read(1)[5]


def read_ry():
    # 右搖桿上下：0 ~ 99，50＝中間，往上變大
    return sensor.read(1)[7]


# ---------- 程式從這裡開始 ----------
# 清除螢幕
ev3.screen.clear()
# 一直重複
while True:
    # 方向鍵：上 1、右 2、下 4、左 8
    ev3.screen.draw_text(0, 0, "DPAD " + str(read_dpad1()) + "      ", Color.BLACK, Color.WHITE)
    # 形狀鍵：▲1、●2、✕4、■8
    ev3.screen.draw_text(0, 16, "SHAPE " + str(read_shape()) + "      ", Color.BLACK, Color.WHITE)
    # 肩鍵：L2 1、R2 2、L1 4、R1 8
    ev3.screen.draw_text(0, 32, "SHLD " + str(read_shoulder()) + "      ", Color.BLACK, Color.WHITE)
    # 功能鍵：SELECT 1、L3 2、R3 4、START 8
    ev3.screen.draw_text(0, 48, "FUNC " + str(read_func()) + "      ", Color.BLACK, Color.WHITE)
    # 左搖桿左右（50＝中間）
    ev3.screen.draw_text(0, 64, "LX " + str(read_lx()) + "      ", Color.BLACK, Color.WHITE)
    # 左搖桿上下（50＝中間）
    ev3.screen.draw_text(0, 80, "LY " + str(read_ly()) + "      ", Color.BLACK, Color.WHITE)
    # 右搖桿左右
    ev3.screen.draw_text(0, 96, "RX " + str(read_rx()) + "      ", Color.BLACK, Color.WHITE)
    # 右搖桿上下
    ev3.screen.draw_text(0, 112, "RY " + str(read_ry()) + "      ", Color.BLACK, Color.WHITE)
    # 等 0.1 秒
    wait(100)
