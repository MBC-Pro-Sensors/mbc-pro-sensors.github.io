# 測距者 2 路 命令範例 04（Pybricks，SPIKE Prime／Robot Inventor 主機）：回出廠設定
# 回出廠設定（命令 E3，參數一定要是 0xA5）：先把距離上限改成 30 cm，看讀數被「蓋」在 30；
# 按中間鍵送 E3 回出廠 → 上限回到 120 cm，讀數又看得到遠的東西
# 擺法：白板放在左右兩顆雷射的正前方約 60 公分（比 30 遠、比 120 近）
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

laser2.cmd(0xE4, 30)                # 先把距離上限改成 30 cm（超過 30 = 當成沒東西，讀到 30）
laser2.wait_done()
hub.display.number(30)
print('距離上限 30 cm：按中間鍵送 E3 回出廠')
hub.system.set_stop_button(None)   # 中間鍵拿來用（要停程式：按住中間鍵不放）

t = StopWatch()
while True:
    if Button.CENTER in hub.buttons.pressed():   # 每 10 ms 看一次按鍵
        laser2.cmd(0xE3, 0xA5, again=True)   # 回出廠；again=True：同一個命令也要再做一次
        ok = laser2.wait_done()
        hub.display.number(120)
        hub.speaker.beep()
        print('回出廠', '完成' if ok else '失敗', '：上限回到 120 cm')
        while hub.buttons.pressed():
            wait(10)
    if t.time() >= 300:                # 每 0.3 秒印一次距離
        t.reset()
        print('左', laser2.left(), '右', laser2.right(), 'cm｜左', laser2.left_mm(), 'mm')
    wait(10)
