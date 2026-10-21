# 循行者 16 路 範例 3：路口在哪一邊（左邊有岔路、右邊有岔路，還是十字路口）
# 接線：循行者 16 路 → C 埠
# 想法：一般的線只有幾路踩到；碰到路口時黑色會變很寬（線寬變大），
#       再看這一大塊黑色偏左、偏右還是在中間，就知道岔路在哪一邊。
from pybricks.hubs import PrimeHub
from pybricks.parameters import Icon
from pybricks.tools import wait
from MBC_line16_obj_Lib import MBC_LINE16

hub = PrimeHub()
line16 = MBC_LINE16(3)                    # 3 = C 埠

while True:
    pos = line16.pos16()                  # 黑色那一塊的中心：負 = 偏左，正 = 偏右
    width = line16.width()                # 有幾路踩到黑色（0 ~ 16）
    if width == 0:                        # 沒看到線（這時位置會停在最後一次的值，不能拿來判斷）
        hub.display.icon(Icon.FALSE)      # 顯示 ✕
    elif width < 6:                       # 窄窄的一條 → 一般的線
        hub.display.icon(Icon.ARROW_UP)
    elif pos < -3:                        # 很寬而且偏左 → 左邊有岔路
        hub.display.icon(Icon.ARROW_LEFT)
    elif pos > 3:                         # 很寬而且偏右 → 右邊有岔路
        hub.display.icon(Icon.ARROW_RIGHT)
    else:                                 # 很寬又在中間 → 十字路口
        hub.display.char('+')
    wait(100)
