# 範例 09：MBC 感應器插在 EXP6 上 (MBC Sensors on EXP6)
# ℹ️ 版本提醒 (Version Reminder):
# [ZH] 本程式適用於 LEGO MINDSTORMS Robot Inventor App 10.x（玩具版 51515），需要 EXP6 韌體 2026-11-03 以後
# [EN] For LEGO MINDSTORMS Robot Inventor App 10.x (51515); needs EXP6 firmware dated 2026-11-03 or later
#
# ⚠️ 測試提醒 (Testing Reminder):
# [ZH] 執行前請先在主機中執行過 `Install_MBC_exp6_Lib.py`。感應器直接插上就能用、不用另外設定。
#      官方 App 每次「換讀別的欄位」約多 40~100 ms；同一組欄位（例如 pos8 與 width）連續讀最快。
# [EN] Run `Install_MBC_exp6_Lib.py` on the hub first. Sensors work as soon as they are plugged in.

from hub import port
import time
# 玩具版：函式庫裝在主機的 /projects/mbc_exp6/（先執行一次 Install_MBC_exp6_Lib.py）
from projects.mbc_exp6.MBC_exp6_SPIKE_App_Lib import MBC_EXP6

print("==============================")
print(" 範例 09：MBC 感應器")
print("==============================")

exp = MBC_EXP6(port.F)   # EXP6 插在 Hub 的 F 孔
time.sleep(1)

line8  = exp.line8(2)    # 循行者 8 路插在 EXP6 的 2 號孔
laser2 = exp.laser2(3)   # 兩路雷射插在 3 號孔
ps     = exp.ps4(4)      # PS4 藍牙遙控插在 4 號孔（PS2 用 exp.ps2(4)）
cam    = exp.unicam(5, 13)   # 神攝手插在 5 號孔，相機選單切到 1-3（顏色追蹤 標準），mode 要跟相機一樣

# [展示 1] 循行者 8 路
print("\n[展示 1] 循行者 8 路")
for i in range(5):
    print("  pos8={}  width={}  pos100={}".format(line8.pos8(), line8.width(), line8.pos100()))
    time.sleep(0.2)

# [展示 2] 兩路雷射：left/right/average/min_side/diff 都在同一組欄位
print("\n[展示 2] 兩路雷射")
for i in range(5):
    print("  左={} 右={} 較近的一邊={}".format(laser2.left(), laser2.right(), laser2.min_side()))
    time.sleep(0.2)

# [展示 3] 遙控器：按 ✕ 結束
print("\n[展示 3] 遙控器（按 ✕ 結束）")
while not ps.pressed("cross"):
    print("  按下:", ps.pressed_buttons())
    time.sleep(0.2)

# [展示 4] 神攝手：色 1 的位置與面積（xy 一次讀到 X、Y；換到 area 要多等一點）
print("\n[展示 4] 神攝手（模式 13）")
for i in range(5):
    print("  色1 位置={} 面積={}%".format(cam.xy(1), cam.area(1)))
    time.sleep(0.5)

print("\n測試結束！")
