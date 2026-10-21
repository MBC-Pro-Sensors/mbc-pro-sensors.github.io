# 循行者 16 路 範例 1：看見黑線（顯示線位置）
# 接線：循行者 16 路 → C 埠
from pybricks.hubs import PrimeHub
from pybricks.tools import wait
from MBC_line16_obj_Lib import MBC_LINE16

hub = PrimeHub()
line16 = MBC_LINE16(3)        # 3 = C 埠（1=A、2=B、3=C、4=D、5=E、6=F）

while True:
    pos = line16.pos16()      # 線位置：-16（線在左）~ 0（中間）~ 16（線在右）
    hub.display.number(pos)   # 主機螢幕顯示這個數字
    print(pos)                # 電腦畫面也印出來
    wait(200)                 # 等 0.2 秒再讀下一次
