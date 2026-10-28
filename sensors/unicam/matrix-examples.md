<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 程式碼區塊＝examples/unicam/matrix/*/*.ino 原文（解碼小幫手正本：02_相機感應器\06_通用版\通用版v1\上位機函式庫\matrix\MBC_UniCam_MATRIX.h）；拆法表依規格定案第 5 節（spec_layout.json host mx） -->
# 🟩 MATRIX 範例：神攝手通用版 v1 × MATRIX Mini R4

[🔙 回到神攝手通用版 v1 產品頁](/sensors/unicam/index.md)　｜　[MATRIX mVision 讀值教學](/sensors/unicam/matrix/mvision.md)　｜　[📋 讀值參考（MATRIX）](/sensors/unicam/reference.md#host-mx)

> [!WARNING]
> **v1 尚未上機驗證**：數值排法已定案並通過電腦端測試，實機數值（fps、循線門檻、I2C 腳位）上機後可能調整。

神攝手通用版 v1（MATRIX 版韌體）對 MATRIX Mini R4 來說就是一台 **mVision 攝影機**：相機每 **20 ms** 送一包 10 格資料，走 **Serial1 115200**，MATRIXblock 的「mVision」積木就能讀，不用裝函式庫、不走 I2C。

這一頁有兩部分：
1. **只用 MATRIXblock 積木**：下面的「積木怎麼拆」表，告訴你每個模式的「mVision 資料 0～9」各放了什麼、要怎麼算出來。
2. **兩支 C++ 範例**（跟著色塊轉、循線小車），用 **Arduino IDE** 上傳。

> [!NOTE]
> - MATRIXblock 的程式碼窗是**唯讀**的（只能看積木產生的程式，不能貼 C++），所以 C++ 範例要用 Arduino IDE：開發板管理員裝「Arduino UNO R4 Boards」、函式庫管理員裝「**MatrixMiniR4**」、開發板選 **Arduino UNO R4 WiFi**。範例在電腦上用 arduino-cli（UNO R4 WiFi＋MatrixMiniR4 1.2.2）編譯通過，**還沒上車跑過**。
> - MATRIX 讀不到相機現在的模式，也沒有切模式的命令：**一定要在相機選單切模式**，程式裡的 `MODE` 要跟相機一樣。

## 這頁會用到的數值

- **模式編號** ＝ 相機模式 × 10 ＋ 子選項（例 13＝顏色追蹤 ③ 標準、41＝循線 ① 黑線）。
- **座標**：像素 X 0～159（左 → 右）、Y 0～119（上 → 下）；**0＝沒找到**。
- **色相碼** 0～255：0～240 色相（紅 0／240、黃 40、綠 80、青 120、藍 160、紫 200）；241～248 白／灰；249～255 黑。
- **會有負數的值**（循線偏移、角度、左右緣）**加 100 送**，讀到後減 100。偏移：站在車子後面往前看，**線在左＝負**。

## 積木怎麼拆（mVision 資料 0～9）

每一格是 0～65535 的整數。大部分一格放兩個值：

- **低**＝資料 ÷ 256 的**餘數**（MATRIXblock 的「餘數」積木）
- **高**＝資料 ÷ 256 **取整數**（「÷」之後再「無條件捨去」）
- 模式 11 例外：一格放兩色的「X格×10＋Y格」，**前一色＝資料 ÷ 100 取整數**、**後一色＝資料 ÷ 100 的餘數**；再把「X格×10＋Y格」拆成 X 格＝÷10 取整數（1～8）、Y 格＝÷10 的餘數（1～6，上＝小），像素 ≈ 格 × 20 − 10。

**相機模式 1 顏色追蹤**

| 模式 | 資料 0 | 資料 1 | 資料 2 | 資料 3～8 | 資料 9 |
|---|---|---|---|---|---|
| 11 ① 最多色（16 色） | 色 1（÷100）、色 2（餘數） | 色 3、色 4 | 色 5、色 6 | 資料 n ＝ 色 2n+1、色 2n+2（到色 16 在資料 7）；**資料 8＝找到幾色** | 不用（0） |
| 12 ② 多色（10 色） | 低＝色 1 X、高＝色 1 Y | 色 2 X、Y | 色 3 X、Y | 資料 n ＝ 色 n+1 的 X、Y | 色 10 X、Y |
| 13 ③ 標準（5 色） | 低＝色 1 X、高＝色 1 Y | 低＝色 1 面積 %、高＝色 1 數量 | 色 2 X、Y | 每色兩格：位置一格、面積＋數量一格 | 色 5 面積、數量 |
| 14 ④ 精細（3 色） | 低＝色 1 X、高＝色 1 Y | 低＝色 1 寬、高＝色 1 高（像素） | 低＝色 1 面積 %、高＝色 1 數量 | 資料 3～5＝色 2、資料 6～8＝色 3（同樣三格） | 找到幾色（不用拆） |

**相機模式 2 座標取色**

| 模式 | 拆法 |
|---|---|
| 21 ① 色相（20 點） | 資料 n：低＝S(2n+1) 色相碼、高＝S(2n+2) 色相碼（資料 0＝S1、S2 … 資料 9＝S19、S20） |
| 22 ② 色相＋飽和（10 點） | 資料 n：低＝S(n+1) 色相碼、高＝S(n+1) 飽和度 |
| 23 ③ 色相＋飽和＋明度（6 點） | 10 格依序拆成 20 個 byte（資料 0 低、資料 0 高、資料 1 低…），每 3 個一點：S1 色相、飽和、明度、S2 色相…；資料 0＝S1 色相＋飽和、資料 1＝S1 明度＋S2 色相、資料 2＝S2 飽和＋明度，之後每 3 格重複；資料 9 不用 |

**相機模式 3 照順序給色號**（模式 30，最多 18 塊）

| 資料 0～8 | 資料 9 |
|---|---|
| 資料 n：低＝左起第 2n+1 塊色相碼、高＝第 2n+2 塊 | **總塊數**（不用拆） |

**相機模式 4 循線**（模式 41／42／43 排法一樣）

| 資料 | 內容 | 拆法 |
|---|---|---|
| 0 | 近處偏移 | **減 100**（−100～100） |
| 1 | 中段偏移 | 減 100 |
| 2 | 遠處偏移 | 減 100 |
| 3 | 線角度 | 減 100（−90～90） |
| 4 | 路口類型 0 無 1 十字 2 T 3 左 4 右 5 盡頭 | 不用拆 |
| 5 | 有線旗標（近 1＋中 2＋遠 4，0＝沒看到線） | 不用拆 |
| 6 | 低＝近處線寬 %、高＝路口距離 % | ÷256 |
| 7 | 低＝左緣、高＝右緣 | ÷256 之後各**減 100** |
| 8 | 低＝遠處線寬 %、高＝穿過的彩色標記線色號（第幾個目標色 1～16，0＝還沒穿過） | ÷256 |
| 9 | 低＝標記線累計條數、高＝最後離線方向 0 有線 1 左 2 右 | ÷256 |

> [!TIP]
> 例：顏色追蹤 ③ 標準，色 1 在不在畫面左半邊 →「mVision 資料 0 ÷ 256 的餘數」就是色 1 X，`X ＞ 0 且 X ＜ 80` 就是左半邊。循線：「mVision 資料 0 − 100」就是近處偏移。

---

## 範例 1：跟著色塊轉

**這個範例會做什麼**：色 1（相機教的第 1 個顏色）在左邊 → 車子往左轉；在右邊 → 往右轉；在中間 → 往前開。色塊夠大（夠近）或看不到 → 停車。OLED 顯示 X 和面積。

**要準備什麼**
- MATRIX Mini R4＋兩顆 DC 馬達的小車（左輪 M1、右輪 M2）＋電池
- 神攝手通用版 v1（MATRIX 版韌體）
- 一顆顏色鮮豔的球或積木

**相機設定**：教好「目標 1」；相機模式 **1 顏色追蹤 ・子選項 ③ 標準**（模式編號 13）。

**怎麼接**：相機接 Mini R4 給 mVision 用的 UART 孔；左輪馬達 M1、右輪馬達 M2。

**下載程式**：[unicam_mx_01_track_turn.ino](/examples/unicam/matrix/unicam_mx_01_track_turn/unicam_mx_01_track_turn.ino)（放進同名資料夾，用 Arduino IDE 開）

**程式**

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

**一步一步看懂程式**
1. 程式最上面的「TOP」段宣告 `CamData[10]`（mVision 10 格）和 `UcBlob` 這些盒子；最下面的「小幫手」段負責拆格子，不用改。
2. `MiniR4.Vision.Begin()`：開始收 mVision（Serial1 115200）。
3. 每圈 `MiniR4.Vision.SmartCamReader(CamData)` 讀一包；`CamStatus > 0` 才算讀到，沒讀到就停車。
4. `ucBlob(MODE, 1, b)`：把資料 0、1 拆成色 1 的 X、Y、面積、數量。
5. `x == 0` → 沒看到 → 停；`area ≥ 25` → 夠近 → 停；其他：`turn = (x − 80) ÷ 2`，`drive(BASE + turn, BASE − turn)`，色塊在右 → 左輪快 → 右轉。

**跑起來應該看到什麼**：把球放在車子前面偏左，車子邊走邊往左轉、對準球；靠到很近停下來。OLED 第一行 X（左小右大），第二行 A（面積 %）。

**常見問題**
- **車子往後跑／原地轉圈？** → 某一輪方向反了：把 `setup()` 裡那一輪的 `setReverse` 改成相反（true／false）。
- **OLED 一直不動？** → 沒收到相機資料：看相機螢幕左上主機名稱是不是綠色、UART 線有沒有接對孔、相機韌體是不是 MATRIX 版。
- **還很遠就停？** → `STOP_AREA` 調大；球很小就調小。

---

## 範例 2：循線小車（PD 循線＋路口停車）

**這個範例會做什麼**：沿著線走，線偏左就往左修、偏右就往右修（近處＋遠處偏移一起看）；遇到**十字或 T 字路口**停車；看不到線就往最後看到線的那一邊原地轉。OLED 顯示近處偏移。

**要準備什麼**：範例 1 的小車＋白底黑線場地（線寬約 2 cm），中間做一個十字路口。

**相機設定**：相機裝車頭、斜斜往前下方看地面；相機模式 **4 循線 ・子選項 ① 黑線**（模式編號 41；白線 ②、色線 ③ 程式不用改）。

**怎麼接**：同範例 1。

**下載程式**：[unicam_mx_02_line_follow.ino](/examples/unicam/matrix/unicam_mx_02_line_follow/unicam_mx_02_line_follow.ino)

**程式**

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

**一步一步看懂程式**
1. `ucLine(l)`：把 10 格拆成 14 個循線值（＋100 的已經減回來）。
2. `l.junction` 是 1（十字）或 2（T）→ 停車，`stopped = true` 之後一直停著（按 RESET 重來）。
3. `l.flags == 0` → 沒看到線 → 看 `l.lost`（1＝最後在左）原地轉回去。
4. `err = 近×0.7 ＋ 遠×0.3`、`turn = KP×err ＋ KD×(err − 上一次)`；`drive(BASE + turn, BASE − turn)`：線在左（負）→ 左輪慢 → 往左修。

**跑起來應該看到什麼**：車子沿線走，OLED 的 N 在線偏左時是負數、偏右是正數；走到十字路口停住。

**常見問題**
- **左右晃很厲害？** → `KP` 減小、`BASE` 調慢；彎道衝出去 → `KP` 加大。
- **修的方向反了？** → 馬達左右接反：M1、M2 對調，或對調 `drive()` 裡的 `+`／`−`。
- **還沒到路口就停？** → 相機看得遠，可以改成「`l.junctionDist` 大於某個數字才停」。

---

## 下一步

- 每一格的完整對照：[讀值參考（MATRIX）](/sensors/unicam/reference.md#host-mx)
- 解碼小幫手正本與其他平台：[下載中心](/downloads.md)
- 產品頁與相機選單操作：[神攝手通用版 v1](/sensors/unicam/index.md)
