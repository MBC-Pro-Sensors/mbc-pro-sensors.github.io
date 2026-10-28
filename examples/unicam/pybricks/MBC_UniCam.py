# MBC 神攝手通用版 v1：Pybricks 函式庫（SPIKE Prime／Robot Inventor）
# MBC Sharpshooter Vision Universal v1: Pybricks library (SPIKE Prime / Robot Inventor)
# 作者 Author：MBC Robot　日期 Date：2026-10-28　版本 Version：v1.0（v1 尚未上機驗證 / not yet hardware-verified）
# 適用 For：Pybricks v4.1（code.pybricks.com）
#
# ！！把 MBC_UniCam.py 跟你的程式一起匯入 Pybricks（放同一層）！！
# !! Import MBC_UniCam.py into Pybricks next to your program (same folder) !!
#
# ════════════════════════════════════════════════════════════════
#  用法 Usage：
#      from MBC_UniCam import UniCam
#      cam = UniCam(Port.C, mode=13)      # mode = 相機模式 ×10 ＋ 子選項（要跟相機選單一樣）
#      x, y = cam.xy(1)                   # 色 1 的位置（像素）/ colour 1 position (pixels)
#
#  mode（模式編號 mode number）＝ 相機模式 camera mode × 10 ＋ 子選項 option：
#      11 顏色追蹤 最多色 tracking-max   12 多色 multi   13 標準 XY＋面積 std   14 精細 XYWH fine
#      21 座標取色 色相 spot H   22 色相＋飽和 H+S   23 色相＋飽和＋明度 H+S+V
#      30 照順序給色號 colour order
#      41 循線 黑線 black line   42 白線 white line   43 色線 colour line
#  SPIKE 讀不到相機目前的模式，所以 mode 一定要跟相機選單切的一樣。
#  SPIKE cannot read the camera's current mode: mode must match the camera menu.
#
#  座標 Coordinates：像素 X 0~159（左→右）、Y 0~119（上→下）；(0, 0)＝沒有目標 / no target.
#  色相碼 Hue code：0~240 色相（紅 0/240、黃 40、綠 80、青 120、藍 160、紫 200）；
#                   241~248 白／灰 white/grey；249~255 黑 black。
#
#  讀模式 9（14 byte，每個 byte 一個值）；數量（模式 11／12）才讀模式 6；裝置 ID 讀模式 8。
#  Reads mode 9 (14 bytes, one value per byte); counts in modes 11/12 read mode 6; device ID = mode 8.
#  換模式要多等約 30 ms：count()／colors_found() 在模式 11、12 不要每圈都叫。
#
#  拔線 Unplugged：不會停止程式，回傳上一次讀到的值；從來沒讀到過回傳 None。cam.sensor_ok() 查狀態。
#  多工 Multitask：UniCam(Port.C, 13, True)，物件要在 run_task() 之前建立，每個方法都要 await。
# ════════════════════════════════════════════════════════════════

from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port
from pybricks.tools import StopWatch as _StopWatch

_CACHE_MS = 10
_RETRY_MS = 1000
_watch = _StopWatch()
MODES = (11, 12, 13, 14, 21, 22, 23, 30, 41, 42, 43)
_NCOL = {11: 14, 12: 7, 13: 4, 14: 2}     # 這個模式給幾色 / colours per mode
_NPT = {21: 14, 22: 7, 23: 4}              # 給幾點 / spots per mode


def _port(p):
    if isinstance(p, int):
        n = ('A', 'B', 'C', 'D', 'E', 'F')
        if p < 1 or p > 6:
            raise ValueError('port 要填 1~6 / port must be 1~6')
        return getattr(Port, n[p - 1]), n[p - 1]
    return p, str(p).split('.')[-1]


async def _aw(x):
    return x


def _bytes(v, n):
    # int16 清單 → n 個無號 byte（低位元組在前）/ int16 list -> n unsigned bytes (low byte first)
    b = []
    for w in v:
        w = int(w) & 0xFFFF
        b.append(w & 0xFF)
        b.append(w >> 8)
    return b[:n]


def _s8(b):
    # 有號 byte（循線偏移、角度、左右緣）/ signed byte
    return b - 256 if b > 127 else b


def color_name(h):
    """色相碼 0~255 → 顏色名稱 / hue code -> colour name:
    'red' 'orange' 'yellow' 'green' 'cyan' 'blue' 'purple' 'white' 'black'（None＝沒有 / none）"""
    if h is None:
        return None
    if h > 248:
        return 'black'
    if h > 240:
        return 'white'
    if h < 12 or h >= 220:
        return 'red'
    if h < 28:
        return 'orange'
    if h < 56:
        return 'yellow'
    if h < 100:
        return 'green'
    if h < 140:
        return 'cyan'
    if h < 180:
        return 'blue'
    return 'purple'


# ───── 解碼函式：v = 模式 9 讀到的 7 個 int16 / decoders: v = 7 int16 of mode 9 ─────
def _xy(v, m, c):
    b = _bytes(v, 14)
    if m == 11:
        g = b[c - 1]
        if not g:
            return (0, 0)
        return (((g - 1) % 16) * 10 + 5, ((g - 1) // 16) * 10 + 5)    # 格子中心 ±5 像素 / cell centre ±5 px
    k = {12: 2, 13: 3, 14: 6}[m] * (c - 1)
    return (b[k], b[k + 1])


def _cell(v, m, c):
    g = _bytes(v, 14)[c - 1]
    return (0, 0) if not g else ((g - 1) % 16 + 1, (g - 1) // 16 + 1)


def _area(v, m, c):
    b = _bytes(v, 14)
    return b[3 * c - 1] if m == 13 else b[6 * c - 1]


def _size(v, m, c):
    b = _bytes(v, 14)
    return (b[6 * c - 4], b[6 * c - 3])


def _count9(v, m, c):
    b = _bytes(v, 14)
    if m == 13:
        x = b[12 + (c - 1) // 2]
        return x >> 4 if c % 2 else x & 15
    return b[6 * c - 2]


def _count6(v, m, c):
    b = _bytes(v, 6)
    if m == 11:
        bits = 0
        for i in range(6):
            bits |= b[i] << (8 * i)
        return (bits >> (3 * (c - 1))) & 7 if c else (bits >> 42) & 15
    if c == 0:
        return b[3] & 15
    x = b[(c - 1) // 2]
    return x >> 4 if c % 2 else x & 15


def _found14(v, m, c):
    return _bytes(v, 14)[12]


def _hsv(v, m, i):
    b = _bytes(v, 14)
    if m == 21:
        return (b[i - 1], None, None)
    if m == 22:
        return (b[2 * i - 2], b[2 * i - 1], None)
    return (b[3 * i - 3], b[3 * i - 2], b[3 * i - 1])


def _order(v, m, a):
    b = _bytes(v, 14)
    return b[:min(13, b[13])]


def _total(v, m, a):
    return _bytes(v, 14)[13]


def _line(v, m, a):
    b = _bytes(v, 14)
    s = _s8
    return (s(b[0]), s(b[1]), s(b[2]), s(b[3]), b[4], b[5], b[6], b[7], s(b[8]), s(b[9]), b[10], b[11], b[12], b[13])


class UniCam:
    """神攝手通用版 v1 / Sharpshooter Vision Universal v1: cam = UniCam(port, mode=13, multitask=False)"""

    def __init__(self, port, mode=13, multitask=False):
        if mode not in MODES:
            raise ValueError('mode 要填 %s / mode must be one of %s' % (MODES, MODES))
        self._p, self._n = _port(port)
        self._m = mode
        self._a = bool(multitask)
        self._d = None
        self._c = {}
        self._bad = 0
        self._try = None
        if self._a:
            self._open()

    # ───── 讀取、快取、拔線容錯（跟 MBC_sensors.py 同一套做法）/ read, cache, unplug-safe ─────
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

    def _old(self, m):
        e = self._c.get(m)
        if e is None:
            return False, None
        return _watch.time() - e[0] < _CACHE_MS, e[1]

    def _put(self, m, v):
        self._c[m] = (_watch.time(), v)
        self._bad = 0
        return v

    def _rd(self, m):
        ok, v = self._old(m)
        if ok:
            return v
        d = self._open()
        if d is not None:
            try:
                return self._put(m, d.read(m))
            except OSError as e:
                self._warn(e)
        return v

    async def _rda(self, m):
        ok, v = self._old(m)
        if ok:
            return v
        d = self._open()
        if d is not None:
            try:
                return self._put(m, await d.read(m))
            except OSError as e:
                self._warn(e)
        return v

    def _g(self, m, f, a=None):
        if self._a:
            return self._ga(m, f, a)
        v = self._rd(m)
        return None if v is None else f(v, self._m, a)

    async def _ga(self, m, f, a):
        v = await self._rda(m)
        return None if v is None else f(v, self._m, a)

    def _r(self, x):
        return _aw(x) if self._a else x

    def _need(self, name, ok):
        if self._m not in ok:
            raise ValueError('%s(): 模式 %d 沒有這個值，要用模式 %s / not available in mode %d, use mode %s'
                             % (name, self._m, ok, self._m, ok))

    def _col(self, c, n):
        if not 1 <= c <= n:
            raise ValueError('第幾色／第幾點要填 1~%d / colour or spot must be 1~%d' % (n, n))

    # ───── 共用 / common ─────
    def mode_number(self):
        """建構時設定的模式編號（11~43）/ the mode number given to UniCam()"""
        return self._r(self._m)

    def sensor_ok(self):
        """最近一次有沒有讀到感應器 / did the last read succeed: True／False"""
        if self._d is None:
            self._open()
        return self._r(self._d is not None and not self._bad)

    def device_id(self):
        """模式 8：(位址 0x4C, hw0, 韌體版字)，通用版 hw0＝3 / mode 8: (0x4C, hw0, firmware word); universal hw0 = 3"""
        return self._g(8, lambda v, m, a: (int(v[0]) & 0xFF, (int(v[0]) >> 8) & 0xFF, int(v[2]) & 0xFFFF))

    def is_unicam(self):
        """是不是通用版（模式 8 的 hw0＝3）/ is this the universal version (hw0 = 3)"""
        return self._g(8, lambda v, m, a: (int(v[0]) & 0xFFFF) == 0x034C)

    def firmware_version(self):
        """韌體版本 (主版, 次版)，例如 (1, 0) / firmware (major, minor)"""
        return self._g(8, lambda v, m, a: ((int(v[2]) & 0xFFFF) // 100, (int(v[2]) & 0xFFFF) % 100))

    # ───── 相機模式 1 顏色追蹤（11~14）/ camera mode 1 colour tracking ─────
    def max_color(self):
        """這個模式給幾色：11→14、12→7、13→4、14→2 / colours available in this mode"""
        self._need('max_color', (11, 12, 13, 14))
        return self._r(_NCOL[self._m])

    def xy(self, color):
        """色 color 的位置 (X 0~159, Y 0~119)，(0, 0)＝沒找到；模式 11 是格子中心（±5 像素）
        position of colour (pixels); (0, 0) = not found; mode 11 gives the grid-cell centre (±5 px)"""
        self._need('xy', (11, 12, 13, 14))
        self._col(color, _NCOL[self._m])
        return self._g(9, _xy, color)

    def x(self, color):
        """色 color 的 X（0＝沒找到）/ X of colour (0 = not found)"""
        self._need('x', (11, 12, 13, 14))
        self._col(color, _NCOL[self._m])
        return self._g(9, lambda v, m, c: _xy(v, m, c)[0], color)

    def y(self, color):
        """色 color 的 Y（上＝小；0＝沒找到）/ Y of colour (top = small; 0 = not found)"""
        self._need('y', (11, 12, 13, 14))
        self._col(color, _NCOL[self._m])
        return self._g(9, lambda v, m, c: _xy(v, m, c)[1], color)

    def found(self, color):
        """有沒有找到色 color / is colour found: True／False"""
        self._need('found', (11, 12, 13, 14))
        self._col(color, _NCOL[self._m])
        return self._g(9, lambda v, m, c: _xy(v, m, c)[0] > 0, color)

    def cell(self, color):
        """模式 11：16×12 格的格子 (1~16, 1~12)，(0, 0)＝沒找到 / mode 11: grid cell"""
        self._need('cell', (11,))
        self._col(color, 14)
        return self._g(9, _cell, color)

    def area(self, color):
        """模式 13、14：面積（佔畫面 %，0＝沒找到）/ modes 13, 14: area in % of the picture"""
        self._need('area', (13, 14))
        self._col(color, _NCOL[self._m])
        return self._g(9, _area, color)

    def size(self, color):
        """模式 14：(寬, 高) 像素 / mode 14: (width, height) in pixels"""
        self._need('size', (14,))
        self._col(color, 2)
        return self._g(9, _size, color)

    def count(self, color):
        """色 color 有幾塊：模式 11 是 0~7（7＝7 塊以上，讀模式 6）、12 讀模式 6（0~15）、13／14 是 0~15
        number of blobs of colour: mode 11 0~7 (7 = 7 or more, mode 6), 12 mode 6 (0~15), 13/14 0~15"""
        self._need('count', (11, 12, 13, 14))
        self._col(color, _NCOL[self._m])
        if self._m in (11, 12):
            return self._g(6, _count6, color)
        return self._g(9, _count9, color)

    def colors_found(self):
        """16 個目標色裡找到幾色（模式 11、12 讀模式 6；模式 14）/ how many of the 16 colours are found"""
        self._need('colors_found', (11, 12, 14))
        if self._m == 14:
            return self._g(9, _found14)
        return self._g(6, _count6, 0)

    # ───── 相機模式 2 座標取色（21~23）/ camera mode 2 spot colour ─────
    def max_point(self):
        """這個模式給幾點：21→14、22→7、23→4 / spots available in this mode"""
        self._need('max_point', (21, 22, 23))
        return self._r(_NPT[self._m])

    def hue(self, point):
        """取樣點 point 的色相碼 0~255 / hue code of spot"""
        self._need('hue', (21, 22, 23))
        self._col(point, _NPT[self._m])
        return self._g(9, lambda v, m, i: _hsv(v, m, i)[0], point)

    def sat(self, point):
        """飽和度 0~100（模式 22、23）/ saturation"""
        self._need('sat', (22, 23))
        self._col(point, _NPT[self._m])
        return self._g(9, lambda v, m, i: _hsv(v, m, i)[1], point)

    def val(self, point):
        """明度 0~100（模式 23）/ value (brightness)"""
        self._need('val', (23,))
        self._col(point, 4)
        return self._g(9, lambda v, m, i: _hsv(v, m, i)[2], point)

    def hsv(self, point):
        """(色相碼, 飽和度, 明度)；這個模式沒給的是 None / (hue, sat, val); None if not in this mode"""
        self._need('hsv', (21, 22, 23))
        self._col(point, _NPT[self._m])
        return self._g(9, _hsv, point)

    def color(self, point):
        """取樣點的顏色名稱（color_name(hue)）/ colour name of spot"""
        self._need('color', (21, 22, 23))
        self._col(point, _NPT[self._m])
        return self._g(9, lambda v, m, i: color_name(_hsv(v, m, i)[0]), point)

    # ───── 相機模式 3 照順序給色號（30）/ camera mode 3 colour order ─────
    def order(self):
        """由左到右每塊的色相碼（最多 13 塊）/ hue codes of the blocks from left to right (up to 13)"""
        self._need('order', (30,))
        return self._g(9, _order)

    def total(self):
        """畫面裡總共幾塊（可能比 13 多）/ total number of blocks seen (may be more than 13)"""
        self._need('total', (30,))
        return self._g(9, _total)

    # ───── 相機模式 4 循線（41~43）/ camera mode 4 line following ─────
    def line_all(self):
        """14 個值：(近, 中, 遠偏移, 角度, 近線寬, 有線旗標, 路口類型, 路口距離, 左緣, 右緣, 遠線寬, 標記色號, 標記條數, 離線方向)
        14 values: (near, mid, far, angle, width, flags, junction, junction dist, left, right, far width, marker, markers, lost)"""
        self._need('line_all', (41, 42, 43))
        return self._g(9, _line)

    def _li(self, name, i):
        self._need(name, (41, 42, 43))
        return self._g(9, lambda v, m, a: _line(v, m, a)[i])

    def near(self):
        """近處偏移 −100~100（線在左＝負）/ near offset (line on the left = negative)"""
        return self._li('near', 0)

    def mid(self):
        """中段偏移 −100~100 / middle offset"""
        return self._li('mid', 1)

    def far(self):
        """遠處偏移 −100~100 / far offset"""
        return self._li('far', 2)

    def angle(self):
        """線角度 −90~90 / line angle"""
        return self._li('angle', 3)

    def width(self):
        """近處線寬 % 0~100 / near line width %"""
        return self._li('width', 4)

    def flags(self):
        """有線旗標：近 1＋中 2＋遠 4（0＝都沒看到線）/ line seen: near 1 + mid 2 + far 4"""
        return self._li('flags', 5)

    def junction(self):
        """路口類型 0 無 1 十字 2 T 3 左 4 右 5 盡頭 / junction: 0 none 1 cross 2 T 3 left 4 right 5 end"""
        return self._li('junction', 6)

    def junction_dist(self):
        """路口距離 % 0~100 / junction distance %"""
        return self._li('junction_dist', 7)

    def edges(self):
        """近處線 (左緣, 右緣) −100~100 / near line (left edge, right edge)"""
        self._need('edges', (41, 42, 43))
        return self._g(9, lambda v, m, a: _line(v, m, a)[8:10])

    def far_width(self):
        """遠處線寬 % 0~100 / far line width %"""
        return self._li('far_width', 10)

    def marker(self):
        """(穿過的彩色標記線色號 0~8, 累計條數 0~99) / (marker colour 0~8, marker count 0~99)"""
        self._need('marker', (41, 42, 43))
        return self._g(9, lambda v, m, a: _line(v, m, a)[11:13])

    def lost(self):
        """最後離線方向 0 有線 1 左 2 右 / last lost side: 0 on line, 1 left, 2 right"""
        return self._li('lost', 13)
