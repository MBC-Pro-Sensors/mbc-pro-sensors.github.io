# 掌控者PS4 範例 3：按鍵控制燈和聲音
# 接線：掌控者PS4 接收器 → C 埠
# ▲ 綠燈、● 紅燈、✕ 藍燈、■ 粉紅燈（跟手把上的顏色一樣），都沒按就關燈；L1 低音嗶、R1 高音嗶
from pybricks.hubs import PrimeHub
from pybricks.parameters import Color, Port
from pybricks.tools import wait
from MBC_gamepad_Lib import MBC_GAMEPAD

hub = PrimeHub()
pad = MBC_GAMEPAD(Port.C)               # 接收器插在 C 埠

while True:
    pad.update()                        # 先讀一次手把
    if pad.pressed('TRIANGLE'):         # ▲
        hub.light.on(Color.GREEN)
    elif pad.pressed('CIRCLE'):         # ●
        hub.light.on(Color.RED)
    elif pad.pressed('CROSS'):          # ✕
        hub.light.on(Color.BLUE)
    elif pad.pressed('SQUARE'):         # ■
        hub.light.on(Color.MAGENTA)
    else:                               # 4 個都沒按
        hub.light.off()

    if pad.pressed('L1'):
        hub.speaker.beep(262, 100)      # 低音 Do（262 Hz），響 0.1 秒
    if pad.pressed('R1'):
        hub.speaker.beep(523, 100)      # 高音 Do（523 Hz）
    wait(50)
