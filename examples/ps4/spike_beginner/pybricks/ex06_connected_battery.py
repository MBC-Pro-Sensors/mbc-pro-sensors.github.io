# 掌控者PS4 範例 6：連線狀態與電量（等手把連上才開始，螢幕顯示電量）
# 接線：掌控者PS4 接收器 → C 埠
# 只有 Pybricks 版：連線狀態和電量放在接收器的「模式 3」，官方 App 和積木網站讀不到；接收器韌體要 2026-10 以後
from pybricks.hubs import PrimeHub
from pybricks.parameters import Color, Port
from pybricks.tools import wait
from MBC_gamepad_Lib import MBC_GAMEPAD

hub = PrimeHub()
pad = MBC_GAMEPAD(Port.C)                    # 接收器插在 C 埠

hub.light.on(Color.RED)                      # 紅燈 = 手把還沒連上
while not pad.connected():                   # 等到手把連上（True）才往下
    wait(200)
hub.light.on(Color.GREEN)                    # 綠燈 = 連上了
hub.speaker.beep()
print('接收器種類', pad.kind(), ' 韌體', pad.firmware_version())   # 例如 PS4 (1, 0)

while True:
    if pad.connected():
        battery = pad.battery()              # 手把電量 0~100
        hub.display.number(min(battery, 99)) # 螢幕最多只能顯示兩位數
        hub.light.on(Color.GREEN)
        print('已連線  電量', battery, '%')
    else:                                    # 手把關機或離太遠 → 斷線
        hub.display.text('--')
        hub.light.on(Color.RED)
        print('斷線了')
    wait(500)
