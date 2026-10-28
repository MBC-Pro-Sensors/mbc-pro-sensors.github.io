<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# MATRIX mVision 讀值教學

> [!WARNING]
> **v1 尚未上機驗證**：數值排法已定案並通過電腦端測試，實機數值（fps、循線門檻、I2C 腳位）上機後可能調整。

神攝手（MATRIX 版）對 MATRIX Mini R4 來說就是一台 **mVision 攝影機**：用 MATRIXblock 內建的 mVision 積木讀，**不用裝函式庫**，也不走 I2C。每一格的完整對照在 [讀值參考：MATRIX](/sensors/unicam/reference.md#host-mx)，整支範例在 [MATRIX 範例](/sensors/unicam/matrix-examples.md)。

## 接線

1. 相機接 Mini R4 給 mVision 攝影機用的串口（Serial1，115200）。
2. 相機螢幕左上角顯示主機名稱：**綠色＝連線中**、紅色＝斷線。
3. 在相機的 **MODE** 選單選相機模式和子選項（見 [產品首頁](/sensors/unicam/index.md) 的「相機上的操作」）。MATRIX 讀不到相機現在的模式，程式要照你選的模式解讀。

## 10 格資料

- 積木「**mVision 資料 0～9**」每格一個 0～65535 的數。讀值參考的「格 1」＝積木的**資料 0**、「格 10」＝**資料 9**。
- **資料 10 不要用**：MATRIX 程式只宣告 10 格，第 10 格讀到的是別的記憶體。
- 一格常常放兩個值：**A＋B×256**，A＝讀到的數除以 256 的**餘數**、B＝讀到的數 ÷ 256（取整數）。
- 座標用 **160×120 像素**（X 0～159 左→右、Y 0～119 上→下），0＝沒看到。
- 會有負數的值（循線偏移、角度、左右緣）是 **＋100** 送，讀到後減 100。

以**顏色追蹤 ③ 標準**（5 色 XY＋面積）為例：

| 積木 | 內容 | 換算 |
| :--- | :--- | :--- |
| mVision 資料 0 | 色 1 X ＋ 色 1 Y × 256 | X＝餘數（÷256）、Y＝÷256 取整數 |
| mVision 資料 1 | 色 1 面積 % ＋ 色 1 數量 × 256 | 面積＝餘數、數量＝÷256 取整數 |
| mVision 資料 2、3 | 色 2 位置、面積與數量 | 同上 |
| mVision 資料 4～9 | 色 3～色 5 | 同上 |

在 MATRIXblock 用「數學」的除法、取餘數積木換算；先判斷 X 不是 0 再用座標。

## 其他相機模式

| 相機模式 | MATRIX 給多少 |
| :--- | :--- |
| 顏色追蹤 ① 最多色／② 多色／④ 精細 | 16 色／10 色／3 色 XYWH |
| 座標取色 ①／②／③ | 20 點色相／10 點色相＋飽和／6 點色相＋飽和＋明度 |
| 照順序給色號 | 18 塊色相碼＋總塊數 |
| 循線 | 14 個值（偏移、角度、線寬、路口…） |
| 物體偵測 ① 物體清單／② 物體詳細 | 4 個物體代碼＋面積、XY＋物體數／3 個物體再加寬高（下面一節） |

每一格放什麼看 [讀值參考：MATRIX](/sensors/unicam/reference.md#host-mx)。

## 相機模式 5 物體偵測

相機把畫面裡的物體由大到小排好（物體 1 最大），每個物體給**物體代碼＝色號 × 10 ＋ 形狀**（例 13＝目標 1 的三角形，0＝沒有）。形狀：0 其他、1 圓、2 橢圓、3 三角、4 正方、5 長方、6 五邊、7 六邊、8 星、9 梯形。X、Y 是物體的**質心**；只看目標色 1～8。模式編號 **51＝① 物體清單**、**52＝② 物體詳細**。代碼再拆：色號＝代碼 ÷ 10（取整數）、形狀＝代碼除以 10 的餘數。

| 積木 | 51 物體清單（4 個） | 52 物體詳細（3 個） |
| :--- | :--- | :--- |
| mVision 資料 0 | 物體數（不用拆） | 物體數（不用拆） |
| 物體 i 第 1 格 | 資料 2i−1：代碼＝餘數（÷256）、面積 %＝÷256 取整數 | 資料 3i−2：代碼、面積 % |
| 物體 i 第 2 格 | 資料 2i：X＝餘數、Y＝÷256 | 資料 3i−1：X、Y |
| 物體 i 第 3 格 | — | 資料 3i：寬＝餘數、高＝÷256（像素） |

51 的資料 9 不用。MATRIX 不給角度、頂點數、追蹤編號（要這些用 I2C 版）。在 MATRIXblock 用「數學」的除法、取餘數積木拆；C++ 這樣讀物體 1：

```cpp
#include <MatrixMiniR4.h>
unsigned int CamData[10];                // mVision 的 10 格（資料 0～9）

void setup() {
  MiniR4.begin();
  MiniR4.Vision.Begin();                 // 開始收 mVision（Serial1 115200）
  Serial.begin(115200);
}

void loop() {
  if (MiniR4.Vision.SmartCamReader(CamData) > 0) {   // 相機切在 物體偵測 ① 物體清單（51）
    int n = CamData[0];                  // 資料 0：物體數
    int code = CamData[1] % 256;         // 資料 1 餘數：物體 1 代碼（色號 × 10 ＋ 形狀）
    int area = CamData[1] / 256;         // 資料 1 ÷256：物體 1 面積 %
    int x = CamData[2] % 256;            // 資料 2 餘數：物體 1 X
    int y = CamData[2] / 256;            // 資料 2 ÷256：物體 1 Y
    Serial.print("物體數 "); Serial.print(n);
    Serial.print("  物體 1 代碼 "); Serial.print(code);
    Serial.print(" 色號 "); Serial.print(code / 10);
    Serial.print(" 形狀 "); Serial.print(code % 10);
    Serial.print(" X "); Serial.print(x);
    Serial.print(" Y "); Serial.print(y);
    Serial.print(" 面積 "); Serial.println(area);
  }
  delay(50);
}
```

範例裡的小幫手 `ucObject(mode, i, o)`、`ucObjectCount(mode)`、`ucShapeName(s)` 已經包好這些，看 [MATRIX 範例 3](/sensors/unicam/matrix-examples.md)。

## 逐格對照與範例

- [讀值參考：MATRIX](/sensors/unicam/reference.md#host-mx)
- [MATRIX 範例程式](/sensors/unicam/matrix-examples.md)
- 色相碼、格子換算：[產品首頁](/sensors/unicam/index.md)

## 常見問題

- **讀到怪數字？** 確認沒有用到「資料 10」；只有資料 0～9 有意義。也確認相機右上角的「模式-子選項」跟你程式解讀的一樣。
- **用顏色感應器積木讀不到？** 這個產品在 MATRIX 走串口，只能用 mVision 積木。
