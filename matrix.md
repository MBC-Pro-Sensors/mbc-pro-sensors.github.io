<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# 🟢 MATRIX Mini R4

<!-- hide:hideMatrixBigChannel -->
MBC 的 MATRIX 版感應器接在 Mini R4 的 **I2C 孔**（位址 `0x22`），在 MATRIXblock 裡用原廠「顏色感應器」積木就能讀；神攝手走 **串口**，用原廠「mVision」攝影機積木。本頁是三種用法的總覽，每個產品各自的頁面在最後。
<!-- /hide -->
<!-- show:hideMatrixBigChannel -->
MBC 的 MATRIX 版感應器接在 Mini R4 的 **I2C 孔**（位址 `0x22`），在 MATRIXblock 裡用原廠「顏色感應器」積木就能讀；神攝手走 **串口**，用原廠「mVision」攝影機積木。兩種都不用安裝或更換任何檔案。本頁是總覽，每個產品各自的頁面在最後。
<!-- /show -->

<!-- hide:hideMatrixBigChannel -->
| 用法 | 產品 | 要裝的東西 | 能讀到 |
| :--- | :--- | :--- | :--- |
| **A. 原廠積木（0x22）** | 循行者 8／16 路、測距者 2／8 路、IMU、掌控者 PS2／PS4 | 不用裝，MATRIXblock 內建 | R、G、B、C、M、Y、K 七個 0~255 的數 |
| **B. LauMo 大通道** | 同上 | 換兩個函式庫檔（[下載](/downloads/MiniR4ColorSensorExt_LauMo_v2.zip)） | 一包 30 byte，產品所有數值（含 mm、位元圖、狀態） |
| **C. mVision（串口）** | 神攝手 學校版／WRO 版 | 不用裝 | 攝影機積木「mVision 資料 0~9」 |

> [!IMPORTANT]
> A 和 B 要裝**不同的函式庫檔**，同一台電腦一次只能用一種；韌體兩種都支援，換用法不用重燒感應器。
<!-- /hide -->
<!-- show:hideMatrixBigChannel -->
| 用法 | 產品 | 要裝的東西 | 能讀到 |
| :--- | :--- | :--- | :--- |
| **A. 原廠積木（0x22）** | 循行者 8／16 路、測距者 2／8 路、IMU、掌控者 PS2／PS4 | 不用裝，MATRIXblock 內建 | R、G、B、C、M、Y、K 七個 0~255 的數 |
| **B. mVision（串口）** | 神攝手 學校版／WRO 版 | 不用裝 | 攝影機積木「mVision 資料 0~9」 |
<!-- /show -->

---

## A. 原廠積木（0x22）

積木「顏色感應器 → R／G／B／C／M／Y／K」各讀 1 個 0~255 的數，韌體把產品資料塞在這七格。

> [!WARNING]
> **不要用 H／S／V、顏色辨識（ColorID）積木**：那是主機拿 R、G、B 自己算的，對 MBC 產品沒有意義。灰階、顏色編號積木已被 MATRIX 拿掉，所以只有這七格能用。

### 七格對照表

| 積木 | 循行者 8 路 | 循行者 16 路 | 測距者 2 路 | 測距者 8 路 | IMU 陀螺儀 | 掌控者 PS2／PS4 |
| :---: | :--- | :--- | :--- | :--- | :--- | :--- |
| **R** | 線位置 0~16（減 8） | 線位置 0~32（減 16） | 左距離 cm | 第 1 顆 cm（正左） | 陀螺 3D 航向 低 byte | 方向鍵＋形狀鍵 |
| **G** | 線寬 0~8 | 線寬 0~16 | 右距離 cm | 第 2 顆（左前 60°） | 陀螺 3D 航向 高 byte | 肩鍵＋功能鍵 |
| **B** | 細位置 0~200（減 100） | 細位置 0~200（減 100） | 最近距離 cm | 第 3 顆（左前 30°） | 融合航向 低 | 左搖桿左右 0~99 |
| **C** | 第 1、2 路光值 | 第 1~4 路光值 | 0 | **第 4、5 顆取較近（正前方）** | 融合航向 高 | 左搖桿上下 0~99 |
| **M** | 第 3、4 路 | 第 5~8 路 | 0 | 第 6 顆（右前 30°） | 陀螺 1D 航向 低 | 右搖桿左右 0~99 |
| **Y** | 第 5、6 路 | 第 9~12 路 | 0 | 第 7 顆（右前 60°） | 陀螺 1D 航向 高 | 右搖桿上下 0~99 |
| **K** | 第 7、8 路 | 第 13~16 路 | 0 | 第 8 顆（正右） | 0 | 已連線（1／0） |

### 解碼方式

| 產品 | 積木 | 解法 |
| :--- | :--- | :--- |
| 循行者 8 路 | R／B | 線位置 = R − 8（負 = 線在左，站在車後往前看）；細位置 = B − 100 |
| | C~K | 兩路一組：十位 = 前一路、個位 = 後一路，各 1~9（1 最暗、9 最亮） |
| 循行者 16 路 | R／B | 線位置 = R − 16；細位置 = B − 100 |
| | C~K | 四路一組，每路 2 bit（0 最暗 ~ 3 最亮）：第 1 路 = ÷64 取整數、第 2 路 = ÷16 取整數再除以 4 的餘數、第 3 路 = ÷4 取整數再除以 4 的餘數、第 4 路 = 除以 4 的餘數 |
| 測距者 2 路 | R~B | 0~120 cm，120 = 那一側沒東西 |
| 測距者 8 路 | R~K | 直接用，0~120 cm（1 cm），120 = 那個方向沒東西；由左到右排，C = 正前方（第 4、5 顆取較近，避免窄物體被平均掉）。最近距離／方向用「比較」積木自己找 |
| IMU | R~Y | 角度 = 高 byte × 256 ＋ 低 byte − 32768（可以超過 360，左轉為負） |
| 掌控者 | R | 上 1、右 2、下 4、左 8、▲ 16、● 32、✕ 64、■ 128（同時按相加） |
| | G | L2 1、R2 2、L1 4、R1 8、SELECT 16、L3 32、R3 64、START 128 |
| | B~Y | 0~99，50 = 中間 |

```cpp
// MATRIXblock 的 Arduino 模式：原廠函式庫，讀 R 格
int r = MiniR4.I2C1.MXColor.getColor(R);   // 循行者 8 路：線位置 + 8
int linePos = r - 8;
```

---

<!-- hide:hideMatrixBigChannel -->
## B. LauMo 大通道

大通道由 LauMo 教練設計：把原廠 `MiniR4ColorSensorExt` 兩個檔換掉，同一組顏色感應器積木就變成「指標讀法」，一次拿到一包 **30 byte**（24 byte 產品資料＋狀態＋4 byte 擴充＋CRC）。循行者 16 路與 IMU（R4 大通道版韌體）早已用這個格式出貨，其他產品 2026-10 起全部支援。

### 安裝函式庫

1. 下載 [MiniR4ColorSensorExt_LauMo_v2.zip](/downloads/MiniR4ColorSensorExt_LauMo_v2.zip)（`MiniR4ColorSensorExt.cpp`、`MiniR4ColorSensorExt.h`，v2 把 `begin()` 的等待從 1 ms 改成 **5 ms**，ESP32 產品才來得及回應）。
2. 關閉 MATRIXblock，到安裝目錄覆蓋這兩個檔（預設 `C:\matrixblock-r4\arduino\libraries\MatrixMiniR4\src\Modules\Sensors\`）。先把原檔備份，要回原廠 0x22 用法時放回去。
3. 重開 MATRIXblock，積木不變，讀法改成下面的指標讀法。

### 指標讀法

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

讀法：**ColorID →（Blue 讀 byte 0）→ Red →（Blue 讀 byte 1）→ Red → …**，ColorID 之後先讀一次再按 Red。要跳到第 n 個 byte 就「ColorID → Blue → Red × n」。

<figure class="ref-fig"><img src="/images/figures/common/packet-big30.svg" alt="LauMo 大通道 30 byte 封包" loading="lazy"></figure>

函式庫規則（韌體照這個做，寫程式不用管）：寫 `0x21`、等 300 µs、讀 30 byte；byte 29 是 CRC-8（多項式 0x07，涵蓋 0~28），收不滿或 CRC 錯就丟掉沿用舊資料；最快 5 ms 讀一次；`begin()` 送命令 1 要回 `0x43`。

### 各產品 byte 表（byte 0~23）

**循行者 8 路**（2026-10 新增）

| byte | 積木 | 內容 |
| :---: | :--- | :--- |
| 0 | Blue | 線位置 + 8（0~16） |
| 1 | Blue | 線寬 0~8 |
| 2 | Blue | 細位置 + 100（0~200） |
| 3~10 | Blue | 第 1~8 路光值 0~100 |
| 11 | Blue | 壓線位元圖（bit0 = 第 1 路） |
| 12 | Blue | 線段數 |

**循行者 16 路**（已流出，0~18 不動）

| byte | 積木 | 內容 |
| :---: | :--- | :--- |
| 0 | Blue | 線位置 + 16（0~32） |
| 1 | Blue | 線寬 0~16 |
| 2 | Blue | 細位置 + 100（0~200） |
| 3~18 | Blue | 第 1~16 路光值 0~100 |
| 19~20 | Saturation | 壓線位元圖 16 bit（新增） |
| 21 | Blue | 線段數（新增） |

**測距者 2 路**

| byte | 積木 | 內容 |
| :---: | :--- | :--- |
| 0 | Blue | 左距離 cm |
| 1 | Blue | 右距離 cm |
| 2 | Blue | 最近距離 cm |
| 3 | Blue | 初始化位元圖（bit0 左、bit1 右） |
| 4 | Blue | 故障位元圖 |
| 5~6 | Saturation | 左距離 mm 0~1200（新增） |
| 7~8 | Saturation | 右距離 mm（新增） |

**測距者 8 路**

| byte | 積木 | 內容 |
| :---: | :--- | :--- |
| 0~7 | Blue | 第 1~8 顆距離 cm（第 1 顆 = 正左） |
| 8 | Blue | 最近方向 0~16（8 = 正前方） |
| 9 | Blue | 最近距離 cm |
| 10 | Blue | 最空曠方向 0~16 |
| 11 | Blue | 最空曠距離 cm |
| 12 | Blue | 初始化位元圖（bit0 = 第 1 顆） |
| 13 | Blue | 故障位元圖 |
| 14~15 | Saturation | 最近距離 mm（新增） |
| 16~17 | Saturation | 正前方（第 4、5 顆平均）mm（新增） |

**IMU 陀螺儀**（已流出，0~7 不動）

| byte | 積木 | 內容 |
| :---: | :--- | :--- |
| 0~1 | Hue | 陀螺 3D 航向（度） |
| 2~3 | Hue | 融合航向 |
| 4~5 | Hue | 陀螺 1D 航向 |
| 6~7 | Hue | 磁力計航向 |
| 8~9 | Hue | 轉向軸角速度（0.1 度／秒）（新增） |
| 10~11 | Hue | 俯仰角（度）（新增） |
| 12~13 | Hue | 俯仰角速度（0.1 度／秒）（新增） |

**掌控者 PS2／PS4**（2026-10 新增）

| byte | 積木 | 內容 |
| :---: | :--- | :--- |
| 0 | Blue | 方向鍵 0~15（上 1、右 2、下 4、左 8） |
| 1 | Blue | 形狀鍵 0~15（▲1、●2、✕4、■8） |
| 2 | Blue | 肩鍵 0~15（L2 1、R2 2、L1 4、R1 8） |
| 3 | Blue | 功能鍵 0~15（SELECT 1、L3 2、R3 4、START 8） |
| 4~7 | Blue | LX、LY、RX、RY（0~99，50 = 中間） |
| 8~9 | Saturation | 16 鍵合在一起（PSX 排列） |
| 10 | Blue | 已連線 1／0 |
| 11 | Blue | 電量 0~100（PS4；PS2 = 0） |

### 尾段：ColorID／Cyan／Magenta／Yellow／Key

| 積木 | 循行者 16 路 | IMU | 循行者 8 路、測距者、掌控者 |
| :--- | :--- | :--- | :--- |
| ColorID 回傳 | 0（已流出維持） | 0（已流出維持） | 狀態旗標 byte 24（＝I2C v2 F3，bit0 = 就緒） |
| Cyan | 黑色校準 | 航向歸零 | 循行者 8 路：黑色校準；其他：無動作 |
| Magenta | 白色校準 | 航向歸零 | 循行者 8 路：白色校準；其他：無動作 |
| Yellow | 0 | 航向歸零 | 裝置 ID（循行者 0x16、測距者 0x29、掌控者 0x42） |
| Key | 0 | 航向歸零 | 循行者通道數 8／測距者顆數 2 或 8／掌控者 1 = PS2、2 = PS4 |

循行者 8 路的 byte 25／26 另外記 Cyan／Magenta 動作完成次數。

### Arduino 寫法（MATRIXblock 的 Arduino 模式）

```cpp
// 大通道：讀第 n 個 byte（uint8）／第 n 個起的 2 byte（uint16 小端）
uint8_t bigU8(uint8_t n) {
  MiniR4.I2C1.MXColor.getColorID();                 // 指標回到 byte 0
  uint8_t v = MiniR4.I2C1.MXColor.getColor(B);      // 讀 byte 0
  for (uint8_t i = 0; i < n; i++) { MiniR4.I2C1.MXColor.getColor(R); v = MiniR4.I2C1.MXColor.getColor(B); }
  return v;
}
uint16_t bigU16(uint8_t n) {
  MiniR4.I2C1.MXColor.getColorID();
  MiniR4.I2C1.MXColor.getColor(B);
  for (uint8_t i = 0; i < n; i++) MiniR4.I2C1.MXColor.getColor(R);
  return (uint16_t)MiniR4.I2C1.MXColor.getS();      // Saturation = 讀 2 byte
}
int16_t bigI16(uint8_t n) {
  MiniR4.I2C1.MXColor.getColorID();
  MiniR4.I2C1.MXColor.getColor(B);
  for (uint8_t i = 0; i < n; i++) MiniR4.I2C1.MXColor.getColor(R);
  return (int16_t)MiniR4.I2C1.MXColor.getH();       // Hue = 讀 2 byte 有正負
}

int leftMm = bigU16(5);     // 測距者 2 路：左距離 mm（byte 5~6）
int heading = bigI16(2);    // IMU：融合航向（byte 2~3）
```

> [!NOTE]
> PS4 接收器是 ESP32，I2C 從機要 ≥ 2 ms 才能回應；韌體會預先載好最新一包，函式庫 v2 的 `begin()` 等 5 ms 就是為了它。**上機待測**。

---
<!-- /hide -->

<!-- hide:hideMatrixBigChannel -->
## C. 神攝手：mVision 攝影機積木（串口）
<!-- /hide -->
<!-- show:hideMatrixBigChannel -->
## B. 神攝手：mVision 攝影機積木（串口）
<!-- /show -->

<!-- hide:hideMatrixBigChannel -->
MATRIXblock 的攝影機積木只有 mVision（Serial1 115200）和 HuskyLens，沒有 I2C 版，所以神攝手 **不走 0x22／大通道**，而是照 mVision 的封包格式送資料：積木「mVision 資料 0~9」每格一個數，**每 20 ms 一包**。
<!-- /hide -->
<!-- show:hideMatrixBigChannel -->
MATRIXblock 的攝影機積木只有 mVision（Serial1 115200）和 HuskyLens，沒有 I2C 版，所以神攝手 **不走 0x22**，而是照 mVision 的封包格式送資料：積木「mVision 資料 0~9」每格一個數，**每 20 ms 一包**。
<!-- /show -->

> [!WARNING]
> **資料 10 不要用**：MATRIX 程式只宣告 10 格（資料 0~9），第 10 格讀到的是別的記憶體。

| mVision 資料 | 學校版（2026-10 起） | WRO 版（HUE12，不動） |
| :---: | :--- | :--- |
| 0 | 色相 0~100（**255 = 黑白灰**） | 維持現有 HUE12 打包（12 色點顏色碼＋鮮豔度），照 `HUE12_AS_mVisionSensor_v1.0.8.mbr4` 範例解碼 |
| 1 | 飽和度 0~100 | |
| 2 | 明度 0~100 | |
| 3 | 動態 0~100 | |
| 4 | 色塊 X 0~100（左→右） | |
| 5 | 色塊 Y 0~75（下→上） | |
| 6 | 色塊寬 W 0~100（0 = 沒找到） | |
| 7 | 色塊高 H 0~75 | |
| 8 | 色塊面積 W × H | |
| 9 | 有找到色塊 1／0 | |

數值範圍跟 EV3／SPIKE 版一致，程式邏輯可以直接搬。WRO 2026 隊伍用的 HUE12 範例不受影響。

<figure class="ref-fig"><img src="/images/figures/common/packet-mvision.svg" alt="mVision 封包格式" loading="lazy"></figure>

---

## 📖 各產品的 MATRIX 頁

<!-- hide:hideMatrixBigChannel -->
每款產品一頁：接線圖、0x22 七格對照、大通道 byte 表（連到該值的指令參考）與 Arduino 寫法。
<!-- /hide -->
<!-- show:hideMatrixBigChannel -->
每款產品一頁：接線圖、0x22 七格對照（連到該值的指令參考）與 Arduino 寫法。
<!-- /show -->

| 產品 | MATRIX 頁 |
| :--- | :--- |
| 循行者 8 路 | [line8 × MATRIX](/sensors/line8/matrix.md) |
| 循行者 16 路 | [line16 × MATRIX](/sensors/line16/matrix.md) |
| 測距者 2 路 | [tof2 × MATRIX](/sensors/tof2/matrix.md) |
| 測距者 8 路 | [tof8 × MATRIX](/sensors/tof8/matrix.md) |
| 掌控者 PS2 | [ps2 × MATRIX](/sensors/ps2/matrix.md) |
| 掌控者 PS4／PS5 | [ps4 × MATRIX](/sensors/ps4/matrix.md) |
| IMU 陀螺儀 | [imu × MATRIX](/sensors/imu/matrix.md) |
| 神攝手 ESP32CAM | [esp32cam × MATRIX（mVision）](/sensors/esp32cam/matrix.md) |

EXP6／Servo6 沒有 MATRIX 版。函式庫下載與其他平台的檔案：[下載專區](/downloads.md)。
