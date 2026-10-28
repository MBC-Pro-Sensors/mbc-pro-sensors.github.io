#!/usr/bin/env pybricks-micropython
# 神攝手 × EV3 範例 1：跟著色塊轉
# Sharpshooter Vision × EV3 example 1: turn to follow a colour blob
# 需要把相機選單切到 相機模式 1 顏色追蹤 ・子選項 ③ 標準（XY＋面積）＝模式編號 13
# Camera menu: camera mode 1 Colour tracking, option 3 Standard (XY + area) = mode number 13
# 軟體 Software：Pybricks v4（code.pybricks.com）或 樂高官方 EV3 MicroPython v2（同一支程式都能跑）
# 準備：先在相機上教「目標 1」的顏色 / teach "target 1" on the camera first
# 接線 Wiring：相機 → 1 號孔 / camera → port 1；左輪 B、右輪 C / left wheel B, right wheel C
# 同一個資料夾要有 MBC_UniCam_EV3.py / put MBC_UniCam_EV3.py in the same folder
from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.parameters import Port
from pybricks.robotics import DriveBase
from pybricks.tools import wait
from MBC_UniCam_EV3 import UniCam

ev3 = EV3Brick()
cam = UniCam(Port.S1)                            # 不填 mode：自動讀相機目前的模式 / mode read from the camera
left = Motor(Port.B)
right = Motor(Port.C)
robot = DriveBase(left, right, 56, 114)          # 輪徑 56 mm、輪距 114 mm / wheel 56 mm, axle 114 mm

if cam.mode_number() != 13:                      # EV3 讀得到相機的模式，切錯了就提醒 / EV3 can check the mode
    print('請把相機切到 模式 13（顏色追蹤 ③）/ please set the camera to mode 13')
    ev3.speaker.beep(200, 500)
    wait(3000)
    raise SystemExit

KP = 3            # 👉 可以改：偏 1 像素就多轉幾度／秒 / turn rate per pixel of error
STOP_AREA = 20    # 👉 可以改：面積 ≥ 20 % 就算很近 / area (%) that counts as "close"

while True:
    p = cam.xy(1)                                # 色 1 的 (X, Y) 像素（EV3 是 8×6 格，±10）/ pixels (±10 on EV3)
    if p is None:                                # 讀不到相機 / no camera
        robot.stop()
    elif p[0] == 0:                              # 沒看到：原地慢慢轉著找 / not seen: spin to search
        robot.drive(0, 60)
    else:
        error = p[0] - 80                        # 畫面中間 X = 80；色塊在右 → 正 / right of centre = positive
        speed = 0 if cam.area(1) >= STOP_AREA else 80
        robot.drive(speed, error * KP)           # 面積在同一個模式（模式 1），不用多等 / same EV3 mode, no wait
    wait(20)
