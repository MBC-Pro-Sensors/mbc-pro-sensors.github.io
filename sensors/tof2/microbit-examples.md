<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 W4 micro:bit 範例產生器產生（scratchpad w4/gen_pages.mjs＋spec/tof2.mjs）；程式碼區塊來自 examples/tof2/microbit/*.ts，改程式請改 .ts 後重跑產生器 -->
# 🟧 micro:bit 範例：測距者 2 路

[🔙 回到測距者 2 路 micro:bit 頁](/sensors/tof2/microbit.md)　｜　[📖 產品頁](/sensors/tof2/index.md)　｜　[📋 指令參考](/sensors/tof2/reference.md)

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
| 供電 | 兩顆雷射約 40 mA，可以先試 micro:bit 的 3V 腳（V1 約 90 mA、V2 約 190 mA 上限）；讀值不穩就外接 5 V | 上機待測 |
| I2C 上拉 | micro:bit 板上已有 4.7 kΩ；產品板若也有上拉，並聯後要確認低電位 ≤ 0.3×VDD | 上機待測 |
| 時脈延展 | STM32 從機預設會時脈延展，V2（nRF52833）對延展有已知問題，要 V1、V2 各測一次 | 上機待測 |
| 位址衝突 | 出廠位址 0x29，和測距者 8 路一樣；兩個測距者同時接要先用「裝置」分類的「改成位址」積木換掉一個 | — |
| 就緒時間 | 兩顆雷射都初始化完成才就緒（F3 b0），約 1 秒；壞掉的那顆距離會回 120 cm | 上機待測 |

讀不到值時先看：接線（SDA＝P20、SCL＝P19）→ 外接電源有沒有開 → 擴充「裝置」分類的「位址 … 的裝置就緒？」積木是不是 true。

### 接線（所有範例共用）

<figure class="ref-fig"><img src="/images/figures/tof2/wiring-microbit.svg" alt="測距者 2 路 × micro:bit" loading="lazy"></figure>

| 感應器 | micro:bit |
| :--- | :--- |
| SDA | P20 |
| SCL | P19 |
| VCC | 3V（或外接 5 V） |
| GND | GND（伺服電池負極也接這裡） |

## 這頁會用到的數值

距離單位是**公分**（0～120）；**120 = 前面沒東西**（或那顆壞了）。「左、右」是站在車後往前看。

| 積木（繁中） | JavaScript | 範圍 | 意思 | 用在 |
| :--- | :--- | :--- | :--- | :--- |
| 測距者 2 路 較近距離（公分） | `mbc.tof2NearestCm()` | 0～120 | 左右兩顆取較近的 | 1, 2, 3 |
| 測距者 2 路 左／右 距離（公分／公釐） | `mbc.tof2Distance(MbcTofSide.Left, MbcDistUnit.Cm)` | 0～120 / 0～1200 | 單獨一顆的距離 | 1, 2, 4, 5 |
| 當測距者 2 路 距離小於 … 公分 | `mbc.onTof2Near(10, ...)` | 事件 | 較近距離第一次小於門檻時觸發一次，離開再靠近才會再觸發 | 2, 5 |
| 測距者 2 路 平均距離（公釐） | `mbc.tof2AverageMm()` | 0～1200 | (左 + 右) ÷ 2，單位 mm（本頁沒用到，要更細的數字可以換這個） | — |
| 等位址 … 的裝置就緒 | `mbc.waitReady(mbc.ADDR_TOF, 3000)` | true / false | 最多等 3 秒，兩顆雷射都好了才 true（進階 → 裝置） | 1 |

---

## 範例 1：前面多遠

**這個範例會做什麼**：LED 長條顯示前面的東西有多近：越近亮越多格。按 A 顯示左邊那顆的公分數、按 B 顯示右邊那顆。用來確認接線，也能看兩顆雷射是不是都正常。

**要準備什麼**
- micro:bit V1 或 V2＋USB 線
- 測距者 2 路（通用 I2C 版）＋4P 線
- 一本書或一塊板子當目標

**怎麼接**：照上面的接線表：SDA → P20、SCL → P19、VCC → 3V、GND → GND。

**下載程式**：[ex01_show_distance.ts](/examples/tof2/microbit/ex01_show_distance.ts)（右鍵另存）→ MakeCode 新專案 → 齒輪 → 擴充 → 貼 `https://github.com/contactmbcrobot/pxt-mbc-sensors` → 右上切到 **JavaScript** → 全選刪掉原本的內容、把檔案內容貼上 → **下載**，把 .hex 拖進 MICROBIT 磁碟。

**程式**

```typescript
// 測距者 2 路 × micro:bit 範例 1：前面多遠（LED 長條＋數字）
// 做什麼：手越靠近感應器，LED 亮的格子越多；按 A 顯示左邊那顆的公分數、按 B 顯示右邊那顆。
// 接線：感應器 SDA → P20、SCL → P19、VCC → 3V（或外接 5 V）、GND → GND。
// 準備：MakeCode 新專案 → 擴充 → 貼 https://github.com/contactmbcrobot/pxt-mbc-sensors → 切到 JavaScript 把本檔全部貼上 → 下載到 micro:bit。
// 看到什麼：開機先打勾；前面沒東西 → 全暗；手放 60 cm → 亮一半；貼到 10 cm 內 → 幾乎全亮。

let busy = false

if (mbc.waitReady(mbc.ADDR_TOF, 3000)) {                  // 兩顆雷射都開好了才算就緒
    basic.showIcon(IconNames.Yes)
} else {
    basic.showIcon(IconNames.No)                          // 沒回應：檢查 P19／P20 和電源
}

basic.forever(function () {
    if (busy) {
        basic.pause(100)
        return
    }
    let d = mbc.tof2NearestCm()                           // 左右兩顆取較近的，0~120 cm，120 = 前面沒東西
    led.plotBarGraph(120 - d, 120)                        // 越近長條越長
    basic.pause(100)
})

input.onButtonPressed(Button.A, function () {
    busy = true
    basic.showNumber(mbc.tof2Distance(MbcTofSide.Left, MbcDistUnit.Cm))    // 左邊那顆（站在車後往前看）
    busy = false
})
input.onButtonPressed(Button.B, function () {
    busy = true
    basic.showNumber(mbc.tof2Distance(MbcTofSide.Right, MbcDistUnit.Cm))   // 右邊那顆
    busy = false
})
```

**一步一步看懂程式**
1. `mbc.waitReady(mbc.ADDR_TOF, 3000)`：等兩顆雷射都初始化好，打勾；3 秒沒回應打 X。
2. `basic.forever`：每 0.1 秒讀一次 `mbc.tof2NearestCm()`（左右取較近，0～120）。
3. `led.plotBarGraph(120 - d, 120)`：長條圖積木。`120 − d` 讓越近的數字越大、亮越多格。
4. 按 A／B：`mbc.tof2Distance(MbcTofSide.Left, MbcDistUnit.Cm)` 讀單獨一顆，`basic.showNumber` 捲過螢幕；`busy` 讓長條先暫停。

**跑起來應該看到什麼**
- 開機：打勾。
- 前面沒東西：全暗（120）。
- 板子放 60 cm：亮一半；慢慢靠近到 10 cm：幾乎全亮。
- 按 A 和 B：兩個數字應該差不多（板子正對著時）。

**常見問題**
- **一直 120 不變？** → 目標太遠（超過 120 cm）、或太小／太黑（黑色絨布反射不回來）。換一本白色的書試。
- **A 和 B 其中一個一直是 120？** → 那顆雷射沒初始化或故障。用進階「裝置」積木讀狀態 bit1（任一顆失效）確認，重新上電再試。
- **開機就是 X（找不到感應器）？** → 看接線 SDA＝P20、SCL＝P19 有沒有接反；外接電源有沒有開；用鱷魚夾接的話夾子有沒有碰到旁邊的腳。
- **模擬器裡數字都是 0？** → 正常。MakeCode 模擬器沒有真的 I2C，讀到的都是 0；要看真的數值一定要下載到 micro:bit。
- **V1 沒有聲音？** → V1 沒有喇叭，在 P0 和 GND 之間接蜂鳴器或耳機。

---

## 範例 2：倒車雷達

**這個範例會做什麼**：40 cm 內開始嗶，越近嗶得越急；進到 10 cm 內畫一個 X（用事件積木，每次靠近只畫一次）。按 A 看東西偏左還是偏右。

**要準備什麼**
- 範例 1 的東西
- V1 另外要蜂鳴器或耳機接 P0

**怎麼接**：同範例 1。

**下載程式**：[ex02_parking_alarm.ts](/examples/tof2/microbit/ex02_parking_alarm.ts)（右鍵另存）→ MakeCode 新專案 → 齒輪 → 擴充 → 貼 `https://github.com/contactmbcrobot/pxt-mbc-sensors` → 右上切到 **JavaScript** → 全選刪掉原本的內容、把檔案內容貼上 → **下載**，把 .hex 拖進 MICROBIT 磁碟。

**程式**

```typescript
// 測距者 2 路 × micro:bit 範例 2：倒車雷達（越近嗶越急）
// 做什麼：40 cm 內開始嗶，越近嗶得越急；進到 10 cm 內畫一個 X。按 A 看東西偏左還是偏右。
// 接線：感應器 SDA → P20、SCL → P19、VCC → 3V、GND → GND。V1 要在 P0 接蜂鳴器或耳機才有聲音（V2 有內建喇叭）。
// 準備：擴充與貼程式的方法同範例 1。
// 看到什麼：手慢慢靠近 → 嗶…嗶…嗶嗶嗶；10 cm 內 → X；按 A → 箭頭指向比較近的那一邊。

mbc.onTof2Near(10, function () {                          // 較近距離第一次小於 10 cm 時觸發（每次靠近只觸發一次）
    basic.showIcon(IconNames.No, 0)
})

basic.forever(function () {
    let d = mbc.tof2NearestCm()                           // 0~120 cm
    if (d < 40) {
        led.plotBarGraph(40 - d, 40)
        music.playTone(Note.A, 100)                       // 嗶 0.1 秒
        basic.pause(d * 10)                               // 30 cm 等 0.3 秒、10 cm 等 0.1 秒：越近越急
    } else {
        basic.showIcon(IconNames.Happy, 0)                // 前面很空
        basic.pause(100)
    }
})

input.onButtonPressed(Button.A, function () {
    let l = mbc.tof2Distance(MbcTofSide.Left, MbcDistUnit.Cm)
    let r = mbc.tof2Distance(MbcTofSide.Right, MbcDistUnit.Cm)
    if (l < r) {
        basic.showArrow(ArrowNames.West)                  // 左邊那顆比較近
    } else if (r < l) {
        basic.showArrow(ArrowNames.East)
    } else {
        basic.showArrow(ArrowNames.North)                 // 一樣近：正前方
    }
})
```

**一步一步看懂程式**
1. `mbc.onTof2Near(10, ...)`：事件積木。較近距離第一次小於 10 cm 時執行一次 → 畫 X。東西離開再靠近才會再觸發。
2. `basic.forever`：讀 `d = mbc.tof2NearestCm()`。
3. `d < 40`：先 `led.plotBarGraph` 畫長條，`music.playTone(Note.A, 100)` 嗶 0.1 秒，再 `basic.pause(d * 10)`：30 cm 等 0.3 秒、10 cm 等 0.1 秒，所以越近越急。
4. `d >= 40`：笑臉、安靜。
5. 按 A：比較左右兩顆，哪邊比較近就畫哪邊的箭頭；一樣近畫向上箭頭。

**跑起來應該看到什麼**
- 手從遠處慢慢靠近：笑臉 → 開始嗶 → 越來越急。
- 進到 10 cm：X 閃一下（然後長條繼續畫）。
- 手偏左放、按 A：左箭頭。

**常見問題**
- **X 只出現一下就不見？** → 正常，forever 迴圈下一輪就把長條畫回去了。想留久一點在事件裡加 `basic.pause(500)`。
- **嗶聲斷斷續續不好聽？** → 把 `Note.A` 改成 `Note.C5`、100 改 50 試試；或改成只畫長條不出聲。

---

## 範例 3：自動柵欄（伺服）

**這個範例會做什麼**：有東西進到 15 cm 內 → 柵欄打開（伺服轉到 90°）；東西離開 2 秒後自動關上（0°）。按 A／B 把距離門檻加 5／減 5。學會「只有狀態改變時才動馬達」。

**要準備什麼**
- 範例 1 的東西
- 9g 伺服（SG90）×1＋冰棒棍當柵欄
- 4.8～6 V 電池盒＋杜邦線

**怎麼接**：感應器同範例 1。伺服 **橘（訊號）→ P1**、紅 → 外接 4.8～6 V 電池正極、棕 → 電池負極，電池負極再接一條到 micro:bit GND（共地）。

**下載程式**：[ex03_servo_gate.ts](/examples/tof2/microbit/ex03_servo_gate.ts)（右鍵另存）→ MakeCode 新專案 → 齒輪 → 擴充 → 貼 `https://github.com/contactmbcrobot/pxt-mbc-sensors` → 右上切到 **JavaScript** → 全選刪掉原本的內容、把檔案內容貼上 → **下載**，把 .hex 拖進 MICROBIT 磁碟。

**程式**

```typescript
// 測距者 2 路 × micro:bit 範例 3：自動柵欄（伺服）
// 做什麼：有東西進到 15 cm 內 → 柵欄打開（伺服轉到 90°）；離開 2 秒後關上（0°）。按 A／B 把距離門檻 +5／−5。
// 接線：感應器 SDA → P20、SCL → P19、VCC → 3V、GND → GND。伺服 橘（訊號）→ P1、紅 → 外接 4.8~6 V 電池正極、棕 → 電池負極並接 micro:bit GND。
// 準備：9g 伺服（SG90）＋冰棒棍當柵欄。
// 看到什麼：手伸到感應器前 → 柵欄抬起、打勾；手拿開 2 秒 → 柵欄放下、X。

let limit = 15                                            // 幾公分以內算「有東西」
let lastSeen = -10000                                     // 上次看到東西的時間（毫秒）
let open = false

pins.servoWritePin(AnalogPin.P1, 0)                       // 開機先關上
basic.showIcon(IconNames.No)

basic.forever(function () {
    let d = mbc.tof2NearestCm()
    if (d > 0 && d < limit) {
        lastSeen = control.millis()                       // 記下現在的時間
    }
    let wantOpen = control.millis() - lastSeen < 2000     // 2 秒內看過東西就要開著
    if (wantOpen != open) {                               // 狀態變了才動伺服
        open = wantOpen
        pins.servoWritePin(AnalogPin.P1, open ? 90 : 0)
        basic.showIcon(open ? IconNames.Yes : IconNames.No, 0)
    }
    basic.pause(50)
})

input.onButtonPressed(Button.A, function () {
    limit = Math.constrain(limit + 5, 5, 100)
    basic.showNumber(limit)
})
input.onButtonPressed(Button.B, function () {
    limit = Math.constrain(limit - 5, 5, 100)
    basic.showNumber(limit)
})
```

**一步一步看懂程式**
1. `limit = 15`：門檻；`lastSeen`：上次看到東西的時間；`open`：柵欄現在開著嗎。
2. 開機 `pins.servoWritePin(AnalogPin.P1, 0)` 先關上。
3. `forever`：`d` 小於門檻就 `lastSeen = control.millis()`（記下現在幾毫秒）。
4. `wantOpen = 現在 − lastSeen < 2000`：2 秒內看過東西就「應該開著」。
5. `wantOpen != open`：只有「應該的狀態」和「現在的狀態」不一樣時才轉伺服、換圖案；伺服不會一直抖。
6. A／B：`limit ± 5`，用 `Math.constrain` 限制在 5～100，顯示新門檻。

**跑起來應該看到什麼**
- 開機：柵欄在 0°、X。
- 手伸到感應器前 15 cm 內：柵欄抬到 90°、打勾。
- 手拿開：2 秒後放下、X。
- 按 A：顯示 20，現在 20 cm 內就會開。

**常見問題**
- **柵欄一直開開關關？** → 東西剛好在門檻邊緣。把門檻加大，或把 2000 改 3000 讓它開久一點。
- **伺服抖、micro:bit 重開？** → 伺服一定要外接電池、共地，不能從 3V 腳供電。

---

## 範例 4：跟著我走的小車

**這個範例會做什麼**：車子跟前面的東西保持 20 cm：太遠就前進、太近就後退，東西偏左（右邊那顆比較遠）就左轉。按 A 開始、B 停。兩顆雷射的差就是方向。

**要準備什麼**
- 兩輪小車：兩顆連續旋轉伺服（FS90R 之類）當輪子＋一顆萬向輪
- 測距者裝在車頭朝前
- 4.8～6 V 電池盒
- 一塊 A4 大小的板子當「人」

**怎麼接**：感應器同範例 1，裝在車頭朝前。**左輪連續旋轉伺服訊號 → P1、右輪 → P2**，兩顆伺服的紅線接 4.8～6 V 電池盒正極、棕線接電池負極，電池負極再接 micro:bit GND。

**下載程式**：[ex04_follow_me.ts](/examples/tof2/microbit/ex04_follow_me.ts)（右鍵另存）→ MakeCode 新專案 → 齒輪 → 擴充 → 貼 `https://github.com/contactmbcrobot/pxt-mbc-sensors` → 右上切到 **JavaScript** → 全選刪掉原本的內容、把檔案內容貼上 → **下載**，把 .hex 拖進 MICROBIT 磁碟。

**程式**

```typescript
// 測距者 2 路 × micro:bit 範例 4：跟著我走的小車
// 做什麼：車子跟前面的東西（你的手或一塊板子）保持 20 cm：太遠就前進、太近就後退，東西偏左就左轉。按 A 開始、B 停。
// 接線：感應器 SDA → P20、SCL → P19、VCC → 3V、GND → GND。左輪連續旋轉伺服訊號 → P1、右輪 → P2；伺服電源 4.8~6 V 電池盒，電池負極接 micro:bit GND。
// 準備：兩輪小車，感應器裝在車頭朝前。
// 看到什麼：按 A 後把手放在車前 20 cm：手往後退車就跟上來，手靠近車就倒退；手拿開（超過 60 cm）車子停下、顯示問號臉。

const TARGET = 20                                         // 想保持的距離（cm）
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
    basic.showIcon(IconNames.Happy, 0)
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
    let l = mbc.tof2Distance(MbcTofSide.Left, MbcDistUnit.Cm)
    let r = mbc.tof2Distance(MbcTofSide.Right, MbcDistUnit.Cm)
    let d = Math.min(l, r)                                // 較近的那顆
    if (d >= 60) {
        drive(0, 0)                                       // 太遠：東西不見了，停
        basic.showIcon(IconNames.Confused, 0)
    } else {
        let speed = Math.constrain((d - TARGET) * 4, -60, 60)   // 比 20 cm 遠 → 正 = 前進；近 → 負 = 後退
        let turn = Math.constrain((r - l) * 2, -30, 30)         // 右邊那顆比較遠 → 東西偏左 → 左轉
        drive(speed - turn, speed + turn)
        if (turn < -5) {
            basic.showArrow(ArrowNames.East, 0)
        } else if (turn > 5) {
            basic.showArrow(ArrowNames.West, 0)
        } else {
            basic.showArrow(ArrowNames.North, 0)
        }
    }
    basic.pause(30)
})
```

**一步一步看懂程式**
1. `function drive(left, right)`：左輪 P1、右輪 P2，−100～100，正 = 前進；右輪裝反所以用減的。
2. A：`running = true`；B：停車。
3. `forever`：讀左 `l`、右 `r`，`d = Math.min(l, r)`。
4. `d >= 60`：東西不見了 → 停、問號臉。
5. `speed = (d − 20) × 4`：離 30 cm → +40 前進；離 10 cm → −40 後退；用 `Math.constrain` 限制在 ±60。
6. `turn = (r − l) × 2`：右邊比較遠 → 東西偏左 → turn 正 → `drive(speed − turn, speed + turn)` 左輪慢 = 左轉。箭頭顯示轉向。

**跑起來應該看到什麼**
- 按 A，板子放車前 20 cm：車子不動、箭頭朝上。
- 板子往後退：車子跟上來；板子靠近：車子倒退。
- 板子往左移：車頭跟著轉向左。
- 板子拿走：停、問號臉。

**常見問題**
- **車子前後抖個不停？** → `× 4` 改 `× 3`，或把 `basic.pause(30)` 改 60。
- **轉向反了？** → 感應器裝反（左右顆對調）：把 `(r − l)` 改 `(l − r)`。
- **按 A 車子後退？** → 伺服裝反：改 `drive` 裡的正負號（見循行者範例 4 的說明）。

---

## 範例 5：避障小車（綜合）

**這個範例會做什麼**：車子一直往前走；15 cm 內有東西就倒退一下、往比較空的那一邊原地轉，再繼續走；LED 顯示躲了幾次，進到 15 cm 內的瞬間閃骷髏（事件）。A 開始、B 停。

**要準備什麼**
- 範例 4 的小車
- 幾個箱子當障礙物

**怎麼接**：同範例 4。

**下載程式**：[ex05_obstacle_avoid.ts](/examples/tof2/microbit/ex05_obstacle_avoid.ts)（右鍵另存）→ MakeCode 新專案 → 齒輪 → 擴充 → 貼 `https://github.com/contactmbcrobot/pxt-mbc-sensors` → 右上切到 **JavaScript** → 全選刪掉原本的內容、把檔案內容貼上 → **下載**，把 .hex 拖進 MICROBIT 磁碟。

**程式**

```typescript
// 測距者 2 路 × micro:bit 範例 5：避障小車（綜合）
// 做什麼：車子一直往前走；15 cm 內有東西 → 倒退一下，往比較空的那一邊原地轉，再繼續走；LED 顯示躲了幾次。A 開始、B 停。
// 接線：同範例 4（感應器 P19／P20、左輪 P1、右輪 P2、伺服外接電池、共地）。
// 準備：兩輪小車＋幾個箱子當障礙物。
// 看到什麼：按 A 車子前進，碰到障礙物前先倒退、轉彎、繼續；每躲一次 LED 數字 +1；進到 15 cm 內瞬間會閃一個骷髏。

const STOP_CM = 15
let running = false
let detours = 0

// 左輪伺服 P1、右輪伺服 P2（連續旋轉伺服）；left／right = −100~100，正 = 前進
function drive(left: number, right: number): void {
    left = Math.constrain(left, -100, 100)
    right = Math.constrain(right, -100, 100)
    pins.servoWritePin(AnalogPin.P1, Math.round(90 + left * 0.9))    // 90 = 停、180 = 全速前進、0 = 全速後退
    pins.servoWritePin(AnalogPin.P2, Math.round(90 - right * 0.9))   // 右輪裝的方向相反，所以用減的
}

mbc.onTof2Near(STOP_CM, function () {                     // 事件：每次進到 15 cm 內閃一下
    basic.showIcon(IconNames.Skull, 0)
})

input.onButtonPressed(Button.A, function () {
    detours = 0
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
    let l = mbc.tof2Distance(MbcTofSide.Left, MbcDistUnit.Cm)
    let r = mbc.tof2Distance(MbcTofSide.Right, MbcDistUnit.Cm)
    if (Math.min(l, r) < STOP_CM) {
        detours += 1
        drive(-50, -50)                                   // 先倒退 0.4 秒
        basic.pause(400)
        if (l > r) {
            drive(-40, 40)                                // 左邊比較空 → 原地左轉
        } else {
            drive(40, -40)                                // 原地右轉
        }
        basic.pause(500)
        basic.showNumber(detours)
    } else {
        drive(50, 50)                                     // 前面沒東西：直走
        basic.showArrow(ArrowNames.North, 0)
    }
    basic.pause(30)
})
```

**一步一步看懂程式**
1. `STOP_CM = 15`：幾公分以內算障礙。
2. `mbc.onTof2Near(STOP_CM, ...)`：事件，每次靠近閃一次骷髏（和主迴圈分開跑）。
3. `forever`：讀 `l`、`r`；`Math.min(l, r) < STOP_CM` 就進避障。
4. 避障：`detours + 1`、`drive(−50, −50)` 倒退 0.4 秒；`l > r`（左邊比較空）→ `drive(−40, 40)` 原地左轉 0.5 秒，否則右轉；顯示次數。
5. 沒障礙：`drive(50, 50)` 直走、箭頭朝上。

**跑起來應該看到什麼**
- 按 A：車子前進；靠近箱子 → 骷髏 → 倒退 → 轉彎 → 顯示 1 → 繼續。
- 箱子在左前方：往右轉；在右前方：往左轉。
- 按 B：停。

**常見問題**
- **轉不夠、還是撞到？** → `basic.pause(500)` 改 700 轉久一點，或 `STOP_CM` 改 20 早一點反應。
- **在角落一直來回？** → 兩邊都不空。把倒退時間 400 改 800，退遠一點再轉。

---

## 下一步

- 全部積木、數值和硬體說明：[測距者 2 路 × micro:bit](/sensors/tof2/microbit.md)
- 每個數值編號的意思：[指令參考](/sensors/tof2/reference.md)
- 擴充原始碼與回報問題：<https://github.com/contactmbcrobot/pxt-mbc-sensors>
