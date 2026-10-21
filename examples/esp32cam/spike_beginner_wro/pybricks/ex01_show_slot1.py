# 神攝手 WRO 版（HUE12）範例 1：讀第 1 格的色相（顯示在主機螢幕）
# 接線：HUE12 → C 埠
from pybricks.hubs import PrimeHub
from pybricks.parameters import Port
from pybricks.tools import wait
from MBC_hue12_Lib import HUE12Sensor

hub = PrimeHub()
cam = HUE12Sensor(Port.C)               # HUE12 插在 C 孔

while True:
    cam.update()                        # 讀一次最新資料
    name, hue, sat = cam.get_slot(1)    # 第 1 格：顏色名稱、色相、飽和度
    print(name, hue, sat)               # 電腦畫面印出，例如 Red 5 200
    hub.display.text(str(hue))          # 主機螢幕一個字一個字顯示色相
    wait(500)
