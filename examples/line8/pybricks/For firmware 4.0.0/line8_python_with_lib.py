from pybricks.hubs import PrimeHub
from pybricks.parameters import Direction, Port
from pybricks.pupdevices import Motor
from pybricks.tools import multitask, run_task, wait

from MBC_line8_obj_Lib import MBC_LINE8

# Set up.
line8 = MBC_LINE8(3, True)
prime_hub = PrimeHub()
motorL = Motor(Port.E, Direction.COUNTERCLOCKWISE)
motorR = Motor(Port.F, Direction.CLOCKWISE)
PD = 0
posLast = 0

async def subtask():
    while True:
        await wait(0)
        print(await line8.pos8(), await line8.pos100(), await line8.ir_ch(1))
        await wait(10)

async def subtask2():
    global PD, posLast
    while True:
        await wait(0)
        if await line8.width() > 6:
            motorL.dc(0)
            motorR.dc(0)
        else:
            PD = await line8.pos100() * 1.2 + (await line8.pos100() - posLast) * 3.6
            motorL.dc(75 + PD)
            motorR.dc(75 - PD)
            posLast = await line8.pos100()
            await wait(10)

async def main():
    global PD, posLast
    # Connect Line8 sensor to port C (3) and enable multitasking (true).
    await multitask(
        subtask(),
        subtask2(),
    )


run_task(main())