# MBC 掌控者 PS2／PS4 遙控接收器：Pybricks 小函式庫（兩款共用）
# 作者：MBC Robot　日期：2026-10
#
# ！！把這個檔案 MBC_gamepad_Lib.py 跟範例一起匯入 Pybricks ！！
#
# 接收器要是「SPIKE 版韌體」。這個函式庫讀模式 9（7 個數字）：
#   第 0 個：方向鍵＋圖形鍵（上 1、右 2、下 4、左 8、▲ 16、● 32、✕ 64、■ 128）
#   第 1 個：肩鍵＋功能鍵（L2 1、R2 2、L1 4、R1 8、SELECT 16、L3 32、R3 64、START 128）
#   第 2~5 個：左搖桿 X、左搖桿 Y、右搖桿 X、右搖桿 Y（1~9，5 = 中間；往上、往右推數字變大）
#   第 6 個：PS2 和 PS4 排列不同，這個函式庫不用它
#
# 用法：
#   from MBC_gamepad_Lib import MBC_GAMEPAD
#   pad = MBC_GAMEPAD(Port.C)        # 接收器插在 C 埠（也可以寫 3）
#   while True:
#       pad.update()                 # 每一圈先讀一次手把
#       if pad.pressed('CROSS'):     # ✕ 鍵有沒有被按住？
#           ...
#       y = pad.stick('LY')          # 左搖桿上下：1~9，5 = 中間
#
# 按鍵名稱（大寫英文）：
#   UP 上、RIGHT 右、DOWN 下、LEFT 左
#   TRIANGLE ▲、CIRCLE ●、CROSS ✕、SQUARE ■
#   L1、R1、L2、R2、L3（左搖桿按下去）、R3（右搖桿按下去）
#   SELECT（PS4 叫 SHARE、PS5 叫「建立」CREATE）、START（PS4／PS5 叫 OPTIONS）
#   PS 鍵和觸控板讀不到。
# 搖桿名稱：LX 左搖桿左右、LY 左搖桿上下、RX 右搖桿左右、RY 右搖桿上下

from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

_PORTS = {1: Port.A, 2: Port.B, 3: Port.C, 4: Port.D, 5: Port.E, 6: Port.F}

# 每個按鍵在「16 位元按鍵值」裡是第幾個位元
_BUTTONS = {
    'UP': 0, 'RIGHT': 1, 'DOWN': 2, 'LEFT': 3,
    'TRIANGLE': 4, 'CIRCLE': 5, 'CROSS': 6, 'SQUARE': 7,
    'L2': 8, 'R2': 9, 'L1': 10, 'R1': 11,
    'SELECT': 12, 'L3': 13, 'R3': 14, 'START': 15,
    # PS4／PS5 的叫法
    'SHARE': 12, 'CREATE': 12, 'OPTIONS': 15,
}

# 每支搖桿在讀到的 7 個數字裡是第幾個
_STICKS = {'LX': 2, 'LY': 3, 'RX': 4, 'RY': 5}


def _lo(word):
    # 取一個數字的低 8 位元（0~255）
    return int(word) & 0xFF


class MBC_GAMEPAD:
    def __init__(self, port):
        if isinstance(port, int):
            port = _PORTS[port]
        self._dev = PUPDevice(port)
        self._v = None

    def update(self):
        """從接收器讀一次最新的按鍵和搖桿（每一圈迴圈開頭呼叫一次）"""
        self._v = self._dev.read(9)

    def _data(self):
        if self._v is None:
            self.update()
        return self._v

    def buttons(self):
        """全部按鍵合成一個數字（0~65535）：沒按 = 0，每個鍵有自己的數值，同時按就相加"""
        v = self._data()
        return _lo(v[0]) | (_lo(v[1]) << 8)

    def pressed(self, name):
        """這個鍵有沒有被按住？回傳 True（有）或 False（沒有）"""
        if name not in _BUTTONS:
            raise ValueError('沒有這個按鍵名稱：' + str(name))
        return ((self.buttons() >> _BUTTONS[name]) & 1) == 1

    def any_pressed(self):
        """有沒有任何一個鍵被按住"""
        return self.buttons() != 0

    def stick(self, name):
        """搖桿位置：1~9，5 = 中間"""
        if name not in _STICKS:
            raise ValueError('沒有這個搖桿名稱：' + str(name))
        return _lo(self._data()[_STICKS[name]])

    def stick_centered(self, name):
        """搖桿位置減 5：-4~4，0 = 中間"""
        return self.stick(name) - 5
