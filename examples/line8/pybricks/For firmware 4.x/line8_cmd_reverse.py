# 循行者 8 路 × Pybricks：E9 左右反向／Pathfinder 8 × Pybricks: E9 swap left and right
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

press_left('黑線放在感應器左邊（站在車後往前看）／put the line on the LEFT side (seen from behind the robot)')
print('原本 normal：位置 pos =', line8.pos8(), '（線在左＝負 / left = negative）')
wait(1000)

line8.cmd(0xE9, 1)                    # E9 1：左右反過來（感應器反著裝時用；存在 RAM，重開機回 0）／swap left and right (RAM only)
ok = line8.wait_done()
print('E9 1 左右反向：', RESULT[ok], '狀態', STATE[line8.cmd_state()])
wait(300)
print('反向後 swapped：位置 pos =', line8.pos8(), '（正負號反過來 / sign flipped）')
wait(1000)

line8.cmd(0xE9, 0)                    # E9 0：恢復正常／back to normal
ok = line8.wait_done()
print('E9 0 恢復：', RESULT[ok], '狀態', STATE[line8.cmd_state()])
wait(300)
print('恢復後 normal：位置 pos =', line8.pos8())
wait(1000)

line8.cmd(0xE9, 5)                    # 參數只能 0 或 1：送 5 會回「失敗」／only 0 or 1 is allowed: 5 returns "failed"
ok = line8.wait_done()
print('E9 5（錯的參數 wrong value）：', RESULT[ok], '狀態', STATE[line8.cmd_state()])
wait(1000)
