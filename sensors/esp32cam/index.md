<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# 神攝手舊版（停止支援）

<div class="lgb-hero"><a class="lgb-go" href="/sensors/unicam/index.md">你要找的是目前的神攝手 →</a></div>

> [!WARNING]
> **停止支援**：2026-10 以前出貨的神攝手（學校版／WRO 版）已停止支援，說明與範例保留供參考。需要升級成目前的神攝手：請聯繫原購買的經銷商寄回，**韌體更新免費**，來回運費由買家負擔。

這一頁集中舊版神攝手 ESP32CAM（學校版、WRO 版 HUE12）的說明、範例、互動教學與下載。舊網址都沒變，原本存的連結照樣能用。

<!-- platform-grid -->

## 📚 各平台說明與範例

每個主機的「平台說明」頁同時寫學校版和 WRO 版；範例與值表兩版分開。

| 主機 | 平台說明（依軟體） | 學校版 | WRO 版（HUE12） |
| :--- | :--- | :--- | :--- |
| SPIKE | [SPIKE](/sensors/esp32cam/spike/index.md)：[樂高官方 App](/sensors/esp32cam/spike/official.md)・[MBC 積木網站](/sensors/esp32cam/spike/mbcblocks.md)・[Pybricks](/sensors/esp32cam/spike/pybricks.md) | [SPIKE 新手範例](/sensors/esp32cam/spike-examples.md) | [SPIKE 新手範例](/sensors/esp32cam/spike-examples-wro.md) |
| EV3 | [EV3](/sensors/esp32cam/ev3/index.md)：[EV3-G 官方軟體](/sensors/esp32cam/ev3/ev3g.md)・[EV3 Classroom](/sensors/esp32cam/ev3/classroom.md)・[Pybricks](/sensors/esp32cam/ev3/pybricks.md)・[clev3r](/sensors/esp32cam/ev3/clev3r.md) | [EV3 新手範例](/sensors/esp32cam/ev3-examples.md) | [EV3 新手範例](/sensors/esp32cam/ev3-examples-wro.md) |
| Arduino | [Arduino](/sensors/esp32cam/arduino/index.md)：[Arduino IDE](/sensors/esp32cam/arduino/ide.md) | [I2C 值表](/sensors/esp32cam/arduino-i2c.md)・[Arduino 範例](/sensors/esp32cam/arduino-examples.md) | [I2C 值表](/sensors/esp32cam/arduino-i2c-wro.md) |
| mBlock | [mBlock](/sensors/esp32cam/mblock/index.md)：[mBlock 5](/sensors/esp32cam/mblock/mblock5.md) | [積木表](/sensors/esp32cam/mblock.md)・[mBlock 範例](/sensors/esp32cam/mblock-examples.md) | [積木表](/sensors/esp32cam/mblock-wro.md) |
| MATRIX | [MATRIX](/sensors/esp32cam/matrix/index.md)：[mVision 積木](/sensors/esp32cam/matrix/mvision.md) | [MATRIX 值表](/sensors/esp32cam/matrix.md) | [MATRIX 值表](/sensors/esp32cam/matrix-wro.md) |
| micro:bit | [micro:bit](/sensors/esp32cam/microbit/index.md)：[MakeCode](/sensors/esp32cam/microbit/makecode.md) | [MakeCode 積木表](/sensors/esp32cam/microbit.md)・[micro:bit 範例](/sensors/esp32cam/microbit-examples.md) | [MakeCode 積木表](/sensors/esp32cam/microbit-wro.md) |

指令參考（每個值在各主機怎麼讀）：[學校版](/sensors/esp32cam/reference.md)・[WRO 版（HUE12）](/sensors/esp32cam/reference-wro.md)

## 🧩 互動教學

不用接主機，在網頁上點點看。

| 主機 | 學校版 | WRO 版（HUE12） |
| :--- | :--- | :--- |
| SPIKE | [MicroPython（Pybricks）](/learn/cam_school/micropython.html)・[Pybricks 積木](/learn/cam_school/pybricks-blocks.html)・[MBC 積木網站](/learn/cam_school/mbc-blocks.html)・[樂高官方 SPIKE App](/learn/cam_school/spike-app.html) | [MicroPython（Pybricks）](/learn/hue12/micropython.html)・[Pybricks 積木](/learn/hue12/pybricks-blocks.html)・[MBC 積木網站](/learn/hue12/mbc-blocks.html)・[樂高官方 SPIKE App](/learn/hue12/spike-app.html) |
| EV3 | [Pybricks／EV3 MicroPython](/learn/cam_school/ev3-pybricks.html)・[clev3r](/learn/cam_school/clev3r.html)・[EV3 Classroom](/learn/cam_school/ev3-classroom.html)・[EV3-G 官方軟體](/learn/cam_school/ev3g.html) | [Pybricks／EV3 MicroPython](/learn/hue12/ev3-pybricks.html)・[clev3r](/learn/hue12/clev3r.html)・[EV3 Classroom](/learn/hue12/ev3-classroom.html)・[EV3-G 官方軟體](/learn/hue12/ev3g.html) |
| Arduino | [Arduino IDE](/learn/cam_school/arduino.html) | [Arduino IDE](/learn/hue12/arduino.html) |
| mBlock | [mBlock 5](/learn/cam_school/mblock.html) | [mBlock 5](/learn/hue12/mblock.html) |
| MATRIX | [MATRIXblock](/learn/cam_school/matrix.html) | [MATRIXblock](/learn/hue12/matrix.html) |
| micro:bit | [MakeCode](/learn/cam_school/makecode.html) | [MakeCode](/learn/hue12/makecode.html) |

## ⬇️ 下載

| 檔案 | 內容 |
| :--- | :--- |
| [esp32cam_arduino_examples.zip](/examples/esp32cam/arduino/esp32cam_arduino_examples.zip)・[English](/examples/esp32cam/arduino-en/esp32cam_arduino_examples_en.zip) | Arduino 範例（I2C，學校版＋WRO 版） |
| [MBC_cam_school.py](/examples/esp32cam/pybricks/MBC_cam_school.py)・[MBC_hue12.py](/examples/esp32cam/pybricks/MBC_hue12.py) | Pybricks 函式庫（學校版、WRO 版各一個檔） |
| [MBC_cam_school_Lib.py](/examples/esp32cam/pybricks/MBC_cam_school_Lib.py)・[MBC_hue12_Lib.py](/examples/esp32cam/pybricks/MBC_hue12_Lib.py) | 舊檔名函式庫（舊專案直接換檔用） |
| [MBC_sensors.py](/examples/esp32cam/pybricks/MBC_sensors.py) | MBC 統一函式庫（全部 MBC 感應器共用） |
| [下載中心](/downloads.md) | mBlock 5 擴展（「神攝手」分類就是舊版的積木） |

SPIKE 官方 App、MBC 積木網站、EV3 官方軟體的新手範例檔不另外打包，在上面的「新手範例」頁逐一下載。

## 📋 舊版產品資料

### 🚀 產品概述

樂高原廠的顏色感應器只能偵測正下方的一個點，完全無法「追蹤移動中的色塊」或「判斷目標在畫面中的位置」。

神攝手 ESP32CAM 視覺感應器讓樂高機器人擁有真正的眼睛：即時找出指定顏色的色塊位置與大小、讀取畫面上任一點的顏色、偵測畫面中的動作。所有設定都在**感應器背面的彩色螢幕與按鍵**上完成，看著即時畫面調整，設定值存在機身，下次開機直接使用。

### 🎯 核心優勢

- 🖥️ **機身螢幕即時預覽**：背面 1.8 吋彩色螢幕顯示鏡頭畫面與偵測結果，所見即所得
- 🎛️ **按鍵選單直接設定**：取色位置、HSV 容許範圍、最小色塊面積，都能在感應器上調整，不需電腦
- 💾 **設定斷電保存**：顏色設定與鏡頭參數（亮度、對比、飽和度、曝光、白平衡、翻轉、鏡像）存在機身
- 🎯 **一次輸出 5 種資訊**：取色點顏色、色塊面積、色塊 X / Y 座標、動態偵測值
- 💡 **內建補光燈**：光線不足的場地也能穩定辨色
- 🔭 **可選配鏡頭視角**：依場地需求選擇窄角（看遠）或廣角（看寬）

<div style="text-align: center;">
<img src="/images/sensors/esp32cam/esp32cam-product2.webp" alt="神攝手背面的 1.8 吋彩色螢幕" style="max-width: 320px; margin: 1rem auto; display: block;" />
</div>

### 📋 產品規格

| 項目 | 規格 |
| :--- | :--- |
| 影像解析度 | 160 × 120（輸出的座標已縮小：SPIKE 版 X `0` ~ `98`、Y `1` ~ `75`，見下方「輸出資料」） |
| 機身螢幕 | 1.8 吋 128 × 160 彩色 TFT |
| 操作按鍵 | 左、右、確認、側鍵（短按進出選單；在選單中長按 1 秒清除該組顏色） |
| 鏡頭視角選配 | 65° / 110° / 120° / 160° |
| 補光燈 | 內建，在機身選單開關（設定會保存） |

### 📤 輸出資料

以下是 **SPIKE 版**目前韌體的數值（EV3 版見下一節）。

| 輸出 | 內容（SPIKE 版） |
| :--- | :--- |
| 取色點顏色 | 畫面上指定座標點的色相：`0` ~ `100`（紅 0/100、黃約 17、綠約 33、藍約 67），`-1` = 黑白灰；Robot Inventor 官方 App 是 `0` ~ `10`。還沒設定取色點時是 `0`（跟紅色一樣） |
| 色塊面積 | `0` ~ `100`（`0` = 沒找到，此時 X / Y 無效）；只有官方 App 讀得到，Pybricks 模式 9 沒有面積 |
| 色塊 X 座標 | `0` ~ `98`，左 → 右，中心約 `50`；沒找到 = `0` |
| 色塊 Y 座標 | `1` ~ `75`，**下 → 上**（下面小、上面大），中心約 `37`；沒找到 = `0` |
| 動態偵測值 | `0` ~ `100`，畫面變化越大數值越高 |

官方 SPIKE App（教育版）的積木對應：「顏色」= 色相、「反射光」= 動態偵測值、「原始紅／綠／藍」= 色塊 X／Y／面積。

#### EV3 版讀到的數值

EV3 版有兩種韌體，讀法不同：

| 韌體 | EV3 上顯示成 | 讀到的數值 |
| :--- | :--- | :--- |
| 學校老師創意賽版（取色＋色塊追蹤＋動態偵測） | 紅外線感應器 | 模式 0：顏色 Hue（`0` ~ `100`，紅 0/100、黃約 17、綠約 33、藍約 67；`-1` = 黑白灰）。模式 1（8 個值）：Hue、鮮豔度、亮度、動態、色塊 X（`0` ~ `100`，左→右）、Y（`0` ~ `75`，下→上）、寬、高；沒找到色塊時 X/Y/寬/高都是 `0`，請用「寬 = 0」判斷 |
| WRO 2026 版（HUE12，12 個色點） | 第三方感應器（type 95） | 12 個色點各自的色相碼（`0` ~ `239` 彩色、`240` ~ `245` 白、`246` ~ `250` 黑、`255` = 色點沒啟用）與鮮豔度。只能用 EV3-G（MBC 自訂積木）、clev3r、Pybricks，**EV3 Classroom 讀不到** |

EV3 新手範例：[學校老師創意賽版](/sensors/esp32cam/ev3-examples.md)、[WRO 2026 版](/sensors/esp32cam/ev3-examples-wro.md)。

### 🔌 硬體接線

1. 將感應器插入 SPIKE（Port A~F）或 EV3（Port 1~4）
2. 感應器要接上**已經開機**的主機才會啟動（螢幕先顯示「LEGO Sync...」），出現鏡頭畫面就可以使用
3. 短按側鍵進入設定選單，用左／右鍵選擇、確認鍵進入；設定好的顏色與鏡頭參數會自動保存
