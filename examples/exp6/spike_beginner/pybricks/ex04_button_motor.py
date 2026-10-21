# EXP6 範例 4：用主機按鈕控制馬達（Pybricks）
# 按住左鍵 → 正轉；按住右鍵 → 反轉；放開 → 停；左右鍵一起按 → 結束
# 接線：EXP6 的 Host 線 → 主機 C 孔；馬達 → EXP6 擴充孔 1；EXP6 接上電池
from pybricks.hubs import PrimeHub
from pybricks.parameters import Button
from MBC_exp6_obj_Lib import MBC_EXP6

hub = PrimeHub()
exp6 = MBC_EXP6(3, False)    # 3 = C 孔

while exp6.get_device_id(1) != 5:        # 等擴充孔 1 的馬達準備好
    exp6.keep_alive_wait(100)

while not (Button.LEFT in hub.buttons.pressed() and Button.RIGHT in hub.buttons.pressed()):   # 左右鍵一起按住 → 結束
    if Button.LEFT in hub.buttons.pressed():         # 按住左鍵
        exp6.motor_run(1, 50)                        # 正轉，速度 50%
    elif Button.RIGHT in hub.buttons.pressed():      # 按住右鍵
        exp6.motor_run(1, -50)                       # 反轉
    else:                                            # 都沒按
        exp6.motor_stop(1, 2)                        # 停止（2 = 煞車）
    exp6.keep_alive_wait(50)

exp6.stop_all(1)
