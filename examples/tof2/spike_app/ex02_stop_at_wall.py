# 測距者 2 路 × 樂高官方 SPIKE App（教育版 Python）範例 2：停在牆前 20 公分
# Ranger 2-Way × LEGO SPIKE App (Education, Python) example 2: stop 20 cm in front of a wall
# 接線：測距者 → C 孔；左輪 E、右輪 F / Wiring: Ranger → port C; left wheel E, right wheel F
# 太遠就往前、太近就後退，越接近 20 公分開越慢 / too far: forward, too near: back up, slower near 20 cm
# ===== 測距者 2 路 讀值小工具（樂高官方 SPIKE App 3 教育版 Python，照抄就好、不用改）=====
# ===== Ranger 2-Way reading helper (LEGO SPIKE App 3, Education, Python: copy as is) =====
# 測距者在 SPIKE App 裡是「顏色感應器」：原始紅＝左距離×256、原始綠＝右距離×256、原始藍＝平均距離×256（公分）。
# In the SPIKE App the Ranger is a colour sensor: raw red = left × 256, raw green = right × 256, raw blue = average × 256 (cm).
# 顏色、反射光可能被 App 限制在 0~10、0~100，這個小工具不用它們 / colour and reflection may be clipped by the App, so they are not used.
# 120＝前面沒東西（或那顆雷射失效）/ 120 = nothing in range (or that laser failed)
import color_sensor


class Laser2App:
    def __init__(self, port_number):
        self.port = port_number
        self.b = [0] * 8                      # byte 0~7；byte 0、1 不讀，一直是 0 / bytes 0~7; bytes 0, 1 are not read and stay 0

    def update(self):
        """讀一次最新資料 / read once"""
        rgb = color_sensor.rgbi(self.port)             # 原始紅、綠、藍（各 2 byte）/ raw red, green, blue
        b = [0, 0]
        for v in rgb[:3]:
            v = int(v) & 0xFFFF                        # 負數也換回 0~65535 / negatives back to 0~65535
            b += [v & 255, v >> 8]
        self.b = b
        return True

    def _w(self, i):
        # 原始紅（i=0）、綠（1）、藍（2）的 0~65535 / raw red, green, blue word
        return self.b[2 + 2 * i] | (self.b[3 + 2 * i] << 8)

    def left(self):
        """左距離 0~120 cm（原始紅 ÷ 256 取整數）/ left distance 0~120 cm (raw red ÷ 256)"""
        return self._w(0) >> 8

    def right(self):
        """右距離 0~120 cm（原始綠 ÷ 256 取整數）/ right distance 0~120 cm (raw green ÷ 256)"""
        return self._w(1) >> 8

    def average(self):
        """左右平均 0~120 cm（原始藍 ÷ 256，感應器自己算好的）/ average 0~120 cm (raw blue ÷ 256, worked out by the sensor)"""
        return self._w(2) >> 8

    def min_distance(self):
        """左右比較近的那一個 0~120 cm / the nearer of left and right, 0~120 cm"""
        return min(self.left(), self.right())

    def diff(self):
        """左減右 −120~+120：負＝左邊比較近、正＝右邊比較近 / left minus right: negative = left is nearer"""
        return self.left() - self.right()
# ===== 小工具到這裡結束 / end of helper =====
from hub import port
import motor_pair
import runloop

laser2 = Laser2App(port.C)
TARGET = 20       # 👉 可以改：要停在幾公分 / distance to stop at (cm)
KP = 15           # 👉 可以改：差 1 公分開多快 / speed per cm of error


async def main():
    motor_pair.pair(motor_pair.PAIR_1, port.E, port.F)
    while True:
        laser2.update()
        error = laser2.average() - TARGET         # 太遠＝正 → 往前；太近＝負 → 後退 / too far = positive
        speed = max(-400, min(400, KP * error))
        motor_pair.move_tank(motor_pair.PAIR_1, speed, speed)
        await runloop.sleep_ms(50)

runloop.run(main())
