# 測距者 2 路 範例 5：往前開到離牆 20 公分停下，再原地轉到兩邊距離一樣（車頭對正牆面）
# 接線：測距者 2 路 → C 埠（裝在車頭、朝前）；左輪馬達 → E 埠；右輪馬達 → F 埠
from pybricks.hubs import PrimeHub
from pybricks.parameters import Direction, Icon, Port
from pybricks.pupdevices import Motor
from pybricks.robotics import DriveBase
from pybricks.tools import wait
from MBC_laser2_obj_Lib import MBC_LASER2

hub = PrimeHub()
laser2 = MBC_LASER2(3)                                 # 3 = C 埠
left = Motor(Port.E, Direction.COUNTERCLOCKWISE)
right = Motor(Port.F)
robot = DriveBase(left, right, wheel_diameter=56, axle_track=112)

# 第 1 步：往前開，直到左右平均距離 ≤ 20 公分
robot.drive(100, 0)
while laser2.average() > 20:
    wait(20)
robot.stop()

# 第 2 步：原地轉，直到左右只差 1 公分以內
while abs(laser2.diff()) > 1:          # diff = 左 減 右
    if laser2.diff() < 0:              # 左邊比較近 → 車頭偏右了 → 往左轉
        robot.drive(0, -30)            # 速度 0（不前進），每秒左轉 30 度
    else:                              # 右邊比較近 → 往右轉
        robot.drive(0, 30)
    wait(20)
robot.stop()

# 第 3 步：完成！
hub.speaker.beep(880, 500)
hub.display.icon(Icon.HAPPY)
