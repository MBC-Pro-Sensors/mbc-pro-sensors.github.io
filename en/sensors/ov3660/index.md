<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# Long-Range Colour Sensor OV3660 (In Development)

<!-- product-hero -->

---

> [!NOTE]
> This product is **still in development**: the circuit and firmware design are final, but the custom board has not been fabricated yet, and there is no launch date or price. This page is a preview; features and specifications may change. [Contact us](/en/contact.md) if you want to join the testing.

## 🚀 Why this sensor

The LEGO colour sensor must be within 1-2 cm of an object to read reliably, and colours wash out at a distance; photo-diode colour sensors are easily fooled by ambient light. On competition fields the coloured blocks and markers are often 10-30 cm ahead of the robot, so the robot has to drive up before it knows the colour.

The **Long-Range Colour Sensor OV3660** uses a 3-megapixel image sensor and **reads only a small window in the centre of the frame** (20×20 or 40×40 pixels): a distant colour block fills that window with one uniform colour, so the decision is fast and stable without analysing a whole image like a camera.

## 🧠 Planned features

- 🎯 **Colour at a distance**: centre-window colour detection stays accurate far away; window size is adjustable.
- ⚡ **120 fps update**: hardware crop and scale inside the image sensor; the controller only moves the small window, so updates are faster than ordinary colour sensors.
- 🔇 **Clean analog power**: the sensor's three supply rails are regulated and filtered separately, so noise does not make colour thresholds jump.
- 📊 **Colour plus uniformity**: besides the colour, it reports how uniform the brightness in the window is, which tells you whether you are actually on the block.
- 🔌 **Hub powered**: no battery needed.

## 🧩 Planned platforms

| Platform | How |
| :--- | :--- |
| LEGO MINDSTORMS EV3 | Emulates an official sensor; read it with the standard blocks |
| LEGO SPIKE Prime / Robot Inventor | Emulates the official colour sensor; Pybricks and official-app libraries |
| Arduino / ESP32 / Raspberry Pi | [MBC universal I2C protocol](/en/i2c-protocol.md) |

## 📌 Status

| Item | Status |
| :--- | :--- |
| Circuit architecture, power, pin-out | ✅ Final (2026-07) |
| Capture and colour firmware | ✅ Being validated on a dev board |
| Custom board | ⏳ Not fabricated yet |
| EV3 / SPIKE / I2C communication | ⏳ After the board is made |
| Launch date, price | TBD |

> [!TIP]
> Need colour vision now? The [Sharpshooter Vision](/en/sensors/unicam/index.md) camera sensor lets you set the colour to track on its own screen and gives blob position and area, plus colour at points and line following (in development).
