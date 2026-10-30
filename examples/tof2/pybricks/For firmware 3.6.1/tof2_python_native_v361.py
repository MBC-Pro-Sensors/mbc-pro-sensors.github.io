from pybricks.parameters import Port
from pybricks.pupdevices import ColorSensor
from pybricks.tools import wait

# Set up.
Laser2 = ColorSensor(Port.B)


# The main program starts here.
while True:
    print(int(Laser2.reflection()), int(Laser2.ambient()))   # Pybricks 4.1.2 起回傳小數，先取整數
    wait(50)
