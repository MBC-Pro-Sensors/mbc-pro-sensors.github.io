from pybricks.hubs import PrimeHub
from pybricks.parameters import Port
from pybricks.pupdevices import ColorSensor
from pybricks.tools import wait

# Set up all devices.
prime_hub = PrimeHub()
PS4 = ColorSensor(Port.C)

def PS4_LX():
    return (round(PS4.ambient() * 100) // 1000) % 10

def PS4_UP():
    return (PS4.reflection() // 1) % 2

def PS4_LY():
    return (round(PS4.ambient() * 100) // 100) % 10

def PS4_Right():
    return (PS4.reflection() // 2) % 2

def PS4_RX():
    return (round(PS4.ambient() * 100) // 10) % 10

def PS4_Down():
    return (PS4.reflection() // 4) % 2

def PS4_Left():
    return (PS4.reflection() // 8) % 2

def PS4_RY():
    return (round(PS4.ambient() * 100) // 1) % 10

def PS4_Tri():
    return (PS4.reflection() // 16) % 2

def PS4_Cir():
    return (PS4.reflection() // 32) % 2

def PS4_Crs():
    return (PS4.reflection() // 64) % 2

def PS4_Sqr():
    return (PS4.reflection() // 128) % 2

def PS4_L1():
    return (PS4.reflection() // 256) % 2

def PS4_R1():
    return (PS4.reflection() // 512) % 2

def PS4_Sel():
    return (PS4.reflection() // 1024) % 2


# The main program starts here.
while True:
    print(PS4_UP(), PS4_Right(), PS4_Down(), PS4_Left(), PS4_Tri(), PS4_Cir(), PS4_Crs(), PS4_Sqr(), PS4_L1(), PS4_R1(), PS4_Sel(), PS4_LX(), PS4_LY(), PS4_RX(), PS4_RY(), sep=",")
    wait(10)
