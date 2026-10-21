# ============================================================
# IMU 陀螺儀感應器 EV3 新手範例 04：走直線自動修正
# 軟體：Pybricks v4（用 Chrome 打開 https://code.pybricks.com ，EV3 版目前是 Beta）
# 接法：IMU 陀螺儀插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：兩個輪子不可能完全一樣快，車子直走久了會歪。用陀螺儀看歪了幾度，就往反方向補回來，車子就能走得很直。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor, GyroSensor
from pybricks.parameters import Port
from pybricks.tools import wait

ev3 = EV3Brick()
gyro = GyroSensor(Port.S1)        # IMU 陀螺儀插 1 號孔
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def read_angle():
    # 機器人轉了幾度：連續累加（轉一圈＝360，可超過 360）
    return gyro.angle()


# ---------- 程式從這裡開始 ----------
# 角度歸零：現在的方向就是「正前方」
gyro.reset_angle(0)
# 一直重複
while True:
    # 修正力量 fix ＝ 角度 × 2
    fix = read_angle() * 2
    # 往右歪（角度變大）：左輪減速、右輪加速，把車頭拉回來
    left_motor.dc(30 - fix)
    right_motor.dc(30 + fix)
