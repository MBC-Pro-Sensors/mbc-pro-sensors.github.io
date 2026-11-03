# Ranger 2-Way command example 03 (Pybricks, SPIKE Prime / Robot Inventor hub): timing budget: faster or more accurate
# Switch the timing budget (command E2) with the left / right buttons: 20, 33 (factory), 100, 200 ms
# Setup: a box or whiteboard about 50 cm in front of both lasers, kept still. Every second it prints "how many times the value changed" (≈ updates) and "how many mm it jumps"
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

CHOICES = [20, 33, 100, 200]
k = 1
laser2.cmd(0xE3, 0xA5)              # Factory reset first (33 ms)
laser2.wait_done()
laser2.cmd(0xE1, 0)                 # Filter off, so the difference is easier to see
laser2.wait_done()
wait(500)                        # Wait for fresh readings before counting
hub.display.number(CHOICES[k])

while True:
    last, changes, lo, hi = -1, 0, 99999, -1
    t = StopWatch()
    while t.time() < 1000:
        mm = laser2.left_mm()
        if mm != last:
            changes += 1
            last = mm
        lo = min(lo, mm)
        hi = max(hi, mm)
        pressed = hub.buttons.pressed()
        if Button.RIGHT in pressed or Button.LEFT in pressed:
            k = (k + (1 if Button.RIGHT in pressed else 3)) % 4
            laser2.cmd(0xE2, CHOICES[k])    # Change the timing budget (re-configures the laser chips)
            ok = laser2.wait_done()
            hub.display.number(CHOICES[k])
            print('timing budget', CHOICES[k], 'ms', 'done' if ok else 'failed')
            while hub.buttons.pressed():
                wait(10)
            last, changes, lo, hi = -1, 0, 99999, -1
            t.reset()
        wait(5)
    print(CHOICES[k], 'ms | changed', changes, 'times in 1 s | jumps', hi - lo, 'mm')
