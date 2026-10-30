# 循行者 8 路 × Pybricks：黑白校準（E2 黑、E3 白）／Pathfinder 8 × Pybricks: black / white calibration (E2 black, E3 white)
# 需要：MBC_sensors.py 統一函式庫（先上傳到 Pybricks Code，跟這個程式放在一起）／upload MBC_sensors.py to Pybricks Code first
# 接線：循行者 → C 孔／Wiring: sensor → port C
# 操作：照輸出區提示擺好，按主機「左鍵」進下一步；中間鍵照常是停止程式／follow the hints and press LEFT; CENTER still stops the program
# 下命令的寫法：line8.cmd(命令碼, 參數) 送出 → line8.wait_done() 等它做完（True＝完成）→ line8.cmd_state() 看狀態（0 完成、1 執行中、2 失敗、3 拒絕）
# How to send a command: line8.cmd(code, param) → line8.wait_done() (True = done) → line8.cmd_state() (0 done, 1 busy, 2 failed, 3 rejected)
from pybricks.hubs import PrimeHub
from pybricks.parameters import Button, Port
from pybricks.tools import wait

from MBC_sensors import MBC_LINE8

hub = PrimeHub()
line8 = MBC_LINE8(Port.C)      # 循行者 8 路接 C 孔／Pathfinder 8 on port C

RESULT = {True: '完成 done', False: '沒有完成 not done'}
STATE = {0: '完成 done', 1: '執行中 busy', 2: '失敗（參數不對）failed', 3: '拒絕（沒有這個命令或正在校準）rejected', None: '讀不到感應器 no sensor'}


def press_left(text):
    """印提示，等使用者按「左鍵」再放開／print a hint and wait for the LEFT button"""
    print('>>> ' + text + '（好了按左鍵 / press LEFT when ready）')
    while Button.LEFT not in hub.buttons.pressed():
        wait(10)
    while Button.LEFT in hub.buttons.pressed():
        wait(10)

press_left('感應器整排 8 顆都放在黑色上、高度固定／put all 8 channels over black, fixed height')
line8.cmd(0xE2)                       # E2：把現在的讀值記成「黑」（約 50 ms，存進 EEPROM，斷電還在）／store current reading as black
ok = line8.wait_done()
print('E2 黑色校準：', RESULT[ok], '狀態', STATE[line8.cmd_state()])
print('黑色光值（0～100，越小越黑）：', line8.ir_calibration())
wait(1000)

press_left('感應器整排放在白紙上、高度固定／put all channels over white paper, fixed height')
line8.cmd(0xE3)                       # E3：把現在的讀值記成「白」／store current reading as white
ok = line8.wait_done()
print('E3 白色校準：', RESULT[ok], '狀態', STATE[line8.cmd_state()])
print('白紙光值（0～100，越大越白）：', line8.ir_calibration())
wait(1000)
print('校準完成：黑色上應該都接近 0、白紙上都在 80 以上／black should be near 0, white 80 or more')
