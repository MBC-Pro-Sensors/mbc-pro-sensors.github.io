# 循行者 8 路 × Pybricks：E7 5 秒自動校準／Pathfinder 8 × Pybricks: E7 5-second auto calibration
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

press_left('黑線放在感應器旁邊；按左鍵後 5 秒內，固定高度、平推來回掃過黑線 3 次（不要傾斜）／after LEFT, sweep across the black line 3 times in 5 s, fixed height')
line8.cmd(0xE7)                       # E7：5 秒內記下每一路最黑和最白的值（校準燈會亮）／record darkest and brightest for 5 s
wait(200)
print('校準中 calibrating…', '狀態', STATE[line8.cmd_state()])
ok = line8.wait_done(8000)            # 最多等 8 秒（校準要 5 秒）／wait up to 8 s
print('E7 自動校準：', RESULT[ok], '狀態', STATE[line8.cmd_state()])
wait(1000)

press_left('感應器整排放在白紙上看結果／put the sensor over white paper to check')
print('白紙光值（0～100）：', line8.ir_calibration())
wait(1000)
print('白紙上都在 70 以上就可以用；太低的那一端表示掃的時候那邊離紙太近，再做一次／70+ on white is OK; if one end is low, redo with a steady height')
