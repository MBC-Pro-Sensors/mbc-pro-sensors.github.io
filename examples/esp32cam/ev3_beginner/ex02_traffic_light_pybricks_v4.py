# ============================================================
# 神攝手 ESP32CAM 視覺感應器（學校老師創意賽版） EV3 新手範例 02：紅綠燈車：看到綠色才走
# 軟體：Pybricks v4（用 Chrome 打開 https://code.pybricks.com ，EV3 版目前是 Beta）
# 接法：神攝手（學校版）插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：用模式 0 只讀「顏色」。看到綠色就往前開，其他顏色（紅色）就停。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = LUMPDevice(Port.S1)       # 神攝手（學校版）插 1 號孔
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def read_hue():
    # 顏色（色相）：0 ~ 100（紅 0 或 100、黃約 17、綠約 33、藍約 67），-1＝黑白灰
    return sensor.read(0)[0]


# ---------- 程式從這裡開始 ----------
# 一直重複
while True:
    # 如果顏色在 25~45 之間（綠色）
    if read_hue() >= 25 and read_hue() <= 45:
        # 是綠色：往前開
        left_motor.dc(30)
        right_motor.dc(30)
    else:
        # 不是綠色：停車
        left_motor.brake()
        right_motor.brake()
