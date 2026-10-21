# 測距者 8 路 範例 1：最近的東西在哪個方向（螢幕顯示 -7 ~ 7，沒東西顯示 ✕）
# 接線：測距者 8 路 → C 埠
from pybricks.hubs import PrimeHub
from pybricks.parameters import Icon
from pybricks.tools import wait
from MBC_laser8_obj_Lib import MBC_LASER8

hub = PrimeHub()
laser8 = MBC_LASER8(3)                      # 3 = C 埠（1=A、2=B、3=C、4=D、5=E、6=F）

while True:
    dist = laser8.min_distance()            # 最近的東西有多遠（公分，0~120；120 = 四周都沒東西）
    direction = laser8.min_direction()      # 最近的東西在哪個方向：負的 = 左邊、0 = 正前方、正的 = 右邊
    if dist >= 120:
        hub.display.icon(Icon.FALSE)        # 沒看到東西 → 顯示 ✕
    else:
        hub.display.number(direction)
        print('方向', direction, '  距離', dist)
    wait(200)                               # 等 0.2 秒再讀下一次
