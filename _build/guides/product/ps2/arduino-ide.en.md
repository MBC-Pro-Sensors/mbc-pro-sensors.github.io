### Connect the gamepad to the receiver

The PS2 2.4G gamepad and its receiver are **paired at the factory**; no pairing button needed.

1. Connect the receiver and turn the controller on.
2. **Switch the gamepad on**; it connects by itself.
3. Watch the **green LED** on the receiver: **on = no gamepad yet**; **off = gamepad connected**.
4. The **ANALOG (or MODE) light** on the gamepad must be on for the sticks to give values. The receiver switches to this mode on connect; if the light is off, press the ANALOG button in the middle of the gamepad.

> [!CAUTION]
> 2.4 GHz is a shared band (Wi-Fi, microwave ovens). With **12 or more sets** in use at one venue they may interfere; in that case use the Bluetooth version (PS4 / PS5).

> [!WARNING]
> **With only ■ pressed, 16 buttons (id `01`) = `0x8000` = -32768**, exactly the same as `MBC_NONE` for no answer. To detect "no answer", use the true / false returned by `mbcReadMany`, not a test for -32768: `int16_t b; if (mbcReadMany(0x42, 0x01, 1, &b)) { … }`; true means it was read, then use `(uint16_t)b` as the 16 buttons.
