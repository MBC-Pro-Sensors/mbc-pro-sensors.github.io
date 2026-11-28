# 神攝手 × Pybricks（SPIKE）範例 10：用程式教色、清掉目標（teach 命令 E3、clear 命令 E4）
# SharpShot × Pybricks (SPIKE) example 10: teach and clear targets from the program (teach E3, clear E4)
# 相機模式：13 顏色追蹤 ・③ 標準 / mode 13 Colour tracking, standard
# 相機韌體 2026-10-29 以後 / camera firmware 2026-10-29 or later
# 準備：把要教的顏色（色卡、球）放在相機畫面「正中央」/ put the colour in the CENTRE of the picture
# 接線 Wiring：相機 → C 孔 / camera → port C
# 同一個資料夾要有 MBC_UniCam.py / MBC_UniCam.py must be imported too
#
# cam.teach(目標 1~16)：用畫面中央的顏色登錄成這個目標（同在相機上按住 OK 教色），每次叫都會重教
# cam.teach(target 1~16): the colour in the centre becomes this target (like holding OK on the camera); teaches every call
# cam.clear(目標 1~16)：清掉這個目標（SPIKE 不能一次全部清）/ clears one target (SPIKE cannot clear all at once)
# 回 0 成功、2 失敗（中央完全分不出顏色）、None 舊韌體或沒接；教的結果存進相機，斷電後還在
# 0 ok, 2 failed (no colour in the centre), None old firmware / unplugged; stored in the camera (kept after power-off)
from pybricks.hubs import PrimeHub
from pybricks.parameters import Port
from pybricks.tools import wait
from MBC_UniCam import UniCam

hub = PrimeHub()
cam = UniCam(Port.C, mode=13)

TARGET = 1         # 👉 可以改：教成第幾個目標 1~16 / which target to teach
OLD = 2            # 👉 可以改：要清掉的舊目標 / an old target to clear

for n in (3, 2, 1):                              # 倒數 3 秒，把顏色擺到中央 / 3 s countdown
    hub.display.number(n)
    wait(1000)
r = cam.teach(TARGET)
if r == 0:
    print('教色成功 taught target', TARGET)
    hub.speaker.beep()
elif r == 2:
    print('教色失敗：中央看不出顏色，換個顏色或開補光燈 / teach failed: nothing in the centre')
else:
    print('沒有回應：相機韌體太舊或沒接 / no answer: old firmware or unplugged', r)
print('clear(%d) →' % OLD, cam.clear(OLD))     # 清掉不要的舊目標 / clear an old target

while True:                                      # 看剛教的目標在哪裡 / where is the new target?
    p = cam.xy(TARGET)
    if p is None or p == (0, 0):
        hub.display.char('-')
    else:
        hub.display.number(p[0])                 # 螢幕顯示 X / show X
    wait(100)
