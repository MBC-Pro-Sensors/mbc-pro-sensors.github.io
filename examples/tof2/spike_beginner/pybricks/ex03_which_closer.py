# 測距者 2 路 範例 3：哪一邊比較近（主機螢幕顯示箭頭）
# 接線：測距者 2 路 → C 埠
from pybricks.hubs import PrimeHub
from pybricks.parameters import Icon
from pybricks.tools import wait
from MBC_laser2_obj_Lib import MBC_LASER2

hub = PrimeHub()
laser2 = MBC_LASER2(3)                      # 3 = C 埠

while True:
    diff = laser2.diff()                    # 左距離 減 右距離：負的 = 左邊比較近，正的 = 右邊比較近
    if laser2.min_distance() >= 120:        # 兩邊都是 120 → 前面什麼都沒有
        hub.display.icon(Icon.FALSE)        # 顯示 ✕
    elif diff < -5:                         # 左邊近了 5 公分以上
        hub.display.icon(Icon.ARROW_LEFT)
    elif diff > 5:                          # 右邊近了 5 公分以上
        hub.display.icon(Icon.ARROW_RIGHT)
    else:                                   # 兩邊差不多 → 東西在正前方
        hub.display.icon(Icon.ARROW_UP)
    wait(100)
