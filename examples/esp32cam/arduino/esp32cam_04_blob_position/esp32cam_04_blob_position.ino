/*
 * 神攝手 ESP32CAM × Arduino　範例 4：色塊在哪裡？
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   讀色塊的位置與大小（編號 05~0A，一次讀 6 個）：
 *     05 X 0~100（左 = 0、右 = 100）、06 Y 0~75（下 = 0、上 = 75）
 *     07 寬、08 高、09 面積（寬×高）、0A 有找到（1 = 畫面裡有目標色）
 *   每 0.2 秒用中文說色塊在「左／中／右」「上／中／下」，還有大小。
 *   沒找到時 X、Y、寬、高、面積都是 0。
 *
 * 你需要：神攝手 ESP32CAM（I2C 版韌體）、Arduino UNO／Nano／UNO R4 或 ESP32。
 *
 * 接線：
 *   神攝手 5V  → 5V       神攝手 GND → GND
 *   神攝手 IO3（SDA）→ A4   神攝手 IO1（SCL）→ A5      （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *   ※ 硬體已確認可接，I2C 版韌體上機待測。IO1/IO3 跟 EV3／SPIKE 串口版共用，韌體二選一。
 *
 * 準備：
 *   先在神攝手背面的螢幕設定好取樣點 S1 和要追的顏色（設定存在機身）。
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。拿目標顏色的東西在鏡頭前左右、上下、遠近移動。
 *
 * 小提醒：
 *   這支是「學校版」韌體用的（編號 01~0A）。WRO 版（HUE12）請看範例 6。
 */

#include <Wire.h>                        // Arduino 內建的 I2C 通訊工具

const uint8_t ADDR = 0x4C;               // 神攝手的「門牌號碼」（I2C 位址），學校版與 WRO 版都是 0x4C
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

// 小幫手的宣告（實作在檔案最下面）；mbcWrite 的參數可以省略
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

unsigned long lastPrint = 0;

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 8000)) {
    Serial.println(F("找不到神攝手，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！拿目標顏色的東西到鏡頭前。"));
}

void loop() {
  if (millis() - lastPrint >= 200) {     // 👉 可以改：每 0.2 秒印一次
    lastPrint = millis();

    // 一次讀編號 05~0A 共 6 個值：[0] X [1] Y [2] 寬 [3] 高 [4] 面積 [5] 有找到
    int16_t b[6];
    if (!mbcReadMany(ADDR, 0x05, 6, b)) { Serial.println(F("讀不到神攝手")); return; }

    if (!b[5]) {                         // 0A = 0：畫面裡沒有目標色
      Serial.println(F("沒有找到色塊"));
      return;
    }

    // X：0~100 分成左／中／右；Y：0~75 分成下／中／上
    Serial.print(F("色塊在 "));
    if (b[0] < 35)      Serial.print(F("左"));
    else if (b[0] > 65) Serial.print(F("右"));
    else                Serial.print(F("中"));
    if (b[1] < 25)      Serial.print(F("下"));
    else if (b[1] > 50) Serial.print(F("上"));
    else                Serial.print(F("中"));

    Serial.print(F("  X=")); Serial.print(b[0]);
    Serial.print(F(" Y=")); Serial.print(b[1]);
    Serial.print(F("  寬 ")); Serial.print(b[2]);
    Serial.print(F(" 高 ")); Serial.print(b[3]);
    Serial.print(F(" 面積 ")); Serial.print(b[4]);
    if (b[4] > 2000)     Serial.print(F("（很近／很大）"));
    else if (b[4] < 100) Serial.print(F("（很遠／很小）"));
    Serial.println();
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
