# ============================================================
# 神攝手 ESP32CAM 視覺感應器（WRO 2026 版 HUE12） EV3 新手範例 01：色點 S1 看到什麼顏色？
# 軟體：Pybricks v4（用 Chrome 打開 https://code.pybricks.com ，EV3 版目前是 Beta）
# 接法：神攝手（WRO 版）插 1 號孔
# 這個範例會做什麼：WRO 版有 12 個色點，每個色點回報「那一個位置現在的顏色」。先設定 S1，把它的顏色碼和鮮豔度顯示在螢幕上。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port, Color
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = LUMPDevice(Port.S1)       # 神攝手（WRO 版）插 1 號孔


def slot_byte(n):
    # 神攝手 WRO 版每 3 個 byte 打包成一個數字，這裡把第 n 個 byte 拆出來
    data = sensor.read(0)
    return (data[n // 3] >> (8 * (n % 3))) & 255


def read_hue1():
    # 色點 S1 的顏色碼：0 ~ 239 彩色（紅 0/239、黃 40、綠 80、青 120、藍 160、紫 200）；240~245 白；246~250 黑；255＝色點沒開
    return slot_byte(0)


def read_sta1():
    # 色點 S1 的鮮豔度：0 ~ 255（黑色固定 1）
    return slot_byte(1)


# ---------- 程式從這裡開始 ----------
# 清除螢幕
ev3.screen.clear()
# 一直重複
while True:
    # 第 1 行：S1 的顏色碼
    ev3.screen.draw_text(0, 0, "S1 " + str(read_hue1()) + "      ", Color.BLACK, Color.WHITE)
    # 第 2 行：S1 的鮮豔度
    ev3.screen.draw_text(0, 16, "T1 " + str(read_sta1()) + "      ", Color.BLACK, Color.WHITE)
    # 等 0.1 秒
    wait(100)
