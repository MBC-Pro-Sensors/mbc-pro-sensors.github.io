# EXP6 範例 5：兩輪小車走正方形（Pybricks）
# 接線：EXP6 的 Host 線 → 主機 C 孔；左輪馬達 → 擴充孔 1；右輪馬達 → 擴充孔 2；EXP6 接上電池
# 小車：輪子直徑 56 mm、兩輪距離 112 mm（樂高標準小車）
from pybricks.hubs import PrimeHub
from MBC_exp6_obj_Lib import MBC_EXP6

hub = PrimeHub()
exp6 = MBC_EXP6(3, False)          # 3 = C 孔
exp6.set_motor_inverted(1, True)   # 左輪是反過來裝的：設成「反裝」，速度正的才會往前

for m in (1, 2):                   # 兩顆馬達都要準備好
    while exp6.get_device_id(m) != 5:
        exp6.keep_alive_wait(100)
exp6.keep_alive_wait(300)

for side in range(4):              # 正方形有 4 個邊
    exp6.reset_angle(2)                        # 右輪角度歸零，用來量走了多遠
    exp6.drive(1, 2, 40, 40)                   # 左輪孔 1、右輪孔 2，兩輪都 40% → 直走
    while exp6.get_motor_angle(2) < 720:       # 右輪轉滿 720 度（2 圈，大約 35 公分）
        exp6.keep_alive_wait(10)
    exp6.stop_all(2)                           # 煞車
    exp6.keep_alive_wait(300)

    exp6.reset_angle(2)
    exp6.drive(1, 2, 30, -30)                  # 左輪往前、右輪往後 → 原地右轉
    while exp6.get_motor_angle(2) > -180:      # 右輪倒轉 180 度 ≈ 車子轉 90 度
        exp6.keep_alive_wait(10)
    exp6.stop_all(2)
    exp6.keep_alive_wait(300)

exp6.stop_all(1)
