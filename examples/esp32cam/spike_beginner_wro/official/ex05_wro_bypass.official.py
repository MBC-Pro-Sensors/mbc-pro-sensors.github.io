# ===== 讀 HUE12 的小工具（跟官方範例 Hue12_demo_For_Spike_v3.4.3 的讀法一樣，照抄就好、不用改）=====
import device
import utime
from hub import port


class HUE12:
    def __init__(self, port_number):
        self.port = port_number
        self.slots = [(255, 0)] * 12          # 12 格的（色相, 飽和度）；255 = 這一格沒設定

    def update(self):
        """讀一次最新資料；讀不到回傳 False"""
        raw = device.data(self.port)          # 感應器送來 8 個整數
        if not raw or len(raw) < 8:
            return False
        b = []
        for val in raw[:8]:
            if val < 0:
                val += 2 ** 32
            b += [val & 0xFF, (val >> 8) & 0xFF, (val >> 16) & 0xFF]   # 每個整數拆成 3 個數字
        for i in range(12):
            self.slots[i] = (b[i * 2], b[i * 2 + 1])
        return True

    def hue(self, slot):
        """第 slot 格（1~12）的色相：0~239 彩色、240~245 白、246~250 黑、255 沒設定"""
        return self.slots[slot - 1][0]

    def sat(self, slot):
        """第 slot 格的飽和度 0~255（越大顏色越鮮豔）"""
        return self.slots[slot - 1][1]
# ===== 小工具到這裡結束，下面才是範例 =====

import motor_pair
from hub import light_matrix

cam = HUE12(port.C)
motor_pair.pair(motor_pair.PAIR_1, port.E, port.F)


def go(steering, ms):
    """用 steering（-100 左轉 ~ 0 直走 ~ 100 右轉）走 ms 毫秒，然後停"""
    motor_pair.move(motor_pair.PAIR_1, steering, velocity=200)
    utime.sleep_ms(ms)
    motor_pair.stop(motor_pair.PAIR_1)


while True:
    cam.update()
    hue = cam.hue(1)
    if hue < 10 or (hue >= 230 and hue < 240):    # 紅色物塊 → 從右邊繞過
        light_matrix.write("R")
        go(100, 450)      # 右轉約 45 度
        go(0, 1500)       # 直走約 15 公分
        go(-100, 450)     # 左轉回來
    elif hue >= 60 and hue <= 110:                # 綠色物塊 → 從左邊繞過
        light_matrix.write("G")
        go(-100, 450)
        go(0, 1500)
        go(100, 450)
    else:                                         # 沒看到物塊 → 慢慢往前
        motor_pair.move(motor_pair.PAIR_1, 0, velocity=150)
    utime.sleep_ms(20)
