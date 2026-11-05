# 範例 07：馬達正裝與反裝 (Motor Direction Inversion)
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
print(" 範例 07：馬達正裝/反裝測試")
print("==============================")
print("請將馬達接在擴充板 1 號孔，觀察馬達實際轉動方向")

exp = MBC_EXP6(port.F)
time.sleep(1)

PORT = 1

# ---------------------------------------------------------
# 展示：預設狀態（正裝，inverted=False）下 speed 正值的實際轉向
# ---------------------------------------------------------
print("\n[展示 1] 預設狀態（正裝）下，speed=50 的角度變化方向")
exp.reset_angle(PORT)
exp.motor_run(PORT, 50)
time.sleep(1)
print("正裝時角度變化: {}".format(exp.get_motor_angle(PORT)))
exp.motor_stop(PORT, 2)
time.sleep(0.5)

# ---------------------------------------------------------
# API 展示：set_motor_inverted(port, inverted=True) - 設為反裝
# ---------------------------------------------------------
print("\n[展示 2] set_motor_inverted(port, True) - 設為反裝")
print("反裝後，指令方向與角度讀值都會自動反過來，寫程式時完全不用改 speed 正負號")
exp.set_motor_inverted(PORT, True)
exp.reset_angle(PORT)
exp.motor_run(PORT, 50)
time.sleep(1)
print("反裝時角度變化: {}（應該跟展示 1 方向相反）".format(exp.get_motor_angle(PORT)))
exp.motor_stop(PORT, 2)
time.sleep(0.5)

# ---------------------------------------------------------
# API 展示：set_motor_inverted(port, inverted=False) - 改回正裝
# ---------------------------------------------------------
print("\n[展示 3] set_motor_inverted(port, False) - 改回正裝")
exp.set_motor_inverted(PORT, False)
exp.reset_angle(PORT)
exp.motor_run(PORT, 50)
time.sleep(1)
print("改回正裝後角度變化: {}（應該恢復跟展示 1 一致）".format(exp.get_motor_angle(PORT)))
exp.motor_stop(PORT, 2)

print("\n測試結束！適合用在馬達實際裝反、但不想每次下指令都手動加負號的情境。")
