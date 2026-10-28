<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 95_產品網站\mBlock範例產生器（node build.js）產生：改內容請改產生器的 sets/<產品>.js 再重建，不要手改本檔 -->
<!-- moved: /sensors/esp32cam/mblock/mblock5.md -->
<p class="moved-note">這頁內容已整合到新的說明頁：<a href="/sensors/esp32cam/mblock/mblock5.md">神攝手 ESP32CAM › mBlock › mBlock 5</a></p>

# 🧱 mBlock 新手範例：神攝手（學校版）

[🔙 回到神攝手（學校版）產品頁](/sensors/esp32cam/index.md)

這一頁是寫給**完全沒寫過程式**的人：用 mBlock 5 的積木在 **mBot** 上讀神攝手（學校版）。5 個範例從「把數字變成聲音」開始，一步一步做到會自己跑的車子。每個範例都有積木圖、可下載的專案檔，還有一支做同樣事情的 Arduino 程式。

| 寫法 | 適合誰 | 怎麼打開範例檔 |
|---|---|---|
| 🟧 **mBlock 5 電腦版**（積木，上傳模式）＋ **MBC 感應器擴展 v2** | 學校、第一次用 mBot 的人 | 下載 `.mblock` → 打開 mBlock 5 → 「檔案 → 從電腦打開」→ 選檔案 → 右下切到「上傳」→ 按「上傳」 |
| 🔧 **Arduino IDE**（文字程式） | 想看積木背後長什麼樣、或自己寫程式的人 | 下載 `.ino` → 裝好 Makeblock 與 MBC_Sensors 函式庫（見 [MakeBlock 積木擴展](/makeblock.md)）→ 主板選 Arduino Uno → 上傳 |

> [!IMPORTANT]
> - 感應器要是 **MakeBlock 版、2026-10 之後的協定 v2 韌體**，經 **mBot 插座片**接 RJ25 連接埠；擴展要用 **v2.0.0**（舊韌體配舊版 v1 擴展，兩者不能混用）。
> - 主板是 **mBot（mCore）**。mBot Ranger、Orion、MegaPi 也能用，但要把「當 mBot 啟動時」換成那塊主板的起始積木、連接埠改成它能用的號碼（見 [支援的主板與連接埠](/makeblock.md)）；mBot2（CyberPi）沒有 RJ25，不支援。
> - 積木圖裡的控制與運算積木（forever、if…then…else、wait、set…to）以英文顯示，在 mBlock 的中文介面就是「重複無限次」「如果…那麼…否則」「等待…秒」「變數設為」。

## 開始之前

<figure class="ref-fig"><img src="/images/figures/esp32cam/wiring-mbot.svg" alt="神攝手（學校版） × mBot 接線" loading="lazy"></figure>

感應器 → mBot 插座片 → 主板 RJ25 連接埠（S1 = 主機送、S2 = 主機收，UART 38400）。

1. **裝擴展**：到 [MakeBlock 積木擴展](/makeblock.md) 取得 **MBC Pro Sensors v2.0.0** 的 `.mext`，拖進 mBlock 5 視窗；左下「設備」加入 **mBot**，右下切到「上傳」。
2. **接線**：神攝手 → mBot 插座片 → mBot **連接埠 1**（這頁所有範例都接這一埠；換埠就把積木上的連接埠數字改掉）。
3. **先開機再讀**：每個範例一開始都「等待 1 秒」讓感應器開機；太早讀會讀到 -32768（還沒收到）。
4. **在感應器上設定要看的顏色（程式改不了）**：先把感應器插上 mBot、開機，把要用的顏色積木擺在鏡頭前。畫面上方顯示 `RUN` 時**短按側鍵**進選單：`SET 1:COLOR` 設「色相」的取樣點（範例 1：用左／右把紅線、綠線移到積木上，容許範圍先用預設）；`SET 2:TRACK` 設要追的色塊（範例 2~5：十字對準色塊正中間，`TOL_HUE`／`TOL_STA` 調到只有你的積木被標記，`MIN AREA` 濾掉小雜點）。設定會記在感應器裡。
5. **補光燈先不要開**（積木會反光變白）。回到 `RUN` 畫面，螢幕上方會一直顯示 `H…（色相） A…（面積） X… Y…`，範例讀到的就是這幾個數字。

## 這頁會用到的積木

每顆讀值積木就是感應器值表裡的一個「值編號」（跟 Arduino I2C 版的暫存器編號相同）；完整積木表在 [MakeBlock 積木擴展](/makeblock.md)。

| 積木 | 傳回 | 意思 |
|---|---|---|
| 神攝手舊版 連接埠 ［1］ 色相 | -1~100 | S1 取樣點色相 0~100（色相環 0~240 等比縮放）；−1 = 黑白灰（飽和度 < 10）；S1 沒設定 = 0 |
| 神攝手舊版 連接埠 ［1］ 有找到色塊？ | 是／否 | 1 = 畫面裡有目標色的色塊（W > 0），0 = 沒有 |
| 神攝手舊版 連接埠 ［1］ 色塊 X | 0~100 | 色塊中心 X 0~100，左 = 0；沒找到 = 0 |
| 神攝手舊版 連接埠 ［1］ 色塊面積 | 0~7500 | W×H（0~7500）；沒找到 = 0 |

**這頁的範例**

1. [聽見顏色](#%E7%AF%84%E4%BE%8B-1%EF%BC%9A%E8%81%BD%E8%A6%8B%E9%A1%8F%E8%89%B2)
2. [找到色塊亮綠燈](#%E7%AF%84%E4%BE%8B-2%EF%BC%9A%E6%89%BE%E5%88%B0%E8%89%B2%E5%A1%8A%E4%BA%AE%E7%B6%A0%E7%87%88)
3. [一個輪子轉向色塊](#%E7%AF%84%E4%BE%8B-3%EF%BC%9A%E4%B8%80%E5%80%8B%E8%BC%AA%E5%AD%90%E8%BD%89%E5%90%91%E8%89%B2%E5%A1%8A)
4. [追著色塊走](#%E7%AF%84%E4%BE%8B-4%EF%BC%9A%E8%BF%BD%E8%91%97%E8%89%B2%E5%A1%8A%E8%B5%B0)
5. [追色塊、靠近就停＋燈號](#%E7%AF%84%E4%BE%8B-5%EF%BC%9A%E8%BF%BD%E8%89%B2%E5%A1%8A%E3%80%81%E9%9D%A0%E8%BF%91%E5%B0%B1%E5%81%9C%EF%BC%8B%E7%87%88%E8%99%9F)

> [!NOTE]
> `.mblock` 是由產生器照 mBlock 5 的存檔格式產生的，**還沒在 mBlock 5 裡開啟驗證**（等擴展 v2 上架後補驗）。開不起來或積木顯示空白，請先確認 v2 擴展已拖進 mBlock、設備是 mBot，或照下面的積木圖自己拼（5 分鐘內拼得完）；也歡迎 [LINE 告訴我們](https://line.me/R/ti/p/@692vcvuk)。

---

## 範例 1：聽見顏色

**這個範例會做什麼**：mBot 沒有螢幕，這個範例把取樣點的「色相」（0 ~ 100：紅 0、黃約 17、綠約 33、藍約 67；黑白灰是 −1）變成蜂鳴器的音高。先學會「讀到數字」。

**要準備什麼**
- mBot（mCore 主板）＋ mBlock 5 電腦版
- 神攝手（學校版）（MakeBlock 版）＋ mBot 插座片
- 一塊顏色鮮豔的積木（紅、綠、藍都可以）

**怎麼接**：神攝手 → mBot 插座片 → mBot **連接埠 1**。鏡頭朝前、裝在車頭。先照上面「開始之前」在感應器上設定要看的顏色。

**下載程式**

| mBlock 5 專案 | 等價 Arduino 程式 |
|---|---|
| [ex01_hear_colour.mblock](/examples/esp32cam/mblock/ex01_hear_colour.mblock) | [ex01_hear_colour.ino](/examples/esp32cam/mblock/ex01_hear_colour/ex01_hear_colour.ino) |

<details open>
<summary>🧱 mBlock 積木（照著拼就好）</summary>

<pre class="blocks">當 mBot 啟動時 :: events hat
wait (1) seconds
forever
  播放音頻 ((200) + ((神攝手舊版 連接埠 [1 v] 色相 :: #C2255C) * (6))) 赫茲，持續 (0.1) 秒 :: sound
end</pre>

</details>

<details>
<summary>🔧 等價 Arduino 程式（mBot）</summary>

```cpp
// 神攝手（學校版） × mBlock 5　範例 1：聽見顏色（mBot 等價 Arduino 程式）
// 接線：神攝手 → mBot 插座片 → mBot 連接埠 1。鏡頭朝前、裝在車頭。先照上面「開始之前」在感應器上設定要看的顏色。
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

// 左／右輪動力 -100~100（正 = 往前）：mBot 的左輪裝反，所以 M1 給負的；% 換算成 PWM 0~255
void wheels(int l, int r) {
  l = constrain(l, -100, 100); r = constrain(r, -100, 100);
  motorL.run(-l * 255 / 100);
  motorR.run(r * 255 / 100);
}

void setup() {   // 跟 mBlock 產生的程式一樣，積木全部在 setup() 裡跑
  mbc_port1.begin();
  delay(1000);   // 等 1 秒讓感應器開機
  while (1) {   // 一直重複
    buzzer.tone((200 + (mbcRead(mbc_port1, 0x01) * 6)), 100);   // 音高 ＝ 200 ＋ 色相 × 6 赫茲，響 0.1 秒
  }
}

void loop() {}   // 這裡什麼都沒有：上面的積木不會結束
```

</details>

**一步一步看懂程式**
1. 等 1 秒讓感應器開機
2. 一直重複
   - 音高 ＝ 200 ＋ 色相 × 6 赫茲，響 0.1 秒

**跑起來應該看到什麼**：紅色積木放在取樣點：200 赫茲低音；綠色約 400 赫茲；藍色約 600 赫茲；白紙或黑色是最低音（194 赫茲）。

**常見問題**
- **聽不到聲音？** → mBot 的蜂鳴器在主板上，聲音不大；把耳朵靠近，或把 0.1 秒改成 0.3 秒。
- **顏色換了音沒變？** → 積木沒有放在取樣點（感應器螢幕上的十字）；用「開始之前」的 COLOR 設定把十字移到積木會經過的地方。
- **一直「沒找到」？** → 感應器上的 TRACK 設定還沒做、或容許範圍太小。看感應器螢幕：有追蹤框才算找到。
- **什麼都沒反應、或讀值一直是 -32768？** → 連接埠選錯（這頁都接連接埠 1）、插座片沒插緊、或感應器不是 MakeBlock 版。先用「MBC 共用 → 連線正常？」積木確認；「協議版本」讀到 2 才是新韌體（舊韌體請改用舊版擴展 v1）。
- **上傳失敗、或積木是空白的？** → 確認右下角切到「上傳」模式、擴展 v2 已拖進 mBlock、USB 線接的是 mBot 主板。

---

## 範例 2：找到色塊亮綠燈

**這個範例會做什麼**：用「有找到色塊？」布林積木和「如果…就…否則…」：畫面裡有設定的顏色就亮綠燈，沒有就亮紅燈。這是追色塊程式的第一個判斷。

**要準備什麼**
- mBot（mCore 主板）＋ mBlock 5 電腦版
- 神攝手（學校版）（MakeBlock 版）＋ mBot 插座片
- 一塊顏色鮮豔的積木（紅、綠、藍都可以）

**怎麼接**：神攝手 → mBot 插座片 → mBot **連接埠 1**。鏡頭朝前、裝在車頭。先照上面「開始之前」在感應器上設定要看的顏色。

**下載程式**

| mBlock 5 專案 | 等價 Arduino 程式 |
|---|---|
| [ex02_found_led.mblock](/examples/esp32cam/mblock/ex02_found_led.mblock) | [ex02_found_led.ino](/examples/esp32cam/mblock/ex02_found_led/ex02_found_led.ino) |

<details open>
<summary>🧱 mBlock 積木（照著拼就好）</summary>

<pre class="blocks">當 mBot 啟動時 :: events hat
wait (1) seconds
forever
  if &lt;神攝手舊版 連接埠 [1 v] 有找到色塊？ :: #C2255C&gt; then
    LED 燈位置 [全部 v] 的三原色數值為 紅 (0) 綠 (255) 藍 (0) :: looks
  else
    LED 燈位置 [全部 v] 的三原色數值為 紅 (255) 綠 (0) 藍 (0) :: looks
  end
end</pre>

</details>

<details>
<summary>🔧 等價 Arduino 程式（mBot）</summary>

```cpp
// 神攝手（學校版） × mBlock 5　範例 2：找到色塊亮綠燈（mBot 等價 Arduino 程式）
// 接線：神攝手 → mBot 插座片 → mBot 連接埠 1。鏡頭朝前、裝在車頭。先照上面「開始之前」在感應器上設定要看的顏色。
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

// 左／右輪動力 -100~100（正 = 往前）：mBot 的左輪裝反，所以 M1 給負的；% 換算成 PWM 0~255
void wheels(int l, int r) {
  l = constrain(l, -100, 100); r = constrain(r, -100, 100);
  motorL.run(-l * 255 / 100);
  motorR.run(r * 255 / 100);
}

void setup() {   // 跟 mBlock 產生的程式一樣，積木全部在 setup() 裡跑
  mbc_port1.begin();
  delay(1000);   // 等 1 秒讓感應器開機
  while (1) {   // 一直重複
    if ((mbcRead(mbc_port1, 0x0A) == 1)) {   // 如果有找到色塊
      rgbled.setColor(0, 0, 255, 0); rgbled.show();   // 有找到色塊：綠燈
    } else {
      rgbled.setColor(0, 255, 0, 0); rgbled.show();   // 沒找到：紅燈
    }
  }
}

void loop() {}   // 這裡什麼都沒有：上面的積木不會結束
```

</details>

**一步一步看懂程式**
1. 等 1 秒讓感應器開機
2. 一直重複
   - 如果有找到色塊
      - 有找到色塊：綠燈
      - 否則：
         - 沒找到：紅燈

**跑起來應該看到什麼**：把積木拿到鏡頭前：綠燈（感應器螢幕上也會出現追蹤框）；拿開：紅燈。

**常見問題**
- **一直「沒找到」？** → 感應器上的 TRACK 設定還沒做、或容許範圍太小。看感應器螢幕：有追蹤框才算找到。
- **什麼都沒反應、或讀值一直是 -32768？** → 連接埠選錯（這頁都接連接埠 1）、插座片沒插緊、或感應器不是 MakeBlock 版。先用「MBC 共用 → 連線正常？」積木確認；「協議版本」讀到 2 才是新韌體（舊韌體請改用舊版擴展 v1）。
- **上傳失敗、或積木是空白的？** → 確認右下角切到「上傳」模式、擴展 v2 已拖進 mBlock、USB 線接的是 mBot 主板。

---

## 範例 3：一個輪子轉向色塊

**這個範例會做什麼**：「色塊 X」是色塊在畫面的左右位置（0 最左、50 中間、100 最右）。只用左輪：動力 ＝（色塊 X − 50）× 0.8，色塊在右邊左輪往前轉、車頭往右；在左邊倒轉、車頭往左；在中間不動。沒找到就停。

**要準備什麼**
- mBot（mCore 主板）＋ mBlock 5 電腦版
- mBot 兩輪小車（左輪接 M1、右輪接 M2，出廠就是這樣接）
- 神攝手（學校版）（MakeBlock 版）＋ mBot 插座片
- 一塊顏色鮮豔的積木（紅、綠、藍都可以）

**怎麼接**：神攝手 → mBot 插座片 → mBot **連接埠 1**。鏡頭朝前、裝在車頭。先照上面「開始之前」在感應器上設定要看的顏色。

**下載程式**

| mBlock 5 專案 | 等價 Arduino 程式 |
|---|---|
| [ex03_one_wheel.mblock](/examples/esp32cam/mblock/ex03_one_wheel.mblock) | [ex03_one_wheel.ino](/examples/esp32cam/mblock/ex03_one_wheel/ex03_one_wheel.ino) |

<details open>
<summary>🧱 mBlock 積木（照著拼就好）</summary>

<pre class="blocks">當 mBot 啟動時 :: events hat
wait (1) seconds
forever
  if &lt;神攝手舊版 連接埠 [1 v] 有找到色塊？ :: #C2255C&gt; then
    左輪動力 (((神攝手舊版 連接埠 [1 v] 色塊 X :: #C2255C) - (50)) * (0.8)) %，右輪動力 (0) % :: motion
  else
    停止移動 :: motion
  end
end</pre>

</details>

<details>
<summary>🔧 等價 Arduino 程式（mBot）</summary>

```cpp
// 神攝手（學校版） × mBlock 5　範例 3：一個輪子轉向色塊（mBot 等價 Arduino 程式）
// 接線：神攝手 → mBot 插座片 → mBot 連接埠 1。鏡頭朝前、裝在車頭。先照上面「開始之前」在感應器上設定要看的顏色。
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

// 左／右輪動力 -100~100（正 = 往前）：mBot 的左輪裝反，所以 M1 給負的；% 換算成 PWM 0~255
void wheels(int l, int r) {
  l = constrain(l, -100, 100); r = constrain(r, -100, 100);
  motorL.run(-l * 255 / 100);
  motorR.run(r * 255 / 100);
}

void setup() {   // 跟 mBlock 產生的程式一樣，積木全部在 setup() 裡跑
  mbc_port1.begin();
  delay(1000);   // 等 1 秒讓感應器開機
  while (1) {   // 一直重複
    if ((mbcRead(mbc_port1, 0x0A) == 1)) {   // 如果有找到色塊
      wheels(((mbcRead(mbc_port1, 0x05) - 50) * 0.8), 0);   // 左輪動力 ＝（色塊 X − 50）× 0.8（−40 ~ +40），右輪 0
    } else {
      wheels(0, 0);   // 沒找到：停
    }
  }
}

void loop() {}   // 這裡什麼都沒有：上面的積木不會結束
```

</details>

**一步一步看懂程式**
1. 等 1 秒讓感應器開機
2. 一直重複
   - 如果有找到色塊
      - 左輪動力 ＝（色塊 X − 50）× 0.8（−40 ~ +40），右輪 0
      - 否則：
         - 沒找到：停

**跑起來應該看到什麼**：把積木拿到車子右前方：左輪往前轉、車頭轉向積木，對準就停；移到左邊，左輪倒轉。

**常見問題**
- **車子往反方向轉、越修越歪？** → 左右輪的「＋」「－」對調（或把左輪、右輪的動力互換）。mBot 出廠左輪 M1、右輪 M2，自己接線的請確認。
- **畫面左右跟實際相反？** → 鏡頭設定裡的 H-Mirror 開了，關掉；或把 0.8 改成 -0.8。
- **一直「沒找到」？** → 感應器上的 TRACK 設定還沒做、或容許範圍太小。看感應器螢幕：有追蹤框才算找到。
- **什麼都沒反應、或讀值一直是 -32768？** → 連接埠選錯（這頁都接連接埠 1）、插座片沒插緊、或感應器不是 MakeBlock 版。先用「MBC 共用 → 連線正常？」積木確認；「協議版本」讀到 2 才是新韌體（舊韌體請改用舊版擴展 v1）。
- **上傳失敗、或積木是空白的？** → 確認右下角切到「上傳」模式、擴展 v2 已拖進 mBlock、USB 線接的是 mBot 主板。

---

## 範例 4：追著色塊走

**這個範例會做什麼**：範例 3 換成兩個輪子：轉向 ＝（色塊 X − 50）× 0.6，左輪 35 ＋ 轉向、右輪 35 − 轉向，車子會一邊往前一邊把色塊對到畫面中間，也就是跟著積木走。沒找到就停。

**要準備什麼**
- mBot（mCore 主板）＋ mBlock 5 電腦版
- mBot 兩輪小車（左輪接 M1、右輪接 M2，出廠就是這樣接）
- 神攝手（學校版）（MakeBlock 版）＋ mBot 插座片
- 一塊顏色鮮豔的積木（紅、綠、藍都可以）

**怎麼接**：神攝手 → mBot 插座片 → mBot **連接埠 1**。鏡頭朝前、裝在車頭。先照上面「開始之前」在感應器上設定要看的顏色。

**下載程式**

| mBlock 5 專案 | 等價 Arduino 程式 |
|---|---|
| [ex04_follow.mblock](/examples/esp32cam/mblock/ex04_follow.mblock) | [ex04_follow.ino](/examples/esp32cam/mblock/ex04_follow/ex04_follow.ino) |

<details open>
<summary>🧱 mBlock 積木（照著拼就好）</summary>

<pre class="blocks">當 mBot 啟動時 :: events hat
wait (1) seconds
forever
  if &lt;神攝手舊版 連接埠 [1 v] 有找到色塊？ :: #C2255C&gt; then
    set [轉向 v] to (((神攝手舊版 連接埠 [1 v] 色塊 X :: #C2255C) - (50)) * (0.6))
    左輪動力 ((35) + (轉向)) %，右輪動力 ((35) - (轉向)) % :: motion
  else
    停止移動 :: motion
  end
end</pre>

</details>

<details>
<summary>🔧 等價 Arduino 程式（mBot）</summary>

```cpp
// 神攝手（學校版） × mBlock 5　範例 4：追著色塊走（mBot 等價 Arduino 程式）
// 接線：神攝手 → mBot 插座片 → mBot 連接埠 1。鏡頭朝前、裝在車頭。先照上面「開始之前」在感應器上設定要看的顏色。
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
float turn = 0;               // 變數：轉向

// 左／右輪動力 -100~100（正 = 往前）：mBot 的左輪裝反，所以 M1 給負的；% 換算成 PWM 0~255
void wheels(int l, int r) {
  l = constrain(l, -100, 100); r = constrain(r, -100, 100);
  motorL.run(-l * 255 / 100);
  motorR.run(r * 255 / 100);
}

void setup() {   // 跟 mBlock 產生的程式一樣，積木全部在 setup() 裡跑
  mbc_port1.begin();
  delay(1000);   // 等 1 秒讓感應器開機
  while (1) {   // 一直重複
    if ((mbcRead(mbc_port1, 0x0A) == 1)) {   // 如果有找到色塊
      turn = ((mbcRead(mbc_port1, 0x05) - 50) * 0.6);   // 轉向 ＝（色塊 X − 50）× 0.6（−30 ~ +30）
      wheels((35 + turn), (35 - turn));   // 左輪 ＝ 35 ＋ 轉向、右輪 ＝ 35 − 轉向：色塊在右（正）就左輪快、往右轉
    } else {
      wheels(0, 0);   // 沒找到：停
    }
  }
}

void loop() {}   // 這裡什麼都沒有：上面的積木不會結束
```

</details>

**一步一步看懂程式**
1. 等 1 秒讓感應器開機
2. 一直重複
   - 如果有找到色塊
      - 轉向 ＝（色塊 X − 50）× 0.6（−30 ~ +30）
      - 左輪 ＝ 35 ＋ 轉向、右輪 ＝ 35 − 轉向：色塊在右（正）就左輪快、往右轉
      - 否則：
         - 沒找到：停

**跑起來應該看到什麼**：拿著積木在車子前面慢慢移動，車子會跟著走；把積木藏起來車子就停。

**常見問題**
- **車子往反方向轉、越修越歪？** → 左右輪的「＋」「－」對調（或把左輪、右輪的動力互換）。mBot 出廠左輪 M1、右輪 M2，自己接線的請確認。
- **車子左右搖？** → 把 0.6 改小（例如 0.4）。
- **一直「沒找到」？** → 感應器上的 TRACK 設定還沒做、或容許範圍太小。看感應器螢幕：有追蹤框才算找到。
- **什麼都沒反應、或讀值一直是 -32768？** → 連接埠選錯（這頁都接連接埠 1）、插座片沒插緊、或感應器不是 MakeBlock 版。先用「MBC 共用 → 連線正常？」積木確認；「協議版本」讀到 2 才是新韌體（舊韌體請改用舊版擴展 v1）。
- **上傳失敗、或積木是空白的？** → 確認右下角切到「上傳」模式、擴展 v2 已拖進 mBlock、USB 線接的是 mBot 主板。

---

## 範例 5：追色塊、靠近就停＋燈號

**這個範例會做什麼**：把範例 2 和 4 合起來，再加「色塊面積」：沒找到停車亮紅燈；面積大於 2500（積木很近了）停車亮藍燈、嗶一聲；其他時候追色塊亮綠燈。這就是「找到目標 → 開過去 → 到了停下」的完整流程。

**要準備什麼**
- mBot（mCore 主板）＋ mBlock 5 電腦版
- mBot 兩輪小車（左輪接 M1、右輪接 M2，出廠就是這樣接）
- 神攝手（學校版）（MakeBlock 版）＋ mBot 插座片
- 一塊顏色鮮豔的積木（紅、綠、藍都可以）

**怎麼接**：神攝手 → mBot 插座片 → mBot **連接埠 1**。鏡頭朝前、裝在車頭。先照上面「開始之前」在感應器上設定要看的顏色。

**下載程式**

| mBlock 5 專案 | 等價 Arduino 程式 |
|---|---|
| [ex05_follow_stop.mblock](/examples/esp32cam/mblock/ex05_follow_stop.mblock) | [ex05_follow_stop.ino](/examples/esp32cam/mblock/ex05_follow_stop/ex05_follow_stop.ino) |

<details open>
<summary>🧱 mBlock 積木（照著拼就好）</summary>

<pre class="blocks">當 mBot 啟動時 :: events hat
wait (1) seconds
forever
  if &lt;神攝手舊版 連接埠 [1 v] 有找到色塊？ :: #C2255C&gt; then
    if &lt;(神攝手舊版 連接埠 [1 v] 色塊面積 :: #C2255C) &gt; (2500)&gt; then
      停止移動 :: motion
      LED 燈位置 [全部 v] 的三原色數值為 紅 (0) 綠 (0) 藍 (255) :: looks
      播放音頻 (880) 赫茲，持續 (0.3) 秒 :: sound
    else
      LED 燈位置 [全部 v] 的三原色數值為 紅 (0) 綠 (255) 藍 (0) :: looks
      set [轉向 v] to (((神攝手舊版 連接埠 [1 v] 色塊 X :: #C2255C) - (50)) * (0.6))
      左輪動力 ((35) + (轉向)) %，右輪動力 ((35) - (轉向)) % :: motion
    end
  else
    停止移動 :: motion
    LED 燈位置 [全部 v] 的三原色數值為 紅 (255) 綠 (0) 藍 (0) :: looks
  end
end</pre>

</details>

<details>
<summary>🔧 等價 Arduino 程式（mBot）</summary>

```cpp
// 神攝手（學校版） × mBlock 5　範例 5：追色塊、靠近就停＋燈號（mBot 等價 Arduino 程式）
// 接線：神攝手 → mBot 插座片 → mBot 連接埠 1。鏡頭朝前、裝在車頭。先照上面「開始之前」在感應器上設定要看的顏色。
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
float turn = 0;               // 變數：轉向

// 左／右輪動力 -100~100（正 = 往前）：mBot 的左輪裝反，所以 M1 給負的；% 換算成 PWM 0~255
void wheels(int l, int r) {
  l = constrain(l, -100, 100); r = constrain(r, -100, 100);
  motorL.run(-l * 255 / 100);
  motorR.run(r * 255 / 100);
}

void setup() {   // 跟 mBlock 產生的程式一樣，積木全部在 setup() 裡跑
  mbc_port1.begin();
  delay(1000);   // 等 1 秒讓感應器開機
  while (1) {   // 一直重複
    if ((mbcRead(mbc_port1, 0x0A) == 1)) {   // 如果有找到色塊
      if ((mbcRead(mbc_port1, 0x09) > 2500)) {   // 如果色塊面積大於 2500
        wheels(0, 0);   // 色塊很大（很近了）：停車
        rgbled.setColor(0, 0, 0, 255); rgbled.show();   // 藍燈
        buzzer.tone(880, 300);   // 嗶一聲
      } else {
        rgbled.setColor(0, 0, 255, 0); rgbled.show();   // 綠燈
        turn = ((mbcRead(mbc_port1, 0x05) - 50) * 0.6);   // 轉向 ＝（色塊 X − 50）× 0.6
        wheels((35 + turn), (35 - turn));   // 左輪 ＝ 35 ＋ 轉向、右輪 ＝ 35 − 轉向
      }
    } else {
      wheels(0, 0);   // 沒找到：停車
      rgbled.setColor(0, 255, 0, 0); rgbled.show();   // 紅燈
    }
  }
}

void loop() {}   // 這裡什麼都沒有：上面的積木不會結束
```

</details>

**一步一步看懂程式**
1. 等 1 秒讓感應器開機
2. 一直重複
   - 如果有找到色塊
      - 如果色塊面積大於 2500
         - 色塊很大（很近了）：停車
         - 藍燈
         - 嗶一聲
         - 否則：
            - 綠燈
            - 轉向 ＝（色塊 X − 50）× 0.6
            - 左輪 ＝ 35 ＋ 轉向、右輪 ＝ 35 − 轉向
      - 否則：
         - 沒找到：停車
         - 紅燈

**跑起來應該看到什麼**：積木放在前方 1 公尺：車子亮綠燈開過去，到積木前面變藍燈、嗶一聲停下；把積木拿走變紅燈。

**常見問題**
- **還很遠就停了、或撞到積木才停？** → 面積跟積木大小有關：先用感應器螢幕看積木在想停的距離時 A（面積）是多少，再改 2500。
- **車子往反方向轉、越修越歪？** → 左右輪的「＋」「－」對調（或把左輪、右輪的動力互換）。mBot 出廠左輪 M1、右輪 M2，自己接線的請確認。
- **一直「沒找到」？** → 感應器上的 TRACK 設定還沒做、或容許範圍太小。看感應器螢幕：有追蹤框才算找到。
- **什麼都沒反應、或讀值一直是 -32768？** → 連接埠選錯（這頁都接連接埠 1）、插座片沒插緊、或感應器不是 MakeBlock 版。先用「MBC 共用 → 連線正常？」積木確認；「協議版本」讀到 2 才是新韌體（舊韌體請改用舊版擴展 v1）。
- **上傳失敗、或積木是空白的？** → 確認右下角切到「上傳」模式、擴展 v2 已拖進 mBlock、USB 線接的是 mBot 主板。

---

## 下一步

- 全部 神攝手積木、各主板可用的連接埠：[MakeBlock 積木擴展](/makeblock.md)；這款產品的 mBlock 積木表：[神攝手（學校版） × mBlock 5](/sensors/esp32cam/mblock.md)
- 同樣的 5 個範例在樂高上怎麼寫：[SPIKE 新手範例](/sensors/esp32cam/spike-examples.md)、[EV3 新手範例](/sensors/esp32cam/ev3-examples.md)
- 想用 Arduino 直接接感應器（I2C 版，不經 mBot）：[Arduino 範例](/sensors/esp32cam/arduino-examples.md)
