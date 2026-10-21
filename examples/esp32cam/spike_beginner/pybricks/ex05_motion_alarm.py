# 神攝手（學校版）範例 5：動態偵測警報（有東西在鏡頭前動，就亮紅燈、嗶一聲、記一次）
# 接線：神攝手 → C 埠。機器人要放著不動！
from pybricks.hubs import PrimeHub
from pybricks.parameters import Color, Port
from pybricks.tools import wait
from MBC_cam_school_Lib import MBC_CAM_SCHOOL

hub = PrimeHub()
cam = MBC_CAM_SCHOOL(Port.C)
count = 0                             # 警報響了幾次

while True:
    if cam.motion() > 20:             # 動態值：畫面變化越大數字越大（0~100）
        hub.light.on(Color.RED)
        hub.speaker.beep(880, 200)
        count = count + 1
        hub.display.number(count)     # 螢幕顯示第幾次
        print(count)
        wait(1000)                    # 等 1 秒，同一個動作不要重複算
    else:
        hub.light.on(Color.GREEN)     # 沒動靜：綠燈
    wait(100)
