/*
 * 掌控者 PS2 × Arduino　範例 1：跟接收器打招呼
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   確認接收器有沒有接好。接好了，會印出它的身分證（裝置 ID、款式、協議版本、
 *   韌體版本）和目前狀態。之後一直看著手把：連上、斷線都會告訴你。
 *   每次接新的接收器，都建議先跑這個範例。
 *
 * 你需要：掌控者 PS2 接收器（通用 I2C 版）＋ PS2 手把、Arduino UNO／Nano／UNO R4 或 ESP32。
 *
 * 接線：
 *   接收器 VCC → 5V      接收器 GND → GND
 *   接收器 RX（= SDA） → A4      接收器 TX（= SCL） → A5         （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *   （I2C 版接收器上印的 TX 就是 SCL、RX 就是 SDA）
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。
 *   把手把開機（接收器的燈亮），看電腦有沒有說「手把連上了」。
 *
 * 小提醒：
 *   手把中間的 ANALOG 鍵要讓燈亮著（類比模式），搖桿才會有數字。
 */

#include <Wire.h>                        // Arduino 內建的 I2C 通訊工具，用來跟接收器講話

const uint8_t ADDR = 0x42;               // 掌控者接收器的「門牌號碼」（I2C 位址），PS2 與 PS4 都是 0x42
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

// 小幫手的宣告（實作在檔案最下面）；mbcWrite 的參數可以省略
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

bool wasConnected = false;               // 上一次看的時候，手把有沒有連上

void setup() {
  Serial.begin(115200);                  // 打開跟電腦講話的通道，速度 115200
  mbcBegin();                            // 準備好 I2C 通訊（小幫手在檔案最下面）
  delay(500);                            // 等接收器開機 0.5 秒

  Serial.println(F("=== 掌控者 PS2：跟接收器打招呼 ==="));

  // 等接收器說「我準備好了」（狀態旗標 F3 的 bit0），最多等 5 秒
  if (!mbcWaitReady(ADDR, 5000)) {
    Serial.println(F("找不到接收器！請檢查："));
    Serial.println(F("  1. RX（SDA）接 A4、TX（SCL）接 A5（ESP32：21 / 22），有沒有接反？"));
    Serial.println(F("  2. VCC 接 5V、GND 接 GND，有沒有插緊？接收器的燈有沒有亮？"));
    Serial.println(F("  3. 你的接收器是「通用 I2C 版」嗎？（SPIKE 版、EV3 版不能接 Arduino）"));
    while (true) {}                      // 找不到就停在這裡
  }

  // ---- F0 裝置 ID：低 byte = 出廠位址、高 byte = 款式（1 = PS2、2 = PS4/PS5） ----
  int16_t id = mbcRead(ADDR, 0xF0);
  uint8_t factoryAddr = (uint8_t)(id & 0xFF);        // 出廠位址（換過位址也不變）
  uint8_t kind        = (uint8_t)((id >> 8) & 0xFF); // hw[0]：1 = PS2 接收器、2 = PS4/PS5 接收器
  Serial.print(F("找到了！出廠位址 0x"));
  Serial.println(factoryAddr, HEX);      // 應該是 42
  Serial.print(F("款式："));
  if (kind == 1)      Serial.println(F("PS2 2.4G 接收器"));
  else if (kind == 2) Serial.println(F("PS4/PS5 藍牙接收器"));
  else                Serial.println(F("不認識的款式"));

  // ---- F1 協議版本（這份範例是照版本 2 寫的） ----
  Serial.print(F("協議版本："));
  Serial.println(mbcRead(ADDR, 0xF1));

  // ---- F2 韌體版本：主版×100 + 次版（例如 102 = 1.2） ----
  int16_t fw = mbcRead(ADDR, 0xF2);
  Serial.print(F("韌體版本："));
  Serial.print(fw / 100);
  Serial.print('.');
  Serial.println(fw % 100);

  // ---- F3 狀態旗標：一個 bit 代表一件事 ----
  int16_t st = mbcRead(ADDR, 0xF3);
  Serial.println(F("目前狀態："));
  Serial.print(F("  bit0 就緒        ")); Serial.println((st & 0x01) ? F("是") : F("否"));
  Serial.print(F("  bit1 手把已連線  ")); Serial.println((st & 0x02) ? F("是") : F("否"));

  // ---- F5 開機秒數（順便看它開機多久了） ----
  Serial.print(F("開機秒數："));
  Serial.println(mbcRead(ADDR, 0xF5));

  Serial.println(F("接線正確！現在把手把開機（接收器的燈亮）試試看。"));
}

void loop() {
  // 每一圈都看一下 F3 的 bit1（手把有沒有連上），有變化才印，畫面才不會洗版
  int16_t st = mbcRead(ADDR, 0xF3);
  if (st == MBC_NONE) {                  // 讀不到 = 線鬆掉了
    Serial.println(F("讀不到接收器，線鬆掉了嗎？"));
    delay(1000);
    return;
  }
  bool connected = st & 0x02;
  if (connected != wasConnected) {
    if (connected) {
      Serial.print(F("手把連上了！電量 "));
      Serial.print(mbcRead(ADDR, 0x07));  // 07 電量 0~100（PS2 手把沒有電量，恆 0）
      Serial.println(F("%"));
    } else {
      Serial.println(F("手把沒有連線"));
    }
    wasConnected = connected;
  }
  delay(50);
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
