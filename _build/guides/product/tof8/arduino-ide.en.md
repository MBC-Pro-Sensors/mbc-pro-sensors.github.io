### Wait 2~4 seconds after power-on

The Ranger 8-Way needs no calibration; plug it in and go. **After power-on, wait 2~4 seconds** for the 8 lasers to get ready; meanwhile everything reads 120 (cm). Start your program with a short wait before reading distances.

| Channel | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |
| :--- | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| Facing | left | left-front 60° | left-front 30° | front | front | right-front 30° | right-front 60° | right |

The table assumes the sensor faces forward with the components up. Put your hand on the robot's left: example 1 should show a **negative** direction. If it shows a positive one, the sensor is mounted the other way, and "turn left" / "turn right" in the examples must be swapped too.

### Test results

The universal I2C edition **passed hardware testing on an Arduino Nano (2026-11-02)**: the value table, all 8 distances, 1000 reads in a row and mixed reads and writes all worked with no failed reads.

### Change the settings with commands (E0 ~ E4)

On the universal I2C edition, commands change the ranging mode (normal / long range 200 cm / accurate / fast), filter, timing budget and max distance, or do a factory reset; **tested on an Arduino Nano (2026-11-02)**. Settings stay in the sensor memory and go back to factory after a power cut; "nothing" reads as the limit (normal 120, long range 200). For each command's parameters, when to use it and Examples 7 ~ 11, see [Arduino examples: change the settings with commands](/en/sensors/tof8/arduino-examples.md#cmd).
