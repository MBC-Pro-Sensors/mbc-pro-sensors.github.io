// IMU 陀螺儀 × mBlock 5　範例 3：前傾後仰控制一個輪子（mBot 等價 Arduino 程式）
// 接線：IMU 陀螺儀 → mBot 插座片 → mBot 連接埠 2。感應器平放固定在車身上（不要晃動），板子絲印的 X 軸跟車子前後方向平行。程式開始時車子要靜止不動。
// 跟 mBlock 積木範例做的事完全一樣，給想看「積木背後長什麼樣」或用 Arduino IDE 的人。
// 需要的函式庫：
//   1. Makeblock（Arduino IDE「函式庫管理員」搜尋 Makeblock，或 mBlock 5 安裝資料夾裡的 Makeblock 函式庫）
//   2. MBC_Sensors v2：擴展 .mext 解壓後的 src/ 四個檔（MBC_Sensors.h/.cpp、mbc_proto.h、mbc_ids.h），
//      放進 文件/Arduino/libraries/MBC_Sensors/ 就能 #include <MBC_Sensors.h>（https://mbc-pro-sensors.github.io/makeblock.html）
// 主板選 Arduino Uno（mBot 的 mCore 就是 Uno），上傳後拔掉 USB 用電池跑。

#include <MeMCore.h>            // mBot（mCore）：馬達、LED、蜂鳴器
#include <SoftwareSerial.h>
#include <MBC_Sensors.h>        // MBC 感應器 v2（跟 mBlock 擴展同一份函式庫）

MBCPort mbc_port2(2);           // 感應器接在 mBot 連接埠 2
MeDCMotor motorL(9);           // 左輪（M1）
MeDCMotor motorR(10);          // 右輪（M2）
MeRGBLed rgbled(7, 2);         // 板載 RGB LED（2 顆）
MeBuzzer buzzer;               // 板載蜂鳴器

// 左／右輪動力 -100~100（正 = 往前）：mBot 的左輪裝反，所以 M1 給負的；% 換算成 PWM 0~255
void wheels(int l, int r) {
  l = constrain(l, -100, 100); r = constrain(r, -100, 100);
  motorL.run(-l * 255 / 100);
  motorR.run(r * 255 / 100);
}

void setup() {   // 跟 mBlock 產生的程式一樣，積木全部在 setup() 裡跑
  mbc_port2.begin();
  delay(1000);   // 等 1 秒讓感應器開機
  while (1) {   // 一直重複
    wheels((mbcRead(mbc_port2, 0x06) * 2), 0);   // 左輪動力 ＝ 俯仰角 × 2，右輪 0
  }
}

void loop() {}   // 這裡什麼都沒有：上面的積木不會結束
