# 測距者 8 路 範例 6（進階）：最近的東西有幾毫米（螢幕捲動顯示毫米、燈號分三段）
# 接線：測距者 8 路 → C 埠
# 只有 Pybricks 版：毫米距離放在感應器的「模式 6」，官方 App 和積木網站讀不到；感應器韌體要 2026-10 以後
from pybricks.hubs import PrimeHub
from pybricks.parameters import Color, Icon
from pybricks.tools import wait
from MBC_laser8_obj_Lib import MBC_LASER8

hub = PrimeHub()
laser8 = MBC_LASER8(3)                       # 3 = C 埠

while True:
    mm = laser8.nearest_mm()                 # 最近的東西有幾毫米（0~1200；1200 = 四周沒東西）
    direction = laser8.nearest_dir()         # 它在哪個方向：-8 ~ 8（0 = 正前方，負 = 左）
    front = laser8.front_mm()                # 正前方（第 4、5 顆平均）有幾毫米
    if mm >= 1200:                           # 四周沒東西
        hub.display.icon(Icon.FALSE)
        hub.light.on(Color.GREEN)
    else:
        hub.display.text(str(mm))            # 螢幕捲動顯示毫米，例如「300」
        if mm < 100:                         # 10 公分以內：紅燈
            hub.light.on(Color.RED)
        elif mm < 300:                       # 10~30 公分：黃燈
            hub.light.on(Color.YELLOW)
        else:                                # 更遠：綠燈
            hub.light.on(Color.GREEN)
    print('最近', mm, 'mm  方向', direction, '  正前方', front, 'mm')
    wait(200)
