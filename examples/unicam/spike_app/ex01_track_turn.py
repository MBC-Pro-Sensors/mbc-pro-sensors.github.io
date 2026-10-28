# 神攝手通用版 v1 × 樂高官方 SPIKE App（教育版 Python）範例 1：跟著色塊轉
# Sharpshooter Vision Universal v1 × LEGO SPIKE App (Education, Python) example 1: turn to follow a colour blob
# 需要把相機選單切到 相機模式 1 顏色追蹤 ・子選項 ③ 標準（XY＋面積）＝模式編號 13
# Camera menu: camera mode 1 Colour tracking, option 3 Standard (XY + area) = mode number 13
# 準備：先在相機上教「目標 1」的顏色 / teach "target 1" on the camera first
# 接線 Wiring：相機 → C 孔；左輪 E、右輪 F（驅動底座）/ camera → port C; left wheel E, right wheel F
# 用法：SPIKE App 新增「Python」專案，整支貼上 / new Python project in the SPIKE App, paste all of this
# ===== 神攝手通用版 讀值小工具（樂高官方 SPIKE App 3 教育版 Python，照抄就好、不用改）=====
# ===== UniCam reading helper (LEGO SPIKE App 3, Education, Python: copy as is) =====
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
from hub import port, light_matrix
import motor_pair
import runloop

cam = UniCamApp(port.C, 13)                       # 模式編號要跟相機選單一樣 / must match the camera menu
BASE = 200        # 👉 可以改：前進速度（度／秒）/ forward speed (deg/s)
K = 40            # 👉 可以改：偏 1 格多轉多少 / turn per grid cell of error


async def main():
    motor_pair.pair(motor_pair.PAIR_1, port.E, port.F)
    while True:
        cam.update()
        cx, cy = cam.cell(1)                      # 色 1 的格子：X 1~16、Y 1~12；(0, 0) = 沒看到 / grid cell
        if cx == 0:                               # 沒看到：原地轉著找 / not seen: spin to search
            motor_pair.move_tank(motor_pair.PAIR_1, 120, -120)
            light_matrix.write('?')
        else:
            error = cx - 8.5                      # 畫面中間 = 8.5 格；在右 → 正 / centre = 8.5; right = positive
            motor_pair.move_tank(motor_pair.PAIR_1, int(BASE + K * error), int(BASE - K * error))
            light_matrix.write(str(cx))
        await runloop.sleep_ms(30)

runloop.run(main())
