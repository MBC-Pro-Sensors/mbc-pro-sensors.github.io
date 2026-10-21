# 神攝手（學校版）範例 4：轉頭追色塊（色塊在左就往左轉、在右就往右轉、在中間就停）
# 接線：神攝手 → C 埠；左輪馬達 → E 埠；右輪馬達 → F 埠
from pybricks.parameters import Direction, Port
from pybricks.pupdevices import Motor
from pybricks.robotics import DriveBase
from pybricks.tools import wait
from MBC_cam_school_Lib import MBC_CAM_SCHOOL

cam = MBC_CAM_SCHOOL(Port.C)
left = Motor(Port.E, Direction.COUNTERCLOCKWISE)       # 左輪：反過來裝，所以設成逆時針才是往前
right = Motor(Port.F)                                  # 右輪
robot = DriveBase(left, right, wheel_diameter=56, axle_track=112)   # 輪子直徑 56 mm、兩輪距離 112 mm

while True:
    x = cam.blob_x()
    if not cam.found():           # 沒看到色塊 → 停下來
        robot.stop()
    elif x < 40:                  # 色塊在左邊 → 原地左轉（速度 0，每秒轉 60 度）
        robot.drive(0, -60)
    elif x > 60:                  # 色塊在右邊 → 原地右轉
        robot.drive(0, 60)
    else:                         # 色塊在中間 → 已經對準，停
        robot.stop()
    wait(50)
