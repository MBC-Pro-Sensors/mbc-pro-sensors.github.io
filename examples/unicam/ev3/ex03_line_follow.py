#!/usr/bin/env pybricks-micropython
# 神攝手 × EV3 範例 3：PD 循線＋路口停車
# Sharpshooter Vision × EV3 example 3: PD line following + stop at a junction
# 需要把相機選單切到 相機模式 4 循線 ・子選項 ① 黑線 ＝模式編號 41（白線 42、色線 43 也可以）
# Camera menu: camera mode 4 Line, option 1 Black line = mode number 41 (42 / 43 also work)
# 軟體 Software：Pybricks v4 或 樂高官方 EV3 MicroPython v2
# 準備：相機裝在車頭、斜斜往前下方看線 / camera at the front, looking down and forward
# 接線 Wiring：相機 → 1 號孔；左輪 B、右輪 C / camera → port 1; left wheel B, right wheel C
# 方向：站在車後往前看，線在左＝負 / standing behind the robot: line on the left = negative
# EV3 每換一次模式約 0.5 秒：這支只用模式 1 的值（近、遠、旗標、路口）/ only mode-1 values (no mode switch)
from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.parameters import Port
from pybricks.robotics import DriveBase
from pybricks.tools import wait
from MBC_UniCam_EV3 import UniCam

ev3 = EV3Brick()
cam = UniCam(Port.S1)
left = Motor(Port.B)
right = Motor(Port.C)
robot = DriveBase(left, right, 56, 114)

if cam.mode_number() not in (41, 42, 43):        # 相機沒切到循線就提醒 / camera not in line mode
    print('請把相機切到 循線（模式 41／42／43）/ please set the camera to line mode')
    ev3.speaker.beep(200, 500)
    wait(3000)
    raise SystemExit

SPEED = 120       # 👉 可以改：前進速度 mm/s / forward speed
KP = 1.5          # 👉 可以改：比例 / proportional gain
KD = 4.0          # 👉 可以改：微分 / derivative gain
last_error = 0

while True:
    junction = cam.junction()                    # 0 無 1 十字 2 T 3 左 4 右 5 盡頭 / junction type
    if junction is None:                         # 讀不到相機：停車 / no camera: stop
        robot.stop()
        wait(50)
        continue
    if junction in (1, 2):                       # 十字或 T 字路口：停車嗶一聲 / cross or T: stop
        robot.stop()
        ev3.speaker.beep()
        break
    if cam.flags() == 0:                         # 近中遠都沒看到線：往上一次偏的那邊找 / line lost
        robot.drive(0, -120 if last_error < 0 else 120)
        wait(10)
        continue
    error = 0.7 * cam.near() + 0.3 * cam.far()   # 近處為主、遠處提早轉彎 / mostly near, a bit of far
    turn = KP * error + KD * (error - last_error)
    last_error = error
    robot.drive(SPEED, turn)                     # 線在左（負）→ 往左轉 / line on the left → turn left
    wait(10)
