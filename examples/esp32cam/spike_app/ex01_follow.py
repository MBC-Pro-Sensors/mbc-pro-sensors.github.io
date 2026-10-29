# 神攝手（學校版）× 樂高官方 SPIKE App（教育版 Python）範例 1：跟著色塊轉
# Sharpshooter Vision (school) × LEGO SPIKE App (Education, Python) example 1: turn toward a blob
# 接線：相機 → C 孔；左輪 E、右輪 F / Wiring: camera → port C; left wheel E, right wheel F
# 準備：先在相機上做「設定 2：TRACK」選顏色 / choose the colour with "2: TRACK" on the camera first
# 用法：SPIKE App 新增「Python」專案，整支貼上 / new Python project in the SPIKE App, paste all of this
# ===== 神攝手（學校版）讀值小工具（樂高官方 SPIKE App 3 教育版 Python，照抄就好、不用改）=====
# ===== Sharpshooter Vision (school) reading helper (LEGO SPIKE App 3, Education, Python: copy as is) =====
# 相機在 SPIKE App 裡是「顏色感應器」：顏色＝色相、反射光＝動態值、原始紅＝色塊 X、原始綠＝色塊 Y、原始藍＝色塊面積。
# In the SPIKE App the camera is a colour sensor: colour = hue, reflection = motion, raw red = blob X, raw green = blob Y, raw blue = blob area.
# 顏色看「設定 1：COLOR」的取色點、色塊看「設定 2：TRACK」選的顏色，都在相機機身上設定 / set up "1: COLOR" and "2: TRACK" on the camera itself
import color_sensor


class CamSchoolApp:
    def __init__(self, port_number):
        self.port = port_number
        self.b = [0] * 8                      # byte 0~7

    def update(self):
        """讀一次最新資料 / read once"""
        c = color_sensor.color(self.port)              # byte 0 = 色相 / hue
        r = color_sensor.reflection(self.port)         # byte 1 = 動態值 / motion
        rgb = color_sensor.rgbi(self.port)             # 原始紅、綠、藍＝X、Y、面積 / raw red, green, blue = X, Y, area
        b = [int(c) & 255, int(r) & 255]
        for v in rgb[:3]:
            v = int(v) & 0xFFFF                        # 負數也換回 0~65535 / negatives back to 0~65535
            b += [v & 255, v >> 8]
        self.b = b
        return True

    def _w(self, i):
        # 原始紅（i=0）、綠（1）、藍（2）的 0~65535 / raw red, green, blue word
        return self.b[2 + 2 * i] | (self.b[3 + 2 * i] << 8)

    def hue(self):
        """取色點的色相 0~100（紅 0、黃約 17、綠約 33、青約 50、藍約 67、洋紅約 83，100 也是紅）；-1＝沒有顏色（黑白灰） / hue at the colour point 0~100, -1 = no colour"""
        h = self.b[0]
        return h - 256 if h > 127 else h

    def motion(self):
        """動態值 0~100：畫面變了多少（機器人要停住才準） / motion 0~100: how much the picture changed"""
        return self.b[1]

    def blob_x(self):
        """色塊 X：左 0~右 98，中間約 50；沒找到是 0 / blob X: left 0 ~ right 98; 0 = not found"""
        return self._w(0)

    def blob_y(self):
        """色塊 Y：下 1~上 75；沒找到是 0 / blob Y: bottom 1 ~ top 75; 0 = not found"""
        return self._w(1)

    def area(self):
        """色塊面積 0~100，0＝沒找到（跟 Pybricks 的 area() 單位不一樣） / blob area 0~100, 0 = not found (not the same unit as Pybricks area())"""
        return self._w(2)

    def found(self):
        """有沒有找到色塊：面積大於 0 / is a blob found: area above 0"""
        return self._w(2) > 0
# ===== 小工具到這裡結束 / end of helper =====
from hub import port, light_matrix
import motor_pair
import runloop

cam = CamSchoolApp(port.C)
BASE = 150        # 👉 可以改：前進速度（度／秒）/ forward speed (deg/s)
GAIN = 4          # 👉 可以改：X 偏 1 多轉多少 / turn per step of X error


async def main():
    motor_pair.pair(motor_pair.PAIR_1, port.E, port.F)
    while True:
        cam.update()
        if not cam.found():                       # 沒找到：原地轉著找 / not found: spin to search
            motor_pair.move_tank(motor_pair.PAIR_1, 120, -120)
            light_matrix.write('?')
        else:
            error = cam.blob_x() - 50             # 中間 50；在右 → 正 / middle = 50; right = positive
            motor_pair.move_tank(motor_pair.PAIR_1, BASE + GAIN * error, BASE - GAIN * error)
        await runloop.sleep_ms(30)

runloop.run(main())
