# 掌控者PS2 範例 2：搖桿數值（螢幕顯示左搖桿上下的位置）
# 接線：掌控者PS2 接收器 → C 埠
from pybricks.hubs import PrimeHub
from pybricks.parameters import Port
from pybricks.tools import wait
from MBC_gamepad_Lib import MBC_GAMEPAD

hub = PrimeHub()
pad = MBC_GAMEPAD(Port.C)       # 接收器插在 C 埠

while True:
    pad.update()                # 先讀一次手把
    y = pad.stick('LY')         # 左搖桿上下：1（拉到最下）~ 5（中間）~ 9（推到最上）
    hub.display.number(y)       # 主機螢幕顯示這個數字
    print('LX', pad.stick('LX'), 'LY', y, 'RX', pad.stick('RX'), 'RY', pad.stick('RY'))   # 電腦畫面印出 4 個方向
    wait(100)
