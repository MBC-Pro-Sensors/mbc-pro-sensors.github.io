# ============================================================
# 神攝手 ESP32CAM 視覺感應器（WRO 2026 版 HUE12） EV3 新手範例 03：交通號誌柱：紅停綠走
# 軟體：Pybricks v4（用 Chrome 打開 https://code.pybricks.com ，EV3 版目前是 Beta）
# 接法：神攝手（WRO 版）插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：WRO 未來工程師常見的紅綠柱子：S1 對準前方，看到紅色就停車、看到綠色就往前開。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = LUMPDevice(Port.S1)       # 神攝手（WRO 版）插 1 號孔
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def slot_byte(n):
    # 神攝手 WRO 版每 3 個 byte 打包成一個數字，這裡把第 n 個 byte 拆出來
    data = sensor.read(0)
    return (data[n // 3] >> (8 * (n % 3))) & 255


def read_hue1():
    # 色點 S1 的顏色碼：0 ~ 239 彩色（紅 0/239、黃 40、綠 80、青 120、藍 160、紫 200）；240~245 白；246~250 黑；255＝色點沒開
    return slot_byte(0)


# ---------- 程式從這裡開始 ----------
# 一直重複
while True:
    # 把 S1 的顏色碼存進 hue
    hue = read_hue1()
    # 如果是綠色（60~100）
    if hue >= 60 and hue <= 100:
        # 綠色：往前開
        left_motor.dc(30)
        right_motor.dc(30)
    # 否則，如果是紅色
    elif hue < 15 or (hue > 225 and hue < 240):
        # 紅色：停車
        left_motor.brake()
        right_motor.brake()
