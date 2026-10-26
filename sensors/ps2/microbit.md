<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-reference.mjs 從 _build/protocol/ps2.json 產生（microbit），請勿手改；改內容請改 JSON 後重跑 node _build/gen-reference.mjs -->
<!-- moved: /sensors/ps2/microbit/makecode.md -->
<p class="moved-note">這頁內容已整合到新的說明頁：<a href="/sensors/ps2/microbit/makecode.md">掌控者 PS2 › micro:bit › MakeCode</a></p>

# 掌控者 PS2 × micro:bit（MakeCode 擴充）

[🔙 回到掌控者 PS2產品頁](/sensors/ps2/index.md)

掌控者 PS2通用 I2C 版可以接 BBC micro:bit V1／V2，用 MakeCode 擴充 **pxt-mbc-sensors v2** 的積木讀值（內部走 [MBC I2C 協議 v2](/i2c-protocol.md)）。擴充安裝、硬體注意事項（3.3 V、供電）請看 [micro:bit 總覽](/microbit.md)。

<p class="chips"><a class="chip chip-spike" href="/sensors/ps2/index.md">SPIKE</a><a class="chip chip-ev3" href="/sensors/ps2/index.md">EV3</a><a class="chip chip-arduino" href="/sensors/ps2/arduino-i2c.md">Arduino I2C</a><a class="chip chip-mblock" href="/sensors/ps2/mblock.md">mBlock</a><a class="chip chip-matrix" href="/sensors/ps2/matrix.md">MATRIX</a><a class="chip chip-microbit current" href="/sensors/ps2/microbit.md">micro:bit</a></p>

> [!IMPORTANT]
> 擴充 v2 尚未上機驗證；積木名稱以發布版為準： <https://github.com/contactmbcrobot/pxt-mbc-sensors>

## 接線

<figure class="ref-fig"><img src="/images/figures/ps2/wiring-microbit.svg" alt="掌控者 PS2 × micro:bit" loading="lazy"></figure>

| 感應器 | micro:bit |
| :--- | :--- |
| SDA（RX） | P20 |
| SCL（TX） | P19 |
| VCC | micro:bit 3V（電池供電時建議外接 5 V） |
| GND | GND |

## 讀值積木

| 積木 | 傳回 | 編號 | 說明 |
| :--- | :--- | :--- | :--- |
| 掌控者 十六鍵位元圖 | 0~65535 bit | [`01`](/sensors/ps2/reference.md#v01) | 16 個按鍵合成一個數（PSX 排列，1 = 按下）：bit0 SELECT、1 L3、2 R3、3 START、4 上、5 右、6 下、7 左、8 L2、9 R2、10 L1、11 R1、12 ▲、13 ●、14 ✕、15 ■；未連線 = 0 |
| 掌控者 ［方向鍵］（0~15） | 0~15 bit | [`02`](/sensors/ps2/reference.md#v02) | 上 1、右 2、下 4、左 8（同時按相加），0~15；未連線 = 0 |
| 掌控者 ［形狀鍵］（0~15） | 0~15 bit | [`03`](/sensors/ps2/reference.md#v03) | ▲ 1、● 2、✕ 4、■ 8（同時按相加），0~15；未連線 = 0；0x22 的 R 高 4 bit |
| 掌控者 ［肩鍵］（0~15） | 0~15 bit | [`04`](/sensors/ps2/reference.md#v04) | L2 1、R2 2、L1 4、R1 8（同時按相加），0~15；未連線 = 0 |
| 掌控者 ［功能鍵］（0~15） | 0~15 bit | [`05`](/sensors/ps2/reference.md#v05) | SELECT 1、L3 2、R3 4、START 8（同時按相加），0~15；未連線 = 0；0x22 的 G 高 4 bit |
| 掌控者 手把已連線？ | 0~1 | [`06`](/sensors/ps2/reference.md#v06) | 1 = 手把已連線（psx.read() 成功），0 = 沒有；未連線時所有按鍵 0、搖桿回中 |
| 掌控者 電量（百分比） | 0 % | [`07`](/sensors/ps2/reference.md#v07) | 0~100；PS2 手把沒有電量 → 恆 0（PS4 才有） |
| 掌控者 扳機 ［L2］ 類比值 | 0~255 | [`08`](/sensors/ps2/reference.md#v08) | L2 壓力 0~255（DualShock 2 類比模式才有壓力值；手把沒有壓力值時按下 = 255、放開 = 0）；未連線 = 0 |
| 掌控者 扳機 ［R2］ 類比值 | 0~255 | [`09`](/sensors/ps2/reference.md#v09) | R2 壓力 0~255（同 08）；未連線 = 0 |
| 掌控者 手把類型 | 0~2 | [`0B`](/sensors/ps2/reference.md#v0b) | 0 沒有手把、1 PS2 數位模式、2 PS2 類比模式（3 PS4、4 PS5 只有 PS4 接收器會出現） |
| 掌控者 搖桿 ［左搖桿 X／左搖桿 Y／右搖桿 X／右搖桿 Y］ | -100~+100 | [`10~13`](/sensors/ps2/reference.md#v10) | 10 LX、11 LY、12 RX、13 RY：−100 ~ +100，0 = 中間，右／上為正（原始 0~255 死區 ±7 吸成中心）；未連線 = 0。EV3／SPIKE／MATRIX 送的是 0~99（50 = 中間）或 1~9（5 = 中間），見各平台欄 |

### 其他積木（是／否、事件）

| 積木 | 相關的值 |
| :--- | :--- |
| 掌控者 按鈕 ［✕］ 被按住？ | [`01`](/sensors/ps2/reference.md#v01) |
| 當掌控者 按鈕 ［✕］ ［按下］ | [`01`](/sensors/ps2/reference.md#v01) |

## 命令積木

| 積木 | 編號 | 說明 |
| :--- | :--- | :--- |
| 掌控者 震動 強度 200 | [`E5`](/sensors/ps2/reference.md#ce5) | [強度 0~255]，0 = 停；PS2 接收器收到後回 ACK 但忽略（250614 已關手把震動），PS4 才有作用 |
| 掌控者 燈條顏色 #ff0000 | [`E6`](/sensors/ps2/reference.md#ce6) | [R][G][B] 各 0~255；PS2 手把沒有燈條，接收器收到後回 ACK 但忽略，PS4 才有作用 |

## 狀態

| bit | 名稱 | 說明 |
| :--- | :--- | :--- |
| b0 | 就緒 (`ready`) | 恆 1 |
| b1 | 手把已連線 (`connected`) | = 06 已連線 |

## JavaScript 寫法

```typescript
if (!(mbc.waitReady(0x42, 3000))) {
    basic.showIcon(IconNames.No)   // 讀不到：檢查接線與電源
}
basic.forever(function () {
    basic.showNumber(mbc.padButtons())   // 十六鍵
    basic.pause(100)
})
```

## 本頁依據與假設

本頁由 `_build/gen-reference.mjs` 從 `_build/protocol/ps2.json`（單一真相來源，轉錄自 2026-10-21 協定定案）產生。

- hw[0] = 1（PS2）、電量 07 恆 0、系統鍵 0A 恆 0、手把類型 0B 只會出現 0／1／2（I2C 定案 §2、§4）。
- 08／09 L2／R2 類比照定案寫 0~255：DS2 類比模式有壓力值；沒有壓力值時按下 = 255、放開 = 0（沿用 v1 0x14 的做法，盤點 .ino:598-603）。
- E5 震動、E6 燈條照 I2C 定案 §3 列在 PS2／PS4 共用命令表；PS2 收到回 ACK 後忽略（跟 v1 0x10／0x23 一樣）。
- 搖桿 10~13 在 I2C／mBlock 是 ±100（由 raw (v−128)×100/128、Y 上為正）；EV3／0x22 維持 0~99（50 中間）、SPIKE 維持 1~9（5 中間）照盤點，不改鎖住模式。
- EV3：模式 0、1、2、4 鎖住（照盤點）；模式 3 改 方向|形狀<<4|已連線<<8（原本沒有 bit8）；模式 5 改成 ID 格式 B（原本 = 按鍵／搖桿合併版）。
- SPIKE：0/5/7/9 鎖住，1、2、4、6 維持不動；模式 3 改 [已連線, 電量(0), 0]（原本 = 4 組按鍵）；模式 8 改 v[0] = ID|hw[0]<<8（原本 = 搖桿）。模式 9 v[6] button16 = PSX 排列（D3）。
- SPIKE 模式 5 只容得下 11 鍵（L2、R2、L3、R3、START 讀不到），鎖住不改。
- 擴充內部用 `Int16LE` 讀、寫完編號 `basic.pause(2)`（D5）；感應器沒接好時讀值積木回 0、是／否積木回「否」。
