from pybricks.ev3devices import Motor
from pybricks.hubs import EV3Brick
from pybricks.parameters import Direction, Port
from pybricks.tools import wait

from MBC_line8_EV3_Lib import (
    line_edge_l,
    line_edge_r,
    line_init_port_multitask,
    line_ir_ch,
    line_pos100,
    line_pos8,
    line_width,
)

# Set up.
ev3 = EV3Brick()
motorL = Motor(Port.B, Direction.CLOCKWISE)
motorR = Motor(Port.C, Direction.CLOCKWISE)
PID = 0
posLast = 0


# The main program starts here.
line_init_port_multitask(4, False)
while True:
    print(line_pos100(), line_width(), line_edge_l(), line_edge_r())
    if line_width() > 10:
        motorL.dc(0)
        motorR.dc(0)
    else:
        PID = line_pos100() * 1.2 + (line_pos100() - posLast) * 3.6
        posLast = line_pos100()
        motorL.dc(75 + PID)
        motorR.dc(75 - PID)
    wait(10)
