# 範例 03：底盤運動控制 (Drive Base Control)
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
print(" 範例 03：底盤雙輪驅動測試")
print("==============================")
print("假設您的車子：左輪接在擴充板 1 號孔，右輪接在 2 號孔")

exp = MBC_EXP6(port.F)
time.sleep(1)

# 定義左右輪孔位
LEFT = 1
RIGHT = 2

# ---------------------------------------------------------
# API 展示：drive(left_port, right_port, left_speed, right_speed)
# ---------------------------------------------------------
print("\n[展示 1] drive() - 雙輪同步前進")
print("左右輪都以 50 速度前進 (直走)...")
exp.drive(LEFT, RIGHT, 50, 50)
time.sleep(2)

print("\n[展示 2] drive() - 雙輪同步後退")
print("左右輪都以 -40 速度後退...")
exp.drive(LEFT, RIGHT, -40, -40)
time.sleep(2)

print("\n[展示 3] drive() - 原地旋轉 (坦克轉)")
print("左輪後退 (-50)，右輪前進 (50)，車子會原地左轉...")
exp.drive(LEFT, RIGHT, -50, 50)
time.sleep(1.5)

print("\n[展示 4] drive() - 大彎弧線 (差速轉向)")
print("左輪慢 (20)，右輪快 (80)，車子會向左畫大圓弧...")
exp.drive(LEFT, RIGHT, 20, 80)
time.sleep(2)

# ---------------------------------------------------------
# API 展示：drive_stop(left_port, right_port, stop) - stop 必填，同 motor_stop 的 4 種狀態
# ---------------------------------------------------------
print("\n[展示 5] drive_stop() - 停止雙輪 (stop=1，滑行)")
exp.drive(LEFT, RIGHT, 50, 50)
time.sleep(1)
exp.drive_stop(LEFT, RIGHT, 1)

time.sleep(1)
print("\n[展示 6] drive_stop() - 停止雙輪 (stop=3，兩輪協同鎖死)")
print("兩輪同步中直接下 hold，車體會維持朝向、不會一邊鎖死一邊被推著轉...")
exp.drive(LEFT, RIGHT, 50, 50)
time.sleep(1)
exp.drive_stop(LEFT, RIGHT, 3)
time.sleep(1)

exp.stop_all(1)
print("\n測試結束！")
