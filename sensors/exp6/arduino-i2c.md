<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- moved: /sensors/exp6/arduino/ide.md -->
<p class="moved-note">這頁內容已整合到新的說明頁：<a href="/sensors/exp6/arduino/ide.md">SPIKE 6 路擴充器 › Arduino › Arduino IDE</a></p>

# ∿ EXP6 × Arduino：請改用序列埠（UART）

[🔙 回到 EXP6 產品介紹](/sensors/exp6/index.md)

> [!IMPORTANT]
> **EXP6 不能用 I2C 連接 Arduino。** EXP6 的主機接頭有一顆**隔離保護晶片**，保護 Arduino 和 EXP6 不會互相燒壞。這顆晶片的訊號只能單向傳送，而 I2C 需要雙向的資料線，所以接 I2C 不會通。
>
> Arduino 請改用**序列埠（UART）**控制 EXP6：不用換韌體、不用改硬體，出廠的 EXP6 就能直接用。

## 👉 請看新的 Arduino 範例

[🧪 EXP6 × Arduino 範例（序列埠）](/sensors/exp6/arduino-examples.md)：6 個可以直接複製上傳的範例，從「確認連上」到「兩輪小車」，每一行都有中文說明。

| 你想做的事 | 範例 |
| :--- | :--- |
| 確認連上，看 6 個埠接了什麼、電池電壓 | 範例 1 |
| 讓馬達轉、停 | 範例 2 |
| 讓馬達轉指定角度 | 範例 3 |
| 讀馬達角度 | 範例 4 |
| 讀顏色、超音波、力量感應器 | 範例 5 |
| 兩輪小車 | 範例 6 |

> [!NOTE]
> 序列埠要用到 Arduino 的第二組序列埠，建議用 **Arduino UNO R4**（外型跟 UNO 一樣）。Arduino Mega、ESP32 也可以；舊款 UNO R3／Nano 只有一組序列埠（給 USB 用了），不能接 EXP6。詳細接線請看範例頁。
