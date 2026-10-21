# 循行者 16 路 範例 5：平順循線，遇到十字路口就停車
# 接線：循行者 16 路 → C 埠；左輪馬達 → E 埠；右輪馬達 → F 埠
from pybricks.hubs import PrimeHub
from pybricks.parameters import Direction, Port
from pybricks.pupdevices import Motor
from pybricks.robotics import DriveBase
from pybricks.tools import wait
from MBC_line16_obj_Lib import MBC_LINE16

hub = PrimeHub()
line16 = MBC_LINE16(3)                                 # 3 = C 埠
left = Motor(Port.E, Direction.COUNTERCLOCKWISE)
right = Motor(Port.F)
robot = DriveBase(left, right, wheel_diameter=56, axle_track=112)

while True:
    if line16.width() >= 11:      # 11 路以上同時踩到黑線 → 十字路口（整排 16 路幾乎全黑）
        robot.stop()
        hub.speaker.beep(880, 500)
        break                     # 跳出迴圈，程式結束
    pos = line16.pos100()         # 更精細的位置：-100 ~ 100
    robot.drive(120, pos * 1.5)   # 偏多少就轉多少：偏越多轉越快
    wait(10)
