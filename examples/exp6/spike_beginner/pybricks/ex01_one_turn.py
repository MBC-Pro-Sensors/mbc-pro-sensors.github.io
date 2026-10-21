# EXP6 範例 1：一顆馬達轉一圈再轉回來（Pybricks）
# 接線：EXP6 的 Host 線 → 主機 C 孔；馬達 → EXP6 擴充孔 1；EXP6 接上電池
from pybricks.hubs import PrimeHub
from MBC_exp6_obj_Lib import MBC_EXP6

hub = PrimeHub()
exp6 = MBC_EXP6(3, False)    # 3 = C 孔（1=A、2=B、3=C、4=D、5=E、6=F）；False = 一般寫法

while exp6.get_device_id(1) != 5:        # 等擴充孔 1 的馬達準備好（5 = 馬達，開機大約 1 秒）
    exp6.keep_alive_wait(100)

exp6.motor_run_degrees(1, 50, 360, 2)    # 擴充孔 1：速度 50%，轉 360 度（一圈），到了就煞車（2）
exp6.keep_alive_wait(1500)               # 等它轉完。等待一律用 keep_alive_wait，不要用 wait
hub.display.number(exp6.get_motor_angle(1))   # 螢幕顯示現在的角度（大約 360）
exp6.keep_alive_wait(2000)

exp6.motor_run_degrees(1, 50, -360, 2)   # 角度寫負的 = 反方向轉一圈，回到原來的位置
exp6.keep_alive_wait(1500)
hub.display.number(exp6.get_motor_angle(1))   # 大約 0
exp6.keep_alive_wait(2000)

exp6.stop_all(1)             # 最後全部停止（1 = 滑行）
