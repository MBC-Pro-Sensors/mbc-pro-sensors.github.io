# 測距者 2 路 命令範例 03（Pybricks，SPIKE Prime／Robot Inventor 主機）：更新時間：快一點還是準一點
# 用主機的左、右鍵切換更新時間（命令 E2）：20、33（出廠）、100、200 ms
# 擺法：盒子或白板放在左右兩顆雷射的正前方約 50 公分不要動。每秒印「值換了幾次」（≈ 更新幾次）和「跳動幾 mm」
# 接線：測距者 2 路 → C 埠；跟 MBC_laser2_obj_Lib.py 一起上傳（Pybricks Code 把兩個檔放在一起）
# 命令只有 SPIKE 版能下（感應器韌體 2026-11 以後）；設定存在感應器記憶體，拔線就回出廠
#   cmd(命令碼, 參數)：送命令；wait_done()：等它做完，True = 完成、False = 失敗（參數超出範圍）或沒回應
#   命令表：E0 測距模式、E1 濾波、E2 更新時間、E3 回出廠（參數 0xA5）、E4 距離上限
from pybricks.hubs import PrimeHub
from pybricks.parameters import Button, Color
from pybricks.tools import wait, StopWatch
from MBC_laser2_obj_Lib import MBC_LASER2

hub = PrimeHub()
laser2 = MBC_LASER2(3)                   # 3 = C 埠（1=A、2=B、3=C、4=D、5=E、6=F）

CHOICES = [20, 33, 100, 200]
k = 1
laser2.cmd(0xE3, 0xA5)              # 先回出廠設定（33 ms）
laser2.wait_done()
laser2.cmd(0xE1, 0)                 # 關濾波，比較看得出差別
laser2.wait_done()
wait(500)                        # 等感應器量到新的值再開始算
hub.display.number(CHOICES[k])

while True:
    last, changes, lo, hi = -1, 0, 99999, -1
    t = StopWatch()
    while t.time() < 1000:
        mm = laser2.left_mm()
        if mm != last:
            changes += 1
            last = mm
        lo = min(lo, mm)
        hi = max(hi, mm)
        pressed = hub.buttons.pressed()
        if Button.RIGHT in pressed or Button.LEFT in pressed:
            k = (k + (1 if Button.RIGHT in pressed else 3)) % 4
            laser2.cmd(0xE2, CHOICES[k])    # 換更新時間（要重新設定雷射晶片）
            ok = laser2.wait_done()
            hub.display.number(CHOICES[k])
            print('換成', CHOICES[k], 'ms', '完成' if ok else '失敗')
            while hub.buttons.pressed():
                wait(10)
            last, changes, lo, hi = -1, 0, 99999, -1
            t.reset()
        wait(5)
    print(CHOICES[k], 'ms｜1 秒內換了', changes, '次｜跳動', hi - lo, 'mm')
