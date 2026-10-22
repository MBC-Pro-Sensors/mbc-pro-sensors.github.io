# 測距者 2 路 範例 6（進階）：用毫米精準停車（離牆 20 公分放慢、15 公分停）
# 接線：測距者 2 路 → C 埠（裝在車頭朝前）；左輪馬達 → E 埠；右輪馬達 → F 埠
# 只有 Pybricks 版：毫米距離放在感應器的「模式 6」，官方 App 和積木網站讀不到；感應器韌體要 2026-10 以後
from pybricks.hubs import PrimeHub
from pybricks.parameters import Direction, Port
from pybricks.pupdevices import Motor
from pybricks.robotics import DriveBase
from pybricks.tools import wait
from MBC_laser2_obj_Lib import MBC_LASER2

hub = PrimeHub()
laser2 = MBC_LASER2(3)                                 # 3 = C 埠
left = Motor(Port.E, Direction.COUNTERCLOCKWISE)
right = Motor(Port.F)
robot = DriveBase(left, right, wheel_diameter=56, axle_track=112)

robot.drive(100, 0)                                    # 每秒 100 mm 往前開
while laser2.average_mm() > 200:                       # 左右平均距離（毫米）還大於 200 mm = 20 公分
    wait(10)
robot.drive(40, 0)                                     # 剩 20 公分：放慢到每秒 40 mm
while laser2.average_mm() > 150:                       # 到 150 mm = 15 公分
    wait(10)
robot.stop()                                           # 停車
hub.speaker.beep()
mm = laser2.average_mm()                               # 停下來之後再量一次
hub.display.number(mm // 10)                           # 螢幕顯示公分（15 或 14）
print('停在', mm, 'mm')
