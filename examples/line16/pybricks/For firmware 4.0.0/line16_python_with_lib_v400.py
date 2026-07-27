from pybricks.hubs import PrimeHub
from pybricks.parameters import Direction, Port
from pybricks.pupdevices import Motor
from pybricks.tools import multitask, run_task, wait

from MBC_line16_obj_Lib import MBC_LINE16

# Set up.
line16 = MBC_LINE16(3, True)
prime_hub = PrimeHub()
motorL = Motor(Port.E, Direction.COUNTERCLOCKWISE)
motorR = Motor(Port.F, Direction.CLOCKWISE)
PD = 0
lastError = 0

async def subtask():
    while True:
        await wait(0)
        print(await line16.pos16(), await line16.pos100(), await line16.ir_ch(1))
        await wait(10)

async def subtask2():
    global PD, lastError
    while True:
        await wait(0)
        if await line16.width() > 6:
            motorL.dc(0)
            motorR.dc(0)
        else:
            PD = await line16.pos100() * 1.2 + (await line16.pos100() - lastError) * 3.6
            motorL.dc(75 + PD)
            motorR.dc(75 - PD)
            lastError = await line16.pos100()
            await wait(10)

async def main():
    global PD, lastError
    # Connect Line16 sensor to port C (3) and enable multitasking (true).
    await multitask(
        subtask(),
        subtask2(),
    )


run_task(main())