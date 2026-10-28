# 神攝手 × Pybricks（SPIKE）範例 6：找最大的紅色三角形、轉過去
# Sharpshooter Vision × Pybricks (SPIKE) example 6: find the biggest red triangle and drive to it
# 需要把相機選單切到 相機模式 5 物體偵測 ・子選項 ① 物體清單＝模式編號 51
# Camera menu: camera mode 5 Object detection, option 1 Object list = mode number 51
# 準備：在相機上把「目標 1」教成紅色（形狀可以不指定）/ teach "target 1" as red on the camera
# 接線 Wiring：相機 → C 孔 / camera → port C；左輪 E、右輪 F / left wheel E, right wheel F
# 同一個資料夾要有 MBC_UniCam.py / MBC_UniCam.py must be imported too
#
# 物體代碼＝色號×10＋形狀：目標 1 的三角形＝1×10＋3＝13。物體 1、2、3、4 是面積由大到小，
# 所以從物體 1 往下找，第一個代碼是 13 的就是「最大的紅色三角形」。
# Object code = colour × 10 + shape: target 1 triangle = 13. Objects 1~4 are sorted largest first,
# so the first object with code 13 is the biggest red triangle.
from pybricks.hubs import PrimeHub
from pybricks.pupdevices import Motor
from pybricks.parameters import Port, Direction, Color
from pybricks.robotics import DriveBase
from pybricks.tools import wait
from MBC_UniCam import UniCam, shape_name

hub = PrimeHub()
cam = UniCam(Port.C, mode=51)                    # 模式編號要跟相機選單一樣 / must match the camera menu
left = Motor(Port.E, Direction.COUNTERCLOCKWISE)
right = Motor(Port.F)
robot = DriveBase(left, right, 56, 112)          # 輪徑 56 mm、輪距 112 mm / wheel 56 mm, axle 112 mm

WANT = 1 * 10 + 3  # 👉 可以改：目標 1 × 10 ＋ 形狀 3（三角）；正方形改成 14、圓改成 11 / square 14, circle 11
KP = 3             # 👉 可以改：偏 1 像素就多轉幾度／秒 / turn rate per pixel of error
NEAR_Y = 100       # 👉 可以改：物體的 Y ≥ 100（畫面最下面）就算到了 / Y at the bottom of the picture = arrived


def find(want):
    # 從最大的物體往下找代碼＝want 的，回傳它的 (X, Y)；找不到 (0, 0) / largest first; (0, 0) = not found
    for i in range(1, cam.max_object() + 1):     # SPIKE 模式 51 給 4 個物體 / 4 objects on SPIKE
        if cam.object(i) == want:
            return cam.object_xy(i)
    return (0, 0)


print('要找 looking for: 目標 target', WANT // 10, shape_name(WANT % 10, True), shape_name(WANT % 10))
while True:
    n = cam.object_count()                       # 畫面裡有幾個物體 0~8 / objects in view
    if n is None:                                # 從來沒讀到相機 / camera never answered
        robot.stop()
        hub.light.on(Color.RED)
        wait(100)
        continue
    hub.display.number(n)
    x, y = find(WANT)
    if x == 0:                                   # 沒看到紅色三角形：原地慢慢轉著找 / not seen: spin to search
        robot.drive(0, 60)
        hub.light.on(Color.ORANGE)
    elif y >= NEAR_Y:                            # 到了：停車、嗶一聲 / arrived: stop and beep
        robot.stop()
        hub.light.on(Color.GREEN)
        hub.speaker.beep()
        break
    else:
        error = x - 80                           # 畫面中間 X = 80；在右 → 正 → 右轉 / right of centre = positive
        robot.drive(80, error * KP)
        hub.light.on(Color.BLUE)
    wait(20)
