# 神攝手 × Pybricks（SPIKE）範例 8：用程式擺取樣點（命令 E1）——左、中、右三點同時看顏色
# SharpShot × Pybricks (SPIKE) example 8: place the spots from the program (command E1)
#   read the colour at left, centre and right at the same time
# 相機模式：22 座標取色 ・② 色相＋飽和（SPIKE 給 7 點）/ mode 22 Spot colour Hue + saturation (7 spots)
# 相機韌體 2026-10-29 以後 / camera firmware 2026-10-29 or later
# 接線 Wiring：相機 → C 孔 / camera → port C
# 同一個資料夾要有 MBC_UniCam.py / MBC_UniCam.py must be imported too
#
# 函式庫沒有 E1 專用方法，用 cam.cmd(0xE1, 第幾點, X, Y) 送、cam.wait_done() 等結果（0＝好了）
# There is no E1 method: send cam.cmd(0xE1, spot, X, Y) and wait with cam.wait_done() (0 = done)
#   第幾點 spot 1~20、X 0~159（左→右）、Y 0~119（上→下）；X、Y 都填 255＝清掉這一點，第幾點填 0＝全部清掉
#   spot 1~20, X 0~159, Y 0~119; X = Y = 255 clears the spot, spot 0 with 255, 255 clears all
#   沒設定過的點 H、S、V 都是 0；取樣點位置存進相機，斷電後還在
#   spots never set read 0; positions are stored in the camera (kept after power-off)
from pybricks.hubs import PrimeHub
from pybricks.parameters import Port
from pybricks.tools import wait
from MBC_UniCam import UniCam

hub = PrimeHub()
cam = UniCam(Port.C, mode=22)

SPOTS = [(1, 30, 60), (2, 80, 60), (3, 130, 60)]  # 👉 可以改：(第幾點, X, Y) 左、中、右 / left, centre, right
NAME = {'red': '紅', 'orange': '橙', 'yellow': '黃', 'green': '綠', 'cyan': '青',
        'blue': '藍', 'purple': '紫', 'white': '白／灰', 'black': '黑'}

for n, x, y in SPOTS:
    cam.cmd(0xE1, n, x, y)                       # 送出、不等 / send without waiting
    r = cam.wait_done()                          # 0 好了、2 失敗（超出範圍）、None 逾時 / 0 done, 2 failed
    print('取樣點 spot', n, '→ (%d, %d)' % (x, y), '結果 result', r)

# cam.cmd(0xE1, 0, 255, 255); cam.wait_done()   # 想全部清掉就打開這行 / uncomment to clear all spots

while True:
    names = []
    for n, x, y in SPOTS:
        c = cam.color(n)                         # 這一點的顏色名稱 / colour name at the spot
        names.append(NAME.get(c, '?') if c else '?')
    print('左 left', names[0], '  中 centre', names[1], '  右 right', names[2])
    wait(300)
