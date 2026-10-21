/*
 * 陀螺儀 IMU × Arduino　範例 1：跟感應器打招呼
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   確認感應器有沒有接好。接好了，會在電腦上印出感應器的名字、版本，
 *   還有「現在哪一面朝上」。每次接新感應器，都建議先跑這個範例。
 *
 * 你需要：
 *   Arduino UNO / Nano（或 ESP32）、陀螺儀 IMU（通用 I2C 版）、4 條杜邦線。
 *
 * 接線（Arduino UNO / Nano）：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。
 *
 * 小提醒：
 *   陀螺儀開機時要先「靜靜不動」大約 0.5 秒，才會說「我準備好了」。
 *   所以請把感應器平放在桌上，等看到結果之前都不要碰它。
 */

#include <Wire.h>                        // Arduino 內建的 I2C 通訊工具，用來跟感應器講話

const uint8_t SENSOR_ADDR = 0x6A;        // 陀螺儀的「門牌號碼」，固定是 0x6A

void setup() {
  Serial.begin(115200);                  // 打開跟電腦講話的通道，速度 115200
  mbcBegin();                            // 準備好 I2C 通訊（小幫手在檔案最下面）
  delay(500);                            // 等感應器開機 0.5 秒

  Serial.println(F("=== 陀螺儀 IMU：跟感應器打招呼 ==="));
  Serial.println(F("請把感應器放在桌上，不要動它……"));

  // 等感應器說「我準備好了」，最多等 5 秒（它要先靜止約 0.5 秒）
  if (!mbcWaitReady(5000)) {
    uint8_t id;
    if (mbcRead(0x02, &id, 1) && id == 0x6A) {
      // 找得到感應器，只是它一直覺得自己在動
      Serial.println(F("找到感應器了，但它一直覺得自己在晃動，沒辦法準備好。請檢查："));
      Serial.println(F("  1. 感應器有沒有放在桌上？用手拿著會一直有一點點晃動。"));
      Serial.println(F("  2. 桌子有沒有在震動？（旁邊有馬達、風扇在轉也會）"));
      Serial.println(F("  3. 放好以後，按一下 Arduino 的 RESET 鍵再試一次。"));
    } else {
      Serial.println(F("找不到感應器！請檢查："));
      Serial.println(F("  1. SDA 接 A4、SCL 接 A5，有沒有接反？"));
      Serial.println(F("  2. VCC 接 5V、GND 接 GND，有沒有插緊？感應器上的紅燈有沒有在閃？"));
      Serial.println(F("  3. 你的感應器是「通用 I2C 版」嗎？（SPIKE 版、EV3 版不能接 Arduino）"));
      Serial.println(F("  4. 還是不行：拔掉感應器的電源再插回去，等 3 秒後按 Arduino 的 RESET 鍵。"));
    }
    while (true) {}                      // 停在這裡，不往下做了
  }

  // ---- 1. 讀「版本資料」：感應器的第 0x01 號格子，共 17 個數字 ----
  uint8_t info[17];
  mbcRead(0x01, info, 17);

  // 第 9 ~ 16 個數字是感應器的名字（英文字母）
  char name[9] = {0};
  memcpy(name, &info[9], 8);

  Serial.print(F("找到了！感應器名字："));
  Serial.println(name);                  // 應該會印出 IMU

  Serial.print(F("韌體版本："));
  Serial.print(info[2]);                 // 主版本
  Serial.print('.');
  Serial.println(info[3]);               // 次版本

  Serial.print(F("韌體日期：20"));
  Serial.print(info[4]);                 // 年（西元末兩位）
  Serial.print('/');
  Serial.print(info[5]);                 // 月
  Serial.print('/');
  Serial.println(info[6]);               // 日

  Serial.print(F("磁力計："));
  if (info[8] == 1) Serial.println(F("正常"));
  else              Serial.println(F("沒有偵測到"));

  // ---- 2. 讀「狀態」：第 0x31 號格子，共 9 個數字 ----
  uint8_t st[9];
  mbcRead(0x31, st, 9);
  uint8_t flags = st[0];                 // 一排「開關」，每一個位元代表一件事
  uint8_t upFace = st[4];                // 現在哪一面朝上：0 ~ 5

  Serial.print(F("現在朝上的面："));
  printFace(upFace);
  Serial.println();

  if (flags & (1u << 3)) {               // 第 3 個開關 = 安裝過歪
    Serial.println(F("注意：感應器裝得太歪了（超過約 45 度），航向可能不準。請裝正一點。"));
  }

  Serial.println(F("接線正確，可以開始玩其他範例了！"));
}

void loop() {
  // 這個範例只要在開機時檢查一次，所以 loop 裡什麼都不做
}

// 把「朝上的面」編號印成看得懂的字
void printFace(uint8_t f) {
  switch (f) {
    case 0:  Serial.print(F("+X 面")); break;
    case 1:  Serial.print(F("+Y 面")); break;
    case 2:  Serial.print(F("+Z 面")); break;
    case 3:  Serial.print(F("-X 面")); break;
    case 4:  Serial.print(F("-Y 面")); break;
    case 5:  Serial.print(F("-Z 面")); break;
    default: Serial.print(F("不知道")); break;
  }
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
// （陀螺儀要先靜止約 0.5 秒才會準備好，所以範例都等 5 秒）
bool mbcWaitReady(uint16_t timeoutMs) {
  unsigned long start = millis();
  uint8_t flags;
  while (millis() - start < timeoutMs) {
    if (mbcRead(0x32, &flags, 1) && (flags & 0x01)) return true;
    delay(50);
  }
  return false;
}
