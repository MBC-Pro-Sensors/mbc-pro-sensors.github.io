<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-reference.mjs 從 _build/protocol/line8.json 產生（mblock），請勿手改；改內容請改 JSON 後重跑 node _build/gen-reference.mjs -->
<!-- moved: /en/sensors/line8/mblock/mblock5.md -->
<p class="moved-note">This page has moved into the new guide: <a href="/en/sensors/line8/mblock/mblock5.md">Pathfinder 8-Way › mBlock › mBlock 5</a></p>

# Line Follower 8-ch × mBlock 5 (Makeblock edition)

[🔙 Back to Line Follower 8-ch](/en/sensors/line8/index.md)

The Makeblock edition of the Line Follower 8-ch plugs into an RJ25 port on mBot, Ranger, Orion or MegaPi and is read with the **MBC Pro Sensors extension v2** blocks. Installation and usable ports per board: [Makeblock extension](/en/makeblock.md).

<p class="chips"><a class="chip chip-spike" href="/en/sensors/line8/spike-hub.md">SPIKE</a><a class="chip chip-ev3" href="/en/sensors/line8/ev3-hub.md">EV3</a><a class="chip chip-arduino" href="/en/sensors/line8/arduino-i2c.md">Arduino I2C</a><a class="chip chip-mblock current" href="/en/sensors/line8/mblock.md">mBlock</a><a class="chip chip-matrix" href="/en/sensors/line8/matrix.md">MATRIX</a><a class="chip chip-microbit" href="/en/sensors/line8/microbit.md">micro:bit</a></p>

> [!NOTE]
> Units shipped before 2026-10 use the legacy extension v1.0.0 (marked "legacy" on the download page); old and new blocks are not interchangeable.

## Wiring

<figure class="ref-fig"><img src="/images/figures/line8/wiring-mbot.svg" alt="Line Follower 8-ch × mBot" loading="lazy"></figure>

Sensor → mBot adapter → RJ25 port on the board (S1 = host TX, S2 = host RX). UART 38400 8N1.

## Reporter blocks

| Block | Returns | Id | Meaning |
| :--- | :--- | :--- | :--- |
| Pathfinder port [port] line position | -8~+8 | [`01`](/en/sensors/line8/reference.md#v01) | which channel the line is under: −8 (far left) to +8 (far right), 0 = centre; seen from behind the robot, negative = left |
| Pathfinder port [port] fine line position | -100~+100 | [`02`](/en/sensors/line8/reference.md#v02) | sub-channel position: −100 to +100, 0 = centre, negative = left (same sign as line position) |
| Pathfinder port [port] line width | 0~8 路 | [`03`](/en/sensors/line8/reference.md#v03) | number of channels on the line, 0~8; 0 = no line |
| Pathfinder port [port] line segments | 0~4 | [`04`](/en/sensors/line8/reference.md#v04) | number of separate line segments: 0 = lost, 1 = one line, ≥2 = fork / crossing |
| Pathfinder port [port] line bitmap | 0~255 bit | [`05`](/en/sensors/line8/reference.md#v05) | bit0 = channel 1 (left) … bit7 = channel 8; 1 = that channel sees the line |
| Pathfinder port [port] last exit direction | -8~+8 | [`06`](/en/sensors/line8/reference.md#v06) | where the line was before it was lost: −8 to +8 (negative = left); keeps the last value |
| Pathfinder port [port] threshold | 1~99 | [`07`](/en/sensors/line8/reference.md#v07) | reflect threshold for 'on line', 1~99, default 50; set with E4 |
| Pathfinder port [port] calibration count | 0~255 次 | [`08`](/en/sensors/line8/reference.md#v08) | number of calibrations completed since power-on (button calibrations included) |
| Pathfinder port [port] channel [1~8] reflect | 0~100 | [`10~17`](/en/sensors/line8/reference.md#v10) | calibrated reflect of channel 1~8, 0~100 (channel 1 = left); ids 10~17 |

## Command blocks

| Block | Id | Meaning |
| :--- | :--- | :--- |
| Pathfinder port [port] black line mode | [`E0`](/en/sensors/line8/reference.md#ce0) | follow a black line (default) |
| Pathfinder port [port] white line mode | [`E1`](/en/sensors/line8/reference.md#ce1) | follow a white line (F3 b1 = 1) |
| Pathfinder port [port] calibrate black | [`E2`](/en/sensors/line8/reference.md#ce2) | store the current reading as black (sensor over the black line), ~50 ms; saved to EEPROM（F3 bit2 = 1 while running） |
| Pathfinder port [port] calibrate white | [`E3`](/en/sensors/line8/reference.md#ce3) | store the current reading as white (sensor over the white surface), ~50 ms; saved to EEPROM（F3 bit2 = 1 while running） |
| Pathfinder port [port] set threshold [threshold] | [`E4`](/en/sensors/line8/reference.md#ce4) | threshold 1~99 (RAM only, back to 50 after reboot) |
| Pathfinder port [port] 5-second auto calibration | [`E7`](/en/sensors/line8/reference.md#ce7) | sweep the sensor across black line and white surface for 5 s; min/max become the references; F3 b2 = 1 meanwhile（F3 bit2 = 1 while running） |

## Status blocks

The extension exposes each `F3` bit as a boolean block:

| bit | Name | Meaning |
| :--- | :--- | :--- |
| b0 | ready (`ready`) | always 1 |
| b1 | white line mode (`whiteMode`) | 1 = following a white line |
| b2 | calibrating (`calibrating`) | E2/E3/E7 in progress; new calibration commands are ignored |
| b3 | line lost (`lost`) | segment count = 0 |
| b4 | fork (`fork`) | segment count ≥ 2 |

## Packet format (for your own decoder)

<figure class="ref-fig"><img src="/images/figures/common/packet-mblock.svg" alt="mBlock 3-byte packet" loading="lazy"></figure>

| Action | Format |
| :--- | :--- |
| Read one value | 主機送 [編號] → 回 [值低][值高][編號^值低^值高] |
| Write command | 寫入命令 E0~EE：收齊參數後回 [00][00][編號] 再執行；20 ms 內沒收齊就丟棄 |
| Unknown id | [00][80][編號^0x80]（= -32768） |
| Pack all (FD) | Send `[FD]` → reply `[AA][10][16 × int16 LE][xor]`, xor = AA ^ N ^ all data bytes; content = this value table in ascending id order (shared ids and commands excluded). The extension fetches it once and serves blocks from a 10 ms cache. |
| Shared ids | `F0` device ID, `F1` protocol version, `F2` firmware version, `F3` status flags |

<figure class="ref-fig"><img src="/images/figures/common/packet-fd.svg" alt="FD pack-all packet" loading="lazy"></figure>

## Sources and assumptions

Generated by `_build/gen-reference.mjs` from `_build/protocol/line8.json` (single source of truth, transcribed from the 2026-10-21 protocol decisions).

- B-11：細位置 02 與線位置 01 同號（負 = 左）；v1 韌體註解寫反，程式碼同號。
- B-12：E7 五秒動態校準補回（v1 的 0x20=0x01），定案表沒列。
- SPIKE 教育版模式 0 的 byte 8~15 補 0（#65 已改，使用者保留）。
- EV3 模式 4 與模式 2 相同內容；模式 3 改壓線位元圖（#67）。
- mBlock 舊版：MBC_MBLOCK_LEGACY=1 時逐 byte 同 2026-10-21 統一後韌體，0x0B 回 1。
- Block wording follows the v2.0.0 extension generator `build_mext.js`; this page is generated from the same JSON.
