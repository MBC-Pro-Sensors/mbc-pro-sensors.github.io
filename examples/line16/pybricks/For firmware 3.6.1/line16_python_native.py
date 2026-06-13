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
lastError = 0

def getLinePos16():
    if line.reflection() >= 0:
        return line.reflection() // 100 - 16
    else:
        return (line.reflection() + 6399) // 100 - 16

def getLineWidth16():
    if line.reflection() >= 0:
        return line.reflection() % 100
    else:
        return (line.reflection() + 6399) % 100

def getLinePos100():
    return round(line.ambient() // 1 - 100)

def getLineWidth100():
    return round((line.ambient() * 100) % 100)


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
        wait(5)
