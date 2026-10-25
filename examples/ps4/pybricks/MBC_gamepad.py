# MBC 樂高相容感應器：掌控者 PS2／PS4 遙控接收器 個別函式庫（SPIKE Prime 與 EV3 共用一份）
# 作者：MBC Robot　適用：Pybricks v4.1（SPIKE Prime／Robot Inventor；EV3 新韌體）
# 這個檔由 MBC_sensors.py 自動切出來，只含 掌控者 PS2／PS4 遙控接收器，檔案比較小、上傳到主機比較快。
#
# ！！把這個檔案 MBC_gamepad.py 跟你的程式一起匯入 Pybricks（放同一層，不要放子資料夾）！！
#
# 兩種函式庫二選一：
#   只接 掌控者 PS2／PS4 遙控接收器 → 用這個檔：    from MBC_gamepad import MBC_GAMEPAD
#   同一台車接兩款以上 MBC 感應器 → 改用統一函式庫 MBC_sensors.py：
#                                  from MBC_sensors import MBC_GAMEPAD
#   類別名稱、方法完全一樣，切換時只改 import 那一行。不要兩個都匯入。
#
#  孔位 port：
#    SPIKE：1~6 = A~F，也可以直接寫 Port.A ~ Port.F
#    EV3  ：1~4 = S1~S4，也可以直接寫 Port.S1 ~ Port.S4（主機自動判斷）
#
#  同步寫法（一般程式，while True + wait）：
#      from MBC_gamepad import MBC_GAMEPAD
#      pad = MBC_GAMEPAD(3)                  # 3 = C 埠
#      while True:
#          pad.update(); pad.pressed('CROSS')
#          wait(10)
#
#  多工寫法（run_task + async）：物件要在 run_task() 之前建立，每個方法都要加 await：
#      pad = MBC_GAMEPAD(3, True)            # True = 多工
#
#  拔線不會讓程式停：讀不到時回傳上一次的值（從來沒讀到過回傳 None），
#  第一次讀不到時印一行提醒；pad.sensor_ok() 可以查目前有沒有讀到。


# ════════════════════════════════════════════════════════════════

from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port
from pybricks.tools import StopWatch as _StopWatch

# EV3 的 Port 有 S1~S4，SPIKE 沒有（EV3 上 hubs 也有 PrimeHub 別名，所以不能用主機類別判斷）
_EV3 = hasattr(Port, 'S1')

_CACHE_MS = 10     # 快取時間
_RETRY_MS = 1000   # 沒有裝置時，多久重試建立一次
_watch = _StopWatch()


async def _aw(x):
    # 把一個值包成可 await 的物件（多工模式下，不用讀硬體的方法也要能 await）
    return x


def _port(p):
    # 孔位正規化：數字 → Port 物件；回傳 (Port, 顯示用名稱)
    if isinstance(p, int):
        n = ('S1', 'S2', 'S3', 'S4') if _EV3 else ('A', 'B', 'C', 'D', 'E', 'F')
        if p < 1 or p > len(n):
            raise ValueError('port 要填 1~%d' % len(n))
        return getattr(Port, n[p - 1]), n[p - 1]
    return p, str(p).split('.')[-1]


# ───── 位元組工具：Pybricks 讀到的是有號整數，先轉成無號再拆 ─────
def _lo(w):
    return int(w) & 0xFF


def _hi(w):
    return (int(w) >> 8) & 0xFF


def _u(w):
    return int(w) & 0xFFFF


def _bits(m, n):
    # n 個布林值：第 i 個 = 第 i 個位元是不是 1
    return [(m >> i) & 1 == 1 for i in range(n)]


def _jn(j):
    # 線段數 → "none"（沒線）／"line"（一條線）／"junction"（路口、岔路）
    return ('none', 'line', 'junction')[min(j, 2)]


def _sc(x):
    # EV3 測距者的公分值是 cm×100/120（0~100），換回公分 0~120
    return (int(x) * 120 + 50) // 100


def _nil(v):
    return None


# ───── 識別資料 ─────
# SPIKE 模式 8：[ID | hw0<<8, 協議版, 韌體版字, 更新序號]
# EV3   模式 5：[ID | hw0<<8, 韌體版字]
# 回傳 (id, hw0, 韌體版字)；韌體版字 = 主版×100＋次版
def _id8(v):
    w = _u(v[0])
    return (w & 0xFF, w >> 8, _u(v[2]))


def _fw8(v):
    f = _u(v[2])
    return (f // 100, f % 100)


def _id5(v):
    w = _u(v[0])
    return (w & 0xFF, w >> 8, v[1])


def _fw5(v):
    return (v[1] // 100, v[1] % 100)


# ════════════════════════════════════════════════════════════════
#  共用基底類別：讀取、快取、孔位、同步／多工、拔線容錯
#  每個產品類別只要寫兩張表：_S（SPIKE）、_E（EV3）
#    方法名稱 → (模式, 解碼函式[, 讀法])
#    讀法：0 = 10 ms 快取（預設）、1 = 一定重讀、2 = 有舊資料就用（掌控者 update() 的快照）
#  表裡沒有的方法，表示那台主機讀不到這個值（呼叫會丟出 ValueError）。
# ════════════════════════════════════════════════════════════════
class _MBC:
    _S = {}
    _E = {}

    def __init__(self, port, multitask=False):
        self._p, self._n = _port(port)
        self._a = bool(multitask)
        self._t = self._E if _EV3 else self._S
        self._d = None      # PUPDevice（第一次用到才建立）
        self._c = {}        # 模式 → (時間, 原始資料)
        self._bad = 0       # 1 = 目前讀不到（提醒已經印過）
        self._try = None    # 上一次嘗試建立裝置的時間
        if self._a:
            # 多工：run_task() 開始後就不能建立裝置，所以現在就建
            self._open()

    def _open(self):
        if self._d is None:
            t = _watch.time()
            if self._try is None or t - self._try >= _RETRY_MS:
                self._try = t
                try:
                    self._d = PUPDevice(self._p)
                except Exception as e:
                    self._warn(e)
        return self._d

    def _warn(self, e):
        if not self._bad:
            print('MBC: Port', self._n, '讀不到感應器，先回傳上一次的值:', e)
        self._bad = 1

    def _old(self, m, k):
        # 回傳 (可以直接用嗎, 舊資料)
        e = self._c.get(m)
        if e is None:
            return False, None
        return k == 2 or (k == 0 and _watch.time() - e[0] < _CACHE_MS), e[1]

    def _put(self, m, v):
        self._c[m] = (_watch.time(), v)
        self._bad = 0
        return v

    def _rd(self, m, k=0):
        ok, v = self._old(m, k)
        if ok:
            return v
        d = self._open()
        if d is not None:
            try:
                return self._put(m, d.read(m))
            except OSError as e:
                self._warn(e)
        return v

    async def _rda(self, m, k=0):
        ok, v = self._old(m, k)
        if ok:
            return v
        d = self._open()
        if d is not None:
            try:
                return self._put(m, await d.read(m))
            except OSError as e:
                self._warn(e)
        return v

    def _g(self, m, f, a=(), k=0):
        if self._a:
            return self._ga(m, f, a, k)
        v = self._rd(m, k)
        return None if v is None else f(v, *a)

    async def _ga(self, m, f, a, k):
        v = await self._rda(m, k)
        return None if v is None else f(v, *a)

    def _q(self, name, *a):
        x = self._t.get(name)
        if x is None:
            raise ValueError(name + '(): ' + ('EV3' if _EV3 else 'SPIKE') + ' 讀不到這個值')
        return self._g(x[0], x[1], a, x[2] if len(x) > 2 else 0)

    def _r(self, x):
        return _aw(x) if self._a else x

    def sensor_ok(self):
        """最近一次有沒有讀到感應器：True／False（拔線、沒接上 = False）"""
        if self._d is None:
            # 還沒讀過值（同步寫法第一次用到才建立裝置）：先試著建立，不然一開始會回 False
            self._open()
        return self._r(self._d is not None and not self._bad)

    def device_id(self):
        """(id, hw0, 韌體版字)，例如 (22, 8, 100)：ID 0x16、8 路、韌體 1.0（感應器韌體 2026-10 以後）"""
        return self._q('device_id')

    def firmware_version(self):
        """感應器韌體版本 (主版, 次版)，例如 (1, 0)"""
        return self._q('firmware_version')


# ════════════════════════════════════════════════════════════════
#  掌控者 PS2／PS4 遙控接收器
#  每一圈先 pad.update() 讀一次手把，之後 pressed()／stick() 都用這一次的資料（快照）。
#  SPIKE 模式 9：v0 低 = 方向鍵＋圖形鍵、v1 低 = 肩鍵＋功能鍵、v2~v5 = 搖桿 1~9、v6 = PSX 排列
#        模式 3：[已連線, 電量, 0]；模式 8：識別
#  EV3   模式 1：[方向鍵, LX, 圖形鍵, LY, 肩鍵, RX, 功能鍵, RY]（搖桿 0~99，換成 1~9）
#        模式 3：方向鍵 | 圖形鍵<<4 | 已連線<<8；模式 5：識別
# ════════════════════════════════════════════════════════════════
_BUTTONS = {
    'UP': 0, 'RIGHT': 1, 'DOWN': 2, 'LEFT': 3,
    'TRIANGLE': 4, 'CIRCLE': 5, 'CROSS': 6, 'SQUARE': 7,
    'L2': 8, 'R2': 9, 'L1': 10, 'R1': 11,
    'SELECT': 12, 'L3': 13, 'R3': 14, 'START': 15,
    # PS4／PS5 的叫法
    'SHARE': 12, 'CREATE': 12, 'OPTIONS': 15,
}
_STICKS = {'LX': 2, 'LY': 3, 'RX': 4, 'RY': 5}       # SPIKE 模式 9 第幾個
_STICKS_E = {'LX': 1, 'LY': 3, 'RX': 5, 'RY': 7}     # EV3 模式 1 第幾個

# 模式 9 第 6 個數字（button16）的「PSX 排列」：每個鍵是第幾個位元（PS2、PS4 相同）
#   位元 15~8：■ ✕ ● ▲ R1 L1 R2 L2；位元 7~0：左 下 右 上 START R3 L3 SELECT
PSX_BITS = {
    'SELECT': 0, 'L3': 1, 'R3': 2, 'START': 3,
    'UP': 4, 'RIGHT': 5, 'DOWN': 6, 'LEFT': 7,
    'L2': 8, 'R2': 9, 'L1': 10, 'R1': 11,
    'TRIANGLE': 12, 'CIRCLE': 13, 'CROSS': 14, 'SQUARE': 15,
    'SHARE': 0, 'CREATE': 0, 'OPTIONS': 3,
}
# 也可以直接用常數，例如 pad.buttons_psx() & PSX_CROSS
PSX_SELECT, PSX_L3, PSX_R3, PSX_START = 1 << 0, 1 << 1, 1 << 2, 1 << 3
PSX_UP, PSX_RIGHT, PSX_DOWN, PSX_LEFT = 1 << 4, 1 << 5, 1 << 6, 1 << 7
PSX_L2, PSX_R2, PSX_L1, PSX_R1 = 1 << 8, 1 << 9, 1 << 10, 1 << 11
PSX_TRIANGLE, PSX_CIRCLE, PSX_CROSS, PSX_SQUARE = 1 << 12, 1 << 13, 1 << 14, 1 << 15

_KIND = {1: 'PS2', 2: 'PS4'}


def _pad():
    def mk(m, bt, st, im, idf):
        return {
            'update': (m, _nil, 1),
            'buttons': (m, bt, 2),
            'pressed': (m, lambda v, n: (bt(v) >> _BUTTONS[n]) & 1 == 1, 2),
            'any_pressed': (m, lambda v: bt(v) != 0, 2),
            'stick': (m, st, 2),
            'stick_centered': (m, lambda v, n: st(v, n) - 5, 2),
            'device_id': (im, idf),
            'kind': (im, lambda v: _KIND.get(idf(v)[1], 'UNKNOWN')),
        }

    s = mk(9, lambda v: _lo(v[0]) | (_lo(v[1]) << 8), lambda v, n: _lo(v[_STICKS[n]]), 8, _id8)
    s.update({
        'buttons_psx': (9, lambda v: _u(v[6]), 2),
        'pressed_psx': (9, lambda v, n: (_u(v[6]) >> PSX_BITS[n]) & 1 == 1, 2),
        'connected': (3, lambda v: int(v[0]) == 1),
        'battery': (3, lambda v: int(v[1])),
        'firmware_version': (8, _fw8),
    })
    e = mk(1, lambda v: (v[0] & 15) | (v[2] & 15) << 4 | (v[4] & 15) << 8 | (v[6] & 15) << 12,
           lambda v, n: 1 + _lo(v[_STICKS_E[n]]) * 9 // 100, 5, _id5)
    e.update({
        'connected': (3, lambda v: (int(v[0]) >> 8) & 1 == 1),
        'firmware_version': (5, _fw5),
    })
    return s, e


def _name(d, name, what):
    if name not in d:
        raise ValueError('沒有這個' + what + '名稱：' + str(name))


class MBC_GAMEPAD(_MBC):
    """掌控者 PS2／PS4：pad = MBC_GAMEPAD(port, multitask=False)"""
    _S, _E = _pad()

    def update(self):
        """從接收器讀一次最新的按鍵和搖桿（每一圈迴圈開頭呼叫一次）"""
        return self._q('update')

    def buttons(self):
        """全部按鍵合成一個數字（0~65535）：沒按 = 0"""
        return self._q('buttons')

    def pressed(self, name):
        """這個鍵有沒有被按住：True／False。名稱例如 'CROSS'、'UP'、'L1'"""
        _name(_BUTTONS, name, '按鍵')
        return self._q('pressed', name)

    def any_pressed(self):
        """有沒有任何一個鍵被按住"""
        return self._q('any_pressed')

    def stick(self, name):
        """搖桿位置 1~9（5 = 中間）。名稱：LX、LY、RX、RY"""
        _name(_STICKS, name, '搖桿')
        return self._q('stick', name)

    def stick_centered(self, name):
        """搖桿位置減 5：-4~4（0 = 中間）"""
        _name(_STICKS, name, '搖桿')
        return self._q('stick_centered', name)

    def buttons_psx(self):
        """16 個鍵合成一個數字，用「PSX 排列」（見 PSX_BITS；只有 SPIKE）"""
        return self._q('buttons_psx')

    def pressed_psx(self, name):
        """用 PSX 排列判斷某個鍵有沒有被按住（只有 SPIKE）"""
        _name(PSX_BITS, name, '按鍵')
        return self._q('pressed_psx', name)

    def connected(self):
        """手把有沒有連上接收器：True／False（接收器韌體 2026-10 以後）"""
        return self._q('connected')

    def battery(self):
        """手把電量 0~100（PS4／PS5 才有；PS2 一直是 0；只有 SPIKE）"""
        return self._q('battery')

    def kind(self):
        """接收器種類：'PS2'、'PS4'；認不出來 = 'UNKNOWN'"""
        return self._q('kind')
