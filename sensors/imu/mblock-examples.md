<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 95_產品網站\mBlock範例產生器（node build.js）產生：改內容請改產生器的 sets/<產品>.js 再重建，不要手改本檔 -->
<!-- moved: /sensors/imu/mblock/mblock5.md -->
<p class="moved-note">這頁內容已整合到新的說明頁：<a href="/sensors/imu/mblock/mblock5.md">陀螺儀 IMU 九軸 › mBlock › mBlock 5</a></p>

# 🧱 mBlock 新手範例：IMU 陀螺儀

[🔙 回到IMU 陀螺儀產品頁](/sensors/imu/index.md)

這一頁是寫給**完全沒寫過程式**的人：用 mBlock 5 的積木在 **mBot** 上讀IMU 陀螺儀。5 個範例從「把數字變成聲音」開始，一步一步做到會自己跑的車子。每個範例都有積木圖、可下載的專案檔，還有一支做同樣事情的 Arduino 程式。

| 寫法 | 適合誰 | 怎麼打開範例檔 |
|---|---|---|
| 🟧 **mBlock 5 電腦版**（積木，上傳模式）＋ **MBC 感應器擴展 v2** | 學校、第一次用 mBot 的人 | 下載 `.mblock` → 打開 mBlock 5 → 「檔案 → 從電腦打開」→ 選檔案 → 右下切到「上傳」→ 按「上傳」 |
| 🔧 **Arduino IDE**（文字程式） | 想看積木背後長什麼樣、或自己寫程式的人 | 下載 `.ino` → 裝好 Makeblock 與 MBC_Sensors 函式庫（見 [MakeBlock 積木擴展](/makeblock.md)）→ 主板選 Arduino Uno → 上傳 |

> [!IMPORTANT]
> - 感應器要是 **MakeBlock 版、2026-10 之後的協定 v2 韌體**，經 **mBot 插座片**接 RJ25 連接埠；擴展要用 **v2.0.0**（舊韌體配舊版 v1 擴展，兩者不能混用）。
> - 主板是 **mBot（mCore）**。mBot Ranger、Orion、MegaPi 也能用，但要把「當 mBot 啟動時」換成那塊主板的起始積木、連接埠改成它能用的號碼（見 [支援的主板與連接埠](/makeblock.md)）；mBot2（CyberPi）沒有 RJ25，不支援。
> - 積木圖裡的控制與運算積木（forever、if…then…else、wait、set…to）以英文顯示，在 mBlock 的中文介面就是「重複無限次」「如果…那麼…否則」「等待…秒」「變數設為」。

## 開始之前

<figure class="ref-fig"><img src="/images/figures/imu/wiring-mbot.svg" alt="IMU 陀螺儀 × mBot 接線" loading="lazy"></figure>

感應器 → mBot 插座片 → 主板 RJ25 連接埠（S1 = 主機送、S2 = 主機收，UART 38400）。

1. **裝擴展**：到 [MakeBlock 積木擴展](/makeblock.md) 取得 **MBC Pro Sensors v2.0.0** 的 `.mext`，拖進 mBlock 5 視窗；左下「設備」加入 **mBot**，右下切到「上傳」。
2. **接線**：IMU 陀螺儀 → mBot 插座片 → mBot **連接埠 2**（這頁所有範例都接這一埠；換埠就把積木上的連接埠數字改掉）。
3. **先開機再讀**：每個範例一開始都「等待 1 秒」讓感應器開機；太早讀會讀到 -32768（還沒收到）。
4. IMU 平放固定在車身上，**板子絲印的 X 軸跟車子前後方向平行**；開機和「航向歸零」的那一下車子要完全靜止。從上往下看**右轉航向變大**（右轉 90 度 = 90），一直累加不會繞回 0。

## 這頁會用到的積木

每顆讀值積木就是感應器值表裡的一個「值編號」（跟 Arduino I2C 版的暫存器編號相同）；完整積木表在 [MakeBlock 積木擴展](/makeblock.md)。

| 積木 | 傳回 | 意思 |
|---|---|---|
| IMU 陀螺儀 連接埠 ［2］ 俯仰角 | -90~+90 ° | 板子絲印 X 軸相對水平面的仰角：X 軸朝上 +90、朝下 −90、水平 0（用重力向量 atan2 算） |
| IMU 陀螺儀 連接埠 ［2］ 陀螺 1D 航向 | -32768~32767 ° | 只繞轉向軸積分的航向，度，開機／歸零 = 0；可超過 360 |
| IMU 陀螺儀 連接埠 ［2］ 航向歸零 | — | 四個航向都歸零（zeroHeadings()）；EV3 另有原廠 WRITE 0x11 陀螺歸零 |

**這頁的範例**

1. [歪一邊音就變](#%E7%AF%84%E4%BE%8B-1%EF%BC%9A%E6%AD%AA%E4%B8%80%E9%82%8A%E9%9F%B3%E5%B0%B1%E8%AE%8A)
2. [轉頭看燈](#%E7%AF%84%E4%BE%8B-2%EF%BC%9A%E8%BD%89%E9%A0%AD%E7%9C%8B%E7%87%88)
3. [前傾後仰控制一個輪子](#%E7%AF%84%E4%BE%8B-3%EF%BC%9A%E5%89%8D%E5%82%BE%E5%BE%8C%E4%BB%B0%E6%8E%A7%E5%88%B6%E4%B8%80%E5%80%8B%E8%BC%AA%E5%AD%90)
4. [走直線自動修正](#%E7%AF%84%E4%BE%8B-4%EF%BC%9A%E8%B5%B0%E7%9B%B4%E7%B7%9A%E8%87%AA%E5%8B%95%E4%BF%AE%E6%AD%A3)
5. [直走、右轉 90 度、再直走](#%E7%AF%84%E4%BE%8B-5%EF%BC%9A%E7%9B%B4%E8%B5%B0%E3%80%81%E5%8F%B3%E8%BD%89-90-%E5%BA%A6%E3%80%81%E5%86%8D%E7%9B%B4%E8%B5%B0)

> [!NOTE]
> `.mblock` 是由產生器照 mBlock 5 的存檔格式產生的，**還沒在 mBlock 5 裡開啟驗證**（等擴展 v2 上架後補驗）。開不起來或積木顯示空白，請先確認 v2 擴展已拖進 mBlock、設備是 mBot，或照下面的積木圖自己拼（5 分鐘內拼得完）；也歡迎 [LINE 告訴我們](https://line.me/R/ti/p/@692vcvuk)。

---

## 範例 1：歪一邊音就變

**這個範例會做什麼**：mBot 沒有螢幕，這個範例把「俯仰角」（車頭抬高或壓低幾度，平放是 0）變成蜂鳴器的音高：平放 440 赫茲，抬頭音變高、低頭音變低。先學會「讀到數字」。

**要準備什麼**
- mBot（mCore 主板）＋ mBlock 5 電腦版
- IMU 陀螺儀（MakeBlock 版）＋ mBot 插座片

**怎麼接**：IMU 陀螺儀 → mBot 插座片 → mBot **連接埠 2**。感應器平放固定在車身上（不要晃動），板子絲印的 X 軸跟車子前後方向平行。程式開始時車子要靜止不動。

**下載程式**

| mBlock 5 專案 | 等價 Arduino 程式 |
|---|---|
| [ex01_tilt_tone.mblock](/examples/imu/mblock/ex01_tilt_tone.mblock) | [ex01_tilt_tone.ino](/examples/imu/mblock/ex01_tilt_tone/ex01_tilt_tone.ino) |

<details open>
<summary>🧱 mBlock 積木（照著拼就好）</summary>

<pre class="blocks">當 mBot 啟動時 :: events hat
wait (1) seconds
forever
  播放音頻 ((440) + ((IMU 陀螺儀 連接埠 [2 v] 俯仰角 :: #D9480F) * (4))) 赫茲，持續 (0.1) 秒 :: sound
end</pre>

</details>

<details>
<summary>🔧 等價 Arduino 程式（mBot）</summary>

```cpp
// IMU 陀螺儀 × mBlock 5　範例 1：歪一邊音就變（mBot 等價 Arduino 程式）
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
    buzzer.tone((440 + (mbcRead(mbc_port2, 0x06) * 4)), 100);   // 音高 ＝ 440 ＋ 俯仰角 × 4 赫茲，響 0.1 秒
  }
}

void loop() {}   // 這裡什麼都沒有：上面的積木不會結束
```

</details>

**一步一步看懂程式**
1. 等 1 秒讓感應器開機
2. 一直重複
   - 音高 ＝ 440 ＋ 俯仰角 × 4 赫茲，響 0.1 秒

**跑起來應該看到什麼**：車子平放：440 赫茲。把車頭慢慢抬高，音越來越高（抬 45 度約 620 赫茲）；壓低車頭音變低。

**常見問題**
- **聽不到聲音？** → mBot 的蜂鳴器在主板上，聲音不大；把耳朵靠近，或把 0.1 秒改成 0.3 秒。
- **抬頭音反而變低？** → 正常，正負跟安裝方向有關；把感應器轉 180 度裝，或把 × 4 改成 × -4。
- **車子沒動，航向卻一直慢慢變？** → 開機或歸零的那一下車子要完全靜止。把車子放好，拔掉感應器重插一次再試。
- **什麼都沒反應、或讀值一直是 -32768？** → 連接埠選錯（這頁都接連接埠 2）、插座片沒插緊、或感應器不是 MakeBlock 版。先用「MBC 共用 → 連線正常？」積木確認；「協議版本」讀到 2 才是新韌體（舊韌體請改用舊版擴展 v1）。
- **上傳失敗、或積木是空白的？** → 確認右下角切到「上傳」模式、擴展 v2 已拖進 mBlock、USB 線接的是 mBot 主板。

---

## 範例 2：轉頭看燈

**這個範例會做什麼**：程式開始先「航向歸零」，之後用手轉動車子：右轉超過 20 度亮藍燈、左轉超過 20 度亮紅燈、對著原來的方向亮綠燈。這是後面「走直線」「轉 90 度」的判斷骨架。

**要準備什麼**
- mBot（mCore 主板）＋ mBlock 5 電腦版
- IMU 陀螺儀（MakeBlock 版）＋ mBot 插座片

**怎麼接**：IMU 陀螺儀 → mBot 插座片 → mBot **連接埠 2**。感應器平放固定在車身上（不要晃動），板子絲印的 X 軸跟車子前後方向平行。程式開始時車子要靜止不動。

**下載程式**

| mBlock 5 專案 | 等價 Arduino 程式 |
|---|---|
| [ex02_led_heading.mblock](/examples/imu/mblock/ex02_led_heading.mblock) | [ex02_led_heading.ino](/examples/imu/mblock/ex02_led_heading/ex02_led_heading.ino) |

<details open>
<summary>🧱 mBlock 積木（照著拼就好）</summary>

<pre class="blocks">當 mBot 啟動時 :: events hat
wait (1) seconds
IMU 陀螺儀 連接埠 [2 v] 航向歸零 :: #D9480F
forever
  if &lt;(IMU 陀螺儀 連接埠 [2 v] 陀螺 1D 航向 :: #D9480F) &gt; (20)&gt; then
    LED 燈位置 [全部 v] 的三原色數值為 紅 (0) 綠 (0) 藍 (255) :: looks
  else
    if &lt;(IMU 陀螺儀 連接埠 [2 v] 陀螺 1D 航向 :: #D9480F) &lt; (-20)&gt; then
      LED 燈位置 [全部 v] 的三原色數值為 紅 (255) 綠 (0) 藍 (0) :: looks
    else
      LED 燈位置 [全部 v] 的三原色數值為 紅 (0) 綠 (255) 藍 (0) :: looks
    end
  end
end</pre>

</details>

<details>
<summary>🔧 等價 Arduino 程式（mBot）</summary>

```cpp
// IMU 陀螺儀 × mBlock 5　範例 2：轉頭看燈（mBot 等價 Arduino 程式）
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
  mbcWrite(mbc_port2, 0xE0);   // 航向歸零：現在的方向就是 0
  while (1) {   // 一直重複
    if ((mbcRead(mbc_port2, 0x03) > 20)) {   // 如果陀螺 1D 航向大於 20
      rgbled.setColor(0, 0, 0, 255); rgbled.show();   // 右轉超過 20 度：藍燈
    } else {
      if ((mbcRead(mbc_port2, 0x03) < -20)) {   // 否則，如果陀螺 1D 航向小於 -20
        rgbled.setColor(0, 255, 0, 0); rgbled.show();   // 左轉超過 20 度：紅燈
      } else {
        rgbled.setColor(0, 0, 255, 0); rgbled.show();   // 對著原來的方向：綠燈
      }
    }
  }
}

void loop() {}   // 這裡什麼都沒有：上面的積木不會結束
```

</details>

**一步一步看懂程式**
1. 等 1 秒讓感應器開機
2. 航向歸零：現在的方向就是 0
3. 一直重複
   - 如果陀螺 1D 航向大於 20
      - 右轉超過 20 度：藍燈
      - 否則：
         - 否則，如果陀螺 1D 航向小於 -20
            - 左轉超過 20 度：紅燈
            - 否則：
               - 對著原來的方向：綠燈

**跑起來應該看到什麼**：開始時綠燈；把車子往右轉，過 20 度變藍燈；轉回來又變綠；往左轉過 20 度變紅燈。轉一整圈回來是 360，不會回到 0，所以還是藍燈。

**常見問題**
- **右轉亮的是紅燈？** → 感應器裝反了（上下顛倒）。翻過來裝，或把程式裡的 20 和 -20 對調。
- **車子沒動，航向卻一直慢慢變？** → 開機或歸零的那一下車子要完全靜止。把車子放好，拔掉感應器重插一次再試。
- **什麼都沒反應、或讀值一直是 -32768？** → 連接埠選錯（這頁都接連接埠 2）、插座片沒插緊、或感應器不是 MakeBlock 版。先用「MBC 共用 → 連線正常？」積木確認；「協議版本」讀到 2 才是新韌體（舊韌體請改用舊版擴展 v1）。
- **上傳失敗、或積木是空白的？** → 確認右下角切到「上傳」模式、擴展 v2 已拖進 mBlock、USB 線接的是 mBot 主板。

---

## 範例 3：前傾後仰控制一個輪子

**這個範例會做什麼**：把「俯仰角 × 2」直接當左輪的動力：車頭抬高輪子往前轉、壓低倒轉、平放不動，歪越多轉越快。先把車子架高看。

**要準備什麼**
- mBot（mCore 主板）＋ mBlock 5 電腦版
- mBot 兩輪小車（左輪接 M1、右輪接 M2，出廠就是這樣接）
- IMU 陀螺儀（MakeBlock 版）＋ mBot 插座片

**怎麼接**：IMU 陀螺儀 → mBot 插座片 → mBot **連接埠 2**。感應器平放固定在車身上（不要晃動），板子絲印的 X 軸跟車子前後方向平行。程式開始時車子要靜止不動。

**下載程式**

| mBlock 5 專案 | 等價 Arduino 程式 |
|---|---|
| [ex03_tilt_one_wheel.mblock](/examples/imu/mblock/ex03_tilt_one_wheel.mblock) | [ex03_tilt_one_wheel.ino](/examples/imu/mblock/ex03_tilt_one_wheel/ex03_tilt_one_wheel.ino) |

<details open>
<summary>🧱 mBlock 積木（照著拼就好）</summary>

<pre class="blocks">當 mBot 啟動時 :: events hat
wait (1) seconds
forever
  左輪動力 ((IMU 陀螺儀 連接埠 [2 v] 俯仰角 :: #D9480F) * (2)) %，右輪動力 (0) % :: motion
end</pre>

</details>

<details>
<summary>🔧 等價 Arduino 程式（mBot）</summary>

```cpp
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
```

</details>

**一步一步看懂程式**
1. 等 1 秒讓感應器開機
2. 一直重複
   - 左輪動力 ＝ 俯仰角 × 2，右輪 0

**跑起來應該看到什麼**：平放左輪不動；車頭抬高 30 度左輪用 60 的動力往前轉；壓低車頭倒轉。

**常見問題**
- **車子往反方向轉、越修越歪？** → 左右輪的「＋」「－」對調（或把左輪、右輪的動力互換）。mBot 出廠左輪 M1、右輪 M2，自己接線的請確認。
- **平放輪子還微微轉？** → 俯仰角不一定剛好 0。加一個「如果 絕對值 小於 3 就動力 0」。
- **車子沒動，航向卻一直慢慢變？** → 開機或歸零的那一下車子要完全靜止。把車子放好，拔掉感應器重插一次再試。
- **什麼都沒反應、或讀值一直是 -32768？** → 連接埠選錯（這頁都接連接埠 2）、插座片沒插緊、或感應器不是 MakeBlock 版。先用「MBC 共用 → 連線正常？」積木確認；「協議版本」讀到 2 才是新韌體（舊韌體請改用舊版擴展 v1）。
- **上傳失敗、或積木是空白的？** → 確認右下角切到「上傳」模式、擴展 v2 已拖進 mBlock、USB 線接的是 mBot 主板。

---

## 範例 4：走直線自動修正

**這個範例會做什麼**：兩個輪子不可能完全一樣快，車子直走久了會歪。先航向歸零，然後用「航向 × 2」當修正力量：往右歪（航向變大）左輪減速、右輪加速，把車頭拉回來。

**要準備什麼**
- mBot（mCore 主板）＋ mBlock 5 電腦版
- mBot 兩輪小車（左輪接 M1、右輪接 M2，出廠就是這樣接）
- IMU 陀螺儀（MakeBlock 版）＋ mBot 插座片

**怎麼接**：IMU 陀螺儀 → mBot 插座片 → mBot **連接埠 2**。感應器平放固定在車身上（不要晃動），板子絲印的 X 軸跟車子前後方向平行。程式開始時車子要靜止不動。

**下載程式**

| mBlock 5 專案 | 等價 Arduino 程式 |
|---|---|
| [ex04_drive_straight.mblock](/examples/imu/mblock/ex04_drive_straight.mblock) | [ex04_drive_straight.ino](/examples/imu/mblock/ex04_drive_straight/ex04_drive_straight.ino) |

<details open>
<summary>🧱 mBlock 積木（照著拼就好）</summary>

<pre class="blocks">當 mBot 啟動時 :: events hat
wait (1) seconds
IMU 陀螺儀 連接埠 [2 v] 航向歸零 :: #D9480F
forever
  set [修正 v] to ((IMU 陀螺儀 連接埠 [2 v] 陀螺 1D 航向 :: #D9480F) * (2))
  左輪動力 ((40) - (修正)) %，右輪動力 ((40) + (修正)) % :: motion
end</pre>

</details>

<details>
<summary>🔧 等價 Arduino 程式（mBot）</summary>

```cpp
// IMU 陀螺儀 × mBlock 5　範例 4：走直線自動修正（mBot 等價 Arduino 程式）
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
float fix = 0;                // 變數：修正

// 左／右輪動力 -100~100（正 = 往前）：mBot 的左輪裝反，所以 M1 給負的；% 換算成 PWM 0~255
void wheels(int l, int r) {
  l = constrain(l, -100, 100); r = constrain(r, -100, 100);
  motorL.run(-l * 255 / 100);
  motorR.run(r * 255 / 100);
}

void setup() {   // 跟 mBlock 產生的程式一樣，積木全部在 setup() 裡跑
  mbc_port2.begin();
  delay(1000);   // 等 1 秒讓感應器開機
  mbcWrite(mbc_port2, 0xE0);   // 航向歸零：現在的方向就是 0
  while (1) {   // 一直重複
    fix = (mbcRead(mbc_port2, 0x03) * 2);   // 修正 ＝ 陀螺 1D 航向 × 2
    wheels((40 - fix), (40 + fix));   // 左輪 ＝ 40 − 修正、右輪 ＝ 40 ＋ 修正：往右歪（航向為正）就左輪慢、右輪快
  }
}

void loop() {}   // 這裡什麼都沒有：上面的積木不會結束
```

</details>

**一步一步看懂程式**
1. 等 1 秒讓感應器開機
2. 航向歸零：現在的方向就是 0
3. 一直重複
   - 修正 ＝ 陀螺 1D 航向 × 2
   - 左輪 ＝ 40 − 修正、右輪 ＝ 40 ＋ 修正：往右歪（航向為正）就左輪慢、右輪快

**跑起來應該看到什麼**：車子走直線；用手把車頭推歪，它會自己轉回原來的方向繼續直走。

**常見問題**
- **越走越歪、甚至原地打轉？** → 修正方向相反了（感應器裝反）：把「40 − 修正」和「40 ＋ 修正」對調。
- **左右抖動？** → 把 × 2 改小（例如 × 1）。
- **車子沒動，航向卻一直慢慢變？** → 開機或歸零的那一下車子要完全靜止。把車子放好，拔掉感應器重插一次再試。
- **什麼都沒反應、或讀值一直是 -32768？** → 連接埠選錯（這頁都接連接埠 2）、插座片沒插緊、或感應器不是 MakeBlock 版。先用「MBC 共用 → 連線正常？」積木確認；「協議版本」讀到 2 才是新韌體（舊韌體請改用舊版擴展 v1）。
- **上傳失敗、或積木是空白的？** → 確認右下角切到「上傳」模式、擴展 v2 已拖進 mBlock、USB 線接的是 mBot 主板。

---

## 範例 5：直走、右轉 90 度、再直走

**這個範例會做什麼**：比賽常用的動作：直走 2 秒 → 原地右轉到航向 90 度 → 嗶一聲 → 再直走 2 秒。靠陀螺儀轉彎比「轉幾秒」準很多：用「等到航向大於 88」等它轉到位（留 2 度給煞車）。這個範例沒有「一直重複」，做完就停。

**要準備什麼**
- mBot（mCore 主板）＋ mBlock 5 電腦版
- mBot 兩輪小車（左輪接 M1、右輪接 M2，出廠就是這樣接）
- IMU 陀螺儀（MakeBlock 版）＋ mBot 插座片

**怎麼接**：IMU 陀螺儀 → mBot 插座片 → mBot **連接埠 2**。感應器平放固定在車身上（不要晃動），板子絲印的 X 軸跟車子前後方向平行。程式開始時車子要靜止不動。

**下載程式**

| mBlock 5 專案 | 等價 Arduino 程式 |
|---|---|
| [ex05_turn_90.mblock](/examples/imu/mblock/ex05_turn_90.mblock) | [ex05_turn_90.ino](/examples/imu/mblock/ex05_turn_90/ex05_turn_90.ino) |

<details open>
<summary>🧱 mBlock 積木（照著拼就好）</summary>

<pre class="blocks">當 mBot 啟動時 :: events hat
wait (1) seconds
IMU 陀螺儀 連接埠 [2 v] 航向歸零 :: #D9480F
左輪動力 (40) %，右輪動力 (40) % :: motion
wait (2) seconds
停止移動 :: motion
wait (0.5) seconds
IMU 陀螺儀 連接埠 [2 v] 航向歸零 :: #D9480F
左輪動力 (25) %，右輪動力 (-25) % :: motion
wait until &lt;(IMU 陀螺儀 連接埠 [2 v] 陀螺 1D 航向 :: #D9480F) &gt; (88)&gt;
停止移動 :: motion
播放音頻 (880) 赫茲，持續 (0.3) 秒 :: sound
左輪動力 (40) %，右輪動力 (40) % :: motion
wait (2) seconds
停止移動 :: motion</pre>

</details>

<details>
<summary>🔧 等價 Arduino 程式（mBot）</summary>

```cpp
// IMU 陀螺儀 × mBlock 5　範例 5：直走、右轉 90 度、再直走（mBot 等價 Arduino 程式）
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
  mbcWrite(mbc_port2, 0xE0);   // 航向歸零：現在的方向就是 0
  wheels(40, 40);   // 直走
  delay(2000);   // 走 2 秒
  wheels(0, 0);   // 停一下
  delay(500);   // 等 0.5 秒讓車子靜止
  mbcWrite(mbc_port2, 0xE0);   // 航向歸零：現在的方向就是 0
  wheels(25, -25);   // 左輪往前、右輪往後：原地右轉（慢一點比較準）
  while (!(mbcRead(mbc_port2, 0x03) > 88)) delay(5);   // 等到航向大於 88 度
  wheels(0, 0);   // 轉到 90 度：停車
  buzzer.tone(880, 300);   // 嗶一聲
  wheels(40, 40);   // 再直走
  delay(2000);   // 走 2 秒
  wheels(0, 0);   // 停車
}

void loop() {}   // 這裡什麼都沒有：上面的積木不會結束
```

</details>

**一步一步看懂程式**
1. 等 1 秒讓感應器開機
2. 航向歸零：現在的方向就是 0
3. 直走
4. 走 2 秒
5. 停一下
6. 等 0.5 秒讓車子靜止
7. 航向歸零：現在的方向就是 0
8. 左輪往前、右輪往後：原地右轉（慢一點比較準）
9. 等到航向大於 88 度
10. 轉到 90 度：停車
11. 嗶一聲
12. 再直走
13. 走 2 秒
14. 停車

**跑起來應該看到什麼**：車子直走 2 秒、原地右轉約 90 度、嗶一聲、再走 2 秒停下，走出一個 L 形。

**常見問題**
- **每次都轉過頭幾度？** → 把 88 改成 85，或把轉速 25 再調慢。
- **一直轉不停？** → 右轉時航向沒有變大（感應器裝反）：把「25、-25」改成「-25、25」。
- **車子沒動，航向卻一直慢慢變？** → 開機或歸零的那一下車子要完全靜止。把車子放好，拔掉感應器重插一次再試。
- **什麼都沒反應、或讀值一直是 -32768？** → 連接埠選錯（這頁都接連接埠 2）、插座片沒插緊、或感應器不是 MakeBlock 版。先用「MBC 共用 → 連線正常？」積木確認；「協議版本」讀到 2 才是新韌體（舊韌體請改用舊版擴展 v1）。
- **上傳失敗、或積木是空白的？** → 確認右下角切到「上傳」模式、擴展 v2 已拖進 mBlock、USB 線接的是 mBot 主板。

---

## 下一步

- 全部 IMU 陀螺儀積木、各主板可用的連接埠：[MakeBlock 積木擴展](/makeblock.md)；這款產品的 mBlock 積木表：[IMU 陀螺儀 × mBlock 5](/sensors/imu/mblock.md)
- 同樣的 5 個範例在樂高上怎麼寫：[SPIKE 新手範例](/sensors/imu/ev3-examples.md)、[EV3 新手範例](/sensors/imu/ev3-examples.md)
- 想用 Arduino 直接接感應器（I2C 版，不經 mBot）：[Arduino 範例](/sensors/imu/arduino-examples.md)
