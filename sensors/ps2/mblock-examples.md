<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 95_產品網站\mBlock範例產生器（node build.js）產生：改內容請改產生器的 sets/<產品>.js 再重建，不要手改本檔 -->
<!-- moved: /sensors/ps2/mblock/mblock5.md -->
<p class="moved-note">這頁內容已整合到新的說明頁：<a href="/sensors/ps2/mblock/mblock5.md">掌控者 PS2 › mBlock › mBlock 5</a></p>

# 🧱 mBlock 新手範例：掌控者 PS2

[🔙 回到掌控者 PS2產品頁](/sensors/ps2/index.md)

這一頁是寫給**完全沒寫過程式**的人：用 mBlock 5 的積木在 **mBot** 上讀掌控者 PS2。5 個範例從「把數字變成聲音」開始，一步一步做到會自己跑的車子。每個範例都有積木圖、可下載的專案檔，還有一支做同樣事情的 Arduino 程式。

| 寫法 | 適合誰 | 怎麼打開範例檔 |
|---|---|---|
| 🟧 **mBlock 5 電腦版**（積木，上傳模式）＋ **MBC 感應器擴展 v2** | 學校、第一次用 mBot 的人 | 下載 `.mblock` → 打開 mBlock 5 → 「檔案 → 從電腦打開」→ 選檔案 → 右下切到「上傳」→ 按「上傳」 |
| 🔧 **Arduino IDE**（文字程式） | 想看積木背後長什麼樣、或自己寫程式的人 | 下載 `.ino` → 裝好 Makeblock 與 MBC_Sensors 函式庫（見 [MakeBlock 積木擴展](/makeblock.md)）→ 主板選 Arduino Uno → 上傳 |

> [!IMPORTANT]
> - 感應器要是 **MakeBlock 版、2026-10 之後的協定 v2 韌體**，經 **mBot 插座片**接 RJ25 連接埠；擴展要用 **v2.0.0**（舊韌體配舊版 v1 擴展，兩者不能混用）。
> - 主板是 **mBot（mCore）**。mBot Ranger、Orion、MegaPi 也能用，但要把「當 mBot 啟動時」換成那塊主板的起始積木、連接埠改成它能用的號碼（見 [支援的主板與連接埠](/makeblock.md)）；mBot2（CyberPi）沒有 RJ25，不支援。
> - 積木圖裡的控制與運算積木（forever、if…then…else、wait、set…to）以英文顯示，在 mBlock 的中文介面就是「重複無限次」「如果…那麼…否則」「等待…秒」「變數設為」。

## 開始之前

<figure class="ref-fig"><img src="/images/figures/ps2/wiring-mbot.svg" alt="掌控者 PS2 × mBot 接線" loading="lazy"></figure>

感應器 → mBot 插座片 → 主板 RJ25 連接埠（S1 = 主機送、S2 = 主機收，UART 38400）。

1. **裝擴展**：到 [MakeBlock 積木擴展](/makeblock.md) 取得 **MBC Pro Sensors v2.0.0** 的 `.mext`，拖進 mBlock 5 視窗；左下「設備」加入 **mBot**，右下切到「上傳」。
2. **接線**：掌控者 → mBot 插座片 → mBot **連接埠 4**（這頁所有範例都接這一埠；換埠就把積木上的連接埠數字改掉）。
3. **先開機再讀**：每個範例一開始都「等待 1 秒」讓感應器開機；太早讀會讀到 -32768（還沒收到）。
4. **讓手把連上**：PS2 的 2.4G 手把和接收器是原廠成對的，不用配對。打開手把電源，接收器的**綠燈熄掉**＝已連上（亮著＝還沒）。手把上的 **ANALOG 燈要亮**，搖桿才有數值。
5. **怎麼知道連好了？** 跑範例 5：連上亮綠燈。

## 這頁會用到的積木

每顆讀值積木就是感應器值表裡的一個「值編號」（跟 Arduino I2C 版的暫存器編號相同）；完整積木表在 [MakeBlock 積木擴展](/makeblock.md)。

| 積木 | 傳回 | 意思 |
|---|---|---|
| 掌控者 連接埠 ［4］ 搖桿 ［左搖桿 x／左搖桿 y／右搖桿 x／右搖桿 y］ | -100~+100 | 10 LX、11 LY、12 RX、13 RY：−100 ~ +100，0 = 中間，右／上為正（原始 0~255 死區 ±7 吸成中心）；未連線 = 0。EV3／SPIKE／MATRIX 送的是 0~99（50 = 中間）或 1~9（5 = 中間），見各平台欄 |
| 掌控者 連接埠 ［4］ ［Ⅹ／●／▲／■／上／下／左／右／L1／R1／L2／R2／L3／R3／SELECT／START］ 被按下？ | 是／否 | 從十六鍵 01 拆出的單一按鈕 |
| 掌控者 連接埠 ［4］ 手把已連線？ | 是／否 | 1 = 手把已連線（psx.read() 成功），0 = 沒有；未連線時所有按鍵 0、搖桿回中 |

**這頁的範例**

1. [搖桿變音高](#%E7%AF%84%E4%BE%8B-1%EF%BC%9A%E6%90%96%E6%A1%BF%E8%AE%8A%E9%9F%B3%E9%AB%98)
2. [按鍵亮燈](#%E7%AF%84%E4%BE%8B-2%EF%BC%9A%E6%8C%89%E9%8D%B5%E4%BA%AE%E7%87%88)
3. [左搖桿控制一個輪子](#%E7%AF%84%E4%BE%8B-3%EF%BC%9A%E5%B7%A6%E6%90%96%E6%A1%BF%E6%8E%A7%E5%88%B6%E4%B8%80%E5%80%8B%E8%BC%AA%E5%AD%90)
4. [方向鍵開車](#%E7%AF%84%E4%BE%8B-4%EF%BC%9A%E6%96%B9%E5%90%91%E9%8D%B5%E9%96%8B%E8%BB%8A)
5. [雙搖桿坦克車＋斷線停車](#%E7%AF%84%E4%BE%8B-5%EF%BC%9A%E9%9B%99%E6%90%96%E6%A1%BF%E5%9D%A6%E5%85%8B%E8%BB%8A%EF%BC%8B%E6%96%B7%E7%B7%9A%E5%81%9C%E8%BB%8A)

> [!NOTE]
> `.mblock` 是由產生器照 mBlock 5 的存檔格式產生的，**還沒在 mBlock 5 裡開啟驗證**（等擴展 v2 上架後補驗）。開不起來或積木顯示空白，請先確認 v2 擴展已拖進 mBlock、設備是 mBot，或照下面的積木圖自己拼（5 分鐘內拼得完）；也歡迎 [LINE 告訴我們](https://line.me/R/ti/p/@692vcvuk)。

---

## 範例 1：搖桿變音高

**這個範例會做什麼**：mBot 沒有螢幕，這個範例把「左搖桿 y」（−100 ~ +100，放開是 0）變成蜂鳴器的音高：往上推音變高、往下拉音變低。先學會「讀到數字」。

**要準備什麼**
- mBot（mCore 主板）＋ mBlock 5 電腦版
- 掌控者 PS2 2.4G 接收器（MakeBlock 版）＋手把＋ mBot 插座片

**怎麼接**：2.4G 接收器 → mBot 插座片 → mBot **連接埠 4**。打開手把電源，接收器的綠燈熄掉就是連上了。

**下載程式**

| mBlock 5 專案 | 等價 Arduino 程式 |
|---|---|
| [ex01_stick_tone.mblock](/examples/ps2/mblock/ex01_stick_tone.mblock) | [ex01_stick_tone.ino](/examples/ps2/mblock/ex01_stick_tone/ex01_stick_tone.ino) |

<details open>
<summary>🧱 mBlock 積木（照著拼就好）</summary>

<pre class="blocks">當 mBot 啟動時 :: events hat
wait (1) seconds
forever
  播放音頻 ((440) + ((掌控者 連接埠 [4 v] 搖桿 [左搖桿 y v] :: #2B6CD8) * (3))) 赫茲，持續 (0.1) 秒 :: sound
end</pre>

</details>

<details>
<summary>🔧 等價 Arduino 程式（mBot）</summary>

```cpp
// 掌控者 PS2 × mBlock 5　範例 1：搖桿變音高（mBot 等價 Arduino 程式）
// 接線：2.4G 接收器 → mBot 插座片 → mBot 連接埠 4。打開手把電源，接收器的綠燈熄掉就是連上了。
// 跟 mBlock 積木範例做的事完全一樣，給想看「積木背後長什麼樣」或用 Arduino IDE 的人。
// 需要的函式庫：
//   1. Makeblock（Arduino IDE「函式庫管理員」搜尋 Makeblock，或 mBlock 5 安裝資料夾裡的 Makeblock 函式庫）
//   2. MBC_Sensors v2：擴展 .mext 解壓後的 src/ 四個檔（MBC_Sensors.h/.cpp、mbc_proto.h、mbc_ids.h），
//      放進 文件/Arduino/libraries/MBC_Sensors/ 就能 #include <MBC_Sensors.h>（https://mbc-pro-sensors.github.io/makeblock.html）
// 主板選 Arduino Uno（mBot 的 mCore 就是 Uno），上傳後拔掉 USB 用電池跑。

#include <MeMCore.h>            // mBot（mCore）：馬達、LED、蜂鳴器
#include <SoftwareSerial.h>
#include <MBC_Sensors.h>        // MBC 感應器 v2（跟 mBlock 擴展同一份函式庫）

MBCPort mbc_port4(4);           // 感應器接在 mBot 連接埠 4
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
  mbc_port4.begin();
  delay(1000);   // 等 1 秒讓接收器開機
  while (1) {   // 一直重複
    // 音高 ＝ 440 ＋ 左搖桿 y × 3 赫茲，響 0.1 秒
    buzzer.tone((440 + (mbcReadN(mbc_port4, 0x10, 2, 4) * 3)), 100);
  }
}

void loop() {}   // 這裡什麼都沒有：上面的積木不會結束
```

</details>

**一步一步看懂程式**
1. 等 1 秒讓接收器開機
2. 一直重複
   - 音高 ＝ 440 ＋ 左搖桿 y × 3 赫茲，響 0.1 秒

**跑起來應該看到什麼**：放開搖桿：440 赫茲（La）。往上推到底 740 赫茲、往下拉到底 140 赫茲。手把沒連上時搖桿也是 0，所以也是 440。

**常見問題**
- **聽不到聲音？** → mBot 的蜂鳴器在主板上，聲音不大；把耳朵靠近，或把 0.1 秒改成 0.3 秒。
- **想換右搖桿或左右方向？** → 把下拉選單換成「右搖桿 y」或「左搖桿 x」。
- **按鍵、搖桿都沒反應？** → 手把沒連上：先跑範例 5 看燈是紅的還是綠的。紅燈＝沒連上，重開手把電源（PS4 按 PS 鍵），或 mBot 先開機再插接收器。
- **什麼都沒反應、或讀值一直是 -32768？** → 連接埠選錯（這頁都接連接埠 4）、插座片沒插緊、或感應器不是 MakeBlock 版。先用「MBC 共用 → 連線正常？」積木確認；「協議版本」讀到 2 才是新韌體（舊韌體請改用舊版擴展 v1）。
- **上傳失敗、或積木是空白的？** → 確認右下角切到「上傳」模式、擴展 v2 已拖進 mBlock、USB 線接的是 mBot 主板。

---

## 範例 2：按鍵亮燈

**這個範例會做什麼**：用「如果…就…否則…」和「被按下？」積木：按 ▲ 綠燈、● 紅燈、Ⅹ 藍燈、■ 粉紅燈（跟手把上符號的顏色一樣），都沒按就關燈。

**要準備什麼**
- mBot（mCore 主板）＋ mBlock 5 電腦版
- 掌控者 PS2 2.4G 接收器（MakeBlock 版）＋手把＋ mBot 插座片

**怎麼接**：2.4G 接收器 → mBot 插座片 → mBot **連接埠 4**。打開手把電源，接收器的綠燈熄掉就是連上了。

**下載程式**

| mBlock 5 專案 | 等價 Arduino 程式 |
|---|---|
| [ex02_button_led.mblock](/examples/ps2/mblock/ex02_button_led.mblock) | [ex02_button_led.ino](/examples/ps2/mblock/ex02_button_led/ex02_button_led.ino) |

<details open>
<summary>🧱 mBlock 積木（照著拼就好）</summary>

<pre class="blocks">當 mBot 啟動時 :: events hat
wait (1) seconds
forever
  if &lt;掌控者 連接埠 [4 v] [▲ v] 被按下？ :: #2B6CD8&gt; then
    LED 燈位置 [全部 v] 的三原色數值為 紅 (0) 綠 (255) 藍 (0) :: looks
  else
    if &lt;掌控者 連接埠 [4 v] [● v] 被按下？ :: #2B6CD8&gt; then
      LED 燈位置 [全部 v] 的三原色數值為 紅 (255) 綠 (0) 藍 (0) :: looks
    else
      if &lt;掌控者 連接埠 [4 v] [Ⅹ v] 被按下？ :: #2B6CD8&gt; then
        LED 燈位置 [全部 v] 的三原色數值為 紅 (0) 綠 (0) 藍 (255) :: looks
      else
        if &lt;掌控者 連接埠 [4 v] [■ v] 被按下？ :: #2B6CD8&gt; then
          LED 燈位置 [全部 v] 的三原色數值為 紅 (255) 綠 (0) 藍 (255) :: looks
        else
          LED 燈位置 [全部 v] 的三原色數值為 紅 (0) 綠 (0) 藍 (0) :: looks
        end
      end
    end
  end
end</pre>

</details>

<details>
<summary>🔧 等價 Arduino 程式（mBot）</summary>

```cpp
// 掌控者 PS2 × mBlock 5　範例 2：按鍵亮燈（mBot 等價 Arduino 程式）
// 接線：2.4G 接收器 → mBot 插座片 → mBot 連接埠 4。打開手把電源，接收器的綠燈熄掉就是連上了。
// 跟 mBlock 積木範例做的事完全一樣，給想看「積木背後長什麼樣」或用 Arduino IDE 的人。
// 需要的函式庫：
//   1. Makeblock（Arduino IDE「函式庫管理員」搜尋 Makeblock，或 mBlock 5 安裝資料夾裡的 Makeblock 函式庫）
//   2. MBC_Sensors v2：擴展 .mext 解壓後的 src/ 四個檔（MBC_Sensors.h/.cpp、mbc_proto.h、mbc_ids.h），
//      放進 文件/Arduino/libraries/MBC_Sensors/ 就能 #include <MBC_Sensors.h>（https://mbc-pro-sensors.github.io/makeblock.html）
// 主板選 Arduino Uno（mBot 的 mCore 就是 Uno），上傳後拔掉 USB 用電池跑。

#include <MeMCore.h>            // mBot（mCore）：馬達、LED、蜂鳴器
#include <SoftwareSerial.h>
#include <MBC_Sensors.h>        // MBC 感應器 v2（跟 mBlock 擴展同一份函式庫）

MBCPort mbc_port4(4);           // 感應器接在 mBot 連接埠 4
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
  mbc_port4.begin();
  delay(1000);   // 等 1 秒讓接收器開機
  while (1) {   // 一直重複
    if (mbcPadPressed(mbc_port4, 12)) {   // 如果 ▲ 被按下
      rgbled.setColor(0, 0, 255, 0); rgbled.show();   // ▲：綠燈
    } else {
      if (mbcPadPressed(mbc_port4, 13)) {   // 否則，如果 ● 被按下
        rgbled.setColor(0, 255, 0, 0); rgbled.show();   // ●：紅燈
      } else {
        if (mbcPadPressed(mbc_port4, 14)) {   // 否則，如果 Ⅹ 被按下
          rgbled.setColor(0, 0, 0, 255); rgbled.show();   // Ⅹ：藍燈
        } else {
          if (mbcPadPressed(mbc_port4, 15)) {   // 否則，如果 ■ 被按下
            rgbled.setColor(0, 255, 0, 255); rgbled.show();   // ■：粉紅燈
          } else {
            rgbled.setColor(0, 0, 0, 0); rgbled.show();   // 都沒按：關燈
          }
        }
      }
    }
  }
}

void loop() {}   // 這裡什麼都沒有：上面的積木不會結束
```

</details>

**一步一步看懂程式**
1. 等 1 秒讓接收器開機
2. 一直重複
   - 如果 ▲ 被按下
      - ▲：綠燈
      - 否則：
         - 否則，如果 ● 被按下
            - ●：紅燈
            - 否則：
               - 否則，如果 Ⅹ 被按下
                  - Ⅹ：藍燈
                  - 否則：
                     - 否則，如果 ■ 被按下
                        - ■：粉紅燈
                        - 否則：
                           - 都沒按：關燈

**跑起來應該看到什麼**：按住 ▲ 綠燈、放開熄掉；同時按 ▲ 和 ● 是綠燈（上面的「如果」先成立就不看下面的）。

**常見問題**
- **想用方向鍵或 L1／R1？** → 「被按下？」的下拉選單有全部 16 個鍵。
- **按鍵、搖桿都沒反應？** → 手把沒連上：先跑範例 5 看燈是紅的還是綠的。紅燈＝沒連上，重開手把電源（PS4 按 PS 鍵），或 mBot 先開機再插接收器。
- **什麼都沒反應、或讀值一直是 -32768？** → 連接埠選錯（這頁都接連接埠 4）、插座片沒插緊、或感應器不是 MakeBlock 版。先用「MBC 共用 → 連線正常？」積木確認；「協議版本」讀到 2 才是新韌體（舊韌體請改用舊版擴展 v1）。
- **上傳失敗、或積木是空白的？** → 確認右下角切到「上傳」模式、擴展 v2 已拖進 mBlock、USB 線接的是 mBot 主板。

---

## 範例 3：左搖桿控制一個輪子

**這個範例會做什麼**：搖桿的數字 −100 ~ +100 剛好就是馬達動力的範圍，直接把「左搖桿 y」當左輪的動力：推越多轉越快，往下拉就倒轉，放開就停。先把車子架高看。

**要準備什麼**
- mBot（mCore 主板）＋ mBlock 5 電腦版
- mBot 兩輪小車（左輪接 M1、右輪接 M2，出廠就是這樣接）
- 掌控者 PS2 2.4G 接收器（MakeBlock 版）＋手把＋ mBot 插座片

**怎麼接**：2.4G 接收器 → mBot 插座片 → mBot **連接埠 4**。打開手把電源，接收器的綠燈熄掉就是連上了。

**下載程式**

| mBlock 5 專案 | 等價 Arduino 程式 |
|---|---|
| [ex03_stick_one_wheel.mblock](/examples/ps2/mblock/ex03_stick_one_wheel.mblock) | [ex03_stick_one_wheel.ino](/examples/ps2/mblock/ex03_stick_one_wheel/ex03_stick_one_wheel.ino) |

<details open>
<summary>🧱 mBlock 積木（照著拼就好）</summary>

<pre class="blocks">當 mBot 啟動時 :: events hat
wait (1) seconds
forever
  左輪動力 (掌控者 連接埠 [4 v] 搖桿 [左搖桿 y v] :: #2B6CD8) %，右輪動力 (0) % :: motion
end</pre>

</details>

<details>
<summary>🔧 等價 Arduino 程式（mBot）</summary>

```cpp
// 掌控者 PS2 × mBlock 5　範例 3：左搖桿控制一個輪子（mBot 等價 Arduino 程式）
// 接線：2.4G 接收器 → mBot 插座片 → mBot 連接埠 4。打開手把電源，接收器的綠燈熄掉就是連上了。
// 跟 mBlock 積木範例做的事完全一樣，給想看「積木背後長什麼樣」或用 Arduino IDE 的人。
// 需要的函式庫：
//   1. Makeblock（Arduino IDE「函式庫管理員」搜尋 Makeblock，或 mBlock 5 安裝資料夾裡的 Makeblock 函式庫）
//   2. MBC_Sensors v2：擴展 .mext 解壓後的 src/ 四個檔（MBC_Sensors.h/.cpp、mbc_proto.h、mbc_ids.h），
//      放進 文件/Arduino/libraries/MBC_Sensors/ 就能 #include <MBC_Sensors.h>（https://mbc-pro-sensors.github.io/makeblock.html）
// 主板選 Arduino Uno（mBot 的 mCore 就是 Uno），上傳後拔掉 USB 用電池跑。

#include <MeMCore.h>            // mBot（mCore）：馬達、LED、蜂鳴器
#include <SoftwareSerial.h>
#include <MBC_Sensors.h>        // MBC 感應器 v2（跟 mBlock 擴展同一份函式庫）

MBCPort mbc_port4(4);           // 感應器接在 mBot 連接埠 4
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
  mbc_port4.begin();
  delay(1000);   // 等 1 秒讓接收器開機
  while (1) {   // 一直重複
    wheels(mbcReadN(mbc_port4, 0x10, 2, 4), 0);   // 左輪動力 ＝ 左搖桿 y（−100 ~ +100），右輪 0
  }
}

void loop() {}   // 這裡什麼都沒有：上面的積木不會結束
```

</details>

**一步一步看懂程式**
1. 等 1 秒讓接收器開機
2. 一直重複
   - 左輪動力 ＝ 左搖桿 y（−100 ~ +100），右輪 0

**跑起來應該看到什麼**：左搖桿往上推，左輪往前轉、越推越快；往下拉倒轉；放開停止。

**常見問題**
- **車子往反方向轉、越修越歪？** → 左右輪的「＋」「－」對調（或把左輪、右輪的動力互換）。mBot 出廠左輪 M1、右輪 M2，自己接線的請確認。
- **按鍵、搖桿都沒反應？** → 手把沒連上：先跑範例 5 看燈是紅的還是綠的。紅燈＝沒連上，重開手把電源（PS4 按 PS 鍵），或 mBot 先開機再插接收器。
- **什麼都沒反應、或讀值一直是 -32768？** → 連接埠選錯（這頁都接連接埠 4）、插座片沒插緊、或感應器不是 MakeBlock 版。先用「MBC 共用 → 連線正常？」積木確認；「協議版本」讀到 2 才是新韌體（舊韌體請改用舊版擴展 v1）。
- **上傳失敗、或積木是空白的？** → 確認右下角切到「上傳」模式、擴展 v2 已拖進 mBlock、USB 線接的是 mBot 主板。

---

## 範例 4：方向鍵開車

**這個範例會做什麼**：按住方向鍵車子就往那個方向走，放開就停：上＝前進、下＝後退、左＝原地左轉、右＝原地右轉。

**要準備什麼**
- mBot（mCore 主板）＋ mBlock 5 電腦版
- mBot 兩輪小車（左輪接 M1、右輪接 M2，出廠就是這樣接）
- 掌控者 PS2 2.4G 接收器（MakeBlock 版）＋手把＋ mBot 插座片

**怎麼接**：2.4G 接收器 → mBot 插座片 → mBot **連接埠 4**。打開手把電源，接收器的綠燈熄掉就是連上了。

**下載程式**

| mBlock 5 專案 | 等價 Arduino 程式 |
|---|---|
| [ex04_dpad_drive.mblock](/examples/ps2/mblock/ex04_dpad_drive.mblock) | [ex04_dpad_drive.ino](/examples/ps2/mblock/ex04_dpad_drive/ex04_dpad_drive.ino) |

<details open>
<summary>🧱 mBlock 積木（照著拼就好）</summary>

<pre class="blocks">當 mBot 啟動時 :: events hat
wait (1) seconds
forever
  if &lt;掌控者 連接埠 [4 v] [上 v] 被按下？ :: #2B6CD8&gt; then
    左輪動力 (50) %，右輪動力 (50) % :: motion
  else
    if &lt;掌控者 連接埠 [4 v] [下 v] 被按下？ :: #2B6CD8&gt; then
      左輪動力 (-50) %，右輪動力 (-50) % :: motion
    else
      if &lt;掌控者 連接埠 [4 v] [左 v] 被按下？ :: #2B6CD8&gt; then
        左輪動力 (-30) %，右輪動力 (30) % :: motion
      else
        if &lt;掌控者 連接埠 [4 v] [右 v] 被按下？ :: #2B6CD8&gt; then
          左輪動力 (30) %，右輪動力 (-30) % :: motion
        else
          停止移動 :: motion
        end
      end
    end
  end
end</pre>

</details>

<details>
<summary>🔧 等價 Arduino 程式（mBot）</summary>

```cpp
// 掌控者 PS2 × mBlock 5　範例 4：方向鍵開車（mBot 等價 Arduino 程式）
// 接線：2.4G 接收器 → mBot 插座片 → mBot 連接埠 4。打開手把電源，接收器的綠燈熄掉就是連上了。
// 跟 mBlock 積木範例做的事完全一樣，給想看「積木背後長什麼樣」或用 Arduino IDE 的人。
// 需要的函式庫：
//   1. Makeblock（Arduino IDE「函式庫管理員」搜尋 Makeblock，或 mBlock 5 安裝資料夾裡的 Makeblock 函式庫）
//   2. MBC_Sensors v2：擴展 .mext 解壓後的 src/ 四個檔（MBC_Sensors.h/.cpp、mbc_proto.h、mbc_ids.h），
//      放進 文件/Arduino/libraries/MBC_Sensors/ 就能 #include <MBC_Sensors.h>（https://mbc-pro-sensors.github.io/makeblock.html）
// 主板選 Arduino Uno（mBot 的 mCore 就是 Uno），上傳後拔掉 USB 用電池跑。

#include <MeMCore.h>            // mBot（mCore）：馬達、LED、蜂鳴器
#include <SoftwareSerial.h>
#include <MBC_Sensors.h>        // MBC 感應器 v2（跟 mBlock 擴展同一份函式庫）

MBCPort mbc_port4(4);           // 感應器接在 mBot 連接埠 4
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
  mbc_port4.begin();
  delay(1000);   // 等 1 秒讓接收器開機
  while (1) {   // 一直重複
    if (mbcPadPressed(mbc_port4, 4)) {   // 如果 上 被按下
      wheels(50, 50);   // 上：前進
    } else {
      if (mbcPadPressed(mbc_port4, 6)) {   // 否則，如果 下 被按下
        wheels(-50, -50);   // 下：後退
      } else {
        if (mbcPadPressed(mbc_port4, 7)) {   // 否則，如果 左 被按下
          wheels(-30, 30);   // 左：原地左轉（左輪往後、右輪往前）
        } else {
          if (mbcPadPressed(mbc_port4, 5)) {   // 否則，如果 右 被按下
            wheels(30, -30);   // 右：原地右轉
          } else {
            wheels(0, 0);   // 都沒按：停車
          }
        }
      }
    }
  }
}

void loop() {}   // 這裡什麼都沒有：上面的積木不會結束
```

</details>

**一步一步看懂程式**
1. 等 1 秒讓接收器開機
2. 一直重複
   - 如果 上 被按下
      - 上：前進
      - 否則：
         - 否則，如果 下 被按下
            - 下：後退
            - 否則：
               - 否則，如果 左 被按下
                  - 左：原地左轉（左輪往後、右輪往前）
                  - 否則：
                     - 否則，如果 右 被按下
                        - 右：原地右轉
                        - 否則：
                           - 都沒按：停車

**跑起來應該看到什麼**：按住上，車子往前走；放開，車子停。

**常見問題**
- **車子往反方向轉、越修越歪？** → 左右輪的「＋」「－」對調（或把左輪、右輪的動力互換）。mBot 出廠左輪 M1、右輪 M2，自己接線的請確認。
- **手把沒電或走太遠？** → 接收器超過 0.3 秒收不到手把，就把按鍵全部放開、搖桿回到中間，車子會自己停。
- **按鍵、搖桿都沒反應？** → 手把沒連上：先跑範例 5 看燈是紅的還是綠的。紅燈＝沒連上，重開手把電源（PS4 按 PS 鍵），或 mBot 先開機再插接收器。
- **什麼都沒反應、或讀值一直是 -32768？** → 連接埠選錯（這頁都接連接埠 4）、插座片沒插緊、或感應器不是 MakeBlock 版。先用「MBC 共用 → 連線正常？」積木確認；「協議版本」讀到 2 才是新韌體（舊韌體請改用舊版擴展 v1）。
- **上傳失敗、或積木是空白的？** → 確認右下角切到「上傳」模式、擴展 v2 已拖進 mBlock、USB 線接的是 mBot 主板。

---

## 範例 5：雙搖桿坦克車＋斷線停車

**這個範例會做什麼**：像開坦克：左搖桿管左輪、右搖桿管右輪，數字 × 0.7 當動力（最快 70）。先用「手把已連線？」判斷：連上亮綠燈才開，沒連上停車亮紅燈。

**要準備什麼**
- mBot（mCore 主板）＋ mBlock 5 電腦版
- mBot 兩輪小車（左輪接 M1、右輪接 M2，出廠就是這樣接）
- 掌控者 PS2 2.4G 接收器（MakeBlock 版）＋手把＋ mBot 插座片

**怎麼接**：2.4G 接收器 → mBot 插座片 → mBot **連接埠 4**。打開手把電源，接收器的綠燈熄掉就是連上了。

**下載程式**

| mBlock 5 專案 | 等價 Arduino 程式 |
|---|---|
| [ex05_tank.mblock](/examples/ps2/mblock/ex05_tank.mblock) | [ex05_tank.ino](/examples/ps2/mblock/ex05_tank/ex05_tank.ino) |

<details open>
<summary>🧱 mBlock 積木（照著拼就好）</summary>

<pre class="blocks">當 mBot 啟動時 :: events hat
wait (1) seconds
forever
  if &lt;掌控者 連接埠 [4 v] 手把已連線？ :: #2B6CD8&gt; then
    LED 燈位置 [全部 v] 的三原色數值為 紅 (0) 綠 (255) 藍 (0) :: looks
    左輪動力 ((掌控者 連接埠 [4 v] 搖桿 [左搖桿 y v] :: #2B6CD8) * (0.7)) %，右輪動力 ((掌控者 連接埠 [4 v] 搖桿 [右搖桿 y v] :: #2B6CD8) * (0.7)) % :: motion
  else
    LED 燈位置 [全部 v] 的三原色數值為 紅 (255) 綠 (0) 藍 (0) :: looks
    停止移動 :: motion
  end
end</pre>

</details>

<details>
<summary>🔧 等價 Arduino 程式（mBot）</summary>

```cpp
// 掌控者 PS2 × mBlock 5　範例 5：雙搖桿坦克車＋斷線停車（mBot 等價 Arduino 程式）
// 接線：2.4G 接收器 → mBot 插座片 → mBot 連接埠 4。打開手把電源，接收器的綠燈熄掉就是連上了。
// 跟 mBlock 積木範例做的事完全一樣，給想看「積木背後長什麼樣」或用 Arduino IDE 的人。
// 需要的函式庫：
//   1. Makeblock（Arduino IDE「函式庫管理員」搜尋 Makeblock，或 mBlock 5 安裝資料夾裡的 Makeblock 函式庫）
//   2. MBC_Sensors v2：擴展 .mext 解壓後的 src/ 四個檔（MBC_Sensors.h/.cpp、mbc_proto.h、mbc_ids.h），
//      放進 文件/Arduino/libraries/MBC_Sensors/ 就能 #include <MBC_Sensors.h>（https://mbc-pro-sensors.github.io/makeblock.html）
// 主板選 Arduino Uno（mBot 的 mCore 就是 Uno），上傳後拔掉 USB 用電池跑。

#include <MeMCore.h>            // mBot（mCore）：馬達、LED、蜂鳴器
#include <SoftwareSerial.h>
#include <MBC_Sensors.h>        // MBC 感應器 v2（跟 mBlock 擴展同一份函式庫）

MBCPort mbc_port4(4);           // 感應器接在 mBot 連接埠 4
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
  mbc_port4.begin();
  delay(1000);   // 等 1 秒讓接收器開機
  while (1) {   // 一直重複
    if ((mbcRead(mbc_port4, 0x06) == 1)) {   // 如果手把已連線
      rgbled.setColor(0, 0, 255, 0); rgbled.show();   // 連上了：綠燈
      // 左輪 ＝ 左搖桿 y × 0.7、右輪 ＝ 右搖桿 y × 0.7
      wheels((mbcReadN(mbc_port4, 0x10, 2, 4) * 0.7), (mbcReadN(mbc_port4, 0x10, 4, 4) * 0.7));
    } else {
      rgbled.setColor(0, 255, 0, 0); rgbled.show();   // 沒連上：紅燈
      wheels(0, 0);   // 停車
    }
  }
}

void loop() {}   // 這裡什麼都沒有：上面的積木不會結束
```

</details>

**一步一步看懂程式**
1. 等 1 秒讓接收器開機
2. 一直重複
   - 如果手把已連線
      - 連上了：綠燈
      - 左輪 ＝ 左搖桿 y × 0.7、右輪 ＝ 右搖桿 y × 0.7
      - 否則：
         - 沒連上：紅燈
         - 停車

**跑起來應該看到什麼**：手把連上亮綠燈；兩支搖桿一起往前推車子前進，推越多越快；左前右後原地右轉；關掉手把馬上停車、亮紅燈。

**常見問題**
- **太快不好控制？** → 把 0.7 改成 0.5。
- **車子往反方向轉、越修越歪？** → 左右輪的「＋」「－」對調（或把左輪、右輪的動力互換）。mBot 出廠左輪 M1、右輪 M2，自己接線的請確認。
- **按鍵、搖桿都沒反應？** → 手把沒連上：先跑範例 5 看燈是紅的還是綠的。紅燈＝沒連上，重開手把電源（PS4 按 PS 鍵），或 mBot 先開機再插接收器。
- **什麼都沒反應、或讀值一直是 -32768？** → 連接埠選錯（這頁都接連接埠 4）、插座片沒插緊、或感應器不是 MakeBlock 版。先用「MBC 共用 → 連線正常？」積木確認；「協議版本」讀到 2 才是新韌體（舊韌體請改用舊版擴展 v1）。
- **上傳失敗、或積木是空白的？** → 確認右下角切到「上傳」模式、擴展 v2 已拖進 mBlock、USB 線接的是 mBot 主板。

---

## 下一步

- 全部 掌控者積木、各主板可用的連接埠：[MakeBlock 積木擴展](/makeblock.md)；這款產品的 mBlock 積木表：[掌控者 PS2 × mBlock 5](/sensors/ps2/mblock.md)
- 同樣的 5 個範例在樂高上怎麼寫：[SPIKE 新手範例](/sensors/ps2/spike-examples.md)、[EV3 新手範例](/sensors/ps2/ev3-examples.md)
- 想用 Arduino 直接接感應器（I2C 版，不經 mBot）：[Arduino 範例](/sensors/ps2/arduino-examples.md)
