<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 95_產品網站\mBlock範例產生器（node build.js）產生：改內容請改產生器的 sets/<產品>.js 再重建，不要手改本檔 -->
# 🧱 mBlock 新手範例：測距者 8 路

[🔙 回到測距者 8 路產品頁](/sensors/tof8/index.md)

這一頁是寫給**完全沒寫過程式**的人：用 mBlock 5 的積木在 **mBot** 上讀測距者 8 路。5 個範例從「把數字變成聲音」開始，一步一步做到會自己跑的車子。每個範例都有積木圖、可下載的專案檔，還有一支做同樣事情的 Arduino 程式。

| 寫法 | 適合誰 | 怎麼打開範例檔 |
|---|---|---|
| 🟧 **mBlock 5 電腦版**（積木，上傳模式）＋ **MBC 感應器擴展 v2** | 學校、第一次用 mBot 的人 | 下載 `.mblock` → 打開 mBlock 5 → 「檔案 → 從電腦打開」→ 選檔案 → 右下切到「上傳」→ 按「上傳」 |
| 🔧 **Arduino IDE**（文字程式） | 想看積木背後長什麼樣、或自己寫程式的人 | 下載 `.ino` → 裝好 Makeblock 與 MBC_Sensors 函式庫（見 [MakeBlock 積木擴展](/makeblock.md)）→ 主板選 Arduino Uno → 上傳 |

> [!IMPORTANT]
> - 感應器要是 **MakeBlock 版、2026-10 之後的協定 v2 韌體**，經 **mBot 插座片**接 RJ25 連接埠；擴展要用 **v2.0.0**（舊韌體配舊版 v1 擴展，兩者不能混用）。
> - 主板是 **mBot（mCore）**。mBot Ranger、Orion、MegaPi 也能用，但要把「當 mBot 啟動時」換成那塊主板的起始積木、連接埠改成它能用的號碼（見 [支援的主板與連接埠](/makeblock.md)）；mBot2（CyberPi）沒有 RJ25，不支援。
> - 積木圖裡的控制與運算積木（forever、if…then…else、wait、set…to）以英文顯示，在 mBlock 的中文介面就是「重複無限次」「如果…那麼…否則」「等待…秒」「變數設為」。

## 開始之前

<figure class="ref-fig"><img src="/images/figures/tof8/wiring-mbot.svg" alt="測距者 8 路 × mBot 接線" loading="lazy"></figure>

感應器 → mBot 插座片 → 主板 RJ25 連接埠（S1 = 主機送、S2 = 主機收，UART 38400）。

1. **裝擴展**：到 [MakeBlock 積木擴展](/makeblock.md) 取得 **MBC Pro Sensors v2.0.0** 的 `.mext`，拖進 mBlock 5 視窗；左下「設備」加入 **mBot**，右下切到「上傳」。
2. **接線**：測距者 → mBot 插座片 → mBot **連接埠 3**（這頁所有範例都接這一埠；換埠就把積木上的連接埠數字改掉）。
3. **先開機再讀**：每個範例一開始都「等待 3 秒」讓感應器開機；8 顆雷射初始化要 2~4 秒，這段時間讀到 120 是正常的。
4. 測距者 8 路不用校準。8 顆雷射排成半圓：第 1 顆正左、第 4、5 顆正前、第 8 顆正右（每顆隔 30°）；「最近方向」負＝左、0＝正前方、正＝右；四周沒東西讀 **120**。
5. **先用範例 2 確認左右**：手放在車子左邊要亮紅燈；亮藍燈代表感應器裝反了，後面範例的左右會相反。

## 這頁會用到的積木

每顆讀值積木就是感應器值表裡的一個「值編號」（跟 Arduino I2C 版的暫存器編號相同）；完整積木表在 [MakeBlock 積木擴展](/makeblock.md)。

| 積木 | 傳回 | 意思 |
|---|---|---|
| 測距者 8 路 連接埠 ［3］ 最近距離 cm | 0~120 cm | 8 顆裡最小的距離 0~120 cm（失效那顆以 120 參與），120 = 四周沒東西 |
| 測距者 8 路 連接埠 ［3］ 最近方向 | -8~+8 | 最近目標在哪個方向：−7（正左）~ +7（正右），0 = 正前方，負 = 左；±8 = 四周沒東西（沿用上次偏向側） |
| 測距者 8 路 連接埠 ［3］ 最空曠方向 | -7~+7 | 最遠（最空曠）在哪個方向：−7 ~ +7，0 = 正前方，負 = 左；8 顆全 120 時 = 0 |

**這頁的範例**

1. [最近距離變音高](#%E7%AF%84%E4%BE%8B-1%EF%BC%9A%E6%9C%80%E8%BF%91%E8%B7%9D%E9%9B%A2%E8%AE%8A%E9%9F%B3%E9%AB%98)
2. [東西在哪邊就亮什麼燈](#%E7%AF%84%E4%BE%8B-2%EF%BC%9A%E6%9D%B1%E8%A5%BF%E5%9C%A8%E5%93%AA%E9%82%8A%E5%B0%B1%E4%BA%AE%E4%BB%80%E9%BA%BC%E7%87%88)
3. [一個輪子把車頭轉向最近的東西](#%E7%AF%84%E4%BE%8B-3%EF%BC%9A%E4%B8%80%E5%80%8B%E8%BC%AA%E5%AD%90%E6%8A%8A%E8%BB%8A%E9%A0%AD%E8%BD%89%E5%90%91%E6%9C%80%E8%BF%91%E7%9A%84%E6%9D%B1%E8%A5%BF)
4. [避障開車](#%E7%AF%84%E4%BE%8B-4%EF%BC%9A%E9%81%BF%E9%9A%9C%E9%96%8B%E8%BB%8A)
5. [朝最空曠的地方走](#%E7%AF%84%E4%BE%8B-5%EF%BC%9A%E6%9C%9D%E6%9C%80%E7%A9%BA%E6%9B%A0%E7%9A%84%E5%9C%B0%E6%96%B9%E8%B5%B0)

> [!NOTE]
> `.mblock` 是由產生器照 mBlock 5 的存檔格式產生的，**還沒在 mBlock 5 裡開啟驗證**（等擴展 v2 上架後補驗）。開不起來或積木顯示空白，請先確認 v2 擴展已拖進 mBlock、設備是 mBot，或照下面的積木圖自己拼（5 分鐘內拼得完）；也歡迎 [LINE 告訴我們](https://line.me/R/ti/p/@692vcvuk)。

---

## 範例 1：最近距離變音高

**這個範例會做什麼**：mBot 沒有螢幕，這個範例把「最近距離」（8 顆裡最小的，0 ~ 120 公分）變成蜂鳴器的音高：不管東西從哪個方向靠近，音都會變低。先學會「讀到數字」。

**要準備什麼**
- mBot（mCore 主板）＋ mBlock 5 電腦版
- 測距者 8 路（MakeBlock 版）＋ mBot 插座片

**怎麼接**：測距者 → mBot 插座片 → mBot **連接埠 3**。感應器裝在車頭最前面、第 4、5 顆雷射朝正前方（第 1 顆正左、第 8 顆正右）。

**下載程式**

| mBlock 5 專案 | 等價 Arduino 程式 |
|---|---|
| [ex01_theremin.mblock](/examples/tof8/mblock/ex01_theremin.mblock) | [ex01_theremin.ino](/examples/tof8/mblock/ex01_theremin/ex01_theremin.ino) |

<details open>
<summary>🧱 mBlock 積木（照著拼就好）</summary>

<pre class="blocks">當 mBot 啟動時 :: events hat
wait (3) seconds
forever
  播放音頻 ((200) + ((測距者 8 路 連接埠 [3 v] 最近距離 cm :: #0A9E8F) * (10))) 赫茲，持續 (0.1) 秒 :: sound
end</pre>

</details>

<details>
<summary>🔧 等價 Arduino 程式（mBot）</summary>

```cpp
// 測距者 8 路 × mBlock 5　範例 1：最近距離變音高（mBot 等價 Arduino 程式）
// 接線：測距者 → mBot 插座片 → mBot 連接埠 3。感應器裝在車頭最前面、第 4、5 顆雷射朝正前方（第 1 顆正左、第 8 顆正右）。
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
  delay(3000);   // 等 3 秒讓 8 顆雷射初始化
  while (1) {   // 一直重複
    buzzer.tone((200 + (mbcRead(mbc_port3, 0x01) * 10)), 100);   // 音高 ＝ 200 ＋ 最近距離 × 10 赫茲，響 0.1 秒
  }
}

void loop() {}   // 這裡什麼都沒有：上面的積木不會結束
```

</details>

**一步一步看懂程式**
1. 等 3 秒讓 8 顆雷射初始化
2. 一直重複
   - 音高 ＝ 200 ＋ 最近距離 × 10 赫茲，響 0.1 秒

**跑起來應該看到什麼**：四周沒東西：1400 赫茲高音。手從左邊、前面或右邊靠近，音都變低；10 公分時 300 赫茲。

**常見問題**
- **聽不到聲音？** → mBot 的蜂鳴器在主板上，聲音不大；把耳朵靠近，或把 0.1 秒改成 0.3 秒。
- **一開機就是低音？** → 前 3 秒雷射還在初始化會讀到 120（高音）才對；一直低音代表附近有東西（桌面、牆），把車子放空曠一點。
- **左右方向跟範例說的相反？** → 感應器裝反了（元件面朝下或轉了 180 度）。用範例 2 確認：手放車子左邊要亮紅燈。
- **什麼都沒反應、或讀值一直是 -32768？** → 連接埠選錯（這頁都接連接埠 3）、插座片沒插緊、或感應器不是 MakeBlock 版。先用「MBC 共用 → 連線正常？」積木確認；「協議版本」讀到 2 才是新韌體（舊韌體請改用舊版擴展 v1）。
- **上傳失敗、或積木是空白的？** → 確認右下角切到「上傳」模式、擴展 v2 已拖進 mBlock、USB 線接的是 mBot 主板。

---

## 範例 2：東西在哪邊就亮什麼燈

**這個範例會做什麼**：「最近方向」告訴你最近的東西在哪個方向：負＝左、0＝正前方、正＝右。用「如果…就…否則…」：60 公分內有東西時，左邊紅燈、右邊藍燈、正前方綠燈；沒東西關燈。

**要準備什麼**
- mBot（mCore 主板）＋ mBlock 5 電腦版
- 測距者 8 路（MakeBlock 版）＋ mBot 插座片

**怎麼接**：測距者 → mBot 插座片 → mBot **連接埠 3**。感應器裝在車頭最前面、第 4、5 顆雷射朝正前方（第 1 顆正左、第 8 顆正右）。

**下載程式**

| mBlock 5 專案 | 等價 Arduino 程式 |
|---|---|
| [ex02_led_direction.mblock](/examples/tof8/mblock/ex02_led_direction.mblock) | [ex02_led_direction.ino](/examples/tof8/mblock/ex02_led_direction/ex02_led_direction.ino) |

<details open>
<summary>🧱 mBlock 積木（照著拼就好）</summary>

<pre class="blocks">當 mBot 啟動時 :: events hat
wait (3) seconds
forever
  if &lt;(測距者 8 路 連接埠 [3 v] 最近距離 cm :: #0A9E8F) &gt; (60)&gt; then
    LED 燈位置 [全部 v] 的三原色數值為 紅 (0) 綠 (0) 藍 (0) :: looks
  else
    if &lt;(測距者 8 路 連接埠 [3 v] 最近方向 :: #0A9E8F) &lt; (0)&gt; then
      LED 燈位置 [全部 v] 的三原色數值為 紅 (255) 綠 (0) 藍 (0) :: looks
    else
      if &lt;(測距者 8 路 連接埠 [3 v] 最近方向 :: #0A9E8F) &gt; (0)&gt; then
        LED 燈位置 [全部 v] 的三原色數值為 紅 (0) 綠 (0) 藍 (255) :: looks
      else
        LED 燈位置 [全部 v] 的三原色數值為 紅 (0) 綠 (255) 藍 (0) :: looks
      end
    end
  end
end</pre>

</details>

<details>
<summary>🔧 等價 Arduino 程式（mBot）</summary>

```cpp
// 測距者 8 路 × mBlock 5　範例 2：東西在哪邊就亮什麼燈（mBot 等價 Arduino 程式）
// 接線：測距者 → mBot 插座片 → mBot 連接埠 3。感應器裝在車頭最前面、第 4、5 顆雷射朝正前方（第 1 顆正左、第 8 顆正右）。
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
  delay(3000);   // 等 3 秒讓 8 顆雷射初始化
  while (1) {   // 一直重複
    if ((mbcRead(mbc_port3, 0x01) > 60)) {   // 如果最近距離大於 60
      rgbled.setColor(0, 0, 0, 0); rgbled.show();   // 60 公分內沒東西：關燈
    } else {
      if ((mbcRead(mbc_port3, 0x02) < 0)) {   // 否則，如果最近方向小於 0（左）
        rgbled.setColor(0, 255, 0, 0); rgbled.show();   // 最近的東西在左邊：紅燈
      } else {
        if ((mbcRead(mbc_port3, 0x02) > 0)) {   // 否則，如果最近方向大於 0（右）
          rgbled.setColor(0, 0, 0, 255); rgbled.show();   // 在右邊：藍燈
        } else {
          rgbled.setColor(0, 0, 255, 0); rgbled.show();   // 正前方：綠燈
        }
      }
    }
  }
}

void loop() {}   // 這裡什麼都沒有：上面的積木不會結束
```

</details>

**一步一步看懂程式**
1. 等 3 秒讓 8 顆雷射初始化
2. 一直重複
   - 如果最近距離大於 60
      - 60 公分內沒東西：關燈
      - 否則：
         - 否則，如果最近方向小於 0（左）
            - 最近的東西在左邊：紅燈
            - 否則：
               - 否則，如果最近方向大於 0（右）
                  - 在右邊：藍燈
                  - 否則：
                     - 正前方：綠燈

**跑起來應該看到什麼**：手放在車子左邊：紅燈；右邊：藍燈；正前方：綠燈；拿開（60 公分外）燈熄掉。

**常見問題**
- **左右方向跟範例說的相反？** → 感應器裝反了（元件面朝下或轉了 180 度）。用範例 2 確認：手放車子左邊要亮紅燈。
- **什麼都沒反應、或讀值一直是 -32768？** → 連接埠選錯（這頁都接連接埠 3）、插座片沒插緊、或感應器不是 MakeBlock 版。先用「MBC 共用 → 連線正常？」積木確認；「協議版本」讀到 2 才是新韌體（舊韌體請改用舊版擴展 v1）。
- **上傳失敗、或積木是空白的？** → 確認右下角切到「上傳」模式、擴展 v2 已拖進 mBlock、USB 線接的是 mBot 主板。

---

## 範例 3：一個輪子把車頭轉向最近的東西

**這個範例會做什麼**：只用左輪：動力 ＝ 最近方向 × 6。東西在右邊（正）左輪往前轉、車頭往右；在左邊（負）左輪倒轉、車頭往左；正前方（0）不動。60 公分內沒東西就停。放在地上看車頭跟著你的手轉。

**要準備什麼**
- mBot（mCore 主板）＋ mBlock 5 電腦版
- mBot 兩輪小車（左輪接 M1、右輪接 M2，出廠就是這樣接）
- 測距者 8 路（MakeBlock 版）＋ mBot 插座片
- 幾個箱子當障礙物

**怎麼接**：測距者 → mBot 插座片 → mBot **連接埠 3**。感應器裝在車頭最前面、第 4、5 顆雷射朝正前方（第 1 顆正左、第 8 顆正右）。

**下載程式**

| mBlock 5 專案 | 等價 Arduino 程式 |
|---|---|
| [ex03_one_wheel.mblock](/examples/tof8/mblock/ex03_one_wheel.mblock) | [ex03_one_wheel.ino](/examples/tof8/mblock/ex03_one_wheel/ex03_one_wheel.ino) |

<details open>
<summary>🧱 mBlock 積木（照著拼就好）</summary>

<pre class="blocks">當 mBot 啟動時 :: events hat
wait (3) seconds
forever
  if &lt;(測距者 8 路 連接埠 [3 v] 最近距離 cm :: #0A9E8F) &gt; (60)&gt; then
    停止移動 :: motion
  else
    左輪動力 ((測距者 8 路 連接埠 [3 v] 最近方向 :: #0A9E8F) * (6)) %，右輪動力 (0) % :: motion
  end
end</pre>

</details>

<details>
<summary>🔧 等價 Arduino 程式（mBot）</summary>

```cpp
// 測距者 8 路 × mBlock 5　範例 3：一個輪子把車頭轉向最近的東西（mBot 等價 Arduino 程式）
// 接線：測距者 → mBot 插座片 → mBot 連接埠 3。感應器裝在車頭最前面、第 4、5 顆雷射朝正前方（第 1 顆正左、第 8 顆正右）。
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
  delay(3000);   // 等 3 秒讓 8 顆雷射初始化
  while (1) {   // 一直重複
    if ((mbcRead(mbc_port3, 0x01) > 60)) {   // 如果最近距離大於 60
      wheels(0, 0);   // 60 公分內沒東西：停
    } else {
      wheels((mbcRead(mbc_port3, 0x02) * 6), 0);   // 左輪動力 ＝ 最近方向 × 6（−42 ~ +42），右輪 0
    }
  }
}

void loop() {}   // 這裡什麼都沒有：上面的積木不會結束
```

</details>

**一步一步看懂程式**
1. 等 3 秒讓 8 顆雷射初始化
2. 一直重複
   - 如果最近距離大於 60
      - 60 公分內沒東西：停
      - 否則：
         - 左輪動力 ＝ 最近方向 × 6（−42 ~ +42），右輪 0

**跑起來應該看到什麼**：手放在車子右前方，左輪往前轉、車頭轉向你的手，轉到正前方就停；手移到左邊，左輪倒轉。

**常見問題**
- **車子往反方向轉、越修越歪？** → 左右輪的「＋」「－」對調（或把左輪、右輪的動力互換）。mBot 出廠左輪 M1、右輪 M2，自己接線的請確認。
- **左右方向跟範例說的相反？** → 感應器裝反了（元件面朝下或轉了 180 度）。用範例 2 確認：手放車子左邊要亮紅燈。
- **什麼都沒反應、或讀值一直是 -32768？** → 連接埠選錯（這頁都接連接埠 3）、插座片沒插緊、或感應器不是 MakeBlock 版。先用「MBC 共用 → 連線正常？」積木確認；「協議版本」讀到 2 才是新韌體（舊韌體請改用舊版擴展 v1）。
- **上傳失敗、或積木是空白的？** → 確認右下角切到「上傳」模式、擴展 v2 已拖進 mBlock、USB 線接的是 mBot 主板。

---

## 範例 4：避障開車

**這個範例會做什麼**：車子直走；20 公分內有東西時看它在哪邊：在左邊就原地右轉、在右邊（或正前方）就原地左轉，轉到前面沒東西再直走。

**要準備什麼**
- mBot（mCore 主板）＋ mBlock 5 電腦版
- mBot 兩輪小車（左輪接 M1、右輪接 M2，出廠就是這樣接）
- 測距者 8 路（MakeBlock 版）＋ mBot 插座片
- 幾個箱子當障礙物

**怎麼接**：測距者 → mBot 插座片 → mBot **連接埠 3**。感應器裝在車頭最前面、第 4、5 顆雷射朝正前方（第 1 顆正左、第 8 顆正右）。

**下載程式**

| mBlock 5 專案 | 等價 Arduino 程式 |
|---|---|
| [ex04_avoid.mblock](/examples/tof8/mblock/ex04_avoid.mblock) | [ex04_avoid.ino](/examples/tof8/mblock/ex04_avoid/ex04_avoid.ino) |

<details open>
<summary>🧱 mBlock 積木（照著拼就好）</summary>

<pre class="blocks">當 mBot 啟動時 :: events hat
wait (3) seconds
forever
  if &lt;(測距者 8 路 連接埠 [3 v] 最近距離 cm :: #0A9E8F) &lt; (20)&gt; then
    if &lt;(測距者 8 路 連接埠 [3 v] 最近方向 :: #0A9E8F) &lt; (0)&gt; then
      左輪動力 (40) %，右輪動力 (-40) % :: motion
    else
      左輪動力 (-40) %，右輪動力 (40) % :: motion
    end
  else
    左輪動力 (40) %，右輪動力 (40) % :: motion
  end
end</pre>

</details>

<details>
<summary>🔧 等價 Arduino 程式（mBot）</summary>

```cpp
// 測距者 8 路 × mBlock 5　範例 4：避障開車（mBot 等價 Arduino 程式）
// 接線：測距者 → mBot 插座片 → mBot 連接埠 3。感應器裝在車頭最前面、第 4、5 顆雷射朝正前方（第 1 顆正左、第 8 顆正右）。
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
  delay(3000);   // 等 3 秒讓 8 顆雷射初始化
  while (1) {   // 一直重複
    if ((mbcRead(mbc_port3, 0x01) < 20)) {   // 如果最近距離小於 20
      if ((mbcRead(mbc_port3, 0x02) < 0)) {   // 如果最近方向小於 0（左）
        wheels(40, -40);   // 障礙在左邊：原地右轉
      } else {
        wheels(-40, 40);   // 障礙在右邊或正前方：原地左轉
      }
    } else {
      wheels(40, 40);   // 前面是空的：直走
    }
  }
}

void loop() {}   // 這裡什麼都沒有：上面的積木不會結束
```

</details>

**一步一步看懂程式**
1. 等 3 秒讓 8 顆雷射初始化
2. 一直重複
   - 如果最近距離小於 20
      - 如果最近方向小於 0（左）
         - 障礙在左邊：原地右轉
         - 否則：
            - 障礙在右邊或正前方：原地左轉
      - 否則：
         - 前面是空的：直走

**跑起來應該看到什麼**：車子在箱子之間自己繞，碰到東西前就轉開。

**常見問題**
- **側面擦到箱子？** → 側面的雷射（第 1、8 顆）也算在「最近距離」裡，把 20 改大（例如 30）車子就會離遠一點。
- **車子往反方向轉、越修越歪？** → 左右輪的「＋」「－」對調（或把左輪、右輪的動力互換）。mBot 出廠左輪 M1、右輪 M2，自己接線的請確認。
- **左右方向跟範例說的相反？** → 感應器裝反了（元件面朝下或轉了 180 度）。用範例 2 確認：手放車子左邊要亮紅燈。
- **什麼都沒反應、或讀值一直是 -32768？** → 連接埠選錯（這頁都接連接埠 3）、插座片沒插緊、或感應器不是 MakeBlock 版。先用「MBC 共用 → 連線正常？」積木確認；「協議版本」讀到 2 才是新韌體（舊韌體請改用舊版擴展 v1）。
- **上傳失敗、或積木是空白的？** → 確認右下角切到「上傳」模式、擴展 v2 已拖進 mBlock、USB 線接的是 mBot 主板。

---

## 範例 5：朝最空曠的地方走

**這個範例會做什麼**：範例 4 遇到東西才轉，這個範例一邊走一邊往「最空曠方向」偏：轉向 ＝ 最空曠方向 × 6，左輪 35 ＋ 轉向、右輪 35 − 轉向，車子會自己繞開障礙、走到最寬的地方（綠燈）。真的太近（10 公分內）就倒退、亮紅燈。

**要準備什麼**
- mBot（mCore 主板）＋ mBlock 5 電腦版
- mBot 兩輪小車（左輪接 M1、右輪接 M2，出廠就是這樣接）
- 測距者 8 路（MakeBlock 版）＋ mBot 插座片
- 幾個箱子當障礙物

**怎麼接**：測距者 → mBot 插座片 → mBot **連接埠 3**。感應器裝在車頭最前面、第 4、5 顆雷射朝正前方（第 1 顆正左、第 8 顆正右）。

**下載程式**

| mBlock 5 專案 | 等價 Arduino 程式 |
|---|---|
| [ex05_open_space.mblock](/examples/tof8/mblock/ex05_open_space.mblock) | [ex05_open_space.ino](/examples/tof8/mblock/ex05_open_space/ex05_open_space.ino) |

<details open>
<summary>🧱 mBlock 積木（照著拼就好）</summary>

<pre class="blocks">當 mBot 啟動時 :: events hat
wait (3) seconds
forever
  if &lt;(測距者 8 路 連接埠 [3 v] 最近距離 cm :: #0A9E8F) &lt; (10)&gt; then
    LED 燈位置 [全部 v] 的三原色數值為 紅 (255) 綠 (0) 藍 (0) :: looks
    左輪動力 (-30) %，右輪動力 (-30) % :: motion
  else
    LED 燈位置 [全部 v] 的三原色數值為 紅 (0) 綠 (255) 藍 (0) :: looks
    set [轉向 v] to ((測距者 8 路 連接埠 [3 v] 最空曠方向 :: #0A9E8F) * (6))
    左輪動力 ((35) + (轉向)) %，右輪動力 ((35) - (轉向)) % :: motion
  end
end</pre>

</details>

<details>
<summary>🔧 等價 Arduino 程式（mBot）</summary>

```cpp
// 測距者 8 路 × mBlock 5　範例 5：朝最空曠的地方走（mBot 等價 Arduino 程式）
// 接線：測距者 → mBot 插座片 → mBot 連接埠 3。感應器裝在車頭最前面、第 4、5 顆雷射朝正前方（第 1 顆正左、第 8 顆正右）。
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
float turn = 0;               // 變數：轉向

// 左／右輪動力 -100~100（正 = 往前）：mBot 的左輪裝反，所以 M1 給負的；% 換算成 PWM 0~255
void wheels(int l, int r) {
  l = constrain(l, -100, 100); r = constrain(r, -100, 100);
  motorL.run(-l * 255 / 100);
  motorR.run(r * 255 / 100);
}

void setup() {   // 跟 mBlock 產生的程式一樣，積木全部在 setup() 裡跑
  mbc_port3.begin();
  delay(3000);   // 等 3 秒讓 8 顆雷射初始化
  while (1) {   // 一直重複
    if ((mbcRead(mbc_port3, 0x01) < 10)) {   // 如果最近距離小於 10
      rgbled.setColor(0, 255, 0, 0); rgbled.show();   // 太近：紅燈
      wheels(-30, -30);   // 倒退
    } else {
      rgbled.setColor(0, 0, 255, 0); rgbled.show();   // 綠燈
      turn = (mbcRead(mbc_port3, 0x04) * 6);   // 轉向 ＝ 最空曠方向 × 6（−42 ~ +42）
      wheels((35 + turn), (35 - turn));   // 左輪 ＝ 35 ＋ 轉向、右輪 ＝ 35 − 轉向：空曠在左（負）就左輪慢、往左偏
    }
  }
}

void loop() {}   // 這裡什麼都沒有：上面的積木不會結束
```

</details>

**一步一步看懂程式**
1. 等 3 秒讓 8 顆雷射初始化
2. 一直重複
   - 如果最近距離小於 10
      - 太近：紅燈
      - 倒退
      - 否則：
         - 綠燈
         - 轉向 ＝ 最空曠方向 × 6（−42 ~ +42）
         - 左輪 ＝ 35 ＋ 轉向、右輪 ＝ 35 − 轉向：空曠在左（負）就左輪慢、往左偏

**跑起來應該看到什麼**：車子一路往最空的方向走，遇到箱子提早繞開；被圍住時倒退、亮紅燈。

**常見問題**
- **車子一直左右搖？** → 把 × 6 改小（例如 × 4）。
- **車子往反方向轉、越修越歪？** → 左右輪的「＋」「－」對調（或把左輪、右輪的動力互換）。mBot 出廠左輪 M1、右輪 M2，自己接線的請確認。
- **左右方向跟範例說的相反？** → 感應器裝反了（元件面朝下或轉了 180 度）。用範例 2 確認：手放車子左邊要亮紅燈。
- **什麼都沒反應、或讀值一直是 -32768？** → 連接埠選錯（這頁都接連接埠 3）、插座片沒插緊、或感應器不是 MakeBlock 版。先用「MBC 共用 → 連線正常？」積木確認；「協議版本」讀到 2 才是新韌體（舊韌體請改用舊版擴展 v1）。
- **上傳失敗、或積木是空白的？** → 確認右下角切到「上傳」模式、擴展 v2 已拖進 mBlock、USB 線接的是 mBot 主板。

---

## 下一步

- 全部 測距者積木、各主板可用的連接埠：[MakeBlock 積木擴展](/makeblock.md)；這款產品的 mBlock 積木表：[測距者 8 路 × mBlock 5](/sensors/tof8/mblock.md)
- 同樣的 5 個範例在樂高上怎麼寫：[SPIKE 新手範例](/sensors/tof8/spike-examples.md)、[EV3 新手範例](/sensors/tof8/ev3-examples.md)
- 想用 Arduino 直接接感應器（I2C 版，不經 mBot）：[Arduino 範例](/sensors/tof8/arduino-examples.md)
