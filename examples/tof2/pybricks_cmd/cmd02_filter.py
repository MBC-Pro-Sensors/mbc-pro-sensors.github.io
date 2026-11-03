# 測距者 2 路 命令範例 02（Pybricks，SPIKE Prime／Robot Inventor 主機）：濾波：讓數字比較穩
# 用主機的左、右鍵切換濾波（命令 E1）：0 關、1 最近 3 筆中位數（出廠）、2 最近 5 筆中位數
# 擺法：盒子或白板放在左右兩顆雷射的正前方約 50 公分不要動。每秒印「1 秒內最大−最小」＝數字跳動幾 mm，濾波越大越穩
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

filt = 1
laser2.cmd(0xE3, 0xA5)              # 先回出廠設定（濾波 1）
laser2.wait_done()
wait(500)                        # 等感應器量到新的值再開始算
hub.display.number(filt)

while True:
    lo, hi = 99999, -1
    t = StopWatch()
    while t.time() < 1000:           # 1 秒內一直讀，記下最大、最小
        mm = laser2.left_mm()
        lo = min(lo, mm)
        hi = max(hi, mm)
        pressed = hub.buttons.pressed()
        if Button.RIGHT in pressed or Button.LEFT in pressed:
            filt = (filt + (1 if Button.RIGHT in pressed else 2)) % 3
            laser2.cmd(0xE1, filt)      # 換濾波（馬上生效）
            laser2.wait_done()
            hub.display.number(filt)
            print('換成濾波', filt)
            while hub.buttons.pressed():
                wait(10)
            lo, hi = 99999, -1
            t.reset()
        wait(20)
    print('濾波', filt, '｜左 mm', lo, '~', hi, '｜跳動', hi - lo, 'mm')
