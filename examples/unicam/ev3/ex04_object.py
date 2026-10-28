#!/usr/bin/env pybricks-micropython
# 神攝手 × EV3 範例 4：跟著正方形走、螢幕顯示它轉了幾度
# Sharpshooter Vision × EV3 example 4: drive to a square and show its angle on the screen
# 需要把相機選單切到 相機模式 5 物體偵測 ・子選項 ② 物體詳細＝模式編號 52
# Camera menu: camera mode 5 Object detection, option 2 Object detail = mode number 52
# 軟體 Software：Pybricks v4（code.pybricks.com）或 樂高官方 EV3 MicroPython v2（同一支程式都能跑）
# 準備：在相機上教「目標 1」的顏色（例如藍色方塊）/ teach "target 1" on the camera (e.g. a blue block)
# 接線 Wiring：相機 → 1 號孔 / camera → port 1；左輪 B、右輪 C / left wheel B, right wheel C
# 同一個資料夾要有 MBC_UniCam_EV3.py / put MBC_UniCam_EV3.py in the same folder
#
# 物體代碼＝色號×10＋形狀：目標 1 的正方形＝1×10＋4＝14。EV3 模式 52 給 2 個物體（面積大的是物體 1），
# 代碼、位置、面積、角度都在 EV3 模式 1，不用換模式（不會多等 0.5 秒）。
# Object code = colour × 10 + shape: target 1 square = 14. EV3 mode 52 gives 2 objects (object 1 = largest);
# code, position, area and angle are all in EV3 mode 1, so there is no 0.5 s mode switch.
from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.parameters import Port
from pybricks.robotics import DriveBase
from pybricks.tools import wait
from MBC_UniCam_EV3 import UniCam, shape_name

ev3 = EV3Brick()
cam = UniCam(Port.S1)                            # 不填 mode：自動讀相機目前的模式 / mode read from the camera
left = Motor(Port.B)
right = Motor(Port.C)
robot = DriveBase(left, right, 56, 114)          # 輪徑 56 mm、輪距 114 mm / wheel 56 mm, axle 114 mm

if cam.mode_number() != 52:                      # EV3 讀得到相機的模式，切錯了就提醒 / EV3 can check the mode
    print('請把相機切到 模式 52（物體偵測 ② 物體詳細）/ please set the camera to mode 52')
    ev3.speaker.beep(200, 500)
    wait(3000)
    raise SystemExit

WANT = 1 * 10 + 4   # 👉 可以改：目標 1 × 10 ＋ 形狀 4（正方）；三角改成 13、圓改成 11 / triangle 13, circle 11
KP = 3              # 👉 可以改：偏 1 像素就多轉幾度／秒 / turn rate per pixel of error
STOP_AREA = 25      # 👉 可以改：面積 ≥ 25 % 就算到了 / area (%) that counts as "arrived"

print('要找 looking for: 目標 target', WANT // 10, shape_name(WANT % 10, True), shape_name(WANT % 10))
while True:
    c1 = cam.object(1)                           # 物體 1（最大）的代碼 / code of object 1 (largest)
    if c1 is None:                               # 讀不到相機 / no camera
        robot.stop()
        wait(100)
        continue
    if c1 == WANT:                               # 物體 1、2 裡哪一個是要找的（0＝都不是）/ which one we want
        i = 1
    elif cam.object(2) == WANT:
        i = 2
    else:
        i = 0
    if i == 0:                                 # 沒看到：原地慢慢轉著找 / not seen: spin to search
        robot.drive(0, 60)
        wait(20)
        continue
    x, y = cam.object_xy(i)                      # 像素（EV3 是 8×6 格中心，±10）/ pixels (±10 on EV3)
    angle = cam.object_angle(i)                  # 0~179 度（EV3 精度 ±1）/ degrees (±1 on EV3)
    ev3.screen.clear()
    ev3.screen.print(shape_name(WANT % 10), angle)
    if cam.object_area(i) >= STOP_AREA:          # 到了：停車、嗶一聲 / arrived: stop and beep
        robot.stop()
        ev3.speaker.beep()
        break
    robot.drive(80, (x - 80) * KP)               # 畫面中間 X = 80；在右 → 正 → 右轉 / right of centre = positive
    wait(20)
