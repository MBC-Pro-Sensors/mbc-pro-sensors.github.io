# EXP6 × 循行者 8 路 PD 循線（速度 50）／EXP6 + Line Follower 8-ch PD line following (speed 50)
# 接線：EXP6 的 Host 線 → 主機 C 孔；循行者 8 路（SPIKE 模式）→ EXP6 第 5 孔；左馬達 → EXP6 擴充孔 1（反裝）；右馬達 → 擴充孔 2；EXP6 接上電池
# Wiring: EXP6 Host cable → hub port C; Line Follower 8-ch (SPIKE mode) → EXP6 port 5; left motor → EXP6 port 1 (mounted reversed); right motor → port 2
# 函式庫：MBC_exp6_obj_Lib.py、MBC_exp6_mbc_common.py、MBC_exp6_line.py 三個檔放同一個 Pybricks 專案（EXP6 韌體 2026-11-03 以後）
# Libraries: put MBC_exp6_obj_Lib.py, MBC_exp6_mbc_common.py and MBC_exp6_line.py in the same Pybricks project (EXP6 firmware 2026-11-03 or later)
# 只匯入用到的產品檔：主機程式上限約 52 KB，循行者只要 MBC_exp6_line／Import only the product file you use: hub programs are limited to about 52 KB
# 多工寫法：MBC_EXP6(3, True)，每個 EXP6 與感應器的函式都要 await；run_task 要用 MBC_exp6_obj_Lib 的（等待時會在背景送心跳，馬達才不會被看門狗停掉）
# Multitask: MBC_EXP6(3, True), await every EXP6 and sensor call; use run_task from MBC_exp6_obj_Lib (it sends the heartbeat while waiting)
# KP、KD 是建議值，請在你的車上調整／KP and KD are starting values, tune them on your robot
# 積木版：同名的 exp6_line8_pd_block.py（Pybricks Code 打開就是積木畫面）／Block version: exp6_line8_pd_block.py
from pybricks.hubs import PrimeHub
from pybricks.tools import multitask, run_task, wait

from MBC_exp6_line import EXP6_LINE8
from MBC_exp6_obj_Lib import MBC_EXP6, run_task     # 這行要在 pybricks.tools 之後：run_task 用函式庫的／must come after pybricks.tools

# Set up.
hub = PrimeHub()
exp6 = MBC_EXP6(3, True)          # 3 = EXP6 插主機 C 孔；True = 多工／EXP6 on hub port C, multitask
line8 = EXP6_LINE8(exp6, 5)       # 循行者 8 路插在 EXP6 的 5 號孔／Line Follower 8-ch in EXP6 port 5
SPEED = 50     # 前進速度（%）／forward speed (%)
KP = 0.4     # 轉彎靈敏度：車子左右甩就調小／steering gain: lower it if the robot wobbles
KD = 1.5     # 抑制甩動：轉彎太慢就調小／damping: lower it if turns are too slow
error = 0
lastError = 0
PD = 0

async def main():
    global error, PD, lastError
    await exp6.set_motor_inverted(1, True)     # 左馬達反過來裝：設成反裝，兩輪給正值車子都往前／left motor is mounted reversed
    while True:
        await wait(0)
        if await line8.width() > 5:     # 線寬太寬＝十字路口（整排踩到線）：停車／too wide = crossing: stop
            await exp6.motor_power(1, 0)
            await exp6.motor_power(2, 0)
        else:
            error = await line8.pos100()
            PD = error * KP + (error - lastError) * KD     # P：偏多少轉多少；D：偏移變快時多轉一點／P: how far off; D: how fast it drifts
            await exp6.motor_power(1, SPEED + PD)
            await exp6.motor_power(2, SPEED - PD)
            lastError = error
        await wait(10)


run_task(main())
