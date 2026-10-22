<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 95_產品網站\mBlock範例產生器（node build.js）產生：改內容請改產生器的 sets/<產品>.js 再重建，不要手改本檔 -->
# 🧱 mBlock 新手範例：測距者 2 路

[🔙 回到測距者 2 路產品頁](/sensors/tof2/index.md)

這一頁是寫給**完全沒寫過程式**的人：用 mBlock 5 的積木在 **mBot** 上讀測距者 2 路。5 個範例從「把數字變成聲音」開始，一步一步做到會自己跑的車子。每個範例都有積木圖、可下載的專案檔，還有一支做同樣事情的 Arduino 程式。

| 寫法 | 適合誰 | 怎麼打開範例檔 |
|---|---|---|
| 🟧 **mBlock 5 電腦版**（積木，上傳模式）＋ **MBC 感應器擴展 v2** | 學校、第一次用 mBot 的人 | 下載 `.mblock` → 打開 mBlock 5 → 「檔案 → 從電腦打開」→ 選檔案 → 右下切到「上傳」→ 按「上傳」 |
| 🔧 **Arduino IDE**（文字程式） | 想看積木背後長什麼樣、或自己寫程式的人 | 下載 `.ino` → 裝好 Makeblock 與 MBC_Sensors 函式庫（見 [MakeBlock 積木擴展](/makeblock.md)）→ 主板選 Arduino Uno → 上傳 |

> [!IMPORTANT]
> - 感應器要是 **MakeBlock 版、2026-10 之後的協定 v2 韌體**，經 **mBot 插座片**接 RJ25 連接埠；擴展要用 **v2.0.0**（舊韌體配舊版 v1 擴展，兩者不能混用）。
> - 主板是 **mBot（mCore）**。mBot Ranger、Orion、MegaPi 也能用，但要把「當 mBot 啟動時」換成那塊主板的起始積木、連接埠改成它能用的號碼（見 [支援的主板與連接埠](/makeblock.md)）；mBot2（CyberPi）沒有 RJ25，不支援。
> - 積木圖裡的控制與運算積木（forever、if…then…else、wait、set…to）以英文顯示，在 mBlock 的中文介面就是「重複無限次」「如果…那麼…否則」「等待…秒」「變數設為」。

## 開始之前

<figure class="ref-fig"><img src="/images/figures/tof2/wiring-mbot.svg" alt="測距者 2 路 × mBot 接線" loading="lazy"></figure>

感應器 → mBot 插座片 → 主板 RJ25 連接埠（S1 = 主機送、S2 = 主機收，UART 38400）。

1. **裝擴展**：到 [MakeBlock 積木擴展](/makeblock.md) 取得 **MBC Pro Sensors v2.0.0** 的 `.mext`，拖進 mBlock 5 視窗；左下「設備」加入 **mBot**，右下切到「上傳」。
2. **接線**：測距者 → mBot 插座片 → mBot **連接埠 3**（這頁所有範例都接這一埠；換埠就把積木上的連接埠數字改掉）。
3. **先開機再讀**：每個範例一開始都「等待 1 秒」讓感應器開機；太早讀會讀到 -32768（還沒收到）。
4. 測距者 2 路不用校準。左、右兩顆雷射各看自己前方，**站在車後往前看**，左邊那顆是「左距離」；120 公分以外都讀 **120**。

## 這頁會用到的積木

每顆讀值積木就是感應器值表裡的一個「值編號」（跟 Arduino I2C 版的暫存器編號相同）；完整積木表在 [MakeBlock 積木擴展](/makeblock.md)。

| 積木 | 傳回 | 意思 |
|---|---|---|
| 測距者 2 路 連接埠 ［3］ 較近距離 cm | 0~120 cm | 左右兩顆取較近的 0~120 cm |
| 測距者 2 路 連接埠 ［3］ 左距離 cm | 0~120 cm | 左邊那顆的距離 0~120 cm（站在車後往前看，左 = 第 0 顆），120 = 前面沒東西／那顆失效 |
| 測距者 2 路 連接埠 ［3］ 右距離 cm | 0~120 cm | 右邊那顆的距離 0~120 cm（右 = 第 1 顆），120 = 前面沒東西／那顆失效 |

**這頁的範例**

1. [距離變音高（空氣琴）](#%E7%AF%84%E4%BE%8B-1%EF%BC%9A%E8%B7%9D%E9%9B%A2%E8%AE%8A%E9%9F%B3%E9%AB%98%EF%BC%88%E7%A9%BA%E6%B0%A3%E7%90%B4%EF%BC%89)
2. [紅黃綠距離燈](#%E7%AF%84%E4%BE%8B-2%EF%BC%9A%E7%B4%85%E9%BB%83%E7%B6%A0%E8%B7%9D%E9%9B%A2%E7%87%88)
3. [距離控制一個輪子](#%E7%AF%84%E4%BE%8B-3%EF%BC%9A%E8%B7%9D%E9%9B%A2%E6%8E%A7%E5%88%B6%E4%B8%80%E5%80%8B%E8%BC%AA%E5%AD%90)
4. [開到牆前停車](#%E7%AF%84%E4%BE%8B-4%EF%BC%9A%E9%96%8B%E5%88%B0%E7%89%86%E5%89%8D%E5%81%9C%E8%BB%8A)
5. [哪邊近就往另一邊轉](#%E7%AF%84%E4%BE%8B-5%EF%BC%9A%E5%93%AA%E9%82%8A%E8%BF%91%E5%B0%B1%E5%BE%80%E5%8F%A6%E4%B8%80%E9%82%8A%E8%BD%89)

> [!NOTE]
> `.mblock` 是由產生器照 mBlock 5 的存檔格式產生的，**還沒在 mBlock 5 裡開啟驗證**（等擴展 v2 上架後補驗）。開不起來或積木顯示空白，請先確認 v2 擴展已拖進 mBlock、設備是 mBot，或照下面的積木圖自己拼（5 分鐘內拼得完）；也歡迎 [LINE 告訴我們](https://line.me/R/ti/p/@692vcvuk)。

---

## 範例 1：距離變音高（空氣琴）

**這個範例會做什麼**：mBot 沒有螢幕，這個範例把「較近距離」（0 ~ 120 公分）變成蜂鳴器的音高：手越靠近音越低、越遠音越高。先學會「讀到數字」。

**要準備什麼**
- mBot（mCore 主板）＋ mBlock 5 電腦版
- 測距者 2 路（MakeBlock 版）＋ mBot 插座片

**怎麼接**：測距者 → mBot 插座片 → mBot **連接埠 3**。感應器裝在車頭、兩顆雷射朝正前方。

**下載程式**

| mBlock 5 專案 | 等價 Arduino 程式 |
|---|---|
| [ex01_theremin.mblock](/examples/tof2/mblock/ex01_theremin.mblock) | [ex01_theremin.ino](/examples/tof2/mblock/ex01_theremin/ex01_theremin.ino) |

<details open>
<summary>🧱 mBlock 積木（照著拼就好）</summary>

<pre class="blocks">當 mBot 啟動時 :: events hat
wait (1) seconds
forever
  播放音頻 ((200) + ((測距者 2 路 連接埠 [3 v] 較近距離 cm :: #0A9E8F) * (10))) 赫茲，持續 (0.1) 秒 :: sound
end</pre>

</details>

<details>
<summary>🔧 等價 Arduino 程式（mBot）</summary>

```cpp
// 測距者 2 路 × mBlock 5　範例 1：距離變音高（空氣琴）（mBot 等價 Arduino 程式）
// 接線：測距者 → mBot 插座片 → mBot 連接埠 3。感應器裝在車頭、兩顆雷射朝正前方。
// 跟 mBlock 積木範例做的事完全一樣，給想看「積木背後長什麼樣」或用 Arduino IDE 的人。
// 需要的函式庫：
//   1. Makeblock（Arduino IDE「函式庫管理員」搜尋 Makeblock，或 mBlock 5 安裝資料夾裡的 Makeblock 函式庫）
//   2. MBC_Sensors v2：擴展 .mext 解壓後的 src/ 四個檔（MBC_Sensors.h/.cpp、mbc_proto.h、mbc_ids.h），
//      放進 文件/Arduino/libraries/MBC_Sensors/ 就能 #include <MBC_Sensors.h>（https://mbc-pro-sensors.github.io/makeblock.html）
// 主板選 Arduino Uno（mBot 的 mCore 就是 Uno），上傳後拔掉 USB 用電池跑。

#include <MeMCore.h>            // mBot（mCore）：馬達、LED、蜂鳴器
#include <SoftwareSerial.h>
#include <MBC_Sensors.h>        // MBC 感應器 v2（跟 mBlock 擴展同一份函式庫）

MBCPort mbc_port3(3);           // 感應器接在 mBot 連接埠 3
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
  mbc_port3.begin();
  delay(1000);   // 等 1 秒讓感應器開機
  while (1) {   // 一直重複
    buzzer.tone((200 + (mbcRead(mbc_port3, 0x03) * 10)), 100);   // 音高 ＝ 200 ＋ 較近距離 × 10 赫茲，響 0.1 秒
  }
}

void loop() {}   // 這裡什麼都沒有：上面的積木不會結束
```

</details>

**一步一步看懂程式**
1. 等 1 秒讓感應器開機
2. 一直重複
   - 音高 ＝ 200 ＋ 較近距離 × 10 赫茲，響 0.1 秒

**跑起來應該看到什麼**：手放在感應器前 10 公分：300 赫茲的低音；慢慢拿遠，音越來越高；前面沒東西（120）是 1400 赫茲的高音。

**常見問題**
- **聽不到聲音？** → mBot 的蜂鳴器在主板上，聲音不大；把耳朵靠近，或把 0.1 秒改成 0.3 秒。
- **前面明明沒東西，數字卻不是 120？** → 雷射看到地板或桌面了：把感應器裝高一點、或微微朝上。120 公分以外都讀 120。
- **什麼都沒反應、或讀值一直是 -32768？** → 連接埠選錯（這頁都接連接埠 3）、插座片沒插緊、或感應器不是 MakeBlock 版。先用「MBC 共用 → 連線正常？」積木確認；「協議版本」讀到 2 才是新韌體（舊韌體請改用舊版擴展 v1）。
- **上傳失敗、或積木是空白的？** → 確認右下角切到「上傳」模式、擴展 v2 已拖進 mBlock、USB 線接的是 mBot 主板。

---

## 範例 2：紅黃綠距離燈

**這個範例會做什麼**：用「如果…就…否則…」把距離分成三段：15 公分以內紅燈、40 公分以內黃燈、再遠綠燈。這是後面「開到牆前停車」的判斷骨架。

**要準備什麼**
- mBot（mCore 主板）＋ mBlock 5 電腦版
- 測距者 2 路（MakeBlock 版）＋ mBot 插座片

**怎麼接**：測距者 → mBot 插座片 → mBot **連接埠 3**。感應器裝在車頭、兩顆雷射朝正前方。

**下載程式**

| mBlock 5 專案 | 等價 Arduino 程式 |
|---|---|
| [ex02_led_warning.mblock](/examples/tof2/mblock/ex02_led_warning.mblock) | [ex02_led_warning.ino](/examples/tof2/mblock/ex02_led_warning/ex02_led_warning.ino) |

<details open>
<summary>🧱 mBlock 積木（照著拼就好）</summary>

<pre class="blocks">當 mBot 啟動時 :: events hat
wait (1) seconds
forever
  if &lt;(測距者 2 路 連接埠 [3 v] 較近距離 cm :: #0A9E8F) &lt; (15)&gt; then
    LED 燈位置 [全部 v] 的三原色數值為 紅 (255) 綠 (0) 藍 (0) :: looks
  else
    if &lt;(測距者 2 路 連接埠 [3 v] 較近距離 cm :: #0A9E8F) &lt; (40)&gt; then
      LED 燈位置 [全部 v] 的三原色數值為 紅 (255) 綠 (255) 藍 (0) :: looks
    else
      LED 燈位置 [全部 v] 的三原色數值為 紅 (0) 綠 (255) 藍 (0) :: looks
    end
  end
end</pre>

</details>

<details>
<summary>🔧 等價 Arduino 程式（mBot）</summary>

```cpp
// 測距者 2 路 × mBlock 5　範例 2：紅黃綠距離燈（mBot 等價 Arduino 程式）
// 接線：測距者 → mBot 插座片 → mBot 連接埠 3。感應器裝在車頭、兩顆雷射朝正前方。
// 跟 mBlock 積木範例做的事完全一樣，給想看「積木背後長什麼樣」或用 Arduino IDE 的人。
// 需要的函式庫：
//   1. Makeblock（Arduino IDE「函式庫管理員」搜尋 Makeblock，或 mBlock 5 安裝資料夾裡的 Makeblock 函式庫）
//   2. MBC_Sensors v2：擴展 .mext 解壓後的 src/ 四個檔（MBC_Sensors.h/.cpp、mbc_proto.h、mbc_ids.h），
//      放進 文件/Arduino/libraries/MBC_Sensors/ 就能 #include <MBC_Sensors.h>（https://mbc-pro-sensors.github.io/makeblock.html）
// 主板選 Arduino Uno（mBot 的 mCore 就是 Uno），上傳後拔掉 USB 用電池跑。

#include <MeMCore.h>            // mBot（mCore）：馬達、LED、蜂鳴器
#include <SoftwareSerial.h>
#include <MBC_Sensors.h>        // MBC 感應器 v2（跟 mBlock 擴展同一份函式庫）

MBCPort mbc_port3(3);           // 感應器接在 mBot 連接埠 3
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
  mbc_port3.begin();
  delay(1000);   // 等 1 秒讓感應器開機
  while (1) {   // 一直重複
    if ((mbcRead(mbc_port3, 0x03) < 15)) {   // 如果較近距離小於 15
      rgbled.setColor(0, 255, 0, 0); rgbled.show();   // 15 公分以內：紅燈
    } else {
      if ((mbcRead(mbc_port3, 0x03) < 40)) {   // 否則，如果較近距離小於 40
        rgbled.setColor(0, 255, 255, 0); rgbled.show();   // 40 公分以內：黃燈
      } else {
        rgbled.setColor(0, 0, 255, 0); rgbled.show();   // 更遠：綠燈
      }
    }
  }
}

void loop() {}   // 這裡什麼都沒有：上面的積木不會結束
```

</details>

**一步一步看懂程式**
1. 等 1 秒讓感應器開機
2. 一直重複
   - 如果較近距離小於 15
      - 15 公分以內：紅燈
      - 否則：
         - 否則，如果較近距離小於 40
            - 40 公分以內：黃燈
            - 否則：
               - 更遠：綠燈

**跑起來應該看到什麼**：手慢慢靠近感應器：綠燈 → 黃燈（40 公分）→ 紅燈（15 公分）。

**常見問題**
- **前面明明沒東西，數字卻不是 120？** → 雷射看到地板或桌面了：把感應器裝高一點、或微微朝上。120 公分以外都讀 120。
- **什麼都沒反應、或讀值一直是 -32768？** → 連接埠選錯（這頁都接連接埠 3）、插座片沒插緊、或感應器不是 MakeBlock 版。先用「MBC 共用 → 連線正常？」積木確認；「協議版本」讀到 2 才是新韌體（舊韌體請改用舊版擴展 v1）。
- **上傳失敗、或積木是空白的？** → 確認右下角切到「上傳」模式、擴展 v2 已拖進 mBlock、USB 線接的是 mBot 主板。

---

## 範例 3：距離控制一個輪子

**這個範例會做什麼**：把「較近距離 × 0.8」直接當左輪的動力：前面越空輪子轉越快，東西越靠近越慢，貼到前面就停。先把車子架高看一個輪子怎麼反應，下一個範例再放到地上開。

**要準備什麼**
- mBot（mCore 主板）＋ mBlock 5 電腦版
- mBot 兩輪小車（左輪接 M1、右輪接 M2，出廠就是這樣接）
- 測距者 2 路（MakeBlock 版）＋ mBot 插座片
- 一面牆或一個箱子當障礙物

**怎麼接**：測距者 → mBot 插座片 → mBot **連接埠 3**。感應器裝在車頭、兩顆雷射朝正前方。

**下載程式**

| mBlock 5 專案 | 等價 Arduino 程式 |
|---|---|
| [ex03_one_wheel.mblock](/examples/tof2/mblock/ex03_one_wheel.mblock) | [ex03_one_wheel.ino](/examples/tof2/mblock/ex03_one_wheel/ex03_one_wheel.ino) |

<details open>
<summary>🧱 mBlock 積木（照著拼就好）</summary>

<pre class="blocks">當 mBot 啟動時 :: events hat
wait (1) seconds
forever
  左輪動力 ((測距者 2 路 連接埠 [3 v] 較近距離 cm :: #0A9E8F) * (0.8)) %，右輪動力 (0) % :: motion
end</pre>

</details>

<details>
<summary>🔧 等價 Arduino 程式（mBot）</summary>

```cpp
// 測距者 2 路 × mBlock 5　範例 3：距離控制一個輪子（mBot 等價 Arduino 程式）
// 接線：測距者 → mBot 插座片 → mBot 連接埠 3。感應器裝在車頭、兩顆雷射朝正前方。
// 跟 mBlock 積木範例做的事完全一樣，給想看「積木背後長什麼樣」或用 Arduino IDE 的人。
// 需要的函式庫：
//   1. Makeblock（Arduino IDE「函式庫管理員」搜尋 Makeblock，或 mBlock 5 安裝資料夾裡的 Makeblock 函式庫）
//   2. MBC_Sensors v2：擴展 .mext 解壓後的 src/ 四個檔（MBC_Sensors.h/.cpp、mbc_proto.h、mbc_ids.h），
//      放進 文件/Arduino/libraries/MBC_Sensors/ 就能 #include <MBC_Sensors.h>（https://mbc-pro-sensors.github.io/makeblock.html）
// 主板選 Arduino Uno（mBot 的 mCore 就是 Uno），上傳後拔掉 USB 用電池跑。

#include <MeMCore.h>            // mBot（mCore）：馬達、LED、蜂鳴器
#include <SoftwareSerial.h>
#include <MBC_Sensors.h>        // MBC 感應器 v2（跟 mBlock 擴展同一份函式庫）

MBCPort mbc_port3(3);           // 感應器接在 mBot 連接埠 3
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
  mbc_port3.begin();
  delay(1000);   // 等 1 秒讓感應器開機
  while (1) {   // 一直重複
    wheels((mbcRead(mbc_port3, 0x03) * 0.8), 0);   // 左輪動力 ＝ 較近距離 × 0.8（0 ~ 96），右輪 0
  }
}

void loop() {}   // 這裡什麼都沒有：上面的積木不會結束
```

</details>

**一步一步看懂程式**
1. 等 1 秒讓感應器開機
2. 一直重複
   - 左輪動力 ＝ 較近距離 × 0.8（0 ~ 96），右輪 0

**跑起來應該看到什麼**：前面沒東西左輪全速轉；手慢慢靠近，輪子越來越慢；貼到感應器前面輪子停。

**常見問題**
- **前面明明沒東西，數字卻不是 120？** → 雷射看到地板或桌面了：把感應器裝高一點、或微微朝上。120 公分以外都讀 120。
- **什麼都沒反應、或讀值一直是 -32768？** → 連接埠選錯（這頁都接連接埠 3）、插座片沒插緊、或感應器不是 MakeBlock 版。先用「MBC 共用 → 連線正常？」積木確認；「協議版本」讀到 2 才是新韌體（舊韌體請改用舊版擴展 v1）。
- **上傳失敗、或積木是空白的？** → 確認右下角切到「上傳」模式、擴展 v2 已拖進 mBlock、USB 線接的是 mBot 主板。

---

## 範例 4：開到牆前停車

**這個範例會做什麼**：車子往前開，較近距離小於 20 公分就停；把擋在前面的東西拿開又繼續走。這是「感應器＋如果＋馬達」最基本的組合。

**要準備什麼**
- mBot（mCore 主板）＋ mBlock 5 電腦版
- mBot 兩輪小車（左輪接 M1、右輪接 M2，出廠就是這樣接）
- 測距者 2 路（MakeBlock 版）＋ mBot 插座片
- 一面牆或一個箱子當障礙物

**怎麼接**：測距者 → mBot 插座片 → mBot **連接埠 3**。感應器裝在車頭、兩顆雷射朝正前方。

**下載程式**

| mBlock 5 專案 | 等價 Arduino 程式 |
|---|---|
| [ex04_stop_wall.mblock](/examples/tof2/mblock/ex04_stop_wall.mblock) | [ex04_stop_wall.ino](/examples/tof2/mblock/ex04_stop_wall/ex04_stop_wall.ino) |

<details open>
<summary>🧱 mBlock 積木（照著拼就好）</summary>

<pre class="blocks">當 mBot 啟動時 :: events hat
wait (1) seconds
forever
  if &lt;(測距者 2 路 連接埠 [3 v] 較近距離 cm :: #0A9E8F) &gt; (20)&gt; then
    左輪動力 (40) %，右輪動力 (40) % :: motion
  else
    停止移動 :: motion
  end
end</pre>

</details>

<details>
<summary>🔧 等價 Arduino 程式（mBot）</summary>

```cpp
// 測距者 2 路 × mBlock 5　範例 4：開到牆前停車（mBot 等價 Arduino 程式）
// 接線：測距者 → mBot 插座片 → mBot 連接埠 3。感應器裝在車頭、兩顆雷射朝正前方。
// 跟 mBlock 積木範例做的事完全一樣，給想看「積木背後長什麼樣」或用 Arduino IDE 的人。
// 需要的函式庫：
//   1. Makeblock（Arduino IDE「函式庫管理員」搜尋 Makeblock，或 mBlock 5 安裝資料夾裡的 Makeblock 函式庫）
//   2. MBC_Sensors v2：擴展 .mext 解壓後的 src/ 四個檔（MBC_Sensors.h/.cpp、mbc_proto.h、mbc_ids.h），
//      放進 文件/Arduino/libraries/MBC_Sensors/ 就能 #include <MBC_Sensors.h>（https://mbc-pro-sensors.github.io/makeblock.html）
// 主板選 Arduino Uno（mBot 的 mCore 就是 Uno），上傳後拔掉 USB 用電池跑。

#include <MeMCore.h>            // mBot（mCore）：馬達、LED、蜂鳴器
#include <SoftwareSerial.h>
#include <MBC_Sensors.h>        // MBC 感應器 v2（跟 mBlock 擴展同一份函式庫）

MBCPort mbc_port3(3);           // 感應器接在 mBot 連接埠 3
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
  mbc_port3.begin();
  delay(1000);   // 等 1 秒讓感應器開機
  while (1) {   // 一直重複
    if ((mbcRead(mbc_port3, 0x03) > 20)) {   // 如果較近距離大於 20
      wheels(40, 40);   // 前面是空的：兩輪 40 往前
    } else {
      wheels(0, 0);   // 太近：停車
    }
  }
}

void loop() {}   // 這裡什麼都沒有：上面的積木不會結束
```

</details>

**一步一步看懂程式**
1. 等 1 秒讓感應器開機
2. 一直重複
   - 如果較近距離大於 20
      - 前面是空的：兩輪 40 往前
      - 否則：
         - 太近：停車

**跑起來應該看到什麼**：車子往前開，到牆前約 20 公分停下來；用手擋在前面它也會停，手拿開又繼續走。

**常見問題**
- **停下來的時候撞到牆？** → 速度太快煞不住：把 40 改成 30，或把 20 改成 30。
- **車子往反方向轉、越修越歪？** → 左右輪的「＋」「－」對調（或把左輪、右輪的動力互換）。mBot 出廠左輪 M1、右輪 M2，自己接線的請確認。
- **前面明明沒東西，數字卻不是 120？** → 雷射看到地板或桌面了：把感應器裝高一點、或微微朝上。120 公分以外都讀 120。
- **什麼都沒反應、或讀值一直是 -32768？** → 連接埠選錯（這頁都接連接埠 3）、插座片沒插緊、或感應器不是 MakeBlock 版。先用「MBC 共用 → 連線正常？」積木確認；「協議版本」讀到 2 才是新韌體（舊韌體請改用舊版擴展 v1）。
- **上傳失敗、或積木是空白的？** → 確認右下角切到「上傳」模式、擴展 v2 已拖進 mBlock、USB 線接的是 mBot 主板。

---

## 範例 5：哪邊近就往另一邊轉

**這個範例會做什麼**：兩顆雷射的好處：知道障礙物在左邊還是右邊。前面 20 公分內有東西時，比較左、右距離，左邊近就右轉、右邊近就左轉（亮紅燈）；沒東西就直走（亮綠燈）。這就是一台會自己避障的車。

**要準備什麼**
- mBot（mCore 主板）＋ mBlock 5 電腦版
- mBot 兩輪小車（左輪接 M1、右輪接 M2，出廠就是這樣接）
- 測距者 2 路（MakeBlock 版）＋ mBot 插座片
- 一面牆或一個箱子當障礙物

**怎麼接**：測距者 → mBot 插座片 → mBot **連接埠 3**。感應器裝在車頭、兩顆雷射朝正前方。

**下載程式**

| mBlock 5 專案 | 等價 Arduino 程式 |
|---|---|
| [ex05_avoid.mblock](/examples/tof2/mblock/ex05_avoid.mblock) | [ex05_avoid.ino](/examples/tof2/mblock/ex05_avoid/ex05_avoid.ino) |

<details open>
<summary>🧱 mBlock 積木（照著拼就好）</summary>

<pre class="blocks">當 mBot 啟動時 :: events hat
wait (1) seconds
forever
  if &lt;(測距者 2 路 連接埠 [3 v] 較近距離 cm :: #0A9E8F) &lt; (20)&gt; then
    LED 燈位置 [全部 v] 的三原色數值為 紅 (255) 綠 (0) 藍 (0) :: looks
    if &lt;(測距者 2 路 連接埠 [3 v] 左距離 cm :: #0A9E8F) &lt; (測距者 2 路 連接埠 [3 v] 右距離 cm :: #0A9E8F)&gt; then
      左輪動力 (40) %，右輪動力 (-40) % :: motion
    else
      左輪動力 (-40) %，右輪動力 (40) % :: motion
    end
  else
    LED 燈位置 [全部 v] 的三原色數值為 紅 (0) 綠 (255) 藍 (0) :: looks
    左輪動力 (40) %，右輪動力 (40) % :: motion
  end
end</pre>

</details>

<details>
<summary>🔧 等價 Arduino 程式（mBot）</summary>

```cpp
// 測距者 2 路 × mBlock 5　範例 5：哪邊近就往另一邊轉（mBot 等價 Arduino 程式）
// 接線：測距者 → mBot 插座片 → mBot 連接埠 3。感應器裝在車頭、兩顆雷射朝正前方。
// 跟 mBlock 積木範例做的事完全一樣，給想看「積木背後長什麼樣」或用 Arduino IDE 的人。
// 需要的函式庫：
//   1. Makeblock（Arduino IDE「函式庫管理員」搜尋 Makeblock，或 mBlock 5 安裝資料夾裡的 Makeblock 函式庫）
//   2. MBC_Sensors v2：擴展 .mext 解壓後的 src/ 四個檔（MBC_Sensors.h/.cpp、mbc_proto.h、mbc_ids.h），
//      放進 文件/Arduino/libraries/MBC_Sensors/ 就能 #include <MBC_Sensors.h>（https://mbc-pro-sensors.github.io/makeblock.html）
// 主板選 Arduino Uno（mBot 的 mCore 就是 Uno），上傳後拔掉 USB 用電池跑。

#include <MeMCore.h>            // mBot（mCore）：馬達、LED、蜂鳴器
#include <SoftwareSerial.h>
#include <MBC_Sensors.h>        // MBC 感應器 v2（跟 mBlock 擴展同一份函式庫）

MBCPort mbc_port3(3);           // 感應器接在 mBot 連接埠 3
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
  mbc_port3.begin();
  delay(1000);   // 等 1 秒讓感應器開機
  while (1) {   // 一直重複
    if ((mbcRead(mbc_port3, 0x03) < 20)) {   // 如果較近距離小於 20
      rgbled.setColor(0, 255, 0, 0); rgbled.show();   // 有障礙：紅燈
      if ((mbcRead(mbc_port3, 0x01) < mbcRead(mbc_port3, 0x02))) {   // 如果左距離小於右距離
        wheels(40, -40);   // 左邊比較近：原地右轉
      } else {
        wheels(-40, 40);   // 右邊比較近：原地左轉
      }
    } else {
      rgbled.setColor(0, 0, 255, 0); rgbled.show();   // 前面是空的：綠燈
      wheels(40, 40);   // 直走
    }
  }
}

void loop() {}   // 這裡什麼都沒有：上面的積木不會結束
```

</details>

**一步一步看懂程式**
1. 等 1 秒讓感應器開機
2. 一直重複
   - 如果較近距離小於 20
      - 有障礙：紅燈
      - 如果左距離小於右距離
         - 左邊比較近：原地右轉
         - 否則：
            - 右邊比較近：原地左轉
      - 否則：
         - 前面是空的：綠燈
         - 直走

**跑起來應該看到什麼**：車子直走亮綠燈；遇到障礙變紅燈並轉向比較空的那一邊，轉到前面沒東西又直走。

**常見問題**
- **卡在角落一直轉？** → 兩邊都近時會左右來回轉。加一段「較近距離小於 8 就先後退 0.5 秒」。
- **車子往反方向轉、越修越歪？** → 左右輪的「＋」「－」對調（或把左輪、右輪的動力互換）。mBot 出廠左輪 M1、右輪 M2，自己接線的請確認。
- **前面明明沒東西，數字卻不是 120？** → 雷射看到地板或桌面了：把感應器裝高一點、或微微朝上。120 公分以外都讀 120。
- **什麼都沒反應、或讀值一直是 -32768？** → 連接埠選錯（這頁都接連接埠 3）、插座片沒插緊、或感應器不是 MakeBlock 版。先用「MBC 共用 → 連線正常？」積木確認；「協議版本」讀到 2 才是新韌體（舊韌體請改用舊版擴展 v1）。
- **上傳失敗、或積木是空白的？** → 確認右下角切到「上傳」模式、擴展 v2 已拖進 mBlock、USB 線接的是 mBot 主板。

---

## 下一步

- 全部 測距者積木、各主板可用的連接埠：[MakeBlock 積木擴展](/makeblock.md)；這款產品的 mBlock 積木表：[測距者 2 路 × mBlock 5](/sensors/tof2/mblock.md)
- 同樣的 5 個範例在樂高上怎麼寫：[SPIKE 新手範例](/sensors/tof2/spike-examples.md)、[EV3 新手範例](/sensors/tof2/ev3-examples.md)
- 想用 Arduino 直接接感應器（I2C 版，不經 mBot）：[Arduino 範例](/sensors/tof2/arduino-examples.md)
