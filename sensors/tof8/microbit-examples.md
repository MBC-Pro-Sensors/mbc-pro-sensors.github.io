<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 W4 micro:bit 範例產生器產生（scratchpad w4/gen_pages.mjs＋spec/tof8.mjs）；程式碼區塊來自 examples/tof8/microbit/*.ts，改程式請改 .ts 後重跑產生器 -->
# 🟧 micro:bit 範例：測距者 8 路

[🔙 回到測距者 8 路 micro:bit 頁](/sensors/tof8/microbit.md)　｜　[📖 產品頁](/sensors/tof8/index.md)　｜　[📋 指令參考](/sensors/tof8/reference.md)

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
| 供電 | 8 顆雷射超過 100 mA，**一定要外接 5 V**，不能從 micro:bit 3V 腳取電（V1 上限約 90 mA） | 上機待測 |
| I2C 上拉 | micro:bit 板上已有 4.7 kΩ；產品板若也有上拉，並聯後要確認低電位 ≤ 0.3×VDD | 上機待測 |
| 時脈延展 | STM32 從機預設會時脈延展，V2（nRF52833）對延展有已知問題，要 V1、V2 各測一次 | 上機待測 |
| 位址衝突 | 出廠位址 0x29，和測距者 2 路一樣；兩個測距者同時接要先用「裝置」分類的「改成位址」積木換掉一個 | — |
| 就緒時間 | 8 顆雷射開機初始化約 2～4 秒，之後 F3 b0 = 1；範例 1 用「等裝置就緒」等 5 秒 | 上機待測 |

讀不到值時先看：接線（SDA＝P20、SCL＝P19）→ 外接電源有沒有開 → 擴充「裝置」分類的「位址 … 的裝置就緒？」積木是不是 true。

### 接線（所有範例共用）

<figure class="ref-fig"><img src="/images/figures/tof8/wiring-microbit.svg" alt="測距者 8 路 × micro:bit" loading="lazy"></figure>

| 感應器 | micro:bit |
| :--- | :--- |
| SDA | P20 |
| SCL | P19 |
| VCC | 外接 5 V（必要） |
| GND | GND（伺服電池負極也接這裡） |

## 這頁會用到的數值

8 顆雷射每顆隔 30°：**S1 正左、S4／S5 正前、S8 正右**（站在車後往前看）。距離 0～120 cm，**120 = 那個方向沒東西**。方向 −7～+7：0 = 正前方、負 = 左。

| 積木（繁中） | JavaScript | 範圍 | 意思 | 用在 |
| :--- | :--- | :--- | :--- | :--- |
| 測距者 8 路 最近距離（公分／公釐） | `mbc.tof8Nearest(MbcDistUnit.Cm)` | 0～120 | 8 顆裡最小的距離 | 1, 2, 3, 5 |
| 測距者 8 路 最近方向 | `mbc.tof8NearestDirection()` | −7～+7（±8 = 沒東西） | 最近的東西在哪個方向 | 1, 2, 3 |
| 測距者 8 路 最空曠方向 | `mbc.tof8OpenDirection()` | −7～+7 | 最遠（最空）的那個方向 | 5 |
| 測距者 8 路 正前方距離（公分） | `mbc.tof8Front(MbcDistUnit.Cm)` | 0～120 | S4、S5 兩顆平均 | 4 |
| 測距者 8 路 第 n 顆距離（公分） | `mbc.tof8Sensor(1, MbcDistUnit.Cm)` | 0～120 | 單獨一顆，1 = 正左 … 8 = 正右 | 3, 4 |
| 當測距者 8 路 距離小於 … 公分 | `mbc.onTof8Near(12, ...)` | 事件 | 最近距離第一次小於門檻時觸發一次 | 5 |
| 等位址 … 的裝置就緒 | `mbc.waitReady(mbc.ADDR_TOF, 5000)` | true / false | 最多等 5 秒，8 顆都好了才 true（進階 → 裝置） | 1 |

---

## 範例 1：雷達畫面

**這個範例會做什麼**：LED 上一根光柱：左右位置 = 最近的東西在哪個方向，高度 = 多近（越近越高）。按 A 顯示最近距離（cm）、按 B 顯示方向（−7～+7）。

**要準備什麼**
- micro:bit V1 或 V2＋USB 線
- 測距者 8 路（通用 I2C 版）＋4P 線
- 外接 5 V 電源（行動電源或電池盒）
- 一本書當目標

**怎麼接**：照上面的接線表：SDA → P20、SCL → P19、VCC → 外接 5 V、GND → GND（外接電源的負極也要接 micro:bit GND）。

**下載程式**：[ex01_radar.ts](/examples/tof8/microbit/ex01_radar.ts)（右鍵另存）→ MakeCode 新專案 → 齒輪 → 擴充 → 貼 `https://github.com/contactmbcrobot/pxt-mbc-sensors` → 右上切到 **JavaScript** → 全選刪掉原本的內容、把檔案內容貼上 → **下載**，把 .hex 拖進 MICROBIT 磁碟。

**程式**

```typescript
// 測距者 8 路 × micro:bit 範例 1：雷達畫面（最近的東西在哪個方向、多近）
// 做什麼：LED 上一根光柱：左右位置 = 最近的東西在哪個方向，高度 = 多近（越近越高）。按 A 顯示最近距離（cm）、按 B 顯示方向（−7~+7）。
// 接線：感應器 SDA → P20、SCL → P19、VCC → 外接 5 V（8 顆雷射耗電大，不要從 micro:bit 3V 取電）、GND → GND 共地。
// 準備：MakeCode 新專案 → 擴充 → 貼 https://github.com/contactmbcrobot/pxt-mbc-sensors → 切到 JavaScript 把本檔全部貼上 → 下載到 micro:bit。
// 看到什麼：開機等 2~4 秒打勾（8 顆雷射開好）；手放在左前方 → 光柱在左；正前方 → 中間；越靠近光柱越高；四周沒東西 → 全暗。

let busy = false

if (mbc.waitReady(mbc.ADDR_TOF, 5000)) {                  // 8 顆雷射開機約 2~4 秒
    basic.showIcon(IconNames.Yes)
} else {
    basic.showIcon(IconNames.No)                          // 沒回應：檢查 P19／P20 和外接電源
}

basic.forever(function () {
    if (busy) {
        basic.pause(100)
        return
    }
    let dir = mbc.tof8NearestDirection()                  // −7（正左）~ +7（正右），0 = 正前方；±8 = 四周沒東西
    let d = mbc.tof8Nearest(MbcDistUnit.Cm)               // 8 顆裡最近的，0~120 cm
    basic.clearScreen()
    if (dir >= -7 && dir <= 7 && d < 120) {
        let x = Math.round(Math.map(dir, -7, 7, 0, 4))    // 方向 → 第 0~4 行
        let h = Math.round(Math.map(d, 0, 120, 5, 1))     // 距離 → 高度 1~5 格
        for (let y = 5 - h; y < 5; y++) {
            led.plot(x, y)
        }
    }
    basic.pause(50)
})

input.onButtonPressed(Button.A, function () {
    busy = true
    basic.showNumber(mbc.tof8Nearest(MbcDistUnit.Cm))
    busy = false
})
input.onButtonPressed(Button.B, function () {
    busy = true
    basic.showNumber(mbc.tof8NearestDirection())
    busy = false
})
```

**一步一步看懂程式**
1. `mbc.waitReady(mbc.ADDR_TOF, 5000)`：8 顆雷射要 2～4 秒才全部開好，所以等 5 秒。
2. `forever`：讀 `dir = mbc.tof8NearestDirection()`（−7～+7）和 `d = mbc.tof8Nearest(Cm)`。
3. `dir` 在 ±7 內而且 `d < 120` 才畫：`x = Math.map(dir, −7, 7, 0, 4)` 方向換成第 0～4 行；`h = Math.map(d, 0, 120, 5, 1)` 距離換成高度 1～5 格。
4. `for (y = 5 − h; y < 5; y++) led.plot(x, y)`：從底部往上畫 h 格。
5. ±8 或 120 = 四周沒東西：`basic.clearScreen()` 全暗。

**跑起來應該看到什麼**
- 開機 2～4 秒後打勾。
- 書放左前方 30 cm：光柱在左邊、約 4 格高；移到正前方：中間那行。
- 拿遠到 1 公尺：剩 1 格；拿走：全暗。

**常見問題**
- **開機是 X？** → 外接 5 V 沒開、或負極沒和 micro:bit 共地。這款不能靠 micro:bit 3V 腳。
- **光柱左右跟實際相反？** → 感應器裝反了（S1 要在左邊）。轉 180°，不要改程式。
- **開機就是 X（找不到感應器）？** → 看接線 SDA＝P20、SCL＝P19 有沒有接反；外接電源有沒有開；用鱷魚夾接的話夾子有沒有碰到旁邊的腳。
- **模擬器裡數字都是 0？** → 正常。MakeCode 模擬器沒有真的 I2C，讀到的都是 0；要看真的數值一定要下載到 micro:bit。
- **V1 沒有聲音？** → V1 沒有喇叭，在 P0 和 GND 之間接蜂鳴器或耳機。

---

## 範例 2：左／中／右警報

**這個範例會做什麼**：20 cm 內有東西：在左邊 → 左箭頭＋低音、正前方 → 上箭頭＋高音、右邊 → 右箭頭＋中音；沒東西 → 笑臉。按 B 開關聲音。

**要準備什麼**
- 範例 1 的東西
- V1 另外要蜂鳴器或耳機接 P0

**怎麼接**：同範例 1。

**下載程式**：[ex02_sector_alarm.ts](/examples/tof8/microbit/ex02_sector_alarm.ts)（右鍵另存）→ MakeCode 新專案 → 齒輪 → 擴充 → 貼 `https://github.com/contactmbcrobot/pxt-mbc-sensors` → 右上切到 **JavaScript** → 全選刪掉原本的內容、把檔案內容貼上 → **下載**，把 .hex 拖進 MICROBIT 磁碟。

**程式**

```typescript
// 測距者 8 路 × micro:bit 範例 2：左／中／右警報
// 做什麼：20 cm 內有東西：在左邊 → 左箭頭＋低音、正前方 → 上箭頭＋高音、右邊 → 右箭頭＋中音；沒東西 → 笑臉。按 B 開關聲音。
// 接線：感應器 SDA → P20、SCL → P19、VCC → 外接 5 V、GND → GND 共地。V1 要在 P0 接蜂鳴器或耳機才有聲音（V2 有內建喇叭）。
// 準備：擴充與貼程式的方法同範例 1。
// 看到什麼：手繞著感應器前方移動，箭頭和音高跟著換邊；手拿遠 → 笑臉、安靜。

const ALARM_CM = 20
let sound = true

input.onButtonPressed(Button.B, function () {
    sound = !sound
    basic.showString(sound ? "S" : "M")                   // S = 有聲音、M = 靜音
})

basic.forever(function () {
    let d = mbc.tof8Nearest(MbcDistUnit.Cm)
    let dir = mbc.tof8NearestDirection()
    if (d < ALARM_CM && Math.abs(dir) <= 7) {
        if (dir < -2) {                                   // 左邊三顆
            basic.showArrow(ArrowNames.West, 0)
            if (sound) {
                music.playTone(Note.C, 200)
            }
        } else if (dir > 2) {                             // 右邊三顆
            basic.showArrow(ArrowNames.East, 0)
            if (sound) {
                music.playTone(Note.G, 200)
            }
        } else {                                          // 正前方（第 4、5 顆）
            basic.showArrow(ArrowNames.North, 0)
            if (sound) {
                music.playTone(Note.C5, 200)
            }
        }
    } else {
        basic.showIcon(IconNames.Happy, 0)
    }
    basic.pause(100)
})
```

**一步一步看懂程式**
1. `ALARM_CM = 20`、`sound = true`。按 B 翻轉 `sound`，顯示 S（有聲）或 M（靜音）。
2. `forever`：讀最近距離 `d` 和方向 `dir`。
3. `d < 20` 而且 `Math.abs(dir) <= 7`（真的有東西）：`dir < −2` 左邊三顆 → 左箭頭＋`Note.C`；`dir > 2` 右邊三顆 → 右箭頭＋`Note.G`；中間 → 上箭頭＋`Note.C5`。
4. `if (sound)` 包住每個 `music.playTone`，靜音時只有箭頭。
5. 沒東西：笑臉。每 0.1 秒一輪。

**跑起來應該看到什麼**
- 手在左邊 15 cm：左箭頭、低音一直響。
- 移到正前方：上箭頭、高音。
- 拿遠：笑臉、安靜。按 B → M，再靠近只有箭頭。

**常見問題**
- **方向跳來跳去？** → 兩顆雷射讀到差不多的距離時「最近方向」會在兩者間切換。把 `−2`／`2` 改成 `−3`／`3` 讓中間區大一點。
- **沒聲音？** → V1 要接蜂鳴器；確認螢幕是 S 不是 M。

---

## 範例 3：雷達轉頭（伺服對準最近的東西）

**這個範例會做什麼**：伺服上貼一個箭頭，箭頭一直指著最近的東西；四周沒東西就回正中。按 A 輪流顯示 8 顆的公分數（S1 → S8）。

**要準備什麼**
- 範例 1 的東西
- 9g 伺服（SG90）×1＋紙箭頭
- 4.8～6 V 電池盒＋杜邦線

**怎麼接**：感應器同範例 1（外接 5 V）。伺服 **橘（訊號）→ P1**、紅 → 外接 4.8～6 V 電池正極、棕 → 電池負極，電池負極再接一條到 micro:bit GND（共地）。

**下載程式**：[ex03_pan_servo.ts](/examples/tof8/microbit/ex03_pan_servo.ts)（右鍵另存）→ MakeCode 新專案 → 齒輪 → 擴充 → 貼 `https://github.com/contactmbcrobot/pxt-mbc-sensors` → 右上切到 **JavaScript** → 全選刪掉原本的內容、把檔案內容貼上 → **下載**，把 .hex 拖進 MICROBIT 磁碟。

**程式**

```typescript
// 測距者 8 路 × micro:bit 範例 3：雷達轉頭（伺服對準最近的東西）
// 做什麼：伺服上貼一個箭頭，箭頭一直指著最近的東西；四周沒東西就回正中。按 A 輪流顯示 8 顆的公分數（S1 最左 → S8 最右）。
// 接線：感應器 SDA → P20、SCL → P19、VCC → 外接 5 V、GND → GND。伺服 橘（訊號）→ P1、紅 → 外接 4.8~6 V 電池正極、棕 → 電池負極並接 micro:bit GND。
// 準備：9g 伺服（SG90）＋紙箭頭。
// 看到什麼：手從左邊繞到右邊，箭頭跟著轉；手拿開 → 回到 90°。

let busy = false

basic.forever(function () {
    if (busy) {
        basic.pause(100)
        return
    }
    let dir = mbc.tof8NearestDirection()                  // −7~+7，負 = 左；±8 = 沒東西
    let d = mbc.tof8Nearest(MbcDistUnit.Cm)
    let angle = 90
    if (Math.abs(dir) <= 7 && d < 120) {
        angle = 90 + dir * 12                             // 每一格 12°：±7 → 6°~174°
    }
    pins.servoWritePin(AnalogPin.P1, angle)
    basic.pause(50)
})

input.onButtonPressed(Button.A, function () {
    busy = true
    for (let n = 1; n <= 8; n++) {
        basic.showString("S" + n)                         // 第 n 顆
        basic.showNumber(mbc.tof8Sensor(n, MbcDistUnit.Cm))
    }
    busy = false
})
```

**一步一步看懂程式**
1. `forever`：讀方向 `dir` 和最近距離 `d`，預設 `angle = 90`。
2. 有東西（`|dir| <= 7` 且 `d < 120`）：`angle = 90 + dir × 12`，每一格 12°，±7 → 6°～174°。
3. `pins.servoWritePin(AnalogPin.P1, angle)` 每 50 ms 更新。
4. 按 A：`for n = 1..8`，先 `showString("S" + n)` 再 `showNumber(mbc.tof8Sensor(n, Cm))`；`busy` 讓伺服迴圈先暫停。

**跑起來應該看到什麼**
- 手從左邊繞到右邊：箭頭跟著轉。
- 手拿開：回 90°。
- 按 A：S1 … 數字 … S2 … 一路到 S8；朝著手的那顆數字最小。

**常見問題**
- **箭頭轉的方向相反？** → 伺服裝的方向不同：把 `90 + dir × 12` 改成 `90 − dir × 12`。
- **伺服跳來跳去？** → 同範例 2，最近方向在兩顆之間切換。加一個簡單濾波：`angle = (angle + 新角度) / 2`。

---

## 範例 4：沿牆走（左牆）

**這個範例會做什麼**：車子靠左邊的牆走：用最左那顆 S1 保持離牆 15 cm（太遠往左靠、太近往右閃），正前方 15 cm 內有牆就原地右轉。A 開始、B 停。

**要準備什麼**
- 兩輪小車：兩顆連續旋轉伺服（FS90R 之類）當輪子＋一顆萬向輪
- 測距者 8 路裝在車頭（S1 朝正左）
- 4.8～6 V 電池盒（伺服用）＋外接 5 V（感應器用）
- 一面牆或一排箱子

**怎麼接**：感應器同範例 1（外接 5 V），裝在車頭：S1 朝正左、S4／S5 朝正前、S8 朝正右。**左輪連續旋轉伺服訊號 → P1、右輪 → P2**，兩顆伺服的紅線接 4.8～6 V 電池盒正極、棕線接電池負極，電池負極再接 micro:bit GND。

**下載程式**：[ex04_wall_follow.ts](/examples/tof8/microbit/ex04_wall_follow.ts)（右鍵另存）→ MakeCode 新專案 → 齒輪 → 擴充 → 貼 `https://github.com/contactmbcrobot/pxt-mbc-sensors` → 右上切到 **JavaScript** → 全選刪掉原本的內容、把檔案內容貼上 → **下載**，把 .hex 拖進 MICROBIT 磁碟。

**程式**

```typescript
// 測距者 8 路 × micro:bit 範例 4：沿牆走（左牆）
// 做什麼：車子靠左邊的牆走，用最左那顆（S1）保持離牆 15 cm；正前方 15 cm 內有牆就原地右轉。A 開始、B 停。
// 接線：感應器 SDA → P20、SCL → P19、VCC → 外接 5 V、GND → GND。左輪連續旋轉伺服訊號 → P1、右輪 → P2；伺服電源 4.8~6 V 電池盒，電池負極接 micro:bit GND。
// 準備：兩輪小車，感應器裝在車頭（S1 朝正左、S4／S5 朝正前）；一面牆或一排箱子。
// 看到什麼：按 A 車子沿著左牆走，離牆太遠就往左靠、太近就往右閃；前面遇到牆角就右轉繼續。

const WALL_CM = 15
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
    let side = mbc.tof8Sensor(1, MbcDistUnit.Cm)          // 第 1 顆 = 正左
    let front = mbc.tof8Front(MbcDistUnit.Cm)             // 正前方（第 4、5 顆平均）
    if (front < WALL_CM) {
        drive(40, -40)                                    // 前面有牆：原地右轉
        basic.showArrow(ArrowNames.East, 0)
    } else {
        let err = side - WALL_CM                          // 正 = 離牆太遠
        let turn = Math.constrain(err * 3, -30, 30)
        drive(50 - turn, 50 + turn)                       // 太遠 → 左輪慢 → 往左靠
        if (turn > 5) {
            basic.showArrow(ArrowNames.NorthWest, 0)
        } else if (turn < -5) {
            basic.showArrow(ArrowNames.NorthEast, 0)
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
3. `forever`：`side = mbc.tof8Sensor(1, Cm)`（正左）、`front = mbc.tof8Front(Cm)`（S4、S5 平均）。
4. `front < 15`：前面有牆 → `drive(40, −40)` 原地右轉、右箭頭。
5. 否則 `err = side − 15`：正 = 離牆太遠；`turn = err × 3` 限制 ±30；`drive(50 − turn, 50 + turn)`：太遠 → 左輪慢 → 往左靠；太近 → 相反。箭頭顯示左上／右上／上。

**跑起來應該看到什麼**
- 車子放在牆的右邊約 15 cm、車頭平行牆：按 A 沿牆走。
- 把車推離牆：往左靠回來；推近牆：往右閃。
- 前面有牆角：右轉後繼續沿新的牆走。

**常見問題**
- **蛇行？** → `× 3` 改 `× 2`，或 `basic.pause(30)` 改 50。
- **右轉轉不夠、撞牆？** → 轉彎時只看 `front`，轉到前方空了就直走。把 `front < 15` 改 `front < 25` 早一點轉。
- **想改沿右牆？** → `tof8Sensor(1, ...)` 改 `tof8Sensor(8, ...)`，`drive(50 − turn, 50 + turn)` 改 `drive(50 + turn, 50 − turn)`，前面有牆改左轉 `drive(−40, 40)`。

---

## 範例 5：自動探索（綜合）

**這個範例會做什麼**：車子一直往「最空曠的方向」偏過去走；12 cm 內有東西就倒退、往空曠那邊原地轉；進到 12 cm 內的瞬間閃骷髏並計數（事件）。A 開始、B 停下來顯示躲了幾次。

**要準備什麼**
- 範例 4 的小車
- 幾個箱子圍成簡單迷宮

**怎麼接**：同範例 4。

**下載程式**：[ex05_explore.ts](/examples/tof8/microbit/ex05_explore.ts)（右鍵另存）→ MakeCode 新專案 → 齒輪 → 擴充 → 貼 `https://github.com/contactmbcrobot/pxt-mbc-sensors` → 右上切到 **JavaScript** → 全選刪掉原本的內容、把檔案內容貼上 → **下載**，把 .hex 拖進 MICROBIT 磁碟。

**程式**

```typescript
// 測距者 8 路 × micro:bit 範例 5：自動探索（往最空曠的方向走，綜合）
// 做什麼：車子一直往「最空曠的方向」偏過去走；12 cm 內有東西 → 倒退、往空曠那邊原地轉；每次閃骷髏並計數。A 開始、B 停。
// 接線：同範例 4（感應器 P19／P20、外接 5 V；左輪 P1、右輪 P2、伺服外接電池、共地）。
// 準備：兩輪小車＋幾個箱子圍成迷宮。
// 看到什麼：按 A 後車子自己在箱子間繞，不會撞上去；按 B 停下來顯示躲了幾次。

const STOP_CM = 12
let running = false
let dodges = 0

// 左輪伺服 P1、右輪伺服 P2（連續旋轉伺服）；left／right = −100~100，正 = 前進
function drive(left: number, right: number): void {
    left = Math.constrain(left, -100, 100)
    right = Math.constrain(right, -100, 100)
    pins.servoWritePin(AnalogPin.P1, Math.round(90 + left * 0.9))    // 90 = 停、180 = 全速前進、0 = 全速後退
    pins.servoWritePin(AnalogPin.P2, Math.round(90 - right * 0.9))   // 右輪裝的方向相反，所以用減的
}

mbc.onTof8Near(STOP_CM, function () {                     // 事件：每次進到 12 cm 內觸發一次
    dodges += 1
    basic.showIcon(IconNames.Skull, 0)
})

input.onButtonPressed(Button.A, function () {
    dodges = 0
    running = true
    basic.showArrow(ArrowNames.North, 0)
})
input.onButtonPressed(Button.B, function () {
    running = false
    drive(0, 0)
    basic.showNumber(dodges)
})

basic.forever(function () {
    if (!running) {
        basic.pause(100)
        return
    }
    let near = mbc.tof8Nearest(MbcDistUnit.Cm)
    let open = mbc.tof8OpenDirection()                    // 最空曠的方向 −7~+7，負 = 左
    if (near < STOP_CM) {
        drive(-40, -40)                                   // 倒退 0.4 秒
        basic.pause(400)
        if (open < 0) {
            drive(-40, 40)                                // 空曠在左 → 原地左轉
        } else {
            drive(40, -40)
        }
        basic.pause(500)
    } else {
        let turn = Math.constrain(open * 8, -40, 40)      // 空曠在左（負）→ 左輪慢 → 往左偏
        drive(50 + turn, 50 - turn)
        basic.showArrow(ArrowNames.North, 0)
    }
    basic.pause(30)
})
```

**一步一步看懂程式**
1. `mbc.onTof8Near(STOP_CM, ...)`：事件，每次有東西進到 12 cm 內 `dodges + 1`、閃骷髏。
2. `forever`：讀 `near = 最近距離`、`open = mbc.tof8OpenDirection()`（最空曠方向，負 = 左）。
3. `near < 12`：`drive(−40, −40)` 倒退 0.4 秒；`open < 0` → `drive(−40, 40)` 原地左轉 0.5 秒，否則右轉。
4. 否則 `turn = open × 8`（限制 ±40），`drive(50 + turn, 50 − turn)`：空曠在左（負）→ 左輪慢 → 往左偏。
5. B：停車並 `showNumber(dodges)`。

**跑起來應該看到什麼**
- 按 A：車子自己在箱子間繞，朝空的地方走。
- 快撞到：骷髏、倒退、轉彎、繼續。
- 按 B：顯示躲了幾次。

**常見問題**
- **一直打轉不前進？** → `× 8` 太大，改 `× 4`。
- **在死路出不來？** → 倒退時間 400 改 800、原地轉 500 改 900（轉約 180°）。

---

## 下一步

- 全部積木、數值和硬體說明：[測距者 8 路 × micro:bit](/sensors/tof8/microbit.md)
- 每個數值編號的意思：[指令參考](/sensors/tof8/reference.md)
- 擴充原始碼與回報問題：<https://github.com/contactmbcrobot/pxt-mbc-sensors>
