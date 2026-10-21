# 神攝手 WRO 版（HUE12）範例 5：看前面物塊的顏色，決定從哪一邊繞過
#   紅色 → 從右邊繞、綠色 → 從左邊繞、沒看到 → 直走
# 接線：HUE12 → C 埠（第 1 格對準車子正前方）；左輪馬達 → E 埠；右輪馬達 → F 埠
from pybricks.hubs import PrimeHub
from pybricks.parameters import Direction, Port
from pybricks.pupdevices import Motor
from pybricks.robotics import DriveBase
from pybricks.tools import wait
from MBC_hue12_Lib import HUE12Sensor

hub = PrimeHub()
cam = HUE12Sensor(Port.C)
left = Motor(Port.E, Direction.COUNTERCLOCKWISE)
right = Motor(Port.F)
robot = DriveBase(left, right, wheel_diameter=56, axle_track=112)

while True:
    cam.update()
    name, hue, sat = cam.get_slot(1)
    if hue < 10 or 230 <= hue < 240:      # 紅色物塊 → 從右邊繞過
        hub.display.char("R")
        robot.turn(45)                    # 右轉 45 度（正數右轉）
        robot.straight(150)               # 直走 150 mm
        robot.turn(-45)                   # 左轉 45 度，車頭轉回原來的方向
    elif 60 <= hue <= 110:                # 綠色物塊 → 從左邊繞過
        hub.display.char("G")
        robot.turn(-45)
        robot.straight(150)
        robot.turn(45)
    else:                                 # 沒看到物塊 → 每秒 100 mm 往前
        robot.drive(100, 0)
    wait(20)
