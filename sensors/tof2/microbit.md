<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-reference.mjs 從 _build/protocol/tof2.json 產生（microbit），請勿手改；改內容請改 JSON 後重跑 node _build/gen-reference.mjs -->
<!-- moved: /sensors/tof2/microbit/makecode.md -->
<p class="moved-note">這頁內容已整合到新的說明頁：<a href="/sensors/tof2/microbit/makecode.md">測距者 2 路 › micro:bit › MakeCode</a></p>

# 測距者 2 路 × micro:bit（MakeCode 擴充）

[🔙 回到測距者 2 路產品頁](/sensors/tof2/index.md)

測距者 2 路通用 I2C 版可以接 BBC micro:bit V1／V2，用 MakeCode 擴充 **pxt-mbc-sensors v2** 的積木讀值（內部走 [MBC I2C 協議 v2](/i2c-protocol.md)）。擴充安裝、硬體注意事項（3.3 V、供電）請看 [micro:bit 總覽](/microbit.md)。

<p class="chips"><a class="chip chip-spike" href="/sensors/tof2/index.md">SPIKE</a><a class="chip chip-ev3" href="/sensors/tof2/index.md">EV3</a><a class="chip chip-arduino" href="/sensors/tof2/arduino-i2c.md">Arduino I2C</a><a class="chip chip-mblock" href="/sensors/tof2/mblock.md">mBlock</a><a class="chip chip-matrix" href="/sensors/tof2/matrix.md">MATRIX</a><a class="chip chip-microbit current" href="/sensors/tof2/microbit.md">micro:bit</a></p>

> [!IMPORTANT]
> 擴充 v2 尚未上機驗證；積木名稱以發布版為準： <https://github.com/contactmbcrobot/pxt-mbc-sensors>

## 接線

<figure class="ref-fig"><img src="/images/figures/tof2/wiring-microbit.svg" alt="測距者 2 路 × micro:bit" loading="lazy"></figure>

| 感應器 | micro:bit |
| :--- | :--- |
| SDA | P20 |
| SCL | P19 |
| 5V | 外接 5 V（或 V2 的 3V 腳，需確認電流） |
| GND | GND |

## 讀值積木

| 積木 | 傳回 | 編號 | 說明 |
| :--- | :--- | :--- | :--- |
| MBC 左距離 cm | 0~120 cm | [`01`](/sensors/tof2/reference.md#v01) | 左邊那顆的距離 0~120 cm（站在車後往前看，左 = 第 0 顆），120 = 前面沒東西／那顆失效 |
| MBC 右距離 cm | 0~120 cm | [`02`](/sensors/tof2/reference.md#v02) | 右邊那顆的距離 0~120 cm（右 = 第 1 顆），120 = 前面沒東西／那顆失效 |
| MBC 較近距離 cm | 0~120 cm | [`03`](/sensors/tof2/reference.md#v03) | 左右兩顆取較近的 0~120 cm |
| MBC 左距離 mm | 0~1200 mm | [`04`](/sensors/tof2/reference.md#v04) | 左邊那顆的距離 0~1200 mm，1200 = 前面沒東西／那顆失效 |
| MBC 右距離 mm | 0~1200 mm | [`05`](/sensors/tof2/reference.md#v05) | 右邊那顆的距離 0~1200 mm，1200 = 前面沒東西／那顆失效 |
| MBC 初始化位元圖 | 0~3 bit | [`06`](/sensors/tof2/reference.md#v06) | bit0 = 左、bit1 = 右，1 = 那顆開機初始化成功（失敗不重試）；3 = 兩顆都好 |
| MBC 故障位元圖 | 0~3 bit | [`07`](/sensors/tof2/reference.md#v07) | bit0 = 左、bit1 = 右，1 = 那顆目前失效（I2C 讀失敗或 200 ms 沒更新）；失效那顆距離回 120／1200 |
| MBC 左右平均 mm | 0~1200 mm | [`08`](/sensors/tof2/reference.md#v08) | (左 mm + 右 mm) / 2，0~1200；跟 EV3 模式 3、SPIKE 模式 6 第 3 個值一致（共識 #8 補上） |

## 狀態

| bit | 名稱 | 說明 |
| :--- | :--- | :--- |
| b0 | 就緒 (`ready`) | 2 顆都初始化完、快照開始更新後恆 1 |
| b1 | 任一顆失效 (`fault`) | 初始化失敗、I2C 讀失敗或 200 ms 沒更新，任一顆就是 1（= 07 ≠ 0 或 06 ≠ 3） |

## JavaScript 寫法

```typescript
basic.forever(function () {
    let leftCm = MBC.readValue(0x29, 0x01)   // 左距離 cm
    basic.showNumber(leftCm)
    basic.pause(100)
})
```

## 本頁依據與假設

本頁由 `_build/gen-reference.mjs` 從 `_build/protocol/tof2.json`（單一真相來源，轉錄自 2026-10-21 協定定案）產生。

- 共識 #8：08 左右平均 mm 補進 I2C／mBlock 值表（定案 §5 已列）。
- 0x22 命令 4~12 不動（4 左、5 右、6 最近、7~11 = 0、12 = 最近）；schema 不收 11/12，所以 12 只寫在 03 的 note。
- EV3：模式 0、2、4 鎖住（照盤點）；模式 1 改 [左,右]×4（index 0/1 各代表左右）、模式 3 改平均 mm、模式 5 改 [左 mm, 右 mm]；模式 1 描述表宣告 int8 −100~100，cm×100/120 在範圍內。
- SPIKE：0/5/7/9 鎖住，另加模式 3 鎖住（網站 sensors/tof2/index.md read(3) = 左／右 cm，共識 #11）；模式 6 改 [左 mm, 右 mm, 平均 mm]；模式 1、2、4、8 維持不動；模式 8 v[0] 改 ID|hw[0]<<8（統一後共用函式）。
- 「左」= 站在車後往前看的左（第 0 顆，XSHUT PA6）；韌體只有 L/R 巨集名，盤點檔標為推測。
- 沒有寫入命令；F8 換位址／EF 重開機用共用的。
- 擴充內部用 `Int16LE` 讀、寫完編號 `basic.pause(2)`（D5）。
