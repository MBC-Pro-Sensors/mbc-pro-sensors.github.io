# 範例 05：進階色彩分析 (Color Sensor Pro)
# ℹ️ 版本提醒 (Version Reminder):
# [ZH] 本程式適用於 LEGO MINDSTORMS Robot Inventor App 10.x（玩具版 51515）
# [EN] This program is designed for LEGO MINDSTORMS Robot Inventor App 10.x (51515)
#
# ⚠️ 測試提醒 (Testing Reminder):
# [ZH] 執行此範例前，請確保已經在主機中執行過 `Install_MBC_exp6_Lib.py` 安裝函數庫。
# [EN] Before running this example, please ensure you have executed `Install_MBC_exp6_Lib.py` to install the library.

from hub import port
import time
# 玩具版：函式庫裝在主機的 /projects/mbc_exp6/（先執行一次 Install_MBC_exp6_Lib.py）
from projects.mbc_exp6.MBC_exp6_SPIKE_App_Lib import MBC_EXP6

print("==============================")
print(" 範例 05：進階色彩分析測試")
print("==============================")
print("請將顏色感測器接在擴充板 5 號孔")

exp = MBC_EXP6(port.F)
time.sleep(1)

PORT_COLOR = 5

for i in range(5):
    print("\n--- 第 {} 次取樣 ---".format(i + 1))

    # ---------------------------------------------------------
    # API 展示：官方色碼與反射光
    # ---------------------------------------------------------
    color_id = exp.get_color_color(PORT_COLOR)
    reflection = exp.get_color_reflection(PORT_COLOR)
    print("官方色碼 (0~10): {}".format(color_id))
    print("反射光強度 (0~100): {}".format(reflection))
    
    # ---------------------------------------------------------
    # API 展示：RGB 三原色讀取
    # ---------------------------------------------------------
    # 可單獨讀取，也可一次讀取 tuple
    r, g, b = exp.get_color_rgb(PORT_COLOR)
    red = exp.get_color_red(PORT_COLOR)
    green = exp.get_color_green(PORT_COLOR)
    blue = exp.get_color_blue(PORT_COLOR)
    print("RGB 陣列: ({}, {}, {})".format(r, g, b))
    
    # ---------------------------------------------------------
    # API 展示：HSV 色彩空間 (可用於精細的循線或顏色辨識演算法)
    # ---------------------------------------------------------
    # H: 色相 (Hue 0~360), S: 飽和度 (Saturation 0~100), V: 明度 (Value 0~100)
    h, s, v = exp.get_color_hsv(PORT_COLOR)
    hue = exp.get_color_hue(PORT_COLOR)
    sat = exp.get_color_sat(PORT_COLOR)
    val = exp.get_color_val(PORT_COLOR)
    print("HSV 色彩: H={}°, S={}%, V={}%".format(hue, sat, val))
    
    time.sleep(1)

print("\n測試結束！")
