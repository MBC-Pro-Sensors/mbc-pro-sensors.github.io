<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-reference.mjs 從 _build/protocol/line16.json 產生（microbit），請勿手改；改內容請改 JSON 後重跑 node _build/gen-reference.mjs -->
<!-- moved: /sensors/line16/microbit/makecode.md -->
<p class="moved-note">這頁內容已整合到新的說明頁：<a href="/sensors/line16/microbit/makecode.md">循行者 16 路 › micro:bit › MakeCode</a></p>

# 循行者 16 路 × micro:bit（MakeCode 擴充）

[🔙 回到循行者 16 路產品頁](/sensors/line16/index.md)

循行者 16 路通用 I2C 版可以接 BBC micro:bit V1／V2，用 MakeCode 擴充 **pxt-mbc-sensors v2** 的積木讀值（內部走 [MBC I2C 協議 v2](/i2c-protocol.md)）。擴充安裝、硬體注意事項（3.3 V、供電）請看 [micro:bit 總覽](/microbit.md)。

<p class="chips"><a class="chip chip-spike" href="/sensors/line16/spike-hub.md">SPIKE</a><a class="chip chip-ev3" href="/sensors/line16/ev3-hub.md">EV3</a><a class="chip chip-arduino" href="/sensors/line16/arduino-i2c.md">Arduino I2C</a><a class="chip chip-mblock" href="/sensors/line16/mblock.md">mBlock</a><a class="chip chip-matrix" href="/sensors/line16/matrix.md">MATRIX</a><a class="chip chip-microbit current" href="/sensors/line16/microbit.md">micro:bit</a></p>

> [!IMPORTANT]
> 擴充 v2 尚未上機驗證；積木名稱以發布版為準： <https://github.com/contactmbcrobot/pxt-mbc-sensors>

## 接線

<figure class="ref-fig"><img src="/images/figures/line16/wiring-microbit.svg" alt="循行者 16 路 × micro:bit" loading="lazy"></figure>

| 感應器 | micro:bit |
| :--- | :--- |
| SDA（RX） | P20 |
| SCL（TX） | P19 |
| VCC | micro:bit 3V（電池供電時建議外接 5 V） |
| GND | GND |

## 讀值積木

| 積木 | 傳回 | 編號 | 說明 |
| :--- | :--- | :--- | :--- |
| 循行者 線位置 | -16~+16 | [`01`](/sensors/line16/reference.md#v01) | 線在哪一路：−16（最左）~+16（最右），0 = 正中間；站在車後往前看，負 = 線在左（平常 ±15，±16 只在最外側掉線後 200 ms 內） |
| 循行者 細位置 | -100~+100 | [`02`](/sensors/line16/reference.md#v02) | 線位置的細分：−100 ~ +100，0 = 正中間，負 = 線在左（與線位置同號） |
| 循行者 線寬 | 0~16 路 | [`03`](/sensors/line16/reference.md#v03) | 壓到線的路數 0~16，0 = 找不到線 |
| 循行者 線段數 | 0~8 | [`04`](/sensors/line16/reference.md#v04) | 看到幾段線：0 丟線、1 一條線、2 以上 = 分岔／十字 |
| 循行者 壓線位元圖 | 0~65535 bit | [`05`](/sensors/line16/reference.md#v05) | bit0 = 第 1 路（最左）… bit15 = 第 16 路，1 = 那一路在線上 |
| 循行者 最後離線方向 | -16~+16 | [`06`](/sensors/line16/reference.md#v06) | 丟線前線最後在哪一側：−16 ~ +16（負 = 左），找回線後維持最後一次 |
| 循行者 門檻 | 1~99 | [`07`](/sensors/line16/reference.md#v07) | 判斷「在線上」的光值門檻 1~99，預設 50；用 E4 改 |
| 循行者 校準次數 | 0~255 次 | [`08`](/sensors/line16/reference.md#v08) | 開機後完成校準的次數（按鈕校準也算） |
| 循行者 第 ［1~16］ 路光值 | 0~100 | [`10~1F`](/sensors/line16/reference.md#v10) | 第 1~16 路校準後光值 0~100（第 1 路最左）；編號 10~1F |

### 其他積木（是／否、事件）

| 積木 | 相關的值 |
| :--- | :--- |
| 循行者 看到線？ | [`04`](/sensors/line16/reference.md#v04) |
| 當循行者 ［丟線］ | [`04`](/sensors/line16/reference.md#v04) |
| 循行者 第 1 路在線上？ | [`05`](/sensors/line16/reference.md#v05) |
| 循行者 校準中？ | [`08`](/sensors/line16/reference.md#v08) |

## 命令積木

| 積木 | 編號 | 說明 |
| :--- | :--- | :--- |
| 循行者 循 ［黑］ 線 | [`E0`](/sensors/line16/reference.md#ce0) | 追黑線（預設） |
| 循行者 循 ［白］ 線 | [`E1`](/sensors/line16/reference.md#ce1) | 追白線（F3 b1 = 1） |
| 循行者 校準 ［目前讀值記為黑］ | [`E2`](/sensors/line16/reference.md#ce2) | 把目前讀值記為黑（感應器壓在黑線上），約 50 ms；寫入 EEPROM |
| 循行者 校準 ［目前讀值記為白］ | [`E3`](/sensors/line16/reference.md#ce3) | 把目前讀值記為白（感應器壓在白底上），約 50 ms；寫入 EEPROM |
| 循行者 門檻設為 50 | [`E4`](/sensors/line16/reference.md#ce4) | 二值化門檻 1~99（RAM，重開機回預設 50） |
| 循行者 校準 ［自動（來回掃 5 秒）］ | [`E7`](/sensors/line16/reference.md#ce7) | 5 秒內把感應器在黑線與白底上來回掃，取極值當黑白參考（共識 B-12，= v1 的 0x20=0x01）；期間 F3 b2 = 1 |

## 狀態

| bit | 名稱 | 說明 |
| :--- | :--- | :--- |
| b0 | 就緒 (`ready`) | 恆 1 |
| b1 | 白線模式 (`whiteMode`) | 1 = 正在追白線 |
| b2 | 校準中 (`calibrating`) | E2／E3／E7 執行中；此時拒收新的校準命令 |
| b3 | 丟線 (`lost`) | 線段數 = 0 |
| b4 | 分岔 (`fork`) | 線段數 ≥ 2 |

## JavaScript 寫法

```typescript
if (!(mbc.waitReady(0x16, 3000))) {
    basic.showIcon(IconNames.No)   // 讀不到：檢查接線與電源
}
basic.forever(function () {
    basic.showNumber(mbc.linePosition())   // 線位置
    basic.pause(100)
})
```

## 本頁依據與假設

本頁由 `_build/gen-reference.mjs` 從 `_build/protocol/line16.json`（單一真相來源，轉錄自 2026-10-21 協定定案）產生。

- B-11：細位置 02 與線位置 01 同號（負 = 左）；韌體 F16:1290 註解寫反，程式碼同號（IR:11 為準）。
- B-12：E7 五秒動態校準補回（v1 的 0x20=0x01），定案表沒列。
- EV3：模式 0~2 鎖住；模式 3 原本不送 → 16 bit 位元圖；模式 4 原本不送 → 同模式 2；模式 5 ID 格式 B（共用函式）。
- SPIKE：0/5/7/9 鎖住，1~4、6 維持不動；教育版模式 0 的 16 路光值是 2-bit（玩具版 1-bit），跟 8 路的 4-bit 不同；byte 8~15 補 0（#65 已改，使用者保留）；模式 9 排法照盤點（四路 4-bit 一個 int16，v6 = 位元圖）；模式 8 v[0] 改 ID|hw[0]<<8。
- 線段數 04 範圍：定案沒寫，16 路最多 8 段（交錯壓線），採 0~8。
- mBlock 舊版：MBC_MBLOCK_LEGACY=1 時逐 byte 同 2026-10-21 統一後韌體（0x01 位置、0x02 線寬、n<<4|03 反向兩路壓一 byte、0x07 細位置 0~200），0x0B 回 1（B-1）。
- Flash：C031 硬體鎖 28 KB，I2C 版餘 848 B；不夠時照 B-17 順序瘦身，JSON 不受影響。
- 擴充內部用 `Int16LE` 讀、寫完編號 `basic.pause(2)`（D5）；感應器沒接好時讀值積木回 0、是／否積木回「否」。
