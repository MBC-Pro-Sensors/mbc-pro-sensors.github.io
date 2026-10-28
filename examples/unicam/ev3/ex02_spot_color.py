#!/usr/bin/env pybricks-micropython
# 神攝手 × EV3 範例 2：座標取色判斷顏色
# Sharpshooter Vision × EV3 example 2: what colour is at the spot?
# 需要把相機選單切到 相機模式 2 座標取色 ・子選項 ③ 色相＋飽和＋明度 ＝模式編號 23（EV3 給 2 點，原始值）
# Camera menu: camera mode 2 Spot colour, option 3 Hue + sat + value = mode number 23 (2 spots, exact values)
# 軟體 Software：Pybricks v4 或 樂高官方 EV3 MicroPython v2
# 準備：取樣點 S1、S2 的位置看相機螢幕上標 1、2 的十字 / spots S1, S2 are the crosses on the camera screen
# 接線 Wiring：相機 → 1 號孔 / camera → port 1
from pybricks.hubs import EV3Brick
from pybricks.parameters import Port, Color
from pybricks.tools import wait
from MBC_UniCam_EV3 import UniCam, color_name

ev3 = EV3Brick()
cam = UniCam(Port.S1, mode=23)                   # 填了 mode 就不用先讀模式 0 / giving mode skips one mode switch

NAME = {'red': '紅', 'orange': '橙', 'yellow': '黃', 'green': '綠', 'cyan': '青',
        'blue': '藍', 'purple': '紫', 'white': '白／灰', 'black': '黑'}
LIGHT = {'red': Color.RED, 'orange': Color.ORANGE, 'yellow': Color.YELLOW, 'green': Color.GREEN}
LOW_SAT = 15      # 👉 可以改：飽和度比這低就當成白／灰 / below this saturation = white / grey

while True:
    names = []
    for point in (1, 2):
        p = cam.hsv(point)                       # (色相碼, 飽和度, 明度) / (hue, sat, val)
        if p is None:
            names.append(None)
            continue
        n = color_name(p[0])
        if p[1] < LOW_SAT and n != 'black':
            n = 'white'
        names.append(n)
        print('S%d 色相碼 hue %d 飽和 sat %d 明度 val %d → %s %s' % (point, p[0], p[1], p[2], NAME[n], n))
    if names[0] in LIGHT:                        # EV3 的燈只有紅、綠、橙、黃 / EV3 light: red, green, orange, yellow
        ev3.light.on(LIGHT[names[0]])
    else:
        ev3.light.off()
    if names[0] is not None and names[0] == names[1]:
        print('兩點一樣顏色 / both spots are the same colour')
    wait(300)
