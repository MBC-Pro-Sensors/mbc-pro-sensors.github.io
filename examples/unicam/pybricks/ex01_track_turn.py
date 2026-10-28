# 神攝手通用版 v1 × Pybricks（SPIKE）範例 1：跟著色塊轉
# Sharpshooter Vision Universal v1 × Pybricks (SPIKE) example 1: turn to follow a colour blob
# 需要把相機選單切到 相機模式 1 顏色追蹤 ・子選項 ③ 標準（XY＋面積）＝模式編號 13
# Camera menu: camera mode 1 Colour tracking, option 3 Standard (XY + area) = mode number 13
# 準備：先在相機上教「目標 1」的顏色 / Teach "target 1" on the camera first
# 接線 Wiring：相機 → C 孔 / camera → port C；左輪 E、右輪 F / left wheel E, right wheel F
# 同一個資料夾要有 MBC_UniCam.py / MBC_UniCam.py must be imported too
from pybricks.hubs import PrimeHub
from pybricks.pupdevices import Motor
from pybricks.parameters import Port, Direction, Color
from pybricks.robotics import DriveBase
from pybricks.tools import wait
from MBC_UniCam import UniCam

hub = PrimeHub()
cam = UniCam(Port.C, mode=13)                    # 模式編號要跟相機選單一樣 / must match the camera menu
left = Motor(Port.E, Direction.COUNTERCLOCKWISE)
right = Motor(Port.F)
robot = DriveBase(left, right, 56, 112)          # 輪徑 56 mm、輪距 112 mm / wheel 56 mm, axle 112 mm

KP = 3            # 👉 可以改：偏 1 像素就多轉幾度／秒 / you can change: turn rate per pixel of error
STOP_AREA = 20    # 👉 可以改：面積 ≥ 20 % 就算很近 / area (%) that counts as "close"

while True:
    p = cam.xy(1)                                # 色 1 的 (X, Y) 像素；(0, 0) = 沒看到 / (0, 0) = not seen
    if p is None:                                # 從來沒讀到相機 / camera never answered
        robot.stop()
        hub.light.on(Color.RED)
    elif p[0] == 0:                              # 沒看到色 1：原地慢慢轉著找 / not seen: spin slowly to search
        robot.drive(0, 60)
        hub.light.on(Color.ORANGE)
    else:
        error = p[0] - 80                        # 畫面中間 X = 80；色塊在右 → 正 / centre is X = 80; right = positive
        if cam.area(1) >= STOP_AREA:             # 很近了：只轉不前進 / close: turn only
            robot.drive(0, error * KP)
        else:                                    # 還遠：邊前進邊轉 / far: drive and turn
            robot.drive(80, error * KP)
        hub.light.on(Color.GREEN)
    wait(20)
