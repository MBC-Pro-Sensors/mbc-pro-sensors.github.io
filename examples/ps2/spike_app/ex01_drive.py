# 掌控者 PS2／PS4 × 樂高官方 SPIKE App（教育版 Python）範例 1：搖桿開車
# Controller PS2 / PS4 × LEGO SPIKE App (Education, Python) example 1: drive with the sticks
# 接線：接收器 → C 孔；左輪 E、右輪 F / Wiring: receiver → port C; left wheel E, right wheel F
# 左搖桿上下＝左輪、右搖桿上下＝右輪（坦克式）；按住 ✕ 停車 / left stick = left wheel, right stick = right wheel (tank style); hold ✕ to stop
# 用法：SPIKE App 新增「Python」專案，整支貼上 / new Python project in the SPIKE App, paste all of this
# ===== 掌控者 PS2／PS4 讀值小工具（樂高官方 SPIKE App 3 教育版 Python，照抄就好、不用改）=====
# ===== Controller PS2 / PS4 reading helper (LEGO SPIKE App 3, Education, Python: copy as is) =====
# 接收器在 SPIKE App 裡是「顏色感應器」：反射光＝方向鍵＋圖形鍵、顏色＝肩鍵＋功能鍵、原始紅＝左搖桿 X×10＋Y、原始綠＝右搖桿。
# In the SPIKE App the receiver is a colour sensor: reflection = D-pad + shape buttons, colour = shoulder + function buttons, raw red = left stick X×10+Y, raw green = right stick.
# PS2、PS4 接收器排法一樣；PS4 的 SHARE＝SELECT、OPTIONS＝START / PS2 and PS4 receivers use the same layout; SHARE = SELECT, OPTIONS = START
# 原始藍（16 鍵另一種排法）這個小工具不用 / raw blue (another 16-button layout) is not used by this helper
import color_sensor

# 按鍵名稱 → 第幾個位元（跟 Pybricks 函式庫 MBC_gamepad 的 buttons() 一樣） / button name → bit (same as buttons() in the Pybricks library)
BUTTONS = {
    'UP': 0, 'RIGHT': 1, 'DOWN': 2, 'LEFT': 3,
    'TRIANGLE': 4, 'CIRCLE': 5, 'CROSS': 6, 'SQUARE': 7,
    'L2': 8, 'R2': 9, 'L1': 10, 'R1': 11,
    'SELECT': 12, 'L3': 13, 'R3': 14, 'START': 15,
    'SHARE': 12, 'CREATE': 12, 'OPTIONS': 15,
}
STICKS = ('LX', 'LY', 'RX', 'RY')


class GamepadApp:
    def __init__(self, port_number):
        self.port = port_number
        self.b = [0] * 8                      # byte 0~7

    def update(self):
        """讀一次最新資料 / read once"""
        c = color_sensor.color(self.port)              # byte 0 = 肩鍵＋功能鍵 / shoulder + function buttons
        r = color_sensor.reflection(self.port)         # byte 1 = 方向鍵＋圖形鍵 / D-pad + shape buttons
        rgb = color_sensor.rgbi(self.port)             # 原始紅、綠、藍（各 2 byte） / raw red, green, blue
        b = [int(c) & 255, int(r) & 255]               # START 讀到 -128，& 255 換成 128 / START reads -128, & 255 makes it 128
        for v in rgb[:3]:
            v = int(v) & 0xFFFF                        # 負數也換回 0~65535 / negatives back to 0~65535
            b += [v & 255, v >> 8]
        self.b = b
        return True

    def _w(self, i):
        # 原始紅（i=0）、綠（1）、藍（2）的 0~65535 / raw red, green, blue word
        return self.b[2 + 2 * i] | (self.b[3 + 2 * i] << 8)

    def buttons(self):
        """全部按鍵合成一個數字 0~65535，沒按＝0 / all buttons in one number 0~65535, 0 = none"""
        return self.b[1] | (self.b[0] << 8)

    def pressed(self, name):
        """這個鍵有沒有按住：True／False，名稱例如 'CROSS'、'UP'、'L1' / is this button held: True or False"""
        if name not in BUTTONS:
            raise ValueError('沒有這個按鍵名稱 / unknown button name: ' + str(name))
        return (self.buttons() >> BUTTONS[name]) & 1 == 1

    def any_pressed(self):
        """有沒有任何一個鍵按著 / is any button held"""
        return self.buttons() != 0

    def stick(self, name):
        """搖桿位置 1~9（5＝中間）：'LX' 'LY' 'RX' 'RY' / stick position 1~9 (5 = centre)"""
        if name not in STICKS:
            raise ValueError('沒有這個搖桿名稱 / unknown stick name: ' + str(name))
        w = self._w(0 if name[0] == 'L' else 1)        # X×10+Y
        return w // 10 if name[1] == 'X' else w % 10

    def stick_centered(self, name):
        """搖桿位置減 5：-4~4（0＝中間） / stick position minus 5: -4~4 (0 = centre)"""
        return self.stick(name) - 5
# ===== 小工具到這裡結束 / end of helper =====
from hub import port, light_matrix
import motor_pair
import runloop

pad = GamepadApp(port.C)
SPEED = 200       # 👉 可以改：搖桿推 1 格多快（度／秒）/ speed per stick step (deg/s)


async def main():
    motor_pair.pair(motor_pair.PAIR_1, port.E, port.F)
    while True:
        pad.update()
        if pad.pressed('CROSS'):                       # 按住 ✕：停車 / hold ✕: stop
            motor_pair.stop(motor_pair.PAIR_1)
            light_matrix.write('X')
        else:
            left = pad.stick_centered('LY') * SPEED    # -4~4 → 速度 / speed
            right = pad.stick_centered('RY') * SPEED
            motor_pair.move_tank(motor_pair.PAIR_1, left, right)
        await runloop.sleep_ms(20)

runloop.run(main())
