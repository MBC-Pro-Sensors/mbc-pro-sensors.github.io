# 神攝手 × Pybricks（SPIKE）範例 13：工作模式 power()（命令 EB）——不用相機時待機省電，要用再叫醒
# SharpShot × Pybricks (SPIKE) example 13: power() (command EB): standby while the camera is not needed
# 相機模式：13 顏色追蹤 ・③ 標準 / mode 13 Colour tracking, standard
# 相機韌體 2026-10-29 以後 / camera firmware 2026-10-29 or later
# 接線 Wiring：相機 → C 孔；左輪 E、右輪 F / camera → port C; left wheel E, right wheel F
# 同一個資料夾要有 MBC_UniCam.py / MBC_UniCam.py must be imported too
#
# cam.power(0)＝全速 full、1＝省電 save（25 fps）、2＝待機 standby（鏡頭關、補光燈關，讀到的都是「沒有」）
# 待機 → 全速要等約 1～2.5 秒才有畫面；不存：相機重開機、SPIKE 斷線、按相機任何鍵都回全速
# standby -> full takes about 1~2.5 s; not stored: a restart, unplugging or any camera key returns to full speed
from pybricks.hubs import PrimeHub
from pybricks.pupdevices import Motor
from pybricks.parameters import Port, Direction
from pybricks.robotics import DriveBase
from pybricks.tools import wait
from MBC_UniCam import UniCam

hub = PrimeHub()
cam = UniCam(Port.C, mode=13)
left = Motor(Port.E, Direction.COUNTERCLOCKWISE)
right = Motor(Port.F)
robot = DriveBase(left, right, 56, 112)

print('power(2) 待機 standby →', cam.power(2))
robot.drive(150, 0)                              # 👉 可以改：這段不用相機，先直走 2 秒 / no camera needed here
wait(2000)
robot.stop()
print('power(0) 全速 full →', cam.power(0))
wait(2500)                                       # 等鏡頭開好（約 1～2.5 秒）/ wait for the camera to start

for i in range(20):                              # 醒來後看目標 1 / look for target 1 again
    p = cam.xy(1)
    print('目標 1 target 1', p)
    if p and p != (0, 0):
        hub.speaker.beep()
        break
    wait(100)
