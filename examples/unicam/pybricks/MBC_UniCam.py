# ！！這個檔案是自動產生的，不要手改！！（正本：85_共用資源\MBC_Pybricks\src\MBC_UniCam.py）
# !! Auto-generated, do not edit by hand (master: 85_共用資源\MBC_Pybricks\src\MBC_UniCam.py) !!

# MBC 神攝手（韌體 v1）：Pybricks 函式庫（SPIKE Prime／Robot Inventor）
# MBC SharpShot Vision Camera Sensor (firmware v1): Pybricks library (SPIKE Prime / Robot Inventor)
# 作者 Author：MBC Robot　日期 Date：2026-10-28　版本 Version：v1.0（2026-10-31 SPIKE 上機驗證通過 / hardware-verified on SPIKE 2026-10-31）
# 適用 For：Pybricks v4.1（code.pybricks.com）
#
# ！！把 MBC_UniCam.py 跟你的程式一起匯入 Pybricks（放同一層）！！
# !! Import MBC_UniCam.py into Pybricks next to your program (same folder) !!
#
# ════════════════════════════════════════════════════════════════
#  用法 Usage：
#      from MBC_UniCam import UniCam
#      cam = UniCam(Port.C, mode=13)      # mode = 相機模式 ×10 ＋ 子選項（建立時自動把相機切到這個模式）
#      x, y = cam.xy(1)                   # 色 1 的位置（像素）/ colour 1 position (pixels)
#
#  mode（模式編號 mode number）＝ 相機模式 camera mode × 10 ＋ 子選項 option：
#      11 顏色追蹤 最多色 tracking-max   12 多色 multi   13 標準 XY＋面積 std   14 精細 XYWH fine
#      21 座標取色 色相 spot H   22 色相＋飽和 H+S   23 色相＋飽和＋明度 H+S+V
#      30 照順序給色號 colour order
#      41 循線 黑線 black line   42 白線 white line   43 色線 colour line
#      51 物體偵測 物體清單 object list   52 物體詳細 object detail
#  建立物件時自動送命令把相機切到 mode（韌體 2026-10-29 起）；舊韌體送不了，mode 要跟相機選單切的一樣。
#  The camera is switched to mode automatically (firmware 2026-10-29+); with older firmware mode must match the camera menu.
#  其他命令 Commands：cam.set_mode(51)、light(50)、teach(1)、clear(1)、target_range()、shape_tolerance()、
#      target_shape()、camera_setting()、white_cal()、power(0/1/2)；cmd()／busy()／wait_done() 送任意命令。
#
#  座標 Coordinates：像素 X 0~159（左→右）、Y 0~119（上→下）；(0, 0)＝沒有目標 / no target.
#  色相碼 Hue code：0~240 色相（紅 0/240、黃 40、綠 80、青 120、藍 160、紫 200）；
#                   241~248 白／灰 white/grey；249~255 黑 black。
#  物體 Object（模式 51／52）：物體代碼＝色號×10＋形狀（0＝沒有）；物體 1＝面積最大 / object 1 = largest.
#      形狀 shape：0 其他 other  1 圓 circle  2 橢圓 ellipse  3 三角 triangle  4 正方 square
#                  5 長方 rectangle  6 五邊 pentagon  7 六邊 hexagon  8 星 star  9 梯形 trapezoid
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
from pybricks.tools import wait as _wait

_CACHE_MS = 10
_CMD_FLAG = 0x100       # 模式 8 v1（協議版）有這個 bit ＝ 有命令通道（韌體 2026-10-29 起）/ firmware has commands
_RETRY_MS = 1000
_watch = _StopWatch()
MODES = (11, 12, 13, 14, 21, 22, 23, 30, 41, 42, 43, 51, 52)
_NCOL = {11: 14, 12: 7, 13: 4, 14: 2}     # 這個模式給幾色 / colours per mode
_NPT = {21: 14, 22: 7, 23: 4}              # 給幾點 / spots per mode
_NOBJ = {51: 4, 52: 2}                     # 給幾個物體 / objects per mode
SHAPES = ('other', 'circle', 'ellipse', 'triangle', 'square', 'rectangle', 'pentagon', 'hexagon', 'star', 'trapezoid')
SHAPES_ZH = ('其他', '圓', '橢圓', '三角', '正方', '長方', '五邊', '六邊', '星', '梯形')


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


def shape_name(s, zh=False):
    """形狀編號 0~9 → 名稱 / shape number -> name: 'other' 'circle' 'ellipse' 'triangle' 'square'
    'rectangle' 'pentagon' 'hexagon' 'star' 'trapezoid'；zh=True 回中文（'圓'…）；None 或超出範圍回 None"""
    if s is None or not 0 <= s <= 9:
        return None
    return SHAPES_ZH[s] if zh else SHAPES[s]


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
        # 只拼需要的 2 個 byte：Pybricks 主機沒有大整數，48 位元會 small int overflow
        o = 3 * (c - 1) if c else 42
        i = o >> 3
        w = b[i] | ((b[i + 1] << 8) if i + 1 < 6 else 0)
        return (w >> (o & 7)) & (7 if c else 15)
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


def _ob(v, m, i):
    # 物體 i 的 (代碼, X, Y, 寬, 高, 面積)；51 沒有寬高面積（None）/ object i: (code, X, Y, w, h, area)
    b = _bytes(v, 14)
    if m == 51:
        k = 3 * i - 2
        return (b[k], b[k + 1], b[k + 2], None, None, None)
    k = 7 * (i - 1)
    return (b[k] * 10 + b[k + 1] if b[k] else 0, b[k + 2], b[k + 3], b[k + 4], b[k + 5], b[k + 6])


def _ocount(v, m, a):
    return _bytes(v, 14)[0]


class UniCam:
    """神攝手（韌體 v1）/ SharpShot Vision (firmware v1): cam = UniCam(port, mode=13, multitask=False)"""

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
        self._seq = None
        self._last = None
        self._oldfw = 0
        if self._a:
            self._open()        # 多工：要自己 await cam.set_mode(mode) / multitask: await cam.set_mode(mode) yourself
        else:
            self.set_mode(mode)  # 把相機切到 mode（不用再到相機選單切）/ switch the camera to mode automatically

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

    def _obj(self, name, i, ok=(51, 52)):
        self._need(name, ok)
        n = _NOBJ[self._m]
        if not 1 <= i <= n:
            raise ValueError('第幾個物體要填 1~%d / object must be 1~%d' % (n, n))

    # ───── 共用 / common ─────
    def mode_number(self):
        """建構時設定的模式編號（11~52）/ the mode number given to UniCam()"""
        return self._r(self._m)

    def sensor_ok(self):
        """最近一次有沒有讀到感應器 / did the last read succeed: True／False"""
        if self._d is None:
            self._open()
        return self._r(self._d is not None and not self._bad)

    def device_id(self):
        """模式 8：(位址 0x4C, hw0, 韌體版字)，目前的神攝手 hw0＝3 / mode 8: (0x4C, hw0, firmware word); current SharpShot Vision hw0 = 3"""
        return self._g(8, lambda v, m, a: (int(v[0]) & 0xFF, (int(v[0]) >> 8) & 0xFF, int(v[2]) & 0xFFFF))

    def is_unicam(self):
        """是不是目前的神攝手（模式 8 的 hw0＝3）/ is this the current SharpShot Vision (hw0 = 3)"""
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
        """取樣點 point 的色相碼 0~255（沒設定的點回 0）/ hue code of spot (0 if the spot is not set)"""
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

    # ───── 相機模式 5 物體偵測（51、52）/ camera mode 5 object detection ─────
    #  物體 1、2…＝面積由大到小；只看目標色 1~8 / object 1 = largest; only target colours 1~8
    def max_object(self):
        """這個模式給幾個物體：51→4、52→2 / objects available in this mode"""
        self._need('max_object', (51, 52))
        return self._r(_NOBJ[self._m])

    def object_count(self):
        """畫面裡有幾個物體 0~8（可能比 max_object() 多；只有模式 51）/ objects in view 0~8 (mode 51 only)"""
        self._need('object_count', (51,))
        return self._g(9, _ocount)

    def object(self, i):
        """物體 i 的代碼＝色號×10＋形狀（例 13＝目標 1 三角形），0＝沒有 / object code = colour×10 + shape, 0 = none"""
        self._obj('object', i)
        return self._g(9, lambda v, m, a: _ob(v, m, a)[0], i)

    def object_found(self, i):
        """有沒有物體 i / is there an object i: True／False"""
        self._obj('object_found', i)
        return self._g(9, lambda v, m, a: _ob(v, m, a)[0] > 0, i)

    def object_color(self, i):
        """物體 i 的色號 1~8（相機上的目標幾），0＝沒有 / colour number 1~8 (camera target), 0 = none"""
        self._obj('object_color', i)
        return self._g(9, lambda v, m, a: _ob(v, m, a)[0] // 10, i)

    def object_shape(self, i):
        """物體 i 的形狀 0~9（shape_name() 換名稱）；沒有物體也是 0，先用 object_found() 判斷
        shape 0~9 (see shape_name()); also 0 when there is no object, check object_found() first"""
        self._obj('object_shape', i)
        return self._g(9, lambda v, m, a: _ob(v, m, a)[0] % 10, i)

    def object_xy(self, i):
        """物體 i 的質心 (X 0~159, Y 0~119) 像素，(0, 0)＝沒有 / centroid in pixels, (0, 0) = none"""
        self._obj('object_xy', i)
        return self._g(9, lambda v, m, a: _ob(v, m, a)[1:3], i)

    def object_size(self, i):
        """模式 52：物體 i 外框 (寬, 高) 像素 / mode 52: bounding box (width, height) in pixels"""
        self._obj('object_size', i, (52,))
        return self._g(9, lambda v, m, a: _ob(v, m, a)[3:5], i)

    def object_area(self, i):
        """模式 52：物體 i 面積（佔畫面 %），0＝沒有 / mode 52: area in % of the picture"""
        self._obj('object_area', i, (52,))
        return self._g(9, lambda v, m, a: _ob(v, m, a)[5], i)

    # ───── 下命令（韌體 2026-10-29 起）/ commands (firmware 2026-10-29 or later) ─────
    #  送法：PUPDevice.write(9, (序號, I2C 命令碼, p1~p5))，命令碼與參數照 I2C 命令表（規格定案 §6）。
    #  相機在模式 8 第 4 個值回 [序號, 狀態]：0 完成 done、1 忙碌 busy、2 失敗 failed、3 拒絕 rejected。
    #  模式 8 第 2 個值（協議版）有 0x100 這個 bit 才是有命令的新韌體。busy()／wait_done() 讀模式 8，要切模式（每次多約 40～60 ms），
    #  不要在讀值的迴圈裡一直叫：下命令 → wait_done() → 再進迴圈。
    #  Sent with PUPDevice.write(9, (seq, I2C code, p1..p5)); the camera answers [seq, status] in value 4 of mode 8
    #  (value 2 of mode 8 has bit 0x100 on firmware with commands). busy()/wait_done() switch to mode 8: call them before your loop.
    #  官方 SPIKE App 送不了命令（只有 Pybricks 可以）/ the official SPIKE App cannot send commands (Pybricks only).
    def _seqfor(self, key, v, again=False):
        if key == self._last and not again:   # 同命令＋同參數沿用同序號（相機不會重做）；again=True 換新序號再做一次 / same command reuses its number unless again
            return self._seq
        if self._seq is None:          # 新程式：從相機目前的序號 +1 開始 / start after the camera's last number
            self._seq = (int(v[3]) >> 8) & 0xFF
        self._seq = self._seq % 255 + 1    # 1~255（0 不是命令 / 0 is not a command）
        self._last = key
        return self._seq

    def _pack(self, code, p):
        if len(p) > 5:
            raise ValueError('參數最多 5 個 / at most 5 parameters')
        return tuple(int(x) for x in p) + (0,) * (5 - len(p))

    def _nocmd(self, e):
        if not self._oldfw:
            print('MBC: Port', self._n, '相機韌體不支援命令（2026-10-29 以前的韌體），請在相機選單切模式 / '
                  'camera firmware has no commands (before 2026-10-29), use the camera menu:', e)
        self._oldfw = 1

    def _newfw(self, v):            # 模式 8 讀值 → 有沒有命令通道（v1 有 0x100）/ firmware has the command channel
        if v is None or len(v) < 4 or not (int(v[1]) & _CMD_FLAG):
            raise ValueError('模式 8 v1 = %s，沒有命令旗標 0x100' % (None if v is None or len(v) < 2 else v[1]))
        return v

    def _st(self, v):
        if v is None or len(v) < 4 or self._seq is None:
            return None
        w = int(v[3]) & 0xFFFF
        if (w >> 8) != self._seq:      # 相機還沒收到這個序號 / not received yet
            return 1
        return w & 0xFF

    def cmd(self, code, *p, again=False):
        """送命令、不等（回序號；送不出去回 None）。同命令＋同參數再叫不會重做，要再做一次寫 again=True
        send a command without waiting: cam.cmd(0xE2, 50); the same command + parameters is not repeated unless again=True"""
        q = self._pack(code, p)
        if self._a:
            return self._cmda(code, q, again)
        d = self._open()
        if d is None:
            return None
        try:
            v = None if self._seq is not None else self._newfw(self._put(8, d.read(8)))
            s = self._seqfor((code,) + q, v, again)
            d.write(9, (s, code) + q)
            self._c.pop(9, None)           # 讀值快取作廢 / drop the cached reading
            return s
        except Exception as e:
            self._nocmd(e)
        return None

    async def _cmda(self, code, q, again=False):
        d = self._open()
        if d is None:
            return None
        try:
            v = None if self._seq is not None else self._newfw(self._put(8, await d.read(8)))
            s = self._seqfor((code,) + q, v, again)
            await d.write(9, (s, code) + q)
            self._c.pop(9, None)
            return s
        except Exception as e:
            self._nocmd(e)
        return None

    def busy(self):
        """最後一個命令還在做嗎 / is the last command still running"""
        if self._a:
            return self._busya()
        d = self._open()
        try:
            return d is not None and self._st(self._put(8, d.read(8))) == 1
        except OSError as e:
            self._warn(e)
        return False

    async def _busya(self):
        d = self._open()
        try:
            return d is not None and self._st(self._put(8, await d.read(8))) == 1
        except OSError as e:
            self._warn(e)
        return False

    def wait_done(self, timeout=5000):
        """等最後一個命令做完：回 0 完成、2 失敗、3 拒絕；逾時回 None
        wait for the last command: 0 done, 2 failed, 3 rejected; None on timeout"""
        if self._a:
            return self._waita(timeout)
        t0 = _watch.time()
        while _watch.time() - t0 < timeout:
            d = self._open()
            if d is not None:
                try:
                    s = self._st(self._put(8, d.read(8)))
                    if s is not None and s != 1:
                        return s
                except OSError as e:
                    self._warn(e)
            _wait(10)
        return None

    async def _waita(self, timeout):
        t0 = _watch.time()
        while _watch.time() - t0 < timeout:
            d = self._open()
            if d is not None:
                try:
                    s = self._st(self._put(8, await d.read(8)))
                    if s is not None and s != 1:
                        return s
                except OSError as e:
                    self._warn(e)
            await _wait(10)
        return None

    def _do(self, timeout, code, *p, again=False):   # 送＋等 / send and wait
        if self._a:
            return self._doa(timeout, code, p, again)
        if self.cmd(code, *p, again=again) is None:
            return None
        return self.wait_done(timeout)

    async def _doa(self, timeout, code, p, again=False):
        if await self._cmda(code, self._pack(code, p), again) is None:
            return None
        return await self._waita(timeout)

    def set_mode(self, mode):
        """切相機模式（E0，存進相機；值沒變不會重寫）/ switch the camera mode (stored in the camera): cam.set_mode(51)"""
        if mode not in MODES:
            raise ValueError('mode 要填 %s / mode must be one of %s' % (MODES, MODES))
        self._m = mode
        self._c = {}
        if self._a:
            return self._set_modea(mode)
        r = self._do(500, 0xE0, mode // 10, mode % 10)
        if r == 0:
            _wait(60)                  # 下一幀才是新模式的值 / the next frame has the new mode
        return r

    async def _set_modea(self, mode):
        r = await self._doa(500, 0xE0, (mode // 10, mode % 10))
        if r == 0:
            await _wait(60)
        return r

    def light(self, pct=None):
        """補光燈亮度 0~100 %（0＝關；None＝回相機選單的 Flash 設定；不存）/ lamp 0~100 % (None = camera menu setting)"""
        return self._do(500, 0xE2, 255 if pct is None else max(0, min(100, int(pct))))

    def teach(self, color):
        """遠端教色（E3）：用畫面中央的顏色登錄成目標 color 1~16（存進相機）。回 0 成功、2 失敗
        （白／灰、黑也可以教，只有完全分不出顏色才失敗）
        teach target colour 1~16 from the centre of the picture; 0 ok, 2 failed
        (white/grey and black can be taught too; fails only when no colour can be told apart)
        每次叫都會重教（換色卡再 teach 同一色號也會教）/ teaches again on every call"""
        return self._do(3000, 0xE3, color, again=True)

    def clear(self, color):
        """清除目標 color 1~16（E4；SPIKE 不開放全部清除）/ clear target colour 1~16"""
        return self._do(1000, 0xE4, color)

    def target_range(self, color, hue=255, sat=255, size=255, search=0):
        """目標 color 的 Hue 範圍 1~119、Sta 範圍 0~31、最小面積 4~254（E5；255＝不改；要先教過）；
        search 搜尋方式：1＝鎖住上次那塊、2＝每幀選最大塊、0＝不改
        hue range 1~119, sat range 0~31, min area 4~254 of a taught colour (255 = keep);
        search: 1 = locked (stay on last blob), 2 = largest blob each frame, 0 = keep"""
        return self._do(1000, 0xE5, color, hue, sat, size, search)

    def shape_tolerance(self, tol):
        """形狀容許 1~5（E6；3＝出廠）/ shape tolerance 1~5 (3 = factory)"""
        return self._do(500, 0xE6, tol)

    def target_shape(self, color, shape):
        """目標色 1~8 的指定形狀 0~9（E7；0＝不指定）/ expected shape 0~9 of target colour 1~8 (0 = any)"""
        return self._do(500, 0xE7, color, shape)

    def camera_setting(self, item, value):
        """鏡頭設定（E8）：item 0~12 同相機 MENU → Setup 的順序（0 亮度 … 12 Light Hz），value 可以是負數
        camera setting: item 0~12 in MENU → Setup order, value may be negative"""
        return self._do(2000, 0xE8, item, value)

    def white_cal(self):
        """白色校正（E9，約 3 秒）：白紙要先蓋滿畫面中央 / white calibration (~3 s): white paper must fill the centre
        每次叫都會重做 / runs again on every call"""
        return self._do(8000, 0xE9, 0xA5, again=True)

    def power(self, mode):
        """工作模式（EB；不存，相機重開機回全速）：0 全速 full、1 省電 save（25fps）、2 待機 standby（鏡頭關）
        待機 → 開工約 1～2.5 秒，這期間讀到的都是「沒有」/ standby -> working takes 1~2.5 s"""
        return self._do(5000, 0xEB, mode)
