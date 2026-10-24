<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 W4 micro:bit 範例產生器產生（scratchpad w4/gen_pages.mjs＋spec/esp32cam.mjs）；程式碼區塊來自 examples/esp32cam/microbit/*.ts，改程式請改 .ts 後重跑產生器 -->
<!-- moved: /sensors/esp32cam/microbit/makecode.md -->
<p class="moved-note">這頁內容已整合到新的說明頁：<a href="/sensors/esp32cam/microbit/makecode.md">神攝手 ESP32CAM › micro:bit › MakeCode</a></p>

# 🟧 micro:bit 範例：神攝手 ESP32CAM

[🔙 回到神攝手 ESP32CAM micro:bit 頁](/sensors/esp32cam/microbit.md)　｜　[📖 產品頁](/sensors/esp32cam/index.md)　｜　[📋 指令參考](/sensors/esp32cam/reference.md)

這一頁是寫給**完全沒寫過程式**的人。6 個範例從「LED 顯示讀值」開始，一步一步做到「開車」。全部用 **MakeCode 網頁版**＋MBC 擴充 **pxt-mbc-sensors v2** 寫：切到 JavaScript 把範例貼上就能用，也可以切回積木模式看它變成哪些積木。

> [!IMPORTANT]
> - 擴充 **pxt-mbc-sensors v2** 還沒上機驗證。本頁的範例都在電腦上用 MakeCode CLI 編譯通過（V1＋V2 各一次），**數值方向、就緒時間、供電**等實機結果見下方「硬體關卡」的「上機待測」。
> - 請用 **通用 I2C 版**的產品。SPIKE 版、EV3 版韌體不同，接 micro:bit 讀不到。

> [!TIP]
> 範例 1～5 是**學校版**（色相／色塊／動態），範例 6 是 **WRO 版（HUE12，12 個色點）**。要看哪個顏色、追哪個色塊**只能在相機機身上用按鍵設定**（程式改不了）：範例 1、2 要先做「設定 1：COLOR」，範例 3～5 要先做「設定 2：TRACK」，步驟看 [產品頁](/sensors/esp32cam/index.md)。

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
| 供電 | ESP32 相機峰值超過 300 mA，**一定要外接 5 V**（行動電源或 2 A 以上的變壓器），不能從 micro:bit 3V 腳取電 | 上機待測 |
| I2C 上拉 | micro:bit 板上已有 4.7 kΩ；產品板若也有上拉，並聯後要確認低電位 ≤ 0.3×VDD | 上機待測 |
| 時脈延展 | ESP32 不能時脈延展：擴充的讀值積木寫完編號會先等 2 ms 再讀，不用自己處理 | 上機待測 |
| 位址衝突 | 出廠位址 0x4C（學校版、WRO 版相同）；I2C 腳是 IO3（SDA）／IO1（SCL），接了 I2C 之後相機的 USB 序列監看就不能用 | — |
| 就緒時間 | 相機開機約 3 秒就緒（F3 b0）；**相機先上電、再開 micro:bit**比較穩。I2C 介面 2026-10-21 定案，硬體已確認、上機待測 | 上機待測 |

讀不到值時先看：接線（SDA＝P20、SCL＝P19）→ 外接電源有沒有開 → 擴充「裝置」分類的「位址 … 的裝置就緒？」積木是不是 true。

### 接線（所有範例共用）

<figure class="ref-fig"><img src="/images/figures/esp32cam/wiring-microbit.svg" alt="神攝手 ESP32CAM × micro:bit" loading="lazy"></figure>

| 感應器 | micro:bit |
| :--- | :--- |
| SDA | P20 |
| SCL | P19 |
| VCC | 外接 5 V（必要） |
| GND | GND（伺服電池負極也接這裡） |

## 這頁會用到的數值

**色相** 0～100：紅 0（100 也是紅）、黃約 17、綠約 33、青約 50、藍約 67、洋紅約 83；**−1 = 黑、白、灰**。色塊 **X** 0～100（左 0、右 100）、**Y** 0～75（下 0、上 75）、**面積** = 寬 × 高（0～7500，0 = 沒找到）。

| 積木（繁中） | JavaScript | 範圍 | 意思 | 用在 |
| :--- | :--- | :--- | :--- | :--- |
| 神攝手 顏色 色相／飽和度／明度 | `mbc.camColor(MbcCamColor.Hue)` | −1～100 / 0～100 | 鏡頭中央取樣點的顏色 | 1, 2 |
| 神攝手 顏色是黑白灰？ | `mbc.camIsGrey()` | true / false | 色相 = −1 | 1, 2 |
| 神攝手 有找到色塊？ | `mbc.camFound()` | true / false | 畫面裡有目標色的色塊 | 3, 4, 5 |
| 神攝手 色塊 X／Y／寬／高／面積 | `mbc.camBlob(MbcCamBlob.X)` | 0～100 / 0～75 / 0～7500 | 色塊的位置和大小 | 3, 4, 5 |
| 當神攝手 色塊 找到／不見 | `mbc.onCamBlob(MbcCamEvent.Found, ...)` | 事件 | 色塊出現或消失的那一瞬間執行一次 | 5 |
| 神攝手 動態 | `mbc.camMotion()` | 0～100 | 整個畫面跟上一張比變了多少 | 5 |
| 神攝手 色點 n 顏色碼（WRO 版） | `mbc.camSpotColor(1)` | 0～255 | 0～239 彩色、240～245 白、246～250 黑、255 = 色點沒開 | 6 |
| 神攝手 色點 n 是 彩色／白／黑／未啟用？（WRO 版） | `mbc.camSpotIs(1, MbcCamSpotKind.Colour)` | true / false | 把顏色碼分成四類 | 6 |
| 神攝手 色點 n 鮮豔度（WRO 版） | `mbc.camSpotVivid(1)` | 0～255 | 飽和度，黑 = 1、色點沒開 = 0 | 6 |
| 等位址 … 的裝置就緒 | `mbc.waitReady(mbc.ADDR_CAM, 5000)` | true / false | 最多等 5 秒，相機開好才 true（進階 → 裝置） | 1, 6 |

---

## 範例 1：看到什麼顏色

**這個範例會做什麼**：LED 長條顯示鏡頭中央看到的色相：紅幾乎全暗（0）、綠亮 1/3、藍亮 2/3；黑白灰畫一個小方塊。按 A 顯示色相數字、按 B 顯示飽和度。

**要準備什麼**
- micro:bit V1 或 V2＋USB 線
- 神攝手 ESP32CAM 學校版（I2C 線接 IO3／IO1）＋外接 5 V 電源
- 紅、綠、藍的積木或色紙
- 相機先做好「設定 1：COLOR」

**怎麼接**：照上面的接線表：相機 IO3（SDA）→ P20、IO1（SCL）→ P19、5V → 外接 5 V、GND → GND（外接電源的負極也要接 micro:bit GND）。

**下載程式**：[ex01_show_hue.ts](/examples/esp32cam/microbit/ex01_show_hue.ts)（右鍵另存）→ MakeCode 新專案 → 齒輪 → 擴充 → 貼 `https://github.com/contactmbcrobot/pxt-mbc-sensors` → 右上切到 **JavaScript** → 全選刪掉原本的內容、把檔案內容貼上 → **下載**，把 .hex 拖進 MICROBIT 磁碟。

**程式**

```typescript
// 神攝手 ESP32CAM（學校版）× micro:bit 範例 1：看到什麼顏色（色相長條）
// 做什麼：LED 長條顯示鏡頭中央看到的色相（紅 0 → 黃 17 → 綠 33 → 藍 67 → 紅 100）；黑白灰畫一個小方塊。按 A 顯示色相數字、按 B 顯示飽和度。
// 接線：相機 SDA（IO3）→ P20、SCL（IO1）→ P19、VCC → 外接 5 V（ESP32 耗電大，不要從 micro:bit 3V 取電）、GND → GND 共地。
// 準備：先在相機機身做「設定 1：COLOR」（看產品頁）。MakeCode 新專案 → 擴充 → 貼 https://github.com/contactmbcrobot/pxt-mbc-sensors → 切到 JavaScript 把本檔全部貼上 → 下載到 micro:bit。
// 看到什麼：開機打勾（找到相機）；紅色積木 → 長條幾乎全暗（0）；綠色 → 亮 1/3；藍色 → 亮 2/3；白紙 → 小方塊。

let busy = false

if (mbc.waitReady(mbc.ADDR_CAM, 5000)) {                  // 相機開機約 3 秒
    basic.showIcon(IconNames.Yes)
} else {
    basic.showIcon(IconNames.No)                          // 沒回應：檢查 P19／P20、外接電源，相機要先接好再開 micro:bit
}

basic.forever(function () {
    if (busy) {
        basic.pause(100)
        return
    }
    if (mbc.camIsGrey()) {                                // 色相 −1：黑、白、灰
        basic.showIcon(IconNames.SmallSquare, 0)
    } else {
        led.plotBarGraph(mbc.camColor(MbcCamColor.Hue), 100)   // 0~100
    }
    basic.pause(100)
})

input.onButtonPressed(Button.A, function () {
    busy = true
    basic.showNumber(mbc.camColor(MbcCamColor.Hue))
    busy = false
})
input.onButtonPressed(Button.B, function () {
    busy = true
    basic.showNumber(mbc.camColor(MbcCamColor.Saturation))   // 0~100，越大顏色越鮮
    busy = false
})
```

**一步一步看懂程式**
1. `mbc.waitReady(mbc.ADDR_CAM, 5000)`：相機開機約 3 秒，等到有回應打勾。
2. `forever`：`mbc.camIsGrey()`（色相 −1）→ 小方塊。
3. 否則 `led.plotBarGraph(mbc.camColor(Hue), 100)`：色相 0～100 畫成長條。
4. A：顯示色相；B：顯示飽和度（0～100，越大越鮮）。`busy` 讓長條先暫停。

**跑起來應該看到什麼**
- 開機 3 秒後打勾。
- 紅積木：長條幾乎全暗，按 A 顯示 0～5 或 95～100。
- 綠積木：約 1/3；藍：約 2/3。
- 白紙：小方塊，按 A 顯示 −1。

**常見問題**
- **色相一直是 0？** → 還沒做「設定 1：COLOR」，0 跟紅色一樣容易誤會。到相機機身設定取樣點。
- **開機是 X？** → 相機沒電或還沒開好：相機先上電等畫面出來，再按 micro:bit 的重置鍵。
- **鏡頭畫面上下顛倒？** → 相機鏡頭選單把 V-Flip 切 ON（見產品頁）。
- **開機就是 X（找不到感應器）？** → 看接線 SDA＝P20、SCL＝P19 有沒有接反；外接電源有沒有開；用鱷魚夾接的話夾子有沒有碰到旁邊的腳。
- **模擬器裡數字都是 0？** → 正常。MakeCode 模擬器沒有真的 I2C，讀到的都是 0；要看真的數值一定要下載到 micro:bit。
- **V1 沒有聲音？** → V1 沒有喇叭，在 P0 和 GND 之間接蜂鳴器或耳機。

---

## 範例 2：看到指定顏色就嗶

**這個範例會做什麼**：LED 顯示要找的顏色（R 紅、Y 黃、G 綠、B 藍）；鏡頭中央看到那個顏色 → 愛心＋嗶。按 A／B 換下一個／上一個顏色。學會「色相是一個圓」的比較方法。

**要準備什麼**
- 範例 1 的東西＋黃色積木
- V1 另外要蜂鳴器或耳機接 P0

**怎麼接**：同範例 1。

**下載程式**：[ex02_color_alarm.ts](/examples/esp32cam/microbit/ex02_color_alarm.ts)（右鍵另存）→ MakeCode 新專案 → 齒輪 → 擴充 → 貼 `https://github.com/contactmbcrobot/pxt-mbc-sensors` → 右上切到 **JavaScript** → 全選刪掉原本的內容、把檔案內容貼上 → **下載**，把 .hex 拖進 MICROBIT 磁碟。

**程式**

```typescript
// 神攝手 ESP32CAM（學校版）× micro:bit 範例 2：看到指定顏色就嗶
// 做什麼：LED 顯示要找的顏色（R 紅、Y 黃、G 綠、B 藍）；鏡頭中央看到那個顏色 → 愛心＋嗶。按 A／B 換下一個／上一個顏色。
// 接線：相機 SDA → P20、SCL → P19、VCC → 外接 5 V、GND → GND 共地。V1 要在 P0 接蜂鳴器或耳機才有聲音（V2 有內建喇叭）。
// 準備：相機做過「設定 1：COLOR」；紅、黃、綠、藍四塊積木。
// 看到什麼：LED 顯示 R，紅積木放到鏡頭前 → 愛心＋嗶；綠積木 → 沒反應；按 A 變成 Y，換黃積木才會響。

const NAMES = ["R", "Y", "G", "B"]
const HUES = [0, 17, 33, 67]                              // 紅、黃、綠、藍的色相（0~100）
const TOL = 8                                             // 差 8 以內算同一色
let sel = 0

// 色相是一個圓：0 和 100 都是紅，所以差距超過 50 要繞另一邊算
function sameHue(h: number, target: number): boolean {
    let d = Math.abs(h - target)
    if (d > 50) {
        d = 100 - d
    }
    return d <= TOL
}

input.onButtonPressed(Button.A, function () {
    sel = (sel + 1) % 4
})
input.onButtonPressed(Button.B, function () {
    sel = (sel + 3) % 4
})

basic.forever(function () {
    let h = mbc.camColor(MbcCamColor.Hue)                 // −1 = 黑白灰
    if (!mbc.camIsGrey() && sameHue(h, HUES[sel])) {
        basic.showIcon(IconNames.Heart, 0)
        music.playTone(Note.E, 150)
    } else {
        basic.showString(NAMES[sel])                      // 單一個字母直接顯示，不捲動
    }
    basic.pause(100)
})
```

**一步一步看懂程式**
1. `NAMES`／`HUES`：四個顏色的字母和色相（紅 0、黃 17、綠 33、藍 67）；`sel` = 目前選第幾個。
2. `sameHue(h, target)`：`d = |h − target|`，超過 50 就 `d = 100 − d`（0 和 100 都是紅，要繞另一邊算），差 8 以內算同色。
3. `forever`：`h = 色相`；不是黑白灰而且 `sameHue` → 愛心＋`Note.E`；否則 `basic.showString(NAMES[sel])`（單一個字母不會捲動）。
4. A：`sel = (sel + 1) % 4`；B：`(sel + 3) % 4` 等於往回一格。

**跑起來應該看到什麼**
- 螢幕 R：紅積木放鏡頭前 → 愛心＋嗶；綠積木 → 沒反應。
- 按 A → Y：換黃積木才響。
- 按 A 到 B（藍）：藍積木響，青色的可能不響（差太多）。

**常見問題**
- **黃色認不出來？** → 燈光偏黃或偏白時色相會跑。按 A 看實際數字，把 `HUES` 裡的 17 改成你量到的值。
- **一直響？** → 背景剛好是那個顏色（例如木頭桌面接近黃）。把 `TOL` 從 8 改 5。

---

## 範例 3：鏡頭雲台追色塊（伺服）

**這個範例會做什麼**：相機裝在伺服上：色塊在畫面左邊 → 伺服往左轉，直到色塊回到畫面中間；沒找到色塊 → 問號臉、伺服不動。按 A 回正中。

**要準備什麼**
- 範例 1 的東西
- 9g 伺服（SG90）×1，相機固定在擺臂上
- 4.8～6 V 電池盒＋杜邦線
- 相機先做好「設定 2：TRACK」（畫面上有追蹤框）

**怎麼接**：相機同範例 1（外接 5 V）。伺服 **橘（訊號）→ P1**、紅 → 外接 4.8～6 V 電池正極、棕 → 電池負極，電池負極再接一條到 micro:bit GND（共地）。相機固定在伺服擺臂上。

**下載程式**：[ex03_pan_servo.ts](/examples/esp32cam/microbit/ex03_pan_servo.ts)（右鍵另存）→ MakeCode 新專案 → 齒輪 → 擴充 → 貼 `https://github.com/contactmbcrobot/pxt-mbc-sensors` → 右上切到 **JavaScript** → 全選刪掉原本的內容、把檔案內容貼上 → **下載**，把 .hex 拖進 MICROBIT 磁碟。

**程式**

```typescript
// 神攝手 ESP32CAM（學校版）× micro:bit 範例 3：鏡頭雲台追色塊（伺服）
// 做什麼：相機裝在伺服上：色塊在畫面左邊 → 伺服往左轉，直到色塊回到畫面中間；沒找到色塊 → 問號臉、伺服不動。按 A 回正中。
// 接線：相機 SDA → P20、SCL → P19、VCC → 外接 5 V、GND → GND。伺服 橘（訊號）→ P1、紅 → 外接 4.8~6 V 電池正極、棕 → 電池負極並接 micro:bit GND。
// 準備：相機做過「設定 2：TRACK」（追蹤框會框住色塊）；9g 伺服（SG90）＋把相機固定在伺服擺臂上。
// 看到什麼：把色塊慢慢往左移，伺服跟著轉過去讓色塊回到中間；拿走色塊 → 問號臉。

const CENTRE = 50                                         // 色塊 X 0~100，50 = 畫面中間
const DEAD = 8                                            // 離中間 8 以內就不動
let angle = 90

basic.forever(function () {
    if (mbc.camFound()) {                                 // 有找到目標色的色塊
        let err = mbc.camBlob(MbcCamBlob.X) - CENTRE      // 正 = 色塊在右邊
        if (Math.abs(err) > DEAD) {
            angle = Math.constrain(angle - err / 10, 0, 180)   // 色塊在右 → 角度變小 = 往右轉（裝反就把減改成加）
            basic.showArrow(err > 0 ? ArrowNames.East : ArrowNames.West, 0)
        } else {
            basic.showIcon(IconNames.Target, 0)           // 對準了
        }
        pins.servoWritePin(AnalogPin.P1, Math.round(angle))
    } else {
        basic.showIcon(IconNames.Confused, 0)
    }
    basic.pause(40)
})

input.onButtonPressed(Button.A, function () {
    angle = 90
    pins.servoWritePin(AnalogPin.P1, 90)
})
```

**一步一步看懂程式**
1. `CENTRE = 50`（色塊 X 的中間）、`DEAD = 8`（離中間 8 以內不動）、`angle = 90`。
2. `forever`：`mbc.camFound()` 有色塊 → `err = 色塊 X − 50`，正 = 色塊在右。
3. `|err| > 8`：`angle = angle − err / 10`（色塊在右 → 角度變小 = 往右轉；裝反就改成加），用 `Math.constrain` 夾在 0～180，箭頭顯示方向；在中間 → 靶心。
4. `pins.servoWritePin(AnalogPin.P1, Math.round(angle))`。
5. 沒色塊 → 問號臉。A：回 90°。

**跑起來應該看到什麼**
- 色塊在畫面中間：靶心、伺服不動。
- 把色塊慢慢往左移：左箭頭、伺服跟著轉，色塊回到中間後停。
- 拿走色塊：問號臉。

**常見問題**
- **伺服越追越遠（反方向）？** → `angle − err / 10` 改 `angle + err / 10`。
- **來回抖？** → `/ 10` 改 `/ 20`（追慢一點），或 `DEAD` 改 12。
- **追蹤框一直跳到別的東西？** → 相機設定的 TOL_HUE 太寬，或 MIN AREA 太小；回相機選單調。

---

## 範例 4：追著色塊跑的小車

**這個範例會做什麼**：色塊在畫面右邊 → 右轉；色塊很小（很遠）→ 前進；很大（很近）→ 後退；沒看到 → 停。A 開始、B 停。用「面積」當距離。

**要準備什麼**
- 兩輪小車：兩顆連續旋轉伺服（FS90R 之類）當輪子＋一顆萬向輪
- 相機固定在車頭朝前（外接 5 V）
- 4.8～6 V 電池盒
- 一顆目標色的球或積木；相機做好「設定 2：TRACK」

**怎麼接**：相機同範例 1（外接 5 V），裝在車頭朝前。**左輪連續旋轉伺服訊號 → P1、右輪 → P2**，兩顆伺服的紅線接 4.8～6 V 電池盒正極、棕線接電池負極，電池負極再接 micro:bit GND。

**下載程式**：[ex04_follow_blob.ts](/examples/esp32cam/microbit/ex04_follow_blob.ts)（右鍵另存）→ MakeCode 新專案 → 齒輪 → 擴充 → 貼 `https://github.com/contactmbcrobot/pxt-mbc-sensors` → 右上切到 **JavaScript** → 全選刪掉原本的內容、把檔案內容貼上 → **下載**，把 .hex 拖進 MICROBIT 磁碟。

**程式**

```typescript
// 神攝手 ESP32CAM（學校版）× micro:bit 範例 4：追著色塊跑的小車
// 做什麼：色塊在畫面右邊 → 右轉；色塊很小（很遠）→ 前進；很大（很近）→ 後退；沒看到 → 停。A 開始、B 停。
// 接線：相機 SDA → P20、SCL → P19、VCC → 外接 5 V、GND → GND。左輪連續旋轉伺服訊號 → P1、右輪 → P2；伺服電源 4.8~6 V 電池盒，電池負極接 micro:bit GND。
// 準備：兩輪小車，相機朝前；相機做過「設定 2：TRACK」；一顆目標色的球或積木。
// 看到什麼：按 A 後把球放在車前面：球往後退車就追上去，球靠近車就倒退，球移到旁邊車就轉過去；球拿走 → 停、問號臉。

const FAR_AREA = 800                                      // 面積比這小 = 太遠 → 前進（依你的球大小調整）
const NEAR_AREA = 2500                                    // 面積比這大 = 太近 → 後退
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
    if (!mbc.camFound()) {
        drive(0, 0)
        basic.showIcon(IconNames.Confused, 0)
    } else {
        let x = mbc.camBlob(MbcCamBlob.X)                 // 0（左）~ 100（右）
        let area = mbc.camBlob(MbcCamBlob.Area)           // 寬 × 高，0~7500：越大越近
        let turn = Math.constrain((x - 50), -40, 40)      // 色塊在右 → 正 → 左輪快 = 右轉
        let speed = 0
        if (area < FAR_AREA) {
            speed = 50                                    // 太遠：前進
        } else if (area > NEAR_AREA) {
            speed = -40                                   // 太近：後退
        }
        drive(speed + turn, speed - turn)
        if (turn > 10) {
            basic.showArrow(ArrowNames.East, 0)
        } else if (turn < -10) {
            basic.showArrow(ArrowNames.West, 0)
        } else if (speed > 0) {
            basic.showArrow(ArrowNames.North, 0)
        } else if (speed < 0) {
            basic.showArrow(ArrowNames.South, 0)
        } else {
            basic.showIcon(IconNames.Target, 0)           // 距離剛好
        }
    }
    basic.pause(40)
})
```

**一步一步看懂程式**
1. `FAR_AREA = 800`、`NEAR_AREA = 2500`：面積（寬 × 高）小於 800 算太遠、大於 2500 算太近，依你的球大小調。
2. `function drive(left, right)`：左輪 P1、右輪 P2。A 開始、B 停。
3. `forever`：`!mbc.camFound()` → 停、問號臉。
4. `x = 色塊 X`、`area = 色塊面積`；`turn = x − 50`（限制 ±40）：色塊在右 → 正 → 左輪快 = 右轉。
5. `speed`：太遠 50、太近 −40、剛好 0；`drive(speed + turn, speed − turn)`。箭頭或靶心顯示在做什麼。

**跑起來應該看到什麼**
- 按 A，球在車前 30 cm：車子前進；球靠近到 10 cm：後退；中間某個距離：停、靶心。
- 球移到右邊：右轉追過去。
- 球拿走：停、問號臉。

**常見問題**
- **不前進也不後退？** → 面積剛好在 800～2500 之間。按相機螢幕上的 A（面積）看實際數字再改兩個門檻。
- **轉向反了？** → `drive(speed + turn, speed − turn)` 改成 `drive(speed − turn, speed + turn)`。
- **車子一動畫面就糊、追蹤框不見？** → 速度 50 改 35，或相機 MIN AREA 調小。

---

## 範例 5：動態偵測警報＋追蹤雲台（綜合）

**這個範例會做什麼**：畫面有動靜（動態 > 30）→ 警報音、次數 +1（2 秒內不重複）；有找到色塊時伺服轉向色塊（同範例 3）；色塊出現／消失用事件畫打勾／X。A 清零、B 開關警報。

**要準備什麼**
- 範例 3 的雲台（相機在伺服上）
- V1 另外要蜂鳴器或耳機接 P0

**怎麼接**：同範例 3。

**下載程式**：[ex05_motion_guard.ts](/examples/esp32cam/microbit/ex05_motion_guard.ts)（右鍵另存）→ MakeCode 新專案 → 齒輪 → 擴充 → 貼 `https://github.com/contactmbcrobot/pxt-mbc-sensors` → 右上切到 **JavaScript** → 全選刪掉原本的內容、把檔案內容貼上 → **下載**，把 .hex 拖進 MICROBIT 磁碟。

**程式**

```typescript
// 神攝手 ESP32CAM（學校版）× micro:bit 範例 5：動態偵測警報＋追蹤雲台（綜合）
// 做什麼：畫面有動靜（動態 > 30）→ 警報音、次數 +1（2 秒內不重複）；有找到色塊時伺服轉向色塊（同範例 3）；色塊出現／消失畫打勾／X。A 清零、B 開關警報。
// 接線：相機 SDA → P20、SCL → P19、VCC → 外接 5 V、GND → GND。伺服訊號 → P1、伺服電源外接 4.8~6 V、共地。V1 聲音要在 P0 接蜂鳴器。
// 準備：相機做過「設定 2：TRACK」；相機固定在伺服擺臂上。
// 看到什麼：有人從鏡頭前走過 → 嗶、數字 +1；拿目標色的積木進來 → 打勾、伺服轉過去跟著它；按 B → M（靜音）。

const MOTION_LEVEL = 30                                   // 動態 0~100，超過就算有人
let armed = true
let alarms = 0
let lastAlarm = -5000
let angle = 90

mbc.onCamBlob(MbcCamEvent.Found, function () {            // 色塊出現
    basic.showIcon(IconNames.Yes, 0)
})
mbc.onCamBlob(MbcCamEvent.Lost, function () {             // 色塊消失
    basic.showIcon(IconNames.No, 0)
})

input.onButtonPressed(Button.A, function () {
    alarms = 0
    basic.showNumber(0)
})
input.onButtonPressed(Button.B, function () {
    armed = !armed
    basic.showString(armed ? "S" : "M")                   // S = 警報開、M = 靜音
})

basic.forever(function () {
    let m = mbc.camMotion()                               // 全畫面動態量 0~100
    if (armed && m > MOTION_LEVEL && control.millis() - lastAlarm > 2000) {
        alarms += 1
        lastAlarm = control.millis()
        music.playTone(Note.A, 300)                       // 警報響時伺服會抖一下，正常（PWM 共用週期）
        basic.showNumber(alarms)
    }
    if (mbc.camFound()) {                                 // 有色塊：雲台跟著它
        let err = mbc.camBlob(MbcCamBlob.X) - 50
        if (Math.abs(err) > 8) {
            angle = Math.constrain(angle - err / 10, 0, 180)
        }
        pins.servoWritePin(AnalogPin.P1, Math.round(angle))
    }
    basic.pause(50)
})
```

**一步一步看懂程式**
1. `MOTION_LEVEL = 30`、`armed = true`、`alarms`、`lastAlarm`。
2. `mbc.onCamBlob(Found, ...)` → 打勾；`Lost` → X。
3. A：`alarms = 0`；B：翻轉 `armed`，顯示 S／M。
4. `forever`：`m = mbc.camMotion()`；`armed` 且 `m > 30` 且距上次警報超過 2 秒 → `alarms + 1`、`Note.A` 0.3 秒、顯示次數。
5. `mbc.camFound()` → 跟範例 3 一樣算 `err`、更新 `angle`、轉伺服。

**跑起來應該看到什麼**
- 有人從鏡頭前走過：嗶、顯示 1；再走一次：2。
- 拿目標色的積木進畫面：打勾、伺服轉過去跟著它；拿走：X。
- 按 B：M，之後不再嗶但還會追。

**常見問題**
- **警報響的時候伺服抖一下？** → 正常：micro:bit 的 PWM 共用週期，蜂鳴器響時伺服脈衝會受影響。不想抖就把 `music.playTone` 換成 LED 閃爍。
- **伺服自己在動也會觸發警報？** → 相機跟著伺服轉，畫面當然在動。追蹤時先不算動態：在 `if (armed && ...)` 前面加 `!mbc.camFound() &&`。
- **一直警報？** → `MOTION_LEVEL` 改 50，或光線在閃（日光燈）。

---

## 範例 6：12 個色點一眼看完（WRO 版 HUE12）

**這個範例會做什麼**：WRO 版專用：LED 前 3 列 = 色點 S1～S12（每列 4 個），彩色亮、白微亮、黑或沒開暗；第 5 列的點 = 目前選的色點。按 A 選下一個色點、按 B 顯示它的顏色碼和鮮豔度。

**要準備什麼**
- micro:bit V1 或 V2＋USB 線
- 神攝手 ESP32CAM **WRO 版（HUE12）**＋外接 5 V
- WRO 場地圖或幾塊彩色、白色、黑色積木
- 相機先在機身上設好 12 個色點的位置

**怎麼接**：同範例 1。

**下載程式**：[ex06_wro_spots.ts](/examples/esp32cam/microbit/ex06_wro_spots.ts)（右鍵另存）→ MakeCode 新專案 → 齒輪 → 擴充 → 貼 `https://github.com/contactmbcrobot/pxt-mbc-sensors` → 右上切到 **JavaScript** → 全選刪掉原本的內容、把檔案內容貼上 → **下載**，把 .hex 拖進 MICROBIT 磁碟。

**程式**

```typescript
// 神攝手 ESP32CAM（WRO 版 HUE12）× micro:bit 範例 6：12 個色點一眼看完
// 做什麼：LED 前 3 列 = 色點 S1~S12（每列 4 個）：彩色 → 亮、白 → 微亮、黑或未啟用 → 暗。第 5 列的亮點 = 目前選的色點。按 A 選下一個色點、按 B 顯示它的顏色碼和鮮豔度。
// 接線：相機 SDA（IO3）→ P20、SCL（IO1）→ P19、VCC → 外接 5 V、GND → GND 共地。
// 準備：WRO 版相機先在機身上設定好 12 個色點的位置（看產品頁）。
// 看到什麼：場地上有彩色積木的色點亮、白底的微亮、黑的暗；按 B 看顏色碼：0~239 彩色（紅約 0、黃約 40、綠約 80、藍約 160）、240~245 白、246~250 黑、255 = 色點沒開。

let sel = 1                                               // 目前選的色點 1~12
let busy = false

if (mbc.waitReady(mbc.ADDR_CAM, 5000)) {
    basic.showIcon(IconNames.Yes)
} else {
    basic.showIcon(IconNames.No)
}

basic.forever(function () {
    if (busy) {
        basic.pause(100)
        return
    }
    basic.clearScreen()
    for (let n = 1; n <= 12; n++) {
        let x = (n - 1) % 4                               // 每列 4 個
        let y = Math.idiv(n - 1, 4)                       // 第 0~2 列
        if (mbc.camSpotIs(n, MbcCamSpotKind.Colour)) {    // 顏色碼 0~239
            led.plotBrightness(x, y, 255)
        } else if (mbc.camSpotIs(n, MbcCamSpotKind.White)) {   // 240~245
            led.plotBrightness(x, y, 40)
        }
    }
    led.plot((sel - 1) % 4, 4)                            // 第 5 列：目前選的色點在哪一行
    basic.pause(100)
})

input.onButtonPressed(Button.A, function () {
    sel = sel % 12 + 1                                    // 1 → 2 → … → 12 → 1
    busy = true
    basic.showString("S" + sel)
    busy = false
})
input.onButtonPressed(Button.B, function () {
    busy = true
    basic.showNumber(mbc.camSpotColor(sel))               // 顏色碼 0~255
    basic.showString("V")
    basic.showNumber(mbc.camSpotVivid(sel))               // 鮮豔度 0~255
    busy = false
})
```

**一步一步看懂程式**
1. `sel = 1`：目前選的色點。`waitReady` 打勾。
2. `forever`：清螢幕，`for n = 1..12`：`x = (n − 1) % 4`、`y = (n − 1) ÷ 4`（整數除法 `Math.idiv`）→ 第 0～2 列每列 4 個。
3. `mbc.camSpotIs(n, Colour)`（顏色碼 0～239）→ `led.plotBrightness(x, y, 255)` 全亮；`White`（240～245）→ 40 微亮；黑／沒開 → 不畫。
4. `led.plot((sel − 1) % 4, 4)`：第 5 列標出目前選的色點在哪一行。
5. A：`sel = sel % 12 + 1` 循環 1～12，顯示 S 加數字；B：顯示 `camSpotColor(sel)`、V、`camSpotVivid(sel)`。

**跑起來應該看到什麼**
- 色點在彩色積木上：那一格全亮；白底：微亮；黑：暗；色點沒開：暗。
- 按 A：S2、S3 …；第 5 列的點跟著移。
- 按 B：例如 40（黃）、V、180。

**常見問題**
- **全部都暗？** → 12 個色點都沒開（顏色碼 255），到相機機身設定色點位置。
- **顏色碼和 SPIKE 頁的數字不一樣？** → I2C 給的是 0～255 的顏色碼（彩色 0～239），SPIKE 模式是另一種縮放；對照表看 [指令參考](/sensors/esp32cam/reference.md)。

---

## 下一步

- 全部積木、數值和硬體說明：[神攝手 ESP32CAM × micro:bit](/sensors/esp32cam/microbit.md)
- 每個數值編號的意思：[指令參考](/sensors/esp32cam/reference.md)
- 擴充原始碼與回報問題：<https://github.com/contactmbcrobot/pxt-mbc-sensors>
