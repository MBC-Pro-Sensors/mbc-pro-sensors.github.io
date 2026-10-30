# 循行者 8 路 PD 循線示範（速度 50）× 樂高官方 MINDSTORMS Robot Inventor App 玩具版 Python
# Pathfinder 8 PD line following demo (speed 50) × LEGO MINDSTORMS Robot Inventor App, Python
# 接線：循行者 → C 孔；左馬達 → E 孔；右馬達 → F 孔。左右兩顆馬達裝的方向相反，左馬達要給「負的」功率車子才會往前
# Wiring: sensor → C, left motor → E, right motor → F. The motors face opposite ways: give the left motor NEGATIVE power to drive forward
# 方向：站在車後往前看，線在左＝負 → 左輪慢、右輪快 → 往左轉／line on the left = negative → left wheel slower → turns left
# KP、KD 是建議值，請在你的車上調整／KP and KD are starting values, tune them on your robot
# 積木版：同名的 .lms 檔／Block version: the matching .lms file
# ===== 循行者 8 路 讀值小工具（樂高 MINDSTORMS Robot Inventor App 玩具版 Python，照抄就好、不用改）=====
# ===== Pathfinder 8-Way reading helper (LEGO MINDSTORMS Robot Inventor App, Python: copy as is) =====
# 循行者在 Robot Inventor App 裡是「顏色感應器」：反射光＝線寬、RGB 強度第 1 個值＝細位置＋100（2026-10-29 實機量過）。
# In the Robot Inventor App the Pathfinder is a colour sensor: reflected light = line width, first RGB intensity value = fine position + 100.
# get_color() 只有少數位置有顏色名稱，不能當位置；粗位置和每一路的光值這個 App 讀不到，要用請改用 Pybricks。
# get_color() only names a few positions, so it is not a position; coarse position and per-channel readings need Pybricks.
# LEGO 已在 2026-10-01 停止支援 Robot Inventor App；新專案建議用 SPIKE App 3 或 Pybricks。
# LEGO ended support for the Robot Inventor App on 2026-10-01; new projects should use SPIKE App 3 or Pybricks.
# 站在車後往前看：線在左＝負、在右＝正 / seen from behind the robot: line on the left = negative
from mindstorms import ColorSensor


class Line8RI:
    def __init__(self, port_letter):
        self.sensor = ColorSensor(port_letter)  # 孔位字母，例如 'C' / port letter, e.g. 'C'
        self.r = 0
        self.red = 100

    def update(self):
        """讀一次最新資料 / read once"""
        self.r = int(self.sensor.get_reflected_light()) & 255           # 線寬 / line width
        self.red = int(self.sensor.get_rgb_intensity()[0]) & 0xFFFF     # 細位置＋100 / fine position + 100
        return True

    def pos100(self):
        """細位置 −100~+100，循線用這個；看不到線時停在最後離開的那一側（−100 或 +100）/ fine position −100~+100 for line following; keeps the last side (−100 or +100) when the line is lost"""
        return self.red - 100

    def width(self):
        """線寬 0~8：幾路踩在線上，0＝看不到線 / line width 0~8: channels on the line, 0 = no line"""
        return self.r
# ===== 小工具到這裡結束 / end of helper =====

from mindstorms import Motor
from mindstorms.control import wait_for_seconds

line8 = Line8RI('C')
motorL = Motor('E')
motorR = Motor('F')
SPEED = 50     # 前進速度（%）／forward speed (%)
KP = 0.4       # 轉彎靈敏度：車子左右甩就調小／steering gain: lower it if the robot wobbles
KD = 1.5       # 抑制甩動：轉彎太慢就調小／damping: lower it if turns are too slow
CROSS = 5    # 線寬大於這個值＝十字路口（整排踩到線）：停車／wider than this = crossing: stop


def main():
    last = 0
    while True:
        line8.update()
        error = line8.pos100()     # 細位置 −100~100（線在左＝負）／fine position, left = negative
        width = line8.width()      # 線寬：幾路踩在線上／line width
        if width > CROSS:
            motorL.stop()
            motorR.stop()
        else:
            PD = error * KP + (error - last) * KD     # P：偏多少轉多少；D：偏移變快時多轉一點
            last = error
            motorL.start_at_power(int(-(SPEED + PD)))   # 左馬達裝反了：給負的
            motorR.start_at_power(int(SPEED - PD))
        wait_for_seconds(0.01)

main()
