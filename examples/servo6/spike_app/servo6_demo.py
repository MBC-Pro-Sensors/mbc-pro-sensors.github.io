# Servo 擴充板上機測試（官方 SPIKE App）。擴充板插在 Port A，舵機插 CN1（定點）、CN2（360° 速度）。
# 先把 MBC_servo6_SPIKE_App_Lib.py 的內容貼在本程式最上方（或依你們的匯入方式載入）。
from hub import port
import time
from MBC_servo6_SPIKE_App_Lib import MBC_SERVO6

sv = MBC_SERVO6(port.A)

# 定點模式：立即到 90°，再以 60°/s 慢慢轉到 0° 與 180°
sv.servo_angle(1, 90)
time.sleep_ms(1000)
sv.servo_angle(1, 0, 10)
sv.servo_wait(1)
sv.servo_angle(1, 180, 10)
sv.servo_wait(1)
print("CN1 電流 mA:", sv.get_current(1), " 總電流 mA:", sv.get_total_current())

# 速度模式：正轉 2 秒、反轉 2 秒、停
sv.servo_run(2, 50)
time.sleep_ms(2000)
sv.servo_run(2, -50)
time.sleep_ms(2000)
sv.servo_run(2, 0)
time.sleep_ms(500)

print("CN1 堵轉:", sv.is_stalled(1), " eFuse 故障:", sv.is_power_fault())
sv.stop_all()
