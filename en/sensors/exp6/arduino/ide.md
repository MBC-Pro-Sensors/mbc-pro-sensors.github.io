<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（exp6 × arduino × ide），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: exp6/arduino/ide -->
# SPIKE 6-Way Expander (EXP6) × Arduino × Arduino IDE

This page covers **Arduino IDE (C++)** only: how to set it up for the SPIKE 6-Way Expander (EXP6) on Arduino / ESP32 / UNO R4 (I2C), how to read every value and examples. Use the switcher above to change product, host or software.

## Which edition and how to connect

The EXP6 **cannot use I2C** with Arduino (the host-port isolator is one-way); use **serial (UART)** instead, ideally with an Arduino UNO R4, Mega or ESP32.

## Setup

<!-- setup: _build/guides/software/arduino-ide.en.md + _build/guides/product/exp6/arduino-ide.en.md {"product":"SPIKE 6-Way Expander (EXP6)","edition":"serial (UART) connection","port":"I2C","lib":"","libUrl":"","addr":"","skipWiring":true} -->
<!-- include-fallback -->
> [!NOTE]
> Setup steps for this software are being moved onto this page. Meanwhile see the previous guide: [SPIKE 6-Way Expander (EXP6): Arduino IDE (C++)](/en/sensors/exp6/arduino-i2c.md).
<!-- /include-fallback -->

## Examples

<!-- examples: en/sensors/exp6/arduino-examples.md host=arduino sw=ide whole -->

## More questions

- **Always reading -32768?** The module is not answering: check SDA / SCL are not swapped, the address is right and 5V / GND are connected.
- **Can I connect several sensors?** Yes, each needs its own address; re-address identical sensors with command F8 (see the universal I2C protocol).

Still stuck? [Contact us](/en/contact.md) with your software and a screenshot of the program.

## Downloads

- [All examples exp6_arduino_examples_en.zip](/examples/exp6/arduino-en/exp6_arduino_examples_en.zip)
- [Download centre (libraries, firmware, catalogue for every product)](/en/downloads.md)

## Sources

- Host and software list：`_build/guides.json`
- Setup：`_build/guides/software/arduino-ide.en.md`、`_build/guides/product/exp6/arduino-ide.en.md`
- Examples：`sensors/exp6/arduino-examples.md`
