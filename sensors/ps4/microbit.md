<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-reference.mjs 從 _build/protocol/ps4.json 產生（microbit），請勿手改；改內容請改 JSON 後重跑 node _build/gen-reference.mjs -->
<!-- moved: /sensors/ps4/microbit/makecode.md -->
<p class="moved-note">這頁內容已整合到新的說明頁：<a href="/sensors/ps4/microbit/makecode.md">掌控者 PS4/PS5 › micro:bit › MakeCode</a></p>

# 掌控者 PS4／PS5 接收器 × micro:bit（MakeCode 擴充）

[🔙 回到掌控者 PS4／PS5 接收器產品頁](/sensors/ps4/index.md)

掌控者 PS4／PS5 接收器通用 I2C 版可以接 BBC micro:bit V1／V2，用 MakeCode 擴充 **pxt-mbc-sensors v2** 的積木讀值（內部走 [MBC I2C 協議 v2](/i2c-protocol.md)）。擴充安裝、硬體注意事項（3.3 V、供電）請看 [micro:bit 總覽](/microbit.md)。

<p class="chips"><a class="chip chip-spike" href="/sensors/ps4/index.md">SPIKE</a><a class="chip chip-ev3" href="/sensors/ps4/index.md">EV3</a><a class="chip chip-arduino" href="/sensors/ps4/arduino-i2c.md">Arduino I2C</a><a class="chip chip-mblock" href="/sensors/ps4/mblock.md">mBlock</a><a class="chip chip-matrix" href="/sensors/ps4/matrix.md">MATRIX</a><a class="chip chip-microbit current" href="/sensors/ps4/microbit.md">micro:bit</a></p>

> [!IMPORTANT]
> 擴充 v2 尚未上機驗證；積木名稱以發布版為準： <https://github.com/contactmbcrobot/pxt-mbc-sensors>

## 接線

<figure class="ref-fig"><img src="/images/figures/ps4/wiring-microbit.svg" alt="掌控者 PS4／PS5 接收器 × micro:bit" loading="lazy"></figure>

| 感應器 | micro:bit |
| :--- | :--- |
| SDA（RX） | P20 |
| SCL（TX） | P19 |
| VCC | **一定要外接 5 V**（GND 要和 micro:bit 共地） |
| GND | GND |

## 讀值積木

| 積木 | 傳回 | 編號 | 說明 |
| :--- | :--- | :--- | :--- |
| 掌控者 十六鍵位元圖 | 0~65535 bit | [`01`](/sensors/ps4/reference.md#v01) | 全部按鍵一個 16 bit（PSX 排列）：bit0 SELECT(SHARE)、1 L3、2 R3、3 START(OPTIONS)、4 上、5 右、6 下、7 左、8 L2、9 R2、10 L1、11 R1、12 ▲、13 ●、14 ✕、15 ■；1 = 按下；未連線 = 0 |
| 掌控者 ［方向鍵］（0~15） | 0~15 bit | [`02`](/sensors/ps4/reference.md#v02) | 上 1、右 2、下 4、左 8，同時按相加（0~15） |
| 掌控者 ［形狀鍵］（0~15） | 0~15 bit | [`03`](/sensors/ps4/reference.md#v03) | ▲ 1、● 2、✕ 4、■ 8，同時按相加（0~15） |
| 掌控者 ［肩鍵］（0~15） | 0~15 bit | [`04`](/sensors/ps4/reference.md#v04) | L2 1、R2 2、L1 4、R1 8，同時按相加（0~15） |
| 掌控者 ［功能鍵］（0~15） | 0~15 bit | [`05`](/sensors/ps4/reference.md#v05) | SELECT(SHARE) 1、L3 2、R3 4、START(OPTIONS) 8，同時按相加（0~15） |
| 掌控者 手把已連線？ | 0~1 | [`06`](/sensors/ps4/reference.md#v06) | 1 = 手把已連線且 300 ms 內有資料，0 = 未連線（斷線時按鍵 0、搖桿中間） |
| 掌控者 電量（百分比） | 0~100 % | [`07`](/sensors/ps4/reference.md#v07) | 手把電量 0~100（PS2 接收器恆 0） |
| 掌控者 扳機 ［L2］ 類比值 | 0~255 | [`08`](/sensors/ps4/reference.md#v08) | L2 扳機壓多深 0~255（未連線 0） |
| 掌控者 扳機 ［R2］ 類比值 | 0~255 | [`09`](/sensors/ps4/reference.md#v09) | R2 扳機壓多深 0~255（未連線 0） |
| 掌控者 手把類型 | 0~4 | [`0B`](/sensors/ps4/reference.md#v0b) | 0 無、1 PS2 數位、2 PS2 類比、3 PS4、4 PS5（或其他） |
| 掌控者 搖桿 ［左搖桿 X／左搖桿 Y／右搖桿 X／右搖桿 Y］ | -100~+100 | [`10~13`](/sensors/ps4/reference.md#v10) | 10 LX、11 LY、12 RX、13 RY：−100 ~ +100，0 = 中間，右／上為正；未連線 0 |

### 其他積木（是／否、事件）

| 積木 | 相關的值 |
| :--- | :--- |
| 掌控者 按鈕 ［✕］ 被按住？ | [`01`](/sensors/ps4/reference.md#v01) |
| 當掌控者 按鈕 ［✕］ ［按下］ | [`01`](/sensors/ps4/reference.md#v01) |

## 命令積木

| 積木 | 編號 | 說明 |
| :--- | :--- | :--- |
| 掌控者 震動 強度 200 | [`E5`](/sensors/ps4/reference.md#ce5) | [強度 0~255]，0 = 停；每次 200 ms（PS2 收到忽略） |
| 掌控者 燈條顏色 #ff0000 | [`E6`](/sensors/ps4/reference.md#ce6) | [R][G][B] 各 0~255，要剛好 3 byte；未連線先記住，連上再套用；預設藍 (0,0,255)（PS2 收到忽略） |

## 狀態

| bit | 名稱 | 說明 |
| :--- | :--- | :--- |
| b0 | 就緒 (`ready`) | 恆 1 |
| b1 | 手把已連線 (`connected`) | = 值 06（linkActive） |

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

本頁由 `_build/gen-reference.mjs` 從 `_build/protocol/ps4.json`（單一真相來源，轉錄自 2026-10-21 協定定案）產生。

- 值表、命令、旗標與 PS2 完全相同（mBlock 定案「兩款相同」＋I2C 定案 §3／§4）；只有 hw0 = 2、model = PS4、E5／E6 有作用。
- D3：SPIKE 模式 0 教育版 byte 6~7、模式 6 v[2]、模式 9 v[6] 的 button16 都是 PSX 排列（#65 已把 PS4 改成 PSX，盤點檔寫的 Bluepad32 排列已過時）。
- 0x22 命令 4 同時裝方向鍵（低 nibble）與形狀鍵（高 nibble）、命令 5 裝肩鍵（低）與功能鍵（高）；JSON 只能把一個命令掛在一個值上，所以掛在 02／04，03／05 的 ch22 = null（內容寫在 02／04 的 note）。SPIKE 模式 0／1／2／9 的 nibble 也用同樣方式在 note 說明。
- 搖桿：I2C／mBlock 送 ±100；EV3／0x22送 0~99（50 = 中間）、SPIKE 送 1~9（5 = 中間），不是線性 offset，主機端各自換算（網站範例已是這樣）。
- EV3 模式 3 = 方向鍵 | 形狀鍵<<4 | 已連線<<8（#67 新增 bit8）；模式 4 內容不變（[方向鍵, LX, 形狀鍵, LY]）；模式 5 改送裝置 ID 格式 B [0x42 | 2<<8, 韌體版]。
- SPIKE 模式 3 改 [已連線, 電量, 0]（3×int8）、模式 8 改 [0x42 | 2<<8, 協議版 2, 韌體版, 更新序號]；其餘空模式 1、2、4、6 維持現狀。
- ESP32 從機不能時脈延展：I2C v2 在 onReceive 就預載從該編號起 32 B；0x22照 MATRIX 定案 §5 #2 預載最新一包，待上機。
- I2C 0A 系統鍵 b1 = 擷取鍵（v1 0x15 放在 b3，v2 改 b1，照 I2C 定案 §4）。
- 擴充內部用 `Int16LE` 讀、寫完編號 `basic.pause(2)`（D5）；感應器沒接好時讀值積木回 0、是／否積木回「否」。
