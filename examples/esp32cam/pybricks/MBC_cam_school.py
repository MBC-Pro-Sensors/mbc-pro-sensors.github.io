# MBC 樂高相容感應器：神攝手（學校老師創意賽版） 個別函式庫（SPIKE Prime 與 EV3 共用一份）
# 作者：MBC Robot　適用：Pybricks v4.1（SPIKE Prime／Robot Inventor；EV3 新韌體）
# 這個檔由 MBC_sensors.py 自動切出來，只含 神攝手（學校老師創意賽版），檔案比較小、上傳到主機比較快。
#
# ！！把這個檔案 MBC_cam_school.py 跟你的程式一起匯入 Pybricks（放同一層，不要放子資料夾）！！
#
# 兩種函式庫二選一：
#   只接 神攝手（學校老師創意賽版） → 用這個檔：    from MBC_cam_school import MBC_CAM_SCHOOL
#   同一台車接兩款以上 MBC 感應器 → 改用統一函式庫 MBC_sensors.py：
#                                  from MBC_sensors import MBC_CAM_SCHOOL
#   類別名稱、方法完全一樣，切換時只改 import 那一行。不要兩個都匯入。
#
#  孔位 port：
#    SPIKE：1~6 = A~F，也可以直接寫 Port.A ~ Port.F
#    EV3  ：1~4 = S1~S4，也可以直接寫 Port.S1 ~ Port.S4（主機自動判斷）
#
#  同步寫法（一般程式，while True + wait）：
#      from MBC_cam_school import MBC_CAM_SCHOOL
#      cam = MBC_CAM_SCHOOL(3)                  # 3 = C 埠
#      while True:
#          print(cam.hue())
#          wait(10)
#
#  多工寫法（run_task + async）：物件要在 run_task() 之前建立，每個方法都要加 await：
#      cam = MBC_CAM_SCHOOL(3, True)            # True = 多工
#
#  拔線不會讓程式停：讀不到時回傳上一次的值（從來沒讀到過回傳 None），
#  第一次讀不到時印一行提醒；cam.sensor_ok() 可以查目前有沒有讀到。


# ════════════════════════════════════════════════════════════════

from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port
from pybricks.tools import StopWatch as _StopWatch, wait as _wait

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
        self._sq = None     # 下命令：最後一個序號（還沒下過 = None）
        self._ck = None     # 　　　　最後一個 (命令碼, 參數…)
        self._pk = None     # 　　　　最後送出的 8 個值（重送用）
        self._ct = 0        # 　　　　最後送出的時間
        self._rs = False
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

    # ───── 下命令（只有 SPIKE；感應器韌體要支援命令：模式 8 v1 的 bit 8 = 1）─────
    #  送：PUPDevice.write(9, (序號, 命令碼, p1~p5))，命令碼與參數照 I2C 命令表
    #  回：模式 8 v3 = (感應器最後收下的序號 << 8) | 狀態（0 完成、1 執行中、2 失敗、3 拒絕）
    #  序號 1~255：同一個命令＋同樣參數沿用同一個序號（放在迴圈裡也只做一次），感應器看到同序號不會重做
    def cmd(self, code, *p, again=False):
        """下命令：code = 命令碼（例 0xE2），p = 參數（最多 5 個）。
        同一個命令＋同樣參數再呼叫不會重做；要再做一次寫 again=True。回傳這次的序號"""
        if self._a:
            return self._cmda(code, p, again)
        pk = self._cpk(code, p, again, self._rd(8, 1) if self._sq is None else None)
        if pk is not None:
            d = self._open()
            if d is not None:
                try:
                    d.write(9, pk)
                except OSError as e:
                    self._warn(e)
            self._c.pop(8, None)
        return self._sq

    async def _cmda(self, code, p, again):
        pk = self._cpk(code, p, again, (await self._rda(8, 1)) if self._sq is None else None)
        if pk is not None:
            d = self._open()
            if d is not None:
                try:
                    await d.write(9, pk)
                except OSError as e:
                    self._warn(e)
            self._c.pop(8, None)
        return self._sq

    def _cpk(self, code, p, again, v):
        # 算出要送的 7 個值；None = 不用送（同命令同參數，或還讀不到感應器）
        if _EV3:
            raise ValueError('cmd(): EV3 不能下命令 / EV3 cannot send commands')
        if len(p) > 5:
            raise ValueError('cmd(): 參數最多 5 個 / at most 5 parameters')
        k = (code,) + tuple(p)
        if k == self._ck and not again:
            return None
        if self._sq is None:
            if v is None:
                return None
            if len(v) < 4 or not (_u(v[1]) & 0x100):
                raise ValueError('cmd(): 感應器韌體太舊，不支援命令，請更新韌體 / sensor firmware too old for commands')
            self._sq = _hi(v[3])       # 新程式：從感應器最後收下的序號 +1 開始，才不會跟上一個程式撞號
        self._sq = self._sq % 255 + 1  # 1~255；0 = 不是命令
        self._ck = k
        self._pk = (self._sq, code) + tuple(p) + (0,) * (5 - len(p))
        self._ct = _watch.time()
        return self._pk

    def _cs(self, v):
        # 最後一個命令的狀態；感應器還沒收到（或沒下過命令）= None
        if self._sq is None or len(v) < 4 or _hi(v[3]) != self._sq:
            return None
        return _lo(v[3])

    def cmd_state(self):
        """最後一個命令的狀態：0 完成、1 執行中、2 失敗、3 拒絕；感應器還沒收到 = None"""
        return self._g(8, self._cs, (), 1)

    def _bz(self, v):
        if self._sq is None:
            return False
        s = self._cs(v)
        return s is None or s == 1

    def busy(self):
        """最後一個命令還沒做完（或感應器還沒收到）= True"""
        return self._g(8, self._bz, (), 1)

    def _wd(self, v, t0, to):
        # wait_done 的一步：True／False = 結束；None = 繼續等（self._rs = 要重送）
        self._rs = False
        if self._sq is None:
            return True
        s = None if v is None else self._cs(v)
        t = _watch.time()
        if s is not None and s != 1:
            return s == 0
        if t - t0 >= to:
            return False
        if s is None and t - self._ct >= 300:   # 300 ms 還沒收到：同一個序號重送（感應器不會重做）
            self._ct = t
            self._rs = True
        return None

    def wait_done(self, timeout=10000):
        """等最後一個命令做完：True = 完成；False = 失敗、拒絕或等超過 timeout 毫秒"""
        if self._a:
            return self._wda(timeout)
        t0 = _watch.time()
        while True:
            r = self._wd(self._rd(8, 1), t0, timeout)
            if r is not None:
                return r
            if self._rs and self._open() is not None:
                try:
                    self._d.write(9, self._pk)
                except OSError as e:
                    self._warn(e)
            _wait(20)

    async def _wda(self, timeout):
        t0 = _watch.time()
        while True:
            r = self._wd(await self._rda(8, 1), t0, timeout)
            if r is not None:
                return r
            if self._rs and self._open() is not None:
                try:
                    await self._d.write(9, self._pk)
                except OSError as e:
                    self._warn(e)
            await _wait(20)


# ════════════════════════════════════════════════════════════════
#  神攝手（學校老師創意賽版）
#  SPIKE 模式 9：8 個數字 [色相, 飽和度, 明度, 動態值, 色塊X, 色塊Y, 色塊寬, 色塊高]，
#                兩個一組放在 int16（低位元組在前）；v4 = 色塊面積（無號）
#        模式 6：[色塊X, 色塊Y, 面積]；模式 8：識別
#  EV3   模式 1：同樣 8 個數字（int8）；面積 = 寬 × 高
# ════════════════════════════════════════════════════════════════
def _byte(v, k):
    # 取出第 k 個數字（-128 ~ 127）：第 k//2 個整數，偶數 = 低位元組、奇數 = 高位元組
    w = v[k // 2]
    b = ((w >> 8) & 0xFF) if k % 2 else (w & 0xFF)
    return b - 256 if b > 127 else b


_CAM = ('hue', 'saturation', 'brightness', 'motion', 'blob_x', 'blob_y', 'blob_w', 'blob_h')


def _cam():
    def mk(m, ra):
        t = {'read_all': (m, ra), 'found': (m, lambda v: ra(v)[6] > 0)}
        for i in range(8):
            t[_CAM[i]] = (m, lambda v, i=i: ra(v)[i])
        return t

    s = mk(9, lambda v: [_byte(v, k) for k in range(8)])
    s.update({
        'area': (9, lambda v: _u(v[4])),
        'blob_xya': (6, lambda v: [int(v[0]), int(v[1]), int(v[2])]),
        'device_id': (8, _id8),
        'firmware_version': (8, _fw8),
    })
    e = mk(1, lambda v: [int(v[k]) for k in range(8)])
    e.update({
        'area': (1, lambda v: int(v[6]) * int(v[7])),
        'blob_xya': (1, lambda v: [int(v[4]), int(v[5]), int(v[6]) * int(v[7])]),
    })
    return s, e


class MBC_CAM_SCHOOL(_MBC):
    """神攝手學校版：cam = MBC_CAM_SCHOOL(port, multitask=False)"""
    _S, _E = _cam()

    def read_all(self):
        """一次讀 8 個數字：[色相, 飽和度, 明度, 動態值, 色塊X, 色塊Y, 色塊寬, 色塊高]"""
        return self._q('read_all')

    def hue(self):
        """色相 0~100（紅 0／100、黃約 17、綠約 33、青約 50、藍約 67、洋紅約 83）；-1 = 黑白灰"""
        return self._q('hue')

    def saturation(self):
        """飽和度 0~100"""
        return self._q('saturation')

    def brightness(self):
        """明度 0~100"""
        return self._q('brightness')

    def motion(self):
        """動態值 0~100（畫面變化越大數字越大）"""
        return self._q('motion')

    def blob_x(self):
        """色塊 X：左 0 ~ 右 98；沒找到是 0"""
        return self._q('blob_x')

    def blob_y(self):
        """色塊 Y：下 1 ~ 上 75；沒找到是 0"""
        return self._q('blob_y')

    def blob_w(self):
        """色塊寬 0~98；沒找到是 0"""
        return self._q('blob_w')

    def blob_h(self):
        """色塊高 0~73；沒找到是 0"""
        return self._q('blob_h')

    def found(self):
        """有沒有找到色塊（寬 > 0）"""
        return self._q('found')

    def area(self):
        """色塊面積（寬 × 高，0~7500）；沒找到是 0"""
        return self._q('area')

    def blob_xya(self):
        """一次讀 [色塊X, 色塊Y, 面積]；沒找到是 [0, 0, 0]"""
        return self._q('blob_xya')
