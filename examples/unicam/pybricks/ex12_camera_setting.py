# 神攝手 × Pybricks（SPIKE）範例 12：白色校正＋鏡頭設定（white_cal 命令 E9、camera_setting 命令 E8）
# SharpShot × Pybricks (SPIKE) example 12: white calibration + camera settings (white_cal E9, camera_setting E8)
# 相機模式：13 顏色追蹤 ・③ 標準（任何模式都可以）/ mode 13 (any mode works)
# 相機韌體 2026-10-29 以後 / camera firmware 2026-10-29 or later
# 準備：白紙蓋滿相機畫面中央 / cover the centre of the picture with white paper
# 接線 Wiring：相機 → C 孔 / camera → port C
# 同一個資料夾要有 MBC_UniCam.py / MBC_UniCam.py must be imported too
#
# cam.white_cal()：白色校正（約 3 秒），換場地、換燈光時做一次；每次叫都會重做
# cam.white_cal(): white calibration (about 3 s), once per new place or light; runs again on every call
# cam.camera_setting(項目, 值)：項目照相機 MENU → Setup 的順序 / items in the camera Setup menu order
#   0 亮度 Brightness −2~2   1 對比 Contrast −2~2   2 飽和度 Saturation −2~2   3 AE Level −2~2
#   4 增益上限 GainCeil 0~6   5 白平衡 WB Mode 0~5   6 AWB 0/1   7 自動曝光 AutoExp 0/1
#   8 特效 Effect 0~6   9 翻轉 Flip 0/1   10 鏡像 Mirror 0/1   11 Flash 0/1   12 Light Hz 0/1
# 回 0 好了、2 失敗（超出範圍、或相機在待機）；兩個都存進相機，斷電後還在（改完 3 秒內別斷電）
# 0 done, 2 failed (out of range, or camera in standby); both are stored in the camera (keep power on 3 s after)
from pybricks.hubs import PrimeHub
from pybricks.parameters import Port
from pybricks.tools import wait
from MBC_UniCam import UniCam

hub = PrimeHub()
cam = UniCam(Port.C, mode=13)

for n in (3, 2, 1):                              # 倒數 3 秒，把白紙擺好 / 3 s to place the white paper
    hub.display.number(n)
    wait(1000)
r = cam.white_cal()
print('white_cal() →', r, '（0＝好了 done）')
r = [cam.camera_setting(0, 1),                   # 👉 可以改：亮度 +1 / brightness +1
     cam.camera_setting(2, 1)]                   # 👉 可以改：飽和度 +1（顏色更鮮豔）/ saturation +1
print('camera_setting →', r)
if r != [0, 0]:
    print('有設定沒成功：值超出範圍，或相機在待機（先 power(0)）/ out of range, or the camera is in standby')
hub.speaker.beep()
