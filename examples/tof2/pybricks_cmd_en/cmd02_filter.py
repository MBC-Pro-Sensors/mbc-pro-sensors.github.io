# Ranger 2-Way command example 02 (Pybricks, SPIKE Prime / Robot Inventor hub): filter: steadier numbers
# Switch the filter (command E1) with the left / right buttons: 0 off, 1 median of the last 3 (factory), 2 median of the last 5
# Setup: a box or whiteboard about 50 cm in front of both lasers, kept still. Every second it prints "max - min within 1 s" = how many mm the number jumps; a bigger filter is steadier
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

filt = 1
laser2.cmd(0xE3, 0xA5)              # Factory reset first (filter 1)
laser2.wait_done()
wait(500)                        # Wait for fresh readings before counting
hub.display.number(filt)

while True:
    lo, hi = 99999, -1
    t = StopWatch()
    while t.time() < 1000:           # Keep reading for 1 s and remember the max and min
        mm = laser2.left_mm()
        lo = min(lo, mm)
        hi = max(hi, mm)
        pressed = hub.buttons.pressed()
        if Button.RIGHT in pressed or Button.LEFT in pressed:
            filt = (filt + (1 if Button.RIGHT in pressed else 2)) % 3
            laser2.cmd(0xE1, filt)      # Change the filter (takes effect at once)
            laser2.wait_done()
            hub.display.number(filt)
            print('filter', filt)
            while hub.buttons.pressed():
                wait(10)
            lo, hi = 99999, -1
            t.reset()
        wait(20)
    print('filter', filt, '| left mm', lo, '~', hi, '| jumps', hi - lo, 'mm')
