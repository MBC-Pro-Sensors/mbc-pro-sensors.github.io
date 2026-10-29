# ===== 循行者 16 路 讀值小工具（樂高官方 SPIKE App 3 教育版 Python，照抄就好、不用改）=====
# ===== Pathfinder 16-Way reading helper (LEGO SPIKE App 3, Education, Python: copy as is) =====
# 循行者 16 路在 SPIKE App 裡是「顏色感應器」：顏色＝線寬、反射光＝粗位置＋16、原始紅＝細位置＋100。
# In the SPIKE App the Pathfinder 16 is a colour sensor: colour = line width, reflection = position + 16, raw red = fine position + 100.
# 原始綠、藍放的是 16 路光值（每路只有 2 bit），這個小工具不拆；要每一路的光值請改用 Pybricks。
# Raw green and blue hold the 16 channel readings (2 bits each); this helper does not split them. Use Pybricks for those.
# 站在車後往前看：線在左＝負、在右＝正 / seen from behind the robot: line on the left = negative
import color_sensor


class Line16App:
    def __init__(self, port_number):
        self.port = port_number
        self.b = [0] * 8                      # byte 0~7

    def update(self):
        """讀一次最新資料 / read once"""
        c = color_sensor.color(self.port)              # byte 0 = 線寬 / line width
        r = color_sensor.reflection(self.port)         # byte 1 = 粗位置＋16 / position + 16
        rgb = color_sensor.rgbi(self.port)             # 原始紅、綠、藍（各 2 byte）/ raw red, green, blue
        b = [int(c) & 255, int(r) & 255]
        for v in rgb[:3]:
            v = int(v) & 0xFFFF                        # 負數也換回 0~65535 / negatives back to 0~65535
            b += [v & 255, v >> 8]
        self.b = b
        return True

    def _w(self, i):
        # 原始紅（i=0）、綠（1）、藍（2）的 0~65535 / raw red, green, blue word
        return self.b[2 + 2 * i] | (self.b[3 + 2 * i] << 8)

    def pos16(self):
        """粗位置 −16~+16，0＝正中間；看不到線時停在最後離開的那一側 / position −16~+16, 0 = centre; keeps the last side when the line is lost"""
        return self.b[1] - 16

    def pos100(self):
        """細位置 −100~+100，循線用這個比較順 / fine position −100~+100, smoother for line following"""
        return self._w(0) - 100

    def width(self):
        """線寬 0~16：幾路踩在線上，0＝看不到線 / line width 0~16: channels on the line, 0 = no line"""
        return self.b[0]
# ===== 小工具到這裡結束 / end of helper =====
