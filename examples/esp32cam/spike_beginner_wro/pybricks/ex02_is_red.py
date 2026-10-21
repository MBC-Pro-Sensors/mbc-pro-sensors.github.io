# 神攝手 WRO 版（HUE12）範例 2：第 1 格是紅色就亮紅燈、嗶一聲
# 接線：HUE12 → C 埠
from pybricks.hubs import PrimeHub
from pybricks.parameters import Color, Port
from pybricks.tools import wait
from MBC_hue12_Lib import HUE12Sensor

hub = PrimeHub()
cam = HUE12Sensor(Port.C)

while True:
    cam.update()
    name, hue, sat = cam.get_slot(1)
    if hue < 10 or 230 <= hue < 240:      # 紅色在色相的兩端：0~9 和 230~239（240 以上是白、黑、沒設定）
        hub.light.on(Color.RED)
        hub.speaker.beep(880, 100)
    else:
        hub.light.off()
    wait(200)
