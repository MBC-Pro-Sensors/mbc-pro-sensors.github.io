# ============================================================
# 神攝手 ESP32CAM 視覺感應器（學校老師創意賽版） EV3 新手範例 01：相機看到了什麼？8 個數字一次看
# 軟體：Pybricks v4（用 Chrome 打開 https://code.pybricks.com ，EV3 版目前是 Beta）
# 接法：神攝手（學校版）插 1 號孔
# 這個範例會做什麼：模式 1 一次送出 8 個數字：顏色、鮮豔度、亮度、動態、色塊的 X、Y、寬、高。全部顯示在 EV3 螢幕上，對照相機畫面就能看懂每個數字的意思。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port, Color
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = LUMPDevice(Port.S1)       # 神攝手（學校版）插 1 號孔


def read_hue1():
    # 顏色（色相）：0 ~ 100，-1＝黑白灰
    return sensor.read(1)[0]


def read_sta():
    # 顏色鮮豔度：0 ~ 100
    return sensor.read(1)[1]


def read_val():
    # 亮度：0 ~ 100
    return sensor.read(1)[2]


def read_motion():
    # 畫面動了多少：0 ~ 100
    return sensor.read(1)[3]


def read_x():
    # 色塊左右位置：0 ~ 100（左 → 右），沒找到＝0
    return sensor.read(1)[4]


def read_y():
    # 色塊上下位置：0 ~ 75（下 → 上），沒找到＝0
    return sensor.read(1)[5]


def read_w():
    # 色塊寬度：0 ~ 100，0＝沒找到
    return sensor.read(1)[6]


def read_h():
    # 色塊高度（越近越大）：0 ~ 75，0＝沒找到
    return sensor.read(1)[7]


# ---------- 程式從這裡開始 ----------
# 清除螢幕
ev3.screen.clear()
# 一直重複
while True:
    # 顏色：紅 0 或 100、黃約 17、綠約 33、藍約 67，-1＝黑白灰
    ev3.screen.draw_text(0, 0, "HUE " + str(read_hue1()) + "      ", Color.BLACK, Color.WHITE)
    # 鮮豔度 0~100
    ev3.screen.draw_text(0, 16, "STA " + str(read_sta()) + "      ", Color.BLACK, Color.WHITE)
    # 亮度 0~100
    ev3.screen.draw_text(0, 32, "VAL " + str(read_val()) + "      ", Color.BLACK, Color.WHITE)
    # 畫面動了多少 0~100
    ev3.screen.draw_text(0, 48, "MOTION " + str(read_motion()) + "      ", Color.BLACK, Color.WHITE)
    # 色塊左右位置 0~100
    ev3.screen.draw_text(0, 64, "X " + str(read_x()) + "      ", Color.BLACK, Color.WHITE)
    # 色塊上下位置 0~75
    ev3.screen.draw_text(0, 80, "Y " + str(read_y()) + "      ", Color.BLACK, Color.WHITE)
    # 色塊寬度，0＝沒找到
    ev3.screen.draw_text(0, 96, "W " + str(read_w()) + "      ", Color.BLACK, Color.WHITE)
    # 色塊高度，越近越大
    ev3.screen.draw_text(0, 112, "H " + str(read_h()) + "      ", Color.BLACK, Color.WHITE)
    # 等 0.1 秒
    wait(100)
