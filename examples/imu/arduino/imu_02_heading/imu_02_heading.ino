/*
 * 陀螺儀 IMU × Arduino　範例 2：我轉了幾度？（航向角）
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   把感應器放在桌上慢慢轉，電腦會一直告訴你「現在轉到幾度」。
 *     從上往下看「順時針」（向右轉）→ 數字變大
 *     從上往下看「逆時針」（向左轉）→ 數字變小（可以變成負數）
 *   數字會一直累加：右轉兩圈就是 720 度，不會跳回 0。
 *   另外也印出「0 ~ 359 度」的寫法，像時鐘一樣轉一圈又回到 0。
 *   在序列埠監控窗輸入 z 按 Enter，就會把角度歸零（現在的方向 = 0 度）。
 *
 * 接線（Arduino UNO / Nano）：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。
 *   等看到「準備好了」再開始轉。
 *
 * 小提醒：
 *   陀螺儀開機時會把「當時的方向」當成 0 度。
 */

#include <Wire.h>

const uint8_t SENSOR_ADDR = 0x6A;        // 陀螺儀的門牌號碼

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);

  Serial.println(F("請把感應器放在桌上，不要動它……"));
  if (!mbcWaitReady(5000)) {
    Serial.println(F("找不到感應器，或感應器一直在晃動。請先跑範例 1 檢查。"));
    while (true) {}
  }
  Serial.println(F("準備好了！慢慢轉動感應器看看。輸入 z 按 Enter 可以歸零。"));
}

void loop() {
  // ---- 1. 收到 z 就歸零 ----
  if (Serial.available()) {
    char c = Serial.read();              // 讀一個你打的字
    if (c == 'z' || c == 'Z') {
      mbcWrite(0x10, 0x01);              // 把 0x01 寫進第 0x10 號格子 = 「航向歸零」
      delay(50);                         // 歸零後要等一下（至少 30 毫秒）再讀，才會讀到新的 0
      Serial.println(F("--- 已歸零！現在的方向就是 0 度 ---"));
    }
  }

  // ---- 2. 讀航向：第 0x11 號格子，共 6 個數字（3 種航向，每種 2 個數字）----
  uint8_t d[6];
  if (!mbcRead(0x11, d, 6)) {            // 讀不到就等一下再試
    delay(100);
    return;
  }

  // 第 2、3 個數字是「立體補償航向（Gyro3D）」，最推薦用這個
  // 兩個數字拼成一個「可以有負號」的數：前面的是高位、後面的是低位
  int16_t heading = (int16_t)(((uint16_t)d[2] << 8) | d[3]);

  // 換成 0 ~ 359 度（像時鐘一樣，轉一圈回到 0）
  int angle360 = ((heading % 360) + 360) % 360;

  Serial.print(F("累加角度："));
  Serial.print(heading);
  Serial.print(F(" 度　｜　0~359 寫法："));
  Serial.print(angle360);
  Serial.println(F(" 度"));

  delay(200);                            // 👉 可以改：每 0.2 秒印一次
}

// ============================================================
// 以下是「通訊小幫手」：負責跟感應器講話。
// 所有 MBC 範例都一樣，不用改，也不用看懂。
// ============================================================

void mbcBegin() {
  Wire.begin();
#if defined(WIRE_HAS_TIMEOUT)
  Wire.setWireTimeout(25000, true);      // 線沒接好時，不會讓 Arduino 當機卡住
#endif
}

bool mbcRead(uint8_t reg, uint8_t *buf, uint8_t n) {
  Wire.beginTransmission(SENSOR_ADDR);
  Wire.write(reg);
  if (Wire.endTransmission() != 0) return false;
  if (Wire.requestFrom(SENSOR_ADDR, n) != n) return false;
  for (uint8_t i = 0; i < n; i++) buf[i] = Wire.read();
  return true;
}

bool mbcWrite(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(SENSOR_ADDR);
  Wire.write(reg);
  Wire.write(value);
  return Wire.endTransmission() == 0;
}

bool mbcWaitReady(uint16_t timeoutMs) {
  unsigned long start = millis();
  uint8_t flags;
  while (millis() - start < timeoutMs) {
    if (mbcRead(0x32, &flags, 1) && (flags & 0x01)) return true;
    delay(50);
  }
  return false;
}
