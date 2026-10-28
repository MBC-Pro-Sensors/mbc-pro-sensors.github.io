/*
 * 神攝手 × MATRIX Mini R4　範例 2：循線小車（近／遠偏移 PD 循線，遇到路口停車）
 * Sharpshooter Vision × MATRIX Mini R4  example 2: line follower (near/far offset PD, stop at a junction)
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
// 一個物體（模式 51／52）：code 物體代碼＝色號×10＋形狀（0 = 沒有）、color 色號 1~8、shape 形狀 0~9、x、y 質心像素、
// w、h 外框像素（52）、area 面積 %；angle、vertices、trackId MATRIX 沒給（-1，要用 I2C 版）；沒給的是 -1
struct UcObject { int code, color, shape, x, y, w, h, area, angle, vertices, trackId; };
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
// 以下是「神攝手 × mVision 解碼小幫手」，所有 MATRIX 範例都一樣，不用改。
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

// 物體偵測：這個模式最多幾個物體：51 → 4、52 → 3
uint8_t ucMaxObject(uint8_t mode) {
  if (mode == 51) return 4;
  if (mode == 52) return 3;
  return 0;
}

// 畫面裡有幾個物體 0~8（模式 51／52 第 1 格）；其他模式回傳 -1
int ucObjectCount(uint8_t mode) {
  if (mode == 51 || mode == 52) return (int)CamData[0];
  return -1;
}

// 讀物體 i（1 = 面積最大）：51 每個物體 2 格（代碼＋面積、X＋Y）、52 每個 3 格（再加 寬＋高）
bool ucObject(uint8_t mode, uint8_t i, UcObject &o) {
  if (i < 1 || i > ucMaxObject(mode)) return false;
  uint8_t base = (mode == 51) ? 2 * i : 3 * i - 1;     // 代碼＋面積 在第 base 格
  o.code = ucLo(base); o.area = ucHi(base);
  o.color = o.code / 10; o.shape = o.code % 10;
  o.x = ucLo(base + 1); o.y = ucHi(base + 1);
  o.w = -1; o.h = -1; o.angle = -1; o.vertices = -1; o.trackId = -1;
  if (mode == 52) { o.w = ucLo(base + 2); o.h = ucHi(base + 2); }
  return true;
}

// 形狀編號 → 英文名稱：0 other 1 circle 2 ellipse 3 triangle 4 square 5 rectangle 6 pentagon 7 hexagon 8 star 9 trapezoid
const char *ucShapeName(int s) {
  switch (s) {
    case 0: return "other";    case 1: return "circle";    case 2: return "ellipse";  case 3: return "triangle";
    case 4: return "square";   case 5: return "rectangle"; case 6: return "pentagon"; case 7: return "hexagon";
    case 8: return "star";     case 9: return "trapezoid";
  }
  return "?";
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
