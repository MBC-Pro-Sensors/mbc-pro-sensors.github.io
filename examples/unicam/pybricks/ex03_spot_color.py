# 神攝手 × Pybricks（SPIKE）範例 3：座標取色判斷顏色
# Sharpshooter Vision × Pybricks (SPIKE) example 3: what colour is at the spot?
# 需要把相機選單切到 相機模式 2 座標取色 ・子選項 ② 色相＋飽和 ＝模式編號 22（SPIKE 給 7 點）
# Camera menu: camera mode 2 Spot colour, option 2 Hue + saturation = mode number 22 (7 spots)
# 準備：取樣點 S1 的位置看相機螢幕上標 1 的十字 / spot S1 is the cross marked 1 on the camera screen
# 接線 Wiring：相機 → C 孔 / camera → port C
from pybricks.hubs import PrimeHub
from pybricks.parameters import Port, Color
from pybricks.tools import wait
from MBC_UniCam import UniCam, color_name

hub = PrimeHub()
cam = UniCam(Port.C, mode=22)

# 顏色名稱 → 主機燈號 / colour name -> hub light
LIGHT = {'red': Color.RED, 'orange': Color.ORANGE, 'yellow': Color.YELLOW, 'green': Color.GREEN,
         'cyan': Color.CYAN, 'blue': Color.BLUE, 'purple': Color.VIOLET, 'white': Color.WHITE, 'black': Color.NONE}
NAME = {'red': '紅', 'orange': '橙', 'yellow': '黃', 'green': '綠', 'cyan': '青',
        'blue': '藍', 'purple': '紫', 'white': '白／灰', 'black': '黑'}
LOW_SAT = 15      # 👉 可以改：飽和度比這低就當成白／灰 / below this saturation = white / grey

while True:
    p = cam.hsv(1)                               # 取樣點 S1 的 (色相碼, 飽和度, None) / (hue, sat, None)
    if p is None:                                # 讀不到相機 / no camera
        hub.light.on(Color.NONE)
        wait(200)
        continue
    h, s = p[0], p[1]
    name = color_name(h)
    if s < LOW_SAT and name != 'black':
        name = 'white'                           # 顏色很淡：當成白／灰 / very pale: white / grey
    hub.light.on(LIGHT[name])
    print('S1 色相碼 hue', h, ' 飽和度 sat', s, ' →', NAME[name], name)
    wait(200)
