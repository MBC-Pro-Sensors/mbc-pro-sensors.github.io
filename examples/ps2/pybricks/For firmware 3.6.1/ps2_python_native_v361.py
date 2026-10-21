from pybricks.hubs import PrimeHub
from pybricks.parameters import Port
from pybricks.pupdevices import ColorSensor
from pybricks.tools import wait

# Set up all devices.
prime_hub = PrimeHub()
PS2 = ColorSensor(Port.C)

def PS2_LX():
    return (round(PS2.ambient() * 100) // 1000) % 10

def PS2_Up():
    return (PS2.reflection() // 1) % 2

def PS2_LY():
    return (round(PS2.ambient() * 100) // 100) % 10

def PS2_Right():
    return (PS2.reflection() // 2) % 2

def PS2_RX():
    return (round(PS2.ambient() * 100) // 10) % 10

def PS2_Down():
    return (PS2.reflection() // 4) % 2

def PS2_Left():
    return (PS2.reflection() // 8) % 2

def PS2_RY():
    return (round(PS2.ambient() * 100) // 1) % 10

def PS2_Tri():
    return (PS2.reflection() // 16) % 2

def PS2_Cir():
    return (PS2.reflection() // 32) % 2

def PS2_Crs():
    return (PS2.reflection() // 64) % 2

def PS2_Sqr():
    return (PS2.reflection() // 128) % 2

def PS2_L1():
    return (PS2.reflection() // 256) % 2

def PS2_R1():
    return (PS2.reflection() // 512) % 2

def PS2_Sel():
    return (PS2.reflection() // 1024) % 2


# The main program starts here.
while True:
    print(PS2_Up(), PS2_Right(), PS2_Down(), PS2_Left(), PS2_Tri(), PS2_Cir(), PS2_Crs(), PS2_Sqr(), PS2_L1(), PS2_R1(), PS2_Sel(), PS2_LX(), PS2_LY(), PS2_RX(), PS2_RY(), sep=",")
    wait(10)
