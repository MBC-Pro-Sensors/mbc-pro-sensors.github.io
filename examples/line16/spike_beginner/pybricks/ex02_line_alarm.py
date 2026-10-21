# 循行者 16 路 範例 2：黑線警報（壓到線亮綠燈；找不到線亮紅燈並嗶一聲）
# 接線：循行者 16 路 → C 埠
from pybricks.hubs import PrimeHub
from pybricks.parameters import Color
from pybricks.tools import wait
from MBC_line16_obj_Lib import MBC_LINE16

hub = PrimeHub()
line16 = MBC_LINE16(3)                # 3 = C 埠

while True:
    if line16.width() > 0:            # 線寬 = 有幾路踩在黑線上；大於 0 就是有看到線
        hub.light.on(Color.GREEN)
    else:                             # 線寬 = 0：16 路都沒踩到 → 找不到線
        hub.light.on(Color.RED)
        hub.speaker.beep(880, 100)    # 頻率 880 Hz，響 0.1 秒
    wait(100)
