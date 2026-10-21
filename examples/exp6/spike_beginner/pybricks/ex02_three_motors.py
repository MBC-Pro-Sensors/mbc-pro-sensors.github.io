# EXP6 範例 2：三顆馬達一起轉（Pybricks）
# 接線：EXP6 的 Host 線 → 主機 C 孔；馬達 → EXP6 擴充孔 1、2、3；EXP6 接上電池
from pybricks.hubs import PrimeHub
from MBC_exp6_obj_Lib import MBC_EXP6

hub = PrimeHub()
exp6 = MBC_EXP6(3, False)    # 3 = C 孔

for m in (1, 2, 3):          # 三顆馬達都要準備好（5 = 馬達）
    while exp6.get_device_id(m) != 5:
        exp6.keep_alive_wait(100)

exp6.motor_run(1, 30)        # 擴充孔 1：速度 30%（慢）
exp6.motor_run(2, 50)        # 擴充孔 2：速度 50%
exp6.motor_run(3, 70)        # 擴充孔 3：速度 70%（快）
exp6.keep_alive_wait(3000)   # 一起轉 3 秒

exp6.motor_run(1, -30)       # 速度寫負的 = 反轉
exp6.motor_run(2, -50)
exp6.motor_run(3, -70)
exp6.keep_alive_wait(3000)

exp6.stop_all(2)             # 全部停止（2 = 煞車，馬上停）
hub.speaker.beep(880, 200)
