# 測距者 8 路 範例 4：避障。一直往前走；正前方（第 4、5 路）小於 30 公分就停下，往最空曠的那一邊轉 45 度
# 接線：測距者 8 路 → C 埠（裝在車頭）；左輪馬達 → E 埠；右輪馬達 → F 埠
from pybricks.parameters import Direction, Port
from pybricks.pupdevices import Motor
from pybricks.robotics import DriveBase
from pybricks.tools import wait
from MBC_laser8_obj_Lib import MBC_LASER8

laser8 = MBC_LASER8(3)                                 # 3 = C 埠
left = Motor(Port.E, Direction.COUNTERCLOCKWISE)
right = Motor(Port.F)
robot = DriveBase(left, right, wheel_diameter=56, axle_track=112)

while True:
    if laser8.ch(4) < 30 or laser8.ch(5) < 30:   # 第 4、5 路都朝正前方，任何一個小於 30 公分 → 前面有東西
        robot.stop()
        if laser8.max_direction() < 0:           # 最空曠的方向在左邊（負的）
            robot.turn(-45)                      # 原地左轉 45 度
        else:
            robot.turn(45)                       # 原地右轉 45 度
    else:
        robot.drive(150, 0)                      # 每秒 150 mm 往前走
    wait(50)
