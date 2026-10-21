# EXP6 範例 2：三顆馬達一起轉（官方 SPIKE App・Python）
# 接線：EXP6 的 Host 線 → 主機 C 孔；馬達 → EXP6 擴充孔 1、2、3；EXP6 接上電池
from hub import port, sound
import time
from MBC_exp6_SPIKE_App_Lib import MBC_EXP6

exp6 = MBC_EXP6(port.C)
time.sleep(2)                # 等馬達準備好

exp6.motor_run(1, 30)        # 擴充孔 1：速度 30%（慢）
exp6.motor_run(2, 50)        # 擴充孔 2：速度 50%
exp6.motor_run(3, 70)        # 擴充孔 3：速度 70%（快）
time.sleep(3)                # 一起轉 3 秒

exp6.motor_run(1, -30)       # 速度寫負的 = 反轉
exp6.motor_run(2, -50)
exp6.motor_run(3, -70)
time.sleep(3)

exp6.stop_all(2)             # 全部停止（2 = 煞車，馬上停）
sound.beep(880, 200)
