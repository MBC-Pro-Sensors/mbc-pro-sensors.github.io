# EXP6 範例 4：用主機按鈕控制馬達（官方 SPIKE App・Python）
# 按住左鍵 → 正轉；按住右鍵 → 反轉；放開 → 停；左右鍵一起按 → 結束
# 接線：EXP6 的 Host 線 → 主機 C 孔；馬達 → EXP6 擴充孔 1；EXP6 接上電池
from hub import port, button
import time
from MBC_exp6_SPIKE_App_Lib import MBC_EXP6

exp6 = MBC_EXP6(port.C)
time.sleep(2)

while not (button.pressed(button.LEFT) and button.pressed(button.RIGHT)):   # 左右鍵一起按住 → 結束
    if button.pressed(button.LEFT):          # 按住左鍵
        exp6.motor_run(1, 50)                # 正轉，速度 50%
    elif button.pressed(button.RIGHT):       # 按住右鍵
        exp6.motor_run(1, -50)               # 反轉
    else:                                    # 都沒按
        exp6.motor_stop(1, 2)                # 停止（2 = 煞車）
    time.sleep(0.05)

exp6.stop_all(1)             # 結束前一定要全部停止
