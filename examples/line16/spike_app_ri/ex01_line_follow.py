# 循行者 16 路 × 樂高 MINDSTORMS Robot Inventor App（玩具版 Python）範例 1：循線＋十字路口停車
# Pathfinder 16-Way × LEGO MINDSTORMS Robot Inventor App (Python) example 1: line following + stop at a crossing
# 接線：循行者 16 路 → C 孔；左輪 E、右輪 F / Wiring: Pathfinder 16 → port C; left wheel E, right wheel F
# 方向：站在車後往前看，線在左＝負 / standing behind the robot: line on the left = negative
# 看不到線時細位置停在 −100 或 +100，車子會自己往那一側轉回去找線 / when the line is lost, the robot turns back toward the last side
# ===== 循行者 16 路 讀值小工具（樂高 MINDSTORMS Robot Inventor App 玩具版 Python，照抄就好、不用改）=====
# ===== Pathfinder 16-Way reading helper (LEGO MINDSTORMS Robot Inventor App, Python: copy as is) =====
# 循行者 16 路在 Robot Inventor App 裡是「顏色感應器」：反射光＝線寬、RGB 強度第 1 個值＝細位置＋100（2026-10-29 實機量過）。
# In the Robot Inventor App the Pathfinder 16 is a colour sensor: reflected light = line width, first RGB intensity value = fine position + 100.
# get_color() 只有少數位置有顏色名稱，不能當位置；粗位置和每一路的光值這個 App 讀不到，要用請改用 Pybricks。
# get_color() only names a few positions, so it is not a position; coarse position and per-channel readings need Pybricks.
# LEGO 已在 2026-10-01 停止支援 Robot Inventor App；新專案建議用 SPIKE App 3 或 Pybricks。
# LEGO ended support for the Robot Inventor App on 2026-10-01; new projects should use SPIKE App 3 or Pybricks.
# 站在車後往前看：線在左＝負、在右＝正 / seen from behind the robot: line on the left = negative
from mindstorms import ColorSensor


class Line16RI:
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
        """線寬 0~16：幾路踩在線上，0＝看不到線 / line width 0~16: channels on the line, 0 = no line"""
        return self.r
# ===== 小工具到這裡結束 / end of helper =====
from mindstorms import MSHub, MotorPair
from mindstorms.control import wait_for_seconds

hub = MSHub()
line16 = Line16RI('C')
wheels = MotorPair('E', 'F')
BASE = 30         # 👉 可以改：前進速度（%）/ forward speed (%)
KP = 0.3          # 👉 可以改：轉彎靈敏度，車子左右甩就調小 / steering gain, lower it if the robot wobbles
CROSS = 11       # 👉 可以改：線寬到這麼寬就當十字路口 / this wide = a crossing

while True:
    line16.update()
    if line16.width() >= CROSS:                   # 整排都踩到：停車嗶一聲 / whole row on the line: stop and beep
        wheels.stop()
        hub.speaker.beep(72, 0.3)
        break
    error = line16.pos100()                       # 線在左＝負 → 左輪慢 → 往左轉 / left = negative → left wheel slower
    wheels.start_tank(int(BASE + KP * error), int(BASE - KP * error))
    wait_for_seconds(0.01)
