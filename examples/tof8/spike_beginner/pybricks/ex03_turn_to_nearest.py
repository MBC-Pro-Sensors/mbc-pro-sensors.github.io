# 測距者 8 路 範例 3：原地轉頭，讓車頭朝向最近的東西（60 公分外的不理它）
# 接線：測距者 8 路 → C 埠（裝在車頭）；左輪馬達 → E 埠；右輪馬達 → F 埠
from pybricks.parameters import Direction, Port
from pybricks.pupdevices import Motor
from pybricks.robotics import DriveBase
from pybricks.tools import wait
from MBC_laser8_obj_Lib import MBC_LASER8

laser8 = MBC_LASER8(3)                                 # 3 = C 埠
left = Motor(Port.E, Direction.COUNTERCLOCKWISE)       # 左輪：反過來裝，所以設成逆時針才是往前
right = Motor(Port.F)                                  # 右輪
robot = DriveBase(left, right, wheel_diameter=56, axle_track=112)   # 輪子直徑 56 mm、兩輪距離 112 mm

while True:
    dist = laser8.min_distance()
    direction = laser8.min_direction()      # -7（最左）~ 0（正前方）~ 7（最右）
    if dist > 60:                           # 太遠或沒東西 → 停著等
        robot.stop()
    elif direction < -1:                    # 東西在左邊 → 原地左轉
        robot.drive(0, -90)                 # 不前進，每秒左轉 90 度
    elif direction > 1:                     # 東西在右邊 → 原地右轉
        robot.drive(0, 90)
    else:                                   # -1、0、1：已經在正前方 → 停
        robot.stop()
    wait(50)
