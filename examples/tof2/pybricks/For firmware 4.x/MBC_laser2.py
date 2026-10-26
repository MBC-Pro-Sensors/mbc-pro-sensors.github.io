# MBC 樂高相容感應器：測距者 2 路 個別函式庫（SPIKE Prime 與 EV3 共用一份）
# 作者：MBC Robot　適用：Pybricks v4.1（SPIKE Prime／Robot Inventor；EV3 新韌體）
# 這個檔由 MBC_sensors.py 自動切出來，只含 測距者 2 路，檔案比較小、上傳到主機比較快。
#
# ！！把這個檔案 MBC_laser2.py 跟你的程式一起匯入 Pybricks（放同一層，不要放子資料夾）！！
#
# 兩種函式庫二選一：
#   只接 測距者 2 路 → 用這個檔：    from MBC_laser2 import MBC_LASER2
#   同一台車接兩款以上 MBC 感應器 → 改用統一函式庫 MBC_sensors.py：
#                                  from MBC_sensors import MBC_LASER2
#   類別名稱、方法完全一樣，切換時只改 import 那一行。不要兩個都匯入。
#
#  孔位 port：
#    SPIKE：1~6 = A~F，也可以直接寫 Port.A ~ Port.F
#    EV3  ：1~4 = S1~S4，也可以直接寫 Port.S1 ~ Port.S4（主機自動判斷）
#
#  同步寫法（一般程式，while True + wait）：
#      from MBC_laser2 import MBC_LASER2
#      laser2 = MBC_LASER2(3)                  # 3 = C 埠
#      while True:
#          print(laser2.left())
#          wait(10)
#
#  多工寫法（run_task + async）：物件要在 run_task() 之前建立，每個方法都要加 await：
#      laser2 = MBC_LASER2(3, True)            # True = 多工
#
#  拔線不會讓程式停：讀不到時回傳上一次的值（從來沒讀到過回傳 None），
#  第一次讀不到時印一行提醒；laser2.sensor_ok() 可以查目前有沒有讀到。


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
            raise ValueError('port 要填 1~%d / port must be 1~%d' % (len(n), len(n)))
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
            print('MBC: Port', self._n, '讀不到感應器，先回傳上一次的值 / no sensor, using the last value:', e)
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
            raise ValueError(name + '(): ' + ('EV3' if _EV3 else 'SPIKE') + ' 讀不到這個值 / cannot read this value')
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
#  測距者共用：距離單位「公分」0~120（120 = 沒有目標或超過 120 cm）
# ════════════════════════════════════════════════════════════════
def _side(a, b):
    return -1 if a < b else (1 if b < a else 0)


def _ok(m, c):
    # 第 c 顆：開機初始化成功（m 低 byte）而且目前沒有故障（m 高 byte）
    return (_lo(m) >> (c - 1)) & 1 == 1 and (_hi(m) >> (c - 1)) & 1 == 0


def _lr(m, p):
    # p(v) = (左 cm, 右 cm)
    return {
        'left': (m, lambda v: p(v)[0]),
        'right': (m, lambda v: p(v)[1]),
        'ch': (m, lambda v, c: p(v)[c - 1]),
        'average': (m, lambda v: (p(v)[0] + p(v)[1]) >> 1),
        'min_distance': (m, lambda v: min(p(v))),
        'min_side': (m, lambda v: _side(*p(v))),
        'diff': (m, lambda v: p(v)[0] - p(v)[1]),
    }


def _ch(n, ch):
    if ch < 1 or ch > n:
        raise ValueError('ch must be 1~{}, got {}'.format(n, ch))


#  測距者 2 路
#  SPIKE 模式 9：v0 = 左 cm（低）、右 cm（高）；v1 = 初始化 OK（低）、故障（高）（bit0 = 左、bit1 = 右）
#        模式 6：[左 mm, 右 mm, 平均 mm]
#  EV3   模式 5：[左 mm, 右 mm]（公分由 mm ÷ 10 算出）
def _laser2():
    def p(v):
        return (_lo(v[0]), _hi(v[0]))

    def al(v):
        l, r = p(v)
        return {
            'left': l, 'right': r, 'average': (l + r) >> 1, 'min_distance': min(l, r),
            'min_side': _side(l, r), 'diff': l - r,
            'init_status': _bits(_lo(v[1]), 2), 'fault_status': _bits(_hi(v[1]), 2),
        }

    s = _lr(9, p)
    s.update({
        'init_status': (9, lambda v: _bits(_lo(v[1]), 2)),
        'fault_status': (9, lambda v: _bits(_hi(v[1]), 2)),
        'is_ok': (9, lambda v, c: _ok(v[1], c)),
        'read_all': (9, al),
        'left_mm': (6, lambda v: int(v[0])),
        'right_mm': (6, lambda v: int(v[1])),
        'average_mm': (6, lambda v: int(v[2])),
        'device_id': (8, _id8),
        'firmware_version': (8, _fw8),
    })
    e = _lr(5, lambda v: (int(v[0]) // 10, int(v[1]) // 10))
    e.update({
        'left_mm': (5, lambda v: int(v[0])),
        'right_mm': (5, lambda v: int(v[1])),
        'average_mm': (5, lambda v: (int(v[0]) + int(v[1])) >> 1),
    })
    return s, e


class MBC_LASER2(_MBC):
    """測距者 2 路：laser2 = MBC_LASER2(port, multitask=False)。CH1 = 左、CH2 = 右"""
    _S, _E = _laser2()

    def left(self):
        """左雷射距離 0~120 cm"""
        return self._q('left')

    def right(self):
        """右雷射距離 0~120 cm"""
        return self._q('right')

    def ch(self, ch):
        """第 ch 個雷射的距離（1 = 左、2 = 右）"""
        if ch < 1 or ch > 2:
            raise ValueError('ch must be 1~2 (1=left, 2=right), got {}'.format(ch))
        return self._q('ch', ch)

    def average(self):
        """左右平均距離 0~120 cm"""
        return self._q('average')

    def min_distance(self):
        """左右比較近的那一個 0~120 cm"""
        return self._q('min_distance')

    def min_side(self):
        """-1 = 左邊比較近、+1 = 右邊比較近、0 = 一樣近"""
        return self._q('min_side')

    def diff(self):
        """左減右 -120~+120（正 = 右邊比較近）"""
        return self._q('diff')

    def is_ok(self, ch):
        """第 ch 個雷射開機成功而且目前沒有故障（只有 SPIKE）"""
        if ch < 1 or ch > 2:
            raise ValueError('ch must be 1~2 (1=left, 2=right), got {}'.format(ch))
        return self._q('is_ok', ch)

    def init_status(self):
        """[左, 右] 開機初始化成功（只有 SPIKE）"""
        return self._q('init_status')

    def fault_status(self):
        """[左, 右] 目前故障（只有 SPIKE）"""
        return self._q('fault_status')

    def read_all(self):
        """全部的值放進一個 dict，只讀一次硬體（只有 SPIKE）"""
        return self._q('read_all')

    def left_mm(self):
        """左雷射距離 0~1200 mm"""
        return self._q('left_mm')

    def right_mm(self):
        """右雷射距離 0~1200 mm"""
        return self._q('right_mm')

    def average_mm(self):
        """左右平均距離 0~1200 mm"""
        return self._q('average_mm')
