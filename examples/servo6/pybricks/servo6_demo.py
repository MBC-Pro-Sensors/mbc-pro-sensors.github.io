# Servo 擴充板上機測試（Pybricks，sync）。擴充板插在 Port A，舵機插 CN1（定點）、CN2（360° 速度）。
# 第一次執行主控台會問 "Press Y to proceed." → 按 Y。
from pybricks.tools import wait
from MBC_servo6_Lib import MBC_SERVO6

sv = MBC_SERVO6(1, False)

# 定點模式：立即到 90°，再以 60°/s 慢慢轉到 0° 與 180°
sv.servo_angle(1, 90)
wait(1000)
sv.servo_angle(1, 0, 10)
sv.servo_wait(1)
sv.servo_angle(1, 180, 10)
while sv.is_moving(1):
    print("CN1 電流 mA:", sv.get_current(1), " 總電流 mA:", sv.get_total_current())
    wait(100)

# 速度模式：正轉 2 秒、反轉 2 秒、停
sv.servo_run(2, 50)
wait(2000)
sv.servo_run(2, -50)
wait(2000)
sv.servo_run(2, 0)
wait(500)

print("CN1 堵轉:", sv.is_stalled(1), " eFuse 故障:", sv.is_power_fault())
sv.stop_all()
