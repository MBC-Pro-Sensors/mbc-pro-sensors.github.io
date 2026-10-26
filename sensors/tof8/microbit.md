<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-reference.mjs 從 _build/protocol/tof8.json 產生（microbit），請勿手改；改內容請改 JSON 後重跑 node _build/gen-reference.mjs -->
<!-- moved: /sensors/tof8/microbit/makecode.md -->
<p class="moved-note">這頁內容已整合到新的說明頁：<a href="/sensors/tof8/microbit/makecode.md">測距者 8 路 › micro:bit › MakeCode</a></p>

# 測距者 8 路 × micro:bit（MakeCode 擴充）

[🔙 回到測距者 8 路產品頁](/sensors/tof8/index.md)

測距者 8 路通用 I2C 版可以接 BBC micro:bit V1／V2，用 MakeCode 擴充 **pxt-mbc-sensors v2** 的積木讀值（內部走 [MBC I2C 協議 v2](/i2c-protocol.md)）。擴充安裝、硬體注意事項（3.3 V、供電）請看 [micro:bit 總覽](/microbit.md)。

<p class="chips"><a class="chip chip-spike" href="/sensors/tof8/index.md">SPIKE</a><a class="chip chip-ev3" href="/sensors/tof8/index.md">EV3</a><a class="chip chip-arduino" href="/sensors/tof8/arduino-i2c.md">Arduino I2C</a><a class="chip chip-mblock" href="/sensors/tof8/mblock.md">mBlock</a><a class="chip chip-matrix" href="/sensors/tof8/matrix.md">MATRIX</a><a class="chip chip-microbit current" href="/sensors/tof8/microbit.md">micro:bit</a></p>

> [!IMPORTANT]
> 擴充 v2 尚未上機驗證；積木名稱以發布版為準： <https://github.com/contactmbcrobot/pxt-mbc-sensors>

## 接線

<figure class="ref-fig"><img src="/images/figures/tof8/wiring-microbit.svg" alt="測距者 8 路 × micro:bit" loading="lazy"></figure>

| 感應器 | micro:bit |
| :--- | :--- |
| SDA（RX） | P20 |
| SCL（TX） | P19 |
| VCC | **一定要外接 5 V**（GND 要和 micro:bit 共地） |
| GND | GND |

## 讀值積木

| 積木 | 傳回 | 編號 | 說明 |
| :--- | :--- | :--- | :--- |
| 測距者 8 路 最近距離（［公分］） | 0~120 cm | [`01`](/sensors/tof8/reference.md#v01) | 8 顆裡最小的距離 0~120 cm（失效那顆以 120 參與），120 = 四周沒東西 |
| 測距者 8 路 最近方向 | -8~+8 | [`02`](/sensors/tof8/reference.md#v02) | 最近目標在哪個方向：−7（正左）~ +7（正右），0 = 正前方，負 = 左；±8 = 四周沒東西（沿用上次偏向側） |
| 測距者 8 路 最空曠距離（公分） | 0~120 cm | [`03`](/sensors/tof8/reference.md#v03) | 8 顆裡最大的距離 0~120 cm（壞掉那顆永遠 120，會被當成最空曠） |
| 測距者 8 路 最空曠方向 | -7~+7 | [`04`](/sensors/tof8/reference.md#v04) | 最遠（最空曠）在哪個方向：−7 ~ +7，0 = 正前方，負 = 左；8 顆全 120 時 = 0 |
| 測距者 8 路 正前方距離（［公分］） | 0~120 cm | [`05`](/sensors/tof8/reference.md#v05) | 第 4、5 顆（兩顆平行朝前）平均 0~120 cm；EV3 模式 3、SPIKE 模式 6 給的是同一個值的 mm；0x22 的 C 是 min(第 4, 第 5 顆) 不是平均（見 10） |
| 測距者 8 路 最近距離（［公釐］） | 0~1200 mm | [`06`](/sensors/tof8/reference.md#v06) | 8 顆 mm 的最小值 0~1200（失效那顆以 1200 參與），1200 = 四周沒東西 |
| 測距者 8 路 正前方距離（［公釐］） | 0~1200 mm | [`09`](/sensors/tof8/reference.md#v09) | 第 4、5 顆（正前方兩顆）平均，0~1200 mm；1200 = 前面沒東西。cm 版是 05 |
| 測距者 8 路 第 ［1~8］ 顆距離（［公分］） | 0~120 cm | [`10~17`](/sensors/tof8/reference.md#v10) | 第 1~8 顆各自的距離 0~120 cm（第 1 顆正左、第 4、5 顆正前、第 8 顆正右，每顆隔 30°）；120 = 那個方向沒東西／那顆失效；編號 10~17 |
| 測距者 8 路 第 ［1~8］ 顆距離（［公釐］） | 0~1200 mm | [`18~1F`](/sensors/tof8/reference.md#v18) | 第 1~8 顆各自的距離 0~1200 mm，1200 = 那個方向沒東西／那顆失效；編號 18~1F |

### 其他積木（是／否、事件）

| 積木 | 相關的值 |
| :--- | :--- |
| 當測距者 8 路 距離小於 10 公分 | [`01`](/sensors/tof8/reference.md#v01) |

## 狀態

| bit | 名稱 | 說明 |
| :--- | :--- | :--- |
| b0 | 就緒 (`ready`) | 8 顆都初始化完（開機約 2~4 秒）、快照開始更新後恆 1 |
| b1 | 任一顆失效 (`fault`) | 初始化失敗、I2C 讀失敗或 200 ms 沒更新，任一顆就是 1（= 08 ≠ 0 或 07 ≠ 255） |

## JavaScript 寫法

```typescript
if (!(mbc.waitReady(0x29, 5000))) {
    basic.showIcon(IconNames.No)   // 讀不到：檢查接線與電源
}
basic.forever(function () {
    basic.showNumber(mbc.tof8Nearest(MbcDistUnit.Cm))   // 最近距離 cm
    basic.pause(100)
})
```

## 本頁依據與假設

本頁由 `_build/gen-reference.mjs` 從 `_build/protocol/tof8.json`（單一真相來源，轉錄自 2026-10-21 協定定案）產生。

- 正前方 mm（EV3 模式 3、SPIKE 模式 6 v[1]）在定案 §5 沒有獨立 I2C 編號 → 掛在 05 正前方 cm 底下，用 scale:mm 標示，不自己加編號。
- 0x22（使用者 2026-10-21 最終定案）：只用一個位址，R~K 由左到右 7 個 cm：第 1、2、3 顆、min(第 4, 第 5)、第 6、7、8 顆；11、12 不放資料；最近距離／方向由主機自己比較。此排法只用在 0x22，EV3／SPIKE不動。
- SPIKE：0/5/7/9 鎖住，1、2、3、4、8 維持不動；模式 6 改 [最近 mm, 正前方 mm, 最近方向 −8~+8]（原本 = 第 2~7 顆兩兩合併）；模式 8 v[0] 改 ID|hw[0]<<8。
- 最空曠方向 04 範圍 −7~+7（maxDirection 永遠不會是 0／16，盤點 INO:1086-1088）；最近方向 02 ±8 = 沒目標、沿用上次偏向側，開機預設 −8。
- 沒有寫入命令；F8 換位址／EF 重開機用共用的。
- 主對話 2026-10-21：補 09 正前方 mm（定案 §5 只有 05 正前方 cm，但 EV3 模式 3、SPIKE 模式 6 都送 mm，I2C／mBlock 應有同一個值）。
- 擴充內部用 `Int16LE` 讀、寫完編號 `basic.pause(2)`（D5）；感應器沒接好時讀值積木回 0、是／否積木回「否」。
