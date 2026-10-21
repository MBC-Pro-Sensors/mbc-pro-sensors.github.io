# 測距者 8 路 範例 5：跟著手走。手在 25~60 公分：一邊往前走一邊轉向手；15~25 公分：停；太近：後退
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
    dist = laser8.min_distance()            # 手（最近的東西）有多遠
    direction = laser8.min_direction()      # 手在哪個方向：-7 ~ 7
    if dist > 60:                           # 看不到手 → 停
        robot.stop()
    elif dist < 15:                         # 太近了 → 每秒 80 mm 往後退
        robot.drive(-80, 0)
    elif dist > 25:                         # 有點遠 → 往前走，手偏多少就轉多少
        robot.drive(100, direction * 15)    # 方向 × 15 = 每秒轉幾度（左負右正）
    else:                                   # 15 ~ 25 公分：剛剛好 → 停
        robot.stop()
    wait(50)
