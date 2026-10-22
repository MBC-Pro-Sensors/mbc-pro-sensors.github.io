# 神攝手（學校版）範例 6（進階）：色塊面積：往色塊開過去，色塊在畫面裡變得夠大（夠近）就停
# 接線：神攝手 → C 埠（裝在車頭）；左輪馬達 → E 埠；右輪馬達 → F 埠。先在感應器上設定 2:TRACK 的顏色
# 只有 Pybricks 版：面積是感應器韌體 2026-10 以後才有的數字，官方 App 和積木網站的積木讀不到
from pybricks.hubs import PrimeHub
from pybricks.parameters import Color, Direction, Port
from pybricks.tools import wait
from MBC_cam_school_Lib import MBC_CAM_SCHOOL
from pybricks.pupdevices import Motor
from pybricks.robotics import DriveBase

hub = PrimeHub()
cam = MBC_CAM_SCHOOL(Port.C)
left = Motor(Port.E, Direction.COUNTERCLOCKWISE)
right = Motor(Port.F)
robot = DriveBase(left, right, wheel_diameter=56, axle_track=112)

while True:
    area = cam.area()                 # 色塊面積 = 寬 × 高（0~7500）；沒找到是 0。越近越大
    x = cam.blob_x()                  # 色塊左右位置 0~98，50 = 中間
    if area == 0:                     # 沒看到色塊 → 停
        robot.stop()
        hub.light.on(Color.RED)
    elif area >= 1500:                # 夠大了 = 夠近了 → 停、綠燈
        robot.stop()
        hub.light.on(Color.GREEN)
    else:                             # 還小 = 還遠 → 往色塊開過去，偏左偏右順便修正
        robot.drive(80, (x - 50) * 2)
        hub.light.on(Color.YELLOW)
    print('面積', area, ' X', x)
    wait(50)
