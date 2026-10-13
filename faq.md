<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# ❓ 常見問題

學生、教練與老師最常問的 MBC-Pro Sensors 問題。找不到答案？加 [LINE @692vcvuk](https://line.me/R/ti/p/@692vcvuk) 問我們。

## 相容性

### MBC 感應器可以用在 LEGO® Education SPIKE™ Prime 和官方 SPIKE App 嗎？

可以。循行者 8 路、16 路直接插上 SPIKE™ Prime 任一個孔就能用，官方 SPIKE 積木可以直接讀取，初學者不必寫 Python。進階隊伍可以改用 Python 或 [Pybricks](/sensors/line8/spike-pybricks.md)，迴圈更快。

### 需要轉接板或安裝驅動程式嗎？

不需要。MBC 感應器使用與 LEGO® 主機孔位相同的接頭與通訊協議，插上主機就會自動辨識。

### 支援哪些主機和軟體？

- **LEGO® SPIKE™ Prime／Robot Inventor**：官方 SPIKE App、Pybricks
- **LEGO® MINDSTORMS® EV3**：EV3 官方軟體、EV3 Classroom、clev3r、Pybricks
- **Arduino、ESP32、樹莓派**：共用的[通用 I2C 協議](/i2c-protocol.md)
- **MATRIX Mini R4、MakeBlock**：部分產品支援，請看[選購指南](/guide.md)

### SPIKE 版和 EV3 版可以互換嗎？

不行。每項產品依主機分成不同版本，接頭與通訊協議都不一樣。下單前請先確認主機型號，[選購指南](/guide.md)會告訴你該選哪一版。

### SPIKE 6 路擴充器（EXP6）可以用 SPIKE 積木嗎？

不行。EXP6 **只支援 SPIKE™ Prime**（不支援 Robot Inventor 與 EV3）。在官方 SPIKE App 裡必須使用 **Python 文字模式**；Pybricks 完整支援。詳見 [EXP6 產品頁](/sensors/exp6/index.md)。

## 比賽規則

### MBC 感應器可以用在 WRO 嗎？

可以。WRO 從 2025 年起開放 RoboMission 使用任何品牌的器材，但有組別限制：相機只限國中、高中組；遙控接收器在比賽中不能使用。各產品的詳細對照請看 [WRO 規則對照](/wro-rules.md)。

### 可以用在 FIRST® LEGO® League（FLL）嗎？

FIRST® LEGO® League Challenge 正式比賽只允許 LEGO 原廠零件，所以不能使用。平時教學、練習與其他開放規則的賽事都可以用。

### 測距者的雷射對眼睛安全嗎？

安全。測距者 2 路與 8 路使用 Class 1 雷射元件（符合 IEC 60825-1:2014），正常使用下對眼睛安全。WRO 檢錄需要護眼安全證明，請看 [WRO 規則對照](/wro-rules.md)。

### 神攝手 ESP32CAM 會用到 Wi-Fi 嗎？

不會。出廠韌體沒有任何 Wi-Fi 或藍牙程式碼，無線電從未啟動，與主機之間用線材連接。

## 選購

### 該選循行者 8 路還是 16 路？

一般地圖要穩定循線，選 **8 路**：體積小，是比賽最常見的選擇。急彎多、十字路口多，選 **16 路**：視野更寬，一次就能看清轉彎和路口。

### 循線感應器怎麼校準？

按下校準鍵，把感應器在場地的白色區域與黑線上方來回掃過，再按一次即完成儲存。校準數據保存在感應器裡，關機也不會消失。詳見[循行者 8 路產品頁](/sensors/line8/index.md)。

### 測距者 8 路（TOF8）可以測多遠？

0～1,200 mm，涵蓋機器人前方 180°（左右各 90°）。讀到 1,200 mm 代表該方向沒有目標。

### PS4/PS5 接收器會連到別隊的手把嗎？

不會。接收器只綁定你自己的手把，比賽現場人再多也不會被別隊干擾。訊號中斷時，0.3 秒內所有輸出歸零。空曠場地實測可穩定連線 100 公尺。

## 價格與購買

### 售價多少？

台灣建議售價（新台幣）：

| 產品 | 建議售價 |
| :--- | :---: |
| 循行者 8 路（Line8） | NT$2,500 |
| 循行者 16 路（Line16） | NT$3,000 |
| 測距者 2 路（TOF2） | NT$1,500 |
| 測距者 8 路（TOF8） | NT$3,000 |
| 神攝手 ESP32CAM | NT$3,000 |
| SPIKE 6 路擴充器（EXP6） | NT$3,600 |
| 掌控者 PS2 | NT$2,000 |
| 掌控者 PS4/PS5 | NT$2,500 |
| 陀螺儀 IMU 九軸（即將推出） | NT$2,500 |

海外售價以美金計、不含運費，請看[英文 FAQ](/en/faq.md)。

### 怎麼購買？有經銷合作嗎？

加 [LINE @692vcvuk](https://line.me/R/ti/p/@692vcvuk) 或寄信到 [mbc.robot89@gmail.com](mailto:mbc.robot89@gmail.com)，告訴我們主機型號、產品與數量，工程師工作日 24 小時內回覆。學校、社團大量採購與海外經銷合作都歡迎，請看[購買管道](/where-to-buy.md)與[經銷合作](/distributors.md)。
