<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- Code blocks = original text of examples/unicam/matrix/*/*.ino (decode helper master: 上位機函式庫\matrix\MBC_UniCam_MATRIX.h); decode table from spec section 5 (spec_layout.json host mx) -->
# 🟩 MATRIX examples: Sharpshooter Vision Universal v1 × MATRIX Mini R4

[🔙 Back to Sharpshooter Vision Universal v1](/en/sensors/unicam/index.md)　｜　[MATRIX mVision reading guide](/en/sensors/unicam/matrix/mvision.md)　｜　[📋 Value reference (MATRIX)](/en/sensors/unicam/reference.md#host-mx)

> [!WARNING]
> **v1 not yet hardware-verified**: the value layout is final and passed computer tests; real-hardware numbers (fps, line thresholds, I2C pins) may change after bench testing.

To a MATRIX Mini R4, Sharpshooter Vision Universal v1 (MATRIX firmware) is an **mVision camera**: every **20 ms** it sends a packet of 10 cells over **Serial1 at 115200**. The MATRIXblock "mVision" blocks read it — no library to install, no I2C.

This page has two parts:
1. **MATRIXblock blocks only**: the "how to split the cells" tables below tell you what each "mVision data 0~9" holds in each mode and how to calculate it.
2. **Two C++ examples** (follow a colour blob, line-following car), uploaded with the **Arduino IDE**.

> [!NOTE]
> - The MATRIXblock code window is **read-only** (you can see the code made from blocks but cannot paste C++), so the C++ examples use the Arduino IDE: Boards Manager "Arduino UNO R4 Boards", Library Manager "**MatrixMiniR4**", board **Arduino UNO R4 WiFi**. The examples compile on a computer with arduino-cli (UNO R4 WiFi + MatrixMiniR4 1.2.2) but **have not been run on a robot yet**.
> - MATRIX cannot read the camera's current mode and has no mode command: **always choose the mode in the camera menu** and make `MODE` in the program match it.

## Values used on this page

- **Mode number** = camera mode × 10 + option (e.g. 13 = colour tracking ③ standard, 41 = line following ① black line).
- **Coordinates**: pixels, X 0~159 (left → right), Y 0~119 (top → bottom); **0 = not found**.
- **Hue code** 0~255: 0~240 hue (red 0/240, yellow 40, green 80, cyan 120, blue 160, purple 200); 241~248 white/grey; 249~255 black.
- **Values that can be negative** (line offsets, angle, edges) are **sent +100**; subtract 100. Offset: standing behind the car looking forward, **line on the left = negative**.

## How to split the cells (mVision data 0~9)

Every cell is a whole number 0~65535. Most cells hold two values:

- **low** = the remainder of data ÷ 256 (MATRIXblock "remainder" block)
- **high** = data ÷ 256 **rounded down**
- Mode 11 is different: one cell holds two colours as "X cell×10 + Y cell": **first colour = data ÷ 100 rounded down**, **second colour = remainder of data ÷ 100**. Split "X cell×10 + Y cell" again: X cell = ÷10 rounded down (1~8), Y cell = remainder of ÷10 (1~6, top = small); pixels ≈ cell × 20 − 10.

**Camera mode 1 colour tracking**

| Mode | Data 0 | Data 1 | Data 2 | Data 3~8 | Data 9 |
|---|---|---|---|---|---|
| 11 ① max (16 colours) | colour 1 (÷100), colour 2 (remainder) | colours 3, 4 | colours 5, 6 | data n = colours 2n+1, 2n+2 (colour 16 in data 7); **data 8 = colours found** | unused (0) |
| 12 ② multi (10 colours) | low = colour 1 X, high = colour 1 Y | colour 2 X, Y | colour 3 X, Y | data n = colour n+1 X, Y | colour 10 X, Y |
| 13 ③ standard (5 colours) | low = colour 1 X, high = colour 1 Y | low = colour 1 area %, high = colour 1 count | colour 2 X, Y | two cells per colour: position, then area + count | colour 5 area, count |
| 14 ④ fine (3 colours) | low = colour 1 X, high = colour 1 Y | low = colour 1 width, high = height (pixels) | low = colour 1 area %, high = count | data 3~5 = colour 2, data 6~8 = colour 3 (same three cells) | colours found (no split) |

**Camera mode 2 spot colour**

| Mode | How to split |
|---|---|
| 21 ① hue (20 spots) | data n: low = hue code of S(2n+1), high = S(2n+2) (data 0 = S1, S2 … data 9 = S19, S20) |
| 22 ② hue + saturation (10 spots) | data n: low = hue code of S(n+1), high = its saturation |
| 23 ③ hue + saturation + value (6 spots) | split the 10 cells into 20 bytes in order (data 0 low, data 0 high, data 1 low …), 3 per spot: S1 hue, sat, val, S2 hue …; data 0 = S1 hue + sat, data 1 = S1 val + S2 hue, data 2 = S2 sat + val, then repeat every 3 cells; data 9 unused |

**Camera mode 3 colour order** (mode 30, up to 18 blocks)

| Data 0~8 | Data 9 |
|---|---|
| data n: low = hue code of block 2n+1 from the left, high = block 2n+2 | **total blocks** (no split) |

**Camera mode 4 line following** (modes 41/42/43 are the same)

| Data | Content | How |
|---|---|---|
| 0 | near offset | **subtract 100** (−100~100) |
| 1 | middle offset | subtract 100 |
| 2 | far offset | subtract 100 |
| 3 | line angle | subtract 100 (−90~90) |
| 4 | junction 0 none 1 cross 2 T 3 left 4 right 5 dead end | no split |
| 5 | line flags (near 1 + mid 2 + far 4; 0 = no line) | no split |
| 6 | low = near line width %, high = junction distance % | ÷256 |
| 7 | low = left edge, high = right edge | ÷256, then **subtract 100** from each |
| 8 | low = far line width %, high = colour marker line crossed (target colour 1–16, 0 = none yet) | ÷256 |
| 9 | low = marker lines counted, high = lost side 0 on line 1 left 2 right | ÷256 |

> [!TIP]
> Example: colour tracking ③ standard, is colour 1 in the left half? → "remainder of mVision data 0 ÷ 256" is colour 1 X; `X > 0 and X < 80` is the left half. Line following: "mVision data 0 − 100" is the near offset.

---

## Example 1: follow the colour blob

**What it does**: colour 1 (the first colour taught to the camera) on the left → the car turns left; on the right → turns right; in the middle → drives forward. Big enough (close) or not seen → stop. The OLED shows X and the area.

**You need**
- MATRIX Mini R4 + a car with two DC motors (left M1, right M2) + battery
- Sharpshooter Vision Universal v1 (MATRIX firmware)
- A brightly coloured ball or block

**Camera setup**: teach "target 1"; camera mode **1 colour tracking · option ③ standard** (mode number 13).

**Wiring**: camera to the Mini R4 UART port used by mVision; left motor M1, right motor M2.

**Download**: [unicam_mx_01_track_turn.ino](/examples/unicam/matrix/unicam_mx_01_track_turn/unicam_mx_01_track_turn.ino) (put it in a folder with the same name, open it in the Arduino IDE)

**Program**

```cpp
/*
 * 神攝手通用版 v1 × MATRIX Mini R4　範例 1：跟著色塊轉
 * Sharpshooter Vision Universal v1 × MATRIX Mini R4  example 1: follow the colour blob
 * ------------------------------------------------------------
 * ★ 需要把相機選單切到 相機模式 1 顏色追蹤 ・子選項 ③ 標準（模式編號 13）
 *   Set the camera menu to camera mode 1 colour tracking · option ③ standard (mode number 13)
 *   （MATRIX 走 mVision 串口，沒有切模式的命令，一定要在相機選單切 / mVision has no mode command: use the camera menu）
 *
 * 這個範例會做什麼 / What it does：
 *   色 1（相機教的第 1 個顏色）在左邊 → 車子往左轉；在右邊 → 往右轉；在中間 → 往前開。
 *   面積夠大（夠近）→ 停車；沒看到 → 停車。OLED 顯示 X 和面積。
 *   Colour 1 on the left → turn left; on the right → turn right; in the middle → drive forward.
 *   Big enough (close) → stop; not seen → stop. The OLED shows X and the area.
 *
 * 接線 Wiring：相機 → MATRIX Mini R4 的 UART 孔（mVision 用 Serial1 115200）；左輪馬達 M1、右輪馬達 M2。
 *   camera → the UART port of MATRIX Mini R4 (mVision uses Serial1 115200); left motor M1, right motor M2.
 *
 * 怎麼上傳 / How to upload：Arduino IDE → 開發板管理員裝「Arduino UNO R4 Boards」→ 函式庫管理員裝「MatrixMiniR4」
 *   → 開發板選 Arduino UNO R4 WiFi → 上傳。（MATRIXblock 的程式碼窗是唯讀的，不能貼 C++ 程式）
 *   Arduino IDE → Boards Manager "Arduino UNO R4 Boards" → Library Manager "MatrixMiniR4" → board "Arduino UNO R4 WiFi" → Upload.
 *
 * ⚠ v1 尚未上機驗證 / v1 not yet hardware-verified：門檻、速度是建議值，請依你的車子調。
 */

#include <MatrixMiniR4.h>

// ==== MBC_UNICAM_MX_TOP_BEGIN ====
unsigned int CamData[10];                // mVision 的 10 格資料（跟 MATRIXblock 產生的程式同名）
int CamStatus;                           // 讀到幾格（> 0 = 讀到）
// 一個色塊：x、y 像素（0 = 沒找到）、w、h 像素、count 塊數、area 面積 %；這個模式沒給的值是 -1
struct UcBlob { int x, y, w, h, count, area; };
// 一個取樣點：h 色相碼 0~255、s 飽和度、v 明度 0~100；沒給的是 -1
struct UcSpot { int h, s, v; };
// 循線 14 個值（近／中／遠偏移 -100~100：線在左 = 負）
struct UcLine { int nearOff, midOff, farOff, angle, width, flags, junction, junctionDist,
                leftEdge, rightEdge, farWidth, marker, markerCount, lost; };
// ==== MBC_UNICAM_MX_TOP_END ====

const uint8_t MODE = 13;                 // 模式編號 = 相機模式 1 × 10 + 子選項 3 / mode number
const int BASE = 30;                     // 👉 可以改：往前的力道 0~100 / forward power
const int STOP_AREA = 25;                // 👉 可以改：面積 ≥ 25 % 就停（夠近了）/ stop when area >= 25 %

void drive(int left, int right) {        // 左輪 M1、右輪 M2，-100~100 / left M1, right M2
  MiniR4.M1.setPower(left);
  MiniR4.M2.setPower(right);
}

void setup() {
  MiniR4.begin();
  MiniR4.PWR.setBattCell(2);             // 2 顆鋰電池 / 2-cell battery
  MiniR4.M1.setReverse(false);           // 👉 某一輪往後轉就改成 true / set true if a wheel turns backwards
  MiniR4.M2.setReverse(true);            // 右輪通常裝反向 / the right motor is usually mounted mirrored
  MiniR4.Vision.Begin();                 // 開始收 mVision 資料（Serial1 115200）/ start mVision
  drive(0, 0);
}

void loop() {
  CamStatus = MiniR4.Vision.SmartCamReader(CamData);   // 讀一包 10 格 / read one 10-cell packet
  if (CamStatus > 0) {
    UcBlob b;
    ucBlob(MODE, 1, b);                  // 色 1：x、y 像素（0 = 沒找到）、area 面積 % / colour 1
    MiniR4.OLED.clearDisplay();
    MiniR4.OLED.setTextSize(2);
    MiniR4.OLED.setTextColor(SSD1306_WHITE);
    MiniR4.OLED.setCursor(0, 0);
    MiniR4.OLED.print("X ");
    MiniR4.OLED.print(b.x);
    MiniR4.OLED.setCursor(0, 20);
    MiniR4.OLED.print("A ");
    MiniR4.OLED.print(b.area);
    MiniR4.OLED.display();
    if (b.x == 0) {                      // 沒找到 → 停 / not found → stop
      drive(0, 0);
    } else if (b.area >= STOP_AREA) {    // 夠近 → 停 / close enough → stop
      drive(0, 0);
    } else {
      int turn = (b.x - 80) / 2;         // 偏右 → 正 → 左輪快 → 右轉 / right of centre → turn right
      drive(BASE + turn, BASE - turn);
    }
  } else {
    drive(0, 0);                         // 收不到相機資料：安全第一，停車 / no camera data: stop
  }
}

// ==== MBC_UNICAM_MX_HELPERS_BEGIN ====
// ============================================================
// 以下是「神攝手通用版 × mVision 解碼小幫手」，所有 MATRIX 範例都一樣，不用改。
// 每格 = 低 byte ＋ 高 byte × 256（模式 11 是 A × 100 ＋ B）
// ============================================================

// 第 i 格（1~10）的低 byte、高 byte
int ucLo(uint8_t i) { return (int)(CamData[i - 1] & 255); }
int ucHi(uint8_t i) { return (int)((CamData[i - 1] >> 8) & 255); }

// 第 k 個 byte（0~19）：10 格依序拆成 20 個 byte（模式 23 用）
int ucByte(uint8_t k) { return (k & 1) ? ucHi(k / 2 + 1) : ucLo(k / 2 + 1); }

// 這個模式最多幾色：11 → 16、12 → 10、13 → 5、14 → 3
uint8_t ucMaxColor(uint8_t mode) {
  if (mode == 11) return 16;
  if (mode == 12) return 10;
  if (mode == 13) return 5;
  if (mode == 14) return 3;
  return 0;
}

// 讀色 color 的色塊（模式 11~14）；模式 11 的位置是 8×6 格的格子中心（±10 像素）
bool ucBlob(uint8_t mode, uint8_t color, UcBlob &b) {
  if (color < 1 || color > ucMaxColor(mode)) return false;
  b.w = -1; b.h = -1; b.count = -1; b.area = -1;
  if (mode == 11) {                      // 一格兩色：(X格×10＋Y格)×100 ＋ (X格×10＋Y格)
    unsigned int c = CamData[(color - 1) / 2];
    int g = (color & 1) ? (int)(c / 100) : (int)(c % 100);
    if (g == 0) { b.x = 0; b.y = 0; return true; }
    b.x = (g / 10 - 1) * 20 + 10;        // 8×6 格 → 像素（格子中心）
    b.y = (g % 10 - 1) * 20 + 10;        // Y 上 = 小
    return true;
  }
  uint8_t base = (mode == 12) ? color : (mode == 13) ? 2 * color - 1 : 3 * color - 2;
  b.x = ucLo(base); b.y = ucHi(base);
  if (mode == 13) { b.area = ucLo(base + 1); b.count = ucHi(base + 1); }
  if (mode == 14) { b.w = ucLo(base + 1); b.h = ucHi(base + 1); b.area = ucLo(base + 2); b.count = ucHi(base + 2); }
  return true;
}

// 16 個目標色裡找到幾色（模式 11 第 9 格、模式 14 第 10 格）；其他模式回傳 -1
int ucColorsFound(uint8_t mode) {
  if (mode == 11) return (int)CamData[8];
  if (mode == 14) return (int)CamData[9];
  return -1;
}

// 這個模式最多幾點：21 → 20、22 → 10、23 → 6
uint8_t ucMaxPoint(uint8_t mode) {
  if (mode == 21) return 20;
  if (mode == 22) return 10;
  if (mode == 23) return 6;
  return 0;
}

// 讀取樣點 point（模式 21~23）
bool ucSpot(uint8_t mode, uint8_t point, UcSpot &p) {
  if (point < 1 || point > ucMaxPoint(mode)) return false;
  p.s = -1; p.v = -1;
  if (mode == 21) { p.h = ucByte(point - 1); return true; }          // 一格兩點色相
  if (mode == 22) { p.h = ucLo(point); p.s = ucHi(point); return true; }
  uint8_t k = 3 * (point - 1);                                        // 23：色相 飽和 明度 依序排
  p.h = ucByte(k); p.s = ucByte(k + 1); p.v = ucByte(k + 2);
  return true;
}

// 照順序給色號（模式 30）：hues 放由左到右的色相碼（最多 18 塊），回傳放了幾個；total = 總塊數
uint8_t ucOrder(int *hues, uint8_t maxN, int &total) {
  total = (int)CamData[9];
  uint8_t n = (total < 18) ? (uint8_t)total : 18;
  if (n > maxN) n = maxN;
  for (uint8_t i = 0; i < n; i++) hues[i] = ucByte(i);
  return n;
}

// 循線（模式 41~43）：10 格拆成 14 個值（＋100 的值減回來）
void ucLine(UcLine &l) {
  l.nearOff = (int)CamData[0] - 100; l.midOff = (int)CamData[1] - 100;
  l.farOff = (int)CamData[2] - 100;  l.angle = (int)CamData[3] - 100;
  l.junction = (int)CamData[4];      l.flags = (int)CamData[5];
  l.width = ucLo(7);                 l.junctionDist = ucHi(7);
  l.leftEdge = ucLo(8) - 100;        l.rightEdge = ucHi(8) - 100;
  l.farWidth = ucLo(9);              l.marker = ucHi(9);
  l.markerCount = ucLo(10);          l.lost = ucHi(10);
}

// 色相碼 → 顏色代號：1 紅 2 橙 3 黃 4 綠 5 青 6 藍 7 紫 8 白／灰 9 黑
uint8_t ucColorCode(int h) {
  if (h > 248) return 9;
  if (h > 240) return 8;
  if (h < 12 || h >= 220) return 1;
  if (h < 28) return 2;
  if (h < 56) return 3;
  if (h < 100) return 4;
  if (h < 140) return 5;
  if (h < 180) return 6;
  return 7;
}
// ==== MBC_UNICAM_MX_HELPERS_END ====
```

**Step by step**
1. The "TOP" section at the top declares `CamData[10]` (the 10 mVision cells) and boxes such as `UcBlob`; the "helpers" section at the bottom splits the cells — no need to change it.
2. `MiniR4.Vision.Begin()`: start receiving mVision (Serial1 115200).
3. Each loop `MiniR4.Vision.SmartCamReader(CamData)` reads one packet; only `CamStatus > 0` counts as received, otherwise the car stops.
4. `ucBlob(MODE, 1, b)`: splits data 0 and 1 into colour 1 X, Y, area and count.
5. `x == 0` → not seen → stop; `area ≥ 25` → close → stop; otherwise `turn = (x − 80) ÷ 2` and `drive(BASE + turn, BASE − turn)`: blob on the right → left wheel faster → turn right.

**What you should see**: put the ball in front of the car, a bit to the left: the car drives and turns left to face it, and stops when it is very close. OLED line 1 X (small = left), line 2 A (area %).

**Common problems**
- **The car drives backwards / spins?** → One wheel is reversed: flip that wheel's `setReverse` in `setup()` (true/false).
- **The OLED never changes?** → No camera data: check the host name at the top left of the camera screen is green, the UART cable is in the right port and the camera has the MATRIX firmware.
- **Stops while still far?** → Increase `STOP_AREA`; for a small ball decrease it.

---

## Example 2: line-following car (PD + stop at a junction)

**What it does**: follows the line, steering left when the line is to the left and right when it is to the right (near + far offsets); stops at a **cross or T junction**; with no line in view it spins toward the side where it last saw the line. The OLED shows the near offset.

**You need**: the car of example 1 + a black line on white (about 2 cm wide) with a cross junction.

**Camera setup**: camera on the front, looking forward and down; camera mode **4 line following · option ① black line** (mode number 41; ② white and ③ colour line need no program change).

**Wiring**: same as example 1.

**Download**: [unicam_mx_02_line_follow.ino](/examples/unicam/matrix/unicam_mx_02_line_follow/unicam_mx_02_line_follow.ino)

**Program**

```cpp
/*
 * 神攝手通用版 v1 × MATRIX Mini R4　範例 2：循線小車（近／遠偏移 PD 循線，遇到路口停車）
 * Sharpshooter Vision Universal v1 × MATRIX Mini R4  example 2: line follower (near/far offset PD, stop at a junction)
 * ------------------------------------------------------------
 * ★ 需要把相機選單切到 相機模式 4 循線 ・子選項 ① 黑線（模式編號 41；白線 ②、色線 ③ 也能用）
 *   Set the camera menu to camera mode 4 line following · option ① black line (mode number 41; ② white, ③ colour also work)
 *
 * 這個範例會做什麼 / What it does：
 *   沿著線走：線偏左就往左修、偏右就往右修（近處＋遠處偏移一起看）；十字或 T 字路口 → 停車；
 *   看不到線 → 往最後看到線的那一邊原地轉。OLED 顯示近處偏移。
 *   Follow the line (near + far offsets); cross or T junction → stop; no line → spin toward the side it was last seen.
 *   The OLED shows the near offset.
 *
 * 接線 Wiring：相機 → UART 孔（mVision，Serial1 115200）；左輪 M1、右輪 M2。相機裝車頭、斜斜往前下方看地面。
 *   camera → UART port (mVision, Serial1 115200); left motor M1, right motor M2; camera on the front looking down-forward.
 * 方向 Direction：站在車子後面往前看，線在左 = 負、線在右 = 正 / behind the car looking forward: line on the left = negative.
 *
 * 怎麼上傳 / How to upload：Arduino IDE＋「Arduino UNO R4 Boards」＋函式庫「MatrixMiniR4」，開發板選 Arduino UNO R4 WiFi。
 *
 * ⚠ v1 尚未上機驗證 / v1 not yet hardware-verified：BASE、KP、KD 是建議值，請依你的車子調 / tune for your car.
 */

#include <MatrixMiniR4.h>

// ==== MBC_UNICAM_MX_TOP_BEGIN ====
unsigned int CamData[10];                // mVision 的 10 格資料（跟 MATRIXblock 產生的程式同名）
int CamStatus;                           // 讀到幾格（> 0 = 讀到）
// 一個色塊：x、y 像素（0 = 沒找到）、w、h 像素、count 塊數、area 面積 %；這個模式沒給的值是 -1
struct UcBlob { int x, y, w, h, count, area; };
// 一個取樣點：h 色相碼 0~255、s 飽和度、v 明度 0~100；沒給的是 -1
struct UcSpot { int h, s, v; };
// 循線 14 個值（近／中／遠偏移 -100~100：線在左 = 負）
struct UcLine { int nearOff, midOff, farOff, angle, width, flags, junction, junctionDist,
                leftEdge, rightEdge, farWidth, marker, markerCount, lost; };
// ==== MBC_UNICAM_MX_TOP_END ====

const int BASE = 35;                     // 👉 可以改：直走力道 / straight power
const float KP = 0.5;                    // 👉 可以改：偏多少就轉多少（P）/ proportional
const float KD = 0.3;                    // 👉 可以改：偏移變化快就多修一點（D）/ derivative
float lastErr = 0;
bool stopped = false;                    // 停在路口後就不再走（按 RESET 重來）/ stays stopped after a junction

void drive(int left, int right) {        // 左輪 M1、右輪 M2，-100~100
  MiniR4.M1.setPower(constrain(left, -100, 100));
  MiniR4.M2.setPower(constrain(right, -100, 100));
}

void setup() {
  MiniR4.begin();
  MiniR4.PWR.setBattCell(2);             // 2 顆鋰電池 / 2-cell battery
  MiniR4.M1.setReverse(false);           // 👉 某一輪往後轉就改成 true / set true if a wheel turns backwards
  MiniR4.M2.setReverse(true);
  MiniR4.Vision.Begin();                 // mVision（Serial1 115200）
  drive(0, 0);
}

void loop() {
  CamStatus = MiniR4.Vision.SmartCamReader(CamData);   // 讀一包 10 格 / read one packet
  if (CamStatus > 0 && !stopped) {
    UcLine l;
    ucLine(l);                           // 10 格拆成 14 個循線值 / 10 cells → 14 line values
    MiniR4.OLED.clearDisplay();
    MiniR4.OLED.setTextSize(2);
    MiniR4.OLED.setTextColor(SSD1306_WHITE);
    MiniR4.OLED.setCursor(0, 0);
    MiniR4.OLED.print("N ");
    MiniR4.OLED.print(l.nearOff);        // 近處偏移 -100~100 / near offset
    MiniR4.OLED.display();
    if (l.junction == 1 || l.junction == 2) {   // 1 十字、2 T 字 → 停車 / cross or T → stop
      drive(0, 0);
      stopped = true;
    } else if (l.flags == 0) {           // 近中遠都沒看到線 / no line at all
      if (l.lost == 1) drive(-30, 30);   // 最後在左邊 → 原地左轉 / last seen left → spin left
      else drive(30, -30);               // 最後在右邊 → 原地右轉 / otherwise spin right
    } else {
      float err = l.nearOff * 0.7 + l.farOff * 0.3;    // 近 7 成、遠 3 成 / near 70 %, far 30 %
      float turn = KP * err + KD * (err - lastErr);    // PD
      lastErr = err;
      drive(BASE + (int)turn, BASE - (int)turn);       // 線在左（負）→ 左輪慢 → 往左修 / line left → steer left
    }
  } else {
    drive(0, 0);                         // 收不到資料或已停在路口：停車 / no data or stopped: stop
  }
}

// ==== MBC_UNICAM_MX_HELPERS_BEGIN ====
// ============================================================
// 以下是「神攝手通用版 × mVision 解碼小幫手」，所有 MATRIX 範例都一樣，不用改。
// 每格 = 低 byte ＋ 高 byte × 256（模式 11 是 A × 100 ＋ B）
// ============================================================

// 第 i 格（1~10）的低 byte、高 byte
int ucLo(uint8_t i) { return (int)(CamData[i - 1] & 255); }
int ucHi(uint8_t i) { return (int)((CamData[i - 1] >> 8) & 255); }

// 第 k 個 byte（0~19）：10 格依序拆成 20 個 byte（模式 23 用）
int ucByte(uint8_t k) { return (k & 1) ? ucHi(k / 2 + 1) : ucLo(k / 2 + 1); }

// 這個模式最多幾色：11 → 16、12 → 10、13 → 5、14 → 3
uint8_t ucMaxColor(uint8_t mode) {
  if (mode == 11) return 16;
  if (mode == 12) return 10;
  if (mode == 13) return 5;
  if (mode == 14) return 3;
  return 0;
}

// 讀色 color 的色塊（模式 11~14）；模式 11 的位置是 8×6 格的格子中心（±10 像素）
bool ucBlob(uint8_t mode, uint8_t color, UcBlob &b) {
  if (color < 1 || color > ucMaxColor(mode)) return false;
  b.w = -1; b.h = -1; b.count = -1; b.area = -1;
  if (mode == 11) {                      // 一格兩色：(X格×10＋Y格)×100 ＋ (X格×10＋Y格)
    unsigned int c = CamData[(color - 1) / 2];
    int g = (color & 1) ? (int)(c / 100) : (int)(c % 100);
    if (g == 0) { b.x = 0; b.y = 0; return true; }
    b.x = (g / 10 - 1) * 20 + 10;        // 8×6 格 → 像素（格子中心）
    b.y = (g % 10 - 1) * 20 + 10;        // Y 上 = 小
    return true;
  }
  uint8_t base = (mode == 12) ? color : (mode == 13) ? 2 * color - 1 : 3 * color - 2;
  b.x = ucLo(base); b.y = ucHi(base);
  if (mode == 13) { b.area = ucLo(base + 1); b.count = ucHi(base + 1); }
  if (mode == 14) { b.w = ucLo(base + 1); b.h = ucHi(base + 1); b.area = ucLo(base + 2); b.count = ucHi(base + 2); }
  return true;
}

// 16 個目標色裡找到幾色（模式 11 第 9 格、模式 14 第 10 格）；其他模式回傳 -1
int ucColorsFound(uint8_t mode) {
  if (mode == 11) return (int)CamData[8];
  if (mode == 14) return (int)CamData[9];
  return -1;
}

// 這個模式最多幾點：21 → 20、22 → 10、23 → 6
uint8_t ucMaxPoint(uint8_t mode) {
  if (mode == 21) return 20;
  if (mode == 22) return 10;
  if (mode == 23) return 6;
  return 0;
}

// 讀取樣點 point（模式 21~23）
bool ucSpot(uint8_t mode, uint8_t point, UcSpot &p) {
  if (point < 1 || point > ucMaxPoint(mode)) return false;
  p.s = -1; p.v = -1;
  if (mode == 21) { p.h = ucByte(point - 1); return true; }          // 一格兩點色相
  if (mode == 22) { p.h = ucLo(point); p.s = ucHi(point); return true; }
  uint8_t k = 3 * (point - 1);                                        // 23：色相 飽和 明度 依序排
  p.h = ucByte(k); p.s = ucByte(k + 1); p.v = ucByte(k + 2);
  return true;
}

// 照順序給色號（模式 30）：hues 放由左到右的色相碼（最多 18 塊），回傳放了幾個；total = 總塊數
uint8_t ucOrder(int *hues, uint8_t maxN, int &total) {
  total = (int)CamData[9];
  uint8_t n = (total < 18) ? (uint8_t)total : 18;
  if (n > maxN) n = maxN;
  for (uint8_t i = 0; i < n; i++) hues[i] = ucByte(i);
  return n;
}

// 循線（模式 41~43）：10 格拆成 14 個值（＋100 的值減回來）
void ucLine(UcLine &l) {
  l.nearOff = (int)CamData[0] - 100; l.midOff = (int)CamData[1] - 100;
  l.farOff = (int)CamData[2] - 100;  l.angle = (int)CamData[3] - 100;
  l.junction = (int)CamData[4];      l.flags = (int)CamData[5];
  l.width = ucLo(7);                 l.junctionDist = ucHi(7);
  l.leftEdge = ucLo(8) - 100;        l.rightEdge = ucHi(8) - 100;
  l.farWidth = ucLo(9);              l.marker = ucHi(9);
  l.markerCount = ucLo(10);          l.lost = ucHi(10);
}

// 色相碼 → 顏色代號：1 紅 2 橙 3 黃 4 綠 5 青 6 藍 7 紫 8 白／灰 9 黑
uint8_t ucColorCode(int h) {
  if (h > 248) return 9;
  if (h > 240) return 8;
  if (h < 12 || h >= 220) return 1;
  if (h < 28) return 2;
  if (h < 56) return 3;
  if (h < 100) return 4;
  if (h < 140) return 5;
  if (h < 180) return 6;
  return 7;
}
// ==== MBC_UNICAM_MX_HELPERS_END ====
```

**Step by step**
1. `ucLine(l)`: splits the 10 cells into 14 line values (the +100 ones are already corrected).
2. `l.junction` is 1 (cross) or 2 (T) → stop; `stopped = true` keeps it stopped (press RESET to start again).
3. `l.flags == 0` → no line → look at `l.lost` (1 = last seen left) and spin back.
4. `err = near×0.7 + far×0.3`, `turn = KP×err + KD×(err − last)`; `drive(BASE + turn, BASE − turn)`: line on the left (negative) → left wheel slower → steer left.

**What you should see**: the car follows the line; N on the OLED is negative when the line is to the left and positive to the right; it stops at the cross.

**Common problems**
- **Wobbles a lot?** → Decrease `KP`, slow `BASE`; runs off on bends → increase `KP`.
- **Steers the wrong way?** → Motors swapped: swap M1 and M2, or swap `+`/`−` in `drive()`.
- **Stops before the junction?** → The camera looks far ahead; stop only when `l.junctionDist` is above a number you choose.

---

## Next

- Every cell in detail: [value reference (MATRIX)](/en/sensors/unicam/reference.md#host-mx)
- Decode helper master files and other hosts: [downloads](/en/downloads.md)
- Product page and camera menu: [Sharpshooter Vision Universal v1](/en/sensors/unicam/index.md)
