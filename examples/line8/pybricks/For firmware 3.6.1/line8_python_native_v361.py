# 注意：這是 Pybricks 3.6.1 用的舊範例，只適用 2026-10-30 以前的循行者韌體。新韌體的環境光改了編碼，用這裡的舊拆法線寬會讀成 ×10（3 變 30）；
#       Pybricks 4.x 請用 For firmware 4.x 的範例。/ Legacy example for Pybricks 3.6.1 and Pathfinder firmware before 2026-10-30 only;
#       newer firmware changed the ambient encoding (width reads ×10 here). For Pybricks 4.x use the For firmware 4.x examples.
from pybricks.hubs import PrimeHub
from pybricks.parameters import Direction, Port
from pybricks.pupdevices import ColorSensor, Motor
from pybricks.tools import wait

# Set up all devices.
prime_hub = PrimeHub()
line = ColorSensor(Port.C)
motorL = Motor(Port.E, Direction.COUNTERCLOCKWISE)
motorR = Motor(Port.F, Direction.CLOCKWISE)

# Initialize variables.
PD = 0
posLast = 0

def getLinePos8():
    return line.reflection() // 10 - 8

def getLineWidth8():
    return line.reflection() % 10

def getLinePos100():
    return round(line.ambient() // 1 - 100)

def getLineWidth100():
    return round((line.ambient() * 100) % 100)


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
