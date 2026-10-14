<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# 🏆 WRO Rules Check: Can I Use MBC Sensors?

Since the 2025 season, WRO RoboMission allows controllers, motors and sensors from **any brand**. A few rules still matter for MBC products: cameras, LIDAR, laser safety and wireless communication. This page goes through them based on the **WRO RoboMission General Rules (2025 edition)**. Last reviewed: 2026-10-13.

> [!IMPORTANT]
> Rules can change every season, and national versions may differ. **Always check the latest rules published by WRO and your National Organizer before you register.** If in doubt, ask your National Organizer.

For other competitions (Robofest, RoboCupJunior, AERC, GRG and more), see the [Competition Guide](/en/competitions.md).

## At a Glance: Which Age Groups Can Use Each Product (RoboMission)

| Product | Elementary | Junior | Senior | Notes |
| :--- | :---: | :---: | :---: | :--- |
| [Pathfinder 8-Way](/en/sensors/line8/index.md) / [16-Way](/en/sensors/line16/index.md) | ✅ | ✅ | ✅ | No limit on sensor type or number |
| [IMU 9-Axis Gyro](/en/sensors/imu/index.md) | ✅ | ✅ | ✅ | Coming soon |
| [Ranger 2-Way (TOF2)](/en/sensors/tof2/index.md) | ✅ | ✅ | ✅ | Laser eye-safety proof required (see point 3) |
| [Ranger 8-Way (TOF8)](/en/sensors/tof8/index.md) | ✅* | ✅* | ✅ | *Eight fixed single-point ToF sensors, not a rotating LIDAR scanner. Confirm with your National Organizer before the event (see point 2) |
| [Sharpshooter Vision (ESP32CAM)](/en/sensors/esp32cam/index.md) | ❌ | ✅ | ✅ | Cameras are limited to Junior and Senior |
| [SPIKE 6-Way Expander (EXP6)](/en/sensors/exp6/index.md) | ✅ | ✅ | ✅ | Motors on the expander count toward your age group's motor limit; the external battery must meet the power rules |
| [Controller 2.4G (PS2)](/en/sensors/ps2/index.md) / [PS4/PS5](/en/sensors/ps4/index.md) | ❌ | ❌ | ❌ | Remote control is not allowed during RoboMission runs. Use them for events that allow remote control, classes and demos |

## The Rules in Detail

### 1. Open hardware (General Rules 5.2)

Teams may build their robot from any materials and components, as long as the weight, battery, voltage and current limits are met. Third-party sensors are allowed.

### 2. Sensors: cameras and LIDAR are limited by age group (5.2.6)

- In general there is no limit on the type or number of sensors.
- **Cameras may only be used in the Junior and Senior age groups**, so the ESP32CAM cannot be used in Elementary.
- **LIDAR and other 3D scanners may only be used in the Senior age group.**

The Ranger 8-Way (TOF8) consists of eight **fixed-direction** single-point ToF laser distance sensors (from -90° to +90°, each measuring one direction). It does not rotate or scan and does not produce a 3D point cloud; it works like an ordinary distance sensor. The final classification is up to the judges and organizers, so **Elementary and Junior teams should confirm with their National Organizer before the event.**

### 3. Laser safety: proof of eye safety is required (5.2.18)

Lasers must not pose a safety risk, and a certificate showing the laser is safe for eyes is required.

The ToF laser ranging components in both Ranger sensors are specified by their manufacturer as a **Class 1 laser device compliant with IEC 60825-1:2014 (3rd edition)**, with a 940 nm emitter that is eye-safe in normal use. For the written statement needed at inspection, please [contact us](/en/contact.md).

### 4. Wireless communication must be off during the event (Chapter 5)

- Radio communication, remote control and wired control are not allowed while the robot is running, and components inside the robot may not communicate wirelessly.
- Bluetooth, Wi-Fi and any remote connection must be switched off during check time and robot runs. If there is any doubt, the team must be able to show that wireless transmission is off and how it was done.

| Product | Wireless status |
| :--- | :--- |
| Sharpshooter Vision (ESP32CAM) | The factory firmware contains **no Wi-Fi or Bluetooth code**, so the radio is never turned on. It connects to the hub by cable |
| Controller PS2 / PS4/PS5 | These are remote-control receivers and **cannot be used in RoboMission** |
| All other products | Connected to the hub by cable, no wireless function |

### 5. Power and size (5.1, 5.2.1–5.2.4)

| Item | Limit |
| :--- | :--- |
| Size before the start | Within 250 × 250 × 250 mm (cables included) |
| Total weight | ≤ 1.5 kg |
| Battery capacity | ≤ 8,000 mAh |
| Voltage | ≤ 18 V |
| Current | ≤ 5 A |

The EXP6 expander powers motors from its own external battery (2 or 3 cells). Make sure the battery you use also meets these limits.

### 6. Number of motors (5.2.7)

Any type of motor is allowed, but each age group has a maximum number. Motors connected through the EXP6 count toward the total.

## What About FLL?

FIRST® LEGO® League Challenge rules require robots to be built only from LEGO-made parts, so MBC products **cannot be used in official FLL Challenge matches**. They are fine for classes, practice and other competitions with open hardware rules.

## Sources

- [WRO: Competition categories](https://wro-association.org/competition/open-category/)
- [WRO: Rules Q&A](https://wro-association.org/competition/questions-answers/)
- [WRO RoboMission General Rules (Singapore national version, January 2025)](https://spacefaculty.asia/wp-content/uploads/2026/07/WRO-Singapore-RoboMission-General-Rules-1.0.pdf)

Still not sure about your age group? Email us at [contactmbcrobot@gmail.com](mailto:contactmbcrobot@gmail.com) with your competition and age group, and an engineer will check for you.
