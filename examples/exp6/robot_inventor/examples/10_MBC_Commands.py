# 範例 10：透過 EXP6 對 MBC 感應器下命令 (Send Commands to MBC Sensors through EXP6)
# ℹ️ 版本提醒 (Version Reminder):
# [ZH] 本程式適用於 LEGO MINDSTORMS Robot Inventor App 10.x（玩具版 51515），需要 EXP6 韌體 2026-11-03 以後
# [EN] For LEGO MINDSTORMS Robot Inventor App 10.x (51515); needs EXP6 firmware dated 2026-11-03 or later
#
# ⚠️ 測試提醒 (Testing Reminder):
# [ZH] 執行前請先在主機中執行過 `Install_MBC_exp6_Lib.py`。8 路循行者插在 EXP6 的 2 號孔。
#      cmd(命令碼, 參數…) 送出命令、回傳序號；wait_done() 等做完（True 完成、False 失敗／拒絕／逾時）；
#      cmd_state()：0 完成、1 執行中、2 失敗、3 拒絕。命令碼與參數跟感應器直插主機時一樣（見各感應器說明）。
#      這支只用重開機就會回到預設的命令（E4 門檻、E9 左右反向）；黑白校準 E2／E3／E7 會寫進感應器，練習時不要亂按。
# [EN] Run `Install_MBC_exp6_Lib.py` on the hub first. Plug an 8-channel line follower into EXP6 port 2.
#      cmd(code, *params) sends a command and returns its sequence number; wait_done() waits for the result
#      (True = done, False = failed / rejected / timeout); cmd_state(): 0 done, 1 busy, 2 failed, 3 rejected.
#      Only commands that reset on power-up are used here (E4 threshold, E9 left/right swap).

from hub import port
import time
# 玩具版：函式庫裝在主機的 /projects/mbc_exp6/（先執行一次 Install_MBC_exp6_Lib.py）
from projects.mbc_exp6.MBC_exp6_SPIKE_App_Lib import MBC_EXP6

print("==============================")
print(" 範例 10：對 MBC 感應器下命令")
print("==============================")

exp = MBC_EXP6(port.F)   # EXP6 插在 Hub 的 F 孔
time.sleep(1)
line8 = exp.line8(2)     # 循行者 8 路插在 EXP6 的 2 號孔

# [展示 1] 改二值化門檻（E4，參數 1~99；開機回到 50）
print("\n[展示 1] 門檻改成 60")
seq = line8.cmd(0xE4, 60)
print("  序號", seq, "→ 完成" if line8.wait_done() else "→ 沒成功，狀態 {}".format(line8.cmd_state()))
for i in range(5):
    print("  pos100={}  黑白={}".format(line8.pos100(), line8.bin_raw()))
    time.sleep(0.2)

# [展示 2] 參數超出範圍：感應器回「失敗」（狀態 2）
print("\n[展示 2] 門檻設 0（超出 1~99）")
line8.cmd(0xE4, 0)
print("  完成了嗎？", line8.wait_done(), "狀態", line8.cmd_state(), "（2 = 失敗）")

# [展示 3] 這款沒有的命令：感應器回「拒絕」（狀態 3）
print("\n[展示 3] 送一個 8 路循行者沒有的命令 E5")
line8.cmd(0xE5)
print("  完成了嗎？", line8.wait_done(), "狀態", line8.cmd_state(), "（3 = 拒絕）")

# [展示 4] 還原預設：門檻 50
print("\n[展示 4] 門檻改回 50")
line8.cmd(0xE4, 50)
print("  完成" if line8.wait_done() else "  沒成功")
print("\n範例 10 結束")
