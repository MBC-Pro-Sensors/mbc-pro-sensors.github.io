<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# SPIKE 讀值教學

> [!WARNING]
> **v1 尚未上機驗證**：數值排法已定案並通過電腦端測試，實機數值（fps、循線門檻、I2C 腳位）上機後可能調整。

神攝手用在 SPIKE Prime（黃色主機）與 Robot Inventor（藍綠色主機）。SPIKE 版韌體會自己認出主機跑的是官方 App 還是 Pybricks，不用換韌體。這一頁講四種讀法怎麼選、最小的讀值程式；每一格的完整對照在 [讀值參考](/sensors/unicam/reference.md)，整支範例在 [SPIKE 範例](/sensors/unicam/spike-examples.md)。

## 先選讀法

| 讀法 | 要準備什麼 | 顏色追蹤 ① 最多色 | 適合 |
| :--- | :--- | :---: | :--- |
| **Pybricks 函式庫**（模式 9／6） | 主機換 Pybricks 韌體＋匯入 MBC 函式庫 | 14 色 | 比賽、要最多資料、要循線全部的值 |
| **Pybricks 內建積木**（模式 5／7） | 主機換 Pybricks 韌體，不用函式庫 | 2 色 | 只要 1～2 色、用 Pybricks 積木寫 |
| **官方 SPIKE App 教育版**（模式 0） | SPIKE Prime 原廠韌體 | 積木 7 色 | 上課、不想換韌體 |
| **SPIKE App 家用版**（模式 0） | Robot Inventor 原廠韌體 | 4 色 | 家用版主機 |

其他相機模式每種讀法給幾色，看 [各上位機給幾色](/sensors/unicam/reference.md#counts)。

## 接線與相機設定

1. 相機插主機 **A～F 任一孔**，由主機供電，不用電池。
2. 相機螢幕左上角會顯示主機名稱：**綠色＝連線中**、紅色＝斷線。
3. 在相機的 **MODE** 選單選相機模式和子選項（操作見 [產品首頁](/sensors/unicam/index.md) 的「相機上的操作」）。子選項名稱會顯示這個主機實際拿到幾色。
4. **主機讀不到相機現在是哪個模式**，程式要跟相機設定一致：Pybricks 函式庫建構時傳模式編號（相機模式 × 10 ＋ 子選項，例 `mode=13`；照順序給色號＝`30`），官方 App 就照你選的模式去解讀。

## Pybricks 函式庫（模式 9）

模式 9 一次讀 7 個 int16＝14 byte，一個 byte 一個值；座標是 160×120 像素（Y 上＝小），0＝沒有目標。MBC 函式庫會照模式編號自動拆好（14 色的格號拆成 X、Y、數量從模式 6 讀），函式庫下載和整支範例在 [SPIKE 範例](/sensors/unicam/spike-examples.md)。

> [!IMPORTANT]
> 這一代神攝手的模式 9 排法**跟舊版學校版／Kito 版不同**，Kito 的 `camLibForKitoMode9` 不能直接拿來用。

不裝函式庫也能讀：用 Pybricks 內建的 `PUPDevice` 讀模式 9，自己拆成 byte。下面是相機設成**顏色追蹤 ③ 標準**（模式編號 13）時讀色 1：

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port
from pybricks.tools import wait

cam = PUPDevice(Port.A)          # 相機插 A 孔

def cam_bytes():
    """讀模式 9：7 個 int16 拆成 14 個 byte（低位元組在前）"""
    out = []
    for v in cam.read(9):
        out.append(v & 0xFF)
        out.append((v >> 8) & 0xFF)
    return out

while True:
    b = cam_bytes()
    x, y, area = b[0], b[1], b[2]    # 標準層：色 1 的 X、Y、面積 %
    if x:
        print("色 1：X", x, "Y", y, "面積", area, "%")
    else:
        print("色 1：沒看到")
    wait(100)
```

循線（模式編號 41／42／43）的偏移、角度、左右緣會有負數，是**有號 byte**：

```python
def s8(v):                       # 0～255 轉成 −128～127
    return v - 256 if v > 127 else v

b = cam_bytes()
near = s8(b[0])                  # 近處偏移 −100～100（負＝線在左）
angle = s8(b[3])                 # 線角度 −90～90
junction = b[6]                  # 路口類型：0 無 1 十字 2 T 3 左 4 右 5 盡頭
```

- **模式 6**：只有顏色追蹤 ① 最多色、② 多色要讀數量時才讀（`cam.read(6)`，換模式多等約 30 ms）；排法看 [讀值參考](/sensors/unicam/reference.md#pyb-11)。
- **模式 8**：裝置 ID（全產品共用，MBC 積木網站用它認產品）。

### 插在 EXP6 擴充器上

孔不夠用時，神攝手可以插在 [EXP6 六路擴充器](/sensors/exp6/index.md) 的 1～6 號孔。改用 EXP6 v2 函式庫 `MBC_exp6_obj_Lib_v2.py`（[下載區](/downloads.md)），功能名稱跟上面的 `UniCam` 一樣；讀不到時回傳 `None`。EXP6 韌體要是 v2。官方 SPIKE App 也有同名寫法（`MBC_exp6_SPIKE_App_Lib_v2.py`）。

```python
from MBC_exp6_obj_Lib_v2 import MBC_EXP6, run_task

exp6 = MBC_EXP6(1, True)      # EXP6 插在主機 A 孔
cam = exp6.unicam(5, 13)      # 神攝手插在 EXP6 的 5 號孔，mode 要跟相機選單一樣

async def main():
    while True:
        print(await cam.xy(1))

run_task(main())
```

> [!NOTE]
> 函式庫已用電腦跟直插版 `UniCam` 逐項比對過，**尚未上機驗證**。

## Pybricks 內建積木（模式 5／7）

不裝函式庫，只用 Pybricks 內建的**顏色感應器**積木：`reflection()`（模式 5）和 `ambient()`（模式 7），每種相機模式各給 1～2 色。顏色追蹤跟 Kito 版一樣是 40×30 格：

```python
from pybricks.pupdevices import ColorSensor
from pybricks.parameters import Port
from pybricks.tools import wait

cam = ColorSensor(Port.A)        # 相機插 A 孔

while True:
    r = int(cam.reflection())
    if r < 0:
        r += 6399                # 超過 3199 會顯示成負數，加 6399 修回
    x1, y1 = r // 100, r % 100   # 色 1：X 1～40、Y 1～30；0 = 沒看到
    a = cam.ambient()            # 色 2：顯示成 X.Y（例 12.05 = X 12、Y 5）
    x2 = int(a)
    y2 = round((a - x2) * 100)
    print(x1, y1, x2, y2)
    wait(100)
```

座標取色、照順序給色號、循線怎麼放進這兩個值，看 [讀值參考：Pybricks 內建積木](/sensors/unicam/reference.md#host-pyblk)。循線的偏移是 **＋100** 送，讀到後減 100。

## 官方 SPIKE App

官方 App 會把相機顯示成**顏色感應器**。用「更多感應器」分類裡的**顏色、反射光、原始紅／綠／藍**積木讀，再照下面換算。座標是格子（Y 上＝小），0＝沒看到。

### 教育版（SPIKE Prime 黃色主機）

以**顏色追蹤 ③ 標準**（4 色，16×12 格）為例：

| 要的值 | 積木 | 換算 |
| :--- | :--- | :--- |
| 色 1 X（1～16） | 顏色 | 讀到的數 |
| 色 1 Y（1～12） | 反射光 | 讀到的數 |
| 色 2 X | 原始紅 | 原始紅 除以 256 的**餘數** |
| 色 2 Y | 原始紅 | 原始紅 ÷ 256，取整數 |
| 色 3、色 4 | 原始綠、原始藍 | 同色 2 |

**顏色追蹤 ① 最多色、② 多色**改用 8×6 格、一格兩色：顏色＝找到幾色、反射光＝色 1 的 X×10＋Y；原始紅＝色 2 ×100 ＋ 色 3，先 ÷100 拆成兩個「X×10＋Y」，再各自 ÷10 拆成 X、Y。

各色的數量、面積放在模式 0 的 byte 8～15，**積木讀不到**；App 的 Python 讀不讀得到還要上機確認。

### 家用版（Robot Inventor 藍綠色主機）

主機送 0x25 時相機自動換成家用版排法：顏色只到 0～10、反射光最多 126、原始紅綠藍 0～255，每格放 1 個值。顏色追蹤是 8×6 格：

| 要的值 | 積木 | 換算 |
| :--- | :--- | :--- |
| 色 1 X（1～8） | 顏色 | 讀到的數 |
| 色 1 Y（1～6） | 反射光 | 讀到的數 |
| 色 2 | 原始紅 | X＝原始紅 ÷ 10（取整數）、Y＝除以 10 的餘數 |
| 色 3、色 4 | 原始綠、原始藍 | 同色 2 |

其他相機模式的每一格看 [讀值參考：教育版](/sensors/unicam/reference.md#host-edu)、[家用版](/sensors/unicam/reference.md#host-toy)。循線、會有負數的值是 **＋100** 送。

## 相機模式 5 物體偵測

相機把畫面裡的物體由大到小排好（物體 1 最大），每個物體給**物體代碼＝色號 × 10 ＋ 形狀**（例 13＝目標 1 的三角形，0＝沒有）。形狀：0 其他、1 圓、2 橢圓、3 三角、4 正方、5 長方、6 五邊、7 六邊、8 星、9 梯形。X、Y 是物體的**質心**；只看目標色 1～8。模式編號 **51＝① 物體清單**、**52＝② 物體詳細**。代碼再拆：色號＝代碼 ÷ 10（取整數）、形狀＝代碼除以 10 的餘數。

**Pybricks 函式庫**（最簡單；[下載 MBC_UniCam.py](/examples/unicam/pybricks/MBC_UniCam.py)，跟程式一起匯入）：

```python
from pybricks.parameters import Port
from pybricks.tools import wait
from MBC_UniCam import UniCam, shape_name

cam = UniCam(Port.A, mode=51)            # 相機切在 物體偵測 ① 物體清單
while True:
    print("物體數", cam.object_count())
    for i in range(1, cam.max_object() + 1):     # SPIKE 給 4 個物體，物體 1 最大
        if cam.object_found(i):
            x, y = cam.object_xy(i)
            print(" 物體", i, "代碼", cam.object(i), shape_name(cam.object_shape(i), True), "X", x, "Y", y)
    wait(200)
```

`mode=52` 改成 2 個物體，另有 `object_size(i)`（寬、高）、`object_area(i)`（面積 %），沒有物體數。方法一覽與整支範例看 [SPIKE 範例 6](/sensors/unicam/spike-examples.md)。

**不裝函式庫（模式 9 自己拆）**：

| 模式 | byte 0 | 之後 |
| :---: | :--- | :--- |
| 51 | 物體數 0～8 | 每個物體 3 byte：代碼、X、Y（物體 1＝byte 1～3 … 物體 4＝byte 10～12） |
| 52 | 物體 1 色號 | 每個物體 7 byte：色號、形狀、X、Y、寬、高、面積 %（物體 1＝byte 0～6、物體 2＝byte 7～13）；沒有物體數、角度 |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port
from pybricks.tools import wait

cam = PUPDevice(Port.A)          # 相機切在 物體偵測 ② 物體詳細（模式編號 52）

while True:
    b = []
    for v in cam.read(9):        # 7 個 int16 拆成 14 個 byte（低位元組在前）
        b.append(v & 0xFF)
        b.append((v >> 8) & 0xFF)
    for i in range(2):           # 52：每個物體 7 byte，物體 1 最大
        color, shape, x, y, w, h, area = b[7 * i:7 * i + 7]
        if color:                # 色號 0 = 沒有這個物體
            print("物體", i + 1, "代碼", color * 10 + shape, "X", x, "Y", y, "寬", w, "高", h, "面積", area, "%")
    wait(100)
```

**Pybricks 內建積木**（`reflection()`、`ambient()`，只給物體 1）：`reflection()`＝物體 1 的 X 格 × 100 ＋ Y 格（40×30 格：X 1～40、Y 1～30，上＝小，0＝沒有；負數加 6399 修回）；`ambient()` 顯示 A.BB：A＝物體 1 代碼、BB＝物體數（51）或物體 1 面積 %（52，100 顯示成 99）。

```python
from pybricks.pupdevices import ColorSensor
from pybricks.parameters import Port
from pybricks.tools import wait

cam = ColorSensor(Port.A)        # 相機切在 物體偵測 ①（51）或 ②（52）：只給物體 1

while True:
    r = int(cam.reflection())
    if r < 0:
        r += 6399                # 超過 3199 會顯示成負數，加 6399 修回
    gx, gy = r // 100, r % 100   # 物體 1 位置：40×30 格（X 1～40、Y 1～30，上＝小）；0 = 沒有
    a = cam.ambient()            # 顯示 A.BB
    code = int(a)                # A：物體 1 代碼（色號 × 10 ＋ 形狀）
    bb = round((a - code) * 100) # BB：51＝物體數、52＝物體 1 面積 %（100 顯示成 99）
    print("代碼", code, "色號", code // 10, "形狀", code % 10, "格", gx, gy, bb)
    wait(100)
```

**官方 SPIKE App**（積木讀得到的 5 個值）：

| 積木 | 教育版 51 | 教育版 52 | 家用版 51 | 家用版 52 |
| :--- | :--- | :--- | :--- | :--- |
| 顏色 | 物體數 | 物體 1 代碼 | 物體數 | 物體數 |
| 反射光 | 物體 1 代碼 | 物體 1 面積 % | 物體 1 代碼 | 物體 1 代碼 |
| 原始紅 | 物體 1 位置（X×10＋Y） | 物體 1 X＝餘數（÷256）、Y＝÷256 取整數 | 物體 1 位置（X×10＋Y） | 物體 1 X（像素） |
| 原始綠 | ÷100 取整數＝物體 2 代碼、餘數＝物體 2 位置 | 物體 1 寬＝餘數、高＝÷256 | 物體 2 代碼 | 物體 1 Y（像素） |
| 原始藍 | 同原始綠，物體 3 | 物體 2 代碼＝餘數、面積 %＝÷256 | 物體 2 位置 | 物體 1 面積 % |

位置是 8×6 格「X×10＋Y」：X＝÷10 取整數（1～8）、Y＝除以 10 的餘數（1～6，上＝小），像素 ≈ 格 × 20 − 10。教育版 App Python 可以用讀值小工具 [MBC_UniCam_App_helper.py](/examples/unicam/spike_app/MBC_UniCam_App_helper.py) 的 `object(i)`、`object_cell(i)`、`object_xy(1)`…；教育版 byte 8～15（物體 4～7、角度、頂點數）積木讀不到。

## 逐格對照與範例

- [讀值參考：Pybricks 函式庫](/sensors/unicam/reference.md#host-pyb)・[SPIKE App 教育版](/sensors/unicam/reference.md#host-edu)・[家用版](/sensors/unicam/reference.md#host-toy)・[Pybricks 內建積木](/sensors/unicam/reference.md#host-pyblk)
- [SPIKE 範例程式（函式庫下載）](/sensors/unicam/spike-examples.md)
- 相機模式、色相碼、格子換算：[產品首頁](/sensors/unicam/index.md)

## 常見問題

- **螢幕左上角一直是紅色？** 主機還沒連上：拔掉重插、等 2 秒，或換一個孔。
- **讀到的值跟表上對不起來？** 先確認相機右上角的「模式-子選項」（例 1-3）跟你程式用的一樣。
- **舊版（學校版）的程式可以直接用嗎？** 不行，排法不同；舊版說明保留在 [神攝手舊版（停止支援）](/sensors/esp32cam/index.md) 供參考。
