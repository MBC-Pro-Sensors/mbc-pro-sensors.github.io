<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# ❓ Frequently Asked Questions

Quick answers about MBC-Pro Sensors for students, coaches and teachers. Can't find your question? Email [contactmbcrobot@gmail.com](mailto:contactmbcrobot@gmail.com).

## Compatibility

### Do MBC sensors work with LEGO® Education SPIKE™ Prime and the official SPIKE app?

Yes. The line followers (Pathfinder 8-Way and 16-Way) plug straight into any SPIKE™ Prime port and can be read with the standard SPIKE blocks, so beginners can start without writing Python. Advanced teams can use Python or [Pybricks](/en/sensors/line8/spike-pybricks.md) for faster loops.

### Do I need an adapter board or a driver?

No. MBC sensors use the same connectors and communication protocols as the LEGO® hub ports. Plug the sensor in and the hub recognizes it.

### Which controllers and software are supported?

- **LEGO® SPIKE™ Prime / Robot Inventor:** official SPIKE app and Pybricks
- **LEGO® MINDSTORMS® EV3:** EV3 software, EV3 Classroom, clev3r and Pybricks
- **Arduino, ESP32, Raspberry Pi:** the shared [Universal I2C Protocol](/en/i2c-protocol.md)
- **MATRIX Mini R4 and MakeBlock:** selected products, see the [Buying Guide](/en/guide.md)

### Are the SPIKE and EV3 editions interchangeable?

No. Each product comes in controller-specific editions with different connectors and protocols. Please confirm your controller model before ordering. The [Buying Guide](/en/guide.md) lists which edition you need.

### Does the SPIKE 6-Way Expander (EXP6) work with SPIKE word blocks?

No. The EXP6 works with **SPIKE™ Prime only** (not Robot Inventor or EV3). In the official SPIKE app it requires **Python text mode**; it is also fully supported in Pybricks. See the [EXP6 page](/en/sensors/exp6/index.md).

## Competitions

### Can I use MBC sensors in WRO?

Yes. Since the 2025 season, WRO RoboMission allows hardware from any brand. Some age-group limits apply: cameras are for Junior and Senior only, and remote-control receivers cannot be used during runs. See our [WRO Rules Check](/en/wro-rules.md) for each product.

### Can I use MBC sensors in FIRST® LEGO® League?

Not in official FIRST® LEGO® League Challenge matches, which allow LEGO-made parts only. You can use them for classes, practice and competitions with open hardware rules.

### Is the laser in the Ranger sensors eye-safe?

Yes. The laser ranging components in both Ranger sensors (TOF2 and TOF8) are Class 1 laser devices compliant with IEC 60825-1:2014. WRO asks for proof of eye safety at inspection; see the [WRO Rules Check](/en/wro-rules.md#_3-laser-safety-proof-of-eye-safety-is-required-5218) for what to show.

### Does the Sharpshooter Vision (ESP32CAM) use Wi-Fi?

No. The factory firmware contains no Wi-Fi or Bluetooth code, so the radio is never turned on. The camera talks to your hub by cable.

## Choosing a Product

### Should I choose the Pathfinder 8-Way or 16-Way?

Choose the **8-Way** for reliable line following on typical maps; it is compact and the most common competition choice. Choose the **16-Way** for fast turns and many intersections, because its wider view reads turns and junctions in one read.

### How do I calibrate the line sensors?

Press the calibration button, sweep the sensor over the white surface and the black line on your field, then press the button again to save. The calibration is stored in the sensor and kept after power-off. Details are on the [Pathfinder 8-Way page](/en/sensors/line8/index.md).

### How far can the Ranger 8-Way (TOF8) see?

From 0 to 1,200 mm, across 180° in front of the robot (90° to each side). A reading of 1,200 mm means there is no target in that direction.

### Will the PS4/PS5 receiver connect to another team's controller?

No. The receiver bonds only to your own controller, so it will not pick up other teams' gamepads at a busy event. If the signal is lost, its outputs return to zero within 0.3 seconds. In open-field tests it held a stable connection at 100 m.

## Pricing and Ordering

### How much do MBC sensors cost?

Manufacturer's suggested retail prices (MSRP) in US dollars, shipping not included:

| Product | MSRP (USD) |
| :--- | :---: |
| Pathfinder 8-Way (Line8) | 100 |
| Pathfinder 16-Way (Line16) | 120 |
| Ranger 2-Way (TOF2) | 60 |
| Ranger 8-Way (TOF8) | 120 |
| Sharpshooter Vision (ESP32CAM) | 120 |
| SPIKE 6-Way Expander (EXP6) | 145 |
| Controller PS4/PS5 | 100 |
| IMU 9-Axis Gyro (coming soon) | 100 |

Controller 2.4G (PS2) is sold in Taiwan only.

### How do I order, and do you work with distributors?

Email [contactmbcrobot@gmail.com](mailto:contactmbcrobot@gmail.com) or message us on [LINE](https://line.me/R/ti/p/@692vcvuk) with your controller, the products you need and your country. We reply within 24 hours on weekdays. School orders and distributor enquiries are welcome.
