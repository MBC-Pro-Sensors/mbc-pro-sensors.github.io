# ！！這個檔案是自動產生的，不要手改！！（MBC_laser2_obj_Lib.py）
# 內容 = 整份 MBC_sensors.py（MBC 共用函式庫）＋ 檔案最後的舊名稱，舊程式只要換掉這個檔就能繼續用。
# 新程式建議直接匯入 MBC_sensors.py。同一個程式同時匯入兩個舊檔名版本，會佔兩份空間（每份編譯後約 12 KB）。

# MBC 樂高相容感應器：Pybricks 共用函式庫（SPIKE Prime 與 EV3 共用一份）
# 作者：MBC Robot　日期：2026-10-24　版本：v1.0
# 適用：Pybricks v4.1.1（SPIKE Prime／Robot Inventor；EV3 新韌體）
#
# ！！把這個檔案 MBC_sensors.py 跟你的程式一起匯入 Pybricks（放同一層，不要放子資料夾）！！
# 舊檔名（MBC_line8_obj_Lib.py 等）是「這整份＋檔案最後的舊名稱」，由產生工具自動做出來，舊程式不用改。
#
# ════════════════════════════════════════════════════════════════
#  一個產品一個類別：
#    MBC_LINE8(port)       循行者 8 路
#    MBC_LINE16(port)      循行者 16 路
#    MBC_LASER2(port)      測距者 2 路（左、右雷射）
#    MBC_LASER8(port)      測距者 8 路（八方位）
#    MBC_GAMEPAD(port)     掌控者 PS2／PS4 遙控接收器
#    MBC_CAM_SCHOOL(port)  神攝手（學校老師創意賽版）
#    MBC_HUE12(port)       神攝手 WRO 版（HUE12）
#
#  孔位 port：
#    SPIKE：1~6 = A~F，也可以直接寫 Port.A ~ Port.F
#    EV3  ：1~4 = S1~S4，也可以直接寫 Port.S1 ~ Port.S4
#    主機是 SPIKE 還是 EV3 會自動判斷（看 Port 有沒有 S1），讀值方式自動換成那台主機的模式表。
#
#  同步寫法（一般程式，while True + wait）：
#      from MBC_sensors import MBC_LINE8
#      line8 = MBC_LINE8(3)                  # 3 = C 埠
#      while True:
#          print(line8.pos8(), line8.width())
#          wait(10)
#
#  多工寫法（run_task + async）：建立物件要放在 run_task() 之前（最外層），
#  每個方法都要加 await：
#      line8 = MBC_LINE8(3, True)            # True = 多工
#      async def main():
#          while True:
#              print(await line8.pos8())
#              await wait(10)
#      run_task(main())
#
#  10 ms 快取：同一個模式 10 ms 內重複呼叫只讀一次硬體（感應器本來就每 10 ms 更新一次）。
#
#  拔線容錯（Pybricks 4.1 起拔掉後再讀會丟出 OSError）：
#    - 讀不到時不會停止程式：回傳「上一次讀到的值」；從來沒讀到過就回傳 None。
#    - 第一次讀不到時在電腦畫面印一行提醒（同一次斷線只印一次）。
#    - 插回去之後下一次讀取就自動恢復；一開始就沒接上的話，同步模式每秒重試建立一次裝置。
#    - 多工模式的裝置只能在 run_task() 之前建立：開機時就沒接上，要重新執行程式。
#    - obj.sensor_ok() 可以查目前有沒有讀到（True／False）。
#
#  換模式會多花時間（SPIKE 幾十 ms、EV3 約 0.5 秒）：
#    device_id()、firmware_version()、毫米距離這類「別的模式」放在迴圈外，或偶爾呼叫就好。

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
#  循行者 8／16 路
#  SPIKE 模式 9（7 個 int16）：
#    v0~v3 = 光值（8 路：每路 1 byte 0~100；16 路：每路 4 bit 0~15）
#    v4 = 粗位置+N（低）、線寬（高）　v5 = 細位置+100（低）、線段數（高）　v6 = 位元圖
#  EV3：模式 0 = 粗位置；模式 1 = 光值（8 路 0~100；16 路兩路壓一格，十位 = 單數路、個位 = 雙數路，1~9）
#       模式 2 = [細位置, 線寬, 第 1 路光值, 最後一路光值]；模式 3 = 位元圖；模式 5 = 識別
#  方向：站在車後面向前看，線在左 = 負、線在右 = 正；第 1 路在最左邊。
# ════════════════════════════════════════════════════════════════
def _line(n):
    def ir(v):
        r = []
        for i in range(4):
            lo, hi = _lo(v[i]), _hi(v[i])
            r += [lo, hi] if n == 8 else [lo & 15, lo >> 4, hi & 15, hi >> 4]
        return r

    def ire(v):
        if n == 8:
            return [v[i] for i in range(8)]
        return [v[i // 2] // 10 if i % 2 == 0 else v[i // 2] % 10 for i in range(16)]

    def bs(v):
        return _lo(v[6]) if n == 8 else _u(v[6])

    def be(v):
        return int(v[0]) & (0xFF if n == 8 else 0xFFFF)

    def on(b):
        return [i + 1 for i in range(n) if b & (1 << i)]

    def gr(b):
        # 位元圖裡有幾段連續的 1（EV3 沒有送線段數，自己算）
        c = p = 0
        for i in range(n):
            x = (b >> i) & 1
            if x and not p:
                c += 1
            p = x
        return c

    def al(v):
        return {
            'ir_calibration': ir(v),
            'line_pos%d' % n: _lo(v[4]) - n,
            'line_width': _hi(v[4]),
            'line_pos100': _lo(v[5]) - 100,
            'bin_raw': bs(v),
            'on_sensors': on(bs(v)),
            'junction': _hi(v[5]),
            'junction_name': _jn(_hi(v[5])),
        }

    s = {
        'pos': (9, lambda v: _lo(v[4]) - n),
        'width': (9, lambda v: _hi(v[4])),
        'pos100': (9, lambda v: _lo(v[5]) - 100),
        'junction': (9, lambda v: _hi(v[5])),
        'junction_name': (9, lambda v: _jn(_hi(v[5]))),
        'bin_raw': (9, bs),
        'bitmap': (9, bs),
        'on_sensors': (9, lambda v: on(bs(v))),
        'ir_calibration': (9, ir),
        'ir_ch': (9, lambda v, c: ir(v)[c - 1]),
        'edge_l': (9, lambda v: ir(v)[0]),
        'edge_r': (9, lambda v: ir(v)[n - 1]),
        'read_all': (9, al),
        'device_id': (8, _id8),
        'firmware_version': (8, _fw8),
    }
    e = {
        'pos': (0, lambda v: v[0]),
        'pos100': (2, lambda v: v[0]),
        'width': (2, lambda v: v[1]),
        'edge_l': (2, lambda v: v[2]),
        'edge_r': (2, lambda v: v[3]),
        'ir_calibration': (1, ire),
        'ir_ch': (1, lambda v, c: ire(v)[c - 1]),
        'bin_raw': (3, be),
        'bitmap': (3, be),
        'on_sensors': (3, lambda v: on(be(v))),
        'junction': (3, lambda v: gr(be(v))),
        'junction_name': (3, lambda v: _jn(gr(be(v)))),
        'device_id': (5, _id5),
        'firmware_version': (5, _fw5),
    }
    return s, e


class _LINE(_MBC):
    _N = 8

    def pos100(self):
        """細位置 -100~+100（0 = 中間）"""
        return self._q('pos100')

    def width(self):
        """線寬：有幾路踩在線上（0 = 沒看到線）"""
        return self._q('width')

    def bin_raw(self):
        """位元圖：第 1 路 = bit0；1 = 那一路在線上"""
        return self._q('bin_raw')

    def bitmap(self):
        """同 bin_raw()（EV3 舊函式庫的名稱）"""
        return self._q('bitmap')

    def on_sensors(self):
        """踩在線上的是哪幾路，例如 [4, 5]"""
        return self._q('on_sensors')

    def junction(self):
        """線段數：0 = 沒線、1 = 一條線、2 以上 = 路口或岔路"""
        return self._q('junction')

    def junction_name(self):
        """線段數的名稱：none（沒線）／line（一條線）／junction（路口）"""
        return self._q('junction_name')

    def ir_calibration(self):
        """每一路的校準後光值（list）"""
        return self._q('ir_calibration')

    def ir_ch(self, ch):
        """第 ch 路的光值（ch = 1 是最左邊）"""
        if ch < 1 or ch > self._N:
            raise ValueError('ch must be 1~{}, got {}'.format(self._N, ch))
        return self._q('ir_ch', ch)

    def edge_l(self):
        """最左邊（第 1 路）的光值"""
        return self._q('edge_l')

    def edge_r(self):
        """最右邊（最後一路）的光值"""
        return self._q('edge_r')

    def read_all(self):
        """全部的值放進一個 dict，只讀一次硬體（只有 SPIKE）"""
        return self._q('read_all')



class MBC_LINE8(_LINE):
    """循行者 8 路：line8 = MBC_LINE8(port, multitask=False)"""
    _S, _E = _line(8)

    def pos8(self):
        """粗位置 -8~+8（0 = 中間，負 = 線在左）"""
        return self._q('pos')



class MBC_LINE16(_LINE):
    """循行者 16 路：line16 = MBC_LINE16(port, multitask=False)"""
    _N = 16
    _S, _E = _line(16)

    def pos16(self):
        """粗位置 -16~+16（0 = 中間，負 = 線在左）"""
        return self._q('pos')



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



#  測距者 8 路（方向 -8~+8：0 = 正前方，負 = CH1 側，正 = CH8 側；±8 = 沒有目標）
#  SPIKE 模式 9：v0~v3 = 8 顆 cm；v4 = 最近方向+8（低）、最近 cm（高）；v5 = 最空曠方向+8、cm；
#                v6 = 初始化 OK（低）、故障（高）
#        模式 6：[最近 mm, 正前方 mm, 最近方向]
#  EV3   模式 1：8 顆（cm×100/120）；模式 2：[最近方向, 最近', 最空曠方向, 最空曠']；
#        模式 3：正前方 mm；模式 5：[最近 mm, 最近方向]
def _laser8():
    def d(v):
        return [_lo(v[i // 2]) if i % 2 == 0 else _hi(v[i // 2]) for i in range(8)]

    def de(v):
        return [_sc(v[i]) for i in range(8)]

    def al(v):
        return {
            'distances': d(v),
            'min_direction': _lo(v[4]) - 8, 'min_distance': _hi(v[4]),
            'max_direction': _lo(v[5]) - 8, 'max_distance': _hi(v[5]),
            'init_status': _bits(_lo(v[6]), 8), 'fault_status': _bits(_hi(v[6]), 8),
        }

    s = {
        'distances': (9, d),
        'ch': (9, lambda v, c: d(v)[c - 1]),
        'min_direction': (9, lambda v: _lo(v[4]) - 8),
        'min_distance': (9, lambda v: _hi(v[4])),
        'max_direction': (9, lambda v: _lo(v[5]) - 8),
        'max_distance': (9, lambda v: _hi(v[5])),
        'init_status': (9, lambda v: _bits(_lo(v[6]), 8)),
        'fault_status': (9, lambda v: _bits(_hi(v[6]), 8)),
        'is_ok': (9, lambda v, c: _ok(v[6], c)),
        'read_all': (9, al),
        'nearest_mm': (6, lambda v: int(v[0])),
        'front_mm': (6, lambda v: int(v[1])),
        'nearest_dir': (6, lambda v: int(v[2])),
        'device_id': (8, _id8),
        'firmware_version': (8, _fw8),
    }
    e = {
        'distances': (1, de),
        'ch': (1, lambda v, c: _sc(v[c - 1])),
        'min_direction': (2, lambda v: int(v[0])),
        'min_distance': (2, lambda v: _sc(v[1])),
        'max_direction': (2, lambda v: int(v[2])),
        'max_distance': (2, lambda v: _sc(v[3])),
        'nearest_mm': (5, lambda v: int(v[0])),
        'nearest_dir': (5, lambda v: int(v[1])),
        'front_mm': (3, lambda v: int(v[0])),
    }
    return s, e


class MBC_LASER8(_MBC):
    """測距者 8 路：laser8 = MBC_LASER8(port, multitask=False)"""
    _S, _E = _laser8()

    def distances(self):
        """8 顆的距離 [cm]*8"""
        return self._q('distances')

    def ch(self, ch):
        """第 ch 顆（1~8）的距離 0~120 cm"""
        _ch(8, ch)
        return self._q('ch', ch)

    def min_direction(self):
        """最近目標的方向 -8~+8"""
        return self._q('min_direction')

    def min_distance(self):
        """最近目標的距離 0~120 cm"""
        return self._q('min_distance')

    def max_direction(self):
        """最空曠的方向 -7~+7"""
        return self._q('max_direction')

    def max_distance(self):
        """最空曠方向的距離 0~120 cm"""
        return self._q('max_distance')

    def is_ok(self, ch):
        """第 ch 顆開機成功而且目前沒有故障（只有 SPIKE）"""
        _ch(8, ch)
        return self._q('is_ok', ch)

    def init_status(self):
        """[bool]*8 開機初始化成功（只有 SPIKE）"""
        return self._q('init_status')

    def fault_status(self):
        """[bool]*8 目前故障（只有 SPIKE）"""
        return self._q('fault_status')

    def read_all(self):
        """全部的值放進一個 dict，只讀一次硬體（只有 SPIKE）"""
        return self._q('read_all')

    def nearest_mm(self):
        """最近目標的距離 0~1200 mm"""
        return self._q('nearest_mm')

    def front_mm(self):
        """正前方（第 4、5 顆平均）的距離 0~1200 mm"""
        return self._q('front_mm')

    def nearest_dir(self):
        """最近目標的方向 -8~+8"""
        return self._q('nearest_dir')



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



# ════════════════════════════════════════════════════════════════
#  舊版「函式寫法」用（例如 line_pos8()）：tools\build_legacy.py 產生的舊檔名版本才會用到。
#  記住目前選的孔；同一個孔不重複建立裝置。
# ════════════════════════════════════════════════════════════════
class _Free:
    def __init__(self, cls):
        self.c = cls
        self.o = None
        self.d = {}

    def init(self, port, multitask=False):
        o = self.c(port)
        o._a = bool(multitask)
        o._d = self.d.get(o._n)
        if o._open() is not None:
            self.d[o._n] = o._d
        self.o = o
        return _aw(None)

    def get(self):
        if self.o is None:
            raise RuntimeError("Sensor not found. Please connect the sensor to the correct port.")
        return self.o

    def pre(self, n):
        # EV3 舊函式庫：匯入時先把 1~n 孔都建好（run_task() 開始後不能再建）
        for p in range(1, n + 1):
            x, k = _port(p)
            try:
                self.d[k] = PUPDevice(x)
            except Exception:
                pass


# ════════════════════════════════════════════════════════════════
#  舊名稱相容（MBC_laser2_obj_Lib 的舊寫法；自動產生）
# ════════════════════════════════════════════════════════════════
_F = _Free(MBC_LASER2)


def laser2_init_port_multitask(port, multitask=False):
    # 初始化：選孔（同一個孔不重複建立裝置）；回傳可 await 的物件
    return _F.init(port, multitask)


def laser2_left():
    return _F.get().left()


def laser2_right():
    return _F.get().right()


def laser2_average():
    return _F.get().average()


def laser2_min_distance():
    return _F.get().min_distance()


def laser2_min_side():
    return _F.get().min_side()


def laser2_diff():
    return _F.get().diff()


def laser2_read_all():
    return _F.get().read_all()


def laser2_ch(ch):
    return _F.get().ch(ch)


def laser2_is_ok(ch):
    return _F.get().is_ok(ch)


def laser2_left_mm():
    return _F.get().left_mm()


def laser2_right_mm():
    return _F.get().right_mm()


def laser2_average_mm():
    return _F.get().average_mm()


def laser2_device_id():
    return _F.get().device_id()


def laser2_firmware_version():
    return _F.get().firmware_version()
