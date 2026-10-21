# 測距者 2 路 範例 2：太近就嗶（任何一邊小於 20 公分：紅燈＋嗶聲；否則綠燈）
# 接線：測距者 2 路 → C 埠
from pybricks.hubs import PrimeHub
from pybricks.parameters import Color
from pybricks.tools import wait
from MBC_laser2_obj_Lib import MBC_LASER2

hub = PrimeHub()
laser2 = MBC_LASER2(3)                  # 3 = C 埠

while True:
    if laser2.min_distance() < 20:      # 左右兩邊比較近的那個距離，小於 20 公分
        hub.light.on(Color.RED)
        hub.speaker.beep(880, 100)      # 頻率 880 Hz，響 0.1 秒
    else:
        hub.light.on(Color.GREEN)
    wait(100)
