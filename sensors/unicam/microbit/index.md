<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# micro:bit 讀值教學

> [!WARNING]
> **v1 尚未上機驗證**：數值排法已定案並通過電腦端測試，實機數值（fps、循線門檻、I2C 腳位）上機後可能調整。

神攝手（通用 I2C 版）跟 Arduino 用同一套 I2C 讀法，micro:bit V1／V2 在 MakeCode 用內建的 I2C 指令就能讀，位址 **`0x4C`**。值表、命令跟 [Arduino 讀值教學](/sensors/unicam/arduino/index.md) 一樣；每一格的完整對照在 [讀值參考：I2C](/sensors/unicam/reference.md#host-i2c)，整支範例在 [micro:bit 範例](/sensors/unicam/microbit-examples.md)。

## 接線

1. 相機的 SDA 接擴充板的 **P20**、SCL 接 **P19**、GND 接 GND。
2. **一定要外接 5 V 供電**（ESP32 相機＋螢幕耗電大），不能吃 micro:bit 的 3V 腳；外部電源的 GND 要和 micro:bit 共地。

> [!CAUTION]
> **I2C 腳位暫定，待上機**：相機端目前是 SDA＝IO3、SCL＝IO1，要等實測後才定案。

## 最小讀值程式（MakeCode JavaScript）

切到**顏色追蹤 ③ 標準**，把色 1 的位置畫在 LED 上（值 1～4＝`0x10`～`0x13`：X、Y、面積 %、數量）：

```typescript
const CAM = 0x4C

// 從編號 id 起連續讀 k 個值（k ≤ 16），每個值 int16 低位元組在前
function camRead(id: number, k: number): number[] {
    pins.i2cWriteNumber(CAM, id, NumberFormat.UInt8LE, false)
    basic.pause(2)
    const buf = pins.i2cReadBuffer(CAM, 2 * k, false)
    const out: number[] = []
    for (let i = 0; i < k; i++) out.push(buf.getNumber(NumberFormat.Int16LE, 2 * i))
    return out
}

// 命令 E0：切到 顏色追蹤 ③ 標準
const cmd = pins.createBuffer(3)
cmd[0] = 0xE0
cmd[1] = 1
cmd[2] = 3
pins.i2cWriteBuffer(CAM, cmd, false)

basic.forever(function () {
    const v = camRead(0x10, 4)          // 色 1：X、Y、面積、數量
    basic.clearScreen()
    if (v[0] > 0) {
        led.plot(Math.idiv(v[0] * 5, 160), Math.idiv(v[1] * 5, 120))
    }
    basic.pause(100)
})
```

貼到 MakeCode 的 JavaScript 分頁就能用；切回積木分頁會看到對應的積木。MBC 的 MakeCode 擴充（pxt-mbc-sensors）還沒有這一代神攝手的積木。

## 相機模式 5 物體偵測

相機把畫面裡的物體由大到小排好（物體 1 最大），每個物體給**物體代碼＝色號 × 10 ＋ 形狀**（例 13＝目標 1 的三角形，0＝沒有）。形狀：0 其他、1 圓、2 橢圓、3 三角、4 正方、5 長方、6 五邊、7 六邊、8 星、9 梯形。X、Y 是物體的**質心**；只看目標色 1～8。模式編號 **51＝① 物體清單**、**52＝② 物體詳細**。代碼再拆：色號＝代碼 ÷ 10（取整數）、形狀＝代碼除以 10 的餘數。

值表跟 Arduino 一樣：51 每個物體 4 個值（色號、形狀、X、Y），物體數在 `0x30`；52 每個物體 10 個值（再加寬、高、面積 %、角度、頂點數、追蹤編號），物體數在 `0x60`（[Arduino 讀值教學](/sensors/unicam/arduino/index.md) 有完整表）。下面切到物體清單（51），把最大的物體畫在 LED 上，按 A 顯示它的代碼：

```typescript
const CAM = 0x4C

// 從編號 id 起連續讀 k 個值（k ≤ 16），每個值 int16 低位元組在前
function camRead(id: number, k: number): number[] {
    pins.i2cWriteNumber(CAM, id, NumberFormat.UInt8LE, false)
    basic.pause(2)
    const buf = pins.i2cReadBuffer(CAM, 2 * k, false)
    const out: number[] = []
    for (let i = 0; i < k; i++) out.push(buf.getNumber(NumberFormat.Int16LE, 2 * i))
    return out
}

// 命令 E0 [5, 1]：切到 物體偵測 ① 物體清單
const cmd = pins.createBuffer(3)
cmd[0] = 0xE0
cmd[1] = 5
cmd[2] = 1
pins.i2cWriteBuffer(CAM, cmd, false)

basic.forever(function () {
    const n = camRead(0x30, 1)[0]       // 0x30：物體數（模式 51）
    const v = camRead(0x10, 4)          // 物體 1（最大）：色號、形狀、X、Y
    basic.clearScreen()
    if (n > 0) {
        led.plot(Math.idiv(v[2] * 5, 160), Math.idiv(v[3] * 5, 120))
    }
    basic.pause(100)
})

input.onButtonPressed(Button.A, function () {
    const v = camRead(0x10, 2)
    basic.showNumber(v[0] * 10 + v[1])  // 物體 1 代碼＝色號 × 10 ＋ 形狀（0＝沒有）
})
```

範例裡的小幫手 `ucReadObject(mode, i)`、`ucObjectCount(mode)`、`ucShapeName(s)` 已經包好這些，看 [micro:bit 範例 4](/sensors/unicam/microbit-examples.md)。

## 逐格對照與範例

- [讀值參考：I2C](/sensors/unicam/reference.md#host-i2c)（值 1＝`0x10`）
- [micro:bit 範例程式](/sensors/unicam/microbit-examples.md)
- 值表與命令：[Arduino 讀值教學](/sensors/unicam/arduino/index.md)

## 常見問題

- **讀到的都是 0 或怪數字？** 先確認外接 5 V 有開、GND 有共地，再讀 `0x01` 看相機模式對不對。
- **讀到 −32768？** 那個編號在目前的相機模式沒有定義。
