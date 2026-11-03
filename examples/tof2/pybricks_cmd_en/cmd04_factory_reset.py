# Ranger 2-Way command example 04 (Pybricks, SPIKE Prime / Robot Inventor hub): factory reset
# Factory reset (command E3, the parameter must be 0xA5): first set the max distance to 30 cm and see the reading "capped" at 30;
# press the center button to send E3 → the limit is back to 120 cm and far objects show up again
# Setup: a whiteboard about 60 cm in front of both lasers (farther than 30, nearer than 120)
# Wiring: Ranger 2-Way → port C; upload it together with MBC_laser2_obj_Lib.py (keep both files in Pybricks Code)
# Only the SPIKE edition takes commands (sensor firmware 2026-11 or later); settings stay in the sensor memory and go back to factory when unplugged
#   cmd(code, parameter): send a command; wait_done(): wait until it is done, True = done, False = failed (parameter out of range) or no reply
#   Commands: E0 ranging mode, E1 filter, E2 timing budget, E3 factory reset (parameter 0xA5), E4 max distance
from pybricks.hubs import PrimeHub
from pybricks.parameters import Button, Color
from pybricks.tools import wait, StopWatch
from MBC_laser2_obj_Lib import MBC_LASER2

hub = PrimeHub()
laser2 = MBC_LASER2(3)                   # 3 = port C (1=A, 2=B, 3=C, 4=D, 5=E, 6=F)

laser2.cmd(0xE4, 30)                # Set the max distance to 30 cm first (beyond 30 = nothing, reads 30)
laser2.wait_done()
hub.display.number(30)
print('Max distance 30 cm: press the center button to send E3 factory reset')
hub.system.set_stop_button(None)   # Use the center button in the program (to stop the program: hold the center button)

t = StopWatch()
while True:
    if Button.CENTER in hub.buttons.pressed():   # Check the buttons every 10 ms
        laser2.cmd(0xE3, 0xA5, again=True)   # Factory reset; again=True: run the same command again too
        ok = laser2.wait_done()
        hub.display.number(120)
        hub.speaker.beep()
        print('factory reset', 'done' if ok else 'failed', ': limit back to 120 cm')
        while hub.buttons.pressed():
            wait(10)
    if t.time() >= 300:                # Print the distances every 0.3 s
        t.reset()
        print('left', laser2.left(), 'right', laser2.right(), 'cm | left', laser2.left_mm(), 'mm')
    wait(10)
