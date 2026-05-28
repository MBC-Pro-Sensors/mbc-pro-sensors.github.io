# pyBricks for EV3

[🔙 回到 EV3 主機選單](/sensors/line16/ev3-hub.md)

<div style="display: flex; align-items: center; justify-content: center; gap: 30px; margin: 30px 0; flex-wrap: wrap;">
  <div style="background: rgba(0,210,255,0.03); border: 1px solid rgba(0,210,255,0.25); border-radius: 12px; padding: 15px 35px; display: flex; align-items: center; gap: 20px; box-shadow: 0 10px 30px rgba(0,0,0,0.15);">
    <img src="../images/hubs/ev3-hub.webp" alt="EV3 Hub" style="max-height: 90px; object-fit: contain; filter: drop-shadow(0 0 15px rgba(0,210,255,0.35));" />
    <span style="font-size: 2.2rem; color: #666; font-weight: 300; line-height: 1;">+</span>
    <img src="../images/hubs/ev3-pybricks-logo.webp" alt="EV3 pyBricks" style="max-height: 90px; object-fit: contain; filter: drop-shadow(0 0 15px rgba(0,210,255,0.25));" />
  </div>
</div>

> [!WARNING] 
> **請注意：您目前觀看的是 【EV3 pyBricks】 環境的教學。**
> 如果您使用的是 SPIKE 或是其他 EV3 軟體，請點擊上方按鈕回到主機選單重新選擇。

適用於在 **LEGO® MINDSTORMS® EV3** 主機上安裝 pyBricks 第三方韌體的使用者。在高階的 Pybricks MicroPython 環境下，玩家可以完全繞過官方的通訊與模式限制，以最精簡、高效的方式與 16 路循線感應器進行高速通訊！

> [!WARNING]
> **🧪 測試版本注意事項**
> 我們測試的 pyBricks 軟體版本號為 **v4.0.0b10 (Pybricks Beta v3.0.0-beta.7)**。請特別注意與小心：目前只有在 **Beta 測試版本** 才有提供 EV3 的韌體可供安裝使用！

> [!IMPORTANT]
> **⚠️ 模式切換等待延遲警告**
> 由於 EV3 硬體底層的通訊機制限制，當程式在不同底層讀取模式之間進行切換時，**仍然會產生通訊等待延遲**。
> **強烈建議**：為了確保程式穩定性與高速反應，請根據任務需求選定一種模式使用，**不要在主迴圈中頻繁交叉呼叫不同模式的函數**（下方函數已為您標註所屬模式群組）。

---

## 🚀 讀取方式與核心模式

在 EV3 Pybricks 環境中，本循線感應器提供兩種靈活的資料讀取管道，適應不同的專案需求：

1. **原生指令讀取 (`device.mode`)**：直接切換感應器的底層 UART 模式來獲取陣列數據，操作最為直接。
2. **導入專屬庫函數 (Library Functions)**：透過導入我們封裝好的 Python 函數，能獲得如同積木般直覺的獨立變數回傳，大幅簡化程式碼邏輯。

---

## <a href="../examples/line16/line16_pybricks_ev3.zip" target="_blank" data-ignore="true" download>📥 範例程式與函數下載 (打包下載)</a>

我們為您提供了核心範例程式以及專屬庫函數。請依照您的開發習慣選擇：

<!-- ========== 方式一：原生指令讀取 ========== -->
<div class="category-section" style="margin: 30px 0 40px 0;">
  <div class="category-header" style="display: flex; align-items: center; gap: 12px; margin-bottom: 20px; padding: 14px 22px; background: linear-gradient(135deg, rgba(0,210,255,0.08), rgba(0,210,255,0.02)); border: 1px solid rgba(0,210,255,0.25); border-radius: 10px;">
    <span style="font-size: 1.6rem;">⚡</span>
    <div>
      <h3 style="margin: 0; color: #00d2ff; font-size: 1.2rem; font-weight: bold;">方式一：原生指令讀取</h3>
      <p style="margin: 4px 0 0 0; font-size: 0.82rem; color: #999; line-height: 1.4;">使用 Pybricks 底層的 <code>hub.port.X.device.mode(mode)</code> 直接獲取陣列數據。</p>
    </div>
  </div>
  <div style="margin-bottom: 25px;">
    <div style="display: grid; grid-template-columns: repeat(auto-fit, minmax(240px, 1fr)); gap: 12px;">
      <div class="func-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(0,210,255,0.15); border-radius: 10px; padding: 16px; transition: all 0.3s ease;">
        <div style="display: flex; align-items: center; justify-content: space-between; margin-bottom: 10px;">
          <code style="color: #00d2ff; font-size: 0.95rem; font-weight: bold;">device.mode(0)</code>
          <span style="background: rgba(255,107,53,0.15); color: #ff6b35; padding: 3px 10px; border-radius: 20px; font-family: monospace; font-size: 0.75rem; white-space: nowrap;">Tuple (1 Byte)</span>
        </div>
        <p style="margin: 0; font-size: 0.82rem; color: #aaa; line-height: 1.5;"><strong>新手模式</strong>：回傳標準位置平移值 <code>[-16, 16]</code>，0 為中央。</p>
      </div>
      <div class="func-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(0,210,255,0.15); border-radius: 10px; padding: 16px; transition: all 0.3s ease;">
        <div style="display: flex; align-items: center; justify-content: space-between; margin-bottom: 10px;">
          <code style="color: #00d2ff; font-size: 0.95rem; font-weight: bold;">device.mode(1)</code>
          <span style="background: rgba(217,163,0,0.15); color: #d9a300; padding: 3px 10px; border-radius: 20px; font-family: monospace; font-size: 0.75rem; white-space: nowrap;">Tuple (8 Bytes)</span>
        </div>
        <p style="margin: 0; font-size: 0.82rem; color: #aaa; line-height: 1.5;"><strong>全覽除錯模式</strong>：回傳 16 路壓縮光值。相鄰兩路壓縮為一個兩位數（<code>11~99</code>）。</p>
      </div>
      <div class="func-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(0,210,255,0.15); border-radius: 10px; padding: 16px; transition: all 0.3s ease;">
        <div style="display: flex; align-items: center; justify-content: space-between; margin-bottom: 10px;">
          <code style="color: #00d2ff; font-size: 0.95rem; font-weight: bold;">device.mode(2)</code>
          <span style="background: rgba(10,186,181,0.15); color: #0abab5; padding: 3px 10px; border-radius: 20px; font-family: monospace; font-size: 0.75rem; white-space: nowrap;">Tuple (4 Bytes)</span>
        </div>
        <p style="margin: 0; font-size: 0.82rem; color: #aaa; line-height: 1.5;"><strong>競技戰術專武</strong>：回傳高精度位置 <code>[-100, 100]</code>、線寬 <code>[0, 16]</code>、左邊界實體光值與右邊界實體光值。</p>
      </div>
    </div>
  </div>
  <div class="pybricks-download-grid" style="display: grid; grid-template-columns: 1fr; gap: 25px;">
    <div style="padding: 40px 20px; text-align: center; background: rgba(255,255,255,0.02); border: 1px dashed rgba(0,210,255,0.3); border-radius: 12px;">
      <span style="font-size: 2.5rem; display: block; margin-bottom: 15px; opacity: 0.8;">⏳</span>
      <h4 style="color: #00d2ff; margin: 0 0 10px 0; font-size: 1.15rem; font-weight: bold;">等待 pyBricks 官方指令更新</h4>
      <p style="color: #aaa; font-size: 0.9rem; margin: 0; line-height: 1.6;">由於官方尚未將對應的圖形與底層指令製作更新，本區塊原生範例程式將於日後補齊。<br/>目前強烈建議您直接使用下方的<strong>「方式二：導入專屬庫函數」</strong>進行高階開發！</p>
    </div>
  </div>
</div>

<!-- ========== 方式二：導入專屬庫函數 ========== -->
<div class="category-section" style="margin: 40px 0 30px 0;">
  <div class="category-header" style="display: flex; align-items: center; gap: 12px; margin-bottom: 20px; padding: 14px 22px; background: linear-gradient(135deg, rgba(10,186,181,0.08), rgba(10,186,181,0.02)); border: 1px solid rgba(10,186,181,0.25); border-radius: 10px;">
    <span style="font-size: 1.6rem;">🔬</span>
    <div>
      <h3 style="margin: 0; color: #0abab5; font-size: 1.2rem; font-weight: bold;">方式二：導入專屬庫函數</h3>
      <p style="margin: 4px 0 0 0; font-size: 0.82rem; color: #999; line-height: 1.4;">透過導入封裝好的 Python 副程式，能獲得獨立變數回傳，大幅降低程式碼複雜度。</p>
    </div>
  </div>
  <div style="margin-bottom: 25px;">
    <div style="display: flex; align-items: center; gap: 8px; margin-bottom: 15px;">
      <h4 style="margin: 0; color: #0abab5; font-size: 1rem;">📋 可用函數一覽</h4>
      <span style="font-size: 0.75rem; color: #888;">— 由 <code style="background: rgba(10,186,181,0.1); padding: 1px 5px; border-radius: 3px; font-size: 0.75rem;">MBC_line16_EV3_Lib.py</code> 提供</span>
    </div>
    <!-- ⚙️ 初始化 -->
    <div style="margin-bottom: 12px;">
      <div style="font-size: 0.82rem; color: #0abab5; font-weight: bold; margin-bottom: 8px; padding-left: 2px;">⚙️ 初始化</div>
      <div style="display: grid; grid-template-columns: 1fr; gap: 12px;">
        <div class="func-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(10,186,181,0.15); border-radius: 10px; padding: 16px; transition: all 0.3s ease;">
          <div style="display: flex; align-items: center; justify-content: space-between; margin-bottom: 10px; flex-wrap: wrap; gap: 6px;">
            <code style="color: #0abab5; font-size: 0.95rem; font-weight: bold;">line_init_port_multitask(port, multitask)</code>
          </div>
          <p style="margin: 0; font-size: 0.82rem; color: #aaa; line-height: 1.5; margin-bottom: 15px;">初始化感應器連接埠。<code style="background: rgba(10,186,181,0.1); padding: 1px 4px; border-radius: 3px; font-size: 0.78rem;">port</code> 傳入 EV3 埠物件（例如 <code>hub.port.S1</code>）。<br>
          👉 <strong>非多工模式</strong>：第二參數 <code>multitask</code> 請填入 <code>False</code>，呼叫函數時前方條件請選擇 <strong><code>call</code></strong>。<br>
          👉 <strong>多工模式</strong>：第二參數 <code>multitask</code> 請填入 <code>True</code>，呼叫函數時前方條件請選擇 <strong><code>await</code></strong>。</p>
          
          <div style="display: grid; grid-template-columns: repeat(auto-fit, minmax(280px, 1fr)); gap: 15px;">
            <!-- 非多工狀態 -->
            <div style="background: rgba(0,0,0,0.2); border: 1px solid rgba(255,255,255,0.1); border-radius: 8px; padding: 12px; text-align: center;">
              <div style="color: #ccc; font-size: 0.85rem; font-weight: bold; margin-bottom: 10px; display: flex; align-items: center; justify-content: center; gap: 6px;">
                <span style="display: inline-block; width: 8px; height: 8px; background: #ff6b35; border-radius: 50%;"></span>
                非多工狀態設定 (常規使用)
              </div>
              <img src="../images/sensors/line16/ev3-pybricks-non-multitask.webp" alt="非多工狀態設定" style="width: 100%; border-radius: 6px; border: 1px solid rgba(255,107,53,0.2);" />
            </div>
            
            <!-- 多工狀態 -->
            <div style="background: rgba(0,0,0,0.2); border: 1px solid rgba(255,255,255,0.1); border-radius: 8px; padding: 12px; text-align: center;">
              <div style="color: #ccc; font-size: 0.85rem; font-weight: bold; margin-bottom: 10px; display: flex; align-items: center; justify-content: center; gap: 6px;">
                <span style="display: inline-block; width: 8px; height: 8px; background: #0abab5; border-radius: 50%;"></span>
                多工狀態設定 (非同步 / await)
              </div>
              <img src="../images/sensors/line16/ev3-pybricks-multitask.webp" alt="多工狀態設定" style="width: 100%; border-radius: 6px; border: 1px solid rgba(10,186,181,0.2);" />
            </div>
          </div>
        </div>
      </div>
    </div>
  <!-- 🟢 模式 0 -->
  <div style="margin-bottom: 12px;">
    <div style="font-size: 0.82rem; color: #0abab5; font-weight: bold; margin-bottom: 8px; padding-left: 2px;">🟢 模式 0 (新手/基礎循線)</div>
    <div style="display: grid; grid-template-columns: 1fr; gap: 12px;">
      <div class="func-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(10,186,181,0.15); border-radius: 10px; padding: 16px; transition: all 0.3s ease;">
        <div style="display: flex; align-items: center; justify-content: space-between; margin-bottom: 10px;">
          <code style="color: #0abab5; font-size: 0.95rem; font-weight: bold;"><span style="background: rgba(255,255,255,0.15); color: #fff; padding: 2px 6px; border-radius: 4px; font-size: 0.75rem; margin-right: 6px;">[模式 0: 基礎]</span>line_pos16()</code>
          <span style="background: rgba(255,107,53,0.15); color: #ff6b35; padding: 3px 10px; border-radius: 20px; font-family: monospace; font-size: 0.75rem; white-space: nowrap;">-16 ~ +16</span>
        </div>
        <p style="margin: 0; font-size: 0.82rem; color: #aaa; line-height: 1.5;">取得整數解析度的循線位置。<code>0</code> 為正中央，負數偏左，正數偏右。</p>
      </div>
    </div>
  </div>
  <!-- 🟡 模式 1 -->
  <div style="margin-bottom: 12px;">
    <div style="font-size: 0.82rem; color: #0abab5; font-weight: bold; margin-bottom: 8px; padding-left: 2px;">🟡 模式 1 (全覽/除錯/自訂算法)</div>
    <div style="display: grid; grid-template-columns: 1fr; gap: 12px;">
      <div class="func-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(10,186,181,0.15); border-radius: 10px; padding: 16px; transition: all 0.3s ease;">
        <div style="display: flex; align-items: center; justify-content: space-between; margin-bottom: 10px;">
          <code style="color: #0abab5; font-size: 0.95rem; font-weight: bold;"><span style="background: rgba(255,255,255,0.15); color: #fff; padding: 2px 6px; border-radius: 4px; font-size: 0.75rem; margin-right: 6px;">[模式 1: 解碼]</span>line_ir_ch(ch)</code>
          <span style="background: rgba(217,163,0,0.15); color: #d9a300; padding: 3px 10px; border-radius: 20px; font-family: monospace; font-size: 0.75rem; white-space: nowrap;">1 ~ 9</span>
        </div>
        <p style="margin: 0; font-size: 0.82rem; color: #aaa; line-height: 1.5;">取得單一感應器通道的即時量化光值。<code>ch</code>=1~16。1=最暗，9=最亮。</p>
      </div>
    </div>
  </div>
  <!-- 🔵 模式 2 -->
  <div style="margin-bottom: 12px;">
    <div style="font-size: 0.82rem; color: #0abab5; font-weight: bold; margin-bottom: 8px; padding-left: 2px;">🔵 模式 2 (競技/PID 循線專用)</div>
    <div style="display: grid; grid-template-columns: repeat(2, 1fr); gap: 12px;">
      <div class="func-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(10,186,181,0.15); border-radius: 10px; padding: 16px; transition: all 0.3s ease;">
        <div style="display: flex; align-items: center; justify-content: space-between; margin-bottom: 10px;">
          <code style="color: #0abab5; font-size: 0.95rem; font-weight: bold;"><span style="background: rgba(255,255,255,0.15); color: #fff; padding: 2px 6px; border-radius: 4px; font-size: 0.75rem; margin-right: 6px;">[模式 2: 競技]</span>line_pos100()</code>
          <span style="background: rgba(255,107,53,0.15); color: #ff6b35; padding: 3px 10px; border-radius: 20px; font-family: monospace; font-size: 0.75rem; white-space: nowrap;">-100 ~ +100</span>
        </div>
        <p style="margin: 0; font-size: 0.82rem; color: #aaa; line-height: 1.5;">高解析度次像素級插值位置，專為高階 PID / PD 控制設計。</p>
      </div>
      <div class="func-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(10,186,181,0.15); border-radius: 10px; padding: 16px; transition: all 0.3s ease;">
        <div style="display: flex; align-items: center; justify-content: space-between; margin-bottom: 10px;">
          <code style="color: #0abab5; font-size: 0.95rem; font-weight: bold;"><span style="background: rgba(255,255,255,0.15); color: #fff; padding: 2px 6px; border-radius: 4px; font-size: 0.75rem; margin-right: 6px;">[模式 2: 競技]</span>line_width()</code>
          <span style="background: rgba(10,186,181,0.15); color: #0abab5; padding: 3px 10px; border-radius: 20px; font-family: monospace; font-size: 0.75rem; white-space: nowrap;">0 ~ 16</span>
        </div>
        <p style="margin: 0; font-size: 0.82rem; color: #aaa; line-height: 1.5;">取得特徵線寬（觸發的感測器數量）。可用於偵測路口與寬線。</p>
      </div>
      <div class="func-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(10,186,181,0.15); border-radius: 10px; padding: 16px; transition: all 0.3s ease;">
        <div style="display: flex; align-items: center; justify-content: space-between; margin-bottom: 10px;">
          <code style="color: #0abab5; font-size: 0.95rem; font-weight: bold;"><span style="background: rgba(255,255,255,0.15); color: #fff; padding: 2px 6px; border-radius: 4px; font-size: 0.75rem; margin-right: 6px;">[模式 2: 競技]</span>line_edge_l()</code>
          <span style="background: rgba(217,163,0,0.15); color: #d9a300; padding: 3px 10px; border-radius: 20px; font-family: monospace; font-size: 0.75rem; white-space: nowrap;">0 ~ 100</span>
        </div>
        <p style="margin: 0; font-size: 0.82rem; color: #aaa; line-height: 1.5;">取得左邊界感測器的實體光值（0=黑, 100=白），用於防掉線。</p>
      </div>
      <div class="func-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(10,186,181,0.15); border-radius: 10px; padding: 16px; transition: all 0.3s ease;">
        <div style="display: flex; align-items: center; justify-content: space-between; margin-bottom: 10px;">
          <code style="color: #0abab5; font-size: 0.95rem; font-weight: bold;"><span style="background: rgba(255,255,255,0.15); color: #fff; padding: 2px 6px; border-radius: 4px; font-size: 0.75rem; margin-right: 6px;">[模式 2: 競技]</span>line_edge_r()</code>
          <span style="background: rgba(217,163,0,0.15); color: #d9a300; padding: 3px 10px; border-radius: 20px; font-family: monospace; font-size: 0.75rem; white-space: nowrap;">0 ~ 100</span>
        </div>
        <p style="margin: 0; font-size: 0.82rem; color: #aaa; line-height: 1.5;">取得右邊界感測器的實體光值（0=黑, 100=白），用於防掉線。</p>
      </div>
    </div>
  </div>

> [!IMPORTANT]
> **⚠️ 模式切換等待延遲警告**
> 由於 EV3 硬體底層的通訊機制限制，當程式在不同底層讀取模式之間進行切換時，**仍然會產生通訊等待延遲**。
> **強烈建議**：為了確保程式穩定性與高速反應，請根據任務需求選定一種模式使用，**不要在主迴圈中頻繁交叉呼叫不同模式的函數**（上方函數已為您標註所屬模式群組）。
> 
> 👉 **實戰建議**：強烈建議一般使用者與競賽選手全程使用 <strong>[模式 2: 競技]</strong> 的函數群；<br>
> 只有當您需要讀取所有感測器原始數值，進行「自訂特殊演算法」開發時，才建議單獨使用 <strong>[模式 1: 解碼]</strong>。

  </div>
  <div class="pybricks-download-grid" style="display: grid; grid-template-columns: repeat(auto-fit, minmax(280px, 1fr)); gap: 25px;">
    <!-- Card 3: Block 專屬庫 -->
    <div class="download-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(0,210,255,0.15); border-radius: 12px; padding: 20px; box-shadow: 0 10px 30px rgba(0,0,0,0.3); transition: all 0.3s ease; display: flex; flex-direction: column; justify-content: space-between;">
      <div>
        <div style="width: 100%; border-radius: 8px; overflow: hidden; border: 1px solid rgba(255,255,255,0.1); background: #000; margin-bottom: 15px;">
          <img src="../images/sensors/line16/ev3-pybricks-block-with-lib.webp" alt="Block 模式 (專屬庫函數)" style="width: 100%; height: 160px; object-fit: cover; opacity: 0.85; transition: opacity 0.3s ease;" />
        </div>
        <h3 style="margin: 0 0 10px 0; color: #0abab5; font-size: 1.15rem; font-weight: bold;">🧩 Block 模式 (導入專屬庫)</h3>
        <p style="margin: 0 0 15px 0; font-size: 0.85rem; color: #aaa; line-height: 1.5;">圖形化積木結合我們為您開發的專屬庫函數，在積木開發下輕鬆取得高解析度高精度數值。</p>
      </div>
      <a href="../examples/line16/ev3/line16_ev3_block_with_lib.py" target="_blank" download="line16_ev3_block_with_lib.py" data-ignore="true" class="btn-download" style="display: block; text-align: center; background: linear-gradient(135deg, #0abab5, #007a75); color: #fff; padding: 10px; border-radius: 6px; text-decoration: none; font-weight: bold; font-size: 0.9rem; box-shadow: 0 4px 15px rgba(10,186,181,0.25); transition: transform 0.2s ease;">📥 下載範例 (.py)</a>
    </div>
    <!-- Card 4: Python 專屬庫 -->
    <div class="download-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(0,210,255,0.15); border-radius: 12px; padding: 20px; box-shadow: 0 10px 30px rgba(0,0,0,0.3); transition: all 0.3s ease; display: flex; flex-direction: column; justify-content: space-between;">
      <div>
        <div style="width: 100%; border-radius: 8px; overflow: hidden; border: 1px solid rgba(255,255,255,0.1); background: #000; margin-bottom: 15px;">
          <img src="../images/sensors/line16/ev3-pybricks-python-with-lib.webp" alt="MicroPython 模式 (專屬庫函數)" style="width: 100%; height: 160px; object-fit: cover; opacity: 0.85; transition: opacity 0.3s ease;" />
        </div>
        <h3 style="margin: 0 0 10px 0; color: #0abab5; font-size: 1.15rem; font-weight: bold;">🐍 Python 模式 (導入專屬庫)</h3>
        <p style="margin: 0 0 15px 0; font-size: 0.85rem; color: #aaa; line-height: 1.5;">高階選手的最佳方案！導入專屬庫進行最底層的完整數據調用與高頻 PID 循線算法開發。</p>
      </div>
      <a href="../examples/line16/ev3/line16_ev3_python_with_lib.py" target="_blank" download="line16_ev3_python_with_lib.py" data-ignore="true" class="btn-download" style="display: block; text-align: center; background: linear-gradient(135deg, #0abab5, #007a75); color: #fff; padding: 10px; border-radius: 6px; text-decoration: none; font-weight: bold; font-size: 0.9rem; box-shadow: 0 4px 15px rgba(10,186,181,0.25); transition: transform 0.2s ease;">📥 下載範例 (.py)</a>
    </div>
  </div>

  <!-- Library Card (Full Width Highlighted Card) -->
  <div class="library-download-box" style="margin-top: 25px; background: rgba(10,186,181,0.03); border: 2px solid rgba(10,186,181,0.3); border-radius: 12px; padding: 25px; box-shadow: 0 15px 35px rgba(0,0,0,0.4); transition: all 0.3s ease;">
    <h3 style="margin: 0 0 15px 0; color: #0abab5; font-size: 1.3rem; font-weight: bold; text-align: center;">📦 EV3 pyBricks 專屬庫函數 (Library File)</h3>
    <div style="display: flex; flex-wrap: wrap; gap: 25px; align-items: stretch;">
      <!-- 左側：庫函數圖片與下載按鈕 -->
      <div style="flex: 1; min-width: 260px; display: flex; flex-direction: column; justify-content: space-between;">
        <div>
          <div style="border-radius: 8px; overflow: hidden; border: 1px solid rgba(10,186,181,0.2); background: #000; margin-bottom: 15px;">
            <img src="../images/sensors/line16/ev3-pybricks-lib-icon.webp" alt="16路循線感應器專屬 Pybricks 庫函數" style="width: 100%; height: 160px; object-fit: cover; opacity: 0.9;" />
          </div>
          <p style="font-size: 0.85rem; color: #ccc; line-height: 1.6; margin-bottom: 15px;">
            這是配合「導入專屬庫」範例不可或缺的驅動核心。請下載此庫檔案，即可完美啟用高階插值位置與 16 路光電數值精細讀取。
          </p>
        </div>
        <a href="../examples/line16/ev3/MBC_line16_EV3_Lib.py" target="_blank" download="MBC_line16_EV3_Lib.py" data-ignore="true" class="btn-download-lib" style="display: block; text-align: center; padding: 12px 20px; background: linear-gradient(135deg, #0abab5, #00d2ff); color: #fff; text-decoration: none; border-radius: 8px; font-weight: bold; font-size: 1rem; box-shadow: 0 6px 20px rgba(10,186,181,0.3); transition: transform 0.2s ease, box-shadow 0.2s ease;">📥 下載專屬庫函數 (MBC_line16_EV3_Lib.py)</a>
      </div>
      <!-- 右側：重要提示與截圖 -->
      <div style="flex: 1.2; min-width: 280px; display: flex;">
        <div style="background: rgba(255,107,53,0.08); border-left: 4px solid #ff6b35; border-radius: 0 8px 8px 0; padding: 15px 20px; flex-grow: 1;">
          <h4 style="margin: 0 0 8px 0; color: #ff6b35; font-size: 1.05rem; display: flex; align-items: center; gap: 6px;">⚠️ 重要提示：檔案放置位置</h4>
          <p style="margin: 0 0 15px 0; font-size: 0.85rem; color: #ddd; line-height: 1.6;">
            請確保將 <code>MBC_line16_EV3_Lib.py</code> 檔案直接匯入到您的 Pybricks 專案中，<strong>且檔案名稱必須完全一致</strong>，主程式的 <code>import</code> 指令才能正確載入函數！
          </p>
          <div style="border-radius: 6px; overflow: hidden; border: 1px solid rgba(255,107,53,0.3); background: #000; box-shadow: 0 5px 15px rgba(0,0,0,0.2);">
            <img src="../images/sensors/line16/ev3-pybricks-upload-lib.webp" alt="Pybricks 匯入庫檔案示範" style="width: 100%; height: auto; display: block;" />
          </div>
        </div>
      </div>
    </div>
  </div>
</div>

<style>
  .table-hover-row:hover {
    background: rgba(0,0,0,0.04) !important;
  }
  .func-card:hover {
    transform: translateY(-2px);
    box-shadow: 0 5px 15px rgba(0, 210, 255, 0.1);
    border-color: rgba(0, 210, 255, 0.4) !important;
  }
  .btn-download:hover {
    transform: translateY(-2px);
    box-shadow: 0 6px 20px rgba(0,210,255,0.4);
  }
</style>
