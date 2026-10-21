# 測距者 2 路 範例 1：看見距離（螢幕捲動顯示「左距離 右距離」）
# 接線：測距者 2 路 → C 埠
from pybricks.hubs import PrimeHub
from pybricks.tools import wait
from MBC_laser2_obj_Lib import MBC_LASER2

hub = PrimeHub()
laser2 = MBC_LASER2(3)              # 3 = C 埠（1=A、2=B、3=C、4=D、5=E、6=F）

while True:
    left = laser2.left()            # 左邊雷射量到的距離（公分，0~120；120 = 前面沒東西）
    right = laser2.right()          # 右邊雷射量到的距離
    hub.display.text(str(left) + ' ' + str(right))   # 主機螢幕捲動顯示，例如「40 80」
    print('左', left, '  右', right)                  # 電腦畫面也印出來
    wait(200)                       # 等 0.2 秒再讀下一次
