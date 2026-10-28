<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# EV3 讀值教學

> [!WARNING]
> **v1 尚未上機驗證**：數值排法已定案並通過電腦端測試，實機數值（fps、循線門檻、I2C 腳位）上機後可能調整。

神攝手（EV3 版）在 EV3 上會顯示成**樂高原廠紅外線感應器**，EV3-G、EV3 Classroom、EV3 MicroPython（Pybricks）都用紅外線感應器的積木或模式讀，不用裝擴充。每一格的完整對照在 [讀值參考：EV3](/sensors/unicam/reference.md#host-ev3)，整支範例在 [EV3 範例](/sensors/unicam/ev3-examples.md)。

## 接線

1. 相機用 EV3 線插主機 **1～4 號孔**，由主機供電。
2. 相機螢幕左上角顯示主機名稱：**綠色＝連線中**、紅色＝斷線。
3. 在相機的 **MODE** 選單選相機模式和子選項（見 [產品首頁](/sensors/unicam/index.md) 的「相機上的操作」）。

## 三個紅外線模式

| 紅外線模式 | 幾個值 | 內容 |
| :--- | :---: | :--- |
| 模式 0 近距（Proximity） | 1 | **相機模式編號**（相機模式 × 10 ＋ 子選項，例 13＝顏色追蹤 ③ 標準）：程式可以先讀它，確認相機設對了 |
| 模式 1 信標（Beacon） | 8 | 通道 1～4 的「方向」「距離」共 8 個值，放主要資料 |
| 模式 2 遙控（Remote） | 4 | 通道 1～4 各 1 個值；只有子選項 ①、照順序給色號、循線用得到 |

- 每個值是 −127～100 的整數。**換模式約 0.5 秒**：同一個迴圈裡不要一直在模式 1、2 之間切換，先想好要用哪一個。
- 模式 5 是裝置 ID（不用管）。

## 怎麼解讀

- **位置用 8×6 格**：讀到的數＝X×10＋Y，X＝讀到的數 ÷ 10（取整數，1～8）、Y＝除以 10 的餘數（1～6）。**EV3 的 Y 是下＝小**（畫面最下面一列是 1）。0＝沒看到。
- **面積**：0～100 %，0＝沒看到。
- **色相**（座標取色、照順序給色號）：正數 0～100＝色相碼 × 100 ÷ 240（紅 0、黃約 17、綠約 33、青 50、藍約 67、紫約 83）；**負數＝黑白灰**：−1～−8 白→灰、−9～−15 黑。
- **循線**：偏移、角度、左右緣照原值送，可以是負數（負＝線在左）。

以**顏色追蹤 ③ 標準**（4 色 XY＋面積）為例，全部在模式 1：

| 要的值 | 積木（模式 1 信標） |
| :--- | :--- |
| 色 1 位置（X×10＋Y） | 通道 1 方向 |
| 色 1 面積 % | 通道 1 距離 |
| 色 2 位置、面積 | 通道 2 方向、距離 |
| 色 3、色 4 | 通道 3、通道 4 |

## EV3-G／EV3 Classroom

- **EV3-G**：紅外線感應器積木 →「測量」→「近接」讀模式 0；「信標」選頻道 1～4，輸出的「方向」「近接」就是上表的方向、距離；「遙控」選頻道讀模式 2。
- **EV3 Classroom**：用 Infrared Sensor 的 proximity、beacon heading／beacon distance（選 channel）、remote 積木，對應同上。
- 讀到的數照上面「怎麼解讀」換算：EV3-G 用「數學」積木的除法和「取餘數」（Modulo）。

## EV3 MicroPython（Pybricks）

用 `LUMPDevice` 讀原始模式（EV3 MicroPython v2 寫法相同）。相機設成**顏色追蹤 ③ 標準**：

```python
#!/usr/bin/env pybricks-micropython
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port
from pybricks.tools import wait

cam = LUMPDevice(Port.S1)        # 相機插 1 號孔

print("相機模式", cam.read(0)[0])   # 模式 0：例 13 = 顏色追蹤 ③ 標準

while True:
    v = cam.read(1)              # 模式 1：信標 8 個值
    pos, area = v[0], v[1]       # 色 1：X×10+Y、面積 %
    if pos:
        print("色 1：X", pos // 10, "Y", pos % 10, "面積", area, "%")
    else:
        print("色 1：沒看到")
    wait(100)
```

子選項 ① 的第 9 色起在模式 2：`cam.read(2)`（4 個值）。

## 逐格對照與範例

- [讀值參考：EV3](/sensors/unicam/reference.md#host-ev3)（每個相機模式・子選項一張表）
- [EV3 範例程式](/sensors/unicam/ev3-examples.md)
- 色相碼、格子換算：[產品首頁](/sensors/unicam/index.md)

## 常見問題

- **EV3 顯示成紅外線感應器？** 正常：相機模擬成樂高原廠感應器，用原廠積木就能讀。
- **模式 0 讀到的不是我要的模式？** 到相機的 MODE 選單改；程式只能讀，不能切換相機模式。
- **數值偶爾卡住半秒？** 換紅外線模式要約 0.5 秒，盡量固定讀同一個模式。
