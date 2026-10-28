<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 95_產品網站\mBlock範例產生器（node build.js）產生：改內容請改產生器的 sets/<產品>.js 再重建，不要手改本檔 -->
# 🧱 mBlock 新手範例：神攝手

[🔙 回到神攝手產品頁](/sensors/unicam/index.md)

這一頁是寫給**完全沒寫過程式**的人：用 mBlock 5 的積木在 **mBot** 上讀神攝手。6 個範例從「把數字變成聲音」開始，一步一步做到會自己跑的車子。每個範例都有積木圖、可下載的專案檔，還有一支做同樣事情的 Arduino 程式。

| 寫法 | 適合誰 | 怎麼打開範例檔 |
|---|---|---|
| 🟧 **mBlock 5 電腦版**（積木，上傳模式）＋ **MBC 感應器擴展 v2.2.0** | 學校、第一次用 mBot 的人 | 下載 `.mblock` → 打開 mBlock 5 → 「檔案 → 從電腦打開」→ 選檔案 → 右下切到「上傳」→ 按「上傳」 |
| 🔧 **Arduino IDE**（文字程式） | 想看積木背後長什麼樣、或自己寫程式的人 | 下載 `.ino` → 裝好 Makeblock 與 MBC_Sensors 函式庫（見 [神攝手 mBlock 教學](/sensors/unicam/mblock/index.md)）→ 主板選 Arduino Uno → 上傳 |

> [!WARNING]
> **v1 尚未上機驗證**：數值排法已定案並通過電腦端測試，實機數值（fps、循線門檻）上機後可能調整；mBlock 版還沒接 mBot 實測。

> [!IMPORTANT]
> - 感應器要燒 **神攝手的 mBlock 版韌體**，經 **mBot 插座片**接 RJ25 連接埠；擴展要用 **MBC 感應器 v2.2.0**（擴展新版開發中，正式上架前可先下載 [匯入檔](/downloads/MBC_Pro_Sensors_mBlock5_v2.2.0_import.mext) 試用，尚未上機驗證；做法見 [mBlock 教學](/sensors/unicam/mblock/index.md)）。
> - 主板是 **mBot（mCore）**。mBot Ranger、Orion、MegaPi 也能用，但要把「當 mBot 啟動時」換成那塊主板的起始積木、連接埠改成它能用的號碼（見 [mBlock 教學](/sensors/unicam/mblock/index.md)）；mBot2（CyberPi）沒有 RJ25，不支援。
> - 積木圖裡的控制與運算積木（forever、if…then…else、wait、set…to）以英文顯示，在 mBlock 的中文介面就是「重複無限次」「如果…那麼…否則」「等待…秒」「變數設為」。

## 開始之前

<figure class="ref-fig"><img src="/images/sensors/unicam/color-track.svg" alt="神攝手顏色追蹤：X 0~159 左→右、Y 0~119 上→下" loading="lazy"></figure>

感應器 → mBot 插座片 → 主板 RJ25 連接埠（UART 38400；相機 RX＝IO3、TX＝IO1，跟主機交叉接）。畫面座標：X 0～159 左→右、Y 0～119 上→下，0＝沒有目標。

1. **裝擴展**：照 [神攝手 mBlock 教學](/sensors/unicam/mblock/index.md) 載入 **MBC 感應器 v2.2.0** 擴展；左下「設備」加入 **mBot**，右下切到「上傳」。
2. **接線**：神攝手 → mBot 插座片 → mBot **連接埠 1**（這頁所有範例都接這一埠；換埠就把積木上的連接埠數字改掉）。
3. **先開機再讀**：每個範例一開始都「等待 2 秒」讓相機開機；太早讀會讀到 -32768（還沒收到）。
4. **程式會自己切相機模式**：每個範例第二顆積木是「設定模式」（例：顏色追蹤 ③ 標準），會存進相機。讀值積木會先問相機目前是什麼模式，再決定讀哪個值；模式不對時讀到 -32768。
5. **先在相機上教顏色**（顏色追蹤、物體偵測用，程式改不了）：即時畫面用左右鍵停在「目標 1」→ 按 OK → 用左右鍵把十字移到積木上（先調 X、再調 Y，各按一次 OK）→ 最後按 OK 取色（按住 OK 0.8 秒可直接完成）。範例 2～4 追的是**目標 1**；範例 6 把目標 1 教成**紅色**（物體偵測只看目標色 1～8）。

## 這頁會用到的積木

每顆讀值積木會先讀相機的「模式編號」（值 0x01），再讀這個模式下對應的值編號（跟 Arduino I2C 版同一張值表）；完整積木表與值編號在 [神攝手 mBlock 教學](/sensors/unicam/mblock/index.md)。

| 積木 | 傳回 | 意思 |
|---|---|---|
| 神攝手 連接埠 ［1］ 取樣點 ［1~16］ 色相 | 0~255 | 色相碼：0~240 色相（紅 0、黃 40、綠 80、青 120、藍 160、紫 200）、241~248 白→灰、249~255 黑 |
| 神攝手 連接埠 ［1］ 設定模式 ［顏色追蹤 ① 最多色／顏色追蹤 ② 多色／顏色追蹤 ③ 標準／顏色追蹤 ④ 精細／座標取色 ① 色相／座標取色 ② 色相＋飽和／座標取色 ③ 色相＋飽和＋明度／照順序給色號／循線 ① 黑線／循線 ② 白線／循線 ③ 色線／物體偵測 ① 物體清單／物體偵測 ② 物體詳細］ | — | 切相機模式與子選項（E0 [相機模式, 子選項]，會存進相機）；切完等 50 ms |
| 神攝手 連接埠 ［1］ 第 ［1~16］ 色有找到？ | 是／否 | 第 n 色的 X 大於 0 |
| 神攝手 連接埠 ［1］ 第 ［1~16］ 色 x | 0~159 | 色塊中心 X 像素（左 → 右）；0 = 沒找到 |
| 神攝手 連接埠 ［1］ 第 ［1~16］ 色 面積 % | 0~100 | 色塊佔畫面的 %（有找到但不到 1% = 1）；③ ④ 有 |
| 神攝手 連接埠 ［1］ 循線 ［近處偏移／中段偏移／遠處偏移／線角度／近處線寬 %／有線旗標／路口類型／路口距離 %／近處線左緣／近處線右緣／遠處線寬 %／標記線色號／標記線條數／最後離線方向］ |  | 下拉選 14 個循線值之一（值 1~14 = 編號 0x10~0x1D）；近處／中段／遠處偏移 −100~100（線在左＝負、右＝正）、線角度 −90~90 |
| 神攝手 連接埠 ［1］ 看到線？ | 是／否 | 有線旗標（近 1＋中 2＋遠 4）不是 0 |
| 神攝手 連接埠 ［1］ ［顏色 1／顏色 2／顏色 3／顏色 4／顏色 5／顏色 6／顏色 7／顏色 8／任何顏色］ 的 ［圓／橢圓／三角形／正方形／長方形／五邊形／六邊形／星形／梯形／其他／任何形狀］ 物體是第幾個 | 0~8 | 第一個（面積最大的）符合的物體是第幾個，可以接到「第 (n) 個物體 x」；0 = 沒有 |
| 神攝手 連接埠 ［1］ 第 ［1~8］ 個物體 x | 0~159 | 質心 X 像素（左 → 右）；0 = 沒有這個物體 |
| 神攝手 連接埠 ［1］ 有 ［顏色 1／顏色 2／顏色 3／顏色 4／顏色 5／顏色 6／顏色 7／顏色 8／任何顏色］ 的 ［圓／橢圓／三角形／正方形／長方形／五邊形／六邊形／星形／梯形／其他／任何形狀］ 物體？ | 是／否 | 畫面裡有沒有這個顏色＋形狀的物體（可選「任何顏色」「任何形狀」） |

**這頁的範例**

1. [聽見顏色](#%E7%AF%84%E4%BE%8B-1%EF%BC%9A%E8%81%BD%E8%A6%8B%E9%A1%8F%E8%89%B2)
2. [找到目標色亮綠燈](#%E7%AF%84%E4%BE%8B-2%EF%BC%9A%E6%89%BE%E5%88%B0%E7%9B%AE%E6%A8%99%E8%89%B2%E4%BA%AE%E7%B6%A0%E7%87%88)
3. [一個輪子轉向色塊](#%E7%AF%84%E4%BE%8B-3%EF%BC%9A%E4%B8%80%E5%80%8B%E8%BC%AA%E5%AD%90%E8%BD%89%E5%90%91%E8%89%B2%E5%A1%8A)
4. [追色塊、靠近就停＋燈號](#%E7%AF%84%E4%BE%8B-4%EF%BC%9A%E8%BF%BD%E8%89%B2%E5%A1%8A%E3%80%81%E9%9D%A0%E8%BF%91%E5%B0%B1%E5%81%9C%EF%BC%8B%E7%87%88%E8%99%9F)
5. [看著黑線走](#%E7%AF%84%E4%BE%8B-5%EF%BC%9A%E7%9C%8B%E8%91%97%E9%BB%91%E7%B7%9A%E8%B5%B0)
6. [追紅色三角形](#%E7%AF%84%E4%BE%8B-6%EF%BC%9A%E8%BF%BD%E7%B4%85%E8%89%B2%E4%B8%89%E8%A7%92%E5%BD%A2)

> [!NOTE]
> `.mblock` 是由產生器照 mBlock 5 的存檔格式產生的，**還沒在 mBlock 5 裡開啟驗證**（擴展 v2.2.0 還在開發，正式上架前補驗）。開不起來或積木顯示空白，請先確認 v2.2.0 擴展已載入、設備是 mBot，或照下面的積木圖自己拼（5 分鐘內拼得完）；也歡迎 [LINE 告訴我們](https://line.me/R/ti/p/@692vcvuk)。

---

## 範例 1：聽見顏色

**這個範例會做什麼**：mBot 沒有螢幕，這個範例把「取樣點 1」看到的色相碼（0 ~ 255：紅 0、黃 40、綠 80、藍 160；白灰黑是 241 以上）變成蜂鳴器的音高。先學會「切模式、讀到數字」。

**要準備什麼**
- mBot（mCore 主板）＋ mBlock 5 電腦版
- 神攝手（MakeBlock 版）＋ mBot 插座片
- 一塊顏色鮮豔的積木（紅、綠、藍都可以）

**怎麼接**：神攝手 → mBot 插座片 → mBot **連接埠 1**。鏡頭朝前、裝在車頭。這個範例不用教顏色；取樣點 1 的位置看相機螢幕上的十字「1」。

**下載程式**

| mBlock 5 專案 | 等價 Arduino 程式 |
|---|---|
| [ex01_hear_hue.mblock](/examples/unicam/mblock/ex01_hear_hue.mblock) | [ex01_hear_hue.ino](/examples/unicam/mblock/ex01_hear_hue/ex01_hear_hue.ino) |

<details open>
<summary>🧱 mBlock 積木（照著拼就好）</summary>

<pre class="blocks">當 mBot 啟動時 :: events hat
wait (2) seconds
神攝手 連接埠 [1 v] 設定模式 [座標取色 ① 色相 v] :: #E8590C
forever
  播放音頻 ((200) + ((神攝手 連接埠 [1 v] 取樣點 [1 v] 色相 :: #E8590C) * (2))) 赫茲，持續 (0.1) 秒 :: sound
end</pre>

</details>

<details>
<summary>🔧 等價 Arduino 程式（mBot）</summary>

```cpp
// 神攝手 × mBlock 5　範例 1：聽見顏色（mBot 等價 Arduino 程式）
// 接線：神攝手 → mBot 插座片 → mBot 連接埠 1。鏡頭朝前、裝在車頭。這個範例不用教顏色；取樣點 1 的位置看相機螢幕上的十字「1」。
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
  delay(2000);   // 等 2 秒讓相機開機
  ucSetMode(mbc_port1, 21);   // 相機切到「座標取色 ① 色相」
  while (1) {   // 一直重複
    buzzer.tone((200 + (ucSpot(mbc_port1, 1, 0) * 2)), 100);   // 音高 ＝ 200 ＋ 取樣點 1 色相 × 2 赫茲，響 0.1 秒
  }
}

void loop() {}   // 這裡什麼都沒有：上面的積木不會結束
```

</details>

**一步一步看懂程式**
1. 等 2 秒讓相機開機
2. 相機切到「座標取色 ① 色相」
3. 一直重複
   - 音高 ＝ 200 ＋ 取樣點 1 色相 × 2 赫茲，響 0.1 秒

**跑起來應該看到什麼**：紅色積木放在十字 1 上：約 200 赫茲低音；綠色約 360 赫茲；藍色約 520 赫茲；白紙或黑色是最高音（680 赫茲以上）。

**常見問題**
- **聽不到聲音？** → mBot 的蜂鳴器在主板上，聲音不大；把耳朵靠近，或把 0.1 秒改成 0.3 秒。
- **顏色換了音沒變？** → 積木沒有放在取樣點 1（相機螢幕上標「1」的十字）；把積木移到十字上。
- **什麼都沒反應、或讀值一直是 -32768？** → 連接埠選錯（這頁都接連接埠 1）、插座片沒插緊、相機燒的不是 mBlock 版韌體，或模式不對（讀值積木只在對應的模式有值）。先用「神攝手 感應器正常？」積木確認連線，再用「目前模式編號」看模式（顏色追蹤 ③ ＝ 13、循線黑線 ＝ 41、物體清單 ＝ 51）。
- **上傳失敗、或積木是空白的？** → 確認右下角切到「上傳」模式、擴展 v2 已拖進 mBlock、USB 線接的是 mBot 主板。

---

## 範例 2：找到目標色亮綠燈

**這個範例會做什麼**：用「第 1 色有找到？」布林積木和「如果…就…否則…」：畫面裡有目標 1 的顏色就亮綠燈，沒有就亮紅燈。這是追色塊程式的第一個判斷。

**要準備什麼**
- mBot（mCore 主板）＋ mBlock 5 電腦版
- 神攝手（MakeBlock 版）＋ mBot 插座片
- 一塊顏色鮮豔的積木（紅、綠、藍都可以）

**怎麼接**：神攝手 → mBot 插座片 → mBot **連接埠 1**。鏡頭朝前、裝在車頭。先照上面「開始之前」在相機上教好目標 1 的顏色。

**下載程式**

| mBlock 5 專案 | 等價 Arduino 程式 |
|---|---|
| [ex02_found_led.mblock](/examples/unicam/mblock/ex02_found_led.mblock) | [ex02_found_led.ino](/examples/unicam/mblock/ex02_found_led/ex02_found_led.ino) |

<details open>
<summary>🧱 mBlock 積木（照著拼就好）</summary>

<pre class="blocks">當 mBot 啟動時 :: events hat
wait (2) seconds
神攝手 連接埠 [1 v] 設定模式 [顏色追蹤 ③ 標準 v] :: #E8590C
forever
  if &lt;神攝手 連接埠 [1 v] 第 [1 v] 色有找到？ :: #E8590C&gt; then
    LED 燈位置 [全部 v] 的三原色數值為 紅 (0) 綠 (255) 藍 (0) :: looks
  else
    LED 燈位置 [全部 v] 的三原色數值為 紅 (255) 綠 (0) 藍 (0) :: looks
  end
end</pre>

</details>

<details>
<summary>🔧 等價 Arduino 程式（mBot）</summary>

```cpp
// 神攝手 × mBlock 5　範例 2：找到目標色亮綠燈（mBot 等價 Arduino 程式）
// 接線：神攝手 → mBot 插座片 → mBot 連接埠 1。鏡頭朝前、裝在車頭。先照上面「開始之前」在相機上教好目標 1 的顏色。
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
  delay(2000);   // 等 2 秒讓相機開機
  ucSetMode(mbc_port1, 13);   // 相機切到「顏色追蹤 ③ 標準」（每色給 X、Y、面積、數量）
  while (1) {   // 一直重複
    if (ucFound(mbc_port1, 1)) {   // 如果第 1 色有找到
      rgbled.setColor(0, 0, 255, 0); rgbled.show();   // 有找到：綠燈
    } else {
      rgbled.setColor(0, 255, 0, 0); rgbled.show();   // 沒找到：紅燈
    }
  }
}

void loop() {}   // 這裡什麼都沒有：上面的積木不會結束
```

</details>

**一步一步看懂程式**
1. 等 2 秒讓相機開機
2. 相機切到「顏色追蹤 ③ 標準」（每色給 X、Y、面積、數量）
3. 一直重複
   - 如果第 1 色有找到
      - 有找到：綠燈
      - 否則：
         - 沒找到：紅燈

**跑起來應該看到什麼**：把積木拿到鏡頭前：綠燈（相機螢幕上也會框住積木）；拿開：紅燈。

**常見問題**
- **一直「沒找到」？** → 相機上還沒教「目標 1」，或教的時候十字沒對準積木。看相機螢幕：有框住積木才算找到；燈光差太多就重新教一次。
- **什麼都沒反應、或讀值一直是 -32768？** → 連接埠選錯（這頁都接連接埠 1）、插座片沒插緊、相機燒的不是 mBlock 版韌體，或模式不對（讀值積木只在對應的模式有值）。先用「神攝手 感應器正常？」積木確認連線，再用「目前模式編號」看模式（顏色追蹤 ③ ＝ 13、循線黑線 ＝ 41、物體清單 ＝ 51）。
- **上傳失敗、或積木是空白的？** → 確認右下角切到「上傳」模式、擴展 v2 已拖進 mBlock、USB 線接的是 mBot 主板。

---

## 範例 3：一個輪子轉向色塊

**這個範例會做什麼**：「第 1 色 x」是色塊中心在畫面的左右位置（像素 1 最左、80 中間、159 最右；0 ＝ 沒找到）。只用左輪：動力 ＝（x − 80）× 0.5，色塊在右邊左輪往前轉、車頭往右；在左邊倒轉、車頭往左；在中間不動。沒找到就停。

**要準備什麼**
- mBot（mCore 主板）＋ mBlock 5 電腦版
- mBot 兩輪小車（左輪接 M1、右輪接 M2，出廠就是這樣接）
- 神攝手（MakeBlock 版）＋ mBot 插座片
- 一塊顏色鮮豔的積木（紅、綠、藍都可以）

**怎麼接**：神攝手 → mBot 插座片 → mBot **連接埠 1**。鏡頭朝前、裝在車頭。先照上面「開始之前」在相機上教好目標 1 的顏色。

**下載程式**

| mBlock 5 專案 | 等價 Arduino 程式 |
|---|---|
| [ex03_one_wheel.mblock](/examples/unicam/mblock/ex03_one_wheel.mblock) | [ex03_one_wheel.ino](/examples/unicam/mblock/ex03_one_wheel/ex03_one_wheel.ino) |

<details open>
<summary>🧱 mBlock 積木（照著拼就好）</summary>

<pre class="blocks">當 mBot 啟動時 :: events hat
wait (2) seconds
神攝手 連接埠 [1 v] 設定模式 [顏色追蹤 ③ 標準 v] :: #E8590C
forever
  if &lt;神攝手 連接埠 [1 v] 第 [1 v] 色有找到？ :: #E8590C&gt; then
    左輪動力 (((神攝手 連接埠 [1 v] 第 [1 v] 色 x :: #E8590C) - (80)) * (0.5)) %，右輪動力 (0) % :: motion
  else
    停止移動 :: motion
  end
end</pre>

</details>

<details>
<summary>🔧 等價 Arduino 程式（mBot）</summary>

```cpp
// 神攝手 × mBlock 5　範例 3：一個輪子轉向色塊（mBot 等價 Arduino 程式）
// 接線：神攝手 → mBot 插座片 → mBot 連接埠 1。鏡頭朝前、裝在車頭。先照上面「開始之前」在相機上教好目標 1 的顏色。
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
  delay(2000);   // 等 2 秒讓相機開機
  ucSetMode(mbc_port1, 13);   // 相機切到「顏色追蹤 ③ 標準」（每色給 X、Y、面積、數量）
  while (1) {   // 一直重複
    if (ucFound(mbc_port1, 1)) {   // 如果第 1 色有找到
      wheels(((ucBlob(mbc_port1, 1, 0) - 80) * 0.5), 0);   // 左輪動力 ＝（第 1 色 x − 80）× 0.5（−40 ~ +40），右輪 0
    } else {
      wheels(0, 0);   // 沒找到：停
    }
  }
}

void loop() {}   // 這裡什麼都沒有：上面的積木不會結束
```

</details>

**一步一步看懂程式**
1. 等 2 秒讓相機開機
2. 相機切到「顏色追蹤 ③ 標準」（每色給 X、Y、面積、數量）
3. 一直重複
   - 如果第 1 色有找到
      - 左輪動力 ＝（第 1 色 x − 80）× 0.5（−40 ~ +40），右輪 0
      - 否則：
         - 沒找到：停

**跑起來應該看到什麼**：把積木拿到車子右前方：左輪往前轉、車頭轉向積木，對準就停；移到左邊，左輪倒轉。

**常見問題**
- **車子往反方向轉、越修越歪？** → 左右輪的「＋」「－」對調（或把左輪、右輪的動力互換）。mBot 出廠左輪 M1、右輪 M2，自己接線的請確認。
- **畫面左右跟實際相反？** → 相機鏡頭設定裡開了鏡像，關掉；或把 0.5 改成 -0.5。
- **一直「沒找到」？** → 相機上還沒教「目標 1」，或教的時候十字沒對準積木。看相機螢幕：有框住積木才算找到；燈光差太多就重新教一次。
- **什麼都沒反應、或讀值一直是 -32768？** → 連接埠選錯（這頁都接連接埠 1）、插座片沒插緊、相機燒的不是 mBlock 版韌體，或模式不對（讀值積木只在對應的模式有值）。先用「神攝手 感應器正常？」積木確認連線，再用「目前模式編號」看模式（顏色追蹤 ③ ＝ 13、循線黑線 ＝ 41、物體清單 ＝ 51）。
- **上傳失敗、或積木是空白的？** → 確認右下角切到「上傳」模式、擴展 v2 已拖進 mBlock、USB 線接的是 mBot 主板。

---

## 範例 4：追色塊、靠近就停＋燈號

**這個範例會做什麼**：兩個輪子追色塊：轉向 ＝（第 1 色 x − 80）× 0.4，左輪 35 ＋ 轉向、右輪 35 − 轉向。再加「第 1 色 面積 %」：沒找到停車亮紅燈；面積大於 15 %（積木很近了）停車亮藍燈、嗶一聲；其他時候追色塊亮綠燈。

**要準備什麼**
- mBot（mCore 主板）＋ mBlock 5 電腦版
- mBot 兩輪小車（左輪接 M1、右輪接 M2，出廠就是這樣接）
- 神攝手（MakeBlock 版）＋ mBot 插座片
- 一塊顏色鮮豔的積木（紅、綠、藍都可以）

**怎麼接**：神攝手 → mBot 插座片 → mBot **連接埠 1**。鏡頭朝前、裝在車頭。先照上面「開始之前」在相機上教好目標 1 的顏色。

**下載程式**

| mBlock 5 專案 | 等價 Arduino 程式 |
|---|---|
| [ex04_follow_stop.mblock](/examples/unicam/mblock/ex04_follow_stop.mblock) | [ex04_follow_stop.ino](/examples/unicam/mblock/ex04_follow_stop/ex04_follow_stop.ino) |

<details open>
<summary>🧱 mBlock 積木（照著拼就好）</summary>

<pre class="blocks">當 mBot 啟動時 :: events hat
wait (2) seconds
神攝手 連接埠 [1 v] 設定模式 [顏色追蹤 ③ 標準 v] :: #E8590C
forever
  if &lt;神攝手 連接埠 [1 v] 第 [1 v] 色有找到？ :: #E8590C&gt; then
    if &lt;(神攝手 連接埠 [1 v] 第 [1 v] 色 面積 % :: #E8590C) &gt; (15)&gt; then
      停止移動 :: motion
      LED 燈位置 [全部 v] 的三原色數值為 紅 (0) 綠 (0) 藍 (255) :: looks
      播放音頻 (880) 赫茲，持續 (0.3) 秒 :: sound
    else
      LED 燈位置 [全部 v] 的三原色數值為 紅 (0) 綠 (255) 藍 (0) :: looks
      set [轉向 v] to (((神攝手 連接埠 [1 v] 第 [1 v] 色 x :: #E8590C) - (80)) * (0.4))
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
// 神攝手 × mBlock 5　範例 4：追色塊、靠近就停＋燈號（mBot 等價 Arduino 程式）
// 接線：神攝手 → mBot 插座片 → mBot 連接埠 1。鏡頭朝前、裝在車頭。先照上面「開始之前」在相機上教好目標 1 的顏色。
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
  delay(2000);   // 等 2 秒讓相機開機
  ucSetMode(mbc_port1, 13);   // 相機切到「顏色追蹤 ③ 標準」（每色給 X、Y、面積、數量）
  while (1) {   // 一直重複
    if (ucFound(mbc_port1, 1)) {   // 如果第 1 色有找到
      if ((ucBlob(mbc_port1, 1, 5) > 15)) {   // 如果第 1 色面積大於 15 %
        wheels(0, 0);   // 色塊很大（很近了）：停車
        rgbled.setColor(0, 0, 0, 255); rgbled.show();   // 藍燈
        buzzer.tone(880, 300);   // 嗶一聲
      } else {
        rgbled.setColor(0, 0, 255, 0); rgbled.show();   // 綠燈
        turn = ((ucBlob(mbc_port1, 1, 0) - 80) * 0.4);   // 轉向 ＝（第 1 色 x − 80）× 0.4（−32 ~ +32）
        wheels((35 + turn), (35 - turn));   // 左輪 ＝ 35 ＋ 轉向、右輪 ＝ 35 − 轉向：色塊在右（正）就左輪快、往右轉
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
1. 等 2 秒讓相機開機
2. 相機切到「顏色追蹤 ③ 標準」（每色給 X、Y、面積、數量）
3. 一直重複
   - 如果第 1 色有找到
      - 如果第 1 色面積大於 15 %
         - 色塊很大（很近了）：停車
         - 藍燈
         - 嗶一聲
         - 否則：
            - 綠燈
            - 轉向 ＝（第 1 色 x − 80）× 0.4（−32 ~ +32）
            - 左輪 ＝ 35 ＋ 轉向、右輪 ＝ 35 − 轉向：色塊在右（正）就左輪快、往右轉
      - 否則：
         - 沒找到：停車
         - 紅燈

**跑起來應該看到什麼**：積木放在前方 1 公尺：車子亮綠燈開過去，到積木前面變藍燈、嗶一聲停下；把積木拿走變紅燈。

**常見問題**
- **還很遠就停了、或撞到積木才停？** → 面積跟積木大小有關：把積木放在想停的距離，看相機螢幕底部的數字（或用「第 1 色 面積 %」積木）是多少，再改 15。
- **車子往反方向轉、越修越歪？** → 左右輪的「＋」「－」對調（或把左輪、右輪的動力互換）。mBot 出廠左輪 M1、右輪 M2，自己接線的請確認。
- **一直「沒找到」？** → 相機上還沒教「目標 1」，或教的時候十字沒對準積木。看相機螢幕：有框住積木才算找到；燈光差太多就重新教一次。
- **什麼都沒反應、或讀值一直是 -32768？** → 連接埠選錯（這頁都接連接埠 1）、插座片沒插緊、相機燒的不是 mBlock 版韌體，或模式不對（讀值積木只在對應的模式有值）。先用「神攝手 感應器正常？」積木確認連線，再用「目前模式編號」看模式（顏色追蹤 ③ ＝ 13、循線黑線 ＝ 41、物體清單 ＝ 51）。
- **上傳失敗、或積木是空白的？** → 確認右下角切到「上傳」模式、擴展 v2 已拖進 mBlock、USB 線接的是 mBot 主板。

---

## 範例 5：看著黑線走

**這個範例會做什麼**：相機切到「循線 ① 黑線」，讀「近處偏移」（−100 ~ 100：線在左＝負、右＝正、正中間 0）。轉向 ＝ 近處偏移 × 0.3，左輪 30 ＋ 轉向、右輪 30 − 轉向；看不到線就停車亮紅燈。

**要準備什麼**
- mBot（mCore 主板）＋ mBlock 5 電腦版
- mBot 兩輪小車（左輪接 M1、右輪接 M2，出廠就是這樣接）
- 神攝手（MakeBlock 版）＋ mBot 插座片
- 白色地面上貼一條黑色膠帶（寬 2～3 公分）

**怎麼接**：神攝手 → mBot 插座片 → mBot **連接埠 1**。鏡頭裝在車頭、斜斜往前下方看地面（看得到車前 10～30 公分的線）。

**下載程式**

| mBlock 5 專案 | 等價 Arduino 程式 |
|---|---|
| [ex05_line_follow.mblock](/examples/unicam/mblock/ex05_line_follow.mblock) | [ex05_line_follow.ino](/examples/unicam/mblock/ex05_line_follow/ex05_line_follow.ino) |

<details open>
<summary>🧱 mBlock 積木（照著拼就好）</summary>

<pre class="blocks">當 mBot 啟動時 :: events hat
wait (2) seconds
神攝手 連接埠 [1 v] 設定模式 [循線 ① 黑線 v] :: #E8590C
forever
  if &lt;神攝手 連接埠 [1 v] 看到線？ :: #E8590C&gt; then
    LED 燈位置 [全部 v] 的三原色數值為 紅 (0) 綠 (255) 藍 (0) :: looks
    set [轉向 v] to ((神攝手 連接埠 [1 v] 循線 [近處偏移 v] :: #E8590C) * (0.3))
    左輪動力 ((30) + (轉向)) %，右輪動力 ((30) - (轉向)) % :: motion
  else
    停止移動 :: motion
    LED 燈位置 [全部 v] 的三原色數值為 紅 (255) 綠 (0) 藍 (0) :: looks
  end
end</pre>

</details>

<details>
<summary>🔧 等價 Arduino 程式（mBot）</summary>

```cpp
// 神攝手 × mBlock 5　範例 5：看著黑線走（mBot 等價 Arduino 程式）
// 接線：神攝手 → mBot 插座片 → mBot 連接埠 1。鏡頭裝在車頭、斜斜往前下方看地面（看得到車前 10～30 公分的線）。
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
  delay(2000);   // 等 2 秒讓相機開機
  ucSetMode(mbc_port1, 41);   // 相機切到「循線 ① 黑線」
  while (1) {   // 一直重複
    if (ucLineSeen(mbc_port1)) {   // 如果看到線
      rgbled.setColor(0, 0, 255, 0); rgbled.show();   // 看到線：綠燈
      turn = (ucLine(mbc_port1, 0) * 0.3);   // 轉向 ＝ 近處偏移 × 0.3（−30 ~ +30）
      wheels((30 + turn), (30 - turn));   // 左輪 ＝ 30 ＋ 轉向、右輪 ＝ 30 − 轉向：線在右（正）就往右轉
    } else {
      wheels(0, 0);   // 看不到線：停車
      rgbled.setColor(0, 255, 0, 0); rgbled.show();   // 紅燈
    }
  }
}

void loop() {}   // 這裡什麼都沒有：上面的積木不會結束
```

</details>

**一步一步看懂程式**
1. 等 2 秒讓相機開機
2. 相機切到「循線 ① 黑線」
3. 一直重複
   - 如果看到線
      - 看到線：綠燈
      - 轉向 ＝ 近處偏移 × 0.3（−30 ~ +30）
      - 左輪 ＝ 30 ＋ 轉向、右輪 ＝ 30 − 轉向：線在右（正）就往右轉
      - 否則：
         - 看不到線：停車
         - 紅燈

**跑起來應該看到什麼**：車子放在黑線上：亮綠燈沿著線走，彎道會跟著轉；把車子拿離線或走到線的盡頭就停車亮紅燈。

**常見問題**
- **車子左右搖、蛇行？** → 把 0.3 改小（例如 0.2），或把 30 改小讓車子慢一點。
- **一直紅燈、看不到線？** → 鏡頭看不到地面（太平、太高）或光線太暗；看相機螢幕上有沒有畫出找到的線。白線請把模式改成「循線 ② 白線」。
- **車子往反方向轉、越修越歪？** → 左右輪的「＋」「－」對調（或把左輪、右輪的動力互換）。mBot 出廠左輪 M1、右輪 M2，自己接線的請確認。
- **什麼都沒反應、或讀值一直是 -32768？** → 連接埠選錯（這頁都接連接埠 1）、插座片沒插緊、相機燒的不是 mBlock 版韌體，或模式不對（讀值積木只在對應的模式有值）。先用「神攝手 感應器正常？」積木確認連線，再用「目前模式編號」看模式（顏色追蹤 ③ ＝ 13、循線黑線 ＝ 41、物體清單 ＝ 51）。
- **上傳失敗、或積木是空白的？** → 確認右下角切到「上傳」模式、擴展 v2 已拖進 mBlock、USB 線接的是 mBot 主板。

---

## 範例 6：追紅色三角形

**這個範例會做什麼**：相機切到「物體偵測 ① 物體清單」，相機會把每個色塊判斷成形狀（圓、三角形、正方形…）。用「有 [顏色 1] 的 [三角形] 物體？」判斷有沒有看到紅色三角形；有就用「是第幾個」找出它是第幾個物體、讀「第 (第幾個) 個物體 x」轉向它，亮綠燈往前開；只看到紅色圓形或什麼都沒看到就停車亮紅燈。

**要準備什麼**
- mBot（mCore 主板）＋ mBlock 5 電腦版
- mBot 兩輪小車（左輪接 M1、右輪接 M2，出廠就是這樣接）
- 神攝手（MakeBlock 版）＋ mBot 插座片
- 一塊顏色鮮豔的積木（紅、綠、藍都可以）
- 紅色厚紙剪一個三角形（邊長 6～10 公分），再剪一個一樣大的紅色圓形來比較

**怎麼接**：神攝手 → mBot 插座片 → mBot **連接埠 1**。鏡頭朝前、裝在車頭。先照上面「開始之前」在相機上把**目標 1 教成紅色**（十字對準紅色三角形）。

**下載程式**

| mBlock 5 專案 | 等價 Arduino 程式 |
|---|---|
| [ex06_red_triangle.mblock](/examples/unicam/mblock/ex06_red_triangle.mblock) | [ex06_red_triangle.ino](/examples/unicam/mblock/ex06_red_triangle/ex06_red_triangle.ino) |

<details open>
<summary>🧱 mBlock 積木（照著拼就好）</summary>

<pre class="blocks">當 mBot 啟動時 :: events hat
wait (2) seconds
神攝手 連接埠 [1 v] 設定模式 [物體偵測 ① 物體清單 v] :: #E8590C
forever
  if &lt;神攝手 連接埠 [1 v] 有 [顏色 1 v] 的 [三角形 v] 物體？ :: #E8590C&gt; then
    LED 燈位置 [全部 v] 的三原色數值為 紅 (0) 綠 (255) 藍 (0) :: looks
    set [第幾個 v] to (神攝手 連接埠 [1 v] [顏色 1 v] 的 [三角形 v] 物體是第幾個 :: #E8590C)
    set [轉向 v] to (((神攝手 連接埠 [1 v] 第 (第幾個) 個物體 x :: #E8590C) - (80)) * (0.4))
    左輪動力 ((25) + (轉向)) %，右輪動力 ((25) - (轉向)) % :: motion
  else
    停止移動 :: motion
    LED 燈位置 [全部 v] 的三原色數值為 紅 (255) 綠 (0) 藍 (0) :: looks
  end
end</pre>

</details>

<details>
<summary>🔧 等價 Arduino 程式（mBot）</summary>

```cpp
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
```

</details>

**一步一步看懂程式**
1. 等 2 秒讓相機開機
2. 相機切到「物體偵測 ① 物體清單」（每個物體給色號、形狀、X、Y）
3. 一直重複
   - 如果有顏色 1 的三角形物體
      - 看到了：綠燈
      - 第幾個 ＝ 顏色 1 的三角形是第幾個物體（物體 1 最大；1 ~ 8）
      - 轉向 ＝（第「第幾個」個物體 x − 80）× 0.4（−32 ~ +32）
      - 左輪 ＝ 25 ＋ 轉向、右輪 ＝ 25 − 轉向：三角形在右（正）就往右轉
      - 否則：
         - 沒看到紅色三角形：停車
         - 紅燈

**跑起來應該看到什麼**：把紅色三角形拿到車子前方：亮綠燈、車頭轉向三角形慢慢開過去；換成紅色圓形：紅燈停車；兩個一起放，車子追的是三角形。

**常見問題**
- **三角形也一直紅燈？** → 形狀是相機從輪廓判斷的：三角形太小（佔畫面太少）、太斜、邊緣破損、或被手擋住一角都會判成別的形狀。拿近一點、擺正，看相機螢幕上它被標成什麼形狀；也可以先用「有 [顏色 1] 的 [任何形狀] 物體？」確認顏色有教對。
- **只能用顏色 1～8？** → 對，物體偵測只看目標色 1～8（9～16 色照樣能做顏色追蹤，只是不當物體）。
- **車子往反方向轉、越修越歪？** → 左右輪的「＋」「－」對調（或把左輪、右輪的動力互換）。mBot 出廠左輪 M1、右輪 M2，自己接線的請確認。
- **一直「沒找到」？** → 相機上還沒教「目標 1」，或教的時候十字沒對準積木。看相機螢幕：有框住積木才算找到；燈光差太多就重新教一次。
- **什麼都沒反應、或讀值一直是 -32768？** → 連接埠選錯（這頁都接連接埠 1）、插座片沒插緊、相機燒的不是 mBlock 版韌體，或模式不對（讀值積木只在對應的模式有值）。先用「神攝手 感應器正常？」積木確認連線，再用「目前模式編號」看模式（顏色追蹤 ③ ＝ 13、循線黑線 ＝ 41、物體清單 ＝ 51）。
- **上傳失敗、或積木是空白的？** → 確認右下角切到「上傳」模式、擴展 v2 已拖進 mBlock、USB 線接的是 mBot 主板。

---

## 下一步

- 全部積木、值編號、各主板可用的連接埠：[神攝手 mBlock 教學](/sensors/unicam/mblock/index.md)
- 同樣的相機模式在樂高上怎麼用：[SPIKE 範例](/sensors/unicam/spike-examples.md)、[EV3 範例](/sensors/unicam/ev3-examples.md)
- 想用 Arduino 直接接感應器（I2C 版，不經 mBot）：[Arduino 範例](/sensors/unicam/arduino-examples.md)
