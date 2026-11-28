# 神攝手 × Pybricks（SPIKE）範例 7：程式裡切相機模式（set_mode，命令 E0）——循線到路口，改追色找目標
# SharpShot × Pybricks (SPIKE) example 7: switch the camera mode from the program (set_mode, command E0)
#   follow the line to a junction, then switch to colour tracking and turn to target 1
# 相機模式：一開始 41 循線黑線，路口後程式切成 13 顏色追蹤標準（不用動相機選單）
# Camera mode: starts at 41 (black line); after the junction the program switches to 13 (colour tracking, standard)
# 準備：目標 1 先教好（相機選單或範例 10）；相機韌體 2026-10-29 以後（出貨版 v1.0-261105 有命令）
# Before: teach target 1 (camera menu or example 10); camera firmware 2026-10-29 or later
# 接線 Wiring：相機 → C 孔；左輪 E、右輪 F / camera → port C; left wheel E, right wheel F
# 同一個資料夾要有 MBC_UniCam.py / MBC_UniCam.py must be imported too
# set_mode 會存進相機：斷電重開還是最後切的模式 / stored in the camera: it keeps the last mode after power-off
from pybricks.hubs import PrimeHub
from pybricks.pupdevices import Motor
from pybricks.parameters import Port, Direction
from pybricks.robotics import DriveBase
from pybricks.tools import wait
from MBC_UniCam import UniCam

hub = PrimeHub()
cam = UniCam(Port.C, mode=41)                    # 建立時自動切到 41 / switched to 41 when created
left = Motor(Port.E, Direction.COUNTERCLOCKWISE)
right = Motor(Port.F)
robot = DriveBase(left, right, 56, 112)

SPEED = 120        # 👉 可以改：循線速度 mm/s / line speed
KP = 1.5           # 👉 可以改：循線比例 / line gain
TURN_KP = 2        # 👉 可以改：追色時偏 1 像素轉幾度／秒 / turn rate per pixel

# ── 第 1 段：循線到十字或 T 字路口 / part 1: follow the line to a cross or T junction ──
while True:
    v = cam.line_all()
    if v is None:                                # 讀不到相機：停車 / no camera: stop
        robot.stop()
        wait(50)
        continue
    near, flags, junction = v[0], v[5], v[6]
    if junction in (1, 2):
        robot.stop()
        break
    robot.drive(SPEED if flags else 0, KP * near if flags else 120)
    wait(10)

# ── 切模式：回 0＝切好（函式庫已多等一幀，下一次讀到的就是新模式）/ switch: 0 = done ──
r = cam.set_mode(13)                             # 13＝相機模式 1 顏色追蹤 ・③ 標準 / colour tracking, standard
print('set_mode(13) →', r, ' 模式 mode', cam.mode_number())
if r != 0:                                       # 2 失敗、3 拒絕、None 舊韌體或沒接 / failed, rejected, old firmware
    print('切模式失敗，請在相機選單切到 13 / switch failed: use the camera menu')
    raise SystemExit

# ── 第 2 段：原地轉，把目標 1 轉到畫面中間就停 / part 2: turn until target 1 is in the middle ──
while True:
    p = cam.xy(1)
    if p is None or p == (0, 0):                 # 還沒看到：原地慢慢轉 / not seen yet: turn slowly
        robot.drive(0, 60)
    elif abs(p[0] - 80) < 15:                    # 在中間了：停車嗶一聲 / centred: stop and beep
        robot.stop()
        hub.speaker.beep()
        break
    else:
        robot.drive(0, TURN_KP * (p[0] - 80))   # 在右是正的 → 往右轉 / right = positive → turn right
    wait(10)
