# 神攝手（學校版）範例 1：看到什麼顏色（主機螢幕顯示色相）
# 接線：神攝手 → C 埠
from pybricks.hubs import PrimeHub
from pybricks.parameters import Port
from pybricks.tools import wait
from MBC_cam_school_Lib import MBC_CAM_SCHOOL

hub = PrimeHub()
cam = MBC_CAM_SCHOOL(Port.C)      # 神攝手插在 C 孔

while True:
    hue = cam.hue()               # 色相：0~100（紅 0／100、綠約 33、藍約 67）；-1 = 黑白灰
    print(hue)                    # 電腦畫面印出來（比較快看到）
    hub.display.text(str(hue))    # 主機螢幕一個字一個字顯示
    wait(500)                     # 等 0.5 秒再讀下一次
