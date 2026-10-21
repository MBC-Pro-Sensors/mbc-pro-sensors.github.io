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

from hub import light_matrix

cam = HUE12(port.C)                 # HUE12 插在 C 孔

while True:
    cam.update()                    # 讀一次最新資料
    hue = cam.hue(1)                # 第 1 格的色相
    print(hue, cam.sat(1))          # 電腦畫面印出 色相、飽和度
    light_matrix.write(str(hue))    # 主機螢幕顯示色相
    utime.sleep_ms(500)             # 等 0.5 秒
