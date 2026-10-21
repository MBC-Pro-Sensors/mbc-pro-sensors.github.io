# 神攝手 WRO 版（HUE12）範例 3：12 格裡有幾格看到顏色（彩色）
# 接線：HUE12 → C 埠
from pybricks.hubs import PrimeHub
from pybricks.parameters import Port
from pybricks.tools import wait
from MBC_hue12_Lib import HUE12Sensor

hub = PrimeHub()
cam = HUE12Sensor(Port.C)

while True:
    cam.update()
    count = 0                                   # 先歸零
    for slot in range(1, 13):                   # 第 1 格到第 12 格，一格一格檢查
        name, hue, sat = cam.get_slot(slot)
        if hue < 240:                           # 小於 240 = 彩色（240 以上是白、黑、沒設定）
            count = count + 1
    print(count)
    hub.display.number(count)                   # 主機螢幕顯示有幾格是彩色（0~12）
    wait(500)
