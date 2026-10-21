/*
 * 測距者 8 路 × Arduino　範例 1：跟感應器打招呼
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   確認感應器有沒有接好。接好了，會在電腦上印出感應器的名字、版本，
 *   並檢查 8 顆雷射是不是都正常。
 *   每次接新感應器，都建議先跑這個範例。
 *
 * 接線（Arduino UNO / Nano）：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。
 *
 * 小提醒：
 *   測距者 8 路一開機要花 2 ~ 4 秒把 8 顆雷射一顆一顆叫醒，
 *   這段時間讀到的距離全部都是 0（看起來像貼著東西），所以一定要先等它「準備好」。
 */

#include <Wire.h>                        // Arduino 內建的 I2C 通訊工具，用來跟感應器講話

const uint8_t SENSOR_ADDR = 0x29;        // 測距者感應器的「門牌號碼」，固定是 0x29

void setup() {
  Serial.begin(115200);                  // 打開跟電腦講話的通道，速度 115200
  mbcBegin();                            // 準備好 I2C 通訊（小幫手在檔案最下面）
  delay(500);                            // 等感應器開機 0.5 秒

  Serial.println(F("=== 測距者 8 路：跟感應器打招呼 ==="));
  Serial.println(F("等 8 顆雷射開機（最多 6 秒）..."));

  // 等感應器說「我準備好了」，最多等 6 秒（8 顆雷射開機要 2 ~ 4 秒）
  if (!mbcWaitReady(6000)) {
    Serial.println(F("找不到感應器！請檢查："));
    Serial.println(F("  1. SDA 接 A4、SCL 接 A5，有沒有接反？"));
    Serial.println(F("  2. VCC 接 5V、GND 接 GND，有沒有插緊？"));
    Serial.println(F("  3. 你的感應器是「通用 I2C 版」嗎？（SPIKE 版、EV3 版不能接 Arduino）"));
    Serial.println(F("  4. 同一組 SDA/SCL 上有沒有接其他門牌號碼也是 0x29 的東西（例如測距者 2 路、市售 VL53L0X 模組）？"));
    while (true) {}                      // 找不到就停在這裡
  }

  // 讀「版本資料」：感應器的第 0x01 號格子，共 17 個數字
  uint8_t info[17];
  if (!mbcRead(0x01, info, 17)) {
    Serial.println(F("讀取失敗，檢查一下線有沒有鬆掉。"));
    while (true) {}
  }

  // 第 9 ~ 16 個數字是感應器的名字（英文字母）
  char name[9] = {0};
  memcpy(name, &info[9], 8);

  Serial.print(F("找到了！感應器名字："));
  Serial.println(name);                  // 應該會印出 LASER8

  Serial.print(F("韌體版本："));
  Serial.print(info[2]);                 // 主版本
  Serial.print('.');
  Serial.println(info[3]);               // 次版本

  Serial.print(F("雷射數量："));
  Serial.print(info[7]);                 // 測距者 8 路 = 8
  Serial.println(F(" 顆"));

  Serial.print(F("最遠量得到："));
  Serial.print(info[8] * 10);            // 這格存的是「幾個 10 毫米」，乘 10 變成毫米（1200）
  Serial.println(F(" mm（超過就當作「沒有東西」）"));

  // 讀「健康檢查」：第 0x31 號格子，共 9 個數字
  //   第 0 個：狀態（第 1 個位元 = 有沒有雷射故障）
  //   第 2 個：每一個位元代表一顆雷射有沒有開機成功（1 = 成功）
  uint8_t st[9];
  if (!mbcRead(0x31, st, 9)) {
    Serial.println(F("讀取失敗，檢查一下線有沒有鬆掉。"));
    while (true) {}
  }

  if (st[0] & 0x02) {                    // 第 1 個位元是 1 → 有雷射故障
    Serial.print(F("注意：有雷射沒開機成功，編號："));
    for (uint8_t i = 0; i < 8; i++) {
      if (!(st[2] & (1u << i))) {        // 這一顆的位元是 0 → 沒開機成功
        Serial.print(i);
        Serial.print(' ');
      }
    }
    Serial.println();
    Serial.println(F("（可以跑範例 2 看看是哪一顆一直顯示 XX，再聯絡我們）"));
  } else {
    Serial.println(F("8 顆雷射都正常！"));
  }

  Serial.println(F("接線正確，可以開始玩其他範例了！"));
}

void loop() {
  // 這個範例只要在開機時檢查一次，所以 loop 裡什麼都不做
}

// ============================================================
// 以下是「通訊小幫手」：負責跟感應器講話。
// 所有 MBC 範例都一樣，不用改，也不用看懂。
// ============================================================

// 開始 I2C 通訊（setup 裡呼叫一次）
void mbcBegin() {
  Wire.begin();
#if defined(WIRE_HAS_TIMEOUT)
  Wire.setWireTimeout(25000, true);      // 線沒接好時，不會讓 Arduino 當機卡住
#endif
}

// 從感應器的第 reg 號格子，讀 n 個數字放進 buf。讀到了回傳 true
bool mbcRead(uint8_t reg, uint8_t *buf, uint8_t n) {
  Wire.beginTransmission(SENSOR_ADDR);
  Wire.write(reg);
  if (Wire.endTransmission() != 0) return false;
  if (Wire.requestFrom(SENSOR_ADDR, n) != n) return false;
  for (uint8_t i = 0; i < n; i++) buf[i] = Wire.read();
  return true;
}

// 把數字 value 寫進感應器的第 reg 號格子。成功回傳 true
bool mbcWrite(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(SENSOR_ADDR);
  Wire.write(reg);
  Wire.write(value);
  return Wire.endTransmission() == 0;
}

// 等感應器準備好，最多等 timeoutMs 毫秒。準備好了回傳 true
bool mbcWaitReady(uint16_t timeoutMs) {
  unsigned long start = millis();
  uint8_t flags;
  while (millis() - start < timeoutMs) {
    if (mbcRead(0x32, &flags, 1) && (flags & 0x01)) return true;
    delay(50);
  }
  return false;
}
