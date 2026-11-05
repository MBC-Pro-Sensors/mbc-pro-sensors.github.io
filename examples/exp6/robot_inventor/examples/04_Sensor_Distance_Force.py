# 範例 04：超音波與力量感測器 (Ultrasonic & Force Sensors)
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
print(" 範例 04：超音波與力量感測器測試")
print("==============================")

exp = MBC_EXP6(port.F)
time.sleep(1)

# 請將超音波接在 3 號孔，觸碰感測器接在 4 號孔進行測試
PORT_US = 3
PORT_FORCE = 4

# ---------------------------------------------------------
# API 展示：get_ultrasonic_distance(port)
# ---------------------------------------------------------
print("\n[展示 1] get_ultrasonic_distance() - 讀取 {} 號孔距離".format(PORT_US))
print("開始連續讀取超音波數值 5 次 (單位: mm)...")
for i in range(5):
    dist = exp.get_ultrasonic_distance(PORT_US)
    print("  距離: {} mm".format(dist))
    time.sleep(0.5)

# ---------------------------------------------------------
# API 展示：get_touch_force(port)
# ---------------------------------------------------------
print("\n[展示 2] get_touch_force() - 讀取 {} 號孔按壓力量".format(PORT_FORCE))
print("請試著按壓觸碰感測器，連續讀取 5 次...")
for i in range(5):
    # force 是 0~100 的百分比數值
    force = exp.get_touch_force(PORT_FORCE)

    if force > 0:
        print("  狀態: 💥 被按下了！力量: {}%".format(force))
    else:
        print("  狀態: ⚪ 未按下 (力量: {}%)".format(force))
    time.sleep(0.5)

print("\n測試結束！")
