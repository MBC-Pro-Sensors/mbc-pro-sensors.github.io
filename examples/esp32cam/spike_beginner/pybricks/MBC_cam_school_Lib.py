# MBC 神攝手 ESP32CAM（學校老師創意賽版）Pybricks 小函式庫
# 把這個檔案和範例程式一起匯入 code.pybricks.com 就能用。
#
#   from MBC_cam_school_Lib import MBC_CAM_SCHOOL
#   cam = MBC_CAM_SCHOOL(Port.C)      # 神攝手插在 C 孔
#   cam.hue()                         # 色相
#
# 原理：感應器的「模式 9」一次送 8 個數字，Pybricks 讀成 7 個 16 位元整數，
# 每個整數裡面裝 2 個數字（低位元組在前、高位元組在後）。
#
# 日期：2026.10　v1.1：新增 area()（模式 9 第 8~9 個位元組 = 色塊面積）、blob_xya()（模式 6 = [X, Y, 面積]）、
#   device_id()／firmware_version()（模式 8）。原本的 read_all()～found() 一個字都沒改。
#   面積與模式 6、8 要感應器韌體 2026-10 以後的版本；舊韌體的面積一直是 0。
#   ★ blob_xya()、device_id() 會讓感應器暫時切到別的模式（幾十 ms），下一個模式 9 的方法再切回來；
#     同一個迴圈裡不要 blob_x() 和 blob_xya() 混著用。
from pybricks.iodevices import PUPDevice


def _byte(v, k):
    """取出第 k 個數字（-128 ~ 127）。第 k 個數字放在第 k//2 個整數裡，偶數是低位元組、奇數是高位元組。"""
    w = v[k // 2]
    b = ((w >> 8) & 0xFF) if k % 2 else (w & 0xFF)
    return b - 256 if b > 127 else b


class MBC_CAM_SCHOOL:
    def __init__(self, port):
        self._dev = PUPDevice(port)

    def read_all(self):
        """一次讀 8 個數字：[色相, 飽和度, 明度, 動態值, 色塊X, 色塊Y, 色塊寬, 色塊高]"""
        v = self._dev.read(9)
        return [_byte(v, k) for k in range(8)]

    def hue(self):
        """色相：0~100（紅 0／100、黃約 17、綠約 33、青約 50、藍約 67、洋紅約 83）；-1 = 黑白灰（沒有顏色）。
        注意：沒在感應器上設定 1:COLOR 時一直是 0（跟紅色一樣）。"""
        return self.read_all()[0]

    def saturation(self):
        """飽和度 0~100：越大顏色越鮮豔"""
        return self.read_all()[1]

    def brightness(self):
        """明度 0~100：越大越亮"""
        return self.read_all()[2]

    def motion(self):
        """動態值 0~100：整個畫面跟上一張比，變化越大數字越大（機器人要停住才準）"""
        return self.read_all()[3]

    def blob_x(self):
        """色塊 X：左 0 ~ 右 98，正中間約 50；沒找到是 0"""
        return self.read_all()[4]

    def blob_y(self):
        """色塊 Y：下 1 ~ 上 75（下面小、上面大），中間約 37；沒找到是 0"""
        return self.read_all()[5]

    def blob_w(self):
        """色塊寬 0~98；沒找到是 0"""
        return self.read_all()[6]

    def blob_h(self):
        """色塊高 0~73；沒找到是 0"""
        return self.read_all()[7]

    def found(self):
        """有沒有找到 2:TRACK 設定的色塊（模式 9 沒有面積，所以用「寬 > 0」判斷）"""
        return self.blob_w() > 0

    # ───── 以下是 v1.1（2026-10）新增 ─────
    def area(self):
        """色塊面積（寬 × 高，0~7500）：沒找到是 0。模式 9 的第 5 個整數（第 8~9 個位元組，無號 16 位元）。
        跟 blob_x() 等一樣讀模式 9，不用換模式。感應器韌體 2026-10 以後才有，舊韌體一直是 0。"""
        return int(self._dev.read(9)[4]) & 0xFFFF

    def blob_xya(self):
        """一次讀 [色塊X, 色塊Y, 面積]（模式 6，3 個 16 位元整數）。沒找到是 [0, 0, 0]。"""
        v = self._dev.read(6)
        return [int(v[0]), int(v[1]), int(v[2])]

    def device_id(self):
        """(id, hw0, 韌體版字)：id = 0x4C（76），hw0 = 1（學校版），韌體版字 = 主版×100＋次版。模式 8。"""
        v = self._dev.read(8)
        w = int(v[0]) & 0xFFFF
        return (w & 0xFF, (w >> 8) & 0xFF, int(v[2]) & 0xFFFF)

    def firmware_version(self):
        """感應器韌體版本 (主版, 次版)，例如 (1, 0)"""
        fw = self.device_id()[2]
        return (fw // 100, fw % 100)
