<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 W4 micro:bit 範例產生器產生（scratchpad w4/gen_pages.mjs＋spec/ps4.mjs）；程式碼區塊來自 examples/ps4/microbit/*.ts，改程式請改 .ts 後重跑產生器 -->
# 🟧 micro:bit 範例：掌控者 PS4/PS5

[🔙 回到掌控者 PS4/PS5 micro:bit 頁](/sensors/ps4/microbit.md)　｜　[📖 產品頁](/sensors/ps4/index.md)　｜　[📋 指令參考](/sensors/ps4/reference.md)

這一頁是寫給**完全沒寫過程式**的人。5 個範例從「LED 顯示讀值」開始，一步一步做到「開車」。全部用 **MakeCode 網頁版**＋MBC 擴充 **pxt-mbc-sensors v2** 寫：切到 JavaScript 把範例貼上就能用，也可以切回積木模式看它變成哪些積木。

> [!IMPORTANT]
> - 擴充 **pxt-mbc-sensors v2** 還沒上機驗證。本頁的範例都在電腦上用 MakeCode CLI 編譯通過（V1＋V2 各一次），**數值方向、就緒時間、供電**等實機結果見下方「硬體關卡」的「上機待測」。
> - 請用 **通用 I2C 版**的產品。SPIKE 版、EV3 版韌體不同，接 micro:bit 讀不到。

> [!TIP]
> 手把要先和接收器**配對**（第一次開機自動開放 30 秒，PS4 長按 SHARE＋PS、PS5 長按 建立＋PS）；接收器綠燈**熄掉**才是連上。詳細步驟看 [產品頁](/sensors/ps4/index.md)。接收器要是**通用 I2C 版**韌體。

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
| 供電 | 接收器是 ESP32（藍牙），耗電超過 100 mA，**一定要外接 5 V**，不能從 micro:bit 3V 腳取電 | 上機待測 |
| I2C 上拉 | micro:bit 板上已有 4.7 kΩ；產品板若也有上拉，並聯後要確認低電位 ≤ 0.3×VDD | 上機待測 |
| 時脈延展 | ESP32 不能時脈延展：擴充的讀值積木寫完編號會先等 2 ms 再讀，不用自己處理 | 上機待測 |
| 位址衝突 | 出廠位址 0x42，和掌控者 PS2 一樣；兩個接收器同時接要先用「裝置」分類的「改成位址」積木換掉一個 | — |
| 就緒時間 | 接收器開機約 1 秒就緒（F3 b0）；手把連上後 F3 b1 = 1，範例用「手把已連線？」判斷 | 上機待測 |

讀不到值時先看：接線（SDA＝P20、SCL＝P19）→ 外接電源有沒有開 → 擴充「裝置」分類的「位址 … 的裝置就緒？」積木是不是 true。

### 接線（所有範例共用）

<figure class="ref-fig"><img src="/images/figures/ps4/wiring-microbit.svg" alt="掌控者 PS4/PS5 × micro:bit" loading="lazy"></figure>

| 感應器 | micro:bit |
| :--- | :--- |
| SDA | P20 |
| SCL | P19 |
| VCC | 外接 5 V（必要） |
| GND | GND（伺服電池負極也接這裡） |

## 這頁會用到的數值

搖桿每一軸 **−100～+100**，放開是 0，**右、上為正**。按鍵直接用下拉選單選名稱。

| 積木（繁中） | JavaScript | 範圍 | 意思 | 用在 |
| :--- | :--- | :--- | :--- | :--- |
| 掌控者 搖桿 左搖桿 X／Y、右搖桿 X／Y | `mbc.padStick(MbcPadAxis.LeftY)` | −100～+100 | 0 = 中間，右／上 = 正 | 1, 5 |
| 掌控者 按鈕 … 被按住？ | `mbc.padButtonPressed(MbcPadButton.Up)` | true / false | 16 個鍵都可以選：上下左右、△○✕□、L1 R1 L2 R2、L3 R3、SELECT／SHARE、START／OPTIONS | 4 |
| 當掌控者 按鈕 … 按下／放開 | `mbc.onPadButton(MbcPadButton.Cross, MbcPadButtonEvent.Pressed, ...)` | 事件 | 按下或放開的那一瞬間執行一次 | 2 |
| 掌控者 扳機 L2／R2 類比值 | `mbc.padTrigger(MbcPadTrigger.R2)` | 0～255 | 扳機壓多深（PS2 數位手把只有 0 或 255） | 3 |
| 掌控者 手把已連線？ | `mbc.padConnected()` | true / false | 沒連線時按鍵全 0、搖桿回中 | 1, 2, 4, 5 |
| 掌控者 電量（%） | `mbc.padBattery()` | 0～100 | 手把電量（PS2 手把恆 0） | 5 |
| 掌控者 震動 強度 … | `mbc.padRumble(200)` | 0～255（命令 E5） | 震 200 ms，0 = 停（PS2 忽略） | 2 |
| 掌控者 燈條顏色 … | `mbc.padLight(0xFF0000)` | RGB（命令 E6） | PS4／PS5 手把燈條顏色（PS2 忽略） | 2, 5 |
| 等位址 … 的裝置就緒 | `mbc.waitReady(mbc.ADDR_PAD, 3000)` | true / false | 最多等 3 秒，接收器有回應就 true（進階 → 裝置） | 1 |

---

## 範例 1：搖桿點點

**這個範例會做什麼**：推左搖桿，LED 上的亮點跟著跑；手把沒連線畫 X。按 A 顯示左搖桿 X 的數字、按 B 顯示 Y。用來確認接收器接好、手把連上、方向正確。

**要準備什麼**
- micro:bit V1 或 V2＋USB 線
- 掌控者 PS4/PS5 接收器（通用 I2C 版）＋4P 線＋已配對的 PS4 或 PS5 手把
- 外接 5 V 電源（行動電源或電池盒）

**怎麼接**：照上面的接線表：SDA → P20、SCL → P19、VCC → 外接 5 V、GND → GND（外接電源的負極也要接 micro:bit GND）。

**下載程式**：[ex01_stick_dot.ts](/examples/ps4/microbit/ex01_stick_dot.ts)（右鍵另存）→ MakeCode 新專案 → 齒輪 → 擴充 → 貼 `https://github.com/contactmbcrobot/pxt-mbc-sensors` → 右上切到 **JavaScript** → 全選刪掉原本的內容、把檔案內容貼上 → **下載**，把 .hex 拖進 MICROBIT 磁碟。

**程式**

```typescript
// 掌控者 PS4／PS5 × micro:bit 範例 1：搖桿點點（LED 上的點跟著左搖桿動）
// 做什麼：推左搖桿，LED 上的亮點跟著跑；手把沒連線畫 X。按 A 顯示左搖桿 X 的數字、按 B 顯示 Y。
// 接線：接收器 SDA → P20、SCL → P19、VCC → 外接 5 V（ESP32 接收器耗電大，不要從 micro:bit 3V 取電）、GND → GND 共地。
// 準備：手把先跟接收器配對好（看產品頁）。MakeCode 新專案 → 擴充 → 貼 https://github.com/contactmbcrobot/pxt-mbc-sensors → 切到 JavaScript 把本檔全部貼上 → 下載到 micro:bit。
// 看到什麼：開機打勾（找到接收器）；手把沒開 → X；放開搖桿 → 點在正中間；推到右上 → 點跑到右上角。

let busy = false

if (mbc.waitReady(mbc.ADDR_PAD, 3000)) {
    basic.showIcon(IconNames.Yes)
} else {
    basic.showIcon(IconNames.No)                          // 沒回應：檢查 P19／P20 和外接電源
}

basic.forever(function () {
    if (busy) {
        basic.pause(100)
        return
    }
    if (!mbc.padConnected()) {                            // 手把沒連上
        basic.showIcon(IconNames.No, 0)
        basic.pause(200)
        return
    }
    let sx = mbc.padStick(MbcPadAxis.LeftX)               // −100（左）~ +100（右），0 = 中間
    let sy = mbc.padStick(MbcPadAxis.LeftY)               // −100（下）~ +100（上）
    let x = Math.round(Math.map(sx, -100, 100, 0, 4))
    let y = Math.round(Math.map(sy, -100, 100, 4, 0))     // 上 = 正 → 第 0 列
    basic.clearScreen()
    led.plot(x, y)
    basic.pause(50)
})

input.onButtonPressed(Button.A, function () {
    busy = true
    basic.showNumber(mbc.padStick(MbcPadAxis.LeftX))
    busy = false
})
input.onButtonPressed(Button.B, function () {
    busy = true
    basic.showNumber(mbc.padStick(MbcPadAxis.LeftY))
    busy = false
})
```

**一步一步看懂程式**
1. `mbc.waitReady(mbc.ADDR_PAD, 3000)`：接收器有回應打勾，沒有打 X。
2. `forever`：`!mbc.padConnected()` 手把沒連上 → X，等 0.2 秒再看。
3. `sx = mbc.padStick(LeftX)`、`sy = mbc.padStick(LeftY)`：−100～+100。
4. `x = Math.map(sx, −100, 100, 0, 4)`；`y = Math.map(sy, −100, 100, 4, 0)`：上是正，所以 Y 要倒過來（第 0 列在上面）。
5. `basic.clearScreen()` 再 `led.plot(x, y)` 畫一個點。
6. A／B：`basic.showNumber` 顯示數字，`busy` 讓點先暫停。

**跑起來應該看到什麼**
- 開機：打勾。手把沒開：X。
- 按手把 PS 鍵連上：點在正中間。
- 左搖桿推到右上：點跑到右上角；按 A 顯示 100、按 B 顯示 100。
- 往左推一半：點在左邊第二行；A 顯示約 −50。

**常見問題**
- **一直 X？** → 手把沒連上：接收器綠燈快閃 = 等配對，一直亮 = 按手把 PS 鍵重連，熄掉才是連上。
- **點只會在中間附近動？** → 正常，搖桿推到底才會到角落；想放大改 `Math.map(sx, −60, 60, 0, 4)`。
- **開機就是 X（找不到感應器）？** → 看接線 SDA＝P20、SCL＝P19 有沒有接反；外接電源有沒有開；用鱷魚夾接的話夾子有沒有碰到旁邊的腳。
- **模擬器裡數字都是 0？** → 正常。MakeCode 模擬器沒有真的 I2C，讀到的都是 0；要看真的數值一定要下載到 micro:bit。
- **V1 沒有聲音？** → V1 沒有喇叭，在 P0 和 GND 之間接蜂鳴器或耳機。

---

## 範例 2：按鍵變圖案和聲音（燈條、震動）

**這個範例會做什麼**：△ ○ ✕ □ 各畫一個圖案＋一個音，方向鍵畫箭頭；按 ✕ 手把震動、燈條跟著按鍵換顏色（跟手把上符號的顏色一樣）。全部用「當掌控者按鈕 … 按下」事件積木寫。

**要準備什麼**
- 範例 1 的東西
- V1 另外要蜂鳴器或耳機接 P0

**怎麼接**：同範例 1。

**下載程式**：[ex02_button_sound.ts](/examples/ps4/microbit/ex02_button_sound.ts)（右鍵另存）→ MakeCode 新專案 → 齒輪 → 擴充 → 貼 `https://github.com/contactmbcrobot/pxt-mbc-sensors` → 右上切到 **JavaScript** → 全選刪掉原本的內容、把檔案內容貼上 → **下載**，把 .hex 拖進 MICROBIT 磁碟。

**程式**

```typescript
// 掌控者 PS4／PS5 × micro:bit 範例 2：按鍵變圖案和聲音（燈條、震動）
// 做什麼：△ ○ ✕ □ 各畫一個圖案＋一個音；方向鍵畫箭頭；按 ✕ 手把震動、燈條跟著按鍵換顏色。
// 接線：接收器 SDA → P20、SCL → P19、VCC → 外接 5 V、GND → GND 共地。V1 要在 P0 接蜂鳴器或耳機才有聲音（V2 有內建喇叭）。
// 準備：手把配對好；擴充與貼程式的方法同範例 1。
// 看到什麼：按 △ → 三角形＋Do；○ → 靶心＋Mi；✕ → X＋Sol、手把震一下；□ → 方形＋高音 Do；方向鍵 → 四個方向的箭頭。

mbc.onPadButton(MbcPadButton.Triangle, MbcPadButtonEvent.Pressed, function () {
    basic.showIcon(IconNames.Triangle, 0)
    mbc.padLight(0x00FF00)                                // 燈條綠色（跟手把上的 △ 一樣）
    music.playTone(Note.C, 200)
})
mbc.onPadButton(MbcPadButton.Circle, MbcPadButtonEvent.Pressed, function () {
    basic.showIcon(IconNames.Target, 0)
    mbc.padLight(0xFF0000)                                // 紅色
    music.playTone(Note.E, 200)
})
mbc.onPadButton(MbcPadButton.Cross, MbcPadButtonEvent.Pressed, function () {
    basic.showIcon(IconNames.No, 0)
    mbc.padLight(0x0000FF)                                // 藍色
    mbc.padRumble(200)                                    // 震動（0~255）
    music.playTone(Note.G, 200)
})
mbc.onPadButton(MbcPadButton.Cross, MbcPadButtonEvent.Released, function () {
    mbc.padRumble(0)                                      // 放開就停止震動
    basic.clearScreen()
})
mbc.onPadButton(MbcPadButton.Square, MbcPadButtonEvent.Pressed, function () {
    basic.showIcon(IconNames.Square, 0)
    mbc.padLight(0xFF00FF)                                // 粉紅
    music.playTone(Note.C5, 200)
})
mbc.onPadButton(MbcPadButton.Up, MbcPadButtonEvent.Pressed, function () {
    basic.showArrow(ArrowNames.North, 0)
})
mbc.onPadButton(MbcPadButton.Down, MbcPadButtonEvent.Pressed, function () {
    basic.showArrow(ArrowNames.South, 0)
})
mbc.onPadButton(MbcPadButton.Left, MbcPadButtonEvent.Pressed, function () {
    basic.showArrow(ArrowNames.West, 0)
})
mbc.onPadButton(MbcPadButton.Right, MbcPadButtonEvent.Pressed, function () {
    basic.showArrow(ArrowNames.East, 0)
})

basic.forever(function () {
    if (!mbc.padConnected()) {                            // 手把沒連線就一直顯示小愛心提醒
        basic.showIcon(IconNames.SmallHeart, 0)
    }
    basic.pause(500)
})
```

**一步一步看懂程式**
1. `mbc.onPadButton(MbcPadButton.Triangle, Pressed, ...)`：△ 按下那一瞬間執行一次 → 三角形圖案、燈條綠（`padLight(0x00FF00)`）、`Note.C`。
2. ○ → 靶心、紅、`Note.E`；□ → 方形、粉紅、`Note.C5`。
3. ✕ 按下 → X 圖案、燈條藍、`padRumble(200)` 震動、`Note.G`；✕ **放開**另外一個事件 → `padRumble(0)` 停震、清螢幕。
4. 上下左右各一個事件 → 四個箭頭。
5. `forever`：手把沒連線時每 0.5 秒畫小愛心提醒。

**跑起來應該看到什麼**
- 按 △：三角形、燈條變綠、Do。
- 按住 ✕：X、燈條藍、手把一直震；放開：停震、螢幕清掉。
- 按方向鍵：箭頭。

**常見問題**
- **按住 ✕ 只震一下？** → 震動命令每次 200 ms，想一直震要在 Pressed 事件裡用迴圈每 150 ms 送一次，直到放開。
- **燈條沒變色？** → 燈條是 PS4／PS5 手把才有；PS2 手把沒有。確認手把真的連上（綠燈熄）。

---

## 範例 3：扳機控制伺服

**這個範例會做什麼**：R2 壓多深，伺服就往右轉多少（到 180°）；L2 往左（到 0°）；都放開回 90°。LED 長條顯示 R2 的壓力。按 A 顯示目前角度。

**要準備什麼**
- 範例 1 的東西
- 9g 伺服（SG90）×1
- 4.8～6 V 電池盒＋杜邦線

**怎麼接**：接收器同範例 1。伺服 **橘（訊號）→ P1**、紅 → 外接 4.8～6 V 電池正極、棕 → 電池負極，電池負極再接一條到 micro:bit GND（共地）。

**下載程式**：[ex03_trigger_servo.ts](/examples/ps4/microbit/ex03_trigger_servo.ts)（右鍵另存）→ MakeCode 新專案 → 齒輪 → 擴充 → 貼 `https://github.com/contactmbcrobot/pxt-mbc-sensors` → 右上切到 **JavaScript** → 全選刪掉原本的內容、把檔案內容貼上 → **下載**，把 .hex 拖進 MICROBIT 磁碟。

**程式**

```typescript
// 掌控者 PS4／PS5 × micro:bit 範例 3：扳機控制伺服
// 做什麼：R2 壓多深，伺服就往右轉多少；L2 往左；兩邊都放開回正中。LED 長條顯示 R2 的壓力。按 A 顯示目前角度。
// 接線：接收器 SDA → P20、SCL → P19、VCC → 外接 5 V、GND → GND。伺服 橘（訊號）→ P1、紅 → 外接 4.8~6 V 電池正極、棕 → 電池負極並接 micro:bit GND。
// 準備：9g 伺服（SG90）。
// 看到什麼：慢慢壓 R2 → 長條變長、伺服慢慢轉到 180°；壓 L2 → 轉到 0°；放開 → 90°。

let angle = 90

basic.forever(function () {
    let r2 = mbc.padTrigger(MbcPadTrigger.R2)             // 0（放開）~ 255（壓到底）
    let l2 = mbc.padTrigger(MbcPadTrigger.L2)
    angle = Math.round(90 + (r2 - l2) * 90 / 255)         // R2 → 90~180、L2 → 90~0
    pins.servoWritePin(AnalogPin.P1, angle)
    led.plotBarGraph(r2, 255)
    basic.pause(30)
})

input.onButtonPressed(Button.A, function () {
    basic.showNumber(angle)
})
```

**一步一步看懂程式**
1. `forever`：`r2 = mbc.padTrigger(R2)`、`l2 = mbc.padTrigger(L2)`，各 0～255。
2. `angle = 90 + (r2 − l2) × 90 / 255`：只壓 R2 → 90～180；只壓 L2 → 90～0；`Math.round` 取整數。
3. `pins.servoWritePin(AnalogPin.P1, angle)` 每 30 ms 更新。
4. `led.plotBarGraph(r2, 255)`：長條顯示 R2 壓力。

**跑起來應該看到什麼**
- 慢慢壓 R2：長條變長、伺服慢慢轉到 180°。
- 壓 L2：轉到 0°。兩個都壓到底：回 90°。
- 放開：90°，長條全暗。

**常見問題**
- **PS2 手把一壓就跳到底？** → PS2 數位手把的 L2／R2 只有 0 或 255，沒有壓力值；要漸變改用右搖桿：`angle = 90 + padStick(RightX) × 0.9`。
- **伺服抖？** → 伺服要外接電池、共地。

---

## 範例 4：方向鍵開車

**這個範例會做什麼**：按住方向鍵車子就往那個方向走，放開就停：上 = 前進、下 = 後退、左 = 原地左轉、右 = 原地右轉。手把斷線車子自動停。

**要準備什麼**
- 兩輪小車：兩顆連續旋轉伺服（FS90R 之類）當輪子＋一顆萬向輪
- 接收器固定在車上
- 4.8～6 V 電池盒（伺服用）＋外接 5 V（接收器用，可以是同一個行動電源）

**怎麼接**：接收器同範例 1。**左輪連續旋轉伺服訊號 → P1、右輪 → P2**，兩顆伺服的紅線接 4.8～6 V 電池盒正極、棕線接電池負極，電池負極再接 micro:bit GND。

**下載程式**：[ex04_dpad_drive.ts](/examples/ps4/microbit/ex04_dpad_drive.ts)（右鍵另存）→ MakeCode 新專案 → 齒輪 → 擴充 → 貼 `https://github.com/contactmbcrobot/pxt-mbc-sensors` → 右上切到 **JavaScript** → 全選刪掉原本的內容、把檔案內容貼上 → **下載**，把 .hex 拖進 MICROBIT 磁碟。

**程式**

```typescript
// 掌控者 PS4／PS5 × micro:bit 範例 4：方向鍵開車
// 做什麼：按住方向鍵車子就往那個方向走，放開就停：上 = 前進、下 = 後退、左 = 原地左轉、右 = 原地右轉。手把斷線車子自動停。
// 接線：接收器 SDA → P20、SCL → P19、VCC → 外接 5 V、GND → GND。左輪連續旋轉伺服訊號 → P1、右輪 → P2；伺服電源 4.8~6 V 電池盒，電池負極接 micro:bit GND。
// 準備：兩輪小車（兩顆連續旋轉伺服當輪子）。
// 看到什麼：按住上 → 車子前進、LED 朝上箭頭；放開 → 停；手把關掉 → X、車停。

// 左輪伺服 P1、右輪伺服 P2（連續旋轉伺服）；left／right = −100~100，正 = 前進
function drive(left: number, right: number): void {
    left = Math.constrain(left, -100, 100)
    right = Math.constrain(right, -100, 100)
    pins.servoWritePin(AnalogPin.P1, Math.round(90 + left * 0.9))    // 90 = 停、180 = 全速前進、0 = 全速後退
    pins.servoWritePin(AnalogPin.P2, Math.round(90 - right * 0.9))   // 右輪裝的方向相反，所以用減的
}

basic.forever(function () {
    if (!mbc.padConnected()) {                            // 斷線：停車
        drive(0, 0)
        basic.showIcon(IconNames.No, 0)
    } else if (mbc.padButtonPressed(MbcPadButton.Up)) {
        drive(50, 50)                                     // 前進
        basic.showArrow(ArrowNames.North, 0)
    } else if (mbc.padButtonPressed(MbcPadButton.Down)) {
        drive(-50, -50)                                   // 後退
        basic.showArrow(ArrowNames.South, 0)
    } else if (mbc.padButtonPressed(MbcPadButton.Left)) {
        drive(-30, 30)                                    // 左輪往後、右輪往前 = 原地左轉
        basic.showArrow(ArrowNames.West, 0)
    } else if (mbc.padButtonPressed(MbcPadButton.Right)) {
        drive(30, -30)                                    // 原地右轉
        basic.showArrow(ArrowNames.East, 0)
    } else {
        drive(0, 0)                                       // 都沒按：停
        basic.clearScreen()
    }
    basic.pause(50)
})
```

**一步一步看懂程式**
1. `function drive(left, right)`：左輪 P1、右輪 P2，−100～100，正 = 前進；右輪裝反所以用減的。
2. `forever`：先看 `!mbc.padConnected()` → 斷線就 `drive(0, 0)`、X。
3. `mbc.padButtonPressed(Up)` → `drive(50, 50)` 前進、上箭頭；`Down` → `drive(−50, −50)`。
4. `Left` → `drive(−30, 30)`：左輪往後、右輪往前 = 原地左轉；`Right` 相反。
5. 都沒按 → `drive(0, 0)` 停、清螢幕。每 50 ms 一輪。

**跑起來應該看到什麼**
- 按住上：前進、上箭頭；放開：停。
- 按住左：原地左轉。
- 關掉手把：X、車停。

**常見問題**
- **按上卻往後？** → 兩顆伺服都裝反：`drive` 裡 `+ left` 改 `- left`、`- right` 改 `+ right`。只有一輪反 → 只改那一行。
- **按上卻原地轉？** → 只有一邊反了，只改那一顆的正負號。
- **放開還慢慢動？** → 連續旋轉伺服的停止點不一定是 90，調伺服的微調螺絲或把 90 改 88～92。

---

## 範例 5：雙搖桿坦克＋安全啟動（綜合）

**這個範例會做什麼**：左搖桿管左輪、右搖桿管右輪，推越多越快，一前一後原地轉。一開始要兩支搖桿都放開（在中間）才會「解鎖」，斷線就鎖回去，車子不會一開機就衝出去。按 A 顯示手把電量；電量低於 20% 燈條變紅。

**要準備什麼**
- 範例 4 的小車

**怎麼接**：同範例 4。

**下載程式**：[ex05_tank_drive.ts](/examples/ps4/microbit/ex05_tank_drive.ts)（右鍵另存）→ MakeCode 新專案 → 齒輪 → 擴充 → 貼 `https://github.com/contactmbcrobot/pxt-mbc-sensors` → 右上切到 **JavaScript** → 全選刪掉原本的內容、把檔案內容貼上 → **下載**，把 .hex 拖進 MICROBIT 磁碟。

**程式**

```typescript
// 掌控者 PS4／PS5 × micro:bit 範例 5：雙搖桿坦克＋安全啟動（綜合）
// 做什麼：左搖桿管左輪、右搖桿管右輪，推越多越快。一開始要兩支搖桿都放開（在中間）才會「解鎖」開車；斷線就鎖回去。按 A 顯示手把電量；電量低於 20% 燈條變紅。
// 接線：同範例 4（接收器 P19／P20、外接 5 V；左輪 P1、右輪 P2、伺服外接電池、共地）。
// 準備：兩輪小車。
// 看到什麼：放開兩支搖桿 → 打勾（解鎖）、燈條綠；兩支都往前推 → 前進；一前一後 → 原地轉；放開 → 停；關手把 → X 並鎖住。

const DEAD = 15                                           // 死區：搖桿在 ±15 以內當成 0
let armed = false
let lowBattery = false

// 左輪伺服 P1、右輪伺服 P2（連續旋轉伺服）；left／right = −100~100，正 = 前進
function drive(left: number, right: number): void {
    left = Math.constrain(left, -100, 100)
    right = Math.constrain(right, -100, 100)
    pins.servoWritePin(AnalogPin.P1, Math.round(90 + left * 0.9))    // 90 = 停、180 = 全速前進、0 = 全速後退
    pins.servoWritePin(AnalogPin.P2, Math.round(90 - right * 0.9))   // 右輪裝的方向相反，所以用減的
}

function deadZone(v: number): number {                    // 搖桿在中間附近就當 0，放開手車子才會停
    if (Math.abs(v) < DEAD) {
        return 0
    }
    return v
}

input.onButtonPressed(Button.A, function () {
    basic.showNumber(mbc.padBattery())                    // 手把電量 0~100（PS2 手把恆 0）
})

basic.forever(function () {
    if (!mbc.padConnected()) {
        drive(0, 0)
        armed = false                                     // 斷線後要重新解鎖
        basic.showIcon(IconNames.No, 0)
        basic.pause(200)
        return
    }
    let ly = deadZone(mbc.padStick(MbcPadAxis.LeftY))     // 左搖桿上下 −100~+100
    let ry = deadZone(mbc.padStick(MbcPadAxis.RightY))    // 右搖桿上下
    if (!armed) {
        if (ly == 0 && ry == 0) {                         // 兩支都放開才解鎖
            armed = true
            basic.showIcon(IconNames.Yes, 0)
            mbc.padLight(0x00FF00)                        // 燈條綠色 = 可以開了
        }
        basic.pause(50)
        return
    }
    drive(ly, ry)                                         // 左搖桿 → 左輪、右搖桿 → 右輪
    let low = mbc.padBattery() < 20
    if (low != lowBattery) {                              // 電量狀態變了才改燈條
        lowBattery = low
        mbc.padLight(low ? 0xFF0000 : 0x00FF00)           // 低電量紅、正常綠
    }
    basic.pause(20)
})
```

**一步一步看懂程式**
1. `DEAD = 15`：死區。`deadZone(v)`：搖桿在 ±15 以內就回傳 0，放開手車子才會真的停。
2. `forever`：斷線 → 停車、`armed = false`、X。
3. `ly`、`ry` = 左右搖桿上下，經過死區。
4. `!armed`：兩支都是 0 才 `armed = true`、打勾、燈條綠；否則什麼都不做。
5. 解鎖後 `drive(ly, ry)`：左搖桿直接給左輪、右搖桿給右輪（−100～100 剛好是 drive 的範圍）。
6. `padBattery() < 20` 狀態改變時換燈條紅／綠；按 A 顯示電量。

**跑起來應該看到什麼**
- 開機、手把連上：兩支搖桿放開 → 打勾、燈條綠。
- 兩支都往前推：前進，推越多越快；左前右後：原地右轉；放開：停。
- 按 A：顯示電量，例如 85。關手把：X，再開要重新放開搖桿才解鎖。

**常見問題**
- **一直沒打勾？** → 搖桿沒回中（有偏移）。把 `DEAD` 改 25。
- **太快不好控制？** → `drive(ly, ry)` 改 `drive(ly / 2, ry / 2)`。
- **想改成一支搖桿開車？** → `drive(ly + lx, ly − lx)`，`lx` 用 `padStick(LeftX)`。

---

## 下一步

- 全部積木、數值和硬體說明：[掌控者 PS4/PS5 × micro:bit](/sensors/ps4/microbit.md)
- 每個數值編號的意思：[指令參考](/sensors/ps4/reference.md)
- 擴充原始碼與回報問題：<https://github.com/contactmbcrobot/pxt-mbc-sensors>
