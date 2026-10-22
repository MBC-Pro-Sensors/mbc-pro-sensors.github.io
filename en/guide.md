<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# 🧭 Buying Guide

Not sure which product or edition you need? Two steps: **pick your controller, then choose by what you need**. Still unsure? [Ask us on LINE](https://line.me/R/ti/p/@692vcvuk) and an engineer will confirm.

> [!IMPORTANT]
> Every MBC product comes in controller-specific editions (different connectors and protocols), and **editions are not interchangeable**. Please confirm your controller model before ordering.

<h2 id="start" style="scroll-margin-top: 96px;">🚀 Already have a sensor? Find your program</h2>

Pick your product and controller to go straight to setup steps, blocks and example downloads (SPIKE includes Robot Inventor; Arduino includes ESP32 and other I2C controllers; mBot includes Ranger and other Makeblock boards):

<!-- gen:guide-start -->
| Product | SPIKE | EV3 | Arduino I2C | mBlock | MATRIX | micro:bit |
| :--- | :---: | :---: | :---: | :---: | :---: | :---: |
| [Pathfinder 8-Way (Line8)](/en/sensors/line8/index.md) | [App · Pybricks](/en/sensors/line8/spike-hub.md) | [App · Pybricks](/en/sensors/line8/ev3-hub.md) | [Examples](/en/sensors/line8/arduino-examples.md) | [Examples](/en/sensors/line8/mblock-examples.md) | [Library](/en/sensors/line8/matrix.md) | [Examples](/en/sensors/line8/microbit-examples.md) |
| [Pathfinder 16-Way (Line16)](/en/sensors/line16/index.md) | [App · Pybricks](/en/sensors/line16/spike-hub.md) | [App · Pybricks](/en/sensors/line16/ev3-hub.md) | [Examples](/en/sensors/line16/arduino-examples.md) | [Examples](/en/sensors/line16/mblock-examples.md) | [Library](/en/sensors/line16/matrix.md) | [Examples](/en/sensors/line16/microbit-examples.md) |
| [Ranger 2-Way (TOF2)](/en/sensors/tof2/index.md) | [Product page](/en/sensors/tof2/index.md) | [Product page](/en/sensors/tof2/index.md) | [Examples](/en/sensors/tof2/arduino-examples.md) | [Examples](/en/sensors/tof2/mblock-examples.md) | [Library](/en/sensors/tof2/matrix.md) | [Examples](/en/sensors/tof2/microbit-examples.md) |
| [Ranger 8-Way (TOF8)](/en/sensors/tof8/index.md) | [Product page](/en/sensors/tof8/index.md) | [Product page](/en/sensors/tof8/index.md) | [Examples](/en/sensors/tof8/arduino-examples.md) | [Examples](/en/sensors/tof8/mblock-examples.md) | [Library](/en/sensors/tof8/matrix.md) | [Examples](/en/sensors/tof8/microbit-examples.md) |
| [Controller 2.4G (PS2)](/en/sensors/ps2/index.md) | [Product page](/en/sensors/ps2/index.md) | [Product page](/en/sensors/ps2/index.md) | [Examples](/en/sensors/ps2/arduino-examples.md) | [Examples](/en/sensors/ps2/mblock-examples.md) | [Library](/en/sensors/ps2/matrix.md) | [Examples](/en/sensors/ps2/microbit-examples.md) |
| [Controller PS4/PS5](/en/sensors/ps4/index.md) | [Product page](/en/sensors/ps4/index.md) | [Product page](/en/sensors/ps4/index.md) | [Examples](/en/sensors/ps4/arduino-examples.md) | [Examples](/en/sensors/ps4/mblock-examples.md) | [Library](/en/sensors/ps4/matrix.md) | [Examples](/en/sensors/ps4/microbit-examples.md) |
| [Sharpshooter Vision (ESP32CAM)](/en/sensors/esp32cam/index.md) | [Product page](/en/sensors/esp32cam/index.md) | [Product page](/en/sensors/esp32cam/index.md) | [Examples](/en/sensors/esp32cam/arduino-examples.md) | [Examples](/en/sensors/esp32cam/mblock-examples.md) | [Library](/en/sensors/esp32cam/matrix.md) | [Examples](/en/sensors/esp32cam/microbit-examples.md) |
| [SPIKE 6-Way Expander (EXP6)](/en/sensors/exp6/index.md) | [Guide](/en/sensors/exp6/spike-official.md) | — | [Examples](/en/sensors/exp6/arduino-examples.md) | — | — | — |
| [IMU 9-Axis Gyro](/en/sensors/imu/index.md) | — | [Product page](/en/sensors/imu/index.md) | [Examples](/en/sensors/imu/arduino-examples.md) | [Examples](/en/sensors/imu/mblock-examples.md) | [Library](/en/sensors/imu/matrix.md) | [Examples](/en/sensors/imu/microbit-examples.md) |
| [SPIKE 6-Port Servo Expander (Servo6)](/en/sensors/servo6/index.md) | [Guide](/en/sensors/servo6/spike-official.md) | — | — | — | — | — |
| [Long-Range Colour Sensor OV3660](/en/sensors/ov3660/index.md) | — | — | — | — | — | — |
<!-- /gen -->

> [!TIP]
> **Not sure which software?** SPIKE hub: the official app's blocks are the easiest for class; for competitions, Pybricks is faster and more consistent. EV3 hub: the EV3 software or EV3 Classroom both work; use Pybricks for Python.

## 1️⃣ Which controller do you use?

<div class="guide-hosts">
<a class="guide-host" href="#spike">🧱 SPIKE Prime<small>incl. Robot Inventor</small></a>
<a class="guide-host" href="#ev3">🧱 EV3<small>MINDSTORMS EV3</small></a>
<a class="guide-host" href="#arduino">∿ Arduino<small>ESP32 / Raspberry Pi</small></a>
<a class="guide-host" href="#matrix">🤖 MATRIX<small>Mini R4</small></a>
<a class="guide-host" href="#makeblock">🤖 MakeBlock<small>mBot / Ranger</small></a>
</div>

<h3 id="spike">🧱 LEGO SPIKE Prime / Robot Inventor</h3>

| Product | Use | Notes |
| :--- | :--- | :--- |
| [Pathfinder 8-Way](/en/sensors/line8/index.md) | Line following | Official app blocks, Pybricks |
| [Pathfinder 16-Way](/en/sensors/line16/index.md) | Fast lines, intersections | Official app blocks, Pybricks |
| [Ranger 2-Way](/en/sensors/tof2/index.md) | Left/right ranging | |
| [Ranger 8-Way](/en/sensors/tof8/index.md) | 180° avoidance, sumo | |
| [SPIKE 6-Way Expander](/en/sensors/exp6/index.md) | More ports | **SPIKE Prime only**; official app requires Python text mode |
| [SPIKE 6-Port Servo Expander](/en/sensors/servo6/index.md) (coming soon) | Servos on your robot (gripper, turntable, arm) | **SPIKE Prime only**; servos powered by the hub, no battery |
| [Controller PS4/PS5](/en/sensors/ps4/index.md) | Bluetooth gamepad control | Best for big events |
| [Controller PS2](/en/sensors/ps2/index.md) (Taiwan only) | 2.4G gamepad control | Classes, projects |
| [Sharpshooter ESP32CAM](/en/sensors/esp32cam/index.md) | Color tracking, vision | |
| [IMU Gyro](/en/sensors/imu/index.md) | Heading, turning | Coming soon |

<h3 id="ev3">🧱 LEGO MINDSTORMS EV3</h3>

| Product | Use | Notes |
| :--- | :--- | :--- |
| [Pathfinder 8-Way](/en/sensors/line8/index.md) | Line following | EV3 software, EV3 Classroom, clev3r, Pybricks |
| [Pathfinder 16-Way](/en/sensors/line16/index.md) | Fast lines, intersections | Same as above |
| [Ranger 2-Way](/en/sensors/tof2/index.md) | Left/right ranging | |
| [Ranger 8-Way](/en/sensors/tof8/index.md) | 180° avoidance, sumo | |
| [Controller PS4/PS5](/en/sensors/ps4/index.md) | Bluetooth gamepad control | |
| [Controller PS2](/en/sensors/ps2/index.md) (Taiwan only) | 2.4G gamepad control | |
| [Sharpshooter ESP32CAM](/en/sensors/esp32cam/index.md) | Color tracking, vision | |
| [IMU Gyro](/en/sensors/imu/index.md) | Heading, turning | Works with the standard gyro blocks; coming soon |

<h3 id="arduino">∿ Arduino / ESP32 / Raspberry Pi (Universal I2C)</h3>

The whole family shares one [Universal I2C Protocol](/en/i2c-protocol.md) — learn one, use them all. Every product has beginner examples you can copy and upload right away. Never written code before? Start with [🔰 First time with Arduino](/en/sensors/arduino-start.md).

| Product | Use | I2C address |
| :--- | :--- | :---: |
| [Pathfinder 8-Way](/en/sensors/line8/arduino-i2c.md) | Line following | `0x16` |
| [Pathfinder 16-Way](/en/sensors/line16/arduino-i2c.md) | Fast lines, intersections | `0x16` |
| [Ranger 8-Way](/en/sensors/tof8/index.md) | 180° avoidance | `0x29` |
| [SPIKE 6-Way Expander](/en/sensors/exp6/arduino-examples.md) | Drive LEGO motors & sensors from Arduino | Serial (UART) |
| [Controller PS4/PS5](/en/sensors/ps4/index.md) | Bluetooth gamepad control | `0x42` |
| [Controller PS2](/en/sensors/ps2/index.md) (Taiwan only) | 2.4G gamepad control | `0x42` |
| [IMU Gyro](/en/sensors/imu/index.md) | Heading, 9-axis data | `0x6A` |

<h3 id="matrix">🤖 MATRIX Mini R4</h3>

| Product | Use |
| :--- | :--- |
| [Pathfinder 8-Way](/en/sensors/line8/index.md) | Line following |
| [Pathfinder 16-Way](/en/sensors/line16/index.md) | Fast lines, intersections |
| [Ranger 8-Way](/en/sensors/tof8/index.md) | 180° avoidance |
| [Sharpshooter ESP32CAM](/en/sensors/esp32cam/index.md) | Color tracking, vision |
| [IMU Gyro](/en/sensors/imu/index.md) | Heading, turning (coming soon) |

<h3 id="makeblock">🤖 MakeBlock</h3>

Works with mBot, mBot Ranger, Orion and Ultimate 2.0 (MegaPi). Choose the MakeBlock edition with the mBot socket board; block download and usable ports per board are on the [MakeBlock block extension](/en/makeblock.md) page.

| Product | Use |
| :--- | :--- |
| [Pathfinder 8-Way](/en/sensors/line8/index.md) | Line following |
| [Pathfinder 16-Way](/en/sensors/line16/index.md) | Fast lines, intersections |
| [Ranger 2-Way](/en/sensors/tof2/index.md) | Left/right ranging |
| [Ranger 8-Way](/en/sensors/tof8/index.md) | 180° avoidance |
| [Controller PS4/PS5](/en/sensors/ps4/index.md) | Bluetooth gamepad control |
| [Controller PS2](/en/sensors/ps2/index.md) (Taiwan only) | 2.4G gamepad control |

## 2️⃣ Choose by Need

| You want to… | Recommended | Why |
| :--- | :--- | :--- |
| Follow lines reliably on typical maps | [Pathfinder 8-Way](/en/sensors/line8/index.md) | 8 channels, compact — the most common competition choice |
| Take fast turns and many intersections | [Pathfinder 16-Way](/en/sensors/line16/index.md) | 16-channel wide view reads turns and junctions at once |
| Detect obstacles left and right | [Ranger 2-Way](/en/sensors/tof2/index.md) | Two laser points, small and fast |
| Sumo, all-round avoidance | [Ranger 8-Way](/en/sensors/tof8/index.md) | 180° in one read, reports the nearest target's direction |
| Get more ports on SPIKE | [SPIKE 6-Way Expander](/en/sensors/exp6/index.md) | Six channels from one port, own power |
| Use servos with SPIKE | [SPIKE 6-Port Servo Expander](/en/sensors/servo6/index.md) | Six servos from one port, hub-powered (coming soon) |
| Remote control at crowded events | [Controller PS4/PS5](/en/sensors/ps4/index.md) | Bonds to your gamepad only, stops on signal loss |
| Remote control for classes on a budget | [Controller PS2](/en/sensors/ps2/index.md) (Taiwan only) | 2.4G gamepad, best value |
| Track colors and find objects | [Sharpshooter ESP32CAM](/en/sensors/esp32cam/index.md) | Set up on its own screen, blob position out of the box |
| Drive straight, turn precisely | [IMU Gyro](/en/sensors/imu/index.md) | Any mounting angle, compass fusion without drift (coming soon) |

<div class="lp-final">
<h2>Still not sure?</h2>
<p>Tell us your controller and competition, and our engineers will suggest the best combination.</p>
<div class="lp-btns">
<a class="lp-btn line" href="https://line.me/R/ti/p/@692vcvuk" target="_blank" rel="noopener">💬 Ask on LINE</a>
<a class="lp-btn ghost" href="/en/contact.md">📬 Other contact options</a>
</div>
</div>
