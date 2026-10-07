<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# IMU 9-Axis Gyro Sensor (Coming Soon)

<div style="text-align: center;">
  <span style="display:inline-block; background:#a371f7; color:#fff; padding:3px 12px; border-radius:20px; font-family:monospace; margin-bottom:1rem; font-size: 0.85rem;">IMU-9AX · COMING SOON</span>
  <p style="font-family: monospace; color: #aaa; letter-spacing: 1px;">
    <strong>9-axis attitude sensor: gyroscope + accelerometer + magnetometer</strong><br>
    Mount at any angle · 3D-compensated heading · Compass fusion with no long-run drift
  </p>
</div>

---

> [!NOTE]
> This product is in development; final specifications may change at launch. [Contact us](/en/contact.md) for early access or launch timing.

## 🚀 Product Overview

Driving straight and turning precise angles depends on the gyro. Ordinary gyros have two old problems: they **must be mounted flat**, and the **heading slowly drifts** over time.

The MBC IMU combines 6-axis inertial sensing (gyroscope + accelerometer) with a magnetometer and offers three heading algorithms:

- **3D-compensated heading (Gyro3D)**: detects the mounting orientation automatically, so the heading is correct **at any mounting angle**. Use this first.
- **Compass-fused heading (GyroWithCompass)**: the magnetometer corrects the gyro's accumulated error — **no drift over long runs**.
- **Single-axis gyro + electronic compass**: classic single-axis heading and compass bearing.

## 🎯 Core Advantages

- 🧭 **Mount at any angle**: the mounting axis is captured automatically — upright, sideways or upside down
- 📉 **No long-run drift**: the compass-fused heading keeps correcting gyro error
- 🧱 **Works as the official EV3 gyro**: read it with the standard gyro blocks in the EV3 software
- 🔘 **Button calibration**: separate buttons for accelerometer and magnetometer calibration, stored on the device
- 📊 **Full 9-axis data**: raw angular rate, acceleration and magnetic field on all three axes

## 🧱 EV3 Block Mapping

In the official EV3 software, use the standard gyro block; each mode maps to:

<img src="/images/sensors/imu/ev3-gyro-blocks-en.webp" alt="EV3 gyro block modes and their heading algorithms" style="max-width: 100%; border-radius: 8px; background: #fff;" />

| EV3 gyro block mode | Function | Notes |
| :--- | :--- | :--- |
| Angle | **Gyro3D** 3D-compensated heading | Preferred; any mounting angle |
| Rate | **GyroWithCompass** fused heading | No drift over long runs; unreliable in distorted magnetic fields |
| Angle and Rate | Single-axis gyro, electronic compass | Classic single-axis heading and compass bearing |
| Reset | Reset all angles | All three headings reset together |

> [!WARNING]
> The magnetometer is affected by motors, steel tables, speakers and other magnetic sources. In magnetically noisy places, use **Gyro3D**.

## 🧩 Supported Platforms

| Platform | How |
| :--- | :--- |
| LEGO MINDSTORMS EV3 | EV3 official software (standard gyro blocks) |
| LEGO SPIKE Prime / Robot Inventor | Dedicated edition |
| Arduino / ESP32 / Raspberry Pi | Universal I2C (address `0x6A`) |
| MATRIX Mini R4 | I2C |

## ∿ Universal I2C Registers (address `0x6A`)

See the [MBC Universal I2C Protocol](/en/i2c-protocol.md) for shared rules and Arduino helpers. All axes are in the sensor body frame; headings are in 1° and read 0 at power-on or after a reset.

| Register | Dir | Len | Content |
| :---: | :---: | :---: | :--- |
| `0x10` | W | 1 | `0x01` = reset heading |
| `0x11` | R | 6 | Heading bundle (= `0x12` ~ `0x14`) |
| `0x12` | R | 2 | Single-axis gyro heading int16, degrees |
| `0x13` | R | 2 | 3D-compensated heading (Gyro3D) int16, degrees |
| `0x14` | R | 2 | Gyro + magnetometer fused heading int16, degrees |
| `0x20` | W | 1 | `0x01` / `0x02` start / stop accelerometer calibration; `0x03` / `0x04` start / stop magnetometer calibration |
| `0x21` | R | 18 | 9-axis bundle (= `0x22` ~ `0x24`) |
| `0x22` | R | 6 | Angular rate X / Y / Z int16, 0.1 °/s |
| `0x23` | R | 6 | Acceleration X / Y / Z int16, mg |
| `0x24` | R | 6 | Magnetic field X / Y / Z int16, mG (calibrated) |
| `0x32` | R | 1 | bit1 stationary, bit2 mounting axis captured, bit3 mounted too tilted, bit4 accel calibrating, bit5 mag calibrating, bit6 fusion initialized, bit7 mag auto-calibration converged |
| `0x34` | R | 1 | Magnetometer trust `0` ~ `255` |
| `0x35` | R | 1 | Magnetic disturbance level |
| `0x36` | R | 1 | Face pointing up (0 = +X, 1 = +Y, 2 = +Z, 3 = -X, 4 = -Y, 5 = -Z) |

```cpp
// Read the three headings
uint8_t d[6];
if (iicRead(0x6A, 0x11, d, 6)) {
  int16_t gyro1D  = (int16_t)((d[0] << 8) | d[1]);
  int16_t gyro3D  = (int16_t)((d[2] << 8) | d[3]);   // recommended
  int16_t compass = (int16_t)((d[4] << 8) | d[5]);
}
iicWrite(0x6A, 0x10, 0x01);                          // reset heading
```
