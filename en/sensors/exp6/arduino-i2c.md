<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# ∿ EXP6 × Arduino: Please Use Serial (UART)

[🔙 Back to EXP6 overview](/en/sensors/exp6/index.md)

> [!IMPORTANT]
> **EXP6 cannot connect to Arduino over I2C.** The EXP6 host port has an **isolation protection chip** that keeps Arduino and EXP6 from damaging each other. This chip only sends signals in one direction, but I2C needs a two-way data line, so I2C will not work.
>
> Please control EXP6 from Arduino over **serial (UART)** instead: no firmware change and no hardware change. EXP6 works right out of the box.

## 👉 See the New Arduino Examples

[🧪 EXP6 × Arduino Examples (UART)](/en/sensors/exp6/arduino-examples.md): 6 examples you can copy and upload right away, from "check the connection" to "two-wheel car", with an English note on every line.

| What you want to do | Example |
| :--- | :--- |
| Check the connection, see what is on the 6 ports and the battery voltage | Example 1 |
| Make a motor run and stop | Example 2 |
| Turn a motor to a set angle | Example 3 |
| Read a motor angle | Example 4 |
| Read the color, ultrasonic and force sensors | Example 5 |
| Two-wheel car | Example 6 |

> [!NOTE]
> Serial needs a second serial port on the Arduino, so we recommend the **Arduino UNO R4** (same shape as the UNO). Arduino Mega and ESP32 also work. The older UNO R3 / Nano have only one serial port (used by USB), so they cannot connect to EXP6. See the examples page for wiring details.
