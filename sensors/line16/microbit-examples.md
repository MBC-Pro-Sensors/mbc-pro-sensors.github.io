<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 W4 micro:bit 範例產生器產生（scratchpad w4/gen_pages.mjs＋spec/line16.mjs）；程式碼區塊來自 examples/line16/microbit/*.ts，改程式請改 .ts 後重跑產生器 -->
# 🟧 micro:bit 範例：循行者 16 路

[🔙 回到循行者 16 路 micro:bit 頁](/sensors/line16/microbit.md)　｜　[📖 產品頁](/sensors/line16/index.md)　｜　[📋 指令參考](/sensors/line16/reference.md)

這一頁是寫給**完全沒寫過程式**的人。5 個範例從「LED 顯示讀值」開始，一步一步做到「開車」。全部用 **MakeCode 網頁版**＋MBC 擴充 **pxt-mbc-sensors v2** 寫：切到 JavaScript 把範例貼上就能用，也可以切回積木模式看它變成哪些積木。

> [!IMPORTANT]
> - 擴充 **pxt-mbc-sensors v2** 還沒上機驗證。本頁的範例都在電腦上用 MakeCode CLI 編譯通過（V1＋V2 各一次），**數值方向、就緒時間、供電**等實機結果見下方「硬體關卡」的「上機待測」。
> - 請用 **通用 I2C 版**的產品。SPIKE 版、EV3 版韌體不同，接 micro:bit 讀不到。

## 軟體對照表

| 寫法 | micro:bit V1 | micro:bit V2 | 怎麼用 |
|---|---|---|---|
| **MakeCode 網頁版**（[makecode.microbit.org](https://makecode.microbit.org/)，積木／JavaScript） | ✔（記憶體較小，本頁範例都編得過） | ✔ | 新專案 → 齒輪 → 擴充 → 貼 `https://github.com/contactmbcrobot/pxt-mbc-sensors` → 右上切到 **JavaScript** → 貼上範例 → 下載 .hex 拖進 MICROBIT 磁碟 |
| MakeCode 離線版（Windows／Mac App） | ✔ | ✔ | 同上；第一次加擴充要連網 |
| Python Editor（MicroPython） | — | — | 還沒有 MBC 模組 |

- **聲音**：V2 有內建喇叭；V1 要在 **P0** 接蜂鳴器或耳機才有聲音。
- **伺服／馬達**：範例用 `pins.servoWritePin`，一顆 9g 伺服（SG90）接 P1；開車用兩顆**連續旋轉伺服**（FS90R 之類），左輪 P1、右輪 P2。伺服電源一律**外接 4.8～6 V 電池盒**，電池負極要和 micro:bit GND 接在一起（共地）；不要從 micro:bit 3V 腳推伺服。
- **積木文字**：MakeCode 語言切成繁體中文，擴充的積木就是中文（例如「循行者 線位置」）；本頁表格同時列出中文積木和 JavaScript 寫法。

## 開始之前：硬體關卡（上機待測）

micro:bit 是 3.3 V 的板子，接 MBC 產品前每一關都要過；打「上機待測」的是擴充 v2 還沒在實機確認的項目。

| 關卡 | 這款產品 | 狀態 |
|---|---|---|
| 電壓 | MBC 產品的 I2C 都是 3.3 V 準位，直接接 P19／P20，不用準位轉換 | ✔ |
| 供電 | 16 路約 45 mA，建議外接 5 V；只用 micro:bit 3V 腳（V1 約 90 mA、V2 約 190 mA 上限）要實測 | 上機待測 |
| I2C 上拉 | micro:bit 板上已有 4.7 kΩ；產品板若也有上拉，並聯後要確認低電位 ≤ 0.3×VDD | 上機待測 |
| 時脈延展 | STM32 從機預設會時脈延展，V2（nRF52833）對延展有已知問題，要 V1、V2 各測一次 | 上機待測 |
| 位址衝突 | 出廠位址 0x16，和 8 路一樣；8 路和 16 路同時接要先換掉一個的位址 | — |
| 就緒時間 | 開機後馬上就緒（F3 b0 恆 1） | 上機待測 |

讀不到值時先看：接線（SDA＝P20、SCL＝P19）→ 外接電源有沒有開 → 擴充「裝置」分類的「位址 … 的裝置就緒？」積木是不是 true。

### 接線（所有範例共用）

<figure class="ref-fig"><img src="/images/figures/line16/wiring-microbit.svg" alt="循行者 16 路 × micro:bit" loading="lazy"></figure>

| 感應器 | micro:bit |
| :--- | :--- |
| SDA | P20 |
| SCL | P19 |
| VCC | 外接 5 V（或試 3V） |
| GND | GND（伺服電池負極也接這裡） |

## 這頁會用到的數值

「左、右」一律是**感應器裝在車上、你站在車子後面往前看**：線在左 = 負數，第 1 路在最左。

| 積木（繁中） | JavaScript | 範圍 | 意思 | 用在 |
| :--- | :--- | :--- | :--- | :--- |
| 循行者 線位置 | `mbc.linePosition()` | −16～+16 | 線在哪一路，0 = 正中間，負 = 線在左 | 1, 3, 4 |
| 循行者 細位置 | `mbc.linePositionFine()` | −100～+100 | 線位置的細分，跟線位置同號 | 5 |
| 循行者 看到線？ | `mbc.lineSeen()` | true / false | 至少有一路壓到線 | 1, 3, 4, 5 |
| 循行者 線寬 | `mbc.lineWidth()` | 0～16 | 壓到線的路數，0 = 沒線 | 1 |
| 循行者 最後離線方向 | `mbc.lineLastDirection()` | −16～+16 | 丟線前線在哪一側，負 = 左 | 5 |
| 當循行者 丟線／找回線／分岔 | `mbc.onLineEvent(...)` | 事件 | 線段數變成 0／變回 >0／變成 ≥2 時各觸發一次 | 2, 5 |
| 循行者 校準 自動（來回掃 5 秒） | `mbc.lineCalibrate(MbcLineCalib.Auto)` | 命令 E7 | 5 秒內在黑線和白底上來回掃，存進感應器 | 2 |
| 循行者 校準中？／校準次數 | `mbc.lineCalibrating() / mbc.lineCalibrationCount()` | true / false、0～255 | 校準旗標（F3 b2）與開機後完成次數 | 2 |
| 循行者 循 黑／白 線 | `mbc.lineSetColor(MbcLineColor.Black)` | 命令 E0／E1 | 白底黑線（預設）或黑底白線 | 2 |
| 等位址 … 的裝置就緒 | `mbc.waitReady(mbc.ADDR_LINE, 3000)` | true / false | 最多等 3 秒，感應器有回應就 true（進階 → 裝置） | 1 |

---

## 範例 1：線在哪一路

**這個範例會做什麼**：LED 上的一根光柱跟著線跑：線在左邊光柱就在左邊，線在正中間光柱在中間。按 A 顯示線位置的數字、按 B 顯示線寬。可以用來確認接線正確、方向沒弄反。

**要準備什麼**
- micro:bit V1 或 V2＋USB 線
- 循行者 16 路（通用 I2C 版）＋4P 線
- 一張白紙＋一條黑色電工膠帶（約 2 cm 寬）

**怎麼接**：照上面的接線表：SDA → P20、SCL → P19、VCC → 3V、GND → GND。

**下載程式**：[ex01_show_position.ts](/examples/line16/microbit/ex01_show_position.ts)（右鍵另存）→ MakeCode 新專案 → 齒輪 → 擴充 → 貼 `https://github.com/contactmbcrobot/pxt-mbc-sensors` → 右上切到 **JavaScript** → 全選刪掉原本的內容、把檔案內容貼上 → **下載**，把 .hex 拖進 MICROBIT 磁碟。

**程式**

```typescript
// 循行者 16 路 × micro:bit 範例 1：線在哪一路（LED 光柱跟著線跑）
// 做什麼：把感應器在黑線上左右移動，LED 上的一根光柱會跟著線跑；按 A 顯示線位置的數字、按 B 顯示線寬。
// 接線：感應器 SDA → P20、SCL → P19、VCC → 3V（或外接 5 V）、GND → GND。
// 準備：MakeCode 新專案 → 擴充 → 貼 https://github.com/contactmbcrobot/pxt-mbc-sensors → 切到 JavaScript 把本檔全部貼上 → 下載到 micro:bit。
// 看到什麼：開機先打勾（找到感應器）；線在左 → 光柱在左邊；線在正中間 → 光柱在中間；沒線 → 畫一個 X。

const CH = 16                                              // 路數：16 路版 8、16 路版 16
let busy = false                                          // 按鈕顯示數字時，暫停畫光柱

if (mbc.waitReady(mbc.ADDR_LINE, 3000)) {                 // 最多等 3 秒，看感應器有沒有接好
    basic.showIcon(IconNames.Yes)
} else {
    basic.showIcon(IconNames.No)                          // 沒回應：檢查 P19／P20 和電源
}

basic.forever(function () {
    if (busy) {
        basic.pause(100)
        return
    }
    if (mbc.lineSeen()) {
        let pos = mbc.linePosition()                      // −CH（最左）~ +CH（最右），0 = 正中間
        let x = Math.round(Math.map(pos, -CH, CH, 0, 4))  // 換算成 LED 的第 0~4 行
        basic.clearScreen()
        for (let y = 0; y < 5; y++) {
            led.plot(x, y)                                // 畫一整根光柱
        }
    } else {
        basic.showIcon(IconNames.No, 0)                   // 沒看到線
    }
    basic.pause(50)
})

input.onButtonPressed(Button.A, function () {
    busy = true
    basic.showNumber(mbc.linePosition())                  // 線位置：負 = 線在左（站在車後往前看）
    busy = false
})
input.onButtonPressed(Button.B, function () {
    busy = true
    basic.showNumber(mbc.lineWidth())                     // 線寬：壓到線的路數，0 = 沒線
    busy = false
})
```

**一步一步看懂程式**
1. `const CH = 16`：路數。8 路版改回 8，其他不用動。
2. `mbc.waitReady(mbc.ADDR_LINE, 3000)`：最多等 3 秒看感應器有沒有回應，有就打勾、沒有就 X。
3. `basic.forever`：一直重複。先問 `mbc.lineSeen()`「有看到線嗎」，沒有就畫 X。
4. 有線：`mbc.linePosition()` 讀線位置（−16～+16），用 `Math.map` 換算成 LED 的第 0～4 行，再用迴圈 `led.plot` 把那一行 5 顆燈全部點亮。
5. 按 A／B：`basic.showNumber` 把數字捲過螢幕；`busy` 這個變數讓光柱在捲數字時先暫停，兩邊才不會搶螢幕。

**跑起來應該看到什麼**
- 開機：打勾（找到感應器）。
- 感應器放在白紙上、黑線在感應器正中間：光柱在中間那一行；按 A 顯示 0。
- 把感應器往右移（線跑到感應器左邊）：光柱往左跑；按 A 顯示負數。
- 拿離紙面：X。

**常見問題**
- **光柱方向跟線相反？** → 感應器裝反了（第 1 路要在車子左邊）。把感應器轉 180°，不要改程式。
- **光柱一直在中間不動？** → 還沒校準：先跑範例 2 按 A 做自動校準，或讀值全 0（看範例 1 的 X 有沒有出現）。
- **開機就是 X（找不到感應器）？** → 看接線 SDA＝P20、SCL＝P19 有沒有接反；外接電源有沒有開；用鱷魚夾接的話夾子有沒有碰到旁邊的腳。
- **模擬器裡數字都是 0？** → 正常。MakeCode 模擬器沒有真的 I2C，讀到的都是 0；要看真的數值一定要下載到 micro:bit。
- **V1 沒有聲音？** → V1 沒有喇叭，在 P0 和 GND 之間接蜂鳴器或耳機。

---

## 範例 2：丟線警報＋一鍵校準

**這個範例會做什麼**：看到線 → 笑臉；線不見 → 哭臉並嗶一聲。按 A 做 5 秒自動校準（LED 倒數 5 4 3 2 1），按 B 切換黑線／白線模式。這是用「事件」積木寫的：線丟掉的那一瞬間才觸發，不是一直問。

**要準備什麼**
- 範例 1 的東西
- V1 另外要蜂鳴器或耳機接 P0

**怎麼接**：同範例 1。

**下載程式**：[ex02_line_alarm.ts](/examples/line16/microbit/ex02_line_alarm.ts)（右鍵另存）→ MakeCode 新專案 → 齒輪 → 擴充 → 貼 `https://github.com/contactmbcrobot/pxt-mbc-sensors` → 右上切到 **JavaScript** → 全選刪掉原本的內容、把檔案內容貼上 → **下載**，把 .hex 拖進 MICROBIT 磁碟。

**程式**

```typescript
// 循行者 16 路 × micro:bit 範例 2：丟線警報＋一鍵校準
// 做什麼：看到線 → 笑臉；丟線 → 哭臉並嗶一聲。按 A 做 5 秒自動校準（LED 倒數），按 B 切換黑線／白線。
// 接線：感應器 SDA → P20、SCL → P19、VCC → 3V（或外接 5 V）、GND → GND。V1 要在 P0 接蜂鳴器或耳機才有聲音（V2 有內建喇叭）。
// 準備：一張白紙＋黑色電工膠帶貼一條線。擴充與貼程式的方法同範例 1。
// 看到什麼：感應器在線上 → 笑臉；拿開 → 哭臉＋嗶；按 A → 5 4 3 2 1 倒數、顯示校準次數、打勾。

let white = false                                         // 目前是不是白線模式

mbc.lineSetColor(MbcLineColor.Black)                      // 開機先設成追黑線（預設）
basic.showIcon(IconNames.Happy)

mbc.onLineEvent(MbcLineEvent.Lost, function () {          // 線段數從 >0 變 0 時觸發一次
    basic.showIcon(IconNames.Sad, 0)
    music.playTone(Note.C, music.beat(BeatFraction.Half)) // 低音 Do 響半拍
})
mbc.onLineEvent(MbcLineEvent.Found, function () {         // 重新看到線
    basic.showIcon(IconNames.Happy, 0)
})

input.onButtonPressed(Button.A, function () {
    mbc.lineCalibrate(MbcLineCalib.Auto)                  // 5 秒內把感應器在黑線和白底上來回掃
    for (let i = 5; i > 0; i--) {
        basic.showNumber(i)                               // 倒數
        basic.pause(1000)
    }
    while (mbc.lineCalibrating()) {                       // 等感應器的「校準中」旗標歸零
        basic.pause(100)
    }
    basic.showNumber(mbc.lineCalibrationCount())          // 開機後完成校準的次數
    basic.showIcon(IconNames.Yes)
})

input.onButtonPressed(Button.B, function () {
    white = !white
    mbc.lineSetColor(white ? MbcLineColor.White : MbcLineColor.Black)   // 白底黑線 ↔ 黑底白線
    basic.showString(white ? "W" : "B")
})
```

**一步一步看懂程式**
1. `mbc.lineSetColor(MbcLineColor.Black)`：先設成追黑線（命令 E0）。
2. `mbc.onLineEvent(MbcLineEvent.Lost, ...)`：線段數從 >0 變 0 時執行一次 → 哭臉＋`music.playTone` 嗶半拍。`Found` 那個是重新看到線 → 笑臉。
3. 按 A：`mbc.lineCalibrate(MbcLineCalib.Auto)` 送出命令 E7，感應器開始 5 秒校準；程式同時用 `for` 迴圈倒數。
4. `while (mbc.lineCalibrating())`：校準旗標（F3 b2）還是 1 就再等 0.1 秒，歸零才往下。
5. `mbc.lineCalibrationCount()`：顯示開機後完成的校準次數，確認真的做完了。
6. 按 B：變數 `white` 翻轉，`lineSetColor` 在黑線（E0）和白線（E1）之間切換，螢幕顯示 B 或 W。

**跑起來應該看到什麼**
- 感應器在線上：笑臉。拿開：哭臉＋嗶一聲（只嗶一次，不會一直響）。
- 按 A，在 5 秒內把感應器在黑線和白紙上來回掃幾次 → 顯示次數（第一次是 1）→ 打勾。之後線位置會更準。
- 按 B 顯示 W：黑紙白線才會被當成線。

**常見問題**
- **按 A 之後一直沒顯示次數？** → 校準旗標沒歸零，通常是感應器沒接好（讀值全 0 時 `lineCalibrating()` 是 false，會馬上跳過）。先跑範例 1 看有沒有打勾。
- **丟線時沒聲音？** → V1 要接蜂鳴器；V2 確認沒有在 MakeCode 裡把喇叭關掉。

---

## 範例 3：方向盤伺服跟著線轉

**這個範例會做什麼**：一顆 9g 伺服當方向盤：線在左伺服就往左轉，線越偏轉越多（最外側 80°）；丟線回正中。這是第一次讓感應器「動」東西。按 A 顯示目前角度。

**要準備什麼**
- 範例 1 的東西，先做過一次範例 2 的校準
- 9g 伺服（SG90）×1
- 4.8～6 V 電池盒（4 顆 AA）＋杜邦線

**怎麼接**：感應器同範例 1。伺服 **橘（訊號）→ P1**、紅 → 外接 4.8～6 V 電池正極、棕 → 電池負極，電池負極再接一條到 micro:bit GND（共地）。

**下載程式**：[ex03_steer_servo.ts](/examples/line16/microbit/ex03_steer_servo.ts)（右鍵另存）→ MakeCode 新專案 → 齒輪 → 擴充 → 貼 `https://github.com/contactmbcrobot/pxt-mbc-sensors` → 右上切到 **JavaScript** → 全選刪掉原本的內容、把檔案內容貼上 → **下載**，把 .hex 拖進 MICROBIT 磁碟。

**程式**

```typescript
// 循行者 16 路 × micro:bit 範例 3：方向盤伺服跟著線轉
// 做什麼：一顆 9g 伺服（SG90）當方向盤：線在左 → 伺服往左轉，線越偏轉越多；丟線 → 回正中。按 A 顯示目前角度。
// 接線：感應器 SDA → P20、SCL → P19、VCC → 3V、GND → GND。伺服 橘（訊號）→ P1、紅 → 外接 4.8~6 V 電池正極、棕 → 電池負極並接 micro:bit GND（共地）。
// 準備：範例 2 的黑線紙＋先做過一次校準。
// 看到什麼：感應器往線的左邊移 → 伺服擺臂往一邊轉；移到右邊 → 往另一邊；拿開 → 回到 90°。

const CH = 16                                              // 路數：16 路版 8、16 路版 16
const GAIN = Math.idiv(80, CH)                            // 每偏一路轉幾度：最外側剛好 ±80°
let angle = 90

basic.forever(function () {
    if (mbc.lineSeen()) {
        angle = 90 + mbc.linePosition() * GAIN            // 線在左（負）→ 角度小於 90
    } else {
        angle = 90                                        // 沒線：回正
    }
    angle = Math.constrain(angle, 0, 180)
    pins.servoWritePin(AnalogPin.P1, angle)               // 0~180°
    basic.pause(30)
})

input.onButtonPressed(Button.A, function () {
    basic.showNumber(angle)
})
```

**一步一步看懂程式**
1. `GAIN = Math.idiv(80, CH)`：每偏一路轉幾度。16 路是 5°，8 路是 10°，最外側都剛好 80°。
2. 有線時 `angle = 90 + 線位置 × GAIN`：線在左（負）角度小於 90，線在右大於 90。
3. 沒線時 `angle = 90` 回正。
4. `Math.constrain(angle, 0, 180)`：伺服只能 0～180°，超過就夾住。
5. `pins.servoWritePin(AnalogPin.P1, angle)`：把角度送到 P1 的伺服。每 30 ms 更新一次。

**跑起來應該看到什麼**
- 線在正中間：伺服在 90°。
- 感應器往右移（線跑到左邊）：擺臂往一邊轉；越偏轉越多。
- 拿開：回 90°。按 A 顯示 90。

**常見問題**
- **伺服一直抖、或 micro:bit 一直重開？** → 伺服不能用 micro:bit 的 3V 腳供電，一定要外接電池盒，而且電池負極要接 micro:bit GND。
- **轉的方向跟你想的相反？** → 把 `90 + ...` 改成 `90 - ...` 就好，看伺服裝的方向。

---

## 範例 4：循線小車（三段式）

**這個範例會做什麼**：按 A 開車：線在左 → 左轉、線在右 → 右轉、正中間 → 直走；丟線 → 停。按 B 停車。只有三種動作，最容易看懂的循線。

**要準備什麼**
- 兩輪小車：兩顆連續旋轉伺服（FS90R 之類）當輪子＋一顆萬向輪
- 循行者裝在車頭、離地 0.5～1 cm
- 4.8～6 V 電池盒
- 黑線跑道（白紙＋黑膠帶，轉彎不要太急）

**怎麼接**：感應器同範例 1，裝在車頭離地 0.5～1 cm。**左輪連續旋轉伺服訊號 → P1、右輪 → P2**，兩顆伺服的紅線接 4.8～6 V 電池盒正極、棕線接電池負極，電池負極再接 micro:bit GND。

**下載程式**：[ex04_simple_follow.ts](/examples/line16/microbit/ex04_simple_follow.ts)（右鍵另存）→ MakeCode 新專案 → 齒輪 → 擴充 → 貼 `https://github.com/contactmbcrobot/pxt-mbc-sensors` → 右上切到 **JavaScript** → 全選刪掉原本的內容、把檔案內容貼上 → **下載**，把 .hex 拖進 MICROBIT 磁碟。

**程式**

```typescript
// 循行者 16 路 × micro:bit 範例 4：循線小車（三段式）
// 做什麼：按 A 開車：線在左 → 左轉、線在右 → 右轉、正中間 → 直走；丟線 → 停。按 B 停車。
// 接線：感應器 SDA → P20、SCL → P19、VCC → 3V、GND → GND。左輪連續旋轉伺服訊號 → P1、右輪 → P2；兩顆伺服的電源接 4.8~6 V 電池盒，電池負極接 micro:bit GND。
// 準備：兩輪小車（兩顆連續旋轉伺服當輪子）、感應器裝在車頭離地 0.5~1 cm、黑線跑道。
// 看到什麼：按 A 後箭頭朝上、車子沿線走；轉彎時箭頭指向轉彎方向；拿開線 → 哭臉、停車。

let running = false

// 左輪伺服 P1、右輪伺服 P2（連續旋轉伺服）；left／right = −100~100，正 = 前進
function drive(left: number, right: number): void {
    left = Math.constrain(left, -100, 100)
    right = Math.constrain(right, -100, 100)
    pins.servoWritePin(AnalogPin.P1, Math.round(90 + left * 0.9))    // 90 = 停、180 = 全速前進、0 = 全速後退
    pins.servoWritePin(AnalogPin.P2, Math.round(90 - right * 0.9))   // 右輪裝的方向相反，所以用減的
}

input.onButtonPressed(Button.A, function () {
    running = true
    basic.showArrow(ArrowNames.North, 0)
})
input.onButtonPressed(Button.B, function () {
    running = false
    drive(0, 0)
    basic.showIcon(IconNames.No, 0)
})

basic.forever(function () {
    if (!running) {
        basic.pause(100)
        return
    }
    if (!mbc.lineSeen()) {
        drive(0, 0)                                       // 丟線就停，才不會跑出跑道
        basic.showIcon(IconNames.Sad, 0)
    } else {
        let pos = mbc.linePosition()                      // 負 = 線在左
        if (pos < 0) {
            drive(20, 60)                                 // 左輪慢、右輪快 = 左轉
            basic.showArrow(ArrowNames.West, 0)
        } else if (pos > 0) {
            drive(60, 20)                                 // 右轉
            basic.showArrow(ArrowNames.East, 0)
        } else {
            drive(50, 50)                                 // 直走
            basic.showArrow(ArrowNames.North, 0)
        }
    }
    basic.pause(20)
})
```

**一步一步看懂程式**
1. `function drive(left, right)`：這一頁所有開車範例都用它。數字 −100～100，正 = 前進。左輪 `90 + left × 0.9`（90 停、180 全速前）；右輪裝的方向相反，所以 `90 − right × 0.9`。
2. 按 A：`running = true`；按 B：`running = false` 並 `drive(0, 0)` 停車。
3. `basic.forever`：沒在跑就什麼都不做（`return`）。
4. `!mbc.lineSeen()`：丟線 → 停車、哭臉，車子才不會衝出跑道。
5. `pos < 0`（線在左）→ `drive(20, 60)`：左輪慢、右輪快 = 左轉。`pos > 0` 相反。`pos == 0` → 兩輪一樣快直走。

**跑起來應該看到什麼**
- 按 A：箭頭朝上、車子沿線走；彎道時箭頭指向轉彎方向。
- 把車抬起來：哭臉、兩輪停。
- 按 B：X、停車。

**常見問題**
- **按 A 車子倒退？** → 兩顆伺服都裝反了：把 `drive` 裡的 `+ left` 改 `- left`、`- right` 改 `+ right`。只有一輪反 → 只改那一行。
- **車子在線上左右甩？** → 把 60／20 改成 50／30（轉彎溫和一點），或進階用範例 5 的比例控制。
- **停不下來、放開還慢慢轉？** → 連續旋轉伺服的「停」不一定剛好是 90，用小螺絲起子調伺服背面的微調孔，或把 90 改成 88～92 試。

---

## 範例 5：平滑循線＋數路口停車（綜合）

**這個範例會做什麼**：用「細位置」做比例控制：線偏多少就修正多少，走起來不會左右甩；遇到十字路口（分岔事件）就計數，第 3 個路口停車；丟線時用「最後離線方向」原地轉回去找線。把前面學的讀值、事件、開車全部放在一起。

**要準備什麼**
- 範例 4 的小車
- 畫了 3 個十字路口的黑線跑道

**怎麼接**：同範例 4。

**下載程式**：[ex05_smooth_follow_stop.ts](/examples/line16/microbit/ex05_smooth_follow_stop.ts)（右鍵另存）→ MakeCode 新專案 → 齒輪 → 擴充 → 貼 `https://github.com/contactmbcrobot/pxt-mbc-sensors` → 右上切到 **JavaScript** → 全選刪掉原本的內容、把檔案內容貼上 → **下載**，把 .hex 拖進 MICROBIT 磁碟。

**程式**

```typescript
// 循行者 16 路 × micro:bit 範例 5：平滑循線＋數路口停車（綜合）
// 做什麼：用「細位置」做比例控制，線越偏修正越多，走起來不抖；遇到十字路口就計數，第 3 個路口停車；丟線時用「最後離線方向」原地轉回去找線。A 開始、B 停。
// 接線：同範例 4（感應器 P19／P20、左輪 P1、右輪 P2、伺服外接電池、共地）。
// 準備：畫了 3 個十字路口的黑線跑道。
// 看到什麼：按 A 車子平順沿線走，每過一個路口 LED 顯示 1、2、3，到 3 停車打勾；把線拿開，車子會往線消失的那一邊轉，找到線再繼續。

const SPEED = 50
const KP = 0.5                                            // 細位置 ±100 → 修正 ±50
let running = false
let crossings = 0

// 左輪伺服 P1、右輪伺服 P2（連續旋轉伺服）；left／right = −100~100，正 = 前進
function drive(left: number, right: number): void {
    left = Math.constrain(left, -100, 100)
    right = Math.constrain(right, -100, 100)
    pins.servoWritePin(AnalogPin.P1, Math.round(90 + left * 0.9))    // 90 = 停、180 = 全速前進、0 = 全速後退
    pins.servoWritePin(AnalogPin.P2, Math.round(90 - right * 0.9))   // 右輪裝的方向相反，所以用減的
}

mbc.onLineEvent(MbcLineEvent.Fork, function () {          // 線段數 ≥ 2：十字或分岔
    if (!running) {
        return
    }
    crossings += 1
    basic.showNumber(crossings)
    if (crossings >= 3) {
        running = false
        drive(0, 0)
        basic.showIcon(IconNames.Yes)
    }
})
mbc.onLineEvent(MbcLineEvent.Lost, function () {
    if (!running) {
        return
    }
    if (mbc.lineLastDirection() < 0) {                    // 線從左邊跑掉 → 原地左轉找線
        drive(-30, 30)
    } else {
        drive(30, -30)
    }
    basic.showIcon(IconNames.Confused, 0)
})

input.onButtonPressed(Button.A, function () {
    crossings = 0
    running = true
    basic.showArrow(ArrowNames.North, 0)
})
input.onButtonPressed(Button.B, function () {
    running = false
    drive(0, 0)
    basic.showIcon(IconNames.No, 0)
})

basic.forever(function () {
    if (running && mbc.lineSeen()) {
        let err = mbc.linePositionFine()                  // −100~+100，負 = 線在左
        let turn = err * KP                               // 線在左 → turn 是負的 → 左輪減速 = 左轉
        drive(SPEED + turn, SPEED - turn)
    }
    basic.pause(20)
})
```

**一步一步看懂程式**
1. `KP = 0.5`：細位置 −100～+100 乘 0.5 = 修正 −50～+50。這個數字越大轉得越兇。
2. `basic.forever`：在跑而且看得到線時，`err = 細位置`，`turn = err × KP`，`drive(SPEED + turn, SPEED − turn)`。線在左（負）→ 左輪減速、右輪加速 = 平滑左轉。
3. `onLineEvent(Fork)`：線段數 ≥ 2（十字）時 `crossings + 1` 顯示在 LED；到 3 就 `running = false` 停車打勾。
4. `onLineEvent(Lost)`：`mbc.lineLastDirection() < 0` 表示線從左邊跑掉 → `drive(−30, 30)` 原地左轉；否則右轉。forever 迴圈在看不到線時不會動馬達，所以會一直轉到找回線為止。
5. A：次數歸零、開始；B：停。

**跑起來應該看到什麼**
- 按 A：車子平順沿線走，比範例 4 穩。
- 每過一個十字：LED 顯示 1、2、3；到 3 停車打勾。
- 把跑道從車底抽走：問號臉、車子往線消失的那一側原地轉；放回去就繼續走。

**常見問題**
- **一個路口算了兩次？** → 車速太慢、經過路口時線段數跳了兩次。加快 `SPEED`，或在 Fork 事件裡加 `basic.pause(500)` 讓同一個路口不重複計。
- **車子還是會左右抖？** → `KP` 從 0.5 降到 0.3；還抖就把 `basic.pause(20)` 改 10（更新快一點）。
- **找線時一直原地轉？** → 線拿太遠了。它只會轉，不會前進；要往前找線可以在 Lost 事件改成 `drive(10, 40)` 這種弧線。

---

## 下一步

- 全部積木、數值和硬體說明：[循行者 16 路 × micro:bit](/sensors/line16/microbit.md)
- 每個數值編號的意思：[指令參考](/sensors/line16/reference.md)
- 擴充原始碼與回報問題：<https://github.com/contactmbcrobot/pxt-mbc-sensors>
