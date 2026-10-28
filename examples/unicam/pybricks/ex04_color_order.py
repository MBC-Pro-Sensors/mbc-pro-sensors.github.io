# 神攝手 × Pybricks（SPIKE）範例 4：照順序給色號（WRO 任務：記下積木的顏色順序）
# Sharpshooter Vision × Pybricks (SPIKE) example 4: colour order (WRO: remember the order)
# 需要把相機選單切到 相機模式 3 照順序給色號（沒有子選項）＝模式編號 30
# Camera menu: camera mode 3 Colour order (no option) = mode number 30
# 準備：在相機上教會場地上會出現的每一種顏色 / teach every colour that can appear on the field
# 接線 Wiring：相機 → C 孔 / camera → port C
from pybricks.hubs import PrimeHub
from pybricks.parameters import Port
from pybricks.tools import wait
from MBC_UniCam import UniCam, color_name

hub = PrimeHub()
cam = UniCam(Port.C, mode=30)

WANT = 4          # 👉 可以改：場地上有幾塊 / how many blocks to expect
LETTER = {'red': 'R', 'orange': 'O', 'yellow': 'Y', 'green': 'G', 'cyan': 'C',
          'blue': 'B', 'purple': 'P', 'white': 'W', 'black': 'K'}

# 連續 5 次讀到一樣的順序才算數（避免剛好有手經過）/ same answer 5 times in a row = stable
last = None
same = 0
names = []
while same < 5:
    hues = cam.order()                           # 由左到右每塊的色相碼 / hue codes from left to right
    names = [color_name(h) for h in hues] if hues else []
    if len(names) == WANT and names == last:
        same += 1
    else:
        same = 0
    last = names
    wait(50)

print('順序 order:', names)
hub.speaker.beep()
for n in names:                                  # 一個字母一個字母顯示 / show one letter at a time
    hub.display.char(LETTER[n])
    wait(700)
hub.display.off()
