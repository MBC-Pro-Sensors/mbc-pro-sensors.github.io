# LEGO® MINDSTORMS® EV3 Classroom

[🔙 回到 EV3 主機選單](/sensors/line16/ev3-hub.md)

<div style="display: flex; align-items: center; justify-content: center; gap: 30px; margin: 30px 0; flex-wrap: wrap;">
  <div style="background: rgba(10,186,181,0.03); border: 1px solid rgba(10,186,181,0.25); border-radius: 12px; padding: 15px 35px; display: flex; align-items: center; gap: 20px; box-shadow: 0 10px 30px rgba(0,0,0,0.15);">
    <img src="../images/hubs/ev3-hub.webp" alt="EV3 Hub" style="max-height: 90px; object-fit: contain; filter: drop-shadow(0 0 15px rgba(10,186,181,0.35));" />
    <span style="font-size: 2.2rem; color: #666; font-weight: 300; line-height: 1;">+</span>
    <img src="../images/hubs/ev3-classroom-app.webp" alt="EV3 Classroom App" style="max-height: 90px; object-fit: contain; filter: drop-shadow(0 0 15px rgba(10,186,181,0.15));" />
  </div>
</div>

> [!WARNING] 
> **請注意：您目前觀看的是 【EV3 Classroom】 環境的教學。**
> 如果您使用的是 SPIKE 或是其他 EV3 軟體，請點擊上方按鈕回到主機選單重新選擇。

適用於 **LEGO® MINDSTORMS® EV3 Classroom App**（Scratch 積木風格）。在此環境下，循行者 16 路感應器會被樂高系統完美識別為官方的 **Infrared Sensor（紅外線感應器）**。

> [!IMPORTANT]
> **⚠️ EV3 硬體重要通訊延遲警告**
> 由於樂高原廠 EV3 主機的硬體通訊機制限制，當程式在不同積木模式（例如從 Proximity 切換至 Remote）進行切換讀取時，**主機會產生約 0.5 秒的物理通訊等待延遲**，這在高速比賽中可能導致車體失控。
> **強烈建議**：請依據您的任務需求（例如：只用 Remote 進行 PID 循線），**在程式一開始選定一種積木，整趟任務請勿中途切換讀取種類！**

> **📌 與官方 EV3 軟體的差異**
> Classroom App 的積木外觀是 Scratch 風格，但感應器讀取模式與官方 EV3 軟體完全一樣：Proximity / Beacon / Remote 三種模式，回傳數值範圍也完全相同。

---

## 讀取線位置 — Proximity 模式

<div style="text-align: center; margin: 15px 0;">
  <img src="../images/sensors/ev3-blocks/classroom/mode-proximity.webp" alt="EV3 Classroom Proximity 模式積木" style="max-height: 80px; border-radius: 8px; filter: drop-shadow(0 5px 15px rgba(10,186,181,0.2));" />
</div>

使用感應器積木，選擇 **Proximity（接近）模式**，讀取 `數值` 即可取得線位置。

<div class="custom-table-wrapper" style="overflow-x: auto; margin: 25px 0; border: 1px solid rgba(10,186,181,0.25); border-radius: 12px; box-shadow: 0 10px 30px rgba(0,0,0,0.3); background: #f8f9fa;">
  <table style="width: 100%; border-collapse: collapse; text-align: left; font-size: 0.95rem;">
    <thead>
      <tr style="background: rgba(10,186,181,0.12); border-bottom: 2px solid rgba(10,186,181,0.25);">
        <th style="padding: 15px 20px; color: #007a75; font-weight: bold; width: 25%;">回傳值</th>
        <th style="padding: 15px 20px; color: #007a75; font-weight: bold; width: 25%;">範圍</th>
        <th style="padding: 15px 20px; color: #007a75; font-weight: bold; width: 50%;">說明</th>
      </tr>
    </thead>
    <tbody>
      <tr class="table-hover-row" style="border: none; background: rgba(0,0,0,0.01); transition: background 0.2s ease;">
        <td style="padding: 18px 20px; font-weight: bold; color: #111; vertical-align: middle;">線位置</td>
        <td style="padding: 18px 20px; color: #444; vertical-align: middle;">
          <span style="display: inline-block; background: rgba(255,107,53,0.15); color: #d32f2f; padding: 2px 8px; border-radius: 4px; font-family: monospace; font-size: 0.85rem; border: 1px solid rgba(255,107,53,0.2);">-16 ~ +16</span>
        </td>
        <td style="padding: 18px 20px; color: #444; line-height: 1.6; vertical-align: middle;">
          👉 <strong><code>0</code> = 黑線在正中央，負數偏左，正數偏右</strong>；<code>±16</code> 表示線在感應器最邊緣。
        </td>
      </tr>
    </tbody>
  </table>
</div>

---

## 讀取 16 路光電反射值 — Beacon 模式

<div style="text-align: center; margin: 15px 0;">
  <img src="../images/sensors/ev3-blocks/classroom/mode-beacon.webp" alt="EV3 Classroom Beacon 模式積木" style="max-height: 80px; border-radius: 8px; filter: drop-shadow(0 5px 15px rgba(10,186,181,0.2));" />
</div>

使用感應器積木，選擇 **Beacon（信標）模式**，讀取 `數值列表`（8 bytes）。

<div class="custom-table-wrapper" style="overflow-x: auto; margin: 25px 0; border: 1px solid rgba(10,186,181,0.25); border-radius: 12px; box-shadow: 0 10px 30px rgba(0,0,0,0.3); background: #f8f9fa;">
  <table style="width: 100%; border-collapse: collapse; text-align: left; font-size: 0.95rem;">
    <thead>
      <tr style="background: rgba(10,186,181,0.12); border-bottom: 2px solid rgba(10,186,181,0.25);">
        <th style="padding: 15px 20px; color: #007a75; font-weight: bold; width: 15%;">Byte</th>
        <th style="padding: 15px 20px; color: #007a75; font-weight: bold; width: 25%;">內容</th>
        <th style="padding: 15px 20px; color: #007a75; font-weight: bold; width: 20%;">範圍</th>
        <th style="padding: 15px 20px; color: #007a75; font-weight: bold; width: 40%;">解碼方式</th>
      </tr>
    </thead>
    <tbody>
      <tr class="table-hover-row" style="border-bottom: 1px solid rgba(0,0,0,0.06); background: rgba(0,0,0,0.01); transition: background 0.2s ease;">
        <td style="padding: 18px 20px; font-weight: bold; color: #111; vertical-align: middle;">0</td>
        <td style="padding: 18px 20px; font-weight: bold; color: #333; vertical-align: middle;">光電反射值0 + 1</td>
        <td style="padding: 18px 20px; color: #444; vertical-align: middle;"><span style="display: inline-block; background: rgba(10,186,181,0.15); color: #007a75; padding: 2px 8px; border-radius: 4px; font-family: monospace; font-size: 0.85rem; border: 1px solid rgba(10,186,181,0.2);">11 ~ 99</span></td>
        <td style="padding: 18px 20px; color: #444; line-height: 1.6; vertical-align: middle;">十位數=通道0，個位數=通道1</td>
      </tr>
      <tr class="table-hover-row" style="border-bottom: 1px solid rgba(0,0,0,0.06); background: rgba(0,0,0,0.01); transition: background 0.2s ease;">
        <td style="padding: 18px 20px; font-weight: bold; color: #111; vertical-align: middle;">1</td>
        <td style="padding: 18px 20px; font-weight: bold; color: #333; vertical-align: middle;">光電反射值2 + 3</td>
        <td style="padding: 18px 20px; color: #444; vertical-align: middle;"><span style="display: inline-block; background: rgba(10,186,181,0.15); color: #007a75; padding: 2px 8px; border-radius: 4px; font-family: monospace; font-size: 0.85rem; border: 1px solid rgba(10,186,181,0.2);">11 ~ 99</span></td>
        <td style="padding: 18px 20px; color: #444; line-height: 1.6; vertical-align: middle;">十位數=通道2，個位數=通道3</td>
      </tr>
      <tr class="table-hover-row" style="border-bottom: 1px solid rgba(0,0,0,0.06); background: rgba(0,0,0,0.01); transition: background 0.2s ease;">
        <td style="padding: 18px 20px; font-weight: bold; color: #111; vertical-align: middle;">2</td>
        <td style="padding: 18px 20px; font-weight: bold; color: #333; vertical-align: middle;">光電反射值4 + 5</td>
        <td style="padding: 18px 20px; color: #444; vertical-align: middle;"><span style="display: inline-block; background: rgba(10,186,181,0.15); color: #007a75; padding: 2px 8px; border-radius: 4px; font-family: monospace; font-size: 0.85rem; border: 1px solid rgba(10,186,181,0.2);">11 ~ 99</span></td>
        <td style="padding: 18px 20px; color: #444; line-height: 1.6; vertical-align: middle;">十位數=通道4，個位數=通道5</td>
      </tr>
      <tr class="table-hover-row" style="border-bottom: 1px solid rgba(0,0,0,0.06); background: rgba(0,0,0,0.01); transition: background 0.2s ease;">
        <td style="padding: 18px 20px; font-weight: bold; color: #111; vertical-align: middle;">3</td>
        <td style="padding: 18px 20px; font-weight: bold; color: #333; vertical-align: middle;">光電反射值6 + 7</td>
        <td style="padding: 18px 20px; color: #444; vertical-align: middle;"><span style="display: inline-block; background: rgba(10,186,181,0.15); color: #007a75; padding: 2px 8px; border-radius: 4px; font-family: monospace; font-size: 0.85rem; border: 1px solid rgba(10,186,181,0.2);">11 ~ 99</span></td>
        <td style="padding: 18px 20px; color: #444; line-height: 1.6; vertical-align: middle;">十位數=通道6，個位數=通道7</td>
      </tr>
      <tr class="table-hover-row" style="border-bottom: 1px solid rgba(0,0,0,0.06); background: rgba(0,0,0,0.01); transition: background 0.2s ease;">
        <td style="padding: 18px 20px; font-weight: bold; color: #111; vertical-align: middle;">4</td>
        <td style="padding: 18px 20px; font-weight: bold; color: #333; vertical-align: middle;">光電反射值8 + 9</td>
        <td style="padding: 18px 20px; color: #444; vertical-align: middle;"><span style="display: inline-block; background: rgba(10,186,181,0.15); color: #007a75; padding: 2px 8px; border-radius: 4px; font-family: monospace; font-size: 0.85rem; border: 1px solid rgba(10,186,181,0.2);">11 ~ 99</span></td>
        <td style="padding: 18px 20px; color: #444; line-height: 1.6; vertical-align: middle;">十位數=通道8，個位數=通道9</td>
      </tr>
      <tr class="table-hover-row" style="border-bottom: 1px solid rgba(0,0,0,0.06); background: rgba(0,0,0,0.01); transition: background 0.2s ease;">
        <td style="padding: 18px 20px; font-weight: bold; color: #111; vertical-align: middle;">5</td>
        <td style="padding: 18px 20px; font-weight: bold; color: #333; vertical-align: middle;">光電反射值10 + 11</td>
        <td style="padding: 18px 20px; color: #444; vertical-align: middle;"><span style="display: inline-block; background: rgba(10,186,181,0.15); color: #007a75; padding: 2px 8px; border-radius: 4px; font-family: monospace; font-size: 0.85rem; border: 1px solid rgba(10,186,181,0.2);">11 ~ 99</span></td>
        <td style="padding: 18px 20px; color: #444; line-height: 1.6; vertical-align: middle;">十位數=通道10，個位數=通道11</td>
      </tr>
      <tr class="table-hover-row" style="border-bottom: 1px solid rgba(0,0,0,0.06); background: rgba(0,0,0,0.01); transition: background 0.2s ease;">
        <td style="padding: 18px 20px; font-weight: bold; color: #111; vertical-align: middle;">6</td>
        <td style="padding: 18px 20px; font-weight: bold; color: #333; vertical-align: middle;">光電反射值12 + 13</td>
        <td style="padding: 18px 20px; color: #444; vertical-align: middle;"><span style="display: inline-block; background: rgba(10,186,181,0.15); color: #007a75; padding: 2px 8px; border-radius: 4px; font-family: monospace; font-size: 0.85rem; border: 1px solid rgba(10,186,181,0.2);">11 ~ 99</span></td>
        <td style="padding: 18px 20px; color: #444; line-height: 1.6; vertical-align: middle;">十位數=通道12，個位數=通道13</td>
      </tr>
      <tr class="table-hover-row" style="border: none; background: rgba(0,0,0,0.01); transition: background 0.2s ease;">
        <td style="padding: 18px 20px; font-weight: bold; color: #111; vertical-align: middle;">7</td>
        <td style="padding: 18px 20px; font-weight: bold; color: #333; vertical-align: middle;">光電反射值14 + 15</td>
        <td style="padding: 18px 20px; color: #444; vertical-align: middle;"><span style="display: inline-block; background: rgba(10,186,181,0.15); color: #007a75; padding: 2px 8px; border-radius: 4px; font-family: monospace; font-size: 0.85rem; border: 1px solid rgba(10,186,181,0.2);">11 ~ 99</span></td>
        <td style="padding: 18px 20px; color: #444; line-height: 1.6; vertical-align: middle;">十位數=通道14，個位數=通道15</td>
      </tr>
    </tbody>
  </table>
</div>

> 每路光電反射值已被量化為 1 至 9（1=最暗，9=最亮），兩路合併為一個兩位數（11 至 99）。
> 👉 **解碼範例**：若讀取到某個 Byte 數值為 **`83`**，其**十位數 (8)** 代表偶數通道（如通道 0）的量化光值，**個位數 (3)** 代表奇數通道（如通道 1）的量化光值。這能完美避開樂高 EV3 的 100 數值限制！

---

## 讀取位置 + 寬度 + 邊界防禦 — Remote 模式 (競技戰術推薦！)

<div style="text-align: center; margin: 15px 0;">
  <img src="../images/sensors/ev3-blocks/classroom/mode-remote.webp" alt="EV3 Classroom Remote 模式積木" style="max-height: 80px; border-radius: 8px; filter: drop-shadow(0 5px 15px rgba(10,186,181,0.2));" />
</div>

使用感應器積木，選擇 **Remote（遙控）模式**，讀取 `數值列表`（4 bytes）。

<div class="custom-table-wrapper" style="overflow-x: auto; margin: 25px 0; border: 1px solid rgba(10,186,181,0.25); border-radius: 12px; box-shadow: 0 10px 30px rgba(0,0,0,0.3); background: #f8f9fa;">
  <table style="width: 100%; border-collapse: collapse; text-align: left; font-size: 0.95rem;">
    <thead>
      <tr style="background: rgba(10,186,181,0.12); border-bottom: 2px solid rgba(10,186,181,0.25);">
        <th style="padding: 15px 20px; color: #007a75; font-weight: bold; width: 15%;">Byte</th>
        <th style="padding: 15px 20px; color: #007a75; font-weight: bold; width: 25%;">內容數據</th>
        <th style="padding: 15px 20px; color: #007a75; font-weight: bold; width: 20%;">數值範圍</th>
        <th style="padding: 15px 20px; color: #007a75; font-weight: bold; width: 40%;">戰術解讀說明</th>
      </tr>
    </thead>
    <tbody>
      <tr class="table-hover-row" style="border-bottom: 1px solid rgba(0,0,0,0.06); background: rgba(0,0,0,0.01); transition: background 0.2s ease;">
        <td style="padding: 18px 20px; font-weight: bold; color: #111; vertical-align: middle;">Byte 0</td>
        <td style="padding: 18px 20px; font-weight: bold; color: #333; vertical-align: middle;">高解析位置平移</td>
        <td style="padding: 18px 20px; color: #444; vertical-align: middle;">
          <span style="display: inline-block; background: rgba(255,107,53,0.15); color: #d32f2f; padding: 2px 8px; border-radius: 4px; font-family: monospace; font-size: 0.85rem; border: 1px solid rgba(255,107,53,0.2);">-100 ~ +100</span>
        </td>
        <td style="padding: 18px 20px; color: #444; line-height: 1.6; vertical-align: middle;">
          👉 <strong><code>0</code> 代表正中央，負值偏左，正值偏右</strong>。超平滑的 PID 誤差輸入首選！
        </td>
      </tr>
      <tr class="table-hover-row" style="border-bottom: 1px solid rgba(0,0,0,0.06); background: rgba(0,0,0,0.01); transition: background 0.2s ease;">
        <td style="padding: 18px 20px; font-weight: bold; color: #111; vertical-align: middle;">Byte 1</td>
        <td style="padding: 18px 20px; font-weight: bold; color: #333; vertical-align: middle;">特徵線寬</td>
        <td style="padding: 18px 20px; color: #444; vertical-align: middle;">
          <span style="display: inline-block; background: rgba(10,186,181,0.15); color: #007a75; padding: 2px 8px; border-radius: 4px; font-family: monospace; font-size: 0.85rem; border: 1px solid rgba(10,186,181,0.2);">0 ~ 16</span>
        </td>
        <td style="padding: 18px 20px; color: #444; line-height: 1.6; vertical-align: middle;">
          👉 觸發的感測器數量。可用於偵測十字路口或 T 字路口。
        </td>
      </tr>
      <tr class="table-hover-row" style="border-bottom: 1px solid rgba(0,0,0,0.06); background: rgba(0,0,0,0.01); transition: background 0.2s ease;">
        <td style="padding: 18px 20px; font-weight: bold; color: #111; vertical-align: middle;">Byte 2</td>
        <td style="padding: 18px 20px; font-weight: bold; color: #333; vertical-align: middle;">左邊界實體光值</td>
        <td style="padding: 18px 20px; color: #444; vertical-align: middle;">
          <span style="display: inline-block; background: rgba(217,163,0,0.15); color: #b88600; padding: 2px 8px; border-radius: 4px; font-family: monospace; font-size: 0.85rem; border: 1px solid rgba(217,163,0,0.2);">0 ~ 100</span>
        </td>
        <td style="padding: 18px 20px; color: #444; line-height: 1.6; vertical-align: middle;">
          👉 <code>dataIrCalib[15]</code> 最左邊感測器的反射光。<code>0</code>=純黑，<code>100</code>=純白。可用於偵測賽道邊界或分岔。
        </td>
      </tr>
      <tr class="table-hover-row" style="border: none; background: rgba(0,0,0,0.01); transition: background 0.2s ease;">
        <td style="padding: 18px 20px; font-weight: bold; color: #111; vertical-align: middle;">Byte 3</td>
        <td style="padding: 18px 20px; font-weight: bold; color: #333; vertical-align: middle;">右邊界實體光值</td>
        <td style="padding: 18px 20px; color: #444; vertical-align: middle;">
          <span style="display: inline-block; background: rgba(217,163,0,0.15); color: #b88600; padding: 2px 8px; border-radius: 4px; font-family: monospace; font-size: 0.85rem; border: 1px solid rgba(217,163,0,0.2);">0 ~ 100</span>
        </td>
        <td style="padding: 18px 20px; color: #444; line-height: 1.6; vertical-align: middle;">
          👉 <code>dataIrCalib[0]</code> 最右邊感測器的反射光。<code>0</code>=純黑，<code>100</code>=純白。可用於偵測賽道邊界或分岔。
        </td>
      </tr>
    </tbody>
  </table>
</div>

> 💡 **戰術防呆技巧**：Remote 模式的 Byte 2 與 Byte 3 能讓您在 Classroom 積木中直接獲得感測器兩側的最邊緣光值。如果兩個數值同時小於 20，表示車頭完全進入十字路口的橫線中；如果其中一邊數值突變，表示即將進入分岔，是防掉線的終極法寶！





<style>
  .table-hover-row:hover {
    background: rgba(0,0,0,0.04) !important;
  }
</style>

---

## <a href="../examples/line16/ev3/line16_ev3_classroom.lmsp" target="_blank" data-ignore="true" download>📥 範例程式下載</a>

<div style="text-align: center; margin: 25px 0;">
  <img src="../images/sensors/line16/ev3-classroom-example.webp" alt="LEGO® MINDSTORMS® EV3 Classroom 範例程式圖控積木範例" style="max-width: 100%; border-radius: 8px; border: 1px solid rgba(10,186,181,0.25); box-shadow: 0 10px 30px rgba(0,0,0,0.5); filter: drop-shadow(0 0 15px rgba(10,186,181,0.15));" />
  <p style="font-size: 0.85rem; color: #888; margin-top: 10px;">💡 上圖為 16路循線感應器於 LEGO® MINDSTORMS® EV3 Classroom 中的經典 PID 循線程式結構示範</p>
</div>

* <a href="../examples/line16/ev3/line16_ev3_classroom.lmsp" target="_blank" download="line16_ev3_classroom.lmsp" style="color: #0abab5; font-weight: bold; text-decoration: none;">📥 Classroom 範例 (.lmsp)</a>

> 下載後用 Classroom App 開啟即可使用。

---

## 🧩 EV3 專屬自訂積木 (My Blocks) 說明

為了讓 EV3 Classroom 的程式撰寫更為簡潔直覺，MBC 16 路循線感應器提供了三個強大的「自訂積木（My Blocks）」。開發時不需要額外安裝繁瑣的擴充包，只需在主程式中呼叫對應的函數，系統就會自動在背景解析硬體封包，並將最新的感應器數據存入專屬變數中，供馬達控制邏輯直接使用。

以下為三個核心函數的功能與使用時機介紹：

### 1. 基礎循線函數：`LinePos16_dataUpdate`
* **功能目的**：快速讀取最基礎的 16 段循線位置，適合新手入門教學與基礎左右轉向判斷。
* **更新變數**：執行此函數後，系統會自動更新變數 `0_LinePos16`。
* **數值解析**：範圍落在 `-16` 到 `+16` 之間。
* **物理直覺**：黑線在左邊為負值、在正中央為 0、在右邊為正值。

### 2. 競技高精度函數：`LinePos100_dataUpdate`
* **功能目的**：取得高解析度次像素級插值位置，最佳 PID 循線選擇。專為高階 PID / PD 循線控制演算法（適用於各類競速賽事）所設計，能提供極致平滑的修正動力。**使用前提是必須確實完成感應器校準，校準越精確數值越準確。**
* **更新變數**：執行此函數後，會同步更新多個關鍵變數：
  * `0_LinePos100`：高精度位置，範圍 `-100` 到 `+100`（同樣維持左負右正的標準）。
  * `0_LineWidth16`：即時線寬偵測，數值代表目前觸發的感應器數量（範圍 `0` 到 `16`）。可用於精準判斷十字路口、T 型路口或終點停止線。
  * **最左 / 最右邊界數值**：同步讀取最左邊與最右邊感應器的實體光值，方便使用者作防掉線或交叉路口等進階策略調整。

### 3. 全陣列展開函數：`LineCh16_dataUpdate`
* **功能目的**：將 16 顆感應器徹底解碼為獨立狀態，適用於撰寫自訂的特殊路口判斷演算法、特定位置的感應器觸發監控，或是將 16 顆感應器的狀態直接繪製於 EV3 主機的螢幕畫面上。
* **更新變數**：執行此函數後，會一氣呵成地更新 `s1` 到 `s16` 共 16 個獨立變數，完美對應感應器由左至右每一顆的即時讀數。

> [!TIP]
> **💡 開發小提醒**
> 這三個函數的底層技術，是巧妙利用了 EV3 內建紅外線感應器（設定於 Port 4）的多頻道切換（Channel 1~4）來傳輸大量資料。實際開發時，**只要將這些自訂積木放入「重複無限次」迴圈中**，即可確保循線車隨時掌握最即時的賽道資訊！

<br>

!!! success "🚀 想要學更厲害的循線控制方法嗎？"
    可以找這幾位厲害的教練上課唷！他們有非常豐富的比賽與教學經驗，保證讓你收穫滿滿～ 💯

<div style="display: flex; flex-wrap: wrap; gap: 20px; justify-content: center; margin-top: 15px; margin-bottom: 30px;">
  <div style="flex: 1; min-width: 250px; max-width: 320px; display: flex; flex-direction: column; align-items: center;">
    <h4 style="margin: 0 0 10px 0; text-align: center;">🏆 <a href="https://www.youtube.com/@LegoLauXiao" target="_blank" style="color: inherit; text-decoration: none;">LegoLauXiao 教練</a></h4>
    <div style="width: 100%; aspect-ratio: 9/16; background: #000; border-radius: 12px; overflow: hidden; border: 1px solid rgba(0,210,255,0.2); box-shadow: 0 10px 25px rgba(0,0,0,0.3);">
      <iframe src="https://www.youtube.com/embed/WgacdWLatbk" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" allowfullscreen loading="lazy" style="width: 100%; height: 100%; border: none;"></iframe>
    </div>
  </div>
  <div style="flex: 1; min-width: 250px; max-width: 320px; display: flex; flex-direction: column; align-items: center;">
    <h4 style="margin: 0 0 10px 0; text-align: center;">🏆 <a href="https://www.youtube.com/@legolaumo" target="_blank" style="color: inherit; text-decoration: none;">legolaumo 教練</a></h4>
    <div style="width: 100%; aspect-ratio: 9/16; background: #000; border-radius: 12px; overflow: hidden; border: 1px solid rgba(0,210,255,0.2); box-shadow: 0 10px 25px rgba(0,0,0,0.3);">
      <iframe src="https://www.youtube.com/embed/T9bcndBNQvQ" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" allowfullscreen loading="lazy" style="width: 100%; height: 100%; border: none;"></iframe>
    </div>
  </div>
</div>

