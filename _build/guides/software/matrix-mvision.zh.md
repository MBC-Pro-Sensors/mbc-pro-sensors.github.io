### 你需要

- **感應器**：{{product}}（{{edition}}）。
- **主機**：MATRIX Mini R4。
- **軟體**：**MATRIXblock**，用內建的 **mVision** 攝影機積木。**不用裝函式庫**。

MATRIXblock 的攝影機積木只有 mVision（Serial1 115200）和 HuskyLens，沒有 I2C 版，所以這個產品**不走 0x22／大通道**，而是照 mVision 的封包格式送資料。

### 安裝軟體

1. 安裝 MATRIXblock（預設裝在 `C:\matrixblock-r4\`）。
2. 不用換任何函式庫檔，0x22 原廠或大通道的函式庫都不影響這個用法。

### 讓軟體認得感應器

感應器對 Mini R4 來說就是一台 mVision 攝影機：

- 積木「**mVision 資料 0~9**」每格一個數，感應器**每 20 ms 送一包**。
- 每一格代表什麼看下面的讀值。

> [!WARNING]
> **資料 10 不要用**：MATRIX 程式只宣告 10 格（資料 0~9），第 10 格讀到的是別的記憶體。

![mVision 封包格式](/images/figures/common/packet-mvision.svg)

### 接線

1. 感應器接 Mini R4 給 mVision 攝影機用的串口（Serial1，115200）。
2. 程式裡用 mVision 積木，不要用顏色感應器積木。

### 第一次測試

1. 用「mVision 資料 0」積木把數字顯示出來。
2. 把有顏色的東西放到鏡頭前，數字跟著變，就是接好了。

### 常見問題

- **讀到怪數字？** 確認沒有用到「資料 10」；只有資料 0~9 有意義。
- **用顏色感應器積木讀不到？** 這個產品在 MATRIX 走串口，只能用 mVision 積木。
