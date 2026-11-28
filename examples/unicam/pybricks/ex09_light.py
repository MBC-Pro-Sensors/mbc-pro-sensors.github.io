# 神攝手 × Pybricks（SPIKE）範例 9：補光燈 light()（命令 E2）——暗的地方開燈讀色，讀完關回去
# SharpShot × Pybricks (SPIKE) example 9: the lamp, light() (command E2): turn it on in the dark, then back off
# 相機模式：23 座標取色 ・③ 色相＋飽和＋明度（看明度 V 變亮）/ mode 23 Spot colour H+S+V (watch V go up)
# 相機韌體 2026-10-29 以後 / camera firmware 2026-10-29 or later
# 接線 Wiring：相機 → C 孔 / camera → port C
# 同一個資料夾要有 MBC_UniCam.py / MBC_UniCam.py must be imported too
#
# cam.light(亮度 0~100 %)：0＝關；cam.light()（不填）＝回到相機選單 Flash 的設定（出廠＝關）
# cam.light(0~100 %): 0 = off; cam.light() with nothing = back to the camera menu Flash setting (factory = off)
# 不存：相機重開機回選單設定 / not stored: back to the menu setting after a restart
from pybricks.hubs import PrimeHub
from pybricks.parameters import Port
from pybricks.tools import wait
from MBC_UniCam import UniCam

hub = PrimeHub()
cam = UniCam(Port.C, mode=23)

LAMP = 60          # 👉 可以改：補光燈亮度 0~100 % / lamp brightness

def show(text):
    p = cam.hsv(1)                               # 取樣點 1 的 (色相, 飽和, 明度) / (hue, sat, val) of spot 1
    print(text, '色相 hue', p[0], ' 飽和 sat', p[1], ' 明度 val', p[2]) if p else print(text, '讀不到 no camera')

show('關燈 lamp off  :')
r = cam.light(LAMP)                              # 0＝好了 / 0 = done
print('light(%d) →' % LAMP, r)
hub.display.number(LAMP)
wait(300)                                        # 等畫面亮起來（自動曝光）/ let the picture settle
show('開燈 lamp %d %%:' % LAMP)
r = cam.light()                                  # 回到選單設定 / back to the menu setting
print('light() →', r)
hub.display.off()
