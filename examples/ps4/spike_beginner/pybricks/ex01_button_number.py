# 掌控者PS4 範例 1：按鍵測試（螢幕顯示按鍵數字）
# 接線：掌控者PS4 接收器 → C 埠
from pybricks.hubs import PrimeHub
from pybricks.parameters import Port
from pybricks.tools import wait
from MBC_gamepad_Lib import MBC_GAMEPAD

hub = PrimeHub()
pad = MBC_GAMEPAD(Port.C)       # 接收器插在 C 埠

while True:
    pad.update()                # 先讀一次手把
    n = pad.buttons()           # 全部按鍵合成一個數字：沒按 = 0，上 = 1、右 = 2、下 = 4、左 = 8…
    hub.display.text(str(n))    # 主機螢幕捲動顯示這個數字
    print(n)                    # 電腦畫面也印出來
    wait(100)                   # 等 0.1 秒再讀下一次
