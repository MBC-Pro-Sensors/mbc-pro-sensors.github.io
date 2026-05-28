# ∿ 通用 I2C 版本開發指南

[🔙 回到 8路循線器首頁](/sensors/line8/index.md)

<div style="display: flex; align-items: center; justify-content: center; gap: 30px; margin: 30px 0; flex-wrap: wrap;">
  <div style="background: rgba(10,186,181,0.03); border: 1px solid rgba(10,186,181,0.25); border-radius: 12px; padding: 25px 45px; display: flex; align-items: center; gap: 25px; box-shadow: 0 10px 30px rgba(0,0,0,0.15);">
    <img src="../images/sensors/line8/line8-product-arduino.webp" alt="循行者8路感應器 (通用 I2C 版)" style="max-height: 144px; object-fit: contain; filter: drop-shadow(0 0 20px rgba(10,186,181,0.75)) drop-shadow(0 0 45px rgba(10,186,181,0.45));" />
    <span style="font-size: 2.5rem; color: #777; font-weight: 300; line-height: 1;">+</span>
    <div style="display: flex; gap: 15px; align-items: center;">
      <svg viewBox="0 0 100 40" style="height: 60px; fill: none; stroke: #00d2ff; stroke-width: 4; stroke-linecap: round; stroke-linejoin: round; filter: drop-shadow(0 0 10px rgba(0, 210, 255, 0.4));">
        <path d="M 5,25 H 25 V 5 H 45 V 35 H 65 V 5 H 85 V 35 H 95" />
      </svg>
    </div>
  </div>
</div>

歡迎來到專業硬體開發專區！MBC 8 路循線感應器除了完美支援樂高生態系，也為 **Arduino、ESP32、樹莓派以及各式自主開發主控板** 開發者提供了開放式的標準 I2C 通訊介面。
本感應器讀寫暫存器完全分離，採無衝突設計，非常適合用於開發高速循線機器人或上位機視覺化儀表板。

---

## 1. 通用 MCU 平台 (Arduino / ESP32 / 樹莓派)

*   **通訊協定**：標準 I2C
*   **預設從機位址 (Slave Address)**：`0x08`

### 📥 寫入暫存器 (I2C Write - 控制指令)
寫入格式為：`[從機位址 0x08] + [暫存器位址] + [設定值]`

<div style="display: grid; grid-template-columns: 1fr; gap: 15px; margin-bottom: 30px;">
<div class="reg-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(255,107,53,0.15); border-radius: 10px; padding: 16px; transition: all 0.3s ease;">
<div style="display: flex; align-items: center; justify-content: space-between; margin-bottom: 10px;">
<code style="color: #ff6b35; font-size: 1.1rem; font-weight: bold;">0x10</code>
<span style="background: rgba(255,107,53,0.15); color: #ff6b35; padding: 3px 10px; border-radius: 20px; font-weight: bold; font-size: 0.85rem;">切換目標線模式</span>
</div>
<ul style="margin: 0; padding-left: 20px; font-size: 0.85rem; color: #aaa; line-height: 1.6;">
<li>寫入 <strong><code>0</code></strong>：循黑線模式。</li>
<li>寫入 <strong><code>1</code></strong>：循白線模式。</li>
</ul>
</div>
<div class="reg-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(255,107,53,0.15); border-radius: 10px; padding: 16px; transition: all 0.3s ease;">
<div style="display: flex; align-items: center; justify-content: space-between; margin-bottom: 10px;">
<code style="color: #ff6b35; font-size: 1.1rem; font-weight: bold;">0x20</code>
<span style="background: rgba(255,107,53,0.15); color: #ff6b35; padding: 3px 10px; border-radius: 20px; font-weight: bold; font-size: 0.85rem;">觸發校準指令</span>
</div>
<div style="font-size: 0.85rem; color: #aaa; line-height: 1.6;">
<div style="color: #0abab5; font-weight: bold; margin-bottom: 4px;">⚡ 秒殺校準法（免移動車體）：</div>
<ul style="margin: 0 0 8px 0; padding-left: 20px;">
<li>寫入 <strong><code>1</code></strong>：啟動 5 秒動態校準（需在賽道上移動車體）。</li>
<li>寫入 <strong><code>2</code></strong>：觸發「靜態單步白色校準」👉 將當下畫面記為白色極值（車體需停在白地上）。</li>
<li>寫入 <strong><code>3</code></strong>：觸發「靜態單步黑色校準」👉 將當下畫面記為黑色極值（車體需壓在黑線上）。</li>
</ul>
<div style="font-size: 0.75rem; color: #888;">*(註：寫入 2 與 3 會觸發底層 EEPROM 非同步寫入與增益重算)*</div>
</div>
</div>
<div class="reg-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(255,107,53,0.15); border-radius: 10px; padding: 16px; transition: all 0.3s ease;">
<div style="display: flex; align-items: center; justify-content: space-between; margin-bottom: 10px;">
<code style="color: #ff6b35; font-size: 1.1rem; font-weight: bold;">0x30</code>
<span style="background: rgba(255,107,53,0.15); color: #ff6b35; padding: 3px 10px; border-radius: 20px; font-weight: bold; font-size: 0.85rem;">動態閾值 (LSA Threshold)</span>
</div>
<p style="margin: 0; font-size: 0.85rem; color: #aaa; line-height: 1.6;">寫入 <strong><code>0 ~ 100</code></strong> (預設為 <code>50</code>)。數值越高越容易判定為黑線，可於網頁端控制面板即時調校。</p>
</div>
</div>

### 📤 讀取暫存器 (I2C Read - 獲取數據)
讀取格式為：先 I2C 寫入欲讀取的 `[暫存器位址]` (不發送 Stop)，接著執行 `I2C RequestFrom` 對應的位元組長度。

<div style="display: grid; grid-template-columns: 1fr; gap: 15px; margin-bottom: 30px;">
<div class="reg-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(0,210,255,0.15); border-radius: 10px; padding: 16px; transition: all 0.3s ease;">
<div style="display: flex; align-items: center; justify-content: space-between; margin-bottom: 10px;">
<div style="display: flex; align-items: center; gap: 10px;">
<code style="color: #00d2ff; font-size: 1.1rem; font-weight: bold;">0x01</code>
<span style="background: rgba(0,210,255,0.15); color: #00d2ff; padding: 3px 10px; border-radius: 20px; font-weight: bold; font-size: 0.85rem;">核心綜合特徵包</span>
</div>
<span style="background: rgba(255,255,255,0.1); color: #ccc; padding: 2px 8px; border-radius: 4px; font-family: monospace; font-size: 0.75rem;">4 Bytes</span>
</div>
<ul style="margin: 0; padding-left: 20px; font-size: 0.85rem; color: #aaa; line-height: 1.6;">
<li><strong>Byte 0</strong>：標準位置平移值 (<code>linePos + 8</code>)，範圍 <code>0 ~ 16</code>。 (8 為正中央)</li>
<li><strong>Byte 1</strong>：特徵線寬 (<code>lineWidth</code>)，範圍 <code>0 ~ 8</code>。</li>
<li><strong>Byte 2</strong>：高解析平滑位置 (<code>linePosHighResolution</code>)，範圍 <code>0 ~ 200</code> (100 為中央)。</li>
<li><strong>Byte 3</strong>：二值化原圖狀態 (<code>binRaw</code> 完整 8 位元)。</li>
</ul>
</div>
<div class="reg-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(0,210,255,0.15); border-radius: 10px; padding: 16px; transition: all 0.3s ease;">
<div style="display: flex; align-items: center; justify-content: space-between; margin-bottom: 10px;">
<div style="display: flex; align-items: center; gap: 10px;">
<code style="color: #00d2ff; font-size: 1.1rem; font-weight: bold;">0x02</code>
<span style="background: rgba(0,210,255,0.15); color: #00d2ff; padding: 3px 10px; border-radius: 20px; font-weight: bold; font-size: 0.85rem;">全景絕對物理光值</span>
</div>
<span style="background: rgba(255,255,255,0.1); color: #ccc; padding: 2px 8px; border-radius: 4px; font-family: monospace; font-size: 0.75rem;">8 Bytes</span>
</div>
<div style="font-size: 0.85rem; color: #aaa; line-height: 1.6;">
一次讀回 8 組感測器精準校準後的物理光值 (<code>dataIrCalib[0~7]</code>)。<br>
每通道範圍 <code>0 ~ 100</code>（100=純白，0=純黑）。<br>
<span style="color: #0abab5; font-weight: bold;">極度推薦用於開發即時長條圖儀表板！</span>
</div>
</div>
<div class="reg-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(0,210,255,0.15); border-radius: 10px; padding: 16px; transition: all 0.3s ease;">
<div style="display: flex; align-items: center; justify-content: space-between; margin-bottom: 10px;">
<div style="display: flex; align-items: center; gap: 10px;">
<code style="color: #00d2ff; font-size: 1.1rem; font-weight: bold;">0x40 ~ 0x47</code>
<span style="background: rgba(0,210,255,0.15); color: #00d2ff; padding: 3px 10px; border-radius: 20px; font-weight: bold; font-size: 0.85rem;">單通道快速讀取</span>
</div>
<span style="background: rgba(255,255,255,0.1); color: #ccc; padding: 2px 8px; border-radius: 4px; font-family: monospace; font-size: 0.75rem;">1 Byte</span>
</div>
<p style="margin: 0; font-size: 0.85rem; color: #aaa; line-height: 1.6;">讀取指定單一通道的光值 (<code>0 ~ 100</code>)。<code>0x40</code> 對應通道 0 (最右)，<code>0x47</code> 對應通道 7 (最左)。</p>
</div>
</div>

<style>
.reg-card:hover {
  transform: translateY(-2px);
  box-shadow: 0 4px 15px rgba(0, 0, 0, 0.3);
  background: rgba(255,255,255,0.04) !important;
}
</style>

### 💻 Arduino C++ 讀取範例

> [!NOTE]
> **⏳ 完整程式範例準備中**
> 搭配 `ARDUINO_IIC` 專屬雙向控制暫存器系統的 Arduino C++ 讀寫範例程式與相關接線圖片，將於之後補全，敬請期待！
