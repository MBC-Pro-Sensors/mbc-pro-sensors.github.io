# MBC 掌控者 PS2／PS4 遙控接收器：Pybricks 小函式庫（兩款共用）
# 作者：MBC Robot　日期：2026-10
# v1.1（2026-10）：新增 connected()、battery()（模式 3）、buttons_psx()／pressed_psx()（模式 9 第 6 個數字，PSX 排列）、
#                 device_id()／kind()／firmware_version()（模式 8）。原本的 update()／pressed()／stick() 一個字都沒改。
#                 模式 3、8 要接收器韌體 2026-10 以後的版本；舊韌體讀到的會是別的東西。
#
# ！！把這個檔案 MBC_gamepad_Lib.py 跟範例一起匯入 Pybricks ！！
#
# 接收器要是「SPIKE 版韌體」。這個函式庫讀模式 9（7 個數字）：
#   第 0 個：方向鍵＋圖形鍵（上 1、右 2、下 4、左 8、▲ 16、● 32、✕ 64、■ 128）
#   第 1 個：肩鍵＋功能鍵（L2 1、R2 2、L1 4、R1 8、SELECT 16、L3 32、R3 64、START 128）
#   第 2~5 個：左搖桿 X、左搖桿 Y、右搖桿 X、右搖桿 Y（1~9，5 = 中間；往上、往右推數字變大）
#   第 6 個：16 個鍵的「PSX 排列」（PS2、PS4 相同，跟 PS2 手把原始資料一樣）→ buttons_psx()
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
#
# 連線與電量（接收器韌體 2026-10 以後）：
#   pad.connected()              # 手把有沒有連上：True／False
#   pad.battery()                # 手把電量 0~100（PS4／PS5）；PS2 沒有電量資訊，一直是 0
#   pad.kind()                   # 接收器種類：'PS2' 或 'PS4'
#   pad.device_id()              # (id, hw0, 韌體版字)，例如 (66, 2, 100)：ID 0x42、2 = PS4、韌體 1.0
#   pad.firmware_version()       # (主版, 次版)，例如 (1, 0)
#   ★ 這幾個會讓接收器暫時切到別的模式（幾十 ms），下一次 update() 再切回來；放在迴圈外或偶爾呼叫就好。
# 搖桿名稱：LX 左搖桿左右、LY 左搖桿上下、RX 右搖桿左右、RY 右搖桿上下

from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port
from pybricks.tools import StopWatch as _StopWatch

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

# 第 6 個數字（button16）的「PSX 排列」：每個鍵是第幾個位元（PS2、PS4 相同）
#   位元 15~8：■ ✕ ● ▲ R1 L1 R2 L2；位元 7~0：左 下 右 上 START R3 L3 SELECT
PSX_BITS = {
    'SELECT': 0, 'L3': 1, 'R3': 2, 'START': 3,
    'UP': 4, 'RIGHT': 5, 'DOWN': 6, 'LEFT': 7,
    'L2': 8, 'R2': 9, 'L1': 10, 'R1': 11,
    'TRIANGLE': 12, 'CIRCLE': 13, 'CROSS': 14, 'SQUARE': 15,
    # PS4／PS5 的叫法
    'SHARE': 0, 'CREATE': 0, 'OPTIONS': 3,
}
# 也可以直接用常數，例如 pad.buttons_psx() & PSX_CROSS
PSX_SELECT, PSX_L3, PSX_R3, PSX_START = 1 << 0, 1 << 1, 1 << 2, 1 << 3
PSX_UP, PSX_RIGHT, PSX_DOWN, PSX_LEFT = 1 << 4, 1 << 5, 1 << 6, 1 << 7
PSX_L2, PSX_R2, PSX_L1, PSX_R1 = 1 << 8, 1 << 9, 1 << 10, 1 << 11
PSX_TRIANGLE, PSX_CIRCLE, PSX_CROSS, PSX_SQUARE = 1 << 12, 1 << 13, 1 << 14, 1 << 15

_STATUS_MODE = 3      # [已連線 0/1, 電量 0~100, 0]
_ID_MODE = 8          # [ID 字, 協議版, 韌體版字, 更新序號]
_CACHE_MS = 10
_watch = _StopWatch()


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

    # ───── 以下是 v1.1（2026-10）新增；用自己的快取，不動上面讀模式 9 的 update() ─────
    def _read_mode(self, mode):
        # 讀別的模式：10 ms 內重複呼叫只讀一次硬體（每個模式各自一份）
        try:
            c = self._mcache
        except AttributeError:
            c = self._mcache = {}
        now = _watch.time()
        e = c.get(mode)
        if e is None or (now - e[0]) >= _CACHE_MS:
            e = (now, self._dev.read(mode))
            c[mode] = e
        return e[1]

    def buttons_psx(self):
        """16 個鍵合成一個數字，但用「PSX 排列」（跟 PS2 手把原始資料、接收器 I2C 0x12 一樣；PS2、PS4 相同）。
        位元 15~8 = ■ ✕ ● ▲ R1 L1 R2 L2，位元 7~0 = 左 下 右 上 START R3 L3 SELECT（見 PSX_BITS）。
        一般請用 buttons()／pressed()；這個是給要跟 Arduino／mBlock 程式對照的人用的。"""
        return int(self._data()[6]) & 0xFFFF

    def pressed_psx(self, name):
        """用 PSX 排列判斷某個鍵有沒有被按住（結果跟 pressed() 一樣，只是查另一個數字）"""
        if name not in PSX_BITS:
            raise ValueError('沒有這個按鍵名稱：' + str(name))
        return ((self.buttons_psx() >> PSX_BITS[name]) & 1) == 1

    def connected(self):
        """手把有沒有連上接收器：True（有）或 False（沒有）。模式 3 的第 0 個數字。"""
        return int(self._read_mode(_STATUS_MODE)[0]) == 1

    def battery(self):
        """手把電量 0~100。PS4／PS5 才有；PS2 的手把沒有電量資訊，一直是 0。模式 3 的第 1 個數字。"""
        return int(self._read_mode(_STATUS_MODE)[1])

    def device_id(self):
        """(id, hw0, 韌體版字)：id = 0x42（66），hw0 = 1（PS2 接收器）或 2（PS4／PS5 接收器），韌體版字 = 主版×100＋次版。模式 8。"""
        v = self._read_mode(_ID_MODE)
        w = int(v[0]) & 0xFFFF
        return (w & 0xFF, (w >> 8) & 0xFF, int(v[2]) & 0xFFFF)

    def kind(self):
        """接收器種類：'PS2'、'PS4'（PS4／PS5 共用）；認不出來回傳 'UNKNOWN'"""
        hw0 = self.device_id()[1]
        return {1: 'PS2', 2: 'PS4'}.get(hw0, 'UNKNOWN')

    def firmware_version(self):
        """接收器韌體版本 (主版, 次版)，例如 (1, 0)"""
        fw = self.device_id()[2]
        return (fw // 100, fw % 100)
