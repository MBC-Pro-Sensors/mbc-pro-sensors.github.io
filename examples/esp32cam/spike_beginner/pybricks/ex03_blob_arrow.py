# 神攝手（學校版）範例 3：色塊在哪一邊（主機螢幕顯示箭頭）
# 接線：神攝手 → C 埠
from pybricks.hubs import PrimeHub
from pybricks.parameters import Icon, Port
from pybricks.tools import wait
from MBC_cam_school_Lib import MBC_CAM_SCHOOL

hub = PrimeHub()
cam = MBC_CAM_SCHOOL(Port.C)

while True:
    x = cam.blob_x()                      # 色塊 X：左 0 ~ 右 98，中間約 50
    if not cam.found():                   # 沒找到色塊（這時 X 是 0，不能拿來判斷）
        hub.display.icon(Icon.FALSE)      # 顯示 ✕
    elif x < 35:
        hub.display.icon(Icon.ARROW_LEFT)
    elif x > 65:
        hub.display.icon(Icon.ARROW_RIGHT)
    else:                                 # 35 ~ 65 都算中間
        hub.display.icon(Icon.ARROW_UP)
    wait(100)
