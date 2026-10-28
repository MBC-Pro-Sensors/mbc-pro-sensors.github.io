# 神攝手 × Pybricks（SPIKE）範例 2：多色找位置
# Sharpshooter Vision × Pybricks (SPIKE) example 2: where are my colours?
# 需要把相機選單切到 相機模式 1 顏色追蹤 ・子選項 ② 多色 ＝模式編號 12（SPIKE 給 7 色的 X、Y）
# Camera menu: camera mode 1 Colour tracking, option 2 Multi = mode number 12 (7 colours, X and Y)
# 準備：在相機上教目標 1～7 的顏色（教幾色都可以）/ teach targets 1~7 (any number)
# 接線 Wiring：相機 → C 孔 / camera → port C
from pybricks.hubs import PrimeHub
from pybricks.parameters import Port, Button
from pybricks.tools import wait
from MBC_UniCam import UniCam

hub = PrimeHub()
cam = UniCam(Port.C, mode=12)


def where(x, y):
    # 像素 → 文字：左／中／右 + 上／中／下 / pixels -> words
    h = '左 left' if x < 54 else ('右 right' if x > 106 else '中 middle')
    v = '上 top' if y < 40 else ('下 bottom' if y > 80 else '中 middle')
    return h + ', ' + v


while True:
    seen = 0
    for c in range(1, cam.max_color() + 1):      # 色 1～7 / colours 1~7
        p = cam.xy(c)
        if p and p[0]:                           # X = 0 表示沒找到 / X = 0 means not found
            seen += 1
            print('色 colour', c, ':', p[0], p[1], where(p[0], p[1]))
    hub.display.number(seen)                     # 主機螢幕顯示看到幾色 / show how many colours are seen
    if Button.LEFT in hub.buttons.pressed():
        # 數量放在模式 6，要多等約 30 ms，所以按鍵才讀 / counts are in mode 6 (30 ms slower): read on demand
        print('16 色裡找到', cam.colors_found(), '色 / colours found;  色 1 有', cam.count(1), '塊 / blobs of colour 1')
    print('---')
    wait(500)
