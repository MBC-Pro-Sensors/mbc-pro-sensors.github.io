# 測距者 8 路 範例 2：有東西靠近就變燈號（< 20 公分紅燈＋嗶、< 50 公分黃燈、其他綠燈）
# 接線：測距者 8 路 → C 埠
from pybricks.hubs import PrimeHub
from pybricks.parameters import Color
from pybricks.tools import wait
from MBC_laser8_obj_Lib import MBC_LASER8

hub = PrimeHub()
laser8 = MBC_LASER8(3)                  # 3 = C 埠

while True:
    dist = laser8.min_distance()        # 8 路裡最近的那個距離（公分）
    if dist < 20:                       # 很近了！
        hub.light.on(Color.RED)
        hub.speaker.beep(880, 100)
    elif dist < 50:                     # 有東西在附近
        hub.light.on(Color.YELLOW)
    else:                               # 50 公分內都沒東西
        hub.light.on(Color.GREEN)
    wait(100)
