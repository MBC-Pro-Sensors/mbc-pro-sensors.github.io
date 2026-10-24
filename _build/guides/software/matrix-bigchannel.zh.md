### 你需要

- **感應器**：{{product}}（{{edition}}）。
- **主機**：MATRIX Mini R4。
- **連接線**：MATRIX 原廠 4 pin 感應器線。
- **軟體**：**MATRIXblock**（積木或 Arduino 模式都可以）。
- **函式庫**：LauMo 大通道函式庫 v2，[MiniR4ColorSensorExt_LauMo_v2.zip](/downloads/MiniR4ColorSensorExt_LauMo_v2.zip)（`MiniR4ColorSensorExt.cpp`、`MiniR4ColorSensorExt.h`）。

大通道由 LauMo 教練設計：換掉原廠兩個檔之後，同一組顏色感應器積木變成「指標讀法」，一次拿到一包 **30 byte**（24 byte 產品資料＋狀態＋4 byte 擴充＋CRC），產品所有數值（含 mm、位元圖、狀態）都讀得到。

### 安裝軟體

1. 安裝 MATRIXblock（預設裝在 `C:\matrixblock-r4\`）。

### 讓軟體認得感應器

換函式庫（只要做一次）：

1. 下載 [MiniR4ColorSensorExt_LauMo_v2.zip](/downloads/MiniR4ColorSensorExt_LauMo_v2.zip)。v2 把 `begin()` 的等待從 1 ms 改成 **5 ms**，ESP32 產品才來得及回應。
2. **關閉 MATRIXblock**，到安裝目錄覆蓋這兩個檔：`C:\matrixblock-r4\arduino\libraries\MatrixMiniR4\src\Modules\Sensors\`。**先把原檔備份**，要回原廠 0x22 用法時放回去。
3. 重開 MATRIXblock。積木外觀不變，讀法改成下面的指標讀法。

指標讀法：

| 積木 | 作用 |
| :--- | :--- |
| ColorID | 指標回到 byte 0，回傳 byte 24（狀態旗標） |
| Blue | 讀 1 byte（0~255） |
| Green | 讀 1 byte 有正負（−128~127） |
| Saturation | 讀 2 byte（0~65535，小端） |
| Hue | 讀 2 byte 有正負（−32768~32767，小端） |
| Value | 讀 4 byte 小數 |
| Red | 指標往後移「上一個讀取積木的大小」，回傳新位置 |
| Cyan／Magenta／Yellow／Key | 送命令 F1／F2／F3／F4，回傳上一包的 byte 25／26／27／28 |

讀法：**ColorID →（Blue 讀 byte 0）→ Red →（Blue 讀 byte 1）→ Red → …**，ColorID 之後先讀一次再按 Red。要跳到第 n 個 byte 就「ColorID → Blue → Red × n」。每個 byte 放什麼看下面的讀值。

![大通道 30 byte 封包](/images/figures/common/packet-big30.svg)

### 接線

1. 用 MATRIX 原廠 4 pin 感應器線，接 Mini R4 的 **I2C1** 或 **I2C2** 孔（I2C 位址 `0x22`）。
2. 積木或程式裡選你接的那個孔（例如 `MiniR4.I2C1`）。
3. 5 V 直接接，不需要準位轉換。

### 第一次測試

1. 「ColorID → Blue」讀 byte 0，把數字顯示出來。
2. 數字跟著感應器前方的東西變化，就是換好、接好了。

### 常見問題

- **讀到的數字亂跳、看不懂？** 函式庫還是原廠版：確認兩個檔已經覆蓋、MATRIXblock 有重開。
- **想回原廠 0x22 用法？** 把備份的原廠兩個檔放回去。同一台電腦一次只能用一種，感應器**不用重燒**。
- **讀到舊資料？** 函式庫收不滿 30 byte 或 CRC 錯會丟掉、沿用上一包；最快 5 ms 讀一次，讀太快也會拿到同一包。
