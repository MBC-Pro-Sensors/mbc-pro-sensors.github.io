# Ranger 2-Way command example 01 (Pybricks, SPIKE Prime / Robot Inventor hub): ranging mode: normal / long range / accurate / fast
# Switch between the 4 ranging modes (command E0) with the left / right buttons on the hub; the screen shows the mode and the console prints the distances:
#   0 normal (factory, up to 120 cm), 1 long range (up to 200 cm), 2 accurate (every 100 ms, steadiest), 3 fast (every 20 ms, fastest)
# Setup: a white wall or big whiteboard about 150 cm in front of both lasers. Normal mode reads 120 (beyond the limit = nothing); mode 1 reads about 150
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

mode = 0
laser2.cmd(0xE3, 0xA5)              # Factory reset first (normal mode)
laser2.wait_done()
hub.display.number(mode)
t = StopWatch()

while True:
    pressed = hub.buttons.pressed()    # Check the buttons every 10 ms, so one press is enough
    if Button.RIGHT in pressed or Button.LEFT in pressed:
        mode = (mode + (1 if Button.RIGHT in pressed else 3)) % 4   # right button +1, left button -1
        laser2.cmd(0xE0, mode)        # Change the ranging mode (re-configures the laser chips, about 0.1~0.5 s)
        ok = laser2.wait_done()       # Wait until done: True = done
        hub.display.number(mode)
        print('mode', mode, ['normal', 'long range', 'accurate', 'fast'][mode], 'done' if ok else 'failed')
        while hub.buttons.pressed():   # Wait until the button is released
            wait(10)
    if t.time() >= 300:                # Print the distances every 0.3 s
        t.reset()
        print('left', laser2.left(), 'right', laser2.right(), 'cm | left', laser2.left_mm(), 'mm')
    wait(10)
