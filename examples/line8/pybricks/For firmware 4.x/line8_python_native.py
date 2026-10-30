from pybricks.hubs import PrimeHub
from pybricks.parameters import Direction, Port
from pybricks.pupdevices import ColorSensor, Motor
from pybricks.tools import wait
# 2026-10-29：Pybricks 4.1.2 的 reflection() 回傳小數，要先 int()（往 0 捨去）再拆
# 2026-10-30：ambient()＝細位置+100 . 線寬（小數第 1 位，16 路最多 9；要 2026-10-30 以後的韌體）

# Set up all devices.
prime_hub = PrimeHub()
line = ColorSensor(Port.C)
motorL = Motor(Port.E, Direction.COUNTERCLOCKWISE)
motorR = Motor(Port.F, Direction.CLOCKWISE)

# Initialize variables.
PD = 0
posLast = 0

def getLinePos8():
    return int(line.reflection()) // 10 - 8

def getLineWidth8():
    return int(line.reflection()) % 10

def getLinePos100():
    return round(line.ambient() // 1 - 100)

def getLineWidth100():
    return round(line.ambient() * 10) % 10


# The main program starts here.
while True:
    print(getLinePos100(), getLineWidth100())
    if getLineWidth100() > 6:
        motorL.dc(0)
        motorR.dc(0)
    else:
        PD = getLinePos100() * 1.2 + (getLinePos100() - posLast) * 3.6
        motorL.dc(75 + PD)
        motorR.dc(75 - PD)
        posLast = getLinePos100()
        wait(10)
