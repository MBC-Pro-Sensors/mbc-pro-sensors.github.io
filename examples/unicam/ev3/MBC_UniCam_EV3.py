# MBC 神攝手（韌體 v1）：EV3 函式庫（Pybricks v4 與 樂高官方 EV3 MicroPython v2 共用）
# MBC Sharpshooter Vision (firmware v1): EV3 library (Pybricks v4 and LEGO EV3 MicroPython v2)
# 作者 Author：MBC Robot　日期 Date：2026-10-28　版本 Version：v1.0（v1 尚未上機驗證 / not yet hardware-verified）
#
# ！！把 MBC_UniCam_EV3.py 跟你的程式放在同一個資料夾！！ / put this file next to your program
#
# ════════════════════════════════════════════════════════════════
#  相機在 EV3 上偽裝成「紅外線感應器」/ the camera poses as an EV3 infrared sensor:
#    模式 0 近距 proximity ＝ 模式編號（相機模式 ×10 ＋ 子選項，例 13）
#    模式 1 信標 seeker   ＝ 8 個值 / 8 values
#    模式 2 遙控 remote   ＝ 4 個值（只有模式 11、21、30、4x、51 用到）/ 4 values
#    模式 5 校正 cal      ＝ [裝置 ID 0x034C, 韌體版 100]
#  每換一次模式約 0.5 秒 / every mode switch takes about 0.5 s.
#
#  用法 Usage：
#      from MBC_UniCam_EV3 import UniCam
#      cam = UniCam(Port.S1)            # 不填 mode：第一次用時讀模式 0 自動取得（多 0.5 秒）
#      cam = UniCam(Port.S1, mode=13)   # 填 mode：省下讀模式 0 的時間（要跟相機選單一樣）
#      x, y = cam.xy(1)
#
#  方法名稱跟 SPIKE 的 MBC_UniCam.py 一樣；座標一律換成像素（X 0~159 左→右、Y 0~119 上→下，跟 SPIKE 一樣），
#  EV3 原始的 8×6 格（Y 下＝小）用 grid() 讀。EV3 格子比較粗，位置、大小是格子中心（±10 像素），
#  座標取色 ①／照順序給色號 的色相碼是 0~100 換回來的（±2）。
#  Method names match MBC_UniCam.py (SPIKE). Coordinates are converted to pixels (Y top = small, like SPIKE);
#  raw EV3 8×6 grid (Y bottom = small) via grid(). EV3 grid is coarse: ±10 px; hue in modes 21/30 is ±2.
#
#  物體偵測 Object detection（51 物體清單 5 個／52 物體詳細 2 個）：物體代碼＝色號×10＋形狀（0＝沒有），
#  物體 1＝面積最大；位置同樣是 8×6 格中心（±10 像素），原始格子用 object_grid()；52 的角度 EV3 送 (角度＋1)÷2，
#  函式庫 ×2 還原成 0~179（±1 度）。/ object code = colour×10 + shape; object 1 = largest; angle restored ×2 (±1°).
#      形狀 shape：0 其他 other  1 圓 circle  2 橢圓 ellipse  3 三角 triangle  4 正方 square
#                  5 長方 rectangle  6 五邊 pentagon  7 六邊 hexagon  8 星 star  9 梯形 trapezoid
# ════════════════════════════════════════════════════════════════

from pybricks.parameters import Port

try:                                            # 樂高官方 EV3 MicroPython v2（ev3dev）
    from pybricks.iodevices import Ev3devSensor as _Ev3dev
except ImportError:
    _Ev3dev = None
if _Ev3dev is None:                             # Pybricks v4（code.pybricks.com）
    try:
        from pybricks.iodevices import LUMPDevice as _Lump
    except ImportError:
        from pybricks.iodevices import PUPDevice as _Lump

MODES = (11, 12, 13, 14, 21, 22, 23, 30, 41, 42, 43, 51, 52)
_NAMES = ('IR-PROX', 'IR-SEEK', 'IR-REMOTE', 'IR-REM-A', 'IR-S-ALT', 'IR-CAL')
_NCOL = {11: 12, 12: 8, 13: 4, 14: 2}
_NPT = {21: 12, 22: 4, 23: 2}
_NOBJ = {51: 5, 52: 2}
SHAPES = ('other', 'circle', 'ellipse', 'triangle', 'square', 'rectangle', 'pentagon', 'hexagon', 'star', 'trapezoid')
SHAPES_ZH = ('其他', '圓', '橢圓', '三角', '正方', '長方', '五邊', '六邊', '星', '梯形')


def color_name(h):
    """色相碼 0~255 → 'red' 'orange' 'yellow' 'green' 'cyan' 'blue' 'purple' 'white' 'black'"""
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
    """形狀編號 0~9 → 'other' 'circle' 'ellipse' 'triangle' 'square' 'rectangle' 'pentagon' 'hexagon' 'star'
    'trapezoid'；zh=True 回中文（'圓'…）；None 或超出範圍回 None / shape number -> name"""
    if s is None or not 0 <= s <= 9:
        return None
    return SHAPES_ZH[s] if zh else SHAPES[s]


def _hue(e):
    # EV3 色相值 → 色相碼：0~100 是 色相碼×100÷240；−1~−15 是 241~255
    # EV3 hue value -> hue code: 0~100 = code*100/240, -1~-15 = 241~255
    e = int(e)
    return 240 - e if e < 0 else (e * 240 + 50) // 100


def _d8(v):
    # 8×6 格（X×10＋Y，Y 下＝小）→ 像素中心 / grid -> pixel centre
    v = int(v)
    if v <= 0:
        return (0, 0)
    return ((v // 10 - 1) * 20 + 10, 109 - (v % 10 - 1) * 20)


class UniCam:
    """神攝手（韌體 v1，EV3）：cam = UniCam(port, mode=None)"""

    def __init__(self, port, mode=None):
        if isinstance(port, int):
            if port < 1 or port > 4:
                raise ValueError('port 要填 1~4 / port must be 1~4')
            port = getattr(Port, 'S%d' % port)
        if mode is not None and mode not in MODES:
            raise ValueError('mode 要填 %s / mode must be one of %s' % (MODES, MODES))
        self._p = port
        self._m = mode
        self._d = None
        self._ok = False

    def _rd(self, m):
        try:
            if self._d is None:
                self._d = _Ev3dev(self._p) if _Ev3dev else _Lump(self._p)
            v = self._d.read(_NAMES[m]) if _Ev3dev else self._d.read(m)
            self._ok = True
            return [int(x) for x in v]
        except OSError as e:
            if self._ok or self._d is None:
                print('MBC: 讀不到感應器 / no sensor:', e)
            self._ok = False
            return None

    def _mode(self):
        if self._m is None:
            v = self._rd(0)
            if v is None:
                return None
            if v[0] not in MODES:
                raise ValueError('相機回傳模式 %d，韌體不對？/ camera reports mode %d, wrong firmware?' % (v[0], v[0]))
            self._m = v[0]
        return self._m

    def _need(self, name, ok):
        m = self._mode()
        if m not in ok:
            raise ValueError('%s(): 模式 %s 沒有這個值，要用模式 %s / not available in mode %s, use mode %s'
                             % (name, m, ok, m, ok))
        return m

    def _col(self, c, n):
        if not 1 <= c <= n:
            raise ValueError('第幾色／第幾點要填 1~%d / colour or spot must be 1~%d' % (n, n))

    def _obj(self, name, i, ok=(51, 52)):
        m = self._need(name, ok)
        n = _NOBJ[m]
        if not 1 <= i <= n:
            raise ValueError('第幾個物體要填 1~%d / object must be 1~%d' % (n, n))
        return m

    def _beacon(self, i):
        # 第 i 個值（1~12）：1~8 在模式 1、9~12 在模式 2 / value i: 1~8 mode 1, 9~12 mode 2
        v = self._rd(1 if i <= 8 else 2)
        return None if v is None else v[(i - 1) % 8 if i <= 8 else i - 9]

    # ───── 共用 / common ─────
    def mode_number(self):
        """相機目前的模式編號（mode=None 時讀模式 0）/ camera mode number"""
        if self._m is None:
            v = self._rd(0)
            return None if v is None else v[0]
        return self._m

    def sensor_ok(self):
        """最近一次有沒有讀到 / did the last read succeed"""
        return self._ok

    def device_id(self):
        """模式 5：(位址 0x4C, hw0, 韌體版字)，目前的神攝手 hw0＝3 / mode 5: (0x4C, hw0, firmware word)"""
        v = self._rd(5)
        return None if v is None else (v[0] & 0xFF, (v[0] >> 8) & 0xFF, v[1] & 0xFFFF)

    def is_unicam(self):
        """是不是目前的神攝手（hw0＝3）/ is this the current Sharpshooter Vision"""
        d = self.device_id()
        return d is not None and d[0] == 0x4C and d[1] == 3

    # ───── 顏色追蹤 11~14 / colour tracking ─────
    def max_color(self):
        """這個模式給幾色：11→12、12→8、13→4、14→2"""
        return _NCOL[self._need('max_color', (11, 12, 13, 14))]

    def grid(self, color):
        """EV3 原始 8×6 格 (X 1~8, Y 1~6，Y 下＝小)，(0, 0)＝沒找到 / raw 8×6 grid (Y bottom = small)"""
        m = self._need('grid', (11, 12, 13, 14))
        self._col(color, _NCOL[m])
        i = {11: color, 12: color, 13: 2 * color - 1, 14: 4 * color - 3}[m]
        g = self._beacon(i)
        return None if g is None else ((0, 0) if g <= 0 else (g // 10, g % 10))

    def xy(self, color):
        """位置（像素，格子中心 ±10）；(0, 0)＝沒找到 / position in pixels (grid centre ±10)"""
        m = self._need('xy', (11, 12, 13, 14))
        self._col(color, _NCOL[m])
        i = {11: color, 12: color, 13: 2 * color - 1, 14: 4 * color - 3}[m]
        g = self._beacon(i)
        return None if g is None else _d8(g)

    def x(self, color):
        """X 像素（0＝沒找到）"""
        p = self.xy(color)
        return None if p is None else p[0]

    def y(self, color):
        """Y 像素（上＝小；0＝沒找到）"""
        p = self.xy(color)
        return None if p is None else p[1]

    def found(self, color):
        """有沒有找到 / found"""
        p = self.xy(color)
        return None if p is None else p[0] > 0

    def area(self, color):
        """模式 13、14：面積 % / area %"""
        m = self._need('area', (13, 14))
        self._col(color, _NCOL[m])
        return self._beacon(2 * color if m == 13 else 4 * color)

    def size(self, color):
        """模式 14：(寬, 高) 像素（格子中心 ±10）/ (w, h) pixels"""
        self._need('size', (14,))
        self._col(color, 2)
        g = self._beacon(4 * color - 2)
        if g is None:
            return None
        return (0, 0) if g <= 0 else ((g // 10 - 1) * 20 + 10, (g % 10 - 1) * 20 + 10)

    def count(self, color):
        """模式 14：色 color 有幾塊 0~15（EV3 其他模式沒有數量）/ mode 14 only on EV3"""
        self._need('count', (14,))
        self._col(color, 2)
        return self._beacon(4 * color - 1)

    # ───── 座標取色 21~23 / spot colour ─────
    def max_point(self):
        """這個模式給幾點：21→12、22→4、23→2"""
        return _NPT[self._need('max_point', (21, 22, 23))]

    def hsv(self, point):
        """(色相碼, 飽和度, 明度)：21 只有色相（±2）；22 色相（±2）＋飽和／明度取到十位數；23 原始值
        21 hue only (±2); 22 hue (±2) + sat/val rounded down to tens; 23 exact"""
        m = self._need('hsv', (21, 22, 23))
        self._col(point, _NPT[m])
        if m == 21:
            e = self._beacon(point)
            return None if e is None else (_hue(e), None, None)
        v = self._rd(1)
        if v is None:
            return None
        k = 2 * (point - 1) if m == 22 else 4 * (point - 1)
        if m == 22:
            return (_hue(v[k]), (v[k + 1] // 10) * 10, (v[k + 1] % 10) * 10)
        return (v[k] * 100 + v[k + 1], v[k + 2], v[k + 3])

    def hue(self, point):
        """色相碼 0~255"""
        p = self.hsv(point)
        return None if p is None else p[0]

    def sat(self, point):
        """飽和度（22、23）"""
        self._need('sat', (22, 23))
        p = self.hsv(point)
        return None if p is None else p[1]

    def val(self, point):
        """明度（22、23）"""
        self._need('val', (22, 23))
        p = self.hsv(point)
        return None if p is None else p[2]

    def color(self, point):
        """顏色名稱 / colour name"""
        return color_name(self.hue(point))

    # ───── 照順序給色號 30 / colour order ─────
    def order(self):
        """由左到右每塊的色相碼（最多 11 塊，±2）/ hue codes left to right (up to 11)"""
        self._need('order', (30,))
        a = self._rd(1)
        b = self._rd(2)
        if a is None or b is None:
            return None
        return [_hue(e) for e in (a + b[:3])][:min(11, b[3])]

    def total(self):
        """總塊數 / total blocks"""
        self._need('total', (30,))
        return self._beacon(12)

    # ───── 循線 41~43 / line following ─────
    def _l(self, name, i):
        self._need(name, (41, 42, 43))
        return self._beacon(i)

    def near(self):
        """近處偏移 −100~100（線在左＝負）"""
        return self._l('near', 1)

    def mid(self):
        """中段偏移"""
        return self._l('mid', 2)

    def far(self):
        """遠處偏移"""
        return self._l('far', 3)

    def angle(self):
        """線角度 −90~90"""
        return self._l('angle', 4)

    def width(self):
        """近處線寬 %"""
        return self._l('width', 5)

    def junction(self):
        """路口類型 0 無 1 十字 2 T 3 左 4 右 5 盡頭"""
        v = self._l('junction', 6)
        return None if v is None else v // 10

    def flags(self):
        """有線旗標：近 1＋中 2＋遠 4"""
        v = self._l('flags', 6)
        return None if v is None else v % 10

    def junction_dist(self):
        """路口距離 %"""
        return self._l('junction_dist', 7)

    def marker(self):
        """(標記色號, 累計條數)；色號在模式 1、條數在模式 2 / (marker colour, count)"""
        a = self._l('marker', 8)
        b = self._beacon(12)
        return None if a is None or b is None else (a, b)

    def edges(self):
        """(左緣, 右緣)（模式 2）"""
        self._need('edges', (41, 42, 43))
        v = self._rd(2)
        return None if v is None else (v[0], v[1])

    def far_width(self):
        """遠處線寬 %（模式 2）"""
        return self._l('far_width', 11)

    def line_all(self):
        """(近, 中, 遠, 角度, 線寬, 旗標, 路口類型, 路口距離, 左緣, 右緣, 遠線寬, 標記色號, 標記條數)；EV3 沒有離線方向"""
        self._need('line_all', (41, 42, 43))
        a = self._rd(1)
        b = self._rd(2)
        if a is None or b is None:
            return None
        return (a[0], a[1], a[2], a[3], a[4], a[5] % 10, a[5] // 10, a[6], b[0], b[1], b[2], a[7], b[3])

    # ───── 物體偵測 51、52 / object detection ─────
    #  51：物體 i 代碼在第 2i−1 個值、位置在第 2i 個（物體 5 在模式 2）；物體數在模式 2 通道 4
    #  52：物體 i 的 代碼、位置、面積、角度÷2 在第 4i−3 ~ 4i 個值（全部在模式 1）
    def _oi(self, m, i, k):
        # 物體 i 第 k 項（0 代碼 1 位置 2 面積 3 角度）的值編號 / value index of item k of object i
        return 2 * i - 1 + k if m == 51 else 4 * i - 3 + k

    def max_object(self):
        """這個模式給幾個物體：51→5、52→2 / objects available in this mode"""
        return _NOBJ[self._need('max_object', (51, 52))]

    def object_count(self):
        """畫面裡有幾個物體 0~8（模式 51，讀模式 2；52 沒有）/ objects in view (mode 51, EV3 mode 2)"""
        self._need('object_count', (51,))
        return self._beacon(12)

    def object(self, i):
        """物體 i 的代碼＝色號×10＋形狀（例 13＝目標 1 三角形），0＝沒有 / object code, 0 = none"""
        m = self._obj('object', i)
        return self._beacon(self._oi(m, i, 0))

    def object_found(self, i):
        """有沒有物體 i / is there an object i"""
        c = self.object(i)
        return None if c is None else c > 0

    def object_color(self, i):
        """物體 i 的色號 1~8，0＝沒有 / colour number 1~8, 0 = none"""
        c = self.object(i)
        return None if c is None else c // 10

    def object_shape(self, i):
        """物體 i 的形狀 0~9（沒有物體也是 0，先用 object_found()）/ shape 0~9 (0 also when none)"""
        c = self.object(i)
        return None if c is None else c % 10

    def object_grid(self, i):
        """物體 i 的原始 8×6 格 (X 1~8, Y 1~6，Y 下＝小)，(0, 0)＝沒有 / raw 8×6 grid (Y bottom = small)"""
        m = self._obj('object_grid', i)
        g = self._beacon(self._oi(m, i, 1))
        return None if g is None else ((0, 0) if g <= 0 else (g // 10, g % 10))

    def object_xy(self, i):
        """物體 i 的質心（像素，格子中心 ±10；Y 上＝小），(0, 0)＝沒有 / centroid in pixels (±10)"""
        m = self._obj('object_xy', i)
        g = self._beacon(self._oi(m, i, 1))
        return None if g is None else _d8(g)

    def object_area(self, i):
        """模式 52：物體 i 面積 %（0＝沒有）/ mode 52: area %"""
        self._obj('object_area', i, (52,))
        return self._beacon(self._oi(52, i, 2))

    def object_angle(self, i):
        """模式 52：物體 i 的角度 0~179 度（長邊往右＝0、往畫面下方轉為正；EV3 送 ÷2，×2 還原，±1 度）
        mode 52: angle 0~179 (long side, right = 0, clockwise on screen positive; sent ÷2, restored ×2, ±1°)"""
        self._obj('object_angle', i, (52,))
        v = self._beacon(self._oi(52, i, 3))
        return None if v is None else (0 if v <= 0 else min(179, 2 * v))
