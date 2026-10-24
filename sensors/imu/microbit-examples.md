<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 W4 micro:bit 範例產生器產生（scratchpad w4/gen_pages.mjs＋spec/imu.mjs）；程式碼區塊來自 examples/imu/microbit/*.ts，改程式請改 .ts 後重跑產生器 -->
<!-- moved: /sensors/imu/microbit/makecode.md -->
<p class="moved-note">這頁內容已整合到新的說明頁：<a href="/sensors/imu/microbit/makecode.md">陀螺儀 IMU 九軸 › micro:bit › MakeCode</a></p>

# 🟧 micro:bit 範例：陀螺儀 IMU

[🔙 回到陀螺儀 IMU micro:bit 頁](/sensors/imu/microbit.md)　｜　[📖 產品頁](/sensors/imu/index.md)　｜　[📋 指令參考](/sensors/imu/reference.md)

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
| 供電 | 約 20 mA，可以先試 micro:bit 的 3V 腳（V1 約 90 mA、V2 約 190 mA 上限） | 上機待測 |
| I2C 上拉 | micro:bit 板上已有 4.7 kΩ；產品板若也有上拉，並聯後要確認低電位 ≤ 0.3×VDD | 上機待測 |
| 時脈延展 | STM32 從機預設會時脈延展，V2（nRF52833）對延展有已知問題，要 V1、V2 各測一次 | 上機待測 |
| 位址衝突 | 出廠位址 0x6A，和其他 MBC 產品都不同，可以直接和循行者、測距者、掌控者串同一條線 | — |
| 就緒時間 | 開機後約 2 秒就緒（F3 b0）；開機和歸零時板子要**靜止平放**，航向才準 | 上機待測 |

讀不到值時先看：接線（SDA＝P20、SCL＝P19）→ 外接電源有沒有開 → 擴充「裝置」分類的「位址 … 的裝置就緒？」積木是不是 true。

### 接線（所有範例共用）

<figure class="ref-fig"><img src="/images/figures/imu/wiring-microbit.svg" alt="陀螺儀 IMU × micro:bit" loading="lazy"></figure>

| 感應器 | micro:bit |
| :--- | :--- |
| SDA | P20 |
| SCL | P19 |
| VCC | 3V（或外接 5 V） |
| GND | GND（伺服電池負極也接這裡） |

## 這頁會用到的數值

航向單位是**度**，開機或歸零時 = 0，**往右轉變大、往左轉變小**，轉超過一圈會一直累加（不會在 360 歸零）。「融合」航向 = 陀螺儀＋磁力計，長時間最穩。

| 積木（繁中） | JavaScript | 範圍 | 意思 | 用在 |
| :--- | :--- | :--- | :--- | :--- |
| 陀螺儀 融合 航向（°） | `mbc.imuHeading(MbcImuHeading.Fused)` | −32768～32767 | 目前朝向，0 = 歸零時的方向，右轉為正 | 1, 3, 4, 5 |
| 陀螺儀 航向歸零 | `mbc.imuZeroHeading()` | 命令 E0 | 把現在的方向設成 0 | 1, 2, 3, 4, 5 |
| 陀螺儀 俯仰角（°） | `mbc.imuPitch()` | −90～+90 | 前端抬高／壓低幾度 | 2 |
| 當陀螺儀 轉了 … ° | `mbc.onImuTurned(90, ...)` | 事件 | 融合航向每累積變化這麼多度觸發一次 | 2 |
| 陀螺儀 靜止？ | `mbc.imuFlag(MbcImuFlag.Still)` | true / false | 狀態旗標 b1：板子沒在動 | 2 |
| 等位址 … 的裝置就緒 | `mbc.waitReady(mbc.ADDR_IMU, 3000)` | true / false | 最多等 3 秒，陀螺儀初始化好才 true（進階 → 裝置） | 1 |

---

## 範例 1：電子羅盤

**這個範例會做什麼**：把 micro:bit 和陀螺儀一起轉，LED 箭頭會反方向轉，一直指著開機時的正前方。按 A 顯示航向角度、按 B 把現在的方向設成 0。

**要準備什麼**
- micro:bit V1 或 V2＋USB 線
- 陀螺儀 IMU（通用 I2C 版）＋4P 線
- 把兩塊板子固定在同一塊紙板上

**怎麼接**：照上面的接線表：SDA → P20、SCL → P19、VCC → 3V、GND → GND。

**下載程式**：[ex01_compass.ts](/examples/imu/microbit/ex01_compass.ts)（右鍵另存）→ MakeCode 新專案 → 齒輪 → 擴充 → 貼 `https://github.com/contactmbcrobot/pxt-mbc-sensors` → 右上切到 **JavaScript** → 全選刪掉原本的內容、把檔案內容貼上 → **下載**，把 .hex 拖進 MICROBIT 磁碟。

**程式**

```typescript
// 陀螺儀 × micro:bit 範例 1：電子羅盤（箭頭一直指著開機時的正前方）
// 做什麼：把 micro:bit 和陀螺儀一起轉，LED 箭頭會反方向轉，一直指著原本的正前方。按 A 顯示航向角度、按 B 把現在的方向設成 0。
// 接線：陀螺儀 SDA → P20、SCL → P19、VCC → 3V（或外接 5 V）、GND → GND。
// 準備：MakeCode 新專案 → 擴充 → 貼 https://github.com/contactmbcrobot/pxt-mbc-sensors → 切到 JavaScript 把本檔全部貼上 → 下載到 micro:bit。陀螺儀平放、開機後 2 秒不要動。
// 看到什麼：開機打勾、箭頭朝上；往右轉 90° → 箭頭指左；轉 180° → 箭頭朝下；轉回來 → 箭頭朝上。

const ARROWS = [ArrowNames.North, ArrowNames.NorthEast, ArrowNames.East, ArrowNames.SouthEast,
    ArrowNames.South, ArrowNames.SouthWest, ArrowNames.West, ArrowNames.NorthWest]   // 順時針 8 個方向
let busy = false

if (mbc.waitReady(mbc.ADDR_IMU, 3000)) {
    basic.showIcon(IconNames.Yes)
} else {
    basic.showIcon(IconNames.No)                          // 沒回應：檢查 P19／P20 和電源
}
mbc.imuZeroHeading()                                      // 現在的方向 = 0°

basic.forever(function () {
    if (busy) {
        basic.pause(100)
        return
    }
    let h = mbc.imuHeading(MbcImuHeading.Fused)           // 度，往右轉變大、往左轉變小，可超過 360
    let a = ((h % 360) + 360) % 360                       // 整理成 0~359
    let idx = Math.round(a / 45) % 8                      // 轉了幾個 45°
    basic.showArrow(ARROWS[(8 - idx) % 8], 0)             // 箭頭要往反方向指回原本的正前方
    basic.pause(50)
})

input.onButtonPressed(Button.A, function () {
    busy = true
    basic.showNumber(mbc.imuHeading(MbcImuHeading.Fused))
    busy = false
})
input.onButtonPressed(Button.B, function () {
    mbc.imuZeroHeading()
})
```

**一步一步看懂程式**
1. `ARROWS`：8 個箭頭，從上開始順時針排。
2. `mbc.waitReady(mbc.ADDR_IMU, 3000)` 打勾後 `mbc.imuZeroHeading()`：現在的方向 = 0°。
3. `forever`：`h = mbc.imuHeading(Fused)`；`a = ((h % 360) + 360) % 360` 把它整理成 0～359（負數也能處理）。
4. `idx = Math.round(a / 45) % 8`：轉了幾個 45°。
5. `ARROWS[(8 − idx) % 8]`：板子往右轉了 idx 格，原本的正前方就在左邊 idx 格，所以箭頭取反方向。
6. A：顯示航向；B：歸零。

**跑起來應該看到什麼**
- 開機（板子不動）：打勾、箭頭朝上。
- 往右轉 90°：箭頭指左；按 A 顯示約 90。
- 轉 180°：箭頭朝下。轉回來：朝上。
- 按 B：不管現在朝哪裡，箭頭都變成朝上。

**常見問題**
- **沒轉它，數字卻慢慢變？** → 陀螺儀飄移。開機時板子要靜止；放著 10 秒讓它自己校準，或按 B 歸零。
- **箭頭轉的方向跟板子一樣（沒有反過來）？** → 把 `(8 − idx) % 8` 改成 `idx`，就變成「箭頭跟著轉」的另一種羅盤。
- **開機就是 X（找不到感應器）？** → 看接線 SDA＝P20、SCL＝P19 有沒有接反；外接電源有沒有開；用鱷魚夾接的話夾子有沒有碰到旁邊的腳。
- **模擬器裡數字都是 0？** → 正常。MakeCode 模擬器沒有真的 I2C，讀到的都是 0；要看真的數值一定要下載到 micro:bit。
- **V1 沒有聲音？** → V1 沒有喇叭，在 P0 和 GND 之間接蜂鳴器或耳機。

---

## 範例 2：轉彎計數＋傾斜警報

**這個範例會做什麼**：每轉 90° 嗶一聲、次數 +1（事件積木）；前後傾斜超過 20° → 哭臉＋低音警報；靜止不動 → 睡臉、有在動 → 笑臉。按 B 歸零。

**要準備什麼**
- 範例 1 的東西
- V1 另外要蜂鳴器或耳機接 P0

**怎麼接**：同範例 1。

**下載程式**：[ex02_turn_alarm.ts](/examples/imu/microbit/ex02_turn_alarm.ts)（右鍵另存）→ MakeCode 新專案 → 齒輪 → 擴充 → 貼 `https://github.com/contactmbcrobot/pxt-mbc-sensors` → 右上切到 **JavaScript** → 全選刪掉原本的內容、把檔案內容貼上 → **下載**，把 .hex 拖進 MICROBIT 磁碟。

**程式**

```typescript
// 陀螺儀 × micro:bit 範例 2：轉彎計數＋傾斜警報
// 做什麼：每轉 90° 嗶一聲並把次數 +1；前後傾斜超過 20° → 哭臉＋低音警報；靜止不動 → 睡臉、有在動 → 笑臉。按 B 歸零。
// 接線：陀螺儀 SDA → P20、SCL → P19、VCC → 3V、GND → GND。V1 要在 P0 接蜂鳴器或耳機才有聲音（V2 有內建喇叭）。
// 準備：擴充與貼程式的方法同範例 1。
// 看到什麼：桌上慢慢轉一圈 → 嗶 4 聲、顯示 1 2 3 4；把板子前端抬高 → 哭臉＋警報；放著不動 → 睡臉。

let turns = 0

mbc.imuZeroHeading()

mbc.onImuTurned(90, function () {                         // 融合航向每累積變化 90° 觸發一次
    turns += 1
    music.playTone(Note.C5, 150)
    basic.showNumber(turns)
})

input.onButtonPressed(Button.B, function () {
    turns = 0
    mbc.imuZeroHeading()
    basic.showNumber(0)
})

basic.forever(function () {
    let p = mbc.imuPitch()                                // 俯仰角，度：前端抬高或壓低
    if (Math.abs(p) > 20) {
        basic.showIcon(IconNames.Sad, 0)
        music.playTone(Note.C, 200)                       // 低音警報
    } else if (mbc.imuFlag(MbcImuFlag.Still)) {           // 狀態旗標 b1：靜止
        basic.showIcon(IconNames.Asleep, 0)
    } else {
        basic.showIcon(IconNames.Happy, 0)
    }
    basic.pause(100)
})
```

**一步一步看懂程式**
1. `mbc.imuZeroHeading()` 先歸零。
2. `mbc.onImuTurned(90, ...)`：融合航向每累積變化 90° 執行一次 → `turns + 1`、`Note.C5`、顯示次數。
3. `forever`：`p = mbc.imuPitch()`（俯仰角）。`Math.abs(p) > 20` → 哭臉＋`Note.C` 低音。
4. 否則 `mbc.imuFlag(MbcImuFlag.Still)` 是 true → 睡臉；不是 → 笑臉。
5. B：`turns = 0`、歸零、顯示 0。

**跑起來應該看到什麼**
- 桌上慢慢轉一圈：嗶 4 聲、顯示 1 2 3 4。
- 把板子前端抬高超過 20°：哭臉＋警報；放平：停。
- 放著不動 1 秒：睡臉；拿起來晃：笑臉。

**常見問題**
- **轉一圈嗶了 5 聲？** → 轉太快有過衝，或轉回來時也算（來回都累積）。這是「變化量」事件，不分方向。
- **傾斜警報一直響？** → 陀螺儀沒平放、或安裝軸還沒擷取（狀態 b2）。放平開機再試。

---

## 範例 3：穩定雲台（伺服永遠指著同一個方向）

**這個範例會做什麼**：伺服裝在陀螺儀旁邊、擺臂上貼箭頭：轉動底座，伺服會反方向轉回去，箭頭一直指著原本的方向（±90° 內）。按 A 重設「原本的方向」。

**要準備什麼**
- 範例 1 的東西
- 9g 伺服（SG90）×1＋紙箭頭
- 4.8～6 V 電池盒＋杜邦線
- 一塊板子把 micro:bit、陀螺儀、伺服固定在一起

**怎麼接**：陀螺儀同範例 1。伺服 **橘（訊號）→ P1**、紅 → 外接 4.8～6 V 電池正極、棕 → 電池負極，電池負極再接一條到 micro:bit GND（共地）。

**下載程式**：[ex03_stabilize_servo.ts](/examples/imu/microbit/ex03_stabilize_servo.ts)（右鍵另存）→ MakeCode 新專案 → 齒輪 → 擴充 → 貼 `https://github.com/contactmbcrobot/pxt-mbc-sensors` → 右上切到 **JavaScript** → 全選刪掉原本的內容、把檔案內容貼上 → **下載**，把 .hex 拖進 MICROBIT 磁碟。

**程式**

```typescript
// 陀螺儀 × micro:bit 範例 3：穩定雲台（伺服永遠指著同一個方向）
// 做什麼：伺服裝在陀螺儀旁邊、上面貼箭頭：轉動底座，伺服會反方向轉回去，箭頭一直指著原本的方向（±90° 內）。按 A 重設「原本的方向」。
// 接線：陀螺儀 SDA → P20、SCL → P19、VCC → 3V、GND → GND。伺服 橘（訊號）→ P1、紅 → 外接 4.8~6 V 電池正極、棕 → 電池負極並接 micro:bit GND。
// 準備：9g 伺服（SG90）、一塊板子把 micro:bit、陀螺儀、伺服固定在一起。
// 看到什麼：底座往右轉 45° → 伺服往左轉 45°，箭頭還是指著原方向；轉超過 90° 伺服到底就跟不上了。

mbc.imuZeroHeading()
basic.showIcon(IconNames.Yes)

basic.forever(function () {
    let h = mbc.imuHeading(MbcImuHeading.Fused)           // 往右轉為正
    let angle = Math.constrain(90 - h, 0, 180)            // 底座右轉 → 伺服左轉
    pins.servoWritePin(AnalogPin.P1, angle)
    basic.pause(20)
})

input.onButtonPressed(Button.A, function () {
    mbc.imuZeroHeading()                                  // 現在的方向當成新的 0°
    basic.showIcon(IconNames.Yes, 0)
})
```

**一步一步看懂程式**
1. `mbc.imuZeroHeading()`：現在的方向 = 0。
2. `forever`：`h = 融合航向`（右轉為正）。
3. `angle = Math.constrain(90 − h, 0, 180)`：底座右轉 h 度，伺服就轉到 90 − h（往左 h 度），箭頭還是指原方向。
4. `pins.servoWritePin(AnalogPin.P1, angle)` 每 20 ms 更新。
5. A：重新歸零，現在的方向變成新的基準。

**跑起來應該看到什麼**
- 底座往右轉 45°：伺服往左 45°，箭頭沒動。
- 轉超過 90°：伺服到底（0° 或 180°）跟不上了。
- 按 A：伺服回 90°、以現在的方向為準。

**常見問題**
- **伺服跟底座同方向轉？** → 伺服裝的方向相反：`90 − h` 改 `90 + h`。
- **慢慢偏掉？** → 陀螺儀飄移，按 A 歸零；靜止 10 秒讓它自動校準會改善。

---

## 範例 4：走直線（陀螺儀修正）

**這個範例會做什麼**：按 A 車子往前走，偏掉就用航向修回來，所以能走得很直；按 B 停。LED 顯示往哪邊修。這是所有陀螺儀機器人的基本功。

**要準備什麼**
- 兩輪小車：兩顆連續旋轉伺服（FS90R 之類）當輪子＋一顆萬向輪
- 陀螺儀平放在車上、絲印箭頭朝前
- 4.8～6 V 電池盒

**怎麼接**：陀螺儀同範例 1，平放在車上、絲印箭頭朝車頭。**左輪連續旋轉伺服訊號 → P1、右輪 → P2**，兩顆伺服的紅線接 4.8～6 V 電池盒正極、棕線接電池負極，電池負極再接 micro:bit GND。

**下載程式**：[ex04_drive_straight.ts](/examples/imu/microbit/ex04_drive_straight.ts)（右鍵另存）→ MakeCode 新專案 → 齒輪 → 擴充 → 貼 `https://github.com/contactmbcrobot/pxt-mbc-sensors` → 右上切到 **JavaScript** → 全選刪掉原本的內容、把檔案內容貼上 → **下載**，把 .hex 拖進 MICROBIT 磁碟。

**程式**

```typescript
// 陀螺儀 × micro:bit 範例 4：走直線（陀螺儀修正）
// 做什麼：按 A 車子往前走，偏掉就用陀螺儀的航向修回來，所以能走得很直；按 B 停。LED 顯示往哪邊修。
// 接線：陀螺儀 SDA → P20、SCL → P19、VCC → 3V、GND → GND。左輪連續旋轉伺服訊號 → P1、右輪 → P2；伺服電源 4.8~6 V 電池盒，電池負極接 micro:bit GND。
// 準備：兩輪小車，陀螺儀平放在車上（絲印箭頭朝前）。
// 看到什麼：按 A 車子直直走；用手把車頭撥歪，它會自己轉回原本的方向繼續走。

const KP = 2                                              // 偏 1° 修正 2% 速度差
let running = false

// 左輪伺服 P1、右輪伺服 P2（連續旋轉伺服）；left／right = −100~100，正 = 前進
function drive(left: number, right: number): void {
    left = Math.constrain(left, -100, 100)
    right = Math.constrain(right, -100, 100)
    pins.servoWritePin(AnalogPin.P1, Math.round(90 + left * 0.9))    // 90 = 停、180 = 全速前進、0 = 全速後退
    pins.servoWritePin(AnalogPin.P2, Math.round(90 - right * 0.9))   // 右輪裝的方向相反，所以用減的
}

input.onButtonPressed(Button.A, function () {
    mbc.imuZeroHeading()                                  // 出發方向 = 0°
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
    let h = mbc.imuHeading(MbcImuHeading.Fused)           // 正 = 車頭偏右了
    let turn = Math.constrain(h * KP, -40, 40)
    drive(50 - turn, 50 + turn)                           // 偏右 → 左輪慢、右輪快 → 轉回左
    if (turn > 5) {
        basic.showArrow(ArrowNames.NorthWest, 0)
    } else if (turn < -5) {
        basic.showArrow(ArrowNames.NorthEast, 0)
    } else {
        basic.showArrow(ArrowNames.North, 0)
    }
    basic.pause(20)
})
```

**一步一步看懂程式**
1. `function drive(left, right)`：左輪 P1、右輪 P2，−100～100，正 = 前進；右輪裝反所以用減的。
2. A：`mbc.imuZeroHeading()` 出發方向 = 0，`running = true`；B：停。
3. `forever`：`h = 融合航向`，正 = 車頭偏右了。
4. `turn = Math.constrain(h × KP, −40, 40)`，`KP = 2`：偏 1° 差 2%。
5. `drive(50 − turn, 50 + turn)`：偏右 → 左輪慢、右輪快 → 轉回左。箭頭顯示修正方向。

**跑起來應該看到什麼**
- 按 A：車子直直走，比沒有陀螺儀時直很多。
- 用手把車頭撥歪 30°：箭頭指斜、車子轉回原本的方向繼續走。
- 按 B：停。

**常見問題**
- **左右擺動？** → `KP` 從 2 改 1。
- **修正方向反了（越偏越歪）？** → 陀螺儀裝反（箭頭朝後）或伺服正負相反。先跑範例 1 確認右轉時數字變大。

---

## 範例 5：走正方形（綜合）

**這個範例會做什麼**：按 A：直走 1.5 秒（用陀螺儀保持直線）→ 原地右轉剛好 90°（看航向，不是看時間）→ 重複 4 次回到原點；LED 顯示第幾邊。按 B 中途停。

**要準備什麼**
- 範例 4 的小車
- 約 1 公尺見方的空地

**怎麼接**：同範例 4。

**下載程式**：[ex05_square_path.ts](/examples/imu/microbit/ex05_square_path.ts)（右鍵另存）→ MakeCode 新專案 → 齒輪 → 擴充 → 貼 `https://github.com/contactmbcrobot/pxt-mbc-sensors` → 右上切到 **JavaScript** → 全選刪掉原本的內容、把檔案內容貼上 → **下載**，把 .hex 拖進 MICROBIT 磁碟。

**程式**

```typescript
// 陀螺儀 × micro:bit 範例 5：走正方形（綜合）
// 做什麼：按 A：直走 1.5 秒（用陀螺儀保持直線）→ 原地右轉剛好 90° → 重複 4 次回到原點，LED 顯示第幾邊；按 B 中途停。
// 接線：同範例 4（陀螺儀 P19／P20；左輪 P1、右輪 P2、伺服外接電池、共地）。
// 準備：兩輪小車＋約 1 公尺見方的空地。
// 看到什麼：按 A 車子走出一個正方形、顯示 1 2 3 4，最後停下來打勾。轉彎不足或過頭 → 把 TURN_SPEED 調小。

const SIDE_MS = 1500                                      // 每邊走多久
const TURN_SPEED = 35
let running = false

// 左輪伺服 P1、右輪伺服 P2（連續旋轉伺服）；left／right = −100~100，正 = 前進
function drive(left: number, right: number): void {
    left = Math.constrain(left, -100, 100)
    right = Math.constrain(right, -100, 100)
    pins.servoWritePin(AnalogPin.P1, Math.round(90 + left * 0.9))    // 90 = 停、180 = 全速前進、0 = 全速後退
    pins.servoWritePin(AnalogPin.P2, Math.round(90 - right * 0.9))   // 右輪裝的方向相反，所以用減的
}

// 直走 ms 毫秒，用陀螺儀保持在 target 這個航向
function straight(target: number, ms: number): void {
    let t0 = control.millis()
    while (running && control.millis() - t0 < ms) {
        let err = mbc.imuHeading(MbcImuHeading.Fused) - target
        let turn = Math.constrain(err * 2, -40, 40)
        drive(50 - turn, 50 + turn)
        basic.pause(20)
    }
}

// 原地轉到 target 航向（誤差 3° 內就停）
function turnTo(target: number): void {
    while (running) {
        let err = target - mbc.imuHeading(MbcImuHeading.Fused)   // 正 = 還要往右轉
        if (Math.abs(err) < 3) {
            break
        }
        let s = err > 0 ? TURN_SPEED : -TURN_SPEED
        drive(s, -s)                                      // 左輪前、右輪後 = 右轉
        basic.pause(20)
    }
    drive(0, 0)
    basic.pause(300)                                      // 停一下讓車身穩定
}

input.onButtonPressed(Button.A, function () {
    if (running) {
        return
    }
    running = true
    mbc.imuZeroHeading()
    let target = 0
    for (let side = 1; side <= 4; side++) {
        basic.showNumber(side)
        straight(target, SIDE_MS)
        target += 90                                      // 下一邊的航向
        turnTo(target)
    }
    drive(0, 0)
    if (running) {
        basic.showIcon(IconNames.Yes)
    }
    running = false
})
input.onButtonPressed(Button.B, function () {
    running = false
    drive(0, 0)
    basic.showIcon(IconNames.No)
})
```

**一步一步看懂程式**
1. `straight(target, ms)`：走 ms 毫秒，每 20 ms 用「航向 − target」修正，跟範例 4 一樣。
2. `turnTo(target)`：`err = target − 航向`，`|err| < 3` 就停；否則 `drive(s, −s)` 原地轉（err 正 → 右轉），轉到位後停 0.3 秒讓車身穩定。
3. A：歸零、`target = 0`；`for side = 1..4`：顯示邊數 → `straight(target, 1500)` → `target += 90` → `turnTo(target)`。
4. 4 邊走完 `drive(0, 0)`、打勾。
5. B：`running = false`，`straight` 和 `turnTo` 的 `while (running)` 會馬上跳出。

**跑起來應該看到什麼**
- 按 A：顯示 1、直走、右轉；2、直走、右轉 … 4 邊後打勾，回到差不多原點。
- 每個角都是 90°，不會越轉越歪（因為是看航向）。

**常見問題**
- **轉過頭又轉回來、抖？** → `TURN_SPEED` 35 改 25，或 `< 3` 改 `< 5`。
- **邊長不一樣？** → 這是用時間走的，電池電壓掉會變短；改用循行者或編碼器才會準。
- **想走三角形？** → `for` 改 3 次、`target += 120`。

---

## 下一步

- 全部積木、數值和硬體說明：[陀螺儀 IMU × micro:bit](/sensors/imu/microbit.md)
- 每個數值編號的意思：[指令參考](/sensors/imu/reference.md)
- 擴充原始碼與回報問題：<https://github.com/contactmbcrobot/pxt-mbc-sensors>
