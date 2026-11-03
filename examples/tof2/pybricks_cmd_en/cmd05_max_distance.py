# Ranger 2-Way command example 05 (Pybricks, SPIKE Prime / Robot Inventor hub): max distance: only near things count
# Max distance (command E4, cm): anything beyond the limit counts as "nothing" and reads as the limit → it only reacts to near things
# Left / right buttons change the limit to 10, 20, 30 ... 120 cm (shown on the screen); if the nearer distance is closer than the limit: red light + beep, otherwise green
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

limit = 30
laser2.cmd(0xE3, 0xA5)              # Factory reset first
laser2.wait_done()
laser2.cmd(0xE4, limit)             # Max distance 30 cm
laser2.wait_done()
hub.display.number(limit)
t = StopWatch()

while True:
    pressed = hub.buttons.pressed()    # Check the buttons every 10 ms
    if Button.RIGHT in pressed or Button.LEFT in pressed:
        limit = limit + 10 if Button.RIGHT in pressed else limit - 10
        limit = max(10, min(120, limit))
        laser2.cmd(0xE4, limit)       # Change the limit (takes effect at once)
        laser2.wait_done()
        hub.display.number(limit)
        while hub.buttons.pressed():
            wait(10)
    if t.time() >= 200:                # Check the distance every 0.2 s
        t.reset()
        cm = min(laser2.left(), laser2.right())
        if cm < limit:               # Closer than the limit = something is really there
            hub.light.on(Color.RED)
            hub.speaker.beep(1000, 50)
        else:
            hub.light.on(Color.GREEN)
        print('limit', limit, 'cm |', 'left', laser2.left(), 'right', laser2.right(), 'cm | left', laser2.left_mm(), 'mm')
    wait(10)
