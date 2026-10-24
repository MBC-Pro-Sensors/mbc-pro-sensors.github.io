<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# 🟠 micro:bit (MakeCode extension)

The universal I2C editions of the Pathfinder, Ranger, Controller, IMU gyro and Sharpshooter all work with BBC micro:bit V1 / V2: add one MakeCode extension, **pxt-mbc-sensors v2**, and an "MBC Sensors" category appears with one block per value. Under the hood it speaks the [MBC universal I2C protocol v2](/en/i2c-protocol.md).

> [!IMPORTANT]
> **Bench test pending**: extension v2.0.2 compiles in MakeCode for V1 and V2, but the blocks have not yet been checked against real sensors one by one. Block names follow the GitHub release: <https://github.com/contactmbcrobot/pxt-mbc-sensors>. This page will be updated after testing.

---

## 📥 Add the extension

1. Open <https://makecode.microbit.org/> → **New Project**.
2. Click **Advanced → Extensions** at the bottom of the toolbox (also in the gearwheel menu).
3. Paste `https://github.com/contactmbcrobot/pxt-mbc-sensors` into the search box and press Enter.
4. Click the **mbc-sensors** card. An "MBC Sensors" category appears with six groups: Line, Distance, Gamepad, IMU, Camera and Device.
5. Traditional Chinese block names are included: switch the editor language to 繁體中文 if you prefer.

> [!TIP]
> No approval, no download: teachers just hand the URL to students. To pin a version, paste `https://github.com/contactmbcrobot/pxt-mbc-sensors#v2.0.2`.

## 🔌 Wiring

The micro:bit I2C pins are fixed: **P19 = SCL, P20 = SDA** (on the edge connector, so you need a breakout board). On the MBC 4-pin sensor cable **TX goes to SCL and RX to SDA**, exactly as when wiring to an Arduino.

| Sensor | micro:bit | Note |
| :--- | :--- | :--- |
| SCL (TX) | **P19** | I2C clock |
| SDA (RX) | **P20** | I2C data |
| VCC | **3V** or external 5 V | see the table below; power-hungry products must not draw from the micro:bit 3V pin |
| GND | GND | with external power, share GND with the micro:bit |

| Product | Power | Why |
| :--- | :--- | :--- |
| Pathfinder 8 / 16-way, Ranger 2-way, Controller PS2, IMU gyro | micro:bit 3V works (external 5 V recommended on batteries) | STM32 products, low current |
| **Ranger 8-way** | **external 5 V required** | 8 lasers ranging at once draw more than the 3V pin can supply |
| **Controller PS4 / PS5** | **external 5 V required** | ESP32 Bluetooth receiver, high peak current |
| **Sharpshooter ESP32CAM** | **external 5 V required** | ESP32 camera + screen + flash light |

Several products can share the same P19 / P20 bus as long as their I2C addresses differ (factory addresses: Pathfinder `0x16`, Ranger `0x29`, Controller `0x42`, IMU `0x6A`, Sharpshooter `0x4C`).

## ⚠️ Hardware checklist

| Item | Status |
| :--- | :--- |
| **Logic level** | MBC products accept 3.3 V or 5 V on VCC; their I2C lines are 3.3 V, so they are compatible with the micro:bit without a level shifter. |
| **3V pin current** | V1 about **90 mA**, V2 about **190 mA**; on batteries the rail can drop to just over 2 V. Products above the limit (see the table) must use external power. |
| **V1 vs V2** | On V1 the external I2C bus is shared with the onboard accelerometer: a sensor holding SDA low also kills the compass and accelerometer. V2 has a separate external bus. Both run at 100 kHz. |
| **Clock stretching** | STM32 products (Pathfinder, Ranger, PS2, IMU) hold SCL while preparing data; the nRF52 on the V2 has a known clock-stretching issue (errata 219), so **V1 and V2 both need a bench test** — if unstable, the firmware will be switched to no-stretch. ESP32 products (PS4, Sharpshooter) do not stretch; the extension already waits 2 ms after writing the value id. |
| **Pull-ups** | The micro:bit has 4.7 kΩ pull-ups on board; sensor boards with their own pull-ups add in parallel, which is normally still fine. |
| **Same address, same bus** | Pathfinder 8 and 16 are both `0x16`, Ranger 2 and 8 both `0x29`, PS2 and PS4 both `0x42`. To use two of a kind together, first change one with the "Device → change device at … to address …" block (saved in the sensor, applied after reboot), then tell the extension with the "… use address" block of that product. |
| **Sensor not connected** | Reporter blocks return **0** (the internal −32768 is filtered) and never throw; use "Device → wait for device at … ready" to confirm the wiring first. |

## 🧩 Block list (83 blocks)

One line per block: block text (English ｜ Traditional Chinese), function, range. **Advanced** blocks are under "… more" in the group.

### Device (shared, advanced)

| Block | Function | Range |
| :--- | :--- | :--- |
| device at … is ready ｜ 位址 … 的裝置就緒？ | device answers and F3 bit0 = 1 | true / false |
| wait for device at … ready, timeout … ms ｜ 等位址 … 的裝置就緒，最多 … 毫秒 | wait until ready or timeout | 0 ~ 60000 ms, default 5000 |
| device at … factory id ｜ 位址 … 的裝置 出廠 ID | F0 low byte: factory address (unchanged after re-addressing) | 0x16 / 0x29 / 0x42 / 0x6A / 0x4C |
| device at … kind ｜ 位址 … 的裝置 類別 | F0 high byte: Pathfinder channels 8 / 16, Ranger sensors 2 / 8, Controller 1 = PS2 / 2 = PS4, IMU 1, Sharpshooter 1 = school / 2 = WRO | 0 ~ 255 |
| device at … protocol version ｜ 位址 … 的裝置 協議版本 | F1, must be 2 for this extension | 2 |
| device at … firmware version ｜ 位址 … 的裝置 韌體版本 | F2 as a "major.minor" string | e.g. "4.0" |
| device at … status flags ｜ 位址 … 的裝置 狀態旗標 | whole F3 byte | 0 ~ 255 |
| device at … status bit … ｜ 位址 … 的裝置 狀態 bit … | one bit of F3 | bit 0 ~ 7 |
| device at … update counter ｜ 位址 … 的裝置 更新序號 | F4, +1 every 10 ms; frozen = device busy | 0 ~ 255 |
| device at … uptime (s) ｜ 位址 … 的裝置 開機秒數 | F5 | 0 ~ 32767 |
| change device at … to address … ｜ 把位址 … 的裝置 改成位址 … | F8, saved to EEPROM, applied after reboot | 0x08 ~ 0x77 |
| restore factory address of device at … ｜ 位址 … 的裝置 回出廠位址 | F8 `[00][FF]`, applied after reboot | — |
| reboot device at … ｜ 位址 … 的裝置 重新開機 | EF | — |

### Line (Pathfinder 8 / 16-way, `0x16`)

| Block | Function | Range |
| :--- | :--- | :--- |
| line position ｜ 循行者 線位置 | which channel the line is under; viewed from behind the robot, **negative = left** | −8 ~ +8 (8-way) / −16 ~ +16 (16-way), 0 = centre |
| fine line position ｜ 循行者 細位置 | sub-channel position, same sign as line position | −100 ~ +100 |
| line is seen ｜ 循行者 看到線？ | segments > 0 | true / false |
| line width ｜ 循行者 線寬 | channels on the line | 0 ~ 8 / 16, 0 = no line |
| line segments ｜ 循行者 線段數 | 0 lost, 1 one line, ≥2 fork / crossing | 0 ~ 4 (8-way) / 0 ~ 8 (16-way) |
| line bitmap ｜ 循行者 壓線位元圖 | bit0 = channel 1 (leftmost) … | 0 ~ 255 / 0 ~ 65535 |
| channel … is on the line ｜ 循行者 第 … 路在線上？ | one bit of the bitmap | channel 1 ~ 16 |
| channel … reflect ｜ 循行者 第 … 路光值 | calibrated reflect, 0 black ~ 100 white | 0 ~ 100 |
| last exit direction ｜ 循行者 最後離線方向 | where the line was before it was lost, negative = left | −8 ~ +8 / −16 ~ +16 |
| line threshold ｜ 循行者 門檻 | current "on line" reflect threshold | 1 ~ 99, default 50 |
| line calibration count ｜ 循行者 校準次數 (advanced) | calibrations completed since power-on | 0 ~ 255 |
| line follower is calibrating ｜ 循行者 校準中？ (advanced) | F3 bit2 | true / false |
| line follower follow … line ｜ 循行者 循 … 線 | black (E0, default) / white (E1) | black / white |
| line follower calibrate … ｜ 循行者 校準 … | current reading is black (E2) / is white (E3) / auto sweep 5 s (E7), saved to EEPROM | 3 modes |
| line follower set threshold … ｜ 循行者 門檻設為 … | E4, RAM only, back to 50 after reboot | 1 ~ 99 |
| on line … ｜ 當循行者 … | event: lost / found / fork | 3 events |
| line follower use address … ｜ 循行者 改用位址 … (advanced) | tell the extension a changed address | 0x08 ~ 0x77 |

### Distance 2-ch (Ranger 2-way, `0x29`)

| Block | Function | Range |
| :--- | :--- | :--- |
| distance 2-ch … (…) ｜ 測距者 2 路 … 距離（…） | left / right sensor, cm or mm | 0 ~ 120 cm (120 = nothing) / 0 ~ 1200 mm |
| distance 2-ch nearest (cm) ｜ 測距者 2 路 較近距離（公分） | the nearer of the two | 0 ~ 120 |
| distance 2-ch average (mm) ｜ 測距者 2 路 平均距離（公釐） | average of left and right (= EV3 mode 3 / SPIKE mode 6) | 0 ~ 1200 |
| distance 2-ch … sensor is OK ｜ 測距者 2 路 … 感應器正常？ (advanced) | that sensor initialised and not faulty | true / false |
| on distance 2-ch nearer than … cm ｜ 當測距者 2 路 距離小於 … 公分 | fires once when the nearer distance drops below the threshold | 1 ~ 120, default 10 |
| distance 2-ch use address … ｜ 測距者 2 路 改用位址 … (advanced) | tell the extension a changed address | 0x08 ~ 0x77 |

### Distance 8-ch (Ranger 8-way, `0x29`)

| Block | Function | Range |
| :--- | :--- | :--- |
| distance 8-ch nearest (…) ｜ 測距者 8 路 最近距離（…） | nearest of all 8, cm or mm | 0 ~ 120 / 0 ~ 1200 |
| distance 8-ch nearest direction ｜ 測距者 8 路 最近方向 | direction of the nearest object, negative = left | −8 ~ +8, 0 = straight ahead, ±8 = nothing |
| distance 8-ch most open (cm) ｜ 測距者 8 路 最空曠距離（公分） | distance of the farthest direction | 0 ~ 120 |
| distance 8-ch most open direction ｜ 測距者 8 路 最空曠方向 | farthest direction, negative = left | −7 ~ +7 |
| distance 8-ch front (…) ｜ 測距者 8 路 正前方距離（…） | average of sensors 4 and 5 | 0 ~ 120 / 0 ~ 1200 |
| distance 8-ch sensor … (…) ｜ 測距者 8 路 第 … 顆距離（…） | one sensor, 1 = leftmost, 8 = rightmost | sensor 1 ~ 8 |
| distance 8-ch sensor … is OK ｜ 測距者 8 路 第 … 顆正常？ (advanced) | that sensor initialised and not faulty | true / false |
| on distance 8-ch nearer than … cm ｜ 當測距者 8 路 距離小於 … 公分 | fires once when the nearest distance drops below the threshold | 1 ~ 120, default 10 |
| distance 8-ch use address … ｜ 測距者 8 路 改用位址 … (advanced) | tell the extension a changed address | 0x08 ~ 0x77 |

All 8 sensors take about 2 ~ 4 s to initialise after power-on: use "Device → wait for device at 0x29 ready" before reading.

### Gamepad (Controller PS2 / PS4 / PS5, `0x42`)

| Block | Function | Range |
| :--- | :--- | :--- |
| gamepad button … is pressed ｜ 掌控者 按鈕 … 被按住？ | any of the 16 buttons | select/share, L3, R3, start/options, up/right/down/left, L2, R2, L1, R1, triangle, circle, cross, square |
| gamepad buttons bit mask ｜ 掌控者 十六鍵位元圖 | PSX layout: bit0 select, 1 L3, 2 R3, 3 start, 4 up, 5 right, 6 down, 7 left, 8 L2, 9 R2, 10 L1, 11 R1, 12 triangle, 13 circle, 14 cross, 15 square | 0 ~ 65535 |
| gamepad … buttons (0-15) ｜ 掌控者 …（0~15） | one 4-button group: d-pad (up 1, right 2, down 4, left 8) / shapes (triangle 1, circle 2, cross 4, square 8) / shoulders (L2 1, R2 2, L1 4, R1 8) / functions (select 1, L3 2, R3 4, start 8) | 0 ~ 15 |
| gamepad joystick … ｜ 掌控者 搖桿 … | left X / left Y / right X / right Y, right and up positive | −100 ~ +100, 0 = centre |
| gamepad trigger … analog ｜ 掌控者 扳機 … 類比值 | L2 / R2 pressure (PS2 digital pads report 255 when pressed) | 0 ~ 255 |
| gamepad is connected ｜ 掌控者 手把已連線？ | F3 bit1 | true / false |
| gamepad battery percent ｜ 掌控者 電量（百分比） | PS4 / PS5 gamepad battery; PS2 always 0 | 0 ~ 100 |
| gamepad system button … is pressed ｜ 掌控者 系統鍵 … 被按住？ | PS / capture button (PS4 / PS5 only) | true / false |
| gamepad type ｜ 掌控者 手把類型 | 0 none, 1 PS2 digital, 2 PS2 analog, 3 PS4, 4 PS5 | 0 ~ 4 |
| gamepad rumble strength … ｜ 掌控者 震動 強度 … | E5, rumbles a PS4 / PS5 gamepad for 200 ms; PS2 ignores | 0 ~ 255, 0 = stop |
| gamepad light bar colour … ｜ 掌控者 燈條顏色 … | E6, PS4 / PS5 light bar RGB; PS2 ignores | colour |
| on gamepad button … … ｜ 當掌控者 按鈕 … … | event: a button pressed / released | 16 buttons × 2 |
| gamepad use address … ｜ 掌控者 改用位址 … (advanced) | tell the extension a changed address | 0x08 ~ 0x77 |

### IMU (gyro, `0x6A`)

| Block | Function | Range |
| :--- | :--- | :--- |
| IMU heading … (°) ｜ 陀螺儀 … 航向（°） | fused / gyro 3D / gyro 1D / magnetic heading; keeps counting past 360, left turn = negative | −32768 ~ 32767 |
| IMU turn rate (0.1 °/s) ｜ 陀螺儀 轉向角速度（0.1 °/s） | rate around the vertical axis, 123 = 12.3 °/s | −32768 ~ 32767 |
| IMU pitch (°) ｜ 陀螺儀 俯仰角（°） | nose up / down angle | ° |
| IMU pitch rate (0.1 °/s) ｜ 陀螺儀 俯仰角速度（0.1 °/s） | pitch rate | −32768 ~ 32767 |
| IMU … axis … ｜ 陀螺儀 … … 軸 | raw values: gyro (0.1 °/s) / accelerometer (mg) / magnetometer (mG), X / Y / Z | −32768 ~ 32767 |
| IMU face up ｜ 陀螺儀 朝上的面 | 0 +X, 1 +Y, 2 +Z, 3 −X, 4 −Y, 5 −Z | 0 ~ 5 |
| IMU face up is … ｜ 陀螺儀 朝上的面是 …？ | test one face | 6 faces |
| IMU magnetic trust ｜ 陀螺儀 磁力信任度 | how much the magnetometer is believed | 0 ~ 255 |
| IMU magnetic disturbance ｜ 陀螺儀 磁場干擾度 | higher = more interference | 0 ~ 255 |
| IMU is … ｜ 陀螺儀 …？ | status flags: still / mounting axis captured / mounted too tilted / accelerometer calibrating / magnetometer calibrating / magnetic fusion ready / magnetic auto-calibration converged | true / false |
| IMU zero heading ｜ 陀螺儀 航向歸零 | E0, all headings to 0 | — |
| IMU calibration … ｜ 陀螺儀 校準 … | E1 ~ E4: accelerometer start / end, magnetometer start / end; results stay in the IMU | 4 steps |
| on IMU turned by … ° ｜ 當陀螺儀 轉了 … ° | fires every time the fused heading changes by the given angle | e.g. 90 |
| IMU use address … ｜ 陀螺儀 改用位址 … (advanced) | tell the extension a changed address | 0x08 ~ 0x77 |

### Camera (Sharpshooter school / WRO edition, `0x4C`)

| Block | Function | Range |
| :--- | :--- | :--- |
| camera blob is found ｜ 神攝手 有找到色塊？ | school edition: a blob of the target colour is found | true / false |
| camera blob … ｜ 神攝手 色塊 … | school edition: X / Y / width / height / area | X 0 ~ 100, Y 0 ~ 75 (bottom → top), width 0 ~ 100, height 0 ~ 75, area = width × height |
| camera colour … ｜ 神攝手 顏色 … | school edition pick point: hue / saturation / brightness | hue 0 ~ 100 (−1 = black / white / grey), saturation 0 ~ 100, brightness 0 ~ 100 |
| camera colour is black / white / grey ｜ 神攝手 顏色是黑白灰？ | pick point has no hue | true / false |
| camera motion ｜ 神攝手 動態 | school edition motion level | 0 ~ 100 |
| camera spot … colour code ｜ 神攝手 色點 … 顏色碼 | WRO edition (HUE12): 0 ~ 239 colour, 240 ~ 245 white, 246 ~ 250 black, 255 = spot off | spot 1 ~ 12 |
| camera spot … vividness ｜ 神攝手 色點 … 鮮豔度 | WRO edition saturation of that spot | 0 ~ 255 |
| camera spot … is … ｜ 神攝手 色點 … 是 …？ | WRO edition: colour / white / black / off | true / false |
| on camera blob … ｜ 當神攝手 色塊 … | school edition event: found / lost | 2 events |
| camera use address … ｜ 神攝手 改用位址 … (advanced) | tell the extension a changed address | 0x08 ~ 0x77 |

The Sharpshooter's I2C uses **SDA = IO3, SCL = IO1** (decided 2026-10-21); once I2C is wired the USB serial monitor is no longer available. Hardware confirmed, **bench test pending**.

## 📖 Per-product micro:bit pages and examples

One page per product lists the blocks it uses, the matching I2C value ids and the JavaScript form; the example pages have 5 programs each (.ts, paste into MakeCode JavaScript mode).

| Product | Blocks | Examples (5) |
| :--- | :--- | :--- |
| Pathfinder 8-way | [line8 × micro:bit](/en/sensors/line8/microbit.md) | [examples](/en/sensors/line8/microbit-examples.md) |
| Pathfinder 16-way | [line16 × micro:bit](/en/sensors/line16/microbit.md) | [examples](/en/sensors/line16/microbit-examples.md) |
| Ranger 2-way | [tof2 × micro:bit](/en/sensors/tof2/microbit.md) | [examples](/en/sensors/tof2/microbit-examples.md) |
| Ranger 8-way | [tof8 × micro:bit](/en/sensors/tof8/microbit.md) | [examples](/en/sensors/tof8/microbit-examples.md) |
| Controller PS2 | [ps2 × micro:bit](/en/sensors/ps2/microbit.md) | [examples](/en/sensors/ps2/microbit-examples.md) |
| Controller PS4 / PS5 | [ps4 × micro:bit](/en/sensors/ps4/microbit.md) | [examples](/en/sensors/ps4/microbit-examples.md) |
| IMU gyro | [imu × micro:bit](/en/sensors/imu/microbit.md) | [examples](/en/sensors/imu/microbit-examples.md) |
| Sharpshooter ESP32CAM | [esp32cam × micro:bit](/en/sensors/esp32cam/microbit.md) | [examples](/en/sensors/esp32cam/microbit-examples.md) |

> [!NOTE]
> The SPIKE 6-way expander (EXP6) uses UART and is not part of this extension; the Servo6 has no micro:bit edition.

## 💻 JavaScript

Every block maps to a function in the `mbc` namespace, so you can write it directly in JavaScript mode:

```typescript
basic.forever(function () {
    if (mbc.waitReady(mbc.ADDR_TOF, 5000)) {
        basic.showNumber(mbc.tof8Nearest(MbcDistUnit.Cm))
    }
    basic.pause(100)
})
```

Advanced: `mbc.readValue(address, id)` reads any I2C v2 value (int16 little-endian, −32768 when the device is missing) and `mbc.writeCmd(address, id, [params])` sends a command; ids are listed in each product's [command reference](/en/i2c-protocol.md).

## 📦 Download and source

- Extension repo (MIT): <https://github.com/contactmbcrobot/pxt-mbc-sensors> (v2.0.2, 2026-10)
- Libraries and examples for the other platforms: [Downloads](/en/downloads.md)
