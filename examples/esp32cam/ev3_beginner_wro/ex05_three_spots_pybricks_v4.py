# ============================================================
# 神攝手 ESP32CAM 視覺感應器（WRO 2026 版 HUE12） EV3 新手範例 05：左中右三個色點：紅色在哪邊？
# 軟體：Pybricks v4（用 Chrome 打開 https://code.pybricks.com ，EV3 版目前是 Beta）
# 接法：神攝手（WRO 版）插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：設定 S1（左）、S2（中）、S3（右）三個色點，看紅色出現在哪一個，車子就往那邊轉。
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


def read_hue3():
    # 色點 S3 的顏色碼：0 ~ 239 彩色（紅 0/239、黃 40、綠 80、青 120、藍 160、紫 200）；240~245 白；246~250 黑；255＝色點沒開
    return slot_byte(4)


def read_hue2():
    # 色點 S2 的顏色碼：0 ~ 239 彩色（紅 0/239、黃 40、綠 80、青 120、藍 160、紫 200）；240~245 白；246~250 黑；255＝色點沒開
    return slot_byte(2)


# ---------- 程式從這裡開始 ----------
# 一直重複
while True:
    # 如果 S1 是紅色
    if read_hue1() < 15 or (read_hue1() > 225 and read_hue1() < 240):
        # 紅色在左邊（S1）：往左轉
        left_motor.dc(-15)
        right_motor.dc(15)
    # 否則，如果 S3 是紅色
    elif read_hue3() < 15 or (read_hue3() > 225 and read_hue3() < 240):
        # 紅色在右邊（S3）：往右轉
        left_motor.dc(15)
        right_motor.dc(-15)
    # 否則，如果 S2 是紅色
    elif read_hue2() < 15 or (read_hue2() > 225 and read_hue2() < 240):
        # 紅色在中間（S2）：往前開
        left_motor.dc(25)
        right_motor.dc(25)
    else:
        # 沒看到紅色：停住
        left_motor.brake()
        right_motor.brake()
