# ============================================================
# IMU 陀螺儀感應器 EV3 新手範例 02：原地右轉 90 度
# 軟體：Pybricks v4（用 Chrome 打開 https://code.pybricks.com ，EV3 版目前是 Beta）
# 接法：IMU 陀螺儀插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：靠陀螺儀轉彎，比「轉幾秒」準很多：一直轉，直到角度到 90 度才停。
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
# 角度歸零
gyro.reset_angle(0)
# 左輪往前、右輪往後：原地向右轉（慢一點比較準）
left_motor.dc(20)
right_motor.dc(-20)
# 等到角度的絕對值大於等於 90
while not (abs(read_angle()) >= 90):
    wait(10)
# 轉到 90 度：停車
left_motor.brake()
right_motor.brake()
# 嗶一聲
ev3.speaker.beep()
