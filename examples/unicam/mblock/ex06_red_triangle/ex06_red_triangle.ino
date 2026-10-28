// 神攝手 × mBlock 5　範例 6：追紅色三角形（mBot 等價 Arduino 程式）
// 接線：神攝手 → mBot 插座片 → mBot 連接埠 1。鏡頭朝前、裝在車頭。先照上面「開始之前」在相機上把目標 1 教成紅色（十字對準紅色三角形）。
// 跟 mBlock 積木範例做的事完全一樣，給想看「積木背後長什麼樣」或用 Arduino IDE 的人。
// 需要的函式庫：
//   1. Makeblock（Arduino IDE「函式庫管理員」搜尋 Makeblock，或 mBlock 5 安裝資料夾裡的 Makeblock 函式庫）
//   2. MBC_Sensors v2：擴展 .mext 解壓後的 src/ 四個檔（MBC_Sensors.h/.cpp、mbc_proto.h、mbc_ids.h），
//      放進 文件/Arduino/libraries/MBC_Sensors/ 就能 #include <MBC_Sensors.h>（https://mbc-pro-sensors.github.io/makeblock.html）
// 主板選 Arduino Uno（mBot 的 mCore 就是 Uno），上傳後拔掉 USB 用電池跑。

#include <MeMCore.h>            // mBot（mCore）：馬達、LED、蜂鳴器
#include <SoftwareSerial.h>
#include <MBC_Sensors.h>        // MBC 感應器 v2（跟 mBlock 擴展同一份函式庫）

MBCPort mbc_port1(1);           // 感應器接在 mBot 連接埠 1
MeDCMotor motorL(9);           // 左輪（M1）
MeDCMotor motorR(10);          // 右輪（M2）
MeRGBLed rgbled(7, 2);         // 板載 RGB LED（2 顆）
MeBuzzer buzzer;               // 板載蜂鳴器
float which = 0;              // 變數：第幾個
float turn = 0;               // 變數：轉向

// 左／右輪動力 -100~100（正 = 往前）：mBot 的左輪裝反，所以 M1 給負的；% 換算成 PWM 0~255
void wheels(int l, int r) {
  l = constrain(l, -100, 100); r = constrain(r, -100, 100);
  motorL.run(-l * 255 / 100);
  motorR.run(r * 255 / 100);
}

void setup() {   // 跟 mBlock 產生的程式一樣，積木全部在 setup() 裡跑
  mbc_port1.begin();
  delay(2000);   // 等 2 秒讓相機開機
  ucSetMode(mbc_port1, 51);   // 相機切到「物體偵測 ① 物體清單」（每個物體給色號、形狀、X、Y）
  while (1) {   // 一直重複
    if (ucHasObj(mbc_port1, 1, 3)) {   // 如果有顏色 1 的三角形物體
      rgbled.setColor(0, 0, 255, 0); rgbled.show();   // 看到了：綠燈
      which = ucObjFind(mbc_port1, 1, 3);   // 第幾個 ＝ 顏色 1 的三角形是第幾個物體（物體 1 最大；1 ~ 8）
      turn = ((ucObj(mbc_port1, which, 3) - 80) * 0.4);   // 轉向 ＝（第「第幾個」個物體 x − 80）× 0.4（−32 ~ +32）
      wheels((25 + turn), (25 - turn));   // 左輪 ＝ 25 ＋ 轉向、右輪 ＝ 25 − 轉向：三角形在右（正）就往右轉
    } else {
      wheels(0, 0);   // 沒看到紅色三角形：停車
      rgbled.setColor(0, 255, 0, 0); rgbled.show();   // 紅燈
    }
  }
}

void loop() {}   // 這裡什麼都沒有：上面的積木不會結束
