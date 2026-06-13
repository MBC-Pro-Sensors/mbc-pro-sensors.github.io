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
