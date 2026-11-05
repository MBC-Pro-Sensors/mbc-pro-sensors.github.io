# 範例 02：馬達進階與編碼器 (Motor Advanced & Encoder)
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
print(" 範例 02：馬達進階與編碼器測試")
print("==============================")

exp = MBC_EXP6(port.F)
time.sleep(1)

# ---------------------------------------------------------
# API 展示：reset_angle(port) 與 get_motor_angle(port)
# ---------------------------------------------------------
print("\n[展示 1] reset_angle() 與 get_motor_angle() - 角度歸零與讀取")
exp.reset_angle(1)
print("已將 1 號馬達角度歸零，目前角度:", exp.get_motor_angle(1))

# ---------------------------------------------------------
# API 展示：motor_run_degrees(port, speed, degrees, stop)
# ---------------------------------------------------------
print("\n[展示 2] motor_run_degrees() - 轉動相對角度")
print("1 號馬達以速度 60 轉動 360 度，到位後鎖死 (stop=3)...")
exp.motor_run_degrees(1, 60, 360, 3)

# 非同步監控轉動過程
time.sleep(0.2)
while abs(exp.get_motor_speed(1)) > 0:
    print("轉動中... 角度:", exp.get_motor_angle(1))
    time.sleep(0.1)
print("轉動完成！最終角度:", exp.get_motor_angle(1))

# ---------------------------------------------------------
# API 展示：motor_run_target(port, speed, target_angle, stop)
# ---------------------------------------------------------
print("\n[展示 3] motor_run_target() - 轉到絕對目標角度")
print("1 號馬達走最短路徑轉到絕對位置 90 度，到位後鎖死 (stop=3)...")
exp.motor_run_target(1, 40, 90, 3)

time.sleep(0.2)
while abs(exp.get_motor_speed(1)) > 0:
    time.sleep(0.1)
print("到達目標！目前角度:", exp.get_motor_angle(1))

# ---------------------------------------------------------
# API 展示：get_motor_abs_angle(port) 與 get_motor_speed(port)
# ---------------------------------------------------------
print("\n[展示 4] get_motor_abs_angle() 與 get_motor_speed()")
print("不管您剛剛怎麼轉，絕對編碼器只會顯示 0~359 度的刻度：")
print("1 號馬達絕對編碼器讀數:", exp.get_motor_abs_angle(1))
print("1 號馬達靜止狀態下的速度:", exp.get_motor_speed(1))

# ---------------------------------------------------------
# API 展示：motor_set_pid(port, p, i, d)
# ---------------------------------------------------------
print("\n[展示 5] motor_set_pid() - 自訂 PID 參數")
print("設定 1 號馬達的高強度 PID (P=400, I=1200, D=5)")
# 這些參數會影響馬達抗干擾與煞車的表現，請依實際負載微調
exp.motor_set_pid(1, 400, 1200, 5)

exp.stop_all(1)
print("\n測試結束！")
