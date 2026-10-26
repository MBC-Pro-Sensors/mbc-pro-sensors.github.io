# MBC 樂高相容感應器：神攝手 WRO 版（HUE12） 個別函式庫（SPIKE Prime 與 EV3 共用一份）
# 作者：MBC Robot　適用：Pybricks v4.1（SPIKE Prime／Robot Inventor；EV3 新韌體）
# 這個檔由 MBC_sensors.py 自動切出來，只含 神攝手 WRO 版（HUE12），檔案比較小、上傳到主機比較快。
#
# ！！把這個檔案 MBC_hue12.py 跟你的程式一起匯入 Pybricks（放同一層，不要放子資料夾）！！
#
# 兩種函式庫二選一：
#   只接 神攝手 WRO 版（HUE12） → 用這個檔：    from MBC_hue12 import MBC_HUE12
#   同一台車接兩款以上 MBC 感應器 → 改用統一函式庫 MBC_sensors.py：
#                                  from MBC_sensors import MBC_HUE12
#   類別名稱、方法完全一樣，切換時只改 import 那一行。不要兩個都匯入。
#
#  孔位 port：
#    SPIKE：1~6 = A~F，也可以直接寫 Port.A ~ Port.F
#    EV3  ：1~4 = S1~S4，也可以直接寫 Port.S1 ~ Port.S4（主機自動判斷）
#
#  同步寫法（一般程式，while True + wait）：
#      from MBC_hue12 import MBC_HUE12
#      cam = MBC_HUE12(3)                  # 3 = C 埠
#      while True:
#          cam.update(); cam.get_slot(1)
#          wait(10)
#
#  多工寫法（run_task + async）：物件要在 run_task() 之前建立，每個方法都要加 await：
#      cam = MBC_HUE12(3, True)            # True = 多工
#
#  拔線不會讓程式停：讀不到時回傳上一次的值（從來沒讀到過回傳 None），
#  第一次讀不到時印一行提醒；cam.sensor_ok() 可以查目前有沒有讀到。


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
#  神攝手 WRO 版（HUE12）：12 格顏色，SPIKE 和 EV3 都是模式 0 的 8 個 int32
#    cam.update()                     讀一次最新資料（成功 True、失敗 False）
#    name, hue, sat = cam.get_slot(1) 第 1 格：顏色名稱、色相、飽和度
#  色相：0~239 彩色、240~245 白（Wht）、246~250 黑（Blk）、255 = 這一格沒設定（NONE）
#  update() 讀不到時回傳 False，get_slot() 繼續回傳上一次的資料。
# ════════════════════════════════════════════════════════════════
_HUE_NAMES = ('Red', 'Org', 'Yel', 'Lim', 'Grn', 'Mnt', 'Cyn', 'Azr', 'Blu', 'Prp', 'Mag', 'Pnk')


class MBC_HUE12(_MBC):
    """神攝手 WRO 版：cam = MBC_HUE12(port, multitask=False)"""

    def __init__(self, port, multitask=False):
        _MBC.__init__(self, port, multitask)
        d = self._open()
        try:
            i = d.info()['id']
        except Exception:
            i = None
        print("Connected to ID:", i)
        self.slots = [(255, 0, "NONE")] * 12

    @property
    def device(self):
        # 舊函式庫的 self.device（PUPDevice；沒接上 = None）
        return self._d

    def _get_color_name(self, hue):
        # 色相數字 → 顏色名稱
        if hue == 255:
            return "NONE"
        if 246 <= hue <= 250:
            return "Blk"
        if 240 <= hue <= 245:
            return "Wht"
        if hue >= 230 or hue < 10:
            return "Red"
        if 10 <= hue < 230:
            return _HUE_NAMES[(hue - 10) // 20 + 1]
        return "Unk"

    def _dec(self, raw):
        if self._bad or not raw or len(raw) < 8:
            return False
        b = []
        for k in range(8):
            val = raw[k]
            b.extend([val & 0xFF, (val >> 8) & 0xFF, (val >> 16) & 0xFF])
        for i in range(12):
            h, s = b[i * 2], b[i * 2 + 1]
            self.slots[i] = (h, s, self._get_color_name(h))
        return True

    async def _upa(self):
        return self._dec(await self._rda(0, 1))

    def update(self):
        """讀一次最新資料：成功 True、讀不到 False（get_slot() 保留上一次的資料）"""
        if self._a:
            return self._upa()
        return self._dec(self._rd(0, 1))

    def get_slot(self, slot_id):
        """第 slot_id 格（1~12）：(顏色名稱, 色相, 飽和度)；超出範圍 = ("Err", 0, 0)"""
        if 1 <= slot_id <= 12:
            h, s, name = self.slots[slot_id - 1]
            r = (name, h, s)
        else:
            r = ("Err", 0, 0)
        return self._r(r)
