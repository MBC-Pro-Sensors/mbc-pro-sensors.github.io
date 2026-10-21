# 神攝手 WRO 版（HUE12）範例 4：依顏色轉向（紅色右轉、綠色左轉、其他停）
# 接線：HUE12 → C 埠；左輪馬達 → E 埠；右輪馬達 → F 埠
from pybricks.parameters import Direction, Port
from pybricks.pupdevices import Motor
from pybricks.robotics import DriveBase
from pybricks.tools import wait
from MBC_hue12_Lib import HUE12Sensor

cam = HUE12Sensor(Port.C)
left = Motor(Port.E, Direction.COUNTERCLOCKWISE)       # 左輪：反過來裝，所以設成逆時針才是往前
right = Motor(Port.F)
robot = DriveBase(left, right, wheel_diameter=56, axle_track=112)

while True:
    cam.update()
    name, hue, sat = cam.get_slot(1)
    if hue < 10 or 230 <= hue < 240:      # 紅色 → 原地右轉（每秒 60 度）
        robot.drive(0, 60)
    elif 60 <= hue <= 110:                # 綠色 → 原地左轉
        robot.drive(0, -60)
    else:                                 # 其他顏色、白、黑、沒設定 → 停
        robot.stop()
    wait(50)
