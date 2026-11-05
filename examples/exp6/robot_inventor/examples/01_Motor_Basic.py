# 範例 01：馬達基礎控制 (Motor Basic Control)
# ℹ️ 版本提醒 (Version Reminder):
# [ZH] 本程式適用於 LEGO MINDSTORMS Robot Inventor App 10.x（玩具版 51515）
# [EN] This program is designed for LEGO MINDSTORMS Robot Inventor App 10.x (51515)
#
# ⚠️ 測試提醒 (Testing Reminder):
# [ZH] 執行此範例前，請確保已經在主機中執行過 `Install_MBC_exp6_Lib.py` 安裝函數庫。
# [EN] Before running this example, please ensure you have executed `Install_MBC_exp6_Lib.py` to install the library.

from hub import port
import time
# 玩具版：函式庫裝在主機的 /projects/mbc_exp6/（先執行一次 Install_MBC_exp6_Lib.py）
from projects.mbc_exp6.MBC_exp6_SPIKE_App_Lib import MBC_EXP6

print("==============================")
print(" 範例 01：馬達基礎控制測試")
print("==============================")

# 建立擴充板連線
exp = MBC_EXP6(port.F)
time.sleep(1)

# ---------------------------------------------------------
# API 展示：motor_run(port, speed)
# ---------------------------------------------------------
print("\n[展示 1] motor_run() - 以速度模式持續轉動")
print("1號孔馬達以速度 50 正轉...")
exp.motor_run(1, 50)
time.sleep(2)

print("1號孔馬達以速度 -80 反轉...")
exp.motor_run(1, -80)
time.sleep(2)

# ---------------------------------------------------------
# API 展示：motor_stop(port, stop) - coast/brake/hold 三種停止方式
# ---------------------------------------------------------
print("\n[展示 2] motor_stop() - stop 參數決定滑行/煞車/鎖死")
exp.motor_run(1, 100)
time.sleep(1)
print("stop=1 -> 馬達會滑行 (Coast) 一段距離才停")
exp.motor_stop(1, 1)
time.sleep(2)

exp.motor_run(1, 100)
time.sleep(1)
print("stop=2 -> 馬達會瞬間被動煞車 (Brake)")
exp.motor_stop(1, 2)
time.sleep(2)

# ---------------------------------------------------------
# API 展示：motor_power(port, power)
# ---------------------------------------------------------
print("\n[展示 3] motor_power() - 原始電力輸出 (無速度閉迴圈控制)")
print("這與 motor_run 不同，是不帶 PID 速度鎖定的純電壓輸出")
print("1號孔馬達以 40% 電力運轉...")
exp.motor_power(1, 40)
time.sleep(2)

# ---------------------------------------------------------
# API 展示：stop_all()
# ---------------------------------------------------------
print("\n[展示 4] stop_all() - 停止所有馬達")
print("讓 1, 2 號馬達同時轉動，然後一次全部停止")
exp.motor_run(1, 50)
exp.motor_run(2, 50)
time.sleep(2)

exp.stop_all(1)
print("所有馬達已停止。測試結束！")
