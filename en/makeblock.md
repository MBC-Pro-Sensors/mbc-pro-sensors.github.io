<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# 🤖 MakeBlock (mBot) Block Extension

Read MBC Pathfinder, Ranger, Controller, IMU gyro and Vision sensors with blocks in mBlock 5 on mBot, mBot Ranger and other Makeblock boards: install one extension and six new categories appear, one block per value.

Software: **mBlock 5 for PC** (Upload mode — the program is flashed to the board).

---

## 📥 Install the extension

### v2.0.0 (new, for protocol v2 firmware shipped from 2026-10)

<div style="background: rgba(0,210,255,0.05); border: 1px solid rgba(0,210,255,0.3); border-radius: 12px; padding: 22px; margin: 20px 0;">
<p style="margin-top: 0;"><b>MBC Pro Sensors extension v2.0.0</b>: 95 blocks for Pathfinder, Ranger, Controller, IMU gyro and Vision sensor, for MakeBlock-edition sensors shipped from October 2026 (protocol v2).</p>
<p><b>Publishing in progress</b>: the extension is under review on the Makeblock extension platform. Once listed, search "MBC" in the mBlock 5 Extension Center.</p>
<p style="margin-bottom: 0;">Advanced users who cannot wait: sign in to <a href="https://ext.mblock.cc">ext.mblock.cc</a> with your own Makeblock account → My extensions → Import data, upload <a href="/downloads/MBC_Pro_Sensors_mBlock5_v2.0.0_import.mext"><code>mbc_pro_sensors_2.0.0_import.mext</code></a> (about 1 minute), then click "Download" to get a <code>.mext</code> you can drop into mBlock.</p>
</div>

### v1.0.0 (legacy)

<div style="background: rgba(128,128,128,0.06); border: 1px solid rgba(128,128,128,0.3); border-radius: 12px; padding: 18px; margin: 20px 0;">
<p style="margin: 0 0 10px 0;"><b>Legacy v1.0.0</b> (15 blocks: Pathfinder, Ranger, Controller): <b>for firmware shipped before 2026-10</b>. To use the old blocks with new firmware, flash the Pathfinder 8/16 with the <code>MBC_MBLOCK_LEGACY=1</code> build (no legacy build exists for the other products).</p>
<a href="/downloads/MBC_Pro_Sensors_mBlock5_v1.0.0.mext" style="display: inline-block; background: #666; color: white; text-decoration: none; padding: 10px 22px; border-radius: 30px; font-weight: bold;">Download legacy .mext</a>
</div>

### Install steps (once you have the .mext)

1. Open **mBlock 5 for PC** and add your board under "Devices" (for example mBot).
2. **Drag the `.mext` file into the mBlock window.** Six categories appear: Pathfinder, Ranger, Controller, IMU Gyro, Vision and MBC common.
3. Switch the mode to **Upload**, start from the board's start block (for example "when mBot starts up") and click "Upload".

> [!IMPORTANT]
> Use the **MakeBlock edition** of the sensor (different factory firmware) connected through the **mBot socket board** to an RJ25 port. SPIKE, EV3 and I2C editions cannot be plugged into Makeblock boards directly.
> The v2 extension only works with v2 firmware and the legacy extension only with legacy firmware; the blocks are not interchangeable. Not sure which firmware you have? Drop in the v2 extension and read "MBC common → protocol version": **2** means new firmware.

> [!NOTE]
> Blocks follow the mBlock language setting (English, Traditional Chinese or Simplified Chinese).

---

## 🧩 Boards and ports

Pick the **port number** on the block; the extension maps it to the right pins for your board. Use these ports:

| Board | Usable ports | Notes |
| :--- | :---: | :--- |
| mBot (mCore) | 1, 2, 3, 4 | All four ports |
| mBot Ranger (Auriga) | 6 – 10 | 1–4 are red motor ports, 5 is serial |
| Orion | 3, 4, 6 | 1–2 are red motor ports; 7–8 are analog-only |
| Ultimate 2.0 (MegaPi) | 6, 7, 8 | Needs the RJ25 shield; 5 is serial |
| MegaPi Pro | 7, 8, 9, 10 | Needs the RJ25 shield |

> [!TIP]
> On a port that cannot be used, or when the sensor is not connected, reporter blocks return **-32768** (never read) or the **last value read**, and "MBC common → connected?" is false. Check that boolean block first, then the numbers.

mBot2 (CyberPi) has no RJ25 ports and is not supported.

---

## 📦 Supported products

<!-- gen:mblock-products -->
| Product | Category | Blocks | Legacy ext. |
| :--- | :--- | :--- | :--- |
| [Vision Sensor (School Edition)](/en/sensors/esp32cam/mblock.md) | Vision | hue, saturation, brightness, motion, blob X, blob Y, blob width, blob height, blob area, blob found? | — |
| [Vision Sensor (WRO Edition, HUE12)](/en/sensors/esp32cam/mblock-wro.md) | Vision WRO | spot N colour code, spot N vividness | — |
| [IMU Gyro Sensor](/en/sensors/imu/mblock.md) | IMU Gyro | gyro 3D heading, fused heading, gyro 1D heading, magnetic heading, yaw rate, pitch angle, pitch rate, magnetometer trust, magnetic disturbance, face up, gyro [axis], accel [axis], mag [axis] | — |
| [Line Follower 16-ch](/en/sensors/line16/mblock.md) | Pathfinder | line position, fine line position, line width, line segments, line bitmap, last exit direction, threshold, calibration count, channel N reflect, channel N on line? | v1.0.0 works (pre-2026-10 firmware) |
| [Line Follower 8-ch](/en/sensors/line8/mblock.md) | Pathfinder | line position, fine line position, line width, line segments, line bitmap, last exit direction, threshold, calibration count, channel N reflect, channel N on line? | v1.0.0 works (pre-2026-10 firmware) |
| [PS2 Gamepad Receiver](/en/sensors/ps2/mblock.md) | Controller | 16 buttons, d-pad, shape buttons, shoulder buttons, function buttons, gamepad connected?, battery, L2 analog, R2 analog, system keys, gamepad type, stick [LX/LY/RX/RY], [button] pressed? | — |
| [Gamepad Receiver PS4/PS5](/en/sensors/ps4/mblock.md) | Controller | 16 buttons, D-pad, shape buttons, shoulder buttons, function buttons, gamepad connected?, battery, L2 analog, R2 analog, system buttons, gamepad type, stick [axis], [button] pressed? | — |
| [Laser Ranger 2-ch](/en/sensors/tof2/mblock.md) | Ranger 2 | left distance cm, right distance cm, nearer distance cm, left distance mm, right distance mm, init bitmap, fault bitmap, average distance mm | — |
| [Laser Ranger 8-ch](/en/sensors/tof8/mblock.md) | Ranger 8 | nearest distance cm, nearest direction, farthest distance cm, farthest direction, front distance cm, nearest distance mm, init bitmap, fault bitmap, front distance mm, sensor N distance cm, sensor N distance mm | — |
<!-- /gen -->

> [!NOTE]
> The MakeBlock editions of the IMU gyro and the Vision sensor are new (October 2026). Please [contact us](/en/contact.md) before ordering to confirm the edition. The Vision sensor talks to mBot over its IO1/IO3 serial port; bench testing is in progress. The PS2 controller is sold in Taiwan only.

---

## 🧭 Sign conventions

- **Negative = line on the left**: line position, fine position and last exit direction are negative when the line is left of centre (seen from behind the robot), positive on the right, 0 in the middle.
- **Sticks: right / up positive**: LX, LY, RX, RY are −100…+100, 0 = centre (released).
- **Distance**: cm blocks 0–120 (120 = nothing ahead), mm blocks 0–1200; Ranger 8 direction −8…+8 (0 = straight ahead, negative = left, ±8 = no target).
- **Hue −1 = grey/black/white**: the Vision hue block returns −1 when there is no colour; WRO spot colour code 255 = spot off.
- **−32768 = no reading**: sensor missing, dropped bytes or checksum error; use the "connected?" block to check.

---

## 🧱 Blocks

One block per value, returning the number you want (int16); command blocks get an ACK before the module executes them. The table is generated from the value tables:

<!-- gen:mblock-blocks -->
| Block | Returns | Product | Meaning |
| :--- | :--- | :--- | :--- |
| Vision port [port] hue | -1~100 | [Vision Sensor (School Edition)](/en/sensors/esp32cam/mblock.md) | hue of sample point S1, 0~100 (hue ring 0~240 scaled); −1 = black/white/grey (saturation < 10); 0 when S1 is not set |
| Vision port [port] saturation | 0~100 | [Vision Sensor (School Edition)](/en/sensors/esp32cam/mblock.md) | saturation of sample point S1, 0~100 |
| Vision port [port] brightness | 0~100 | [Vision Sensor (School Edition)](/en/sensors/esp32cam/mblock.md) | brightness (HSV value) of sample point S1, 0~100 |
| Vision port [port] motion | 0~100 | [Vision Sensor (School Edition)](/en/sensors/esp32cam/mblock.md) | whole-frame motion 0~100 (more change = larger) |
| Vision port [port] blob X | 0~100 | [Vision Sensor (School Edition)](/en/sensors/esp32cam/mblock.md) | blob centre X 0~100, left = 0; 0 when not found |
| Vision port [port] blob Y | 0~75 | [Vision Sensor (School Edition)](/en/sensors/esp32cam/mblock.md) | blob centre Y 0~75, bottom = 0, top = 75; 0 when not found |
| Vision port [port] blob width | 0~100 | [Vision Sensor (School Edition)](/en/sensors/esp32cam/mblock.md) | blob width W 0~100; 0 = not found |
| Vision port [port] blob height | 0~75 | [Vision Sensor (School Edition)](/en/sensors/esp32cam/mblock.md) | blob height H 0~75; 0 when not found |
| Vision port [port] blob area | 0~7500 | [Vision Sensor (School Edition)](/en/sensors/esp32cam/mblock.md) | W×H (0~7500); 0 when not found |
| Vision port [port] blob found? | 0~1 | [Vision Sensor (School Edition)](/en/sensors/esp32cam/mblock.md) | 1 = a blob of the target colour is in view (W > 0), 0 = none |
| Vision port [port] update all values | — | [Vision Sensor (School Edition)](/en/sensors/esp32cam/mblock.md) | reads the whole value table into a cache (FD pack-all); reporter blocks within the next 10 ms use the cache instead of asking one by one |
| Vision WRO port [port] spot [1~12] colour code | 0~255 | [Vision Sensor (WRO Edition, HUE12)](/en/sensors/esp32cam/mblock-wro.md) | 10~1B = colour code of spots S1~S12: 0~239 colour (hue 0~255 scaled to 0~239), 240~245 white (saturation < 30, lower = larger), 246~250 black (value < 30, darker = larger), 255 = spot not enabled |
| Vision WRO port [port] spot [1~12] vividness | 0~255 | [Vision Sensor (WRO Edition, HUE12)](/en/sensors/esp32cam/mblock-wro.md) | 20~2B = vividness (saturation) of spots S1~S12, 0~255: black = 1, white/colour = actual saturation; 0 when the spot is not enabled |
| Vision WRO port [port] update all values | — | [Vision Sensor (WRO Edition, HUE12)](/en/sensors/esp32cam/mblock-wro.md) | reads the whole value table into a cache (FD pack-all); reporter blocks within the next 10 ms use the cache instead of asking one by one |
| IMU Gyro port [port] gyro 3D heading | -32768~32767 ° | [IMU Gyro Sensor](/en/sensors/imu/mblock.md) | heading from the 3D quaternion, degrees, 0 at boot/zero, accumulates past 360 (wraps at ±32767, ~91 turns) |
| IMU Gyro port [port] fused heading | -32768~32767 ° | [IMU Gyro Sensor](/en/sensors/imu/mblock.md) | 3D gyro heading slowly corrected by the magnetometer, degrees, relative to the zero point; may exceed 360 |
| IMU Gyro port [port] gyro 1D heading | -32768~32767 ° | [IMU Gyro Sensor](/en/sensors/imu/mblock.md) | heading integrated about the yaw axis only, degrees, 0 at boot/zero; may exceed 360 |
| IMU Gyro port [port] magnetic heading | -32768~32767 ° | [IMU Gyro Sensor](/en/sensors/imu/mblock.md) | tilt-compensated magnetometer heading, degrees, relative to the zero point |
| IMU Gyro port [port] yaw rate | -32768~32767 0.1 dps | [IMU Gyro Sensor](/en/sensors/imu/mblock.md) | angular rate about the yaw axis (getYawAxis()), 0.1 deg/s, same sign as the heading |
| IMU Gyro port [port] pitch angle | -90~+90 ° | [IMU Gyro Sensor](/en/sensors/imu/mblock.md) | elevation of the board's printed X axis above horizontal: +90 pointing up, −90 pointing down, 0 level (from the gravity vector) |
| IMU Gyro port [port] pitch rate | -32768~32767 0.1 dps | [IMU Gyro Sensor](/en/sensors/imu/mblock.md) | angular rate about the board's printed X axis, 0.1 °/s (right-hand rule, CCW seen from +X is positive); this is gyro X, not the derivative of 06 |
| IMU Gyro port [port] magnetometer trust | 0~255 | [IMU Gyro Sensor](/en/sensors/imu/mblock.md) | how much the fusion trusts the magnetometer, 0~255 (= 0~1 × 255) |
| IMU Gyro port [port] magnetic disturbance | 0~255 0.01° | [IMU Gyro Sensor](/en/sensors/imu/mblock.md) | magnetic disturbance level 0~255 (degrees × 100, capped at 255) |
| IMU Gyro port [port] face up | 0~5 | [IMU Gyro Sensor](/en/sensors/imu/mblock.md) | which axis points up: 0 +X, 1 +Y, 2 +Z, 3 −X, 4 −Y, 5 −Z (physical axes) |
| IMU Gyro port [port] gyro [axis] | -32768~32767 0.1 dps | [IMU Gyro Sensor](/en/sensors/imu/mblock.md) | 10 X, 11 Y, 12 Z angular rate, 0.1 deg/s (physical axes, sign-corrected) |
| IMU Gyro port [port] accel [axis] | -32768~32767 mg | [IMU Gyro Sensor](/en/sensors/imu/mblock.md) | 13 X, 14 Y, 15 Z acceleration, mg (1000 = 1 g) |
| IMU Gyro port [port] mag [axis] | -32768~32767 mG | [IMU Gyro Sensor](/en/sensors/imu/mblock.md) | 16 X, 17 Y, 18 Z magnetic field, mG (milligauss) |
| IMU Gyro port [port] zero heading | — | [IMU Gyro Sensor](/en/sensors/imu/mblock.md) | zero all four headings (zeroHeadings()); EV3 also accepts the stock gyro-reset WRITE 0x11 |
| IMU Gyro port [port] start accelerometer calibration | — | [IMU Gyro Sensor](/en/sensors/imu/mblock.md) | start accelerometer calibration (= v1 0x20=0x01); F3 b4 = 1 meanwhile |
| IMU Gyro port [port] end accelerometer calibration | — | [IMU Gyro Sensor](/en/sensors/imu/mblock.md) | finish accelerometer calibration and save to EEPROM (= v1 0x20=0x02); headings are zeroed once on success |
| IMU Gyro port [port] start magnetometer calibration | — | [IMU Gyro Sensor](/en/sensors/imu/mblock.md) | start magnetometer calibration (= v1 0x20=0x03); F3 b5 = 1 meanwhile |
| IMU Gyro port [port] end magnetometer calibration | — | [IMU Gyro Sensor](/en/sensors/imu/mblock.md) | finish magnetometer calibration and save to EEPROM (= v1 0x20=0x04) |
| IMU Gyro port [port] update all values | — | [IMU Gyro Sensor](/en/sensors/imu/mblock.md) | reads the whole value table into a cache (FD pack-all); reporter blocks within the next 10 ms use the cache instead of asking one by one |
| Pathfinder port [port] line position | -16~+16 | [Line Follower 16-ch](/en/sensors/line16/mblock.md) | which channel the line is under: −16 (far left) to +16 (far right), 0 = centre; seen from behind the robot, negative = left (normally ±15; ±16 only for 200 ms after the line leaves the outermost channel) |
| Pathfinder port [port] fine line position | -100~+100 | [Line Follower 16-ch](/en/sensors/line16/mblock.md) | sub-channel position: −100 to +100, 0 = centre, negative = left (same sign as line position) |
| Pathfinder port [port] line width | 0~16 路 | [Line Follower 16-ch](/en/sensors/line16/mblock.md) | number of channels on the line, 0~16; 0 = no line |
| Pathfinder port [port] line segments | 0~8 | [Line Follower 16-ch](/en/sensors/line16/mblock.md) | number of separate line segments: 0 = lost, 1 = one line, ≥2 = fork / crossing |
| Pathfinder port [port] line bitmap | 0~65535 bit | [Line Follower 16-ch](/en/sensors/line16/mblock.md) | bit0 = channel 1 (left) … bit15 = channel 16; 1 = that channel sees the line |
| Pathfinder port [port] last exit direction | -16~+16 | [Line Follower 16-ch](/en/sensors/line16/mblock.md) | where the line was before it was lost: −16 to +16 (negative = left); keeps the last value |
| Pathfinder port [port] threshold | 1~99 | [Line Follower 16-ch](/en/sensors/line16/mblock.md) | reflect threshold for 'on line', 1~99, default 50; set with E4 |
| Pathfinder port [port] calibration count | 0~255 次 | [Line Follower 16-ch](/en/sensors/line16/mblock.md) | number of calibrations completed since power-on (button calibrations included) |
| Pathfinder port [port] channel [1~16] reflect | 0~100 | [Line Follower 16-ch](/en/sensors/line16/mblock.md) | calibrated reflect of channel 1~16, 0~100 (channel 1 = left); ids 10~1F |
| Pathfinder port [port] channel [1~16] on line? | true/false | [Line Follower 16-ch](/en/sensors/line16/mblock.md) | bit N−1 of line bitmap 05 (channel 1 = bit0, leftmost): true when that channel sees the line |
| Pathfinder port [port] black line mode | — | [Line Follower 16-ch](/en/sensors/line16/mblock.md) | follow a black line (default) |
| Pathfinder port [port] white line mode | — | [Line Follower 16-ch](/en/sensors/line16/mblock.md) | follow a white line (F3 b1 = 1) |
| Pathfinder port [port] calibrate black | — | [Line Follower 16-ch](/en/sensors/line16/mblock.md) | store the current reading as black (sensor over the black line), ~50 ms; saved to EEPROM |
| Pathfinder port [port] calibrate white | — | [Line Follower 16-ch](/en/sensors/line16/mblock.md) | store the current reading as white (sensor over the white surface), ~50 ms; saved to EEPROM |
| Pathfinder port [port] set threshold [threshold] | — | [Line Follower 16-ch](/en/sensors/line16/mblock.md) | threshold 1~99 (RAM only, back to 50 after reboot) |
| Pathfinder port [port] 5-second auto calibration | — | [Line Follower 16-ch](/en/sensors/line16/mblock.md) | sweep the sensor across black line and white surface for 5 s; min/max become the references; F3 b2 = 1 meanwhile |
| Pathfinder port [port] update all values | — | [Line Follower 16-ch](/en/sensors/line16/mblock.md) | reads the whole value table into a cache (FD pack-all); reporter blocks within the next 10 ms use the cache instead of asking one by one |
| Pathfinder port [port] line position | -8~+8 | [Line Follower 8-ch](/en/sensors/line8/mblock.md) | which channel the line is under: −8 (far left) to +8 (far right), 0 = centre; seen from behind the robot, negative = left |
| Pathfinder port [port] fine line position | -100~+100 | [Line Follower 8-ch](/en/sensors/line8/mblock.md) | sub-channel position: −100 to +100, 0 = centre, negative = left (same sign as line position) |
| Pathfinder port [port] line width | 0~8 路 | [Line Follower 8-ch](/en/sensors/line8/mblock.md) | number of channels on the line, 0~8; 0 = no line |
| Pathfinder port [port] line segments | 0~4 | [Line Follower 8-ch](/en/sensors/line8/mblock.md) | number of separate line segments: 0 = lost, 1 = one line, ≥2 = fork / crossing |
| Pathfinder port [port] line bitmap | 0~255 bit | [Line Follower 8-ch](/en/sensors/line8/mblock.md) | bit0 = channel 1 (left) … bit7 = channel 8; 1 = that channel sees the line |
| Pathfinder port [port] last exit direction | -8~+8 | [Line Follower 8-ch](/en/sensors/line8/mblock.md) | where the line was before it was lost: −8 to +8 (negative = left); keeps the last value |
| Pathfinder port [port] threshold | 1~99 | [Line Follower 8-ch](/en/sensors/line8/mblock.md) | reflect threshold for 'on line', 1~99, default 50; set with E4 |
| Pathfinder port [port] calibration count | 0~255 次 | [Line Follower 8-ch](/en/sensors/line8/mblock.md) | number of calibrations completed since power-on (button calibrations included) |
| Pathfinder port [port] channel [1~8] reflect | 0~100 | [Line Follower 8-ch](/en/sensors/line8/mblock.md) | calibrated reflect of channel 1~8, 0~100 (channel 1 = left); ids 10~17 |
| Pathfinder port [port] channel [1~8] on line? | true/false | [Line Follower 8-ch](/en/sensors/line8/mblock.md) | bit N−1 of line bitmap 05 (channel 1 = bit0, leftmost): true when that channel sees the line |
| Pathfinder port [port] black line mode | — | [Line Follower 8-ch](/en/sensors/line8/mblock.md) | follow a black line (default) |
| Pathfinder port [port] white line mode | — | [Line Follower 8-ch](/en/sensors/line8/mblock.md) | follow a white line (F3 b1 = 1) |
| Pathfinder port [port] calibrate black | — | [Line Follower 8-ch](/en/sensors/line8/mblock.md) | store the current reading as black (sensor over the black line), ~50 ms; saved to EEPROM |
| Pathfinder port [port] calibrate white | — | [Line Follower 8-ch](/en/sensors/line8/mblock.md) | store the current reading as white (sensor over the white surface), ~50 ms; saved to EEPROM |
| Pathfinder port [port] set threshold [threshold] | — | [Line Follower 8-ch](/en/sensors/line8/mblock.md) | threshold 1~99 (RAM only, back to 50 after reboot) |
| Pathfinder port [port] 5-second auto calibration | — | [Line Follower 8-ch](/en/sensors/line8/mblock.md) | sweep the sensor across black line and white surface for 5 s; min/max become the references; F3 b2 = 1 meanwhile |
| Pathfinder port [port] update all values | — | [Line Follower 8-ch](/en/sensors/line8/mblock.md) | reads the whole value table into a cache (FD pack-all); reporter blocks within the next 10 ms use the cache instead of asking one by one |
| Controller port [port] 16 buttons | 0~65535 bit | [PS2 Gamepad Receiver](/en/sensors/ps2/mblock.md) | all 16 buttons in one word (PSX order, 1 = pressed): bit0 SELECT, 1 L3, 2 R3, 3 START, 4 Up, 5 Right, 6 Down, 7 Left, 8 L2, 9 R2, 10 L1, 11 R1, 12 Triangle, 13 Circle, 14 Cross, 15 Square; 0 when not connected |
| Controller port [port] d-pad | 0~15 bit | [PS2 Gamepad Receiver](/en/sensors/ps2/mblock.md) | Up 1, Right 2, Down 4, Left 8 (added when pressed together), 0~15; 0 when not connected |
| Controller port [port] shape buttons | 0~15 bit | [PS2 Gamepad Receiver](/en/sensors/ps2/mblock.md) | Triangle 1, Circle 2, Cross 4, Square 8 (added when pressed together), 0~15; 0 when not connected; high nibble of 0x22 'R' |
| Controller port [port] shoulder buttons | 0~15 bit | [PS2 Gamepad Receiver](/en/sensors/ps2/mblock.md) | L2 1, R2 2, L1 4, R1 8 (added when pressed together), 0~15; 0 when not connected |
| Controller port [port] function buttons | 0~15 bit | [PS2 Gamepad Receiver](/en/sensors/ps2/mblock.md) | SELECT 1, L3 2, R3 4, START 8 (added when pressed together), 0~15; 0 when not connected; high nibble of 0x22 'G' |
| Controller port [port] gamepad connected? | 0~1 | [PS2 Gamepad Receiver](/en/sensors/ps2/mblock.md) | 1 = gamepad connected (psx.read() succeeds), 0 = not; while disconnected all buttons read 0 and sticks are centred |
| Controller port [port] battery | 0 % | [PS2 Gamepad Receiver](/en/sensors/ps2/mblock.md) | 0~100; PS2 pads report no battery → always 0 (PS4 only) |
| Controller port [port] L2 analog | 0~255 | [PS2 Gamepad Receiver](/en/sensors/ps2/mblock.md) | L2 pressure 0~255 (only DualShock 2 in analog mode reports pressure; otherwise pressed = 255, released = 0); 0 when not connected |
| Controller port [port] R2 analog | 0~255 | [PS2 Gamepad Receiver](/en/sensors/ps2/mblock.md) | R2 pressure 0~255 (same rule as 08); 0 when not connected |
| Controller port [port] system keys | 0 bit | [PS2 Gamepad Receiver](/en/sensors/ps2/mblock.md) | b0 PS button, b1 capture button; PS2 pads have none → always 0 (PS4 only) |
| Controller port [port] gamepad type | 0~2 | [PS2 Gamepad Receiver](/en/sensors/ps2/mblock.md) | 0 none, 1 PS2 digital mode, 2 PS2 analog mode (3 PS4 / 4 PS5 only appear on the PS4 receiver) |
| Controller port [port] stick [LX/LY/RX/RY] | -100~+100 | [PS2 Gamepad Receiver](/en/sensors/ps2/mblock.md) | 10 LX, 11 LY, 12 RX, 13 RY: −100 to +100, 0 = centre, right / up positive (raw 0~255 with ±7 dead zone); 0 when not connected. EV3 / SPIKE / MATRIX carry 0~99 (50 = centre) or 1~9 (5 = centre), see each platform |
| Controller port [port] [button] pressed? | true/false | [PS2 Gamepad Receiver](/en/sensors/ps2/mblock.md) | one button split out of 16 buttons 01 (PSX order); menu: ✕ ● ▲ ■ up down left right L1 R1 L2 R2 L3 R3 SELECT START |
| Controller port [port] rumble [strength] | — | [PS2 Gamepad Receiver](/en/sensors/ps2/mblock.md) | [strength 0~255], 0 = stop; the PS2 receiver ACKs and ignores it (pad rumble disabled since 250614), effective on PS4 only |
| Controller port [port] light bar colour [red] [green] [blue] | — | [PS2 Gamepad Receiver](/en/sensors/ps2/mblock.md) | [R][G][B] 0~255 each; PS2 pads have no light bar, the receiver ACKs and ignores it, effective on PS4 only |
| Controller port [port] update all values | — | [PS2 Gamepad Receiver](/en/sensors/ps2/mblock.md) | reads the whole value table into a cache (FD pack-all); reporter blocks within the next 10 ms use the cache instead of asking one by one |
| Controller port [port] 16 buttons | 0~65535 bit | [Gamepad Receiver PS4/PS5](/en/sensors/ps4/mblock.md) | all buttons in one 16-bit word (PSX layout): bit0 SELECT(SHARE), 1 L3, 2 R3, 3 START(OPTIONS), 4 up, 5 right, 6 down, 7 left, 8 L2, 9 R2, 10 L1, 11 R1, 12 triangle, 13 circle, 14 cross, 15 square; 1 = pressed; 0 when not connected |
| Controller port [port] D-pad | 0~15 bit | [Gamepad Receiver PS4/PS5](/en/sensors/ps4/mblock.md) | up 1, right 2, down 4, left 8, added when pressed together (0~15) |
| Controller port [port] shape buttons | 0~15 bit | [Gamepad Receiver PS4/PS5](/en/sensors/ps4/mblock.md) | triangle 1, circle 2, cross 4, square 8, added when pressed together (0~15) |
| Controller port [port] shoulder buttons | 0~15 bit | [Gamepad Receiver PS4/PS5](/en/sensors/ps4/mblock.md) | L2 1, R2 2, L1 4, R1 8, added when pressed together (0~15) |
| Controller port [port] function buttons | 0~15 bit | [Gamepad Receiver PS4/PS5](/en/sensors/ps4/mblock.md) | SELECT(SHARE) 1, L3 2, R3 4, START(OPTIONS) 8, added when pressed together (0~15) |
| Controller port [port] gamepad connected? | 0~1 | [Gamepad Receiver PS4/PS5](/en/sensors/ps4/mblock.md) | 1 = gamepad connected and data seen within 300 ms, 0 = not connected (buttons 0, sticks centred while disconnected) |
| Controller port [port] battery | 0~100 % | [Gamepad Receiver PS4/PS5](/en/sensors/ps4/mblock.md) | gamepad battery 0~100 (always 0 on the PS2 receiver) |
| Controller port [port] L2 analog | 0~255 | [Gamepad Receiver PS4/PS5](/en/sensors/ps4/mblock.md) | L2 trigger depth 0~255 (0 when not connected) |
| Controller port [port] R2 analog | 0~255 | [Gamepad Receiver PS4/PS5](/en/sensors/ps4/mblock.md) | R2 trigger depth 0~255 (0 when not connected) |
| Controller port [port] system buttons | 0~3 bit | [Gamepad Receiver PS4/PS5](/en/sensors/ps4/mblock.md) | b0 PS button, b1 capture (PS5 mute) button; 0 when not connected (always 0 on PS2) |
| Controller port [port] gamepad type | 0~4 | [Gamepad Receiver PS4/PS5](/en/sensors/ps4/mblock.md) | 0 none, 1 PS2 digital, 2 PS2 analog, 3 PS4, 4 PS5 (or other) |
| Controller port [port] stick [axis] | -100~+100 | [Gamepad Receiver PS4/PS5](/en/sensors/ps4/mblock.md) | 10 LX, 11 LY, 12 RX, 13 RY: −100 to +100, 0 = centre, right/up positive; 0 when not connected |
| Controller port [port] [button] pressed? | true/false | [Gamepad Receiver PS4/PS5](/en/sensors/ps4/mblock.md) | one button split out of 16 buttons 01 (PSX order); menu: ✕ ● ▲ ■ up down left right L1 R1 L2 R2 L3 R3 SELECT START |
| Controller port [port] rumble [strength] | — | [Gamepad Receiver PS4/PS5](/en/sensors/ps4/mblock.md) | [strength 0~255], 0 = stop; 200 ms per call (ignored by PS2) |
| Controller port [port] light bar colour [R] [G] [B] | — | [Gamepad Receiver PS4/PS5](/en/sensors/ps4/mblock.md) | [R][G][B] each 0~255, exactly 3 bytes; remembered until connected; default blue (0,0,255) (ignored by PS2) |
| Controller port [port] update all values | — | [Gamepad Receiver PS4/PS5](/en/sensors/ps4/mblock.md) | reads the whole value table into a cache (FD pack-all); reporter blocks within the next 10 ms use the cache instead of asking one by one |
| Ranger 2 port [port] left distance cm | 0~120 cm | [Laser Ranger 2-ch](/en/sensors/tof2/mblock.md) | distance from the left sensor, 0~120 cm (left = sensor 0 seen from behind the robot); 120 = nothing in range / sensor failed |
| Ranger 2 port [port] right distance cm | 0~120 cm | [Laser Ranger 2-ch](/en/sensors/tof2/mblock.md) | distance from the right sensor, 0~120 cm (right = sensor 1); 120 = nothing in range / sensor failed |
| Ranger 2 port [port] nearer distance cm | 0~120 cm | [Laser Ranger 2-ch](/en/sensors/tof2/mblock.md) | the smaller of left and right, 0~120 cm |
| Ranger 2 port [port] left distance mm | 0~1200 mm | [Laser Ranger 2-ch](/en/sensors/tof2/mblock.md) | left sensor distance 0~1200 mm; 1200 = nothing in range / sensor failed |
| Ranger 2 port [port] right distance mm | 0~1200 mm | [Laser Ranger 2-ch](/en/sensors/tof2/mblock.md) | right sensor distance 0~1200 mm; 1200 = nothing in range / sensor failed |
| Ranger 2 port [port] init bitmap | 0~3 bit | [Laser Ranger 2-ch](/en/sensors/tof2/mblock.md) | bit0 = left, bit1 = right; 1 = that sensor initialised at power-on (no retry); 3 = both OK |
| Ranger 2 port [port] fault bitmap | 0~3 bit | [Laser Ranger 2-ch](/en/sensors/tof2/mblock.md) | bit0 = left, bit1 = right; 1 = that sensor is currently failing (I2C error or no update for 200 ms); its distance reads 120 / 1200 |
| Ranger 2 port [port] average distance mm | 0~1200 mm | [Laser Ranger 2-ch](/en/sensors/tof2/mblock.md) | (left mm + right mm) / 2, 0~1200; same number as EV3 mode 3 and the 3rd value of SPIKE mode 6 |
| Ranger 2 port [port] update all values | — | [Laser Ranger 2-ch](/en/sensors/tof2/mblock.md) | reads the whole value table into a cache (FD pack-all); reporter blocks within the next 10 ms use the cache instead of asking one by one |
| Ranger 8 port [port] nearest distance cm | 0~120 cm | [Laser Ranger 8-ch](/en/sensors/tof8/mblock.md) | smallest of the 8 distances, 0~120 cm (a failed sensor counts as 120); 120 = nothing around |
| Ranger 8 port [port] nearest direction | -8~+8 | [Laser Ranger 8-ch](/en/sensors/tof8/mblock.md) | direction of the nearest object: −7 (left) to +7 (right), 0 = straight ahead, negative = left; ±8 = nothing around (keeps the last side) |
| Ranger 8 port [port] farthest distance cm | 0~120 cm | [Laser Ranger 8-ch](/en/sensors/tof8/mblock.md) | largest of the 8 distances, 0~120 cm (a failed sensor always reads 120 and looks 'farthest') |
| Ranger 8 port [port] farthest direction | -7~+7 | [Laser Ranger 8-ch](/en/sensors/tof8/mblock.md) | direction of the most open side: −7 to +7, 0 = straight ahead, negative = left; 0 when all 8 read 120 |
| Ranger 8 port [port] front distance cm | 0~120 cm | [Laser Ranger 8-ch](/en/sensors/tof8/mblock.md) | average of sensors 4 and 5 (both facing forward), 0~120 cm; EV3 mode 3 and SPIKE mode 6 carry the same thing in mm; 0x22 'C' is min(4, 5), not the average (see 10) |
| Ranger 8 port [port] nearest distance mm | 0~1200 mm | [Laser Ranger 8-ch](/en/sensors/tof8/mblock.md) | smallest of the 8 mm readings, 0~1200 (failed sensor counts as 1200); 1200 = nothing around |
| Ranger 8 port [port] init bitmap | 0~255 bit | [Laser Ranger 8-ch](/en/sensors/tof8/mblock.md) | bit0 = sensor 1 (left) … bit7 = sensor 8; 1 = initialised at power-on (8 sensors take ~2~4 s, no retry); 255 = all OK |
| Ranger 8 port [port] fault bitmap | 0~255 bit | [Laser Ranger 8-ch](/en/sensors/tof8/mblock.md) | bit0 = sensor 1 … bit7 = sensor 8; 1 = currently failing (I2C error or no update for 200 ms); its distance reads 120 / 1200 |
| Ranger 8 port [port] front distance mm | 0~1200 mm | [Laser Ranger 8-ch](/en/sensors/tof8/mblock.md) | average of sensors 4 and 5 (the two facing forward), 0~1200 mm; 1200 = nothing ahead. cm version is 05 |
| Ranger 8 port [port] sensor [1~8] distance cm | 0~120 cm | [Laser Ranger 8-ch](/en/sensors/tof8/mblock.md) | distance of each sensor 1~8, 0~120 cm (1 = far left, 4 and 5 = front, 8 = far right, 30° apart); 120 = nothing that way / sensor failed; ids 10~17 |
| Ranger 8 port [port] sensor [1~8] distance mm | 0~1200 mm | [Laser Ranger 8-ch](/en/sensors/tof8/mblock.md) | distance of each sensor 1~8, 0~1200 mm; 1200 = nothing that way / sensor failed; ids 18~1F |
| Ranger 8 port [port] update all values | — | [Laser Ranger 8-ch](/en/sensors/tof8/mblock.md) | reads the whole value table into a cache (FD pack-all); reporter blocks within the next 10 ms use the cache instead of asking one by one |
<!-- /gen -->


> [!TIP]
> When one loop reads many blocks, put an "update all values" block first: reporter blocks within the next 10 ms are served from the cache instead of each waiting for its own serial round trip (about 1 ms each).

---

## 📂 Example downloads

Install the **v2** extension first, then open an example with mBlock 5 "File → Open from your computer" (all examples use mBot; for another board, change the start block and the port):

| Example | Product and port | What it does |
| :--- | :--- | :--- |
| [📥 PD line following](/examples/makeblock/MBC_mBot_Line8_PD.mblock) | Pathfinder 8-Way (or 16-Way) → port 1 | PD control on "fine line position", base speed 40 |
| [📥 Obstacle avoidance](/examples/makeblock/MBC_mBot_TOF8_Avoid.mblock) | Ranger 8-Way → port 3 | Turns toward the open side when something is within 20 cm |
| [📥 Controller tank drive](/examples/makeblock/MBC_mBot_Pad_TankDrive.mblock) | Controller PS2 / PS4/PS5 → port 4 | Left stick drives the left wheel, right stick the right wheel; stops when the controller drops |

> [!NOTE]
> These three files were converted from the v1 examples to v2 blocks and have **not yet been opened in mBlock 5 for verification** (we will re-check once the extension is listed). If a file will not open or blocks show up blank, make sure the v2 extension is loaded, or [tell us](/en/contact.md).

### 💡 Example 1: mBot PD line following (Pathfinder)

Pathfinder 8-Way on mBot **port 1**:

1. "when mBot starts up" → wait 1 second (sensor boot) → Pathfinder [port 1] black line mode.
2. Forever:
   - error = Pathfinder [port 1] fine line position (negative = line on the left)
   - turn = error × 0.6 + (error − last error) × 2
   - left speed = base − turn, right speed = base + turn
   - last error = error

When the line is on the left (negative error) the turn is negative; left wheel = base − turn and right wheel = base + turn, so the left wheel slows down and the right wheel speeds up, steering the robot back left onto the line. Start with a base speed of 40. If the robot wobbles, lower 0.6; if it reacts too slowly, raise it.

### 💡 Example 2: mBot obstacle avoidance (Ranger 8)

Ranger 8-Way on mBot **port 3**; wait 3 seconds after boot for the 8 lasers to initialise:

1. If Ranger 8 [port 3] nearest distance cm < 20:
   - if nearest direction < 0 (obstacle on the left) → left 40, right −40 (turn right)
   - else → left −40, right 40 (turn left)
2. Else → both wheels 40, straight ahead.

For a smarter version, use "farthest direction" and turn toward the most open side.

### 💡 Example 3: mBot tank drive (Controller)

Controller on mBot **port 4**:

1. If Controller [port 4] gamepad connected?:
   - left wheel = Controller [port 4] stick [left stick y] × 0.7
   - right wheel = Controller [port 4] stick [right stick y] × 0.7
2. Else → stop.

Sticks are positive upward (max +100); × 0.7 caps the speed at 70 — raise it for a faster robot.

---

## ❓ No readings?

1. "MBC common → connected?" is false: wrong port on the block, the sensor is not the MakeBlock edition, or it runs legacy firmware (use the v1 extension above).
2. **-32768**: nothing has been read since power-on — check the wiring as above; or this product has no such value (for example channel 9 on an 8-way Pathfinder).
3. Value never changes: check the socket board is seated and the sensor LED is on; "protocol version" reading 2 means the link works.
4. Still stuck: [contact us](/en/contact.md) with your board model, the port and the "device ID" reading.

---

## 🔧 Plain Arduino works too

The library inside the extension (`src/MBC_Sensors.h`) can be used in your own Arduino sketch: `MBCPort p(1); p.begin(); int16_t pos = p.read(0x01);`. The protocol is tiny: the host sends one byte (the value id) and the sensor replies with three bytes (value low, value high, checksum = id ^ low ^ high). See each product's command reference for the packet details.
