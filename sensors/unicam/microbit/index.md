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

## 逐格對照與範例

- [讀值參考：I2C](/sensors/unicam/reference.md#host-i2c)（值 1＝`0x10`）
- [micro:bit 範例程式](/sensors/unicam/microbit-examples.md)
- 值表與命令：[Arduino 讀值教學](/sensors/unicam/arduino/index.md)

## 常見問題

- **讀到的都是 0 或怪數字？** 先確認外接 5 V 有開、GND 有共地，再讀 `0x01` 看相機模式對不對。
- **讀到 −32768？** 那個編號在目前的相機模式沒有定義。
