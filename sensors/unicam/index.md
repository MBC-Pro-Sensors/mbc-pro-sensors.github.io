<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# 神攝手通用版 v1（開發中）

> [!WARNING]
> **v1 尚未上機驗證**：數值排法已定案並通過電腦端測試，實機數值（fps、循線門檻、I2C 腳位）上機後可能調整。

> [!TIP]
> **第一次用？先玩互動教學**：不用接主機，在網頁上拖色塊、取樣點、線，程式跟著長出來。有 Pybricks（SPIKE）、EV3、Arduino、mBlock、MATRIX、micro:bit 六種寫法 → [開始互動教學](/learn/index.md#unicam)

<!-- product-hero -->

<!-- platform-grid -->
<!-- 通用版還沒進 _build/guides.json 三層導覽（gen-guides 會改寫 reference.md 與範例頁），上面的 platform-grid 目前是空的；主機格子由 _build/gen-unicam.mjs 產生在下面。以後整合進三層導覽時，刪掉 gen:unicam-hosts 這一段。 -->
<!-- gen:unicam-hosts -->
<div class="pick" id="pick-host"><h2 class="pick-title">① 選你的主機</h2><p class="pick-sub">每個主機一頁：怎麼接、怎麼讀、最小的程式寫法。逐格對照在「讀值參考」。</p>
<div class="pick-grid pick-hosts">
<a class="pick-tile pg-spike" href="/sensors/unicam/spike/index.md"><span class="pt-img"><img src="/images/illust/host-spike.webp" alt="" loading="lazy"></span><span class="pt-name">SPIKE</span><span class="pt-tag">SPIKE Prime（黃色主機）與 Robot Inventor（藍綠色主機）</span><span class="pt-sw">Pybricks 函式庫・Pybricks 積木・官方 App</span></a>
<a class="pick-tile pg-ev3" href="/sensors/unicam/ev3/index.md"><span class="pt-img"><img src="/images/illust/host-ev3.webp" alt="" loading="lazy"></span><span class="pt-name">EV3</span><span class="pt-tag">MINDSTORMS EV3，顯示成紅外線感應器</span><span class="pt-sw">EV3-G・EV3 Classroom・EV3 MicroPython</span></a>
<a class="pick-tile pg-matrix" href="/sensors/unicam/matrix/mvision.md"><span class="pt-img"><img src="/images/illust/host-matrix.webp" alt="" loading="lazy"></span><span class="pt-name">MATRIX</span><span class="pt-tag">MATRIX Mini R4，mVision 攝影機積木</span><span class="pt-sw">mVision 積木</span></a>
<a class="pick-tile pg-arduino" href="/sensors/unicam/arduino/index.md"><span class="pt-img"><img src="/images/illust/host-arduino.webp" alt="" loading="lazy"></span><span class="pt-name">Arduino</span><span class="pt-tag">UNO、Nano、UNO R4、ESP32，用 I2C（0x4C）</span><span class="pt-sw">Arduino IDE</span></a>
<a class="pick-tile pg-microbit" href="/sensors/unicam/microbit/index.md"><span class="pt-img"><img src="/images/illust/host-microbit.webp" alt="" loading="lazy"></span><span class="pt-name">micro:bit</span><span class="pt-tag">micro:bit V1／V2，用 I2C（0x4C）</span><span class="pt-sw">MakeCode</span></a>
<a class="pick-tile pg-mblock" href="/sensors/unicam/mblock/index.md"><span class="pt-img"><img src="/images/illust/host-mblock.webp" alt="" loading="lazy"></span><span class="pt-name">mBlock</span><span class="pt-tag">mBot、Ranger、Orion、MegaPi，RJ25 串口 38400</span><span class="pt-sw">mBlock 5（MBC 感應器擴展 v2.1.0）</span></a>
</div></div>
<!-- /gen -->

> **一台相機、四種用法：顏色追蹤、座標取色、照順序給色號、循線，在相機選單切換；SPIKE、EV3、MATRIX、Arduino、micro:bit 都讀得到，各平台給到它放得下的最多資料。**

## 📸 4 個相機模式

在相機的 **MODE** 選單選相機模式，再選子選項。**子選項＝層級**：層級越精細，每個目標給的值越多、目標數越少；實際給幾色看下一節的總表。模式編號＝相機模式 × 10 ＋ 子選項（例：13＝顏色追蹤 ③ 標準；照順序給色號＝30）。

### 1 顏色追蹤

先在相機上教顏色（最多 16 色），相機找出每個顏色的色塊，給位置 X、Y，以及寬、高、數量、面積占比（看層級）。

<figure class="ref-fig"><img src="/images/sensors/unicam/color-track.svg" alt="顏色追蹤：每個顏色給 X、Y、寬 W、高 H" loading="lazy"></figure>

| 子選項 | 給什麼 | 適合 |
| :--- | :--- | :--- |
| ① 最多色 | 每色只給位置（和數量），色數最多 | 場上很多種顏色，只要知道在哪 |
| ② 多色 | 色數次多、位置比較準 | 多色、要比較準的位置 |
| ③ 標準（XY＋面積） | 每色位置＋面積（＋數量） | 紅黃綠藍 4 個方塊這種最常見的題目 |
| ④ 精細（XYWH） | 位置、寬高、數量、面積 | 色數少、要用寬高估距離 |

### 2 座標取色

在畫面上固定幾個取樣點（S1、S2…），每個點給色相；層級越高再加飽和度（鮮豔度）、明度（亮度）。取樣點在相機上設，或用 I2C 命令 E1 改位置。WRO 任務賽「這一格放的是什麼顏色」用這個。

<figure class="ref-fig"><img src="/images/sensors/unicam/sample-points.svg" alt="座標取色：畫面上的取樣點 1～20" loading="lazy"></figure>

預設排法：8 點以內排一列，超過 8 點排兩列（上圖是 20 點）。每個點取周圍 3×3 像素的平均。

| 子選項 | 每個點給什麼 |
| :--- | :--- |
| ① 色相 | 色相碼 |
| ② 色相＋飽和 | 色相碼、飽和度（可以分出淺色和深色） |
| ③ 色相＋飽和＋明度 | 色相碼、飽和度、明度（H、S、V） |

### 3 照順序給色號

畫面裡的色塊**由左到右**排，依序給每一塊的色相碼（用教色時那個顏色的色相碼），另外給總塊數。WRO 任務賽的排列題直接用。沒有子選項。

<figure class="ref-fig"><img src="/images/sensors/unicam/color-order.svg" alt="照順序給色號：色塊由左到右編號 1～5，色塊上是色相碼" loading="lazy"></figure>

> [!TIP]
> 紅色的色相碼是 **0**，跟「沒有」一樣是 0。先讀**總塊數**，才知道前幾格有效。

### 4 循線

鏡頭斜看前方，在畫面的**近、中、遠三條掃描列**找線：近處偏移拿來做 PID，遠處偏移和角度可以提前知道彎道，路口類型直接告訴你是十字、T 字還是左右轉，也會回報穿過的彩色標記線（JBC 高手組）。

<figure class="ref-fig"><img src="/images/sensors/unicam/line-follow.svg" alt="循線：近、中、遠三條掃描列，箭頭是偏移，虛線是角度" loading="lazy"></figure>

| 子選項 | 說明 |
| :--- | :--- |
| ① 黑線 | 白色地面上的黑線，門檻自動調 |
| ② 白線 | 深色地面上的白線 |
| ③ 色線 | 對準線按 OK 取色（色線＝目標色 1） |

循線給的值：近／中／遠偏移（−100～100，負＝線在左）、線角度（−90～90）、近處與遠處線寬 %、有線旗標（近 1＋中 2＋遠 4）、路口類型（0 無、1 十字、2 T、3 左、4 右、5 盡頭）與路口距離 %、近處線左右緣、彩色標記線色號與累計條數、最後離線方向（0 有線、1 左、2 右）。三種線用同一種排法，每個上位機給它放得下的值。

## 📊 各上位機給幾色

同一個子選項，各上位機能放的資料量不同，相機會**自動給到它放得下的最多**：學生教了幾色就給幾色，放不下的就少給。點數字看那張逐格表。

<!-- gen:unicam-counts -->
| 相機模式・子選項 | Pybricks 函式庫 | App 教育版 | App 家用版 | Pybricks 積木 | EV3 | MATRIX | I2C |
| :--- | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| 1 顏色追蹤 ① 最多色 | [14 色](/sensors/unicam/reference.md#pyb-11) | [14 色](/sensors/unicam/reference.md#edu-11) | [4 色](/sensors/unicam/reference.md#toy-11) | [2 色](/sensors/unicam/reference.md#pyblk-11) | [12 色](/sensors/unicam/reference.md#ev3-11) | [16 色](/sensors/unicam/reference.md#mx-11) | [16 色 XY](/sensors/unicam/reference.md#i2c-11) |
| 1 顏色追蹤 ② 多色 | [7 色](/sensors/unicam/reference.md#pyb-12) | [7 色](/sensors/unicam/reference.md#edu-12) | [4 色](/sensors/unicam/reference.md#toy-12) | [2 色](/sensors/unicam/reference.md#pyblk-12) | [8 色](/sensors/unicam/reference.md#ev3-12) | [10 色](/sensors/unicam/reference.md#mx-12) | [16 色 XY＋數量](/sensors/unicam/reference.md#i2c-12) |
| 1 顏色追蹤 ③ 標準（XY＋面積） | [4 色 XY＋面積](/sensors/unicam/reference.md#pyb-13) | [4 色 XY＋面積](/sensors/unicam/reference.md#edu-13) | [4 色 XY＋面積](/sensors/unicam/reference.md#toy-13) | [2 色 XY](/sensors/unicam/reference.md#pyblk-13) | [4 色 XY＋面積](/sensors/unicam/reference.md#ev3-13) | [5 色 XY＋面積](/sensors/unicam/reference.md#mx-13) | [16 色 XY＋面積＋數量](/sensors/unicam/reference.md#i2c-13) |
| 1 顏色追蹤 ④ 精細（XYWH） | [2 色 XYWH](/sensors/unicam/reference.md#pyb-14) | [2 色 XYWH](/sensors/unicam/reference.md#edu-14) | [2 色 XYWH](/sensors/unicam/reference.md#toy-14) | [1 色 XYWH](/sensors/unicam/reference.md#pyblk-14) | [2 色 XYWH](/sensors/unicam/reference.md#ev3-14) | [3 色 XYWH](/sensors/unicam/reference.md#mx-14) | [16 色 XYWH＋數量＋面積](/sensors/unicam/reference.md#i2c-14) |
| 2 座標取色 ① 色相 | [14 點色相](/sensors/unicam/reference.md#pyb-21) | [14 點色相](/sensors/unicam/reference.md#edu-21) | [4 點色相](/sensors/unicam/reference.md#toy-21) | [2 點色相](/sensors/unicam/reference.md#pyblk-21) | [12 點色相](/sensors/unicam/reference.md#ev3-21) | [20 點色相](/sensors/unicam/reference.md#mx-21) | [16 點色相](/sensors/unicam/reference.md#i2c-21) |
| 2 座標取色 ② 色相＋飽和 | [7 點色相＋飽和](/sensors/unicam/reference.md#pyb-22) | [7 點色相＋飽和](/sensors/unicam/reference.md#edu-22) | [2 點色相＋飽和](/sensors/unicam/reference.md#toy-22) | [2 點色相＋飽和](/sensors/unicam/reference.md#pyblk-22) | [4 點色相＋飽和明度](/sensors/unicam/reference.md#ev3-22) | [10 點色相＋飽和](/sensors/unicam/reference.md#mx-22) | [16 點色相＋飽和](/sensors/unicam/reference.md#i2c-22) |
| 2 座標取色 ③ 色相＋飽和＋明度 | [4 點色相＋飽和＋明度](/sensors/unicam/reference.md#pyb-23) | [4 點色相＋飽和＋明度](/sensors/unicam/reference.md#edu-23) | [2 點色相＋飽和明度](/sensors/unicam/reference.md#toy-23) | [1 點原始色相＋飽和＋明度](/sensors/unicam/reference.md#pyblk-23) | [2 點原始色相＋飽和＋明度](/sensors/unicam/reference.md#ev3-23) | [6 點色相＋飽和＋明度](/sensors/unicam/reference.md#mx-23) | [16 點色相＋飽和＋明度](/sensors/unicam/reference.md#i2c-23) |
| 3 照順序給色號 | [13 塊色相碼](/sensors/unicam/reference.md#pyb-30) | [13 塊色相碼](/sensors/unicam/reference.md#edu-30) | [3 塊色相碼](/sensors/unicam/reference.md#toy-30) | [2 塊色相碼](/sensors/unicam/reference.md#pyblk-30) | [11 塊色相碼](/sensors/unicam/reference.md#ev3-30) | [18 塊色相碼](/sensors/unicam/reference.md#mx-30) | [16 塊色相碼](/sensors/unicam/reference.md#i2c-30) |
| 4 循線 ①②③ 黑線／白線／色線 | [14 個值](/sensors/unicam/reference.md#pyb-41) | [13 個值](/sensors/unicam/reference.md#edu-41) | [5 個值](/sensors/unicam/reference.md#toy-41) | [4 個值](/sensors/unicam/reference.md#pyblk-41) | [12 個值](/sensors/unicam/reference.md#ev3-41) | [14 個值](/sensors/unicam/reference.md#mx-41) | [14 個值](/sensors/unicam/reference.md#i2c-41) |
<!-- /gen -->

- 完整逐格對照（每一格放什麼、範圍、怎麼拆）：[讀值參考](/sensors/unicam/reference.md)。
- 一個上位機一個韌體版本：**SPIKE 版**（官方 App 與 Pybricks 自動分）、**EV3 版**、**MATRIX 版**、**通用 I2C 版**（Arduino、micro:bit）。

## 🎛️ 相機上的操作

相機背面有 1.8 吋彩色螢幕和 4 顆按鍵，設定都在相機上做，不用電腦。

1. **即時畫面**用左右鍵循環：目標 1 … 目標 N → **MODE** → **SET**。N＝已教到的最後一色＋1 個空位（最多 16）。
2. **停在目標按 OK：教色**（AxisX → AxisY → OK 取色；按住 OK 0.8 秒直接完成；之後可調 Hue／Sta／Size／Locked・MaxArea）。停在目標按**側鍵**：清除這個目標。
3. **停在 MODE 按 OK**：左右鍵選相機模式 1～4 → OK → 選子選項 → OK。子選項名稱顯示**目前上位機實際給的數量**（例：EV3「8 XY」、Pybricks 函式庫「7 XY」）；SPIKE 還沒被主機讀過時顯示層級名稱（Max／Multi／Std／Fine、H／HS／HSV）。只有一個子選項的模式自動跳過。選單裡按側鍵＝回上一層。
4. **停在 SET 按 OK**：鏡頭選單（亮度、對比、飽和度、白平衡、白色校正、自測、回預設）。
5. **畫面**：左上角是主機名稱（綠＝連線中、紅＝斷線）；右上角是「模式-子選項」（例 1-3）和目標編號（1 起算，跟送給上位機的色號一樣）；底部資訊列依模式顯示：顏色追蹤＝目前目標的 X Y W H 數量、座標取色＝S1 的 H S V、照順序給色號＝塊數、循線＝近／遠偏移、角度、路口類型。座標取色會畫取樣點十字和編號，照順序給色號在色塊上標順序，循線畫三條掃描列和找到的線。
6. **斷電記住**：目標色（16 個）、鏡頭設定、相機模式與各模式的子選項、I2C 改過的取樣點位置。第一次燒通用版時會全部回預設（顏色追蹤 ③ 標準）。

> [!NOTE]
> SPIKE 主機讀不到相機目前在哪個模式，程式要跟相機設定一致（Pybricks 函式庫建構時傳模式編號，例 `mode=13`）。EV3 讀模式 0 就知道；I2C 讀 `0x01`，也可以用命令 `E0` 從程式切換。

## 📐 數值的共同規則

- **座標**：像素 X 0～159（左→右）、Y 0～119（上→下）。**0＝沒有目標**，有目標時 X、Y 至少送 1。
- **寬高**：像素（右−左、下−上）。
- **面積占比**：色塊像素數 ÷ 19200 × 100，四捨五入；有找到但不到 1% 送 1。
- **數量**：面積大於最小面積（Size）的區塊數。
- **會有負數的值**（循線偏移、角度、左右緣）：EV3、Pybricks 函式庫（有號 byte）、I2C（int16）照原值送；SPIKE App、MATRIX、Pybricks 積木 **＋100** 送，讀到後減 100。

### 色相碼 0～255

<figure class="ref-fig"><img src="/images/sensors/unicam/hue-code.svg" alt="色相碼色環：0～240 是色相，241～248 白到灰，249～255 黑" loading="lazy"></figure>

| 色相碼 | 顏色 |
| :--- | :--- |
| 0～240 | 色相：紅 0（也是 240）、黃 40、綠 80、青 120、藍 160、紫 200 |
| 241～248 | 白→灰（248＝白） |
| 249～255 | 黑（255＝最黑） |

色度（最大−最小）≤ 24 或飽和度 ≤ 12% 算黑白灰，其中明度 < 64 算黑。飽和度、明度都是 0～100。

- **EV3 的色相**：EV3 一格只能放 −127～100，色相碼 0～240 送「色相碼 × 100 ÷ 240」（四捨五入，所以是 0～100），241～255 送負數 −1～−15（−(色相碼−240)）。EV3 座標取色 ③ 改送原始色相碼兩格（百位、除以 100 的餘數）。

### 各平台的格子（全部是 4:3）

資料量小的平台把像素座標縮成格子：

<figure class="ref-fig"><img src="/images/sensors/unicam/grid-8x6.svg" alt="8×6 格：X×10＋Y；EV3 的 Y 從下面數" loading="lazy"></figure>

| 格子 | 用在 | 換算 |
| :--- | :--- | :--- |
| 16×12 | SPIKE App 教育版（標準、精細） | X÷10＋1、Y÷10＋1 |
| 16×12 格號 | Pybricks 函式庫 14 色（一色 1 byte） | (Y÷10)×16＋X÷10＋1，1～192；函式庫拆回 X、Y |
| 8×6 | SPIKE App 家用版；EV3、教育版、MATRIX 的多色層「X×10＋Y」 | X÷20＋1、Y÷20＋1 |
| 40×30 | Pybricks 內建積木 | X÷4＋1、Y÷4＋1 |

EV3 的 Y 是**下＝小**：(119−Y)÷20＋1；SPIKE 系列照 Kito「上＝小」。寬高：8×6 是 W÷20＋1、H÷20＋1；16×12 是 W÷10＋1、H÷10＋1；40×30 是 W÷4、H÷4。

## 🔀 跟學校版／WRO 版怎麼選

通用版是新的一款韌體，**學校版、WRO 版（HUE12）、Kito 版都不受影響**，各自照原本的說明使用。

| | 學校版 | WRO 版（HUE12） | 通用版 v1 |
| :--- | :--- | :--- | :--- |
| 主要功能 | 取色點＋色塊追蹤＋動態偵測 | 12 個色點的顏色碼與鮮豔度 | 顏色追蹤（最多 16 色）、座標取色、照順序給色號、循線 |
| 在相機上切換用法 | — | — | 4 個相機模式＋子選項 |
| 平台 | SPIKE、EV3、MATRIX、Arduino 等 | SPIKE、EV3、MATRIX | SPIKE、EV3、MATRIX、Arduino、micro:bit、mBlock |
| 狀態 | 出貨中 | 出貨中 | **開發中，尚未上機驗證** |
| 說明 | [學校版／WRO 版首頁](/sensors/esp32cam/index.md) | [WRO 版讀值](/sensors/esp32cam/reference-wro.md) | 本頁 |

- 已經用學校版或 WRO 版上課、比賽、寫好程式的，**照原本的用**，不用換。
- 想要一台相機同時能追多色、取色點、排順序、循線，或要接 Arduino／micro:bit 拿完整數值，選通用版。
- 想先試用通用版，請 [聯絡我們](/contact.md)。

## 🧩 支援平台

| 平台 | 讀法 | 說明 |
| :--- | :--- | :--- |
| SPIKE Prime／Robot Inventor | Pybricks 函式庫（模式 9／6）、Pybricks 內建積木（模式 5／7）、官方 App 教育版／家用版（模式 0） | [SPIKE 讀值教學](/sensors/unicam/spike/index.md) |
| EV3 | EV3-G、EV3 Classroom、EV3 MicroPython：紅外線感應器模式 0／1／2 | [EV3 讀值教學](/sensors/unicam/ev3/index.md) |
| MATRIX Mini R4 | MATRIXblock mVision 積木（10 格） | [MATRIX 讀值教學](/sensors/unicam/matrix/mvision.md) |
| Arduino／ESP32／UNO R4 | I2C 0x4C | [Arduino 讀值教學](/sensors/unicam/arduino/index.md) |
| micro:bit | I2C 0x4C（MakeCode） | [micro:bit 讀值教學](/sensors/unicam/microbit/index.md) |
| mBlock（mBot、Ranger、Orion、MegaPi） | RJ25 串口 38400，MBC 感應器擴展 v2.1.0（開發中，正式上架前可先下載匯入檔試用） | [mBlock 讀值教學](/sensors/unicam/mblock/index.md) |

範例程式：[SPIKE](/sensors/unicam/spike-examples.md)・[EV3](/sensors/unicam/ev3-examples.md)・[Arduino](/sensors/unicam/arduino-examples.md)・[micro:bit](/sensors/unicam/microbit-examples.md)・[MATRIX](/sensors/unicam/matrix-examples.md)・[mBlock](/sensors/unicam/mblock-examples.md)。

## ⚠️ 已知限制與上機待測

v1 還沒接實體硬體測過，下面這些上機後可能調整：

1. **fps**：16 色查表與連通標記比 4 色多做事，要量實際 fps（Kito 4 色約 50 fps）。
2. **I2C 腳位**：SDA＝IO3、SCL＝IO1 是暫定，ESP32 從機能不能用 IO1／IO3 要接 Arduino 實測。**mBlock 版**要接 mBot 實測（電腦端只測過傳輸層）。
3. **循線**只用合成畫面測過（直線、偏移、斜線、十字／T／左右轉／盡頭、白線、色線、彩色標記線、離線方向）；教室燈光、陰影、斜看前方的透視要上機調門檻。
4. **SPIKE App Python** 讀不讀得到模式 0 的 byte 8～15 待確認；積木讀得到的 8 byte 不受影響。
5. **取樣點位置**：預設排法＋I2C E1 修改；相機選單還不能移動取樣點。
6. **照順序給色號**最多收 32 塊（各平台實際給 2～18 塊）；引擎上限 16 色，LEGO 平台最多用到 14 色。
