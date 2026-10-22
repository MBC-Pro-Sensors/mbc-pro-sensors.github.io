<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-reference.mjs 從 _build/protocol/ps2.json 產生（mblock），請勿手改；改內容請改 JSON 後重跑 node _build/gen-reference.mjs -->
# PS2 Gamepad Receiver × mBlock 5 (Makeblock edition)

[🔙 Back to PS2 Gamepad Receiver](/en/sensors/ps2/index.md)

The Makeblock edition of the PS2 Gamepad Receiver plugs into an RJ25 port on mBot, Ranger, Orion or MegaPi and is read with the **MBC Pro Sensors extension v2** blocks. Installation and usable ports per board: [Makeblock extension](/en/makeblock.md).

<p class="chips"><a class="chip chip-spike" href="/en/sensors/ps2/index.md">SPIKE</a><a class="chip chip-ev3" href="/en/sensors/ps2/index.md">EV3</a><a class="chip chip-arduino" href="/en/sensors/ps2/arduino-i2c.md">Arduino I2C</a><a class="chip chip-mblock current" href="/en/sensors/ps2/mblock.md">mBlock</a><a class="chip chip-matrix" href="/en/sensors/ps2/matrix.md">MATRIX</a><a class="chip chip-microbit" href="/en/sensors/ps2/microbit.md">micro:bit</a></p>

## Wiring

<figure class="ref-fig"><img src="/images/figures/ps2/wiring-mbot.svg" alt="PS2 Gamepad Receiver × mBot" loading="lazy"></figure>

Sensor → mBot adapter → RJ25 port on the board (S1 = host TX, S2 = host RX). UART 38400 8N1.

## Reporter blocks

| Block | Returns | Id | Meaning |
| :--- | :--- | :--- | :--- |
| Controller port [port] 16 buttons | 0~65535 bit | [`01`](/en/sensors/ps2/reference.md#v01) | all 16 buttons in one word (PSX order, 1 = pressed): bit0 SELECT, 1 L3, 2 R3, 3 START, 4 Up, 5 Right, 6 Down, 7 Left, 8 L2, 9 R2, 10 L1, 11 R1, 12 Triangle, 13 Circle, 14 Cross, 15 Square; 0 when not connected |
| Controller port [port] d-pad | 0~15 bit | [`02`](/en/sensors/ps2/reference.md#v02) | Up 1, Right 2, Down 4, Left 8 (added when pressed together), 0~15; 0 when not connected |
| Controller port [port] shape buttons | 0~15 bit | [`03`](/en/sensors/ps2/reference.md#v03) | Triangle 1, Circle 2, Cross 4, Square 8 (added when pressed together), 0~15; 0 when not connected; high nibble of 0x22 'R' |
| Controller port [port] shoulder buttons | 0~15 bit | [`04`](/en/sensors/ps2/reference.md#v04) | L2 1, R2 2, L1 4, R1 8 (added when pressed together), 0~15; 0 when not connected |
| Controller port [port] function buttons | 0~15 bit | [`05`](/en/sensors/ps2/reference.md#v05) | SELECT 1, L3 2, R3 4, START 8 (added when pressed together), 0~15; 0 when not connected; high nibble of 0x22 'G' |
| Controller port [port] gamepad connected? | 0~1 | [`06`](/en/sensors/ps2/reference.md#v06) | 1 = gamepad connected (psx.read() succeeds), 0 = not; while disconnected all buttons read 0 and sticks are centred |
| Controller port [port] battery | 0 % | [`07`](/en/sensors/ps2/reference.md#v07) | 0~100; PS2 pads report no battery → always 0 (PS4 only) |
| Controller port [port] L2 analog | 0~255 | [`08`](/en/sensors/ps2/reference.md#v08) | L2 pressure 0~255 (only DualShock 2 in analog mode reports pressure; otherwise pressed = 255, released = 0); 0 when not connected |
| Controller port [port] R2 analog | 0~255 | [`09`](/en/sensors/ps2/reference.md#v09) | R2 pressure 0~255 (same rule as 08); 0 when not connected |
| Controller port [port] system keys | 0 bit | [`0A`](/en/sensors/ps2/reference.md#v0a) | b0 PS button, b1 capture button; PS2 pads have none → always 0 (PS4 only) |
| Controller port [port] gamepad type | 0~2 | [`0B`](/en/sensors/ps2/reference.md#v0b) | 0 none, 1 PS2 digital mode, 2 PS2 analog mode (3 PS4 / 4 PS5 only appear on the PS4 receiver) |
| Controller port [port] stick [LX/LY/RX/RY] | -100~+100 | [`10~13`](/en/sensors/ps2/reference.md#v10) | 10 LX, 11 LY, 12 RX, 13 RY: −100 to +100, 0 = centre, right / up positive (raw 0~255 with ±7 dead zone); 0 when not connected. EV3 / SPIKE / MATRIX carry 0~99 (50 = centre) or 1~9 (5 = centre), see each platform |

## Command blocks

| Block | Id | Meaning |
| :--- | :--- | :--- |
| Controller port [port] rumble [strength] | [`E5`](/en/sensors/ps2/reference.md#ce5) | [strength 0~255], 0 = stop; the PS2 receiver ACKs and ignores it (pad rumble disabled since 250614), effective on PS4 only |
| Controller port [port] light bar colour [red] [green] [blue] | [`E6`](/en/sensors/ps2/reference.md#ce6) | [R][G][B] 0~255 each; PS2 pads have no light bar, the receiver ACKs and ignores it, effective on PS4 only |

## Status blocks

The extension exposes each `F3` bit as a boolean block:

| bit | Name | Meaning |
| :--- | :--- | :--- |
| b0 | ready (`ready`) | always 1 |
| b1 | gamepad connected (`connected`) | same as value 06 |

## Packet format (for your own decoder)

<figure class="ref-fig"><img src="/images/figures/common/packet-mblock.svg" alt="mBlock 3-byte packet" loading="lazy"></figure>

| Action | Format |
| :--- | :--- |
| Read one value | 主機送 [編號] → 回 [值低][值高][編號^值低^值高] |
| Write command | 寫入命令 E0~EE：收齊參數後回 [00][00][編號] 再執行；20 ms 內沒收齊就丟棄 |
| Unknown id | [00][80][編號^0x80]（= -32768） |
| Pack all (FD) | Send `[FD]` → reply `[AA][0F][15 × int16 LE][xor]`, xor = AA ^ N ^ all data bytes; content = this value table in ascending id order (shared ids and commands excluded). The extension fetches it once and serves blocks from a 10 ms cache. |
| Shared ids | `F0` device ID, `F1` protocol version, `F2` firmware version, `F3` status flags |

<figure class="ref-fig"><img src="/images/figures/common/packet-fd.svg" alt="FD pack-all packet" loading="lazy"></figure>

## Sources and assumptions

Generated by `_build/gen-reference.mjs` from `_build/protocol/ps2.json` (single source of truth, transcribed from the 2026-10-21 protocol decisions).

- hw[0] = 1（PS2）、電量 07 恆 0、系統鍵 0A 恆 0、手把類型 0B 只會出現 0／1／2（I2C 定案 §2、§4）。
- 08／09 L2／R2 類比照定案寫 0~255：DS2 類比模式有壓力值；沒有壓力值時按下 = 255、放開 = 0（沿用 v1 0x14 的做法，盤點 .ino:598-603）。
- E5 震動、E6 燈條照 I2C 定案 §3 列在 PS2／PS4 共用命令表；PS2 收到回 ACK 後忽略（跟 v1 0x10／0x23 一樣）。
- 0x22 新增（MATRIX 定案 §6）：1 = 0x43、4 = 方向|形狀<<4、5 = 肩|功能<<4、6~9 = LX LY RX RY 0~99、10 = 已連線、11~12 = 0；大通道新增 [0..3] 四組鍵、[4..7] 搖桿 0~99、[8..9] button16、[10] 已連線、[11] 電量，尾段 [24] F3、[27] 0x42、[28] 1；F1~F4 無動作。ch22 命令 4／5 各掛在 02／04 底下（03／05 在 note 說明高 4 bit），避免同一命令重複登記。
- 搖桿 10~13 在 I2C／mBlock 是 ±100（由 raw (v−128)×100/128、Y 上為正）；EV3／大通道／0x22 維持 0~99（50 中間）、SPIKE 維持 1~9（5 中間）照盤點，不改鎖住模式。
- EV3：模式 0、1、2、4 鎖住（照盤點）；模式 3 改 方向|形狀<<4|已連線<<8（原本沒有 bit8）；模式 5 改成 ID 格式 B（原本 = 按鍵／搖桿合併版）。
- SPIKE：0/5/7/9 鎖住，1、2、4、6 維持不動；模式 3 改 [已連線, 電量(0), 0]（原本 = 4 組按鍵）；模式 8 改 v[0] = ID|hw[0]<<8（原本 = 搖桿）。模式 9 v[6] button16 = PSX 排列（D3）。
- SPIKE 模式 5 只容得下 11 鍵（L2、R2、L3、R3、START 讀不到），鎖住不改。
- Block wording follows the v2.0.0 extension generator `build_mext.js`; this page is generated from the same JSON.
