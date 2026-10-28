# 神攝手通用版 v1 × Pybricks（SPIKE）範例 5：PD 循線＋路口停車
# Sharpshooter Vision Universal v1 × Pybricks (SPIKE) example 5: PD line following + stop at a junction
# 需要把相機選單切到 相機模式 4 循線 ・子選項 ① 黑線 ＝模式編號 41（白線 42、色線 43 寫法一樣，改 mode）
# Camera menu: camera mode 4 Line, option 1 Black line = mode number 41 (42 white / 43 colour line: change mode)
# 準備：相機裝在車頭、斜斜往前下方看線 / mount the camera at the front, looking down and forward
# 接線 Wiring：相機 → C 孔；左輪 E、右輪 F / camera → port C; left wheel E, right wheel F
# 方向：站在車後往前看，線在左＝負 / standing behind the robot: line on the left = negative
from pybricks.hubs import PrimeHub
from pybricks.pupdevices import Motor
from pybricks.parameters import Port, Direction
from pybricks.robotics import DriveBase
from pybricks.tools import wait
from MBC_UniCam import UniCam

hub = PrimeHub()
cam = UniCam(Port.C, mode=41)
left = Motor(Port.E, Direction.COUNTERCLOCKWISE)
right = Motor(Port.F)
robot = DriveBase(left, right, 56, 112)

SPEED = 120       # 👉 可以改：前進速度 mm/s / forward speed
KP = 1.5          # 👉 可以改：偏多少就轉多少 / proportional gain
KD = 4.0          # 👉 可以改：偏移變快時多修一點 / derivative gain
last_error = 0

while True:
    v = cam.line_all()                           # 14 個值一次讀 / all 14 line values at once
    if v is None:                                # 讀不到相機：停車 / no camera: stop
        robot.stop()
        wait(50)
        continue
    near, far, flags, junction, lost = v[0], v[2], v[5], v[6], v[13]
    if junction in (1, 2):                       # 1 十字、2 T 字路口：停車嗶一聲 / cross or T junction: stop
        robot.stop()
        hub.speaker.beep()
        break
    if flags == 0:                               # 近中遠都沒看到線：往最後離線那一邊找 / line lost: search
        robot.drive(0, -120 if lost == 1 else 120)
        wait(10)
        continue
    error = 0.7 * near + 0.3 * far               # 近處為主、遠處提早轉彎 / mostly near, a bit of far
    turn = KP * error + KD * (error - last_error)
    last_error = error
    robot.drive(SPEED, turn)                     # 線在左（負）→ 往左轉 / line on the left → turn left
    wait(10)
