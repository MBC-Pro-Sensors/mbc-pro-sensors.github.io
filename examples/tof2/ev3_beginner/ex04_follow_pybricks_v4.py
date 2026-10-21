# ============================================================
# TOF2 雙眼雷射測距感應器 EV3 新手範例 04：跟車：保持固定距離
# 軟體：Pybricks v4（用 Chrome 打開 https://code.pybricks.com ，EV3 版目前是 Beta）
# 接法：TOF2 雙眼雷射插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：拿一本書放在車子前面，往前移車子就跟上、往後推車子就退後，一直保持約 30 公分。用的是「差多少就補多少」的比例控制。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = LUMPDevice(Port.S1)       # TOF2 雙眼雷射插 1 號孔
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def read_dist():
    # 左右兩眼的平均距離：0 ~ 100（1 格＝1.2 公分，100＝前面沒東西）
    return sensor.read(0)[0]


# ---------- 程式從這裡開始 ----------
# 一直重複
while True:
    # 馬達力量 power ＝（距離 − 25）× 2：太遠是正數往前、太近是負數往後
    power = (read_dist() - 25) * 2
    # 如果 power 大於 50
    if power > 50:
        # 最快只用 50，比較安全
        power = 50
    # 兩輪用 power 一起前進或後退
    left_motor.dc(power)
    right_motor.dc(power)
