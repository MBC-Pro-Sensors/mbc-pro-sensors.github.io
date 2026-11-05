# 範例 06：系統與底層數據 (System & Misc)
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
print(" 範例 06：系統與底層數據測試")
print("==============================")

exp = MBC_EXP6(port.F)
time.sleep(1)

# ---------------------------------------------------------
# API 展示：get_voltage()
# ---------------------------------------------------------
print("\n[展示 1] get_voltage() - 讀取擴充板電池電壓")
voltage = exp.get_voltage()
print("目前擴充板電池電壓: {} V".format(voltage))
if voltage < 7.0:
    print("⚠️ 警告：電池電壓過低，請盡快充電！")

# ---------------------------------------------------------
# API 展示：get_device_id(port)
# ---------------------------------------------------------
print("\n[展示 2] get_device_id() - 讀取感測器裝置代碼")
# 回傳該孔實際接的裝置：0 無、1 力量、3 顏色、4 超音波、5 馬達、7 其他（開機約 1 秒內馬達還沒準備好會是 0）
dev_id = exp.get_device_id(1)
print("1 號孔裝置代碼: {}".format(dev_id))

# ---------------------------------------------------------
# API 展示：set_port_mode(port, mode) 與 get_port_raw(port)
# ---------------------------------------------------------
print("\n[展示 3] 底層模式切換與原始數據讀取")
print("這通常由高階 API 自動處理，但您可以手動覆寫。")

print("將 6 號孔強制設定為距離模式 (MODE_DISTANCE)...")
exp.set_port_mode(6, exp.MODE_DISTANCE)
time.sleep(0.1)

print("直接讀取 6 號孔從底層 LPF2 傳來的 raw data:")
raw_data = exp.get_port_raw(6)
print("Raw Data: {}".format(raw_data))

print("\n全部 API 測試結束！")
