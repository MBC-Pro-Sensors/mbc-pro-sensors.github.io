/*
 * IMU 陀螺儀 × Arduino　範例 11：下命令並確認做完了沒（命令狀態 0B）
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   一步一步示範陀螺儀的新命令，每送一個命令就看「命令狀態」（編號 0B）確認結果：
 *     1. 讀現在的航向
 *     2. E5 [90]：把航向設成 90 度 → 讀 01／02／03 都約 90
 *     3. E0：航向歸零 → 都約 0
 *     4. E7：陀螺零點重新估計（要放著不動約 1.5 秒）
 *     5. E6：以磁北為 0 → 02 融合航向 = 現在車頭朝向的磁北方位（0 北、90 東、180 南、270 西）
 *     6. 印出「命令狀態碼對照表」
 *   0B = (最後一個命令碼 × 256) + 狀態，例如 0xE500 = E5 完成、0xE701 = E7 忙碌中。
 *   狀態：0 完成、1 忙碌（還在做）、2 失敗、3 拒絕（現在不能做）。
 *
 * 你需要：IMU 陀螺儀（通用 I2C 版）、Arduino UNO／Nano／UNO R4 或 ESP32。不用小車。
 *
 * 接線：
 *   陀螺儀 VCC → 5V      陀螺儀 GND → GND
 *   陀螺儀 SDA → A4      陀螺儀 SCL → A5         （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。陀螺儀平放在桌上、不要碰，看它一步一步印出來。
 *   跑完之後打任何一個字再按 Enter，可以再跑一次。
 *
 * 小提醒：
 *   E6 要磁力計正常：遠離馬達、喇叭、鐵桌；回「3 拒絕」通常是磁力融合還沒準備好，等幾秒再試，或先跑範例 8 校準磁力計。
 *   E5、E6 只改 RAM，斷電就沒了；開機時航向一樣是 0。
 */

#include <Wire.h>                        // Arduino 內建的 I2C 通訊工具

const uint8_t ADDR = 0x6A;               // IMU 陀螺儀的「門牌號碼」（I2C 位址）
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

// 小幫手的宣告（實作在檔案最下面）；mbcWrite 的參數可以省略
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 3000)) {
    Serial.println(F("找不到陀螺儀，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  delay(2000);                           // 開機後多等 2 秒，讓磁力融合先準備好（E6 要用）
  runDemo();
}

void loop() {
  // 打任何字 + Enter → 再跑一次
  if (Serial.available()) {
    delay(50);
    while (Serial.available()) Serial.read();   // 把這一行剩下的字（例如換行）清掉
    runDemo();
  }
}

// ---- 整個示範流程 ----
void runDemo() {
  Serial.println();
  Serial.println(F("=== IMU 陀螺儀：新命令與命令狀態 0B ==="));

  // ---- 1. 現在的航向 ----
  Serial.println(F("[1] 現在的航向"));
  printHeadings();

  // ---- 2. E5 [90]：航向設成 90 度（參數 int16 小端：先送低 byte 再送高 byte） ----
  Serial.println(F("[2] 送 E5 [90]：把航向設成 90 度"));
  int16_t angle = 90;
  uint8_t p[2] = {(uint8_t)(angle & 0xFF), (uint8_t)((angle >> 8) & 0xFF)};
  int st = mbcCmd(ADDR, 0xE5, p, 2, 1000);
  printStatus(st);
  if (st == 0) {
    waitFused(90);                       // 02 融合航向要等融合重新初始化（最多約 3 秒）
    printHeadings();
  }

  // ---- 3. E0：航向歸零 ----
  Serial.println(F("[3] 送 E0：航向歸零"));
  st = mbcCmd(ADDR, 0xE0, NULL, 0, 1000);
  printStatus(st);
  if (st == 0) {
    waitFused(0);
    printHeadings();
  }

  // ---- 4. E7：陀螺零點重新估計（要靜止約 1.5 秒，4 秒內湊不到就失敗） ----
  Serial.println(F("[4] 送 E7：陀螺零點重新估計。請不要動陀螺儀！"));
  for (int i = 3; i >= 1; i--) {         // 倒數 3 秒，讓手離開
    Serial.print(F("    ")); Serial.print(i); Serial.println(F("…"));
    delay(1000);
  }
  unsigned long t0 = millis();
  st = mbcCmd(ADDR, 0xE7, NULL, 0, 5000);
  printStatus(st);
  Serial.print(F("    花了 ")); Serial.print(millis() - t0); Serial.println(F(" ms"));
  if (st == 2) Serial.println(F("    失敗 = 這段時間被移動了，放平不要碰再跑一次。"));

  // ---- 5. E6：以磁北為 0（完成後 02 融合航向 = 目前的磁北方位 0~359） ----
  Serial.println(F("[5] 送 E6：以磁北為 0"));
  st = mbcCmd(ADDR, 0xE6, NULL, 0, 4000);
  printStatus(st);
  if (st == 0) {
    delay(50);
    int16_t fused = mbcRead(ADDR, 0x02);
    int16_t mag = mbcRead(ADDR, 0x04);
    Serial.print(F("    02 融合航向 = ")); Serial.print(fused);
    Serial.print(F(" 度（= 現在朝向的磁北方位：0 北、90 東、180 南、270 西）  04 磁力航向 = "));
    Serial.println(mag);
  } else if (st == 3) {
    int16_t f3 = mbcRead(ADDR, 0xF3);
    Serial.print(F("    拒絕：磁力計還沒就緒。F3 bit6 磁力融合已初始化 = "));
    Serial.println((f3 != MBC_NONE && (f3 & 0x40)) ? F("是") : F("否"));
    Serial.println(F("    （或正在校準）。遠離鐵器、放著等幾秒再試；一直不行就跑範例 8 校準磁力計。"));
  } else if (st == 2) {
    Serial.println(F("    失敗：融合 3 秒內沒準備好（磁場干擾太大？），換個地方再試。"));
  }

  // ---- 6. 對照表 ----
  printStatusTable();
  Serial.println(F("跑完了。打任何一個字再按 Enter，可以再跑一次。"));
}

// 一次讀 01~03 三個航向印出來
void printHeadings() {
  int16_t h[3];
  if (!mbcReadMany(ADDR, 0x01, 3, h)) { Serial.println(F("    讀不到陀螺儀")); return; }
  Serial.print(F("    01 陀螺3D = ")); Serial.print(h[0]);
  Serial.print(F("   02 融合 = "));    Serial.print(h[1]);
  Serial.print(F("   03 陀螺1D = "));  Serial.println(h[2]);
}

// 等 02 融合航向跟上目標（差 10 度以內），最多 4 秒
void waitFused(int target) {
  unsigned long start = millis();
  while (millis() - start < 4000) {
    int16_t f = mbcRead(ADDR, 0x02);
    if (f != MBC_NONE && abs(f - target) <= 10) return;
    delay(100);
  }
}

// 印出狀態碼的意思，還有 0B 原始值
void printStatus(int st) {
  Serial.print(F("    → 狀態 ")); Serial.print(st); Serial.print(' ');
  switch (st) {
    case 0:  Serial.print(F("完成")); break;
    case 1:  Serial.print(F("忙碌（等到逾時還沒做完）")); break;
    case 2:  Serial.print(F("失敗")); break;
    case 3:  Serial.print(F("拒絕（現在不能做）")); break;
    default: Serial.print(F("讀不到陀螺儀")); break;
  }
  int16_t b = mbcRead(ADDR, 0x0B);
  if (b != MBC_NONE) {
    Serial.print(F("   0B = 0x")); Serial.print((uint16_t)b, HEX);
  }
  Serial.println();
}

void printStatusTable() {
  Serial.println(F("=== 命令狀態碼對照表（0B 的低 byte；高 byte = 命令碼）==="));
  Serial.println(F("  0  完成：命令做完了"));
  Serial.println(F("  1  忙碌：還在做（E6 等磁力融合、E7 等靜止、E8 等擷取安裝軸）"));
  Serial.println(F("  2  失敗：參數錯（少參數、超出範圍、確認碼不對），或做到一半沒成功（例如 E7 被移動）"));
  Serial.println(F("  3  拒絕：現在不能做（校準中不能下 E5~E8、EB；沒在校準卻下 E2／E4／E9；E6 磁力計沒就緒）"));
  Serial.println(F(" -1  讀不到：I2C 沒回應，檢查接線（這是範例自己定的，不是陀螺儀回的）"));
}

// ============================================================
// 以下是「通訊小幫手」（MBC I2C 協議 v2）：負責跟感應器講話。
// 所有 MBC 範例都一樣，不用改，也不用看懂。
//   讀法：寫 [編號] → 等 2 ms → 讀 2×k 個 byte = 從該編號起連續 k 個 int16（小端）
//   沒有這個編號（空號）時感應器回 -32768（MBC_NONE）
// ============================================================

// 開始 I2C 通訊（setup 裡呼叫一次）
// UNO/Nano：SDA=A4、SCL=A5；UNO R4：SDA/SCL 腳；ESP32：預設 GPIO21(SDA)/22(SCL)
void mbcBegin() {
  Wire.begin();
#if defined(WIRE_HAS_TIMEOUT)
  Wire.setWireTimeout(25000, true);      // 線沒接好時，不會讓 Arduino 當機卡住
#endif
}

// 從位址 addr 的感應器，由編號 id 起連續讀 k 個值（1~16）放進 out。全部讀到回傳 true
bool mbcReadMany(uint8_t addr, uint8_t id, uint8_t k, int16_t *out) {
  if (k < 1) k = 1;
  if (k > 16) k = 16;                    // 一次最多 16 個值（32 byte，Wire 緩衝區上限）
  Wire.beginTransmission(addr);
  Wire.write(id);                        // 告訴感應器：我要從第 id 號開始讀
  if (Wire.endTransmission() != 0) return false;
  delay(2);                              // 等 2 ms 再讀（ESP32 型的產品一定要等）
  uint8_t n = (uint8_t)(2 * k);
  if (Wire.requestFrom(addr, n) != n) return false;
  for (uint8_t i = 0; i < k; i++) {
    uint16_t lo = Wire.read();           // 先來的是低位元組
    uint16_t hi = Wire.read();           // 再來高位元組（小端）
    out[i] = (int16_t)(uint16_t)(lo | (hi << 8));
  }
  return true;
}

// 讀一個值。讀不到（沒接好）回傳 MBC_NONE
int16_t mbcRead(uint8_t addr, uint8_t id) {
  int16_t v;
  return mbcReadMany(addr, id, 1, &v) ? v : MBC_NONE;
}

// 送命令：寫 [id][參數…]。p 指向參數、n = 參數幾個 byte（沒參數就不填）。成功回傳 true
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p, uint8_t n) {
  Wire.beginTransmission(addr);
  Wire.write(id);
  for (uint8_t i = 0; i < n; i++) Wire.write(p[i]);
  return Wire.endTransmission() == 0;
}

// 等感應器準備好（狀態旗標 F3 的 bit0 = 1），最多等 ms 毫秒。準備好了回傳 true
bool mbcWaitReady(uint8_t addr, uint16_t ms) {
  unsigned long start = millis();
  while (millis() - start < ms) {
    int16_t f = mbcRead(addr, 0xF3);
    if (f != MBC_NONE && (f & 1)) return true;
    delay(50);
  }
  return false;
}

// ============================================================
// 命令小幫手（範例 11、12 才有）：送命令，等「命令狀態」0B 回報結果。
//   0B = (命令碼 << 8) | 狀態；狀態 0 完成、1 忙碌、2 失敗、3 拒絕。
//   注意：同一個命令連送兩次時，剛送完那一下 0B 可能還是「上一次」的結果。
//   所以先記下 F4（快照序號，每 10 ms 或收到命令後 +1，0~255 繞回），
//   等 F4 比送命令前多 2 以上，那時讀到的 0B 一定是這次命令之後的。
// 回傳：0 完成、1 忙碌（等到 timeoutMs 還沒好）、2 失敗、3 拒絕、-1 讀不到／寫不進去
// ============================================================
int mbcCmd(uint8_t addr, uint8_t code, const uint8_t *p, uint8_t n, uint16_t timeoutMs) {
  int16_t s0 = mbcRead(addr, 0xF4);      // 送命令前的快照序號
  if (s0 == MBC_NONE) return -1;
  if (!mbcWrite(addr, code, p, n)) return -1;
  bool fresh = false;                    // F4 已經多 2 了嗎？（多了以後的 0B 才採信）
  bool gotAny = false;                   // 有沒有讀到過任何東西
  unsigned long start = millis();
  while (true) {
    int16_t s = mbcRead(addr, 0xF4);     // 先讀 F4、再讀 0B（0B 一定不會比 F4 舊）
    int16_t b = mbcRead(addr, 0x0B);
    if (s != MBC_NONE && b != MBC_NONE) {
      gotAny = true;
      if ((uint8_t)((uint8_t)s - (uint8_t)s0) >= 2) fresh = true;
      uint8_t who = (uint16_t)b >> 8;    // 高 byte：最後一個命令碼
      uint8_t state = b & 0xFF;          // 低 byte：狀態
      if (fresh && who == code && state != 1) return state;
    }
    if (millis() - start >= timeoutMs) return gotAny ? 1 : -1;
    delay(10);
  }
}
