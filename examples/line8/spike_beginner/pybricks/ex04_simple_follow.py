# 循行者 8 路 範例 4：最簡單的循線（線在左就左轉、線在右就右轉、中間就直走）
# 接線：循行者 8 路 → C 埠；左輪馬達 → E 埠；右輪馬達 → F 埠
from pybricks.parameters import Direction, Port
from pybricks.pupdevices import Motor
from pybricks.robotics import DriveBase
from pybricks.tools import wait
from MBC_line8_obj_Lib import MBC_LINE8

line8 = MBC_LINE8(3)                                   # 3 = C 埠
left = Motor(Port.E, Direction.COUNTERCLOCKWISE)       # 左輪：反過來裝，所以設成逆時針才是往前
right = Motor(Port.F)                                  # 右輪
robot = DriveBase(left, right, wheel_diameter=56, axle_track=112)   # 輪子直徑 56 mm、兩輪距離 112 mm

while True:
    pos = line8.pos8()
    if pos < -1:                  # 線在左邊 → 一邊前進一邊左轉
        robot.drive(100, -60)     # 速度 100 mm/秒，每秒左轉 60 度
    elif pos > 1:                 # 線在右邊 → 右轉
        robot.drive(100, 60)
    else:                         # 線在中間 → 直走
        robot.drive(100, 0)
    wait(10)
