<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 程式碼區塊＝examples/unicam/microbit/*.ts 原文（小幫手正本：02_相機感應器\06_通用版\通用版v1\上位機函式庫\microbit\MBC_UniCam_microbit.ts）；改程式請改 .ts 再重貼 -->
# 🟧 micro:bit 範例：神攝手

[🔙 回到神攝手產品頁](/sensors/unicam/index.md)　｜　[micro:bit 讀值教學](/sensors/unicam/microbit/index.md)　｜　[📋 讀值參考（I2C）](/sensors/unicam/reference.md#host-i2c)

> [!WARNING]
> **v1 尚未上機驗證**：數值排法已定案並通過電腦端測試，實機數值（fps、循線門檻、I2C 腳位）上機後可能調整。

這一頁是寫給**完全沒寫過程式**的人。4 個範例：**色塊在哪邊**、**取樣點看到什麼顏色**、**循線小車**、**紅色三角形在哪邊（物體偵測）**。全部用 **MakeCode 網頁版**的 JavaScript 寫：切到 JavaScript、把範例整個貼上就能用，也可以切回積木模式看它變成哪些積木。

> [!IMPORTANT]
> - **不用加擴充**。每支範例最下面都有一段「神攝手 小幫手」，用 micro:bit 內建的 `pins.i2c…` 指令跟相機講話。擴充 **pxt-mbc-sensors** 裡的「神攝手」積木是**學校版**用的，讀不到目前神攝手的值，不要混用。
> - 請用 **通用 I2C 版**韌體的相機。SPIKE、EV3、MATRIX 版韌體接 micro:bit 讀不到。
> - 範例都在電腦上用 MakeCode CLI 編譯通過（V1＋V2 各一次），也用假的 I2C 資料跑過邏輯；**還沒接真的相機跑過**。

## 軟體對照表

| 寫法 | micro:bit V1 | micro:bit V2 | 怎麼用 |
|---|---|---|---|
| **MakeCode 網頁版**（[makecode.microbit.org](https://makecode.microbit.org/)，JavaScript） | ✔ | ✔ | 新專案 → 右上切到 **JavaScript** → 全選刪掉原本的內容、貼上範例 → **下載**，把 .hex 拖進 MICROBIT 磁碟 |
| MakeCode 離線版（Windows／Mac App） | ✔ | ✔ | 同上 |
| Python Editor（MicroPython） | — | — | 還沒有範例 |

- **模擬器**：MakeCode 模擬器沒有真的 I2C，讀到的都是 0（範例會顯示 X），要看真的數值一定要下載到 micro:bit。
- **伺服**：範例 3 用兩顆**連續旋轉伺服**（FS90R 之類），左輪 P1、右輪 P2。伺服電源一律**外接 4.8～6 V 電池盒**，電池負極要和 micro:bit GND 接在一起（共地）。

## 開始之前：硬體關卡（上機待測）

| 關卡 | 這款產品 | 狀態 |
|---|---|---|
| 電壓 | 相機的 I2C 是 3.3 V 準位，直接接 P19／P20，不用準位轉換 | ✔ |
| 供電 | ESP32 相機＋螢幕峰值超過 300 mA，**一定要外接 5 V**，不能從 micro:bit 3V 腳取電 | 上機待測 |
| I2C 上拉 | micro:bit 板上已有 4.7 kΩ；相機板若也有上拉，並聯後要確認低電位 ≤ 0.3×VDD | 上機待測 |
| 時脈延展 | ESP32 不能時脈延展：小幫手寫完編號會先等 2 ms 再讀，不用自己處理 | 上機待測 |
| I2C 腳位 | 相機端暫定 SDA＝IO3、SCL＝IO1（跟樂高串口同一對），**要等實測才定案** | 上機待測 |
| 就緒時間 | 小幫手 `ucReady()` 讀 F3 bit0；範例開機會一直等到相機回應才開始 | 上機待測 |

### 接線（所有範例共用）

| 相機 | micro:bit |
| :--- | :--- |
| SDA | P20 |
| SCL | P19 |
| VCC | 外接 5 V（必要） |
| GND | GND（伺服電池負極也接這裡） |

讀不到值（一直顯示 X）時先看：SDA／SCL 有沒有接反 → 外接電源有沒有開 → 相機韌體是不是「通用 I2C 版」。

## 這頁會用到的數值

- **模式編號** ＝ 相機模式 × 10 ＋ 子選項：13＝顏色追蹤 ③ 標準、21＝座標取色 ① 色相、41＝循線 ① 黑線。範例開機時會用 **E0** 命令自動切，也可以在相機選單自己切（兩邊要一樣）。
- **座標**：像素 X 0～159（左 → 右）、Y 0～119（上 → 下）；**0＝沒找到**。
- **色相碼** 0～255：0～240 是色相（紅 0／240、黃 40、綠 80、青 120、藍 160、紫 200）；241～248 白／灰；249～255 黑。
- **循線偏移** −100～100：站在車子後面往前看，**線在左＝負**、線在右＝正。

### I2C 值編號（位址 0x4C）

| 編號 | 內容 |
|---|---|
| `0x01` | 模式編號（例 13；物體偵測 51／52） |
| `0x02` | 子選項 |
| `0x03` | 已教幾色 |
| `0x04` | 這一層給幾個值 |
| `0x10` 起 | 這個模式的值（照 [讀值參考](/sensors/unicam/reference.md#host-i2c) 的順序：值 1＝`0x10`、值 2＝`0x11`…） |
| `0xF0` | 裝置 ID（`0x4C`＋3×256，3＝目前的神攝手） |
| `0xF2` | 韌體版（100＝1.0） |
| `0xF3` | 狀態（bit0＝準備好、bit1＝目前模式有找到東西） |
| 寫 `0xE0` | [相機模式 1～5, 子選項]：切模式（存進相機；物體偵測 [5, 1]／[5, 2]） |
| 寫 `0xE1` | [第幾點 1～20, X 0～159, Y 0～119]：改取樣點位置（存進相機） |

讀法（MBC 通用 I2C 協議 v2）：寫 [編號] → 等 2 ms → 讀 2×k byte，就是從那個編號起連續 k 個值（int16，低位元組在前），一次最多 16 個；沒有的編號回 −32768。

### 小幫手函式

| 函式 | 回傳 | 意思 |
|---|---|---|
| `ucReadMany(id, k)` | 數字陣列 | 從編號 id 起連續讀 k 個值（1～16）；讀不到全部是 `UC_NONE`（−32768） |
| `ucWrite(id, params)` | — | 寫命令 [id][參數…] |
| `ucReady()` | true／false | 相機準備好了嗎（F3 bit0） |
| `ucSetMode(camMode, sub)` | — | E0：切相機模式（照順序給色號的子選項填 0） |
| `ucSetPoint(n, x, y)` | — | E1：把第 n 個取樣點移到 (x, y) |
| `ucModeNumber()` | 數字 | 相機目前的模式編號 |
| `ucReadBlob(mode, color)` | `[x, y, 寬, 高, 塊數, 面積%]` | 顏色追蹤 11～14，色 1～16；x＝0 是沒找到；這個模式沒給的是 −1 |
| `ucColorsFound(mode)` | 數字 | 16 個目標色裡找到幾色 |
| `ucReadSpot(mode, point)` | `[色相碼, 飽和度, 明度]` | 座標取色 21～23，點 1～16；沒給的是 −1 |
| `ucReadOrder()` | 數字陣列 | 照順序給色號：由左到右每塊的色相碼（最多 16 塊） |
| `ucOrderTotal()` | 數字 | 總塊數 |
| `ucReadLine()` | 14 個數字 | 循線：[近, 中, 遠偏移, 角度, 線寬, 有線旗標, 路口類型, 路口距離, 左緣, 右緣, 遠線寬, 標記色號, 標記條數, 離線方向] |
| `ucColorCode(h)` | 1～9 | 色相碼 → 1 紅 2 橙 3 黃 4 綠 5 青 6 藍 7 紫 8 白／灰 9 黑 |
| `ucReadObject(mode, i)` | `[代碼, 色號, 形狀, x, y, 寬, 高, 面積%, 角度, 頂點數, 追蹤編號]` | 物體偵測 51／52，物體 1～8（1＝最大）；代碼＝色號×10＋形狀，0＝沒有；模式 51 沒給的是 −1 |
| `ucObjectCount(mode)` | 數字 | 畫面裡有幾個物體 0～8 |
| `ucShapeName(s)` | 文字 | 形狀 0～9 → `"other"` `"circle"` `"ellipse"` `"triangle"` `"square"` `"rectangle"` `"pentagon"` `"hexagon"` `"star"` `"trapezoid"` |

---

## 範例 1：色塊在哪邊（箭頭）

**這個範例會做什麼**：色 1（相機教的第 1 個顏色）在畫面左邊 → LED 往左箭頭；中間 → 向上箭頭；右邊 → 往右箭頭；沒看到 → 問號。按 A 顯示面積 %。

**要準備什麼**
- micro:bit V1 或 V2＋USB 線
- 神攝手（通用 I2C 版韌體）＋外接 5 V 電源
- 一塊顏色鮮豔的積木

**相機設定**：在相機上教好「目標 1」（即時畫面停在目標 1 → 按 OK 取色）。相機模式 **1 顏色追蹤 ・子選項 ③ 標準**（模式編號 13）——程式開頭會用 E0 自動切。

**怎麼接**：照上面的接線表。

**下載程式**：[ex01_track_arrow.ts](/examples/unicam/microbit/ex01_track_arrow.ts)（右鍵另存）→ MakeCode 新專案 → 右上切到 **JavaScript** → 全選刪掉、貼上 → **下載**。

**程式**

```typescript
// 神攝手 × micro:bit 範例 1：色塊在哪邊（箭頭）
// ★ 需要把相機選單切到 相機模式 1 顏色追蹤 ・子選項 ③ 標準（模式編號 13）（程式開頭也會用 E0 自動切）
// 做什麼：色 1（相機教的第 1 個顏色）在畫面左邊 → 往左箭頭、中間 → 向上箭頭、右邊 → 往右箭頭；沒找到 → 問號。按 A 顯示面積 %。
// 接線：相機 SDA → P20、SCL → P19、VCC → 外接 5 V（不要從 micro:bit 3V 取電）、GND → GND 共地。
// 準備：在相機機身教好「目標 1」的顏色（停在目標 1 按 OK 取色）。MakeCode 新專案 → 右上切到 JavaScript → 把本檔全部貼上 → 下載。不用加擴充。
// 看到什麼：開機打勾；拿目標色的積木在鏡頭前左右移動，箭頭跟著換方向；拿走 → 問號。
// ⚠ v1 尚未上機驗證：數值排法已定案並通過電腦端測試，實機數值上機後可能調整。

const MODE = 13                                   // 模式編號 = 相機模式 1 × 10 + 子選項 3
let ready = false
let busy = false

basic.showIcon(IconNames.SmallDiamond)            // 等相機

basic.forever(function () {
    if (!ready) {                                 // 第一次：等相機準備好，再切到模式 13
        if (ucReady()) {
            ucSetMode(1, 3)                       // E0：相機模式 1、子選項 3
            ready = true
            basic.showIcon(IconNames.Yes)
            basic.pause(500)
        } else {
            basic.showIcon(IconNames.No)          // 沒回應：檢查 P19／P20、外接電源
            basic.pause(500)
        }
        return
    }
    if (busy) {
        basic.pause(100)
        return
    }
    const b = ucReadBlob(MODE, 1)                 // [x, y, 寬, 高, 塊數, 面積%]
    const x = b[0]                                // 0 = 沒找到；1~159 左 → 右
    if (x <= 0) {
        basic.showString("?", 0)
    } else if (x < 60) {                          // 👉 可以改：左邊的範圍
        basic.showArrow(ArrowNames.West, 0)
    } else if (x > 100) {                         // 👉 可以改：右邊的範圍
        basic.showArrow(ArrowNames.East, 0)
    } else {
        basic.showArrow(ArrowNames.North, 0)
    }
    basic.pause(50)
})

input.onButtonPressed(Button.A, function () {
    busy = true
    basic.showNumber(ucReadBlob(MODE, 1)[5])      // 面積：佔畫面 %（0 = 沒找到）
    busy = false
})

// ==== MBC_UNICAM_MB_HELPERS_BEGIN ====
// ===== 神攝手 小幫手（MBC 通用 I2C 協議 v2）：所有範例都一樣，不用改 =====
// ===== UniCam helpers (MBC common I2C protocol v2): same in every example, no need to change =====
const UC_ADDR = 0x4C                     // 相機的 I2C 位址 / camera I2C address
const UC_NONE = -32768                   // 讀不到 / no value

// 從編號 id 起連續讀 k 個 int16（1~16）；讀不到全部是 UC_NONE
function ucReadMany(id: number, k: number): number[] {
    const cmd = pins.createBuffer(1)
    cmd[0] = id
    const r = pins.i2cWriteBuffer(UC_ADDR, cmd, false)
    basic.pause(2)                       // 相機（ESP32）要等 2 ms 再讀
    const out: number[] = []
    if (r) {                             // 實機：非 0 = 沒有回應
        for (let i = 0; i < k; i++) out.push(UC_NONE)
        return out
    }
    const b = pins.i2cReadBuffer(UC_ADDR, 2 * k, false)
    for (let i = 0; i < k; i++) out.push(b.getNumber(NumberFormat.Int16LE, 2 * i))
    return out
}

// 寫命令 [id][參數…] / write a command
function ucWrite(id: number, params: number[]): void {
    const b = pins.createBuffer(1 + params.length)
    b[0] = id
    for (let i = 0; i < params.length; i++) b[1 + i] = params[i]
    pins.i2cWriteBuffer(UC_ADDR, b, false)
}

// 相機準備好了嗎（F3 bit0）/ is the camera ready
function ucReady(): boolean {
    const f = ucReadMany(0xF3, 1)[0]
    return f != UC_NONE && (f & 1) == 1
}

// 切相機模式（E0）：camMode 1~5、sub 子選項（照順序給色號填 0；物體偵測 1 清單、2 詳細）/ switch camera mode
function ucSetMode(camMode: number, sub: number): void {
    ucWrite(0xE0, [camMode, sub])
    basic.pause(50)
}

// 改取樣點位置（E1）：n 1~20、x 0~159、y 0~119 / move a sample spot
function ucSetPoint(n: number, x: number, y: number): void {
    ucWrite(0xE1, [n, x, y])
}

// 相機目前的模式編號（例 13）/ current mode number
function ucModeNumber(): number {
    return ucReadMany(0x01, 1)[0]
}

// 顏色追蹤每色幾個值 / values per colour (modes 11~14)
function ucStride(mode: number): number {
    if (mode == 11) return 2
    if (mode == 12) return 3
    if (mode == 13) return 4
    if (mode == 14) return 6
    return 0
}

// 色 color（1~16）的色塊：[x, y, 寬, 高, 塊數, 面積%]，x = 0 = 沒找到；這個模式沒給的是 -1
// blob of colour: [x, y, w, h, count, area%]; x = 0 = not found; -1 = not in this mode
function ucReadBlob(mode: number, color: number): number[] {
    const k = ucStride(mode)
    const d = ucReadMany(0x10 + (color - 1) * k, k)
    if (mode == 11) return [d[0], d[1], -1, -1, -1, -1]
    if (mode == 12) return [d[0], d[1], -1, -1, d[2], -1]
    if (mode == 13) return [d[0], d[1], -1, -1, d[3], d[2]]
    return [d[0], d[1], d[2], d[3], d[4], d[5]]
}

// 16 個目標色裡找到幾色 / colours found (modes 11~14)
function ucColorsFound(mode: number): number {
    return ucReadMany(0x10 + 16 * ucStride(mode), 1)[0]
}

// 取樣點 point（1~16）：[色相碼, 飽和度, 明度]；沒給的是 -1 / spot: [hue, sat, val]
function ucReadSpot(mode: number, point: number): number[] {
    const k = mode - 20
    const d = ucReadMany(0x10 + (point - 1) * k, k)
    return [d[0], k >= 2 ? d[1] : -1, k >= 3 ? d[2] : -1]
}

// 照順序給色號：由左到右每塊的色相碼（最多 16 塊）/ hue codes left to right
function ucReadOrder(): number[] {
    const total = ucReadMany(0x20, 1)[0]
    const d = ucReadMany(0x10, 16)
    if (total == UC_NONE) return []
    return d.slice(0, Math.min(16, total))
}

// 總塊數 / total blocks (mode 30)
function ucOrderTotal(): number {
    return ucReadMany(0x20, 1)[0]
}

// 循線 14 個值：[近, 中, 遠偏移, 角度, 線寬, 有線旗標, 路口類型, 路口距離, 左緣, 右緣, 遠線寬, 標記色號, 標記條數, 離線方向]
// line values (near/mid/far offset: line on the left = negative)
function ucReadLine(): number[] {
    return ucReadMany(0x10, 14)
}

// 物體偵測每個物體幾個值：51 → 4（色號 形狀 X Y）、52 → 10 / values per object (modes 51, 52)
function ucObjStride(mode: number): number {
    if (mode == 51) return 4
    if (mode == 52) return 10
    return 0
}

// 物體 i（1~8，1 = 面積最大）：[代碼, 色號, 形狀, x, y, 寬, 高, 面積%, 角度, 頂點數, 追蹤編號]
// 代碼 = 色號×10＋形狀，0 = 沒有；模式 51 沒給的是 -1
// object i: [code, colour, shape, x, y, w, h, area%, angle, vertices, trackId]; -1 = not in mode 51
function ucReadObject(mode: number, i: number): number[] {
    const k = ucObjStride(mode)
    const d = ucReadMany(0x10 + (i - 1) * k, k)
    const code = d[0] > 0 ? d[0] * 10 + d[1] : 0
    if (mode == 51) return [code, d[0], d[1], d[2], d[3], -1, -1, -1, -1, -1, -1]
    return [code, d[0], d[1], d[2], d[3], d[4], d[5], d[6], d[7], d[8], d[9]]
}

// 畫面裡有幾個物體 0~8 / number of objects in view (modes 51, 52)
function ucObjectCount(mode: number): number {
    return ucReadMany(0x10 + 8 * ucObjStride(mode), 1)[0]
}

// 形狀編號 → 英文名稱 / shape number -> name
function ucShapeName(s: number): string {
    const n = ["other", "circle", "ellipse", "triangle", "square", "rectangle", "pentagon", "hexagon", "star", "trapezoid"]
    return (s >= 0 && s <= 9) ? n[s] : "?"
}

// 色相碼 → 顏色代號：1 紅 2 橙 3 黃 4 綠 5 青 6 藍 7 紫 8 白／灰 9 黑
function ucColorCode(h: number): number {
    if (h > 248) return 9
    if (h > 240) return 8
    if (h < 12 || h >= 220) return 1
    if (h < 28) return 2
    if (h < 56) return 3
    if (h < 100) return 4
    if (h < 140) return 5
    if (h < 180) return 6
    return 7
}
// ==== MBC_UNICAM_MB_HELPERS_END ====
```

**一步一步看懂程式**
1. 開機顯示小菱形，`forever` 第一次進來先 `ucReady()` 等相機回應；回應了就 `ucSetMode(1, 3)` 切到模式 13、打勾。
2. `ucReadBlob(MODE, 1)` 一次讀色 1 的 4 個值（X、Y、面積、數量），整理成 `[x, y, 寬, 高, 塊數, 面積%]`。
3. `x` 是 0 → 沒看到 → 問號；小於 60 → 左；大於 100 → 右；其他 → 中間（畫面寬 160，中間是 80）。
4. 按 A：`[5]` 是面積 %，用數字顯示。

**跑起來應該看到什麼**
- 開機約 3 秒後打勾。
- 積木放在畫面左邊：←；正中間：↑；右邊：→；拿走：?。
- 積木越靠近鏡頭，按 A 的數字越大。

**常見問題**
- **一直是 X？** → 相機沒回應：看接線、外接電源、韌體是不是通用 I2C 版。相機先上電、等畫面出來，再按 micro:bit 的重置鍵。
- **一直是問號？** → 還沒教「目標 1」，或燈光太暗。看相機螢幕底部資訊列的 X 有沒有數字。
- **左右相反？** → 相機裝反了（鏡頭朝向你）：把 `x < 60` 和 `x > 100` 的箭頭對調。

---

## 範例 2：取樣點看到什麼顏色

**這個範例會做什麼**：LED 顯示取樣點 S1 看到的顏色字母：**R** 紅、**O** 橙、**Y** 黃、**G** 綠、**C** 青、**B** 藍、**P** 紫、**W** 白／灰、**K** 黑。按 A 用 **E1** 命令把 S1 移到畫面正中間；按 B 顯示色相碼數字。

**要準備什麼**：範例 1 的東西＋紅、黃、綠、藍、白、黑的積木或色紙。

**相機設定**：相機模式 **2 座標取色 ・子選項 ① 色相**（模式編號 21）——程式開頭會自動切。

**怎麼接**：同範例 1。

**下載程式**：[ex02_spot_color.ts](/examples/unicam/microbit/ex02_spot_color.ts)（右鍵另存）→ 貼進 MakeCode JavaScript → 下載。

**程式**

```typescript
// 神攝手 × micro:bit 範例 2：取樣點看到什麼顏色
// ★ 需要把相機選單切到 相機模式 2 座標取色 ・子選項 ① 色相（模式編號 21）（程式開頭也會用 E0 自動切）
// 做什麼：LED 顯示取樣點 S1 看到的顏色字母：R 紅、O 橙、Y 黃、G 綠、C 青、B 藍、P 紫、W 白／灰、K 黑。
//         按 A：用 E1 命令把 S1 移到畫面正中間（X 80、Y 60）；按 B：顯示色相碼數字（0~255）。
// 接線：相機 SDA → P20、SCL → P19、VCC → 外接 5 V、GND → GND 共地。
// 準備：紅、黃、綠、藍、白、黑的積木或色紙。MakeCode 切到 JavaScript 貼上本檔 → 下載。不用加擴充。
// 看到什麼：先按 A，把積木放在鏡頭正中間：紅 → R、綠 → G、藍 → B、白紙 → W。
// ⚠ v1 尚未上機驗證：顏色的分界（ucColorCode）是依色相碼估的，每個場地光線不同，請按 B 看數字自己調。

const MODE = 21                                   // 模式編號 = 相機模式 2 × 10 + 子選項 1
const LETTERS = "?ROYGCBPWK"                      // ucColorCode 1~9 對應的字母
let ready = false
let busy = false

basic.showIcon(IconNames.SmallDiamond)            // 等相機

basic.forever(function () {
    if (!ready) {                                 // 第一次：等相機準備好，再切到模式 21
        if (ucReady()) {
            ucSetMode(2, 1)                       // E0：相機模式 2、子選項 1
            ready = true
            basic.showIcon(IconNames.Yes)
            basic.pause(500)
        } else {
            basic.showIcon(IconNames.No)          // 沒回應：檢查 P19／P20、外接電源
            basic.pause(500)
        }
        return
    }
    if (busy) {
        basic.pause(100)
        return
    }
    const h = ucReadSpot(MODE, 1)[0]              // S1 的色相碼 0~255
    if (h == UC_NONE) {
        basic.showString("-", 0)                  // 讀不到
    } else {
        basic.showString(LETTERS.charAt(ucColorCode(h)), 0)
    }
    basic.pause(100)
})

input.onButtonPressed(Button.A, function () {
    busy = true
    ucSetPoint(1, 80, 60)                         // E1：第 1 點移到 X 80、Y 60（畫面中間，會存進相機）
    basic.showIcon(IconNames.Target)
    basic.pause(500)
    busy = false
})

input.onButtonPressed(Button.B, function () {
    busy = true
    basic.showNumber(ucReadSpot(MODE, 1)[0])      // 色相碼：紅 0/240、黃 40、綠 80、青 120、藍 160、紫 200；241~248 白、249~255 黑
    busy = false
})

// ==== MBC_UNICAM_MB_HELPERS_BEGIN ====
// ===== 神攝手 小幫手（MBC 通用 I2C 協議 v2）：所有範例都一樣，不用改 =====
// ===== UniCam helpers (MBC common I2C protocol v2): same in every example, no need to change =====
const UC_ADDR = 0x4C                     // 相機的 I2C 位址 / camera I2C address
const UC_NONE = -32768                   // 讀不到 / no value

// 從編號 id 起連續讀 k 個 int16（1~16）；讀不到全部是 UC_NONE
function ucReadMany(id: number, k: number): number[] {
    const cmd = pins.createBuffer(1)
    cmd[0] = id
    const r = pins.i2cWriteBuffer(UC_ADDR, cmd, false)
    basic.pause(2)                       // 相機（ESP32）要等 2 ms 再讀
    const out: number[] = []
    if (r) {                             // 實機：非 0 = 沒有回應
        for (let i = 0; i < k; i++) out.push(UC_NONE)
        return out
    }
    const b = pins.i2cReadBuffer(UC_ADDR, 2 * k, false)
    for (let i = 0; i < k; i++) out.push(b.getNumber(NumberFormat.Int16LE, 2 * i))
    return out
}

// 寫命令 [id][參數…] / write a command
function ucWrite(id: number, params: number[]): void {
    const b = pins.createBuffer(1 + params.length)
    b[0] = id
    for (let i = 0; i < params.length; i++) b[1 + i] = params[i]
    pins.i2cWriteBuffer(UC_ADDR, b, false)
}

// 相機準備好了嗎（F3 bit0）/ is the camera ready
function ucReady(): boolean {
    const f = ucReadMany(0xF3, 1)[0]
    return f != UC_NONE && (f & 1) == 1
}

// 切相機模式（E0）：camMode 1~5、sub 子選項（照順序給色號填 0；物體偵測 1 清單、2 詳細）/ switch camera mode
function ucSetMode(camMode: number, sub: number): void {
    ucWrite(0xE0, [camMode, sub])
    basic.pause(50)
}

// 改取樣點位置（E1）：n 1~20、x 0~159、y 0~119 / move a sample spot
function ucSetPoint(n: number, x: number, y: number): void {
    ucWrite(0xE1, [n, x, y])
}

// 相機目前的模式編號（例 13）/ current mode number
function ucModeNumber(): number {
    return ucReadMany(0x01, 1)[0]
}

// 顏色追蹤每色幾個值 / values per colour (modes 11~14)
function ucStride(mode: number): number {
    if (mode == 11) return 2
    if (mode == 12) return 3
    if (mode == 13) return 4
    if (mode == 14) return 6
    return 0
}

// 色 color（1~16）的色塊：[x, y, 寬, 高, 塊數, 面積%]，x = 0 = 沒找到；這個模式沒給的是 -1
// blob of colour: [x, y, w, h, count, area%]; x = 0 = not found; -1 = not in this mode
function ucReadBlob(mode: number, color: number): number[] {
    const k = ucStride(mode)
    const d = ucReadMany(0x10 + (color - 1) * k, k)
    if (mode == 11) return [d[0], d[1], -1, -1, -1, -1]
    if (mode == 12) return [d[0], d[1], -1, -1, d[2], -1]
    if (mode == 13) return [d[0], d[1], -1, -1, d[3], d[2]]
    return [d[0], d[1], d[2], d[3], d[4], d[5]]
}

// 16 個目標色裡找到幾色 / colours found (modes 11~14)
function ucColorsFound(mode: number): number {
    return ucReadMany(0x10 + 16 * ucStride(mode), 1)[0]
}

// 取樣點 point（1~16）：[色相碼, 飽和度, 明度]；沒給的是 -1 / spot: [hue, sat, val]
function ucReadSpot(mode: number, point: number): number[] {
    const k = mode - 20
    const d = ucReadMany(0x10 + (point - 1) * k, k)
    return [d[0], k >= 2 ? d[1] : -1, k >= 3 ? d[2] : -1]
}

// 照順序給色號：由左到右每塊的色相碼（最多 16 塊）/ hue codes left to right
function ucReadOrder(): number[] {
    const total = ucReadMany(0x20, 1)[0]
    const d = ucReadMany(0x10, 16)
    if (total == UC_NONE) return []
    return d.slice(0, Math.min(16, total))
}

// 總塊數 / total blocks (mode 30)
function ucOrderTotal(): number {
    return ucReadMany(0x20, 1)[0]
}

// 循線 14 個值：[近, 中, 遠偏移, 角度, 線寬, 有線旗標, 路口類型, 路口距離, 左緣, 右緣, 遠線寬, 標記色號, 標記條數, 離線方向]
// line values (near/mid/far offset: line on the left = negative)
function ucReadLine(): number[] {
    return ucReadMany(0x10, 14)
}

// 物體偵測每個物體幾個值：51 → 4（色號 形狀 X Y）、52 → 10 / values per object (modes 51, 52)
function ucObjStride(mode: number): number {
    if (mode == 51) return 4
    if (mode == 52) return 10
    return 0
}

// 物體 i（1~8，1 = 面積最大）：[代碼, 色號, 形狀, x, y, 寬, 高, 面積%, 角度, 頂點數, 追蹤編號]
// 代碼 = 色號×10＋形狀，0 = 沒有；模式 51 沒給的是 -1
// object i: [code, colour, shape, x, y, w, h, area%, angle, vertices, trackId]; -1 = not in mode 51
function ucReadObject(mode: number, i: number): number[] {
    const k = ucObjStride(mode)
    const d = ucReadMany(0x10 + (i - 1) * k, k)
    const code = d[0] > 0 ? d[0] * 10 + d[1] : 0
    if (mode == 51) return [code, d[0], d[1], d[2], d[3], -1, -1, -1, -1, -1, -1]
    return [code, d[0], d[1], d[2], d[3], d[4], d[5], d[6], d[7], d[8], d[9]]
}

// 畫面裡有幾個物體 0~8 / number of objects in view (modes 51, 52)
function ucObjectCount(mode: number): number {
    return ucReadMany(0x10 + 8 * ucObjStride(mode), 1)[0]
}

// 形狀編號 → 英文名稱 / shape number -> name
function ucShapeName(s: number): string {
    const n = ["other", "circle", "ellipse", "triangle", "square", "rectangle", "pentagon", "hexagon", "star", "trapezoid"]
    return (s >= 0 && s <= 9) ? n[s] : "?"
}

// 色相碼 → 顏色代號：1 紅 2 橙 3 黃 4 綠 5 青 6 藍 7 紫 8 白／灰 9 黑
function ucColorCode(h: number): number {
    if (h > 248) return 9
    if (h > 240) return 8
    if (h < 12 || h >= 220) return 1
    if (h < 28) return 2
    if (h < 56) return 3
    if (h < 100) return 4
    if (h < 140) return 5
    if (h < 180) return 6
    return 7
}
// ==== MBC_UNICAM_MB_HELPERS_END ====
```

**一步一步看懂程式**
1. 開機等相機、`ucSetMode(2, 1)` 切到模式 21（每個取樣點給 1 個值：色相碼）。
2. `ucReadSpot(MODE, 1)[0]`：S1 的色相碼 0～255。
3. `ucColorCode(h)` 把色相碼分成 1～9 類，`LETTERS.charAt(...)` 取出對應的字母。
4. 按 A：`ucSetPoint(1, 80, 60)` 送 E1 命令，S1 移到畫面中間（會存進相機，下次開機還在）。相機螢幕上的十字會跟著移。
5. 按 B：顯示色相碼數字，用來自己調顏色的分界。

**跑起來應該看到什麼**
- 按 A 後相機螢幕上 S1 的十字跑到正中間、micro:bit 顯示靶心。
- 紅積木 → R；黃 → Y；綠 → G；藍 → B；白紙 → W；黑積木 → K。

**常見問題**
- **紅色顯示 O、藍色顯示 C？** → 每個場地的燈光不同。按 B 看數字，再改小幫手外面的判斷（例如自己寫 `if (h < 20) …`）；不要改小幫手本身。
- **白紙顯示成彩色？** → 白紙在暖色燈下會偏黃。到相機 SET 選單做「白色校正」。

---

## 範例 3：循線小車（PD 循線＋路口停車）

**這個範例會做什麼**：按 A 開始沿黑線走：線偏左就往左修、偏右就往右修（近處和遠處偏移一起看，彎道會比較早轉）；遇到**十字路口或 T 字路口**就停車、顯示方塊；看不到線就往最後看到線的那一邊原地轉。按 B 停。

**要準備什麼**
- 範例 1 的東西
- 兩輪小車：左右各一顆連續旋轉伺服＋ 4.8～6 V 電池盒
- 白底黑線的場地（線寬約 2 cm），中間做一個十字路口

**相機設定**：相機裝在車頭、斜斜往前下方看地面。相機模式 **4 循線 ・子選項 ① 黑線**（模式編號 41）——程式開頭會自動切；白線把 `ucSetMode(4, 1)` 改成 `(4, 2)`、色線改 `(4, 3)`。

**怎麼接**：相機同範例 1；左輪伺服訊號 → **P1**、右輪 → **P2**；伺服電源接電池盒，電池負極接 micro:bit GND。

**下載程式**：[ex03_line_follow.ts](/examples/unicam/microbit/ex03_line_follow.ts)（右鍵另存）→ 貼進 MakeCode JavaScript → 下載。

**程式**

```typescript
// 神攝手 × micro:bit 範例 3：循線小車（近／遠偏移 PD 循線，遇到路口停車）
// ★ 需要把相機選單切到 相機模式 4 循線 ・子選項 ① 黑線（模式編號 41）（程式開頭也會用 E0 自動切；白線改 ②、色線改 ③）
// 做什麼：按 A 開始沿黑線走：線偏左就往左修、偏右就往右修（近處＋遠處偏移一起看，轉彎比較早反應）；
//         遇到十字路口或 T 字路口就停車、顯示方塊；看不到線就往最後看到線的那一邊原地轉。按 B 停。
// 接線：相機 SDA → P20、SCL → P19、VCC → 外接 5 V、GND → GND。左輪連續旋轉伺服訊號 → P1、右輪 → P2；
//       伺服電源 4.8~6 V 電池盒，電池負極接 micro:bit GND。
// 準備：兩輪小車，相機裝在車頭、斜斜往前下方看地面；白底黑線的場地（線寬約 2 cm）。
// 方向：站在車子後面往前看，線在左 = 負、線在右 = 正。
// ⚠ v1 尚未上機驗證：BASE、KP、KD 是建議值，請依你的車子調。

const BASE = 40                                   // 👉 可以改：直走速度（-100~100）
const KP = 0.5                                    // 👉 可以改：偏多少就轉多少（P）
const KD = 0.3                                    // 👉 可以改：偏移變化快就多修一點（D）
let ready = false
let running = false
let lastErr = 0

// 左輪伺服 P1、右輪伺服 P2（連續旋轉伺服）；left／right = −100~100，正 = 前進
function drive(left: number, right: number): void {
    left = Math.constrain(left, -100, 100)
    right = Math.constrain(right, -100, 100)
    pins.servoWritePin(AnalogPin.P1, Math.round(90 + left * 0.9))    // 90 = 停、180 = 全速前進、0 = 全速後退
    pins.servoWritePin(AnalogPin.P2, Math.round(90 - right * 0.9))   // 右輪裝的方向相反，所以用減的
}

input.onButtonPressed(Button.A, function () {
    running = true
    lastErr = 0
    basic.showIcon(IconNames.Happy, 0)
})
input.onButtonPressed(Button.B, function () {
    running = false
    drive(0, 0)
    basic.showIcon(IconNames.No, 0)
})

drive(0, 0)
basic.showIcon(IconNames.SmallDiamond)            // 等相機

basic.forever(function () {
    if (!ready) {                                 // 第一次：等相機準備好，再切到模式 41
        if (ucReady()) {
            ucSetMode(4, 1)                       // E0：相機模式 4、子選項 1（黑線）
            ready = true
            basic.showIcon(IconNames.Yes)
        } else {
            basic.showIcon(IconNames.No)          // 沒回應：檢查 P19／P20、外接電源
            basic.pause(500)
        }
        return
    }
    if (!running) {
        basic.pause(100)
        return
    }
    const v = ucReadLine()                        // [近, 中, 遠, 角度, 線寬, 有線旗標, 路口類型, 路口距離, …, 離線方向]
    if (v[0] == UC_NONE) {                        // 讀不到相機：安全第一，停車
        drive(0, 0)
        basic.showIcon(IconNames.No, 0)
        return
    }
    const junction = v[6]                         // 0 無 1 十字 2 T 3 左 4 右 5 盡頭
    if (junction == 1 || junction == 2) {         // 十字或 T 字路口 → 停車
        drive(0, 0)
        running = false
        basic.showIcon(IconNames.Square, 0)
        return
    }
    if (v[5] == 0) {                              // 有線旗標 = 0：近中遠都沒看到線
        if (v[13] == 1) {
            drive(-30, 30)                        // 最後在左邊看到線 → 原地左轉找線
        } else {
            drive(30, -30)                        // 最後在右邊（或不知道）→ 原地右轉
        }
        return
    }
    const err = v[0] * 0.7 + v[2] * 0.3           // 近處偏移占 7 成、遠處占 3 成（線在左 = 負）
    const turn = KP * err + KD * (err - lastErr)  // PD：偏多少修多少＋偏移變多快
    lastErr = err
    drive(BASE + turn, BASE - turn)               // 線在左（負）→ 左輪慢、右輪快 → 往左修
    basic.pause(20)
})

// ==== MBC_UNICAM_MB_HELPERS_BEGIN ====
// ===== 神攝手 小幫手（MBC 通用 I2C 協議 v2）：所有範例都一樣，不用改 =====
// ===== UniCam helpers (MBC common I2C protocol v2): same in every example, no need to change =====
const UC_ADDR = 0x4C                     // 相機的 I2C 位址 / camera I2C address
const UC_NONE = -32768                   // 讀不到 / no value

// 從編號 id 起連續讀 k 個 int16（1~16）；讀不到全部是 UC_NONE
function ucReadMany(id: number, k: number): number[] {
    const cmd = pins.createBuffer(1)
    cmd[0] = id
    const r = pins.i2cWriteBuffer(UC_ADDR, cmd, false)
    basic.pause(2)                       // 相機（ESP32）要等 2 ms 再讀
    const out: number[] = []
    if (r) {                             // 實機：非 0 = 沒有回應
        for (let i = 0; i < k; i++) out.push(UC_NONE)
        return out
    }
    const b = pins.i2cReadBuffer(UC_ADDR, 2 * k, false)
    for (let i = 0; i < k; i++) out.push(b.getNumber(NumberFormat.Int16LE, 2 * i))
    return out
}

// 寫命令 [id][參數…] / write a command
function ucWrite(id: number, params: number[]): void {
    const b = pins.createBuffer(1 + params.length)
    b[0] = id
    for (let i = 0; i < params.length; i++) b[1 + i] = params[i]
    pins.i2cWriteBuffer(UC_ADDR, b, false)
}

// 相機準備好了嗎（F3 bit0）/ is the camera ready
function ucReady(): boolean {
    const f = ucReadMany(0xF3, 1)[0]
    return f != UC_NONE && (f & 1) == 1
}

// 切相機模式（E0）：camMode 1~5、sub 子選項（照順序給色號填 0；物體偵測 1 清單、2 詳細）/ switch camera mode
function ucSetMode(camMode: number, sub: number): void {
    ucWrite(0xE0, [camMode, sub])
    basic.pause(50)
}

// 改取樣點位置（E1）：n 1~20、x 0~159、y 0~119 / move a sample spot
function ucSetPoint(n: number, x: number, y: number): void {
    ucWrite(0xE1, [n, x, y])
}

// 相機目前的模式編號（例 13）/ current mode number
function ucModeNumber(): number {
    return ucReadMany(0x01, 1)[0]
}

// 顏色追蹤每色幾個值 / values per colour (modes 11~14)
function ucStride(mode: number): number {
    if (mode == 11) return 2
    if (mode == 12) return 3
    if (mode == 13) return 4
    if (mode == 14) return 6
    return 0
}

// 色 color（1~16）的色塊：[x, y, 寬, 高, 塊數, 面積%]，x = 0 = 沒找到；這個模式沒給的是 -1
// blob of colour: [x, y, w, h, count, area%]; x = 0 = not found; -1 = not in this mode
function ucReadBlob(mode: number, color: number): number[] {
    const k = ucStride(mode)
    const d = ucReadMany(0x10 + (color - 1) * k, k)
    if (mode == 11) return [d[0], d[1], -1, -1, -1, -1]
    if (mode == 12) return [d[0], d[1], -1, -1, d[2], -1]
    if (mode == 13) return [d[0], d[1], -1, -1, d[3], d[2]]
    return [d[0], d[1], d[2], d[3], d[4], d[5]]
}

// 16 個目標色裡找到幾色 / colours found (modes 11~14)
function ucColorsFound(mode: number): number {
    return ucReadMany(0x10 + 16 * ucStride(mode), 1)[0]
}

// 取樣點 point（1~16）：[色相碼, 飽和度, 明度]；沒給的是 -1 / spot: [hue, sat, val]
function ucReadSpot(mode: number, point: number): number[] {
    const k = mode - 20
    const d = ucReadMany(0x10 + (point - 1) * k, k)
    return [d[0], k >= 2 ? d[1] : -1, k >= 3 ? d[2] : -1]
}

// 照順序給色號：由左到右每塊的色相碼（最多 16 塊）/ hue codes left to right
function ucReadOrder(): number[] {
    const total = ucReadMany(0x20, 1)[0]
    const d = ucReadMany(0x10, 16)
    if (total == UC_NONE) return []
    return d.slice(0, Math.min(16, total))
}

// 總塊數 / total blocks (mode 30)
function ucOrderTotal(): number {
    return ucReadMany(0x20, 1)[0]
}

// 循線 14 個值：[近, 中, 遠偏移, 角度, 線寬, 有線旗標, 路口類型, 路口距離, 左緣, 右緣, 遠線寬, 標記色號, 標記條數, 離線方向]
// line values (near/mid/far offset: line on the left = negative)
function ucReadLine(): number[] {
    return ucReadMany(0x10, 14)
}

// 物體偵測每個物體幾個值：51 → 4（色號 形狀 X Y）、52 → 10 / values per object (modes 51, 52)
function ucObjStride(mode: number): number {
    if (mode == 51) return 4
    if (mode == 52) return 10
    return 0
}

// 物體 i（1~8，1 = 面積最大）：[代碼, 色號, 形狀, x, y, 寬, 高, 面積%, 角度, 頂點數, 追蹤編號]
// 代碼 = 色號×10＋形狀，0 = 沒有；模式 51 沒給的是 -1
// object i: [code, colour, shape, x, y, w, h, area%, angle, vertices, trackId]; -1 = not in mode 51
function ucReadObject(mode: number, i: number): number[] {
    const k = ucObjStride(mode)
    const d = ucReadMany(0x10 + (i - 1) * k, k)
    const code = d[0] > 0 ? d[0] * 10 + d[1] : 0
    if (mode == 51) return [code, d[0], d[1], d[2], d[3], -1, -1, -1, -1, -1, -1]
    return [code, d[0], d[1], d[2], d[3], d[4], d[5], d[6], d[7], d[8], d[9]]
}

// 畫面裡有幾個物體 0~8 / number of objects in view (modes 51, 52)
function ucObjectCount(mode: number): number {
    return ucReadMany(0x10 + 8 * ucObjStride(mode), 1)[0]
}

// 形狀編號 → 英文名稱 / shape number -> name
function ucShapeName(s: number): string {
    const n = ["other", "circle", "ellipse", "triangle", "square", "rectangle", "pentagon", "hexagon", "star", "trapezoid"]
    return (s >= 0 && s <= 9) ? n[s] : "?"
}

// 色相碼 → 顏色代號：1 紅 2 橙 3 黃 4 綠 5 青 6 藍 7 紫 8 白／灰 9 黑
function ucColorCode(h: number): number {
    if (h > 248) return 9
    if (h > 240) return 8
    if (h < 12 || h >= 220) return 1
    if (h < 28) return 2
    if (h < 56) return 3
    if (h < 100) return 4
    if (h < 140) return 5
    if (h < 180) return 6
    return 7
}
// ==== MBC_UNICAM_MB_HELPERS_END ====
```

**一步一步看懂程式**
1. `drive(left, right)`：左右輪 −100～100 換成伺服角度（90＝停）；右輪裝反向，所以用減的。
2. `ucReadLine()` 一次讀 14 個循線值。`v[0]` 是 −32768 → 讀不到相機 → 停車。
3. `v[6]` 路口類型：1 十字、2 T → 停車、`running = false`。
4. `v[5]` 有線旗標＝0 → 近中遠都沒看到線 → 看 `v[13]`（最後離線方向，1＝左）原地轉回去找。
5. 偏移 `err = 近×0.7 ＋ 遠×0.3`；`turn = KP×err ＋ KD×(err − 上一次)`（P＝偏多少修多少、D＝偏移變多快就多修一點）。
6. `drive(BASE + turn, BASE - turn)`：線在左（負）→ 左輪慢、右輪快 → 往左修。

**跑起來應該看到什麼**：按 A（笑臉）後車子沿著線走，彎道會提早轉；走到十字路口停下來、顯示方塊。把車子拿離開線，它會原地轉回去找線。

**常見問題**
- **一直往外衝、修不回來？** → `KP` 加大一點（0.5 → 0.8）；左右晃很厲害 → `KP` 減小、`BASE` 調慢。
- **方向反了（線在左卻往右轉）？** → 伺服接反了：把 P1、P2 的線對調，或把 `drive` 裡兩行的 `+`／`-` 對調。
- **還沒到路口就停？** → 相機看得比較遠，路口在畫面上方就會報。可以先看 `v[7]`（路口距離 %），大於某個數字才停。

---

## 範例 4：紅色三角形在哪邊（物體偵測）

**這個範例會做什麼**：相機把畫面裡的物體由大到小排好（物體 1 最大），每個物體給「物體代碼」＝色號 × 10 ＋ 形狀。程式從最大的往下找「目標 1 的三角形」（代碼 13）：在左邊 → 往左箭頭；中間 → 向上箭頭；右邊 → 往右箭頭；沒找到 → 問號。按 A 顯示畫面裡有幾個物體，按 B 捲動物體 1 的形狀名稱。

**要準備什麼**
- 範例 1 的東西
- 紅色三角形（紙板或積木）；再放一個紅色圓形、別色三角形，看它會不會認錯

**相機設定**：在相機上把「目標 1」教成紅色（形狀可以不指定，相機會自己判斷；只有目標 1～8 會當成物體）。相機模式 **5 物體偵測 ・子選項 ① 物體清單**（模式編號 51）——程式開頭會用 E0 自動切。

**怎麼接**：照上面的接線表。

**下載程式**：[ex04_object.ts](/examples/unicam/microbit/ex04_object.ts)（右鍵另存）→ 貼進 MakeCode JavaScript → 下載。

**程式**

```typescript
// 神攝手 × micro:bit 範例 4：紅色三角形在哪邊（物體偵測）
// ★ 需要把相機選單切到 相機模式 5 物體偵測 ・子選項 ① 物體清單（模式編號 51）（程式開頭也會用 E0 自動切）
// 做什麼：相機把物體由大到小排好（物體 1 最大），每個物體給「物體代碼」＝色號 × 10 ＋ 形狀。從最大的往下找「目標 1 的三角形」（代碼 13）：
//   在左邊 → 往左箭頭、中間 → 向上箭頭、右邊 → 往右箭頭；沒找到 → 問號。按 A 顯示畫面裡有幾個物體，按 B 捲動物體 1 的形狀名稱。
// 形狀：0 其他 1 圓 2 橢圓 3 三角 4 正方 5 長方 6 五邊 7 六邊 8 星 9 梯形。
// 接線：相機 SDA → P20、SCL → P19、VCC → 外接 5 V（不要從 micro:bit 3V 取電）、GND → GND 共地。
// 準備：在相機機身把「目標 1」教成紅色（形狀可以不指定）。MakeCode 新專案 → 右上切到 JavaScript → 把本檔全部貼上 → 下載。不用加擴充。
// 看到什麼：開機打勾；拿紅色三角形在鏡頭前左右移動，箭頭跟著換方向；換成圓形或別的顏色 → 問號。
// ⚠ v1 尚未上機驗證：數值排法已定案並通過電腦端測試，實機數值上機後可能調整。

const MODE = 51                                   // 模式編號 = 相機模式 5 × 10 + 子選項 1
const WANT = 1 * 10 + 3                           // 👉 可以改：目標 1 × 10 ＋ 形狀 3（三角）；正方形 14、圓 11
let ready = false
let busy = false

basic.showIcon(IconNames.SmallDiamond)            // 等相機

// 從最大的物體往下找代碼 = WANT 的，回傳它的 X（0 = 沒找到）
function findX(): number {
    const n = ucObjectCount(MODE)                 // 畫面裡有幾個物體 0~8
    for (let i = 1; i <= n && i <= 8; i++) {
        const o = ucReadObject(MODE, i)           // [代碼, 色號, 形狀, x, y, …]
        if (o[0] == WANT) return o[3]
    }
    return 0
}

basic.forever(function () {
    if (!ready) {                                 // 第一次：等相機準備好，再切到模式 51
        if (ucReady()) {
            ucSetMode(5, 1)                       // E0：相機模式 5、子選項 1
            ready = true
            basic.showIcon(IconNames.Yes)
            basic.pause(500)
        } else {
            basic.showIcon(IconNames.No)          // 沒回應：檢查 P19／P20、外接電源
            basic.pause(500)
        }
        return
    }
    if (busy) {
        basic.pause(100)
        return
    }
    const x = findX()                             // 0 = 沒找到；1~159 左 → 右
    if (x <= 0) {
        basic.showString("?", 0)
    } else if (x < 60) {                          // 👉 可以改：左邊的範圍
        basic.showArrow(ArrowNames.West, 0)
    } else if (x > 100) {                         // 👉 可以改：右邊的範圍
        basic.showArrow(ArrowNames.East, 0)
    } else {
        basic.showArrow(ArrowNames.North, 0)
    }
    basic.pause(50)
})

input.onButtonPressed(Button.A, function () {
    busy = true
    basic.showNumber(ucObjectCount(MODE))         // 畫面裡有幾個物體 0~8
    busy = false
})

input.onButtonPressed(Button.B, function () {
    busy = true
    const o = ucReadObject(MODE, 1)               // 物體 1（最大的）
    basic.showString(o[0] > 0 ? ucShapeName(o[2]) : "-")   // 形狀名稱（英文）；沒有物體顯示 -
    busy = false
})

// ==== MBC_UNICAM_MB_HELPERS_BEGIN ====
// ===== 神攝手 小幫手（MBC 通用 I2C 協議 v2）：所有範例都一樣，不用改 =====
// ===== UniCam helpers (MBC common I2C protocol v2): same in every example, no need to change =====
const UC_ADDR = 0x4C                     // 相機的 I2C 位址 / camera I2C address
const UC_NONE = -32768                   // 讀不到 / no value

// 從編號 id 起連續讀 k 個 int16（1~16）；讀不到全部是 UC_NONE
function ucReadMany(id: number, k: number): number[] {
    const cmd = pins.createBuffer(1)
    cmd[0] = id
    const r = pins.i2cWriteBuffer(UC_ADDR, cmd, false)
    basic.pause(2)                       // 相機（ESP32）要等 2 ms 再讀
    const out: number[] = []
    if (r) {                             // 實機：非 0 = 沒有回應
        for (let i = 0; i < k; i++) out.push(UC_NONE)
        return out
    }
    const b = pins.i2cReadBuffer(UC_ADDR, 2 * k, false)
    for (let i = 0; i < k; i++) out.push(b.getNumber(NumberFormat.Int16LE, 2 * i))
    return out
}

// 寫命令 [id][參數…] / write a command
function ucWrite(id: number, params: number[]): void {
    const b = pins.createBuffer(1 + params.length)
    b[0] = id
    for (let i = 0; i < params.length; i++) b[1 + i] = params[i]
    pins.i2cWriteBuffer(UC_ADDR, b, false)
}

// 相機準備好了嗎（F3 bit0）/ is the camera ready
function ucReady(): boolean {
    const f = ucReadMany(0xF3, 1)[0]
    return f != UC_NONE && (f & 1) == 1
}

// 切相機模式（E0）：camMode 1~5、sub 子選項（照順序給色號填 0；物體偵測 1 清單、2 詳細）/ switch camera mode
function ucSetMode(camMode: number, sub: number): void {
    ucWrite(0xE0, [camMode, sub])
    basic.pause(50)
}

// 改取樣點位置（E1）：n 1~20、x 0~159、y 0~119 / move a sample spot
function ucSetPoint(n: number, x: number, y: number): void {
    ucWrite(0xE1, [n, x, y])
}

// 相機目前的模式編號（例 13）/ current mode number
function ucModeNumber(): number {
    return ucReadMany(0x01, 1)[0]
}

// 顏色追蹤每色幾個值 / values per colour (modes 11~14)
function ucStride(mode: number): number {
    if (mode == 11) return 2
    if (mode == 12) return 3
    if (mode == 13) return 4
    if (mode == 14) return 6
    return 0
}

// 色 color（1~16）的色塊：[x, y, 寬, 高, 塊數, 面積%]，x = 0 = 沒找到；這個模式沒給的是 -1
// blob of colour: [x, y, w, h, count, area%]; x = 0 = not found; -1 = not in this mode
function ucReadBlob(mode: number, color: number): number[] {
    const k = ucStride(mode)
    const d = ucReadMany(0x10 + (color - 1) * k, k)
    if (mode == 11) return [d[0], d[1], -1, -1, -1, -1]
    if (mode == 12) return [d[0], d[1], -1, -1, d[2], -1]
    if (mode == 13) return [d[0], d[1], -1, -1, d[3], d[2]]
    return [d[0], d[1], d[2], d[3], d[4], d[5]]
}

// 16 個目標色裡找到幾色 / colours found (modes 11~14)
function ucColorsFound(mode: number): number {
    return ucReadMany(0x10 + 16 * ucStride(mode), 1)[0]
}

// 取樣點 point（1~16）：[色相碼, 飽和度, 明度]；沒給的是 -1 / spot: [hue, sat, val]
function ucReadSpot(mode: number, point: number): number[] {
    const k = mode - 20
    const d = ucReadMany(0x10 + (point - 1) * k, k)
    return [d[0], k >= 2 ? d[1] : -1, k >= 3 ? d[2] : -1]
}

// 照順序給色號：由左到右每塊的色相碼（最多 16 塊）/ hue codes left to right
function ucReadOrder(): number[] {
    const total = ucReadMany(0x20, 1)[0]
    const d = ucReadMany(0x10, 16)
    if (total == UC_NONE) return []
    return d.slice(0, Math.min(16, total))
}

// 總塊數 / total blocks (mode 30)
function ucOrderTotal(): number {
    return ucReadMany(0x20, 1)[0]
}

// 循線 14 個值：[近, 中, 遠偏移, 角度, 線寬, 有線旗標, 路口類型, 路口距離, 左緣, 右緣, 遠線寬, 標記色號, 標記條數, 離線方向]
// line values (near/mid/far offset: line on the left = negative)
function ucReadLine(): number[] {
    return ucReadMany(0x10, 14)
}

// 物體偵測每個物體幾個值：51 → 4（色號 形狀 X Y）、52 → 10 / values per object (modes 51, 52)
function ucObjStride(mode: number): number {
    if (mode == 51) return 4
    if (mode == 52) return 10
    return 0
}

// 物體 i（1~8，1 = 面積最大）：[代碼, 色號, 形狀, x, y, 寬, 高, 面積%, 角度, 頂點數, 追蹤編號]
// 代碼 = 色號×10＋形狀，0 = 沒有；模式 51 沒給的是 -1
// object i: [code, colour, shape, x, y, w, h, area%, angle, vertices, trackId]; -1 = not in mode 51
function ucReadObject(mode: number, i: number): number[] {
    const k = ucObjStride(mode)
    const d = ucReadMany(0x10 + (i - 1) * k, k)
    const code = d[0] > 0 ? d[0] * 10 + d[1] : 0
    if (mode == 51) return [code, d[0], d[1], d[2], d[3], -1, -1, -1, -1, -1, -1]
    return [code, d[0], d[1], d[2], d[3], d[4], d[5], d[6], d[7], d[8], d[9]]
}

// 畫面裡有幾個物體 0~8 / number of objects in view (modes 51, 52)
function ucObjectCount(mode: number): number {
    return ucReadMany(0x10 + 8 * ucObjStride(mode), 1)[0]
}

// 形狀編號 → 英文名稱 / shape number -> name
function ucShapeName(s: number): string {
    const n = ["other", "circle", "ellipse", "triangle", "square", "rectangle", "pentagon", "hexagon", "star", "trapezoid"]
    return (s >= 0 && s <= 9) ? n[s] : "?"
}

// 色相碼 → 顏色代號：1 紅 2 橙 3 黃 4 綠 5 青 6 藍 7 紫 8 白／灰 9 黑
function ucColorCode(h: number): number {
    if (h > 248) return 9
    if (h > 240) return 8
    if (h < 12 || h >= 220) return 1
    if (h < 28) return 2
    if (h < 56) return 3
    if (h < 100) return 4
    if (h < 140) return 5
    if (h < 180) return 6
    return 7
}
// ==== MBC_UNICAM_MB_HELPERS_END ====
```

**一步一步看懂程式**
1. `WANT = 1 × 10 + 3`：物體代碼＝色號 × 10 ＋ 形狀，目標 1 的三角形＝13（正方形 14、圓 11）。
2. `ucObjectCount(MODE)`：畫面裡有幾個物體（0～8）。
3. `ucReadObject(MODE, i)` → `[代碼, 色號, 形狀, x, y, …]`；物體 1 最大，所以從 1 往下找到的第一個就是最大的。模式 51 只給代碼、色號、形狀、X、Y，後面幾格是 −1。
4. `ucShapeName(o[2])`：形狀編號換成英文名稱（LED 只能捲動英文）。

**跑起來應該看到什麼**：開機打勾；紅色三角形在左邊 → 往左箭頭、在右邊 → 往右箭頭；換成紅色圓形或藍色三角形 → 問號。按 B 會捲動 `triangle` 這樣的字。

**常見問題**
- **一直是問號？** → 相機螢幕右上要顯示 `5-1`；看相機畫面每個物體旁邊標的「色號 形狀」，三角形被判成別的形狀就在相機上給目標 1 指定 Tri，或在 SET 選單把 **ShapeTol** 調大。
- **想要大小、面積、角度？** → 改 `ucSetMode(5, 2)`、`MODE = 52`：`ucReadObject` 會多給寬、高、面積 %、角度 0～179、頂點數、追蹤編號。

---

## 下一步

- 每一個值的完整對照：[讀值參考（I2C）](/sensors/unicam/reference.md#host-i2c)
- 小幫手的正本、Arduino 版與其他平台：[下載中心](/downloads.md)
- 產品頁與相機選單操作：[神攝手](/sensors/unicam/index.md)
