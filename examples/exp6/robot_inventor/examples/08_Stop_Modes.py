# 範例 08：stop 參數四種狀態完整比較 (Stop Modes Comparison)
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
print(" 範例 08：stop 參數四種狀態比較")
print("==============================")
print("請將馬達接在擴充板 1 號孔")

exp = MBC_EXP6(port.F)
time.sleep(1)

PORT = 1

# ---------------------------------------------------------
# API 展示：motor_run_degrees(port, speed, degrees, stop) - 4 種 stop 狀態比較
# stop：1=coast 2=brake 3=hold 4=continue
# ---------------------------------------------------------
print("\n[展示 1] motor_run_degrees() - 轉動 90 度後，四種 stop 狀態的差異")

print("\n(a) stop=1(coast)：到位後滑行，會因慣性多滑一點才真正停下")
exp.reset_angle(PORT)
exp.motor_run_degrees(PORT, 60, 90, 1)
time.sleep(0.2)
while abs(exp.get_motor_speed(PORT)) > 0:
    time.sleep(0.05)
time.sleep(0.3)
print("停止後角度: {}".format(exp.get_motor_angle(PORT)))

print("\n(b) stop=2(brake)：到位後被動煞車，比滑行更快停住")
exp.reset_angle(PORT)
exp.motor_run_degrees(PORT, 60, 90, 2)
time.sleep(0.2)
while abs(exp.get_motor_speed(PORT)) > 0:
    time.sleep(0.05)
time.sleep(0.3)
print("停止後角度: {}".format(exp.get_motor_angle(PORT)))

print("\n(c) stop=3(hold)：到位後主動鎖死，外力推它會自動頂回目標角度")
exp.reset_angle(PORT)
exp.motor_run_degrees(PORT, 60, 90, 3)
time.sleep(0.5)
print("試著用手轉動馬達，應該會感覺到它主動抵抗、頂回原位...")
time.sleep(2)
print("鎖死中的角度: {}".format(exp.get_motor_angle(PORT)))

print("\n(d) stop=4(continue)：到位後『不停』，直接以原速度繼續轉動")
exp.reset_angle(PORT)
exp.motor_run_degrees(PORT, 60, 90, 4)
time.sleep(0.5)
print("到達 90 度後速度應該還在: {}（沒有變 0，代表真的沒停）".format(exp.get_motor_speed(PORT)))
time.sleep(1)

# continue 會讓馬達持續轉動，示範完務必手動停止，否則會一直轉下去
print("示範結束，手動用 motor_stop(stop=2) 停下馬達...")
exp.motor_stop(PORT, 2)
time.sleep(1)

# ---------------------------------------------------------
# API 展示：motor_track_target(port, speed, angle, stop) 的 continue 效果
# ---------------------------------------------------------
print("\n[展示 2] motor_track_target() - 絕對角度目標也支援 continue")
exp.motor_track_target(PORT, 50, 0, 3)
time.sleep(1)
print("先歸位到絕對角度 0 度並鎖死，目前角度: {}".format(exp.get_motor_abs_angle(PORT)))

print("走到絕對角度 180 度，到位後 continue 續轉...")
exp.motor_track_target(PORT, 50, 180, 4)
time.sleep(1)
print("到位後速度應該還在: {}（沒有變 0）".format(exp.get_motor_speed(PORT)))
time.sleep(1)
exp.motor_stop(PORT, 2)
print("已手動煞車停止。")
time.sleep(1)

# ---------------------------------------------------------
# API 展示：motor_stop(port, stop=4) - 對「純停止指令」而言，continue 代表
# 「維持現狀、不送任何指令」，不是 fallback 成 brake，也不是走角度那種續轉。
# ---------------------------------------------------------
print("\n[展示 3] motor_stop() 傳入 stop=4(continue) - 維持馬達現狀，不送任何指令")
print("馬達以速度 60 持續轉動中...")
exp.motor_run(PORT, 60)
time.sleep(1)
print("呼叫 motor_stop(PORT, 4)（不會印出任何提示，也不會送出封包）...")
exp.motor_stop(PORT, 4)
time.sleep(1)
print("馬達應該還在轉，目前速度: {}（沒有被停下來）".format(exp.get_motor_speed(PORT)))
exp.motor_stop(PORT, 2)
print("示範結束，手動用 stop=2(brake) 停下馬達。")
time.sleep(1)

# ---------------------------------------------------------
# API 展示：drive_stop(left_port, right_port, stop) - 跟 motor_stop 共用同一套 4 種狀態
# ---------------------------------------------------------
print("\n[展示 4] drive_stop() - 雙馬達版的 stop 參數")
print("請將另一顆馬達接在擴充板 2 號孔，測試雙馬達同步停止")
LEFT, RIGHT = PORT, 2

print("\n(a) drive_stop(..., stop=1)：兩輪一起滑行")
exp.drive(LEFT, RIGHT, 50, 50)
time.sleep(1)
exp.drive_stop(LEFT, RIGHT, 1)
time.sleep(1)

print("\n(b) drive_stop(..., stop=3)：兩輪協同鎖死，車體維持朝向")
exp.drive(LEFT, RIGHT, 50, 50)
time.sleep(1)
exp.drive_stop(LEFT, RIGHT, 3)
print("試著扳動任一輪，另一輪應該會協同抵抗，不會讓車體被轉走方向...")
time.sleep(2)

print("\n(c) drive_stop(..., stop=4)：維持現狀，不送任何指令")
exp.drive(LEFT, RIGHT, 50, 50)
time.sleep(1)
exp.drive_stop(LEFT, RIGHT, 4)
time.sleep(1)
print("兩輪應該還在轉，左輪速度: {}（沒有被停下來）".format(exp.get_motor_speed(LEFT)))
exp.drive_stop(LEFT, RIGHT, 1)
print("示範結束，手動滑行停止。")

print("\n全部 stop 模式測試結束！")
