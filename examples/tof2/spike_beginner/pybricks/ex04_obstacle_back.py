# 測距者 2 路 範例 4：一直往前走，前面有東西（小於 20 公分）就停車、嗶一聲、後退、右轉
# 接線：測距者 2 路 → C 埠（裝在車頭、朝前）；左輪馬達 → E 埠；右輪馬達 → F 埠
from pybricks.hubs import PrimeHub
from pybricks.parameters import Direction, Port
from pybricks.pupdevices import Motor
from pybricks.robotics import DriveBase
from pybricks.tools import wait
from MBC_laser2_obj_Lib import MBC_LASER2

hub = PrimeHub()
laser2 = MBC_LASER2(3)                                 # 3 = C 埠
left = Motor(Port.E, Direction.COUNTERCLOCKWISE)       # 左輪：反過來裝，所以設成逆時針才是往前
right = Motor(Port.F)                                  # 右輪
robot = DriveBase(left, right, wheel_diameter=56, axle_track=112)   # 輪子直徑 56 mm、兩輪距離 112 mm

while True:
    if laser2.min_distance() < 20:    # 左右任何一邊小於 20 公分 → 前面有東西
        robot.stop()                  # 先停車
        hub.speaker.beep(880, 200)
        robot.straight(-100)          # 後退 100 mm（10 公分）
        robot.turn(90)                # 原地右轉 90 度
    else:
        robot.drive(150, 0)           # 每秒 150 mm 直直往前走
    wait(50)
