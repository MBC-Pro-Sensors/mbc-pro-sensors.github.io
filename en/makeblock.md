<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# 🤖 MakeBlock (mBot) Block Extension

Read MBC Pathfinder, Ranger and Controller sensors with blocks in mBlock 5 on mBot, mBot Ranger and other Makeblock boards: download one extension file, drop it into mBlock and the blocks are ready.

Software: **mBlock 5 for PC** (Upload mode — the program is flashed to the board).

---

## 📥 Download

<div style="background: rgba(0,210,255,0.05); border: 1px solid rgba(0,210,255,0.3); border-radius: 12px; padding: 22px; margin: 20px 0;">
<p style="margin-top: 0;"><b>MBC Pro Sensors extension v1.0.0</b> (2026-10): 15 blocks for Pathfinder, Ranger and Controller.</p>
<a href="/downloads/MBC_Pro_Sensors_mBlock5_v1.0.0.mext" style="display: inline-block; background: linear-gradient(135deg, #00d2ff 0%, #0077ff 100%); color: white; text-decoration: none; padding: 12px 25px; border-radius: 30px; font-weight: bold;">Download the .mext file</a>
</div>

### Install

1. Open **mBlock 5 for PC** and add your board under "Devices" (for example mBot).
2. **Drag the downloaded `.mext` file into the mBlock window.** Three new categories appear: Pathfinder, Ranger and Controller.
3. Switch the mode to **Upload**, start from the board's start block (for example "when mBot starts up") and click "Upload".

> [!IMPORTANT]
> Use the **MakeBlock edition** of the sensor (different factory firmware) connected through the **mBot socket board** to an RJ25 port. SPIKE, EV3 and I2C editions cannot be plugged into Makeblock boards directly.

> [!NOTE]
> Blocks follow the mBlock language setting (English, Traditional Chinese or Simplified Chinese).

---

## 🧩 Boards and ports

Pick the **port number** on the block; the extension maps it to the right pins for your board. Use these ports:

| Board | Usable ports | Notes |
| :--- | :---: | :--- |
| mBot (mCore) | 1, 2, 3, 4 | All four ports |
| mBot Ranger (Auriga) | 6 – 10 | 1–4 are red motor ports, 5 is serial |
| Orion | 3, 4, 6 | 1–2 are red motor ports; 7–8 are analog-only |
| Ultimate 2.0 (MegaPi) | 6, 7, 8 | Needs the RJ25 shield; 5 is serial |
| MegaPi Pro | 7, 8, 9, 10 | Needs the RJ25 shield |

> [!TIP]
> On a port that cannot be used, reading blocks return **-1** and nothing else is affected. If you read -1, check that the port on the block matches the port you plugged into.

mBot2 (CyberPi) has no RJ25 ports and is not supported.

---

## 📦 Supported products

| Product | Category | What you get |
| :--- | :--- | :--- |
| [Pathfinder 8-Way](/en/sensors/line8/index.md) | 循行者 | Position, line width, each channel's value, on-line, black/white line, calibration |
| [Pathfinder 16-Way](/en/sensors/line16/index.md) | 循行者 | Position, line width, each channel's level (1–9) |
| [Ranger 2-Way](/en/sensors/tof2/index.md) | 測距者 | Left, right and nearest distance (cm) |
| [Ranger 8-Way](/en/sensors/tof8/index.md) | 測距者 | Each of the 8 distances, bearing and distance of the nearest / farthest target |
| [Controller 2.4G (PS2)](/en/sensors/ps2/index.md) | 掌控者 | 16 buttons, both sticks, controller connected (Taiwan only) |
| [Controller PS4/PS5](/en/sensors/ps4/index.md) | 掌控者 | Same blocks as PS2 |

> [!NOTE]
> The MakeBlock editions of Ranger 8-Way and Controller 2.4G (PS2) / PS4/PS5 are new (October 2026). Please [contact us](/en/contact.md) before ordering to confirm the edition.

---

## 🧱 Blocks

| Block | Returns | Use |
| :--- | :--- | :--- |
| Pathfinder [port] follow [black line / white line] | — | Pick the line color to follow |
| Pathfinder [port] calibrate [black line / white line] | — | About 5 s; sweep the sensor over the line and the floor (no readings meanwhile) |
| Pathfinder [port] line position | -8 – 8 | 0 = centered, positive = left, negative = right |
| Pathfinder [port] line position (high resolution) | -100 – 100 | For PD line following |
| Pathfinder [port] line width | number | Channels on the line; jumps at intersections |
| Pathfinder 8 [port] channel [1–8] value | 0–100 | Calibrated reflection |
| Pathfinder 8 [port] channel [1–8] on line | true/false | Value below 50 |
| Pathfinder 16 [port] channel [1–16] level | 1–9 | Brightness level per channel |
| Ranger 2 [port] [left / right / nearer side] distance (cm) | 0–120 | 120 = no target |
| Ranger 8 [port] sensor [1–8] distance (cm) | 0–120 | Sensor 1 is the leftmost |
| Ranger 8 [port] [nearest target direction …] | number | Direction -8 (left) to +8 (right), 0 = straight ahead |
| Controller [port] [button] pressed | true/false | Ⅹ ● ▲ ■, D-pad, L1/R1/L2/R2/L3/R3, SELECT, START |
| Controller [port] [stick axis] | 0–99 | 50 = center, up/right is larger |
| Controller [port] [stick axis] offset | -127–127 | 0 = center, handy as a motor speed |
| Controller [port] connected | true/false | Stop the robot when the controller drops |

---

## 📂 Example downloads

Install the extension first, then open an example with mBlock 5 "File → Open from your computer" (all examples use mBot; for another board, change the start block and the port):

| Example | Product and port | What it does |
| :--- | :--- | :--- |
| [📥 PD line following](/examples/makeblock/MBC_mBot_Line8_PD.mblock) | Pathfinder 8-Way (or 16-Way) → port 1 | PD control on the high-resolution position, base speed 40 |
| [📥 Obstacle avoidance](/examples/makeblock/MBC_mBot_TOF8_Avoid.mblock) | Ranger 8-Way → port 3 | Turns toward the open side when something is within 20 cm |
| [📥 Controller tank drive](/examples/makeblock/MBC_mBot_Pad_TankDrive.mblock) | Controller PS2 / PS4/PS5 → port 4 | Left stick drives the left wheel, right stick the right wheel; stops when the controller drops |

## 💡 Example walkthrough: mBot PD line following

Pathfinder 8-Way on mBot **port 1**:

1. "when mBot starts up" → Pathfinder [port 1] follow [black line].
2. Forever:
   - error = Pathfinder [port 1] line position (high resolution)
   - turn = error × 0.6 + (error − last error) × 2
   - left speed = base − turn, right speed = base + turn
   - last error = error

Start with a base speed of 40. If the robot wobbles, lower 0.6; if it reacts too slowly, raise it.

---

## ❓ No readings?

1. **-1**: wrong port on the block, or the sensor is not the MakeBlock edition.
2. Value never changes: check the socket board is seated and the sensor LED is on.
3. Still stuck: [contact us](/en/contact.md) with your board model and port.
