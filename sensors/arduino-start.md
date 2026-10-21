<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# 🔰 第一次用 Arduino：從零開始

這一頁寫給**完全沒寫過程式**的人。照著做一次，大約 15 分鐘，你就能把 MBC 感應器接上 Arduino，在電腦上看到感應器傳回來的數字。之後每個產品的範例都用同樣的方法操作。

> [!TIP]
> 看不懂的名詞先跳過，照步驟做就好。做完一次，自然就懂了。

---

## 你需要準備

| 東西 | 說明 |
| :--- | :--- |
| Arduino UNO（或 Nano） | 最常見的 Arduino 板子。Mega、UNO R4、ESP32 也可以，接腳見下方表格 |
| USB 線 | UNO 用「方頭（USB-B）」線；Nano 依板子是 Mini USB 或 Type-C。**要能傳資料的線**，有些便宜的線只能充電 |
| 電腦 | Windows、Mac 都可以 |
| MBC 感應器（通用 I2C 版） | 附 4 條線的連接線 |

---

## 第 1 步：安裝 Arduino IDE

Arduino IDE 是寫程式、把程式送進 Arduino 的免費軟體。

1. 到官網下載：<https://www.arduino.cc/en/software>，選你的電腦系統（Windows 選 **Windows Win 10 and newer**）。
2. 下載頁會問要不要捐款，點 **JUST DOWNLOAD** 就能直接下載。
3. 安裝時一路按「下一步」，中途問要不要安裝驅動程式，全部按「安裝」。
4. 打開 Arduino IDE。想改成中文介面：**File → Preferences → Language** 選「中文(繁體)」，按 OK 後重開。

## 第 2 步：用 USB 線接上 Arduino

1. 用 USB 線把 Arduino 接上電腦，板子上的燈會亮。
2. 在 Arduino IDE 上方的下拉選單，選你的板子，例如 **Arduino Uno**，右邊會顯示 COM 幾（Mac 是 `/dev/cu.usb…`）。

> [!WARNING]
> **下拉選單裡找不到板子？** 常見原因有兩個：
> 1. USB 線只能充電、不能傳資料 → 換一條線試試。
> 2. 你的板子是相容板（板子上的 USB 晶片寫著 **CH340**）→ 要先安裝 CH340 驅動程式（搜尋「CH340 driver」下載安裝），裝好後拔掉 USB 再插一次。

## 第 3 步：接上 MBC 感應器

MBC 通用 I2C 版感應器只用 **4 條線**：兩條電源（VCC、GND），兩條訊號（SDA、SCL）。

| 感應器上的字 | Arduino UNO / Nano | Arduino Mega | UNO R4 | ESP32 |
| :---: | :---: | :---: | :---: | :---: |
| **VCC**（或 5V） | 5V | 5V | 5V | 5V（板子上標 VIN 或 5V 的腳） |
| **GND** | GND | GND | GND | GND |
| **SDA** | A4 | 20 | A4 | GPIO 21 |
| **SCL** | A5 | 21 | A5 | GPIO 22 |

> [!IMPORTANT]
> - **接線前先拔掉 USB 線**，接好、檢查一次再插回去。
> - UNO 板子靠近 USB 接頭那一側，最上面兩個沒寫數字的腳也標著 **SDA、SCL**，跟 A4、A5 是同一組，接哪邊都可以。
> - 每個產品的接頭排列不太一樣，**請以感應器板子上印的字為準**。

## 第 4 步：把範例程式送進 Arduino

1. 在產品的範例頁，按程式框右上角的 **「複製」**，或下載 `.ino` 檔。
2. 在 Arduino IDE：**檔案 → 新增**，把原本的內容全部刪掉，貼上剛剛複製的程式。
3. 按左上角的 **→（上傳）** 按鈕。第一次會問要存在哪裡，隨便取個名字存起來就好。
4. 下方出現「**上傳完成**」（Done uploading）就成功了。

> [!TIP]
> 上傳失敗、出現紅色字？最常見的是**沒選對板子或 COM 埠**，回到第 2 步再選一次。

## 第 5 步：打開「序列埠監控窗」看數字

Arduino 沒有螢幕，範例會把感應器讀到的東西「印」到電腦上給你看。

1. 按 Arduino IDE 右上角的 **放大鏡圖示**（序列埠監控窗）。
2. 下方視窗右邊的速度選單選 **115200 baud**（所有 MBC 範例都用這個速度）。
3. 你應該會看到範例印出來的文字和數字一直更新。

> [!WARNING]
> **看到一堆亂碼？** 速度沒選 115200。改好就正常了。
>
> **一直出現「找不到感應器」？** 檢查第 3 步的 4 條線：SDA、SCL 是不是接反、VCC 和 GND 有沒有插緊。

---

## 程式裡的符號是什麼意思？

範例裡每一行都有中文說明，先認識這幾個就夠了：

| 你看到的 | 意思 |
| :--- | :--- |
| `//` 後面的字 | **說明文字**，給人看的，Arduino 不會執行 |
| `void setup() { … }` | 開機時**只做一次**的事（例如：準備好感應器） |
| `void loop() { … }` | **一直重複做**的事（例如：一直讀感應器） |
| `delay(500);` | 等一下，單位是毫秒，`500` = 0.5 秒 |
| `Serial.println(…);` | 把東西印到序列埠監控窗，然後換行 |
| `if (…) { … }` | 「如果」括號裡的事成立，就做大括號裡的事 |
| `int distance = …;` 這類 | 開一個叫 `distance` 的「盒子」（變數）把數字存起來，後面可以拿出來用。盒子名稱只能用英文和數字 |

> [!TIP]
> 範例裡標著 **「👉 可以改」** 的數字，都是安全、可以自己改改看的地方。改完再按一次上傳，看看有什麼不一樣。

---

## 各產品的 Arduino 範例

| 產品 | 範例頁 |
| :--- | :--- |
| 循行者 8 路 | [Arduino 範例](/sensors/line8/arduino-examples.md) |
| 循行者 16 路 | [Arduino 範例](/sensors/line16/arduino-examples.md) |
| 測距者 8 路 | [Arduino 範例](/sensors/tof8/arduino-examples.md) |
| 測距者 2 路（新版即將推出） | [Arduino 範例](/sensors/tof2/arduino-examples.md) |
| 掌控者 PS2 | [Arduino 範例](/sensors/ps2/arduino-examples.md) |
| 掌控者 PS4/PS5 | [Arduino 範例](/sensors/ps4/arduino-examples.md) |
| 陀螺儀 IMU | [Arduino 範例](/sensors/imu/arduino-examples.md) |
| SPIKE 6 路擴充器 | [Arduino 範例](/sensors/exp6/arduino-examples.md)（這款用序列埠接線，跟上面第 3 步不同，請看範例頁） |

想知道所有 MBC 產品共用的通訊規則（給會寫程式的人看），請看 [MBC 通用 I2C 協議](/i2c-protocol.md)。
