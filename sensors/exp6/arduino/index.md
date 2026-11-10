<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 2026-11-06：EXP6 的 routes.arduino 拿掉（使用者：格子拿掉、頁面留著），gen-guides 不再產生這頁；改成手寫的開發中說明頁 -->
<!-- noindex -->
# MBC EXP6 六路馬達擴充器（適用 SPIKE™） × Arduino（UART）

> [!WARNING]
> **EXP6 × Arduino（UART）開發與驗證中，尚未支援。** 這頁先保留給參考，請先用 [SPIKE Prime 或 Robot Inventor](/sensors/exp6/index.md) 主機。
<!-- exp6-arduino-dev -->

EXP6 **不能用 I2C** 接 Arduino（主機接頭的隔離晶片只能單向傳送），開發中的做法是改用 **序列埠（UART）**。

- 開發中的範例與接線（參考用）：[EXP6 × Arduino 範例](/sensors/exp6/arduino-examples.md)
- 開發中的互動教學（參考用）：[EXP6 × Arduino 互動教學](/learn/exp6/arduino.html)
- 現在能用的寫法：[SPIKE Prime／Robot Inventor](/sensors/exp6/spike/index.md)

想知道進度？到 [聯絡我們](/contact.md)。
