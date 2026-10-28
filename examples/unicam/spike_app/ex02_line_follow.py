# 神攝手通用版 v1 × 樂高官方 SPIKE App（教育版 Python）範例 2：PD 循線＋路口停車
# Sharpshooter Vision Universal v1 × LEGO SPIKE App (Education, Python) example 2: PD line following + stop at a junction
# 需要把相機選單切到 相機模式 4 循線 ・子選項 ① 黑線 ＝模式編號 41（白線 42、色線 43 也一樣）
# Camera menu: camera mode 4 Line, option 1 Black line = mode number 41 (42 / 43 work the same)
# 準備：相機裝在車頭、斜斜往前下方看線 / camera at the front, looking down and forward
# 接線 Wiring：相機 → C 孔；左輪 E、右輪 F / camera → port C; left wheel E, right wheel F
# 方向：站在車後往前看，線在左＝負 / standing behind the robot: line on the left = negative
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
from hub import port, light_matrix, sound
import motor_pair
import runloop

cam = UniCamApp(port.C, 41)
BASE = 250        # 👉 可以改：前進速度（度／秒）/ forward speed (deg/s)
KP = 3            # 👉 可以改：比例 / proportional gain
KD = 8            # 👉 可以改：微分 / derivative gain


async def main():
    motor_pair.pair(motor_pair.PAIR_1, port.E, port.F)
    last_error = 0
    while True:
        cam.update()
        near, far, angle, junction, width = cam.line()   # 偏移 −100~100、路口類型、線寬 %
        if junction in (1, 2):                    # 1 十字、2 T 字路口：停車嗶一聲 / cross or T: stop
            motor_pair.stop(motor_pair.PAIR_1)
            await sound.beep(880, 300, 100)
            break
        if width == 0:                            # 近處看不到線：往上一次偏的那邊找 / line lost: search
            turn = -150 if last_error < 0 else 150
            motor_pair.move_tank(motor_pair.PAIR_1, turn, -turn)
        else:
            error = 0.7 * near + 0.3 * far        # 近處為主、遠處提早轉彎 / mostly near, a bit of far
            turn = KP * error + KD * (error - last_error)
            last_error = error
            motor_pair.move_tank(motor_pair.PAIR_1, int(BASE + turn), int(BASE - turn))
        light_matrix.write(str(junction))
        await runloop.sleep_ms(20)

runloop.run(main())
