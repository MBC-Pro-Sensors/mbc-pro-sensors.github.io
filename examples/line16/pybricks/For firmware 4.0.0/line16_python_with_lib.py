from pybricks.hubs import PrimeHub
from pybricks.parameters import Direction, Port
from pybricks.pupdevices import Motor
from pybricks.tools import multitask, run_task, wait

from MBC_line16_Lib import (
    line_bin_raw,
    line_init_port_multitask,
    line_ir_calibration,
    line_ir_ch,
    line_junction,
    line_junction_name,
    line_on_sensors,
    line_pos100,
    line_pos16,
    line_read_all,
    line_width,
)

# Set up.
prime_hub = PrimeHub()
motorL = Motor(Port.E, Direction.COUNTERCLOCKWISE)
motorR = Motor(Port.F, Direction.CLOCKWISE)
PD = 0
lastError = 0

async def subtask():
    while True:
        await wait(0)
        print(await line_pos16(), await line_pos100(), await line_ir_ch(1))
        await wait(10)

async def subtask2():
    global PD, lastError
    while True:
        await wait(0)
        if await line_width() > 10:
            motorL.dc(0)
            motorR.dc(0)
        else:
            PD = await line_pos16() * 7 + (await line_pos16() - lastError) * 50
            motorL.dc(75 + PD)
            motorR.dc(75 - PD)
            lastError = await line_pos16()
            await wait(10)

async def main():
    global PD, lastError
    # Connect Line16 sensor to port C (3) and enable multitasking (true).
    await line_init_port_multitask(3, True)
    await multitask(
        subtask(),
        subtask2(),
    )


run_task(main())