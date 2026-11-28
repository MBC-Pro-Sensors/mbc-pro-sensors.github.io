# 神攝手 × Pybricks（SPIKE）範例 11：調目標範圍與形狀（target_range E5、shape_tolerance E6、target_shape E7）
# SharpShot × Pybricks (SPIKE) example 11: tune a target (target_range E5, shape_tolerance E6, target_shape E7)
# 相機模式：51 物體偵測 ・① 物體清單 / mode 51 Object detection, object list
# 相機韌體 2026-10-29 以後；目標 1 要先教過（範例 10 或相機選單）
# camera firmware 2026-10-29 or later; teach target 1 first (example 10 or the camera menu)
# 接線 Wiring：相機 → C 孔 / camera → port C
# 同一個資料夾要有 MBC_UniCam.py / MBC_UniCam.py must be imported too
#
# cam.target_range(目標, hue=1~119, sat=0~31, size=4~254, search=0/1/2)：色相、飽和容許範圍、最小面積；
#     255＝不改；search 1＝鎖住上次那塊、2＝每幀選最大塊、0＝不改
# cam.shape_tolerance(1~5)：形狀判斷的寬鬆度（3＝出廠，越大越寬鬆）/ shape tolerance (3 = factory, bigger = looser)
# cam.target_shape(目標 1~8, 形狀 0~9)：這個目標只算這個形狀（0＝不指定、1 圓、3 三角、4 正方…）
# 回 0 好了、2 失敗（超出範圍或目標沒教過）；三個都存進相機，斷電後還在
# 0 done, 2 failed (out of range or target not taught); all three are stored in the camera
from pybricks.hubs import PrimeHub
from pybricks.parameters import Port
from pybricks.tools import wait
from MBC_UniCam import UniCam, shape_name

hub = PrimeHub()
cam = UniCam(Port.C, mode=51)

r = [cam.target_range(1, hue=20, sat=10, size=30, search=2),   # 👉 可以改：色相 ±20、飽和 10、最小面積 30、每幀選最大
     cam.shape_tolerance(4),                                    # 👉 可以改：比出廠寬鬆一點 / a bit looser
     cam.target_shape(1, 4)]                                    # 👉 可以改：目標 1 只算正方形 / only squares
print('target_range, shape_tolerance, target_shape →', r)
if r != [0, 0, 0]:
    print('有設定沒成功：2＝超出範圍或目標 1 沒教過 / a setting failed: 2 = out of range or target 1 not taught')

WANT = 1 * 10 + 4                                # 目標 1 的正方形＝代碼 14 / target 1 square = code 14
while True:
    hub.display.number(cam.object_count() or 0)  # 畫面裡幾個物體 / objects in view
    for i in range(1, cam.max_object() + 1):
        if cam.object(i) == WANT:
            print('找到 found', shape_name(4, zh=True), shape_name(4), '在 at', cam.object_xy(i))
            break
    wait(200)
