### Wait 2~4 seconds after power-on

The Ranger 8-Way needs no calibration; plug it in and go. **After power-on, wait 2~4 seconds** for the 8 lasers to get ready; meanwhile everything reads 120 (cm). Start your program with a short wait before reading distances.

| Channel | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |
| :--- | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| Facing | left | left-front 60° | left-front 30° | front | front | right-front 30° | right-front 60° | right |

The table assumes the sensor faces forward with the components up. Put your hand on the robot's left: example 1 should show a **negative** direction. If it shows a positive one, the sensor is mounted the other way, and "turn left" / "turn right" in the examples must be swapped too.

### Test results

The MATRIX edition **passed hardware testing on a MATRIX Mini R4 with the stock library (2026-11-02)**: `getColor(R)` to `getColor(K)` give the 7 distances (cm) from left to right, with C being the nearer of the two front lasers 4 and 5, exactly as in the readings below; 500 reads in a row all came back.
