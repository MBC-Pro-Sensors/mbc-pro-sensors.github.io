# ===== 神攝手 讀值小工具（樂高官方 SPIKE App 3 教育版 Python，照抄就好、不用改）=====
# ===== Sharpshooter Vision reading helper (LEGO SPIKE App 3, Education, Python: copy as is) =====
# 相機在 SPIKE App 裡是「顏色感應器」：顏色、反射光、原始紅／綠／藍 這 5 個數字（byte 0~7）積木與 Python 都讀得到。
# byte 8~15 只有 App Python 的 device.data() 可能讀得到，還沒上機確認（任務 #77），這個小工具不用它。
# The camera shows up as a colour sensor: colour, reflection, raw red/green/blue (bytes 0~7).
# Bytes 8~15 may only be readable with device.data() (not yet verified, #77); this helper does not use them.
# mode = 你在相機選單切的模式編號（相機模式×10＋子選項，例 13）/ mode = camera menu mode number
import color_sensor


class UniCamApp:
    def __init__(self, port_number, mode):
        self.port = port_number
        self.mode = mode
        self.b = [0] * 8                      # byte 0~7

    def update(self):
        """讀一次最新資料 / read once"""
        c = color_sensor.color(self.port)              # byte 0
        r = color_sensor.reflection(self.port)         # byte 1
        rgb = color_sensor.rgbi(self.port)             # 原始紅、綠、藍（各 2 byte）
        b = [int(c) & 255, int(r) & 255]
        for v in rgb[:3]:
            v = int(v) & 0xFFFF                        # 負數也換回 0~65535
            b += [v & 255, v >> 8]
        self.b = b
        return True

    def _w(self, i):
        # 原始紅（i=0）、綠（1）、藍（2）的 0~65535 / raw red, green, blue word
        return self.b[2 + 2 * i] | (self.b[3 + 2 * i] << 8)

    def cell(self, color):
        """顏色追蹤：色 color 的格子 (X 格, Y 格)，(0, 0)＝沒找到（Y 上＝小）
        模式 13、14：16×12 格（像素 ≈ 格×10−5）；模式 11、12：8×6 格（像素 ≈ 格×20−10），色 1~7"""
        m = self.mode
        if m in (13, 14):
            if color == 1:
                return (self.b[0], self.b[1])
            return (self.b[2 * color - 2], self.b[2 * color - 1])      # 13：色 2~4；14：色 2
        if color == 1:
            g = self.b[1]
        else:
            w = self._w((color - 2) // 2)
            g = w // 100 if color % 2 == 0 else w % 100
        return (g // 10, g % 10)

    def size_cell(self, color):
        """模式 14：色 1、2 的 (寬格, 高格)（16×12 格：寬 ≈ 格×10）"""
        w = self._w(color)                    # 色 1 在原始綠、色 2 在原始藍
        return (w & 255, w >> 8)

    def colors_found(self):
        """模式 11、12：找到幾色（顏色）"""
        return self.b[0]

    def hue(self, point):
        """座標取色：取樣點的色相碼 0~255（21：點 1~6；22、23：點 1~3）"""
        if self.mode == 21:
            return self.b[1 + point]
        return self.b[2 * point]

    def sat(self, point):
        """飽和度（22、23：點 1~3；22 的點 4 在反射光）"""
        if self.mode == 22 and point == 4:
            return self.b[1]
        return self.b[2 * point + 1]

    def val(self, point):
        """明度（23：只有點 1，在反射光；其他點在 byte 8~15）"""
        return self.b[1] if point == 1 else None

    def order(self):
        """照順序給色號：(左起前 6 塊的色相碼, 總塊數)"""
        n = min(6, self.b[0])
        return (self.b[2:2 + n], self.b[0])

    def line(self):
        """循線：(近處偏移, 遠處偏移, 角度, 路口類型, 近處線寬)；偏移、角度 −100~100（線在左＝負）"""
        return (self._w(0) - 100, self._w(1) - 100, self._w(2) - 100, self.b[0], self.b[1])

    # ── 物體偵測（51 物體清單、52 物體詳細）：物體代碼＝色號×10＋形狀，0＝沒有；物體 1＝面積最大 ──
    # ── object detection (51 list, 52 detail): code = colour×10 + shape, 0 = none; object 1 = largest ──
    def _obj(self, i, n, modes=(51, 52)):
        if self.mode not in modes:
            raise ValueError('模式 %d 沒有這個值，要用模式 %s / not available in mode %d, use %s' % (self.mode, modes, self.mode, modes))
        if not 1 <= i <= n:
            raise ValueError('第幾個物體要填 1~%d / object must be 1~%d' % (n, n))

    def max_object(self):
        """byte 0~7 給幾個物體的代碼：51→3、52→2（52 的位置、大小只有物體 1）"""
        self._obj(1, 1)
        return 3 if self.mode == 51 else 2

    def object_count(self):
        """51：畫面裡有幾個物體 0~8（顏色）/ objects in view (colour)"""
        self._obj(1, 1, (51,))
        return self.b[0]

    def object(self, i):
        """物體 i 的代碼（51：物體 1~3；52：物體 1~2）/ object code"""
        if self.mode == 51:
            self._obj(i, 3)
            return self.b[1] if i == 1 else self._w(i - 1) // 100
        self._obj(i, 2)
        return self.b[0] if i == 1 else self._w(2) & 255

    def object_color(self, i):
        """物體 i 的色號 1~8，0＝沒有"""
        return self.object(i) // 10

    def object_shape(self, i):
        """物體 i 的形狀 0~9（沒有物體也是 0）"""
        return self.object(i) % 10

    def object_cell(self, i):
        """51：物體 i（1~3）的 8×6 格 (X 格, Y 格)，(0, 0)＝沒有（Y 上＝小；像素 ≈ 格×20−10）"""
        self._obj(i, 3, (51,))
        g = self._w(0) if i == 1 else self._w(i - 1) % 100
        return (g // 10, g % 10)

    def object_xy(self, i):
        """52：物體 1 的質心 (X, Y) 像素，(0, 0)＝沒有"""
        self._obj(i, 1, (52,))
        return (self._w(0) & 255, self._w(0) >> 8)

    def object_size(self, i):
        """52：物體 1 的外框 (寬, 高) 像素"""
        self._obj(i, 1, (52,))
        return (self._w(1) & 255, self._w(1) >> 8)

    def object_area(self, i):
        """52：物體 1、2 的面積（佔畫面 %）"""
        self._obj(i, 2, (52,))
        return self.b[1] if i == 1 else self._w(2) >> 8


def shape_name(s, zh=False):
    """形狀 0~9 → 'other' 'circle' 'ellipse' 'triangle' 'square' 'rectangle' 'pentagon' 'hexagon' 'star' 'trapezoid'（zh=True 中文）"""
    if s is None or not 0 <= s <= 9:
        return None
    return ('其他', '圓', '橢圓', '三角', '正方', '長方', '五邊', '六邊', '星', '梯形')[s] if zh else         ('other', 'circle', 'ellipse', 'triangle', 'square', 'rectangle', 'pentagon', 'hexagon', 'star', 'trapezoid')[s]


def color_name(h):
    """色相碼 → 'red' 'orange' 'yellow' 'green' 'cyan' 'blue' 'purple' 'white' 'black'"""
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
# ===== 小工具到這裡結束 / end of helper =====
