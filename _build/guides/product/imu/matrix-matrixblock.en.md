### Zero the heading

MATRIXblock has no "send command" block, but the IMU firmware treats these two actions as **zero the heading**:

- Running the "colour sensor init" block (it already runs once at power-on, so the heading starts at 0).
- Reading the "colour sensor K" cell once.

To zero again during a run, read K once at that point. **Do not read K inside a loop**, or the heading keeps resetting to 0.

> [!TIP]
> **Tested with the stock library (2026-11-02, MatrixMiniR4 1.1.5):** all three headings, pitch and zeroing read correctly; 2000 reads in a row with 0 errors. In the Arduino IDE, choose the board "**UNO R4 WiFi**".
