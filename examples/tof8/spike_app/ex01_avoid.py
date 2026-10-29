# 測距者 8 路 × 樂高官方 SPIKE App（教育版 Python）範例 1：簡單避障
# Ranger 8-Way × LEGO SPIKE App (Education, Python) example 1: simple obstacle avoidance
# 接線：測距者 → C 孔；左輪 E、右輪 F / Wiring: Ranger → port C; left wheel E, right wheel F
# 前面有東西就往比較空的那邊原地轉 / when something is ahead, it spins toward the clearer side
# 用法：SPIKE App 新增「Python」專案，整支貼上 / new Python project in the SPIKE App, paste all of this
# ===== 測距者 8 路 讀值小工具（樂高官方 SPIKE App 3 教育版 Python，照抄就好、不用改）=====
# ===== Ranger 8-Way reading helper (LEGO SPIKE App 3, Education, Python: copy as is) =====
# 測距者在 SPIKE App 裡是「顏色感應器」：顏色＝第 1 路、反射光＝第 2 路、原始紅／綠／藍＝第 3～8 路（一個數字裝兩路）。
# In the SPIKE App the Ranger is a colour sensor: colour = sensor 1, reflection = sensor 2, raw red/green/blue = sensors 3~8 (two per number).
# 距離是公分 0~120，120＝那個方向沒東西（或那一路故障） / distances in cm 0~120, 120 = nothing that way (or that sensor failed)
# 顏色、反射光（第 1、2 路）還沒上機逐一確認，App 可能把它們限制在 0~10、0~100 / sensors 1, 2 not yet hardware-checked: the App may clip them
import color_sensor


class Laser8App:
    def __init__(self, port_number):
        self.port = port_number
        self.b = [0] * 8                      # 第 1~8 路 / sensors 1~8

    def update(self):
        """讀一次最新資料 / read once"""
        c = color_sensor.color(self.port)              # 第 1 路 / sensor 1
        r = color_sensor.reflection(self.port)         # 第 2 路 / sensor 2
        rgb = color_sensor.rgbi(self.port)             # 原始紅、綠、藍＝第 3~8 路 / raw red, green, blue = sensors 3~8
        b = [int(c) & 255, int(r) & 255]
        for v in rgb[:3]:
            v = int(v) & 0xFFFF                        # 負數也換回 0~65535 / negatives back to 0~65535
            b += [v & 255, v >> 8]                     # ÷256 的餘數＝前一路、取整數＝後一路 / low byte = first, high byte = second
        self.b = b
        return True

    def distances(self):
        """8 路距離（公分），第 1 路（正左）排到第 8 路（正右） / the 8 distances (cm), sensor 1 (left) to sensor 8 (right)"""
        return list(self.b)

    def ch(self, k):
        """第 k 路的距離 0~120 公分（k＝1~8） / distance of sensor k (1~8), 0~120 cm"""
        if not 1 <= k <= 8:
            raise ValueError('第幾路要填 1~8 / sensor must be 1~8')
        return self.b[k - 1]

    def front(self):
        """正前方：第 4、5 路比較近的那一個 / front: the nearer of sensors 4 and 5"""
        return min(self.b[3], self.b[4])

    def min_distance(self):
        """8 路裡最近的距離，120＝四周沒有東西 / nearest of the 8, 120 = nothing around"""
        return min(self.b)

    def nearest(self):
        """(最近距離, 第幾路)；一樣近取前面那一路；四周沒東西＝(120, 0) / (nearest cm, sensor number); (120, 0) = nothing"""
        d, k = 120, 0
        for i in range(8):
            if self.b[i] < d:
                d, k = self.b[i], i + 1
        return (d, k)


def direction(k):
    """第 k 路朝哪個方向：k×2−9（−7 正左、−1 和 +1 正前、+7 正右）；k＝0 回傳 0 / direction of sensor k: k×2−9; 0 for k = 0"""
    return k * 2 - 9 if k else 0
# ===== 小工具到這裡結束 / end of helper =====
from hub import port
import motor_pair
import runloop

laser8 = Laser8App(port.C)
SPEED = 300       # 👉 可以改：直走速度（度／秒）/ forward speed (deg/s)
STOP_CM = 30      # 👉 可以改：前面多近就轉彎（公分）/ turn when something is this close (cm)


async def main():
    motor_pair.pair(motor_pair.PAIR_1, port.E, port.F)
    while True:
        laser8.update()
        if laser8.front() < STOP_CM:
            left = min(laser8.ch(1), laser8.ch(2), laser8.ch(3))     # 左邊三路最近的 / nearest on the left
            right = min(laser8.ch(6), laser8.ch(7), laser8.ch(8))    # 右邊三路最近的 / nearest on the right
            if left > right:                                         # 左邊比較空：原地左轉 / more room on the left: spin left
                motor_pair.move_tank(motor_pair.PAIR_1, -SPEED, SPEED)
            else:                                                    # 右邊比較空：原地右轉 / more room on the right: spin right
                motor_pair.move_tank(motor_pair.PAIR_1, SPEED, -SPEED)
            await runloop.sleep_ms(400)
        else:
            motor_pair.move_tank(motor_pair.PAIR_1, SPEED, SPEED)
        await runloop.sleep_ms(20)

runloop.run(main())
