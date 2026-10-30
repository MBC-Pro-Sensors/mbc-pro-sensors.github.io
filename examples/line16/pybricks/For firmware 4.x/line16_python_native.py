from pybricks.hubs import PrimeHub
from pybricks.parameters import Direction, Port
from pybricks.pupdevices import ColorSensor, Motor
from pybricks.tools import wait
# 2026-10-29：Pybricks 4.1.2 的 reflection() 回傳小數，要先 int()（往 0 捨去）再拆
# 2026-10-30：ambient()＝細位置+100 . 線寬（小數第 1 位，16 路最多 9；要 2026-10-30 以後的韌體）

# Set up.
prime_hub = PrimeHub()
Line16 = ColorSensor(Port.C)
motorL = Motor(Port.E, Direction.COUNTERCLOCKWISE)
motorR = Motor(Port.F, Direction.CLOCKWISE)
PD = 0
lastError = 0

def getLinePos16():
    if int(Line16.reflection()) >= 0:
        return int(Line16.reflection()) // 100 - 16
    else:
        return (int(Line16.reflection()) + 6399) // 100 - 16

def getLineWidth16():
    if int(Line16.reflection()) >= 0:
        return int(Line16.reflection()) % 100
    else:
        return (int(Line16.reflection()) + 6399) % 100

def getLinePos100():
    return round(Line16.ambient() // 1 - 100)

def getLineWidth100():
    return round(Line16.ambient() * 10) % 10


# The main program starts here.
while True:
    print(getLinePos16(), getLineWidth16())
    if getLineWidth16() > 10:
        motorL.dc(0)
        motorR.dc(0)
    else:
        PD = getLinePos16() * 7 + (getLinePos16() - lastError) * 50
        motorL.dc(75 + PD)
        motorR.dc(75 - PD)
        lastError = getLinePos16()
        wait(10)
