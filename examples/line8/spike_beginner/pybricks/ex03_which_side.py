# 循行者 8 路 範例 3：線在哪一邊（主機螢幕顯示箭頭）
# 接線：循行者 8 路 → C 埠
from pybricks.hubs import PrimeHub
from pybricks.parameters import Icon
from pybricks.tools import wait
from MBC_line8_obj_Lib import MBC_LINE8

hub = PrimeHub()
line8 = MBC_LINE8(3)                      # 3 = C 埠

while True:
    pos = line8.pos8()                    # 負 = 線在左，正 = 線在右
    if line8.width() == 0:                # 沒看到線（這時位置會停在最後一次的值，不能拿來判斷）
        hub.display.icon(Icon.FALSE)      # 顯示 ✕
    elif pos < -1:
        hub.display.icon(Icon.ARROW_LEFT)
    elif pos > 1:
        hub.display.icon(Icon.ARROW_RIGHT)
    else:                                 # -1、0、1 都算在中間
        hub.display.icon(Icon.ARROW_UP)
    wait(100)
