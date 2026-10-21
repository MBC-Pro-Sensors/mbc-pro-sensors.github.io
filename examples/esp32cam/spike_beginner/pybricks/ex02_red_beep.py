# 神攝手（學校版）範例 2：看到紅色就亮紅燈、嗶一聲
# 接線：神攝手 → C 埠
from pybricks.hubs import PrimeHub
from pybricks.parameters import Color, Port
from pybricks.tools import wait
from MBC_cam_school_Lib import MBC_CAM_SCHOOL

hub = PrimeHub()
cam = MBC_CAM_SCHOOL(Port.C)

while True:
    hue = cam.hue()
    if 0 <= hue <= 5 or hue >= 95:    # 紅色在色相的兩端：0 附近和 100 附近都是紅
        hub.light.on(Color.RED)
        hub.speaker.beep(880, 100)    # 頻率 880 Hz，響 0.1 秒
    else:                             # 其他顏色，或 -1（黑白灰）
        hub.light.off()
    wait(100)
