# ============================================================
# IMU 陀螺儀感應器 EV3 新手範例 07：轉太快就警告
# 軟體：Pybricks v4（用 Chrome 打開 https://code.pybricks.com ，EV3 版目前是 Beta）
# 接法：IMU 陀螺儀插 1 號孔
# 這個範例會做什麼：模式 2 是「轉向角速度」：現在每秒轉幾度（×10）。角度告訴你轉了多少，角速度告訴你轉得多快。這個範例把角速度顯示出來，轉太快（超過每秒 90 度，也就是 900）就嗶一聲，可以拿來檢查機器人轉彎會不會甩尾。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port, Color
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = LUMPDevice(Port.S1)       # IMU 陀螺儀插 1 號孔（當一般 UART 裝置讀原始值）


def read_rate():
    # 轉向的角速度（每秒幾度 × 10）：±3000 左右，正負跟角度同方向；100＝每秒 10 度
    return sensor.read(2)[0]


# ---------- 程式從這裡開始 ----------
# 清除螢幕
ev3.screen.clear()
# 一直重複
while True:
    # 第 1 行顯示角速度（每秒幾度 × 10）
    ev3.screen.draw_text(0, 0, "RATE " + str(read_rate()) + "      ", Color.BLACK, Color.WHITE)
    # 如果角速度的絕對值大於 900（每秒 90 度）
    if abs(read_rate()) > 900:
        # 轉太快（超過每秒 90 度）：嗶一聲
        ev3.speaker.beep()
    # 等 0.1 秒
    wait(100)
