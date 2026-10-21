from pybricks.parameters import Port
from pybricks.pupdevices import ColorSensor
from pybricks.tools import wait

# Set up.
Laser2 = ColorSensor(Port.B)


# The main program starts here.
while True:
    print(Laser2.reflection(), Laser2.ambient())
    wait(50)
