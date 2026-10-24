### EXP6 走 UART，不是 I2C

EXP6 跟 Arduino 用 **UART 序列埠**（TX／RX 兩條線）溝通，**不用上面的 I2C 接線**。出貨的 EXP6 就能直接用，不用重燒韌體：一開機是「樂高模式」，收到 Arduino 送來的第一包指令就自動切到「UART 模式」（跟 Pybricks 同一套指令）。

| 板子 | 可以用嗎 | 跟 EXP6 講話用 | 說明 |
| :--- | :---: | :---: | :--- |
| **Arduino UNO R4**（Minima／WiFi） | ⭐ 推薦 | D0、D1（`Serial1`） | D0／D1 跟 USB 是分開的兩組序列埠，上傳、看序列埠監控窗都不受影響 |
| Arduino Mega 2560 | ✅ | 18、19（`Serial1`） | 有 4 組序列埠 |
| ESP32 開發板 | ✅ | GPIO16、17（`Serial2`） | 訊號是 3.3V，第 4 腳要接 3V3；要先安裝 ESP32 板子套件 |
| Arduino UNO R3、Nano | ❌ | — | 唯一的硬體序列埠被 USB 用掉；軟體序列埠在 115200 收不穩 |

用「樂高 6 針轉杜邦線」（或樂高接頭轉接板）把 EXP6 的 **Host 孔**接到 Arduino：

| EXP6 Host 孔 | UNO R4 | Mega 2560 | ESP32 |
| :---: | :---: | :---: | :---: |
| 第 3 腳 GND | GND | GND | GND |
| 第 4 腳 VCC | **5V** | **5V** | **3V3** |
| 第 5 腳（EXP6 收） | D1（TX） | 18（TX1） | GPIO17（TX2） |
| 第 6 腳（EXP6 送） | D0（RX） | 19（RX1） | GPIO16（RX2） |
| 第 1、2 腳 | 不接 | 不接 | 不接 |

> [!IMPORTANT]
> - EXP6 的 XT30 電源座一定要接 **2～3S 鋰電池**，馬達和感應器都靠它供電；EXP6 **不會供電給 Arduino**。
> - **TX 接 EXP6 收、RX 接 EXP6 送**，兩條線是交叉的。
> - **第 4 腳的電壓要跟板子的訊號一樣**（UNO R4、Mega 接 5V，ESP32 接 3V3），這一腳也是 EXP6 的「開機鑰匙」：沒電 EXP6 不會開機。
> - 接線前先拔掉 USB 線和電池。

> [!WARNING]
> **看門狗**：EXP6 超過 **0.5 秒**沒收到 Arduino 的任何指令，就自動把馬達停下來。等待一律用 `exp6Wait(毫秒)`，不要用 `delay()`。
