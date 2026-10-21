# EXP6 範例 1：一顆馬達轉一圈再轉回來（官方 SPIKE App・Python）
# 接線：EXP6 的 Host 線 → 主機 C 孔；馬達 → EXP6 擴充孔 1；EXP6 接上電池
# 第一次用：先執行一次 install_exp6_lib.llsp3，把函式庫裝進主機
from hub import port, light_matrix
import time
from MBC_exp6_SPIKE_App_Lib import MBC_EXP6

exp6 = MBC_EXP6(port.C)      # EXP6 接在主機 C 孔
time.sleep(2)                # 等 2 秒，讓擴充孔上的馬達準備好（太早下指令會被忽略）

exp6.motor_run_degrees(1, 50, 360, 2)    # 擴充孔 1：速度 50%，轉 360 度（一圈），到了就煞車（2）
time.sleep(1.5)                          # 等它轉完（還沒轉完就送下一個轉角度指令，會被丟掉）
light_matrix.write(str(exp6.get_motor_angle(1)))   # 螢幕顯示現在的角度（大約 360）
time.sleep(2)

exp6.motor_run_degrees(1, 50, -360, 2)   # 角度寫負的 = 反方向轉一圈，回到原來的位置
time.sleep(1.5)
light_matrix.write(str(exp6.get_motor_angle(1)))   # 大約 0
time.sleep(2)

exp6.stop_all(1)             # 最後一定要全部停止（1 = 滑行）
