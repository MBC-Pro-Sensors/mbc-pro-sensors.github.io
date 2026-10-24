<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 95_產品網站\mBlock範例產生器（node build.js）產生：改內容請改產生器的 sets/<產品>.js 再重建，不要手改本檔 -->
<!-- moved: /sensors/line16/mblock/mblock5.md -->
<p class="moved-note">這頁內容已整合到新的說明頁：<a href="/sensors/line16/mblock/mblock5.md">循行者 16 路 › mBlock › mBlock 5</a></p>

# 🧱 mBlock 新手範例：循行者 16 路

[🔙 回到循行者 16 路產品頁](/sensors/line16/index.md)

這一頁是寫給**完全沒寫過程式**的人：用 mBlock 5 的積木在 **mBot** 上讀循行者 16 路。5 個範例從「把數字變成聲音」開始，一步一步做到會自己跑的車子。每個範例都有積木圖、可下載的專案檔，還有一支做同樣事情的 Arduino 程式。

| 寫法 | 適合誰 | 怎麼打開範例檔 |
|---|---|---|
| 🟧 **mBlock 5 電腦版**（積木，上傳模式）＋ **MBC 感應器擴展 v2** | 學校、第一次用 mBot 的人 | 下載 `.mblock` → 打開 mBlock 5 → 「檔案 → 從電腦打開」→ 選檔案 → 右下切到「上傳」→ 按「上傳」 |
| 🔧 **Arduino IDE**（文字程式） | 想看積木背後長什麼樣、或自己寫程式的人 | 下載 `.ino` → 裝好 Makeblock 與 MBC_Sensors 函式庫（見 [MakeBlock 積木擴展](/makeblock.md)）→ 主板選 Arduino Uno → 上傳 |

> [!IMPORTANT]
> - 感應器要是 **MakeBlock 版、2026-10 之後的協定 v2 韌體**，經 **mBot 插座片**接 RJ25 連接埠；擴展要用 **v2.0.0**（舊韌體配舊版 v1 擴展，兩者不能混用）。
> - 主板是 **mBot（mCore）**。mBot Ranger、Orion、MegaPi 也能用，但要把「當 mBot 啟動時」換成那塊主板的起始積木、連接埠改成它能用的號碼（見 [支援的主板與連接埠](/makeblock.md)）；mBot2（CyberPi）沒有 RJ25，不支援。
> - 積木圖裡的控制與運算積木（forever、if…then…else、wait、set…to）以英文顯示，在 mBlock 的中文介面就是「重複無限次」「如果…那麼…否則」「等待…秒」「變數設為」。

## 開始之前

<figure class="ref-fig"><img src="/images/figures/line16/wiring-mbot.svg" alt="循行者 16 路 × mBot 接線" loading="lazy"></figure>

感應器 → mBot 插座片 → 主板 RJ25 連接埠（S1 = 主機送、S2 = 主機收，UART 38400）。

1. **裝擴展**：到 [MakeBlock 積木擴展](/makeblock.md) 取得 **MBC Pro Sensors v2.0.0** 的 `.mext`，拖進 mBlock 5 視窗；左下「設備」加入 **mBot**，右下切到「上傳」。
2. **接線**：循行者 → mBot 插座片 → mBot **連接埠 1**（這頁所有範例都接這一埠；換埠就把積木上的連接埠數字改掉）。
3. **先開機再讀**：每個範例一開始都「等待 1 秒」讓感應器開機；太早讀會讀到 -32768（還沒收到）。
4. **先校準（只要做一次）**：感應器插好、mBot 開機後，**按一下**感應器上的校準按鈕放開，綠燈亮起的 **5 秒內**把感應器在黑線和白色地面之間來回掃幾次（16 個感應點都要掃過黑和白），綠燈熄滅＝完成，會記在感應器裡。也可以用積木「循行者 連接埠 1 5 秒動態校準」做同一件事。
5. **怎麼知道校準好了？** 跑範例 2：放在白色地面上燈熄掉（線寬 0），壓在黑線上有燈。

## 這頁會用到的積木

每顆讀值積木就是感應器值表裡的一個「值編號」（跟 Arduino I2C 版的暫存器編號相同）；完整積木表在 [MakeBlock 積木擴展](/makeblock.md)。

| 積木 | 傳回 | 意思 |
|---|---|---|
| 循行者 連接埠 ［1］ 線的細位置 | -100~+100 | 線位置的細分：−100 ~ +100，0 = 正中間，負 = 線在左（與線位置同號） |
| 循行者 連接埠 ［1］ 線寬 | 0~16 路 | 壓到線的路數 0~16，0 = 找不到線 |
| 循行者 連接埠 ［1］ 線的位置 | -16~+16 | 線在哪一路：−16（最左）~+16（最右），0 = 正中間；站在車後往前看，負 = 線在左（平常 ±15，±16 只在最外側掉線後 200 ms 內） |

**這頁的範例**

1. [聽見線在哪裡](#%E7%AF%84%E4%BE%8B-1%EF%BC%9A%E8%81%BD%E8%A6%8B%E7%B7%9A%E5%9C%A8%E5%93%AA%E8%A3%A1)
2. [線在哪邊就亮什麼燈](#%E7%AF%84%E4%BE%8B-2%EF%BC%9A%E7%B7%9A%E5%9C%A8%E5%93%AA%E9%82%8A%E5%B0%B1%E4%BA%AE%E4%BB%80%E9%BA%BC%E7%87%88)
3. [線的位置控制一個輪子](#%E7%AF%84%E4%BE%8B-3%EF%BC%9A%E7%B7%9A%E7%9A%84%E4%BD%8D%E7%BD%AE%E6%8E%A7%E5%88%B6%E4%B8%80%E5%80%8B%E8%BC%AA%E5%AD%90)
4. [三段式循線](#%E7%AF%84%E4%BE%8B-4%EF%BC%9A%E4%B8%89%E6%AE%B5%E5%BC%8F%E5%BE%AA%E7%B7%9A)
5. [比例循線＋丟線停車](#%E7%AF%84%E4%BE%8B-5%EF%BC%9A%E6%AF%94%E4%BE%8B%E5%BE%AA%E7%B7%9A%EF%BC%8B%E4%B8%9F%E7%B7%9A%E5%81%9C%E8%BB%8A)

> [!NOTE]
> `.mblock` 是由產生器照 mBlock 5 的存檔格式產生的，**還沒在 mBlock 5 裡開啟驗證**（等擴展 v2 上架後補驗）。開不起來或積木顯示空白，請先確認 v2 擴展已拖進 mBlock、設備是 mBot，或照下面的積木圖自己拼（5 分鐘內拼得完）；也歡迎 [LINE 告訴我們](https://line.me/R/ti/p/@692vcvuk)。

---

## 範例 1：聽見線在哪裡

**這個範例會做什麼**：mBot 沒有螢幕，這個範例把「線的細位置」（−100 ~ +100）變成蜂鳴器的音高：線在正中間是 440 赫茲（La），往左音變低、往右音變高。先學會「讀到數字」，後面的循線才看得懂。

**要準備什麼**
- mBot（mCore 主板）＋ mBlock 5 電腦版
- 循行者 16 路（MakeBlock 版）＋ mBot 插座片
- 白底黑線的場地（黑線寬約 2 公分）

**怎麼接**：循行者 → mBot 插座片 → mBot **連接埠 1**。感應器裝在車頭下方、離地約 1 公分，感應器正對地面。

**下載程式**

| mBlock 5 專案 | 等價 Arduino 程式 |
|---|---|
| [ex01_hear_line.mblock](/examples/line16/mblock/ex01_hear_line.mblock) | [ex01_hear_line.ino](/examples/line16/mblock/ex01_hear_line/ex01_hear_line.ino) |

<details open>
<summary>🧱 mBlock 積木（照著拼就好）</summary>

<pre class="blocks">當 mBot 啟動時 :: events hat
wait (1) seconds
forever
  播放音頻 ((440) + ((循行者 連接埠 [1 v] 線的細位置 :: #9300DB) * (2))) 赫茲，持續 (0.1) 秒 :: sound
end</pre>

</details>

<details>
<summary>🔧 等價 Arduino 程式（mBot）</summary>

```cpp
// 循行者 16 路 × mBlock 5　範例 1：聽見線在哪裡（mBot 等價 Arduino 程式）
// 接線：循行者 → mBot 插座片 → mBot 連接埠 1。感應器裝在車頭下方、離地約 1 公分，感應器正對地面。
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
    buzzer.tone((440 + (mbcRead(mbc_port1, 0x02) * 2)), 100);   // 音高 ＝ 440 ＋ 線的細位置 × 2 赫茲，響 0.1 秒
  }
}

void loop() {}   // 這裡什麼都沒有：上面的積木不會結束
```

</details>

**一步一步看懂程式**
1. 等 1 秒讓感應器開機
2. 一直重複
   - 音高 ＝ 440 ＋ 線的細位置 × 2 赫茲，響 0.1 秒

**跑起來應該看到什麼**：黑線在感應器正中間：440 赫茲。慢慢把線往左移：音越來越低（最左 240 赫茲）；往右移越來越高（最右 640 赫茲）。把線拿走，音停在最後一次看到線的位置。

**常見問題**
- **聽不到聲音？** → mBot 的蜂鳴器在主板上，聲音不大；把耳朵靠近，或把 0.1 秒改成 0.3 秒。
- **音高跟線的左右相反？** → 左右是站在車子後面、面向車頭看的。把感應器轉 180 度裝就會反過來。
- **放在黑線上「線寬」還是 0？** → 還沒校準：照上面「開始之前」用感應器上的按鈕校準一次。
- **什麼都沒反應、或讀值一直是 -32768？** → 連接埠選錯（這頁都接連接埠 1）、插座片沒插緊、或感應器不是 MakeBlock 版。先用「MBC 共用 → 連線正常？」積木確認；「協議版本」讀到 2 才是新韌體（舊韌體請改用舊版擴展 v1）。
- **上傳失敗、或積木是空白的？** → 確認右下角切到「上傳」模式、擴展 v2 已拖進 mBlock、USB 線接的是 mBot 主板。

---

## 範例 2：線在哪邊就亮什麼燈

**這個範例會做什麼**：用「如果…就…否則…」判斷線的位置：偏左亮紅燈、偏右亮藍燈、在中間亮綠燈；線寬是 0（找不到線）就關燈。這就是循線程式的判斷骨架，下一個範例只是把燈換成輪子。

**要準備什麼**
- mBot（mCore 主板）＋ mBlock 5 電腦版
- 循行者 16 路（MakeBlock 版）＋ mBot 插座片
- 白底黑線的場地（黑線寬約 2 公分）

**怎麼接**：循行者 → mBot 插座片 → mBot **連接埠 1**。感應器裝在車頭下方、離地約 1 公分，感應器正對地面。

**下載程式**

| mBlock 5 專案 | 等價 Arduino 程式 |
|---|---|
| [ex02_led_side.mblock](/examples/line16/mblock/ex02_led_side.mblock) | [ex02_led_side.ino](/examples/line16/mblock/ex02_led_side/ex02_led_side.ino) |

<details open>
<summary>🧱 mBlock 積木（照著拼就好）</summary>

<pre class="blocks">當 mBot 啟動時 :: events hat
wait (1) seconds
forever
  if &lt;(循行者 連接埠 [1 v] 線寬 :: #9300DB) = (0)&gt; then
    LED 燈位置 [全部 v] 的三原色數值為 紅 (0) 綠 (0) 藍 (0) :: looks
  else
    if &lt;(循行者 連接埠 [1 v] 線的位置 :: #9300DB) &lt; (-4)&gt; then
      LED 燈位置 [全部 v] 的三原色數值為 紅 (255) 綠 (0) 藍 (0) :: looks
    else
      if &lt;(循行者 連接埠 [1 v] 線的位置 :: #9300DB) &gt; (4)&gt; then
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
// 循行者 16 路 × mBlock 5　範例 2：線在哪邊就亮什麼燈（mBot 等價 Arduino 程式）
// 接線：循行者 → mBot 插座片 → mBot 連接埠 1。感應器裝在車頭下方、離地約 1 公分，感應器正對地面。
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
    if ((mbcRead(mbc_port1, 0x03) == 0)) {   // 如果線寬 ＝ 0（找不到線）
      rgbled.setColor(0, 0, 0, 0); rgbled.show();   // 找不到線：關燈
    } else {
      if ((mbcRead(mbc_port1, 0x01) < -4)) {   // 否則，如果線的位置小於 -4（偏左）
        rgbled.setColor(0, 255, 0, 0); rgbled.show();   // 線偏左：紅燈
      } else {
        if ((mbcRead(mbc_port1, 0x01) > 4)) {   // 否則，如果線的位置大於 4（偏右）
          rgbled.setColor(0, 0, 0, 255); rgbled.show();   // 線偏右：藍燈
        } else {
          rgbled.setColor(0, 0, 255, 0); rgbled.show();   // 線在中間：綠燈
        }
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
   - 如果線寬 ＝ 0（找不到線）
      - 找不到線：關燈
      - 否則：
         - 否則，如果線的位置小於 -4（偏左）
            - 線偏左：紅燈
            - 否則：
               - 否則，如果線的位置大於 4（偏右）
                  - 線偏右：藍燈
                  - 否則：
                     - 線在中間：綠燈

**跑起來應該看到什麼**：線在正中間綠燈；往左移超過 4 格變紅燈、往右移變藍燈；放到白色地面上燈熄掉。

**常見問題**
- **燈的顏色怎麼換？** → 改「紅／綠／藍」三個數字（0~255），例如 255、255、0 是黃色。
- **放在黑線上「線寬」還是 0？** → 還沒校準：照上面「開始之前」用感應器上的按鈕校準一次。
- **什麼都沒反應、或讀值一直是 -32768？** → 連接埠選錯（這頁都接連接埠 1）、插座片沒插緊、或感應器不是 MakeBlock 版。先用「MBC 共用 → 連線正常？」積木確認；「協議版本」讀到 2 才是新韌體（舊韌體請改用舊版擴展 v1）。
- **上傳失敗、或積木是空白的？** → 確認右下角切到「上傳」模式、擴展 v2 已拖進 mBlock、USB 線接的是 mBot 主板。

---

## 範例 3：線的位置控制一個輪子

**這個範例會做什麼**：把「線的細位置」（−100 ~ +100）除以 2，直接當左輪的動力：線在右邊左輪往前轉、在左邊倒轉、正中間不動，偏越多轉越快。先把車子架高（輪子離地）看一個輪子怎麼跟著線走，下一個範例再放到地上。

**要準備什麼**
- mBot（mCore 主板）＋ mBlock 5 電腦版
- mBot 兩輪小車（左輪接 M1、右輪接 M2，出廠就是這樣接）
- 循行者 16 路（MakeBlock 版）＋ mBot 插座片
- 白底黑線的場地（黑線寬約 2 公分）

**怎麼接**：循行者 → mBot 插座片 → mBot **連接埠 1**。感應器裝在車頭下方、離地約 1 公分，感應器正對地面。

**下載程式**

| mBlock 5 專案 | 等價 Arduino 程式 |
|---|---|
| [ex03_one_wheel.mblock](/examples/line16/mblock/ex03_one_wheel.mblock) | [ex03_one_wheel.ino](/examples/line16/mblock/ex03_one_wheel/ex03_one_wheel.ino) |

<details open>
<summary>🧱 mBlock 積木（照著拼就好）</summary>

<pre class="blocks">當 mBot 啟動時 :: events hat
wait (1) seconds
forever
  左輪動力 ((循行者 連接埠 [1 v] 線的細位置 :: #9300DB) / (2)) %，右輪動力 (0) % :: motion
end</pre>

</details>

<details>
<summary>🔧 等價 Arduino 程式（mBot）</summary>

```cpp
// 循行者 16 路 × mBlock 5　範例 3：線的位置控制一個輪子（mBot 等價 Arduino 程式）
// 接線：循行者 → mBot 插座片 → mBot 連接埠 1。感應器裝在車頭下方、離地約 1 公分，感應器正對地面。
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
    wheels((mbcRead(mbc_port1, 0x02) / 2), 0);   // 左輪動力 ＝ 線的細位置 ÷ 2（−50 ~ +50），右輪 0
  }
}

void loop() {}   // 這裡什麼都沒有：上面的積木不會結束
```

</details>

**一步一步看懂程式**
1. 等 1 秒讓感應器開機
2. 一直重複
   - 左輪動力 ＝ 線的細位置 ÷ 2（−50 ~ +50），右輪 0

**跑起來應該看到什麼**：線在正中間左輪不動；線往右移左輪往前轉、越偏越快；線往左移左輪倒轉。

**常見問題**
- **車子往反方向轉、越修越歪？** → 左右輪的「＋」「－」對調（或把左輪、右輪的動力互換）。mBot 出廠左輪 M1、右輪 M2，自己接線的請確認。
- **線在中間輪子還會微微動？** → 細位置不一定剛好 0。想要不動，可以加一個「如果 線寬 ＝ 0 或 絕對值小於 6 就動力 0」。
- **放在黑線上「線寬」還是 0？** → 還沒校準：照上面「開始之前」用感應器上的按鈕校準一次。
- **什麼都沒反應、或讀值一直是 -32768？** → 連接埠選錯（這頁都接連接埠 1）、插座片沒插緊、或感應器不是 MakeBlock 版。先用「MBC 共用 → 連線正常？」積木確認；「協議版本」讀到 2 才是新韌體（舊韌體請改用舊版擴展 v1）。
- **上傳失敗、或積木是空白的？** → 確認右下角切到「上傳」模式、擴展 v2 已拖進 mBlock、USB 線接的是 mBot 主板。

---

## 範例 4：三段式循線

**這個範例會做什麼**：把範例 2 的燈換成輪子：線偏左就左輪慢、右輪快（往左修），偏右就反過來，在中間兩輪一樣快直走。車子會左右搖擺地沿著黑線走，這是最簡單、也最容易看懂的循線。

**要準備什麼**
- mBot（mCore 主板）＋ mBlock 5 電腦版
- mBot 兩輪小車（左輪接 M1、右輪接 M2，出廠就是這樣接）
- 循行者 16 路（MakeBlock 版）＋ mBot 插座片
- 白底黑線的場地（黑線寬約 2 公分）

**怎麼接**：循行者 → mBot 插座片 → mBot **連接埠 1**。感應器裝在車頭下方、離地約 1 公分，感應器正對地面。

**下載程式**

| mBlock 5 專案 | 等價 Arduino 程式 |
|---|---|
| [ex04_three_way.mblock](/examples/line16/mblock/ex04_three_way.mblock) | [ex04_three_way.ino](/examples/line16/mblock/ex04_three_way/ex04_three_way.ino) |

<details open>
<summary>🧱 mBlock 積木（照著拼就好）</summary>

<pre class="blocks">當 mBot 啟動時 :: events hat
wait (1) seconds
forever
  if &lt;(循行者 連接埠 [1 v] 線的位置 :: #9300DB) &lt; (-4)&gt; then
    左輪動力 (10) %，右輪動力 (40) % :: motion
  else
    if &lt;(循行者 連接埠 [1 v] 線的位置 :: #9300DB) &gt; (4)&gt; then
      左輪動力 (40) %，右輪動力 (10) % :: motion
    else
      左輪動力 (30) %，右輪動力 (30) % :: motion
    end
  end
end</pre>

</details>

<details>
<summary>🔧 等價 Arduino 程式（mBot）</summary>

```cpp
// 循行者 16 路 × mBlock 5　範例 4：三段式循線（mBot 等價 Arduino 程式）
// 接線：循行者 → mBot 插座片 → mBot 連接埠 1。感應器裝在車頭下方、離地約 1 公分，感應器正對地面。
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
    if ((mbcRead(mbc_port1, 0x01) < -4)) {   // 如果線的位置小於 -4（偏左）
      wheels(10, 40);   // 線偏左：左輪慢、右輪快，車子往左修
    } else {
      if ((mbcRead(mbc_port1, 0x01) > 4)) {   // 否則，如果線的位置大於 4（偏右）
        wheels(40, 10);   // 線偏右：左輪快、右輪慢，車子往右修
      } else {
        wheels(30, 30);   // 線在中間：兩輪一樣快，直直走
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
   - 如果線的位置小於 -4（偏左）
      - 線偏左：左輪慢、右輪快，車子往左修
      - 否則：
         - 否則，如果線的位置大於 4（偏右）
            - 線偏右：左輪快、右輪慢，車子往右修
            - 否則：
               - 線在中間：兩輪一樣快，直直走

**跑起來應該看到什麼**：車子左右搖擺地沿著黑線前進。彎太急衝出去的話，把 40 改小、10 改成 0。

**常見問題**
- **車子往反方向轉、越修越歪？** → 左右輪的「＋」「－」對調（或把左輪、右輪的動力互換）。mBot 出廠左輪 M1、右輪 M2，自己接線的請確認。
- **放在黑線上「線寬」還是 0？** → 還沒校準：照上面「開始之前」用感應器上的按鈕校準一次。
- **什麼都沒反應、或讀值一直是 -32768？** → 連接埠選錯（這頁都接連接埠 1）、插座片沒插緊、或感應器不是 MakeBlock 版。先用「MBC 共用 → 連線正常？」積木確認；「協議版本」讀到 2 才是新韌體（舊韌體請改用舊版擴展 v1）。
- **上傳失敗、或積木是空白的？** → 確認右下角切到「上傳」模式、擴展 v2 已拖進 mBlock、USB 線接的是 mBot 主板。

---

## 範例 5：比例循線＋丟線停車

**這個範例會做什麼**：三段式循線會一直搖擺。這個範例把「線的細位置 × 0.3」當轉彎的力量：偏一點轉一點、偏很多轉很多，走起來更順；同時用線寬判斷丟線：找不到線就停車亮紅燈，正常循線亮綠燈。這是比賽用循線程式的起點。

**要準備什麼**
- mBot（mCore 主板）＋ mBlock 5 電腦版
- mBot 兩輪小車（左輪接 M1、右輪接 M2，出廠就是這樣接）
- 循行者 16 路（MakeBlock 版）＋ mBot 插座片
- 白底黑線的場地（黑線寬約 2 公分）

**怎麼接**：循行者 → mBot 插座片 → mBot **連接埠 1**。感應器裝在車頭下方、離地約 1 公分，感應器正對地面。

**下載程式**

| mBlock 5 專案 | 等價 Arduino 程式 |
|---|---|
| [ex05_p_follow.mblock](/examples/line16/mblock/ex05_p_follow.mblock) | [ex05_p_follow.ino](/examples/line16/mblock/ex05_p_follow/ex05_p_follow.ino) |

<details open>
<summary>🧱 mBlock 積木（照著拼就好）</summary>

<pre class="blocks">當 mBot 啟動時 :: events hat
wait (1) seconds
forever
  if &lt;(循行者 連接埠 [1 v] 線寬 :: #9300DB) = (0)&gt; then
    停止移動 :: motion
    LED 燈位置 [全部 v] 的三原色數值為 紅 (255) 綠 (0) 藍 (0) :: looks
  else
    set [轉向 v] to ((循行者 連接埠 [1 v] 線的細位置 :: #9300DB) * (0.3))
    左輪動力 ((30) + (轉向)) %，右輪動力 ((30) - (轉向)) % :: motion
    LED 燈位置 [全部 v] 的三原色數值為 紅 (0) 綠 (255) 藍 (0) :: looks
  end
end</pre>

</details>

<details>
<summary>🔧 等價 Arduino 程式（mBot）</summary>

```cpp
// 循行者 16 路 × mBlock 5　範例 5：比例循線＋丟線停車（mBot 等價 Arduino 程式）
// 接線：循行者 → mBot 插座片 → mBot 連接埠 1。感應器裝在車頭下方、離地約 1 公分，感應器正對地面。
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
    if ((mbcRead(mbc_port1, 0x03) == 0)) {   // 如果線寬 ＝ 0（找不到線）
      wheels(0, 0);   // 找不到線：停車
      rgbled.setColor(0, 255, 0, 0); rgbled.show();   // 紅燈
    } else {
      turn = (mbcRead(mbc_port1, 0x02) * 0.3);   // 轉向 ＝ 線的細位置 × 0.3（−30 ~ +30）
      wheels((30 + turn), (30 - turn));   // 左輪 ＝ 30 ＋ 轉向、右輪 ＝ 30 − 轉向：線在左（負）就左輪慢、右輪快
      rgbled.setColor(0, 0, 255, 0); rgbled.show();   // 綠燈
    }
  }
}

void loop() {}   // 這裡什麼都沒有：上面的積木不會結束
```

</details>

**一步一步看懂程式**
1. 等 1 秒讓感應器開機
2. 一直重複
   - 如果線寬 ＝ 0（找不到線）
      - 找不到線：停車
      - 紅燈
      - 否則：
         - 轉向 ＝ 線的細位置 × 0.3（−30 ~ +30）
         - 左輪 ＝ 30 ＋ 轉向、右輪 ＝ 30 − 轉向：線在左（負）就左輪慢、右輪快
         - 綠燈

**跑起來應該看到什麼**：車子平順地沿著黑線走、亮綠燈；把車子拿離黑線，馬上停車亮紅燈。直線上還是左右甩的話，把 0.3 改小（例如 0.2）；轉不過彎就改大。

**常見問題**
- **車子往反方向轉、越修越歪？** → 左右輪的「＋」「－」對調（或把左輪、右輪的動力互換）。mBot 出廠左輪 M1、右輪 M2，自己接線的請確認。
- **十字路口也當成丟線停下來？** → 不會，路口線寬反而變大。想在路口停車，把「線寬 ＝ 0」改成「線寬 ＞ 5」試試看。
- **放在黑線上「線寬」還是 0？** → 還沒校準：照上面「開始之前」用感應器上的按鈕校準一次。
- **什麼都沒反應、或讀值一直是 -32768？** → 連接埠選錯（這頁都接連接埠 1）、插座片沒插緊、或感應器不是 MakeBlock 版。先用「MBC 共用 → 連線正常？」積木確認；「協議版本」讀到 2 才是新韌體（舊韌體請改用舊版擴展 v1）。
- **上傳失敗、或積木是空白的？** → 確認右下角切到「上傳」模式、擴展 v2 已拖進 mBlock、USB 線接的是 mBot 主板。

---

## 下一步

- 全部 循行者積木、各主板可用的連接埠：[MakeBlock 積木擴展](/makeblock.md)；這款產品的 mBlock 積木表：[循行者 16 路 × mBlock 5](/sensors/line16/mblock.md)
- 同樣的 5 個範例在樂高上怎麼寫：[SPIKE 新手範例](/sensors/line16/spike-examples.md)、[EV3 新手範例](/sensors/line16/ev3-examples.md)
- 想用 Arduino 直接接感應器（I2C 版，不經 mBot）：[Arduino 範例](/sensors/line16/arduino-examples.md)
