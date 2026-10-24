### 你需要

- **感應器**：{{product}}（{{edition}}，RJ12 接頭）。
- **主機**：LEGO MINDSTORMS EV3。
- **軟體**（二選一）：
  - **Pybricks v4**：用 Chrome 打開 [code.pybricks.com](https://code.pybricks.com)，EV3 要先裝 Pybricks 韌體（目前只有 **Beta 版**有 EV3 韌體）。
  - **EV3 MicroPython v2.0**：樂高官方「EV3 MicroPython」microSD 開機卡＋VS Code 的「LEGO MINDSTORMS EV3 MicroPython」擴充套件。已經在用的人選這個。

### 安裝軟體

**Pybricks v4**

1. 用 Chrome 打開 [code.pybricks.com](https://code.pybricks.com)。
2. 照網站說明把 EV3 換成 Pybricks 韌體。EV3 韌體目前只在 **Beta 測試版**提供，本站測試的版本是 **v4.0.0b10（Pybricks Beta v3.0.0-beta.7）**。
3. 打開範例：把 `.py` 檔拖進編輯區，按執行。

**EV3 MicroPython v2.0**

1. 用樂高官方 EV3 MicroPython 的 microSD 開機卡開機。
2. 在 VS Code 裝好「LEGO MINDSTORMS EV3 MicroPython」擴充套件。
3. 打開範例：把程式內容貼到 `main.py`，按 **F5** 下載執行。

### 讓軟體認得感應器

新手範例**不用裝函式庫**，直接讀感應器的資料：

- Pybricks v4 用 `LUMPDevice(Port.S1)`，`sensor.read(模式)` 拿到該模式的數值。
- EV3 MicroPython v2.0 用 `Ev3devSensor`。
- 可以直接讀 EV3-G、Classroom 讀不到的模式，所以標「僅 Pybricks／EV3 MicroPython／clev3r」的範例也能跑。

> [!IMPORTANT]
> **不要在主迴圈裡頻繁切換模式。** EV3 硬體每次切換感應器模式都會有通訊等待延遲。依任務選定一種模式，整趟都用它。

### 接線

1. 用連接線把感應器插在 EV3 的 **{{port}} 號孔**（1～4 任一孔都可以；程式裡 `Port.S1`～`Port.S4` 選同一個孔）。
2. **EV3 先開機，再插感應器。**
3. 接頭卡榫朝上，輕輕推進去。

### 第一次測試

1. 打開本頁範例 1，按執行（MicroPython 版按 F5）。
2. EV3 螢幕上的數字跟著感應器前方的東西變化，就是接好了。

### 常見問題

- **找不到 EV3 的 Pybricks 韌體？** EV3 韌體目前只在 Beta 版提供，正式版還沒有。
- **數字一直不變？** 拔掉感應器重插一次（EV3 要先開機再插）。
- **程式反應忽快忽慢？** 主迴圈裡交叉讀了不同模式，每次切換都要等，改成只用一種模式。
- **匯入函式庫時出錯？** 函式庫檔要匯入到同一個 Pybricks 專案，檔名必須跟 `import` 的名字完全一致。
