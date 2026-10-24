<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-reference.mjs 從 _build/protocol/tof8.json 產生（mblock），請勿手改；改內容請改 JSON 後重跑 node _build/gen-reference.mjs -->
<!-- moved: /en/sensors/tof8/mblock/mblock5.md -->
<p class="moved-note">This page has moved into the new guide: <a href="/en/sensors/tof8/mblock/mblock5.md">Ranger 8-Way › mBlock › mBlock 5</a></p>

# Laser Ranger 8-ch × mBlock 5 (Makeblock edition)

[🔙 Back to Laser Ranger 8-ch](/en/sensors/tof8/index.md)

The Makeblock edition of the Laser Ranger 8-ch plugs into an RJ25 port on mBot, Ranger, Orion or MegaPi and is read with the **MBC Pro Sensors extension v2** blocks. Installation and usable ports per board: [Makeblock extension](/en/makeblock.md).

<p class="chips"><a class="chip chip-spike" href="/en/sensors/tof8/index.md">SPIKE</a><a class="chip chip-ev3" href="/en/sensors/tof8/index.md">EV3</a><a class="chip chip-arduino" href="/en/sensors/tof8/arduino-i2c.md">Arduino I2C</a><a class="chip chip-mblock current" href="/en/sensors/tof8/mblock.md">mBlock</a><a class="chip chip-matrix" href="/en/sensors/tof8/matrix.md">MATRIX</a><a class="chip chip-microbit" href="/en/sensors/tof8/microbit.md">micro:bit</a></p>

## Wiring

<figure class="ref-fig"><img src="/images/figures/tof8/wiring-mbot.svg" alt="Laser Ranger 8-ch × mBot" loading="lazy"></figure>

Sensor → mBot adapter → RJ25 port on the board (S1 = host TX, S2 = host RX). UART 38400 8N1.

## Reporter blocks

| Block | Returns | Id | Meaning |
| :--- | :--- | :--- | :--- |
| Ranger 8 port [port] nearest distance cm | 0~120 cm | [`01`](/en/sensors/tof8/reference.md#v01) | smallest of the 8 distances, 0~120 cm (a failed sensor counts as 120); 120 = nothing around |
| Ranger 8 port [port] nearest direction | -8~+8 | [`02`](/en/sensors/tof8/reference.md#v02) | direction of the nearest object: −7 (left) to +7 (right), 0 = straight ahead, negative = left; ±8 = nothing around (keeps the last side) |
| Ranger 8 port [port] farthest distance cm | 0~120 cm | [`03`](/en/sensors/tof8/reference.md#v03) | largest of the 8 distances, 0~120 cm (a failed sensor always reads 120 and looks 'farthest') |
| Ranger 8 port [port] farthest direction | -7~+7 | [`04`](/en/sensors/tof8/reference.md#v04) | direction of the most open side: −7 to +7, 0 = straight ahead, negative = left; 0 when all 8 read 120 |
| Ranger 8 port [port] front distance cm | 0~120 cm | [`05`](/en/sensors/tof8/reference.md#v05) | average of sensors 4 and 5 (both facing forward), 0~120 cm; EV3 mode 3 and SPIKE mode 6 carry the same thing in mm; 0x22 'C' is min(4, 5), not the average (see 10) |
| Ranger 8 port [port] nearest distance mm | 0~1200 mm | [`06`](/en/sensors/tof8/reference.md#v06) | smallest of the 8 mm readings, 0~1200 (failed sensor counts as 1200); 1200 = nothing around |
| Ranger 8 port [port] init bitmap | 0~255 bit | [`07`](/en/sensors/tof8/reference.md#v07) | bit0 = sensor 1 (left) … bit7 = sensor 8; 1 = initialised at power-on (8 sensors take ~2~4 s, no retry); 255 = all OK |
| Ranger 8 port [port] fault bitmap | 0~255 bit | [`08`](/en/sensors/tof8/reference.md#v08) | bit0 = sensor 1 … bit7 = sensor 8; 1 = currently failing (I2C error or no update for 200 ms); its distance reads 120 / 1200 |
| Ranger 8 port [port] front distance mm | 0~1200 mm | [`09`](/en/sensors/tof8/reference.md#v09) | average of sensors 4 and 5 (the two facing forward), 0~1200 mm; 1200 = nothing ahead. cm version is 05 |
| Ranger 8 port [port] sensor [1~8] distance cm | 0~120 cm | [`10~17`](/en/sensors/tof8/reference.md#v10) | distance of each sensor 1~8, 0~120 cm (1 = far left, 4 and 5 = front, 8 = far right, 30° apart); 120 = nothing that way / sensor failed; ids 10~17 |
| Ranger 8 port [port] sensor [1~8] distance mm | 0~1200 mm | [`18~1F`](/en/sensors/tof8/reference.md#v18) | distance of each sensor 1~8, 0~1200 mm; 1200 = nothing that way / sensor failed; ids 18~1F |

## Status blocks

The extension exposes each `F3` bit as a boolean block:

| bit | Name | Meaning |
| :--- | :--- | :--- |
| b0 | ready (`ready`) | 1 once all 8 sensors are initialised (~2~4 s after power-on) and snapshots are running |
| b1 | any sensor failed (`fault`) | 1 if any sensor failed to initialise, has an I2C error or no update for 200 ms (08 ≠ 0 or 07 ≠ 255) |

## Packet format (for your own decoder)

<figure class="ref-fig"><img src="/images/figures/common/packet-mblock.svg" alt="mBlock 3-byte packet" loading="lazy"></figure>

| Action | Format |
| :--- | :--- |
| Read one value | 主機送 [編號] → 回 [值低][值高][編號^值低^值高] |
| Write command | 寫入命令 E0~EE：收齊參數後回 [00][00][編號] 再執行；20 ms 內沒收齊就丟棄 |
| Unknown id | [00][80][編號^0x80]（= -32768） |
| Pack all (FD) | Send `[FD]` → reply `[AA][19][25 × int16 LE][xor]`, xor = AA ^ N ^ all data bytes; content = this value table in ascending id order (shared ids and commands excluded). The extension fetches it once and serves blocks from a 10 ms cache. |
| Shared ids | `F0` device ID, `F1` protocol version, `F2` firmware version, `F3` status flags |

<figure class="ref-fig"><img src="/images/figures/common/packet-fd.svg" alt="FD pack-all packet" loading="lazy"></figure>

## Sources and assumptions

Generated by `_build/gen-reference.mjs` from `_build/protocol/tof8.json` (single source of truth, transcribed from the 2026-10-21 protocol decisions).

- 正前方 mm（EV3 模式 3、SPIKE 模式 6 v[1]）在定案 §5 沒有獨立 I2C 編號 → 掛在 05 正前方 cm 底下，用 scale:mm 標示，不自己加編號。
- 0x22（使用者 2026-10-21 最終定案）：只用一個位址，R~K 由左到右 7 個 cm：第 1、2、3 顆、min(第 4, 第 5)、第 6、7、8 顆；11、12 不放資料；最近距離／方向由主機自己比較。此排法只用在 0x22，EV3／SPIKE不動。
- SPIKE：0/5/7/9 鎖住，1、2、3、4、8 維持不動；模式 6 改 [最近 mm, 正前方 mm, 最近方向 −8~+8]（原本 = 第 2~7 顆兩兩合併）；模式 8 v[0] 改 ID|hw[0]<<8。
- 最空曠方向 04 範圍 −7~+7（maxDirection 永遠不會是 0／16，盤點 INO:1086-1088）；最近方向 02 ±8 = 沒目標、沿用上次偏向側，開機預設 −8。
- 沒有寫入命令；F8 換位址／EF 重開機用共用的。
- 主對話 2026-10-21：補 09 正前方 mm（定案 §5 只有 05 正前方 cm，但 EV3 模式 3、SPIKE 模式 6 都送 mm，I2C／mBlock 應有同一個值）。
- Block wording follows the v2.0.0 extension generator `build_mext.js`; this page is generated from the same JSON.
