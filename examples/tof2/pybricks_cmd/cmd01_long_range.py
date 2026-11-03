# 測距者 2 路 命令範例 01（Pybricks，SPIKE Prime／Robot Inventor 主機）：測距模式：一般／遠距離／精準／高速
# 用主機的左、右鍵切換 4 種測距模式（命令 E0），螢幕顯示目前模式，主控台印距離：
#   0 一般（出廠，最遠 120 cm）、1 遠距離（最遠 200 cm）、2 精準（每 100 ms，最穩）、3 高速（每 20 ms，最快）
# 擺法：白牆或大白板放在左右兩顆雷射的正前方約 150 公分。一般模式會讀到 120（超過上限＝當成沒東西），換成 1 就讀到約 150
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

mode = 0
laser2.cmd(0xE3, 0xA5)              # 先回出廠設定（一般模式）
laser2.wait_done()
hub.display.number(mode)
t = StopWatch()

while True:
    pressed = hub.buttons.pressed()    # 每 10 ms 看一次按鍵，按一下就收得到
    if Button.RIGHT in pressed or Button.LEFT in pressed:
        mode = (mode + (1 if Button.RIGHT in pressed else 3)) % 4   # 右鍵 +1、左鍵 -1
        laser2.cmd(0xE0, mode)        # 換測距模式（要重新設定雷射晶片，約 0.1～0.5 秒）
        ok = laser2.wait_done()       # 等它做完：True = 完成
        hub.display.number(mode)
        print('換成模式', mode, ['一般', '遠距離', '精準', '高速'][mode], '完成' if ok else '失敗')
        while hub.buttons.pressed():   # 等放開按鍵
            wait(10)
    if t.time() >= 300:                # 每 0.3 秒印一次距離
        t.reset()
        print('左', laser2.left(), '右', laser2.right(), 'cm｜左', laser2.left_mm(), 'mm')
    wait(10)
