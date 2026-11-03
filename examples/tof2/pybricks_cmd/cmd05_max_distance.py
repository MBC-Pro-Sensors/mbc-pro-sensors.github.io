# 測距者 2 路 命令範例 05（Pybricks，SPIKE Prime／Robot Inventor 主機）：距離上限：只看近的東西
# 距離上限（命令 E4，cm）：超過上限的東西當成「沒東西」，讀到的值就是上限 → 只對近的東西反應
# 左、右鍵把上限改成 10、20、30…120 cm（螢幕顯示），較近距離比上限近就亮紅燈＋嗶，否則綠燈
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

limit = 30
laser2.cmd(0xE3, 0xA5)              # 先回出廠設定
laser2.wait_done()
laser2.cmd(0xE4, limit)             # 距離上限 30 cm
laser2.wait_done()
hub.display.number(limit)
t = StopWatch()

while True:
    pressed = hub.buttons.pressed()    # 每 10 ms 看一次按鍵
    if Button.RIGHT in pressed or Button.LEFT in pressed:
        limit = limit + 10 if Button.RIGHT in pressed else limit - 10
        limit = max(10, min(120, limit))
        laser2.cmd(0xE4, limit)       # 改上限（馬上生效）
        laser2.wait_done()
        hub.display.number(limit)
        while hub.buttons.pressed():
            wait(10)
    if t.time() >= 200:                # 每 0.2 秒看一次距離
        t.reset()
        cm = min(laser2.left(), laser2.right())
        if cm < limit:               # 比上限近 = 真的有東西
            hub.light.on(Color.RED)
            hub.speaker.beep(1000, 50)
        else:
            hub.light.on(Color.GREEN)
        print('上限', limit, 'cm｜', '左', laser2.left(), '右', laser2.right(), 'cm｜左', laser2.left_mm(), 'mm')
    wait(10)
