# pyBricks for SPIKE Prime / Robot Inventor

[🔙 回到 SPIKE 主機選單](/sensors/line8/spike-hub.md)

<div style="display: flex; align-items: center; justify-content: center; gap: 30px; margin: 30px 0; flex-wrap: wrap;">
  <div style="background: rgba(0,210,255,0.03); border: 1px solid rgba(0,210,255,0.25); border-radius: 12px; padding: 15px 35px; display: flex; align-items: center; gap: 20px; box-shadow: 0 10px 30px rgba(0,0,0,0.15);">
    <img src="../images/hubs/spike-hub.webp" alt="SPIKE Prime Hub" style="max-height: 90px; object-fit: contain; filter: drop-shadow(0 0 15px rgba(242,194,0,0.35));" />
    <span style="font-size: 2.2rem; color: #666; font-weight: 300; line-height: 1;">+</span>
    <img src="../images/hubs/inventor-hub.webp" alt="MINDSTORMS Hub" style="max-height: 90px; object-fit: contain; filter: drop-shadow(0 0 15px rgba(10,186,181,0.35));" />
    <span style="font-size: 2.2rem; color: #666; font-weight: 300; line-height: 1;">+</span>
    <img src="../images/hubs/spike-pybricks-logo.webp" alt="pyBricks" style="max-height: 90px; object-fit: contain; filter: drop-shadow(0 0 15px rgba(0,210,255,0.25));" />
  </div>
</div>

> [!WARNING] 
> **請注意：您目前觀看的是 【pyBricks 專區 (雙主機通用)】 的 MicroPython 高階開發教學。**
> 如果您使用的是 EV3 或是其他官方系統軟體，請點擊上方按鈕回到主機選單重新選擇。

本頁適用於 **LEGO® Education SPIKE™ Prime Set**、**LEGO® MINDSTORMS® Robot Inventor**上安裝 pyBricks 第三方 MicroPython 韌體的使用者。

在 Pybricks 開發環境中，使用者可以利用 Python 指令進行高速、穩定的通訊。本感應器支援以內建的圖形指令進行簡易讀取，或導入我們的專屬庫函數獲得高解析度資料與更豐富的控制功能。

---

## 🚀 讀取方式與核心模式

在 Pybricks 開發環境中，本循線感應器提供兩種靈活的資料讀取管道，適應不同的專案需求：

1. **內建感應器指令 (當作顏色感應器讀取)**：直接使用 Pybricks 內建的 `ColorSensor` 指令讀取基本的光電或特徵數據，無需載入額外代碼，操作最直覺。
2. **導入專屬庫函數 (Library Functions)**：透過導入我們為 8路循線感應器特別編寫的 Python 庫函數，能直接獲得高解析度插值位置、完整 8 路反射光數值以及特徵線寬，提供更豐富的高階循線控制。

---

## 📥 範例程式與庫下載 (打包下載)

<div style="display: flex; gap: 15px; margin-top: 15px; flex-wrap: wrap;">
  <a href="../examples/line8/line8_pybricks_spike_v3.6.1.zip" target="_blank" data-ignore="true" download class="btn-download" style="flex: 1; min-width: 200px; text-align: center; background: linear-gradient(135deg, #00d2ff, #007a75); color: #fff; padding: 12px; border-radius: 8px; text-decoration: none; font-weight: bold; font-size: 1rem; box-shadow: 0 4px 15px rgba(0,210,255,0.25); transition: transform 0.2s ease;">📥 打包下載 (韌體 3.6.1 版)</a>
  <a href="../examples/line8/line8_pybricks_spike_v4.0.0.zip" target="_blank" data-ignore="true" download class="btn-download" style="flex: 1; min-width: 200px; text-align: center; background: linear-gradient(135deg, #0abab5, #007a75); color: #fff; padding: 12px; border-radius: 8px; text-decoration: none; font-weight: bold; font-size: 1rem; box-shadow: 0 4px 15px rgba(10,186,181,0.25); transition: transform 0.2s ease;">📥 打包下載 (韌體 4.0.0 版)</a>
</div>

我們為您提供了四個核心範例程式以及專屬庫函數。請依照您的開發模式選擇下載：

<!-- ========== 類別一：內建感應器指令 ========== -->
<div class="category-section" style="margin: 30px 0 40px 0;">
  <div class="category-header" style="display: flex; align-items: center; gap: 12px; margin-bottom: 20px; padding: 14px 22px; background: linear-gradient(135deg, rgba(0,210,255,0.08), rgba(0,210,255,0.02)); border: 1px solid rgba(0,210,255,0.25); border-radius: 10px;">
    <span style="font-size: 1.6rem;">⚡</span>
    <div>
      <h3 style="margin: 0; color: #00d2ff; font-size: 1.2rem; font-weight: bold;">方式一：內建感應器指令</h3>
      <p style="margin: 4px 0 0 0; font-size: 0.82rem; color: #999; line-height: 1.4;">直接使用 Pybricks 內建指令讀取感應器，無需載入額外代碼，快速上手。</p>
    </div>
  </div>
  <div style="margin-bottom: 25px;">
    <div style="display: flex; align-items: center; gap: 8px; margin-bottom: 15px;">
      <h4 style="margin: 0; color: #00d2ff; font-size: 1rem;">📋 可用函數一覽</h4>
      <span style="font-size: 0.75rem; color: #888;">— 透過 Pybricks 內建 <code style="background: rgba(0,210,255,0.1); padding: 1px 5px; border-radius: 3px; font-size: 0.75rem;">ColorSensor</code> 計算得出，無需額外載入庫檔案</span>
    </div>
    <!-- 🎯 基礎組合 -->
    <div style="margin-bottom: 16px;">
      <div style="display: flex; align-items: center; gap: 8px; margin-bottom: 8px; padding-left: 2px;">
        <span style="font-size: 0.85rem; color: #00d2ff; font-weight: bold;">🎯 基礎組合</span>
        <span style="font-size: 0.72rem; color: #777; border-left: 1px solid rgba(255,255,255,0.1); padding-left: 8px;">開箱即用，不需校準，適合快速開發與初學者</span>
      </div>
      <div class="responsive-grid-2">
        <div class="func-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(0,210,255,0.15); border-radius: 10px; padding: 16px; transition: all 0.3s ease;">
          <div style="display: flex; align-items: center; justify-content: space-between; margin-bottom: 10px;">
            <code style="color: #00d2ff; font-size: 0.95rem; font-weight: bold;">getLinePos8()</code>
            <span style="background: rgba(255,107,53,0.15); color: #ff6b35; padding: 3px 10px; border-radius: 20px; font-family: monospace; font-size: 0.75rem; white-space: nowrap;">-8 ~ +8</span>
          </div>
          <p style="margin: 0; font-size: 0.82rem; color: #aaa; line-height: 1.5;">取得循線位置（整數解析度）。<code style="background: rgba(0,210,255,0.1); padding: 1px 4px; border-radius: 3px; font-size: 0.78rem;">0</code> 為正中央，負數偏左，正數偏右。適合基本 PID 循線控制。</p>
        </div>
        <div class="func-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(0,210,255,0.15); border-radius: 10px; padding: 16px; transition: all 0.3s ease;">
          <div style="display: flex; align-items: center; justify-content: space-between; margin-bottom: 10px;">
            <code style="color: #00d2ff; font-size: 0.95rem; font-weight: bold;">getLineWidth8()</code>
            <span style="background: rgba(10,186,181,0.15); color: #0abab5; padding: 3px 10px; border-radius: 20px; font-family: monospace; font-size: 0.75rem; white-space: nowrap;">0 ~ 8</span>
          </div>
          <p style="margin: 0; font-size: 0.82rem; color: #aaa; line-height: 1.5;">取得目前偵測到的特徵線寬（觸發的感測器數量）。可用於偵測十字路口、T 字路口等寬線特徵。</p>
        </div>
      </div>
    </div>
    <!-- 🔬 高精度組合 -->
    <div>
      <div style="display: flex; align-items: center; gap: 8px; margin-bottom: 8px; padding-left: 2px;">
        <span style="font-size: 0.85rem; color: #00d2ff; font-weight: bold;">🔬 高精度組合</span>
        <span style="font-size: 0.72rem; color: #777; border-left: 1px solid rgba(255,255,255,0.1); padding-left: 8px;">建議先完成感應器校準，校準越精確數值越準確</span>
      </div>
      <div class="responsive-grid-2">
        <div class="func-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(0,210,255,0.15); border-radius: 10px; padding: 16px; transition: all 0.3s ease;">
          <div style="display: flex; align-items: center; justify-content: space-between; margin-bottom: 10px;">
            <code style="color: #00d2ff; font-size: 0.95rem; font-weight: bold;">getLinePos100()</code>
            <span style="background: rgba(255,107,53,0.15); color: #ff6b35; padding: 3px 10px; border-radius: 20px; font-family: monospace; font-size: 0.75rem; white-space: nowrap;">-100 ~ +100</span>
          </div>
          <p style="margin: 0; font-size: 0.82rem; color: #aaa; line-height: 1.5;">取得高解析度次像素級插值位置。精度遠高於 <code style="background: rgba(0,210,255,0.1); padding: 1px 4px; border-radius: 3px; font-size: 0.78rem;">getLinePos8()</code>，適合高階 PID 精密循線。</p>
        </div>
        <div class="func-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(0,210,255,0.15); border-radius: 10px; padding: 16px; transition: all 0.3s ease;">
          <div style="display: flex; align-items: center; justify-content: space-between; margin-bottom: 10px;">
            <code style="color: #00d2ff; font-size: 0.95rem; font-weight: bold;">getLineWidth100()</code>
            <span style="background: rgba(10,186,181,0.15); color: #0abab5; padding: 3px 10px; border-radius: 20px; font-family: monospace; font-size: 0.75rem; white-space: nowrap;">0 ~ 8</span>
          </div>
          <p style="margin: 0; font-size: 0.82rem; color: #aaa; line-height: 1.5;">取得高精度模式下的特徵線寬。與 <code style="background: rgba(0,210,255,0.1); padding: 1px 4px; border-radius: 3px; font-size: 0.78rem;">getLinePos100()</code> 搭配使用，確保數據同步一致。</p>
        </div>
      </div>
    </div>
    <div style="margin-top: 12px; padding: 10px 14px; background: rgba(0,210,255,0.04); border-left: 3px solid rgba(0,210,255,0.3); border-radius: 0 6px 6px 0; font-size: 0.78rem; color: #999; line-height: 1.5;">
      💡 <strong style="color: #bbb;">使用提示：</strong>請選擇同一組的位置與線寬函數搭配使用。例如使用 <code style="background: rgba(0,210,255,0.1); padding: 1px 4px; border-radius: 3px; font-size: 0.75rem;">getLinePos8()</code> 就搭配 <code style="background: rgba(0,210,255,0.1); padding: 1px 4px; border-radius: 3px; font-size: 0.75rem;">getLineWidth8()</code>，使用 <code style="background: rgba(0,210,255,0.1); padding: 1px 4px; border-radius: 3px; font-size: 0.75rem;">getLinePos100()</code> 就搭配 <code style="background: rgba(0,210,255,0.1); padding: 1px 4px; border-radius: 3px; font-size: 0.75rem;">getLineWidth100()</code>，這樣才能獲得最即時、最準確的讀取效果。
    </div>
  </div>
  <div class="pybricks-download-grid" style="display: grid; grid-template-columns: repeat(auto-fit, minmax(280px, 1fr)); gap: 25px;">
    <!-- Card 1: Block 內建 -->
    <div class="download-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(0,210,255,0.15); border-radius: 12px; padding: 20px; box-shadow: 0 10px 30px rgba(0,0,0,0.3); transition: all 0.3s ease; display: flex; flex-direction: column; justify-content: space-between;">
      <div>
        <div style="width: 100%; border-radius: 8px; overflow: hidden; border: 1px solid rgba(255,255,255,0.1); background: #000; margin-bottom: 15px;">
          <img src="../images/sensors/line8/pybricks-block-native.webp" alt="Block 模式 (內建指令)" style="width: 100%; height: 160px; object-fit: cover; opacity: 0.85; transition: opacity 0.3s ease;" />
        </div>
        <h3 style="margin: 0 0 10px 0; color: #00d2ff; font-size: 1.15rem; font-weight: bold;">🧩 Block 模式 (內建指令)</h3>
        <p style="margin: 0 0 15px 0; font-size: 0.85rem; color: #aaa; line-height: 1.5;">使用 Pybricks 內建的圖形化積木指令讀取感應器，適合初學者快速上手基本循線邏輯。</p>
      </div>
      <div class="download-btn-group">
        <a href="../examples/line8/pybricks/For%20firmware%203.6.1/line8_block_native.py" target="_blank" download="line8_block_native.py" data-ignore="true" class="btn-download" style="flex: 1; text-align: center; background: linear-gradient(135deg, #00d2ff, #007a75); color: #fff; padding: 8px; border-radius: 6px; text-decoration: none; font-weight: bold; font-size: 0.85rem; box-shadow: 0 4px 15px rgba(0,210,255,0.25); transition: transform 0.2s ease;">📥 3.6.1 版</a>
        <a href="../examples/line8/pybricks/For%20firmware%204.0.0/line8_block_native.py" target="_blank" download="line8_block_native.py" data-ignore="true" class="btn-download" style="flex: 1; text-align: center; background: linear-gradient(135deg, #0abab5, #007a75); color: #fff; padding: 8px; border-radius: 6px; text-decoration: none; font-weight: bold; font-size: 0.85rem; box-shadow: 0 4px 15px rgba(10,186,181,0.25); transition: transform 0.2s ease;">📥 4.0.0 版</a>
      </div>
    </div>
    <!-- Card 2: Python 內建 -->
    <div class="download-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(0,210,255,0.15); border-radius: 12px; padding: 20px; box-shadow: 0 10px 30px rgba(0,0,0,0.3); transition: all 0.3s ease; display: flex; flex-direction: column; justify-content: space-between;">
      <div>
        <div style="width: 100%; border-radius: 8px; overflow: hidden; border: 1px solid rgba(255,255,255,0.1); background: #000; margin-bottom: 15px;">
          <img src="../images/sensors/line8/pybricks-python-native.webp" alt="Python 模式 (內建指令)" style="width: 100%; height: 160px; object-fit: cover; opacity: 0.85; transition: opacity 0.3s ease;" />
        </div>
        <h3 style="margin: 0 0 10px 0; color: #00d2ff; font-size: 1.15rem; font-weight: bold;">🐍 Python 模式 (內建指令)</h3>
        <p style="margin: 0 0 15px 0; font-size: 0.85rem; color: #aaa; line-height: 1.5;">使用 Pybricks 原生 MicroPython 內建感應器指令直接讀取，兼顧程式靈活性與純文字開發體驗。</p>
      </div>
      <div class="download-btn-group">
        <a href="../examples/line8/pybricks/For%20firmware%203.6.1/line8_python_native.py" target="_blank" download="line8_python_native.py" data-ignore="true" class="btn-download" style="flex: 1; text-align: center; background: linear-gradient(135deg, #00d2ff, #007a75); color: #fff; padding: 8px; border-radius: 6px; text-decoration: none; font-weight: bold; font-size: 0.85rem; box-shadow: 0 4px 15px rgba(0,210,255,0.25); transition: transform 0.2s ease;">📥 3.6.1 版</a>
        <a href="../examples/line8/pybricks/For%20firmware%204.0.0/line8_python_native.py" target="_blank" download="line8_python_native.py" data-ignore="true" class="btn-download" style="flex: 1; text-align: center; background: linear-gradient(135deg, #0abab5, #007a75); color: #fff; padding: 8px; border-radius: 6px; text-decoration: none; font-weight: bold; font-size: 0.85rem; box-shadow: 0 4px 15px rgba(10,186,181,0.25); transition: transform 0.2s ease;">📥 4.0.0 版</a>
      </div>
    </div>
  </div>
</div>

<!-- ========== 類別二：導入專屬庫函數 ========== -->
<div class="category-section" style="margin: 40px 0 30px 0;">
  <div class="category-header" style="display: flex; align-items: center; gap: 12px; margin-bottom: 20px; padding: 14px 22px; background: linear-gradient(135deg, rgba(10,186,181,0.08), rgba(10,186,181,0.02)); border: 1px solid rgba(10,186,181,0.25); border-radius: 10px;">
    <span style="font-size: 1.6rem;">🔬</span>
    <div>
      <h3 style="margin: 0; color: #0abab5; font-size: 1.2rem; font-weight: bold;">方式二：導入專屬庫函數</h3>
      <p style="margin: 4px 0 0 0; font-size: 0.82rem; color: #999; line-height: 1.4;">透過導入我們的專屬 Python 庫函數，獲得高解析度插值位置、完整 8 路數據與線寬等進階功能。</p>
    </div>
  </div>
  <div style="margin-bottom: 25px;">
    <div style="display: flex; align-items: center; gap: 8px; margin-bottom: 15px;">
      <h4 style="margin: 0; color: #0abab5; font-size: 1rem;">📋 可用函數一覽</h4>
      <span style="font-size: 0.75rem; color: #888;">— 由 <code style="background: rgba(10,186,181,0.1); padding: 1px 5px; border-radius: 3px; font-size: 0.75rem;">MBC_line8_Lib.py</code> 提供</span>
    </div>
    <!-- ⚙️ 初始化 -->
    <div style="margin-bottom: 12px;">
      <div style="font-size: 0.82rem; color: #0abab5; font-weight: bold; margin-bottom: 8px; padding-left: 2px;">⚙️ 初始化</div>
      <div style="display: grid; grid-template-columns: 1fr; gap: 12px;">
        <div class="func-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(10,186,181,0.15); border-radius: 10px; padding: 16px; transition: all 0.3s ease;">
          <div style="display: flex; align-items: center; justify-content: space-between; margin-bottom: 10px; flex-wrap: wrap; gap: 6px;">
            <code style="color: #0abab5; font-size: 0.95rem; font-weight: bold;">line_init_port_multitask(port, multitask)</code>
          </div>
          <p style="margin: 0; font-size: 0.82rem; color: #aaa; line-height: 1.5;">初始化感應器連接埠。<code style="background: rgba(10,186,181,0.1); padding: 1px 4px; border-radius: 3px; font-size: 0.78rem;">port</code>=1~6 對應 A~F；<code style="background: rgba(10,186,181,0.1); padding: 1px 4px; border-radius: 3px; font-size: 0.78rem;">multitask</code>=True 時啟用非同步模式（搭配 <code style="background: rgba(10,186,181,0.1); padding: 1px 4px; border-radius: 3px; font-size: 0.78rem;">await</code> 使用）。</p>
        </div>
      </div>
    </div>
    <!-- 📍 位置與線寬 -->
    <div style="margin-bottom: 12px;">
      <div style="font-size: 0.82rem; color: #0abab5; font-weight: bold; margin-bottom: 8px; padding-left: 2px;">📍 位置與線寬</div>
      <div style="display: grid; grid-template-columns: repeat(auto-fit, minmax(220px, 1fr)); gap: 12px;">
        <div class="func-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(10,186,181,0.15); border-radius: 10px; padding: 16px; transition: all 0.3s ease;">
          <div style="display: flex; align-items: center; justify-content: space-between; margin-bottom: 10px;">
            <div>
              <code style="color: #0abab5; font-size: 0.95rem; font-weight: bold;">line_pos8()</code>
              <span style="font-size: 0.7rem; color: #888; margin-left: 6px;">🎯 基礎免校準</span>
            </div>
            <span style="background: rgba(255,107,53,0.15); color: #ff6b35; padding: 3px 10px; border-radius: 20px; font-family: monospace; font-size: 0.75rem; white-space: nowrap;">-8 ~ +8</span>
          </div>
          <p style="margin: 0; font-size: 0.82rem; color: #aaa; line-height: 1.5;">取得整數解析度的循線位置。<code style="background: rgba(10,186,181,0.1); padding: 1px 4px; border-radius: 3px; font-size: 0.78rem;">0</code> 為正中央，負數偏左，正數偏右。<strong style="color: #bbb;">開箱即用不需校準，適合快速開發與初學者。</strong></p>
        </div>
        <div class="func-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(10,186,181,0.15); border-radius: 10px; padding: 16px; transition: all 0.3s ease;">
          <div style="display: flex; align-items: center; justify-content: space-between; margin-bottom: 10px;">
            <div>
              <code style="color: #0abab5; font-size: 0.95rem; font-weight: bold;">line_pos100()</code>
              <span style="font-size: 0.7rem; color: #888; margin-left: 6px;">🔬 需精確校準</span>
            </div>
            <span style="background: rgba(255,107,53,0.15); color: #ff6b35; padding: 3px 10px; border-radius: 20px; font-family: monospace; font-size: 0.75rem; white-space: nowrap;">-100 ~ +100</span>
          </div>
          <p style="margin: 0; font-size: 0.82rem; color: #aaa; line-height: 1.5;">取得高解析度次像素級插值位置，最佳 PID 循線選擇。<strong style="color: #bbb;">使用前提是必須確實完成感應器校準，校準越精確數值越準確。</strong></p>
        </div>
        <div class="func-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(10,186,181,0.15); border-radius: 10px; padding: 16px; transition: all 0.3s ease;">
          <div style="display: flex; align-items: center; justify-content: space-between; margin-bottom: 10px;">
            <code style="color: #0abab5; font-size: 0.95rem; font-weight: bold;">line_width()</code>
            <span style="background: rgba(10,186,181,0.15); color: #0abab5; padding: 3px 10px; border-radius: 20px; font-family: monospace; font-size: 0.75rem; white-space: nowrap;">0 ~ 8</span>
          </div>
          <p style="margin: 0; font-size: 0.82rem; color: #aaa; line-height: 1.5;">取得特徵線寬（觸發的感測器數量）。可用於偵測路口與寬線特徵，<strong style="color: #bbb;">同時適用於上述兩種位置函數。</strong></p>
        </div>
      </div>
    </div>
    <!-- 🔦 8 路紅外線光值 -->
    <div style="margin-bottom: 12px;">
      <div style="font-size: 0.82rem; color: #0abab5; font-weight: bold; margin-bottom: 8px; padding-left: 2px;">🔦 8 路紅外線光值</div>
      <div style="display: grid; grid-template-columns: repeat(auto-fit, minmax(240px, 1fr)); gap: 12px;">
        <div class="func-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(10,186,181,0.15); border-radius: 10px; padding: 16px; transition: all 0.3s ease;">
          <div style="display: flex; align-items: center; justify-content: space-between; margin-bottom: 10px;">
            <code style="color: #0abab5; font-size: 0.95rem; font-weight: bold;">line_ir_calibration()</code>
            <span style="background: rgba(217,163,0,0.15); color: #d9a300; padding: 3px 10px; border-radius: 20px; font-family: monospace; font-size: 0.75rem; white-space: nowrap;">list[8]</span>
          </div>
          <p style="margin: 0; font-size: 0.82rem; color: #aaa; line-height: 1.5;">回傳 8 路校正後紅外線反射光值陣列（每值 0~15）。index 0 = 最右側 (ch1)，index 7 = 最左側 (ch8)。</p>
        </div>
        <div class="func-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(10,186,181,0.15); border-radius: 10px; padding: 16px; transition: all 0.3s ease;">
          <div style="display: flex; align-items: center; justify-content: space-between; margin-bottom: 10px;">
            <code style="color: #0abab5; font-size: 0.95rem; font-weight: bold;">line_ir_ch(ch)</code>
            <span style="background: rgba(217,163,0,0.15); color: #d9a300; padding: 3px 10px; border-radius: 20px; font-family: monospace; font-size: 0.75rem; white-space: nowrap;">0 ~ 15</span>
          </div>
          <p style="margin: 0; font-size: 0.82rem; color: #aaa; line-height: 1.5;">取得單一通道的校正光值。<code style="background: rgba(10,186,181,0.1); padding: 1px 4px; border-radius: 3px; font-size: 0.78rem;">ch</code>=1~8，可高效連續呼叫（共用快取）。</p>
        </div>
      </div>
    </div>
    <!-- 🔀 路口偵測與二進位數據 -->
    <div style="margin-bottom: 12px;">
      <div style="font-size: 0.82rem; color: #0abab5; font-weight: bold; margin-bottom: 8px; padding-left: 2px;">🔀 路口偵測與二進位數據</div>
      <div class="responsive-grid-2">
        <div class="func-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(10,186,181,0.15); border-radius: 10px; padding: 16px; transition: all 0.3s ease;">
          <div style="display: flex; align-items: center; justify-content: space-between; margin-bottom: 10px;">
            <code style="color: #0abab5; font-size: 0.95rem; font-weight: bold;">line_junction()</code>
            <span style="background: rgba(156,39,176,0.15); color: #ce93d8; padding: 3px 10px; border-radius: 20px; font-family: monospace; font-size: 0.75rem; white-space: nowrap;">0, 1, 2+</span>
          </div>
          <p style="margin: 0; font-size: 0.82rem; color: #aaa; line-height: 1.5;">回傳偵測到的線群數量。<code style="background: rgba(10,186,181,0.1); padding: 1px 4px; border-radius: 3px; font-size: 0.78rem;">0</code>=無線、<code style="background: rgba(10,186,181,0.1); padding: 1px 4px; border-radius: 3px; font-size: 0.78rem;">1</code>=單線、<code style="background: rgba(10,186,181,0.1); padding: 1px 4px; border-radius: 3px; font-size: 0.78rem;">2+</code>=路口分叉。</p>
        </div>
        <div class="func-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(10,186,181,0.15); border-radius: 10px; padding: 16px; transition: all 0.3s ease;">
          <div style="display: flex; align-items: center; justify-content: space-between; margin-bottom: 10px;">
            <code style="color: #0abab5; font-size: 0.95rem; font-weight: bold;">line_junction_name()</code>
            <span style="background: rgba(156,39,176,0.15); color: #ce93d8; padding: 3px 10px; border-radius: 20px; font-family: monospace; font-size: 0.75rem; white-space: nowrap;">str</span>
          </div>
          <p style="margin: 0; font-size: 0.82rem; color: #aaa; line-height: 1.5;">回傳路口狀態的文字描述：<code style="background: rgba(10,186,181,0.1); padding: 1px 4px; border-radius: 3px; font-size: 0.78rem;">"none"</code>、<code style="background: rgba(10,186,181,0.1); padding: 1px 4px; border-radius: 3px; font-size: 0.78rem;">"line"</code>、<code style="background: rgba(10,186,181,0.1); padding: 1px 4px; border-radius: 3px; font-size: 0.78rem;">"junction"</code>。</p>
        </div>
        <div class="func-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(10,186,181,0.15); border-radius: 10px; padding: 16px; transition: all 0.3s ease;">
          <div style="display: flex; align-items: center; justify-content: space-between; margin-bottom: 10px;">
            <code style="color: #0abab5; font-size: 0.95rem; font-weight: bold;">line_bin_raw()</code>
            <span style="background: rgba(156,39,176,0.15); color: #ce93d8; padding: 3px 10px; border-radius: 20px; font-family: monospace; font-size: 0.75rem; white-space: nowrap;">0 ~ 255</span>
          </div>
          <p style="margin: 0; font-size: 0.82rem; color: #aaa; line-height: 1.5;">取得 8 位元二進位原始觸發遮罩。每個 bit 對應一路感測器 (bit0=ch1)，1=偵測到線。</p>
        </div>
        <div class="func-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(10,186,181,0.15); border-radius: 10px; padding: 16px; transition: all 0.3s ease;">
          <div style="display: flex; align-items: center; justify-content: space-between; margin-bottom: 10px;">
            <code style="color: #0abab5; font-size: 0.95rem; font-weight: bold;">line_on_sensors()</code>
            <span style="background: rgba(156,39,176,0.15); color: #ce93d8; padding: 3px 10px; border-radius: 20px; font-family: monospace; font-size: 0.75rem; white-space: nowrap;">list[int]</span>
          </div>
          <p style="margin: 0; font-size: 0.82rem; color: #aaa; line-height: 1.5;">回傳目前偵測到線的通道編號列表（1~8），例如 <code style="background: rgba(10,186,181,0.1); padding: 1px 4px; border-radius: 3px; font-size: 0.78rem;">[4, 5]</code>。</p>
        </div>
      </div>
    </div>
    <!-- 📦 一次讀取全部 -->
    <div style="margin-bottom: 12px;">
      <div style="font-size: 0.82rem; color: #0abab5; font-weight: bold; margin-bottom: 8px; padding-left: 2px;">📦 一次讀取全部</div>
      <div style="display: grid; grid-template-columns: 1fr; gap: 12px;">
        <div class="func-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(10,186,181,0.15); border-radius: 10px; padding: 16px; transition: all 0.3s ease;">
          <div style="display: flex; align-items: center; justify-content: space-between; margin-bottom: 10px;">
            <code style="color: #0abab5; font-size: 0.95rem; font-weight: bold;">line_read_all()</code>
            <span style="background: rgba(156,39,176,0.15); color: #ce93d8; padding: 3px 10px; border-radius: 20px; font-family: monospace; font-size: 0.75rem; white-space: nowrap;">dict</span>
          </div>
          <p style="margin: 0; font-size: 0.82rem; color: #aaa; line-height: 1.5;">一次讀取所有數據，回傳包含所有欄位的字典：<code style="background: rgba(10,186,181,0.1); padding: 1px 4px; border-radius: 3px; font-size: 0.78rem;">ir_calibration</code>、<code style="background: rgba(10,186,181,0.1); padding: 1px 4px; border-radius: 3px; font-size: 0.78rem;">line_pos8</code>、<code style="background: rgba(10,186,181,0.1); padding: 1px 4px; border-radius: 3px; font-size: 0.78rem;">line_pos100</code>、<code style="background: rgba(10,186,181,0.1); padding: 1px 4px; border-radius: 3px; font-size: 0.78rem;">line_width</code>、<code style="background: rgba(10,186,181,0.1); padding: 1px 4px; border-radius: 3px; font-size: 0.78rem;">bin_raw</code>、<code style="background: rgba(10,186,181,0.1); padding: 1px 4px; border-radius: 3px; font-size: 0.78rem;">on_sensors</code>、<code style="background: rgba(10,186,181,0.1); padding: 1px 4px; border-radius: 3px; font-size: 0.78rem;">junction</code>、<code style="background: rgba(10,186,181,0.1); padding: 1px 4px; border-radius: 3px; font-size: 0.78rem;">junction_name</code>。</p>
        </div>
      </div>
    </div>
  </div>
  <div class="pybricks-download-grid" style="display: grid; grid-template-columns: repeat(auto-fit, minmax(280px, 1fr)); gap: 25px;">
    <!-- Card 3: Block 專屬庫 -->
    <div class="download-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(0,210,255,0.15); border-radius: 12px; padding: 20px; box-shadow: 0 10px 30px rgba(0,0,0,0.3); transition: all 0.3s ease; display: flex; flex-direction: column; justify-content: space-between;">
      <div>
        <div style="width: 100%; border-radius: 8px; overflow: hidden; border: 1px solid rgba(255,255,255,0.1); background: #000; margin-bottom: 15px;">
          <img src="../images/sensors/line8/pybricks-block-with-lib.webp" alt="Block 模式 (導入專屬庫)" style="width: 100%; height: 160px; object-fit: cover; opacity: 0.85; transition: opacity 0.3s ease;" />
        </div>
        <h3 style="margin: 0 0 10px 0; color: #0abab5; font-size: 1.15rem; font-weight: bold;">🧩 Block 模式 (導入專屬庫)</h3>
        <p style="margin: 0 0 15px 0; font-size: 0.85rem; color: #aaa; line-height: 1.5;">圖形化積木結合我們為您開發的專屬庫函數，在積木開發下輕鬆取得高解析度高精度數值。</p>
      </div>
      <div class="download-btn-group">
        <a href="../examples/line8/pybricks/For%20firmware%203.6.1/line8_block_with_lib.py" target="_blank" download="line8_block_with_lib.py" data-ignore="true" class="btn-download" style="flex: 1; text-align: center; background: linear-gradient(135deg, #0abab5, #007a75); color: #fff; padding: 8px; border-radius: 6px; text-decoration: none; font-weight: bold; font-size: 0.85rem; box-shadow: 0 4px 15px rgba(10,186,181,0.25); transition: transform 0.2s ease;">📥 3.6.1 版</a>
        <a href="../examples/line8/pybricks/For%20firmware%204.0.0/line8_block_with_lib.py" target="_blank" download="line8_block_with_lib.py" data-ignore="true" class="btn-download" style="flex: 1; text-align: center; background: linear-gradient(135deg, #0abab5, #007a75); color: #fff; padding: 8px; border-radius: 6px; text-decoration: none; font-weight: bold; font-size: 0.85rem; box-shadow: 0 4px 15px rgba(10,186,181,0.25); transition: transform 0.2s ease;">📥 4.0.0 版</a>
      </div>
    </div>
    <!-- Card 4: Python 專屬庫 -->
    <div class="download-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(0,210,255,0.15); border-radius: 12px; padding: 20px; box-shadow: 0 10px 30px rgba(0,0,0,0.3); transition: all 0.3s ease; display: flex; flex-direction: column; justify-content: space-between;">
      <div>
        <div style="width: 100%; border-radius: 8px; overflow: hidden; border: 1px solid rgba(255,255,255,0.1); background: #000; margin-bottom: 15px;">
          <img src="../images/sensors/line8/pybricks-python-with-lib.webp" alt="Python 模式 (導入專屬庫)" style="width: 100%; height: 160px; object-fit: cover; opacity: 0.85; transition: opacity 0.3s ease;" />
        </div>
        <h3 style="margin: 0 0 10px 0; color: #0abab5; font-size: 1.15rem; font-weight: bold;">🐍 Python 模式 (導入專屬庫)</h3>
        <p style="margin: 0 0 15px 0; font-size: 0.85rem; color: #aaa; line-height: 1.5;">高階選手的最佳方案！導入專屬庫進行最底層的完整數據調用與高頻 PID 循線算法開發。</p>
      </div>
      <div class="download-btn-group">
        <a href="../examples/line8/pybricks/For%20firmware%203.6.1/line8_python_with_lib.py" target="_blank" download="line8_python_with_lib.py" data-ignore="true" class="btn-download" style="flex: 1; text-align: center; background: linear-gradient(135deg, #0abab5, #007a75); color: #fff; padding: 8px; border-radius: 6px; text-decoration: none; font-weight: bold; font-size: 0.85rem; box-shadow: 0 4px 15px rgba(10,186,181,0.25); transition: transform 0.2s ease;">📥 3.6.1 版</a>
        <a href="../examples/line8/pybricks/For%20firmware%204.0.0/line8_python_with_lib.py" target="_blank" download="line8_python_with_lib.py" data-ignore="true" class="btn-download" style="flex: 1; text-align: center; background: linear-gradient(135deg, #0abab5, #007a75); color: #fff; padding: 8px; border-radius: 6px; text-decoration: none; font-weight: bold; font-size: 0.85rem; box-shadow: 0 4px 15px rgba(10,186,181,0.25); transition: transform 0.2s ease;">📥 4.0.0 版</a>
      </div>
    </div>
  </div>

  <!-- Library Card (Full Width Highlighted Card) -->
  <div class="library-download-box" style="margin-top: 25px; background: rgba(10,186,181,0.03); border: 2px solid rgba(10,186,181,0.3); border-radius: 12px; padding: 25px; box-shadow: 0 15px 35px rgba(0,0,0,0.4); transition: all 0.3s ease;">
    <h3 style="margin: 0 0 15px 0; color: #0abab5; font-size: 1.3rem; font-weight: bold; text-align: center;">📦 8路循線感應器專屬 Pybricks 庫函數 (Library File)</h3>
    <div style="display: flex; flex-wrap: wrap; gap: 25px; align-items: stretch;">
      <!-- 左側：庫函數圖片與下載按鈕 -->
      <div style="flex: 1; min-width: 260px; display: flex; flex-direction: column; justify-content: space-between;">
        <div>
          <div style="border-radius: 8px; overflow: hidden; border: 1px solid rgba(10,186,181,0.2); background: #000; margin-bottom: 15px;">
            <img src="../images/sensors/line8/pybricks-lib-icon.webp" alt="專屬庫函數" style="width: 100%; height: 160px; object-fit: cover; opacity: 0.85;" />
          </div>
          <p style="font-size: 0.85rem; color: #ccc; line-height: 1.6; margin-bottom: 15px;">
            這是配合「導入專屬庫」範例不可或缺的驅動核心。請下載此庫檔案，即可完美啟用高階插值位置與 8 路光電數值精細讀取。
          </p>
        </div>
        <div class="download-btn-group">
          <a href="../examples/line8/pybricks/For%20firmware%203.6.1/MBC_line8_Lib.py" target="_blank" download="MBC_line8_Lib.py" data-ignore="true" class="btn-download-lib" style="flex: 1; text-align: center; padding: 10px 15px; background: linear-gradient(135deg, #0abab5, #00d2ff); color: #fff; text-decoration: none; border-radius: 8px; font-weight: bold; font-size: 0.95rem; box-shadow: 0 6px 20px rgba(10,186,181,0.3); transition: transform 0.2s ease, box-shadow 0.2s ease;">📥 3.6.1 版庫函數</a>
          <a href="../examples/line8/pybricks/For%20firmware%204.0.0/MBC_line8_Lib.py" target="_blank" download="MBC_line8_Lib.py" data-ignore="true" class="btn-download-lib" style="flex: 1; text-align: center; padding: 10px 15px; background: linear-gradient(135deg, #0abab5, #00d2ff); color: #fff; text-decoration: none; border-radius: 8px; font-weight: bold; font-size: 0.95rem; box-shadow: 0 6px 20px rgba(10,186,181,0.3); transition: transform 0.2s ease, box-shadow 0.2s ease;">📥 4.0.0 版庫函數</a>
        </div>
      </div>
      <!-- 右側：重要提示與截圖 -->
      <div style="flex: 1.2; min-width: 280px; display: flex;">
        <div style="background: rgba(255,107,53,0.08); border-left: 4px solid #ff6b35; border-radius: 0 8px 8px 0; padding: 15px 20px; flex-grow: 1;">
          <h4 style="margin: 0 0 8px 0; color: #ff6b35; font-size: 1.05rem; display: flex; align-items: center; gap: 6px;">⚠️ 重要提示：檔案放置位置</h4>
          <p style="margin: 0 0 15px 0; font-size: 0.85rem; color: #ddd; line-height: 1.6;">
            您<strong>必須將此庫檔案 (<code style="background: rgba(255,255,255,0.1); padding: 1px 4px; border-radius: 3px;">MBC_line8_Lib.py</code>) 與您的主程式放在同一個專案列表下</strong>。否則程式執行時將會報錯找不到庫檔案！
          </p>
          <div style="border-radius: 6px; overflow: hidden; border: 1px solid rgba(255,107,53,0.3); background: #000; box-shadow: 0 4px 15px rgba(0,0,0,0.2); width: 50%; margin: 0 auto;">
            <img src="../images/sensors/line8/pybricks-upload-lib.webp" alt="檔案放置位置" style="width: 100%; display: block; opacity: 0.9;" />
          </div>
        </div>
      </div>
    </div>
  </div>
</div>



!!! success "🚀 想要學更厲害的循線控制方法嗎？"
    可以找這幾位厲害的教練上課唷！他們有非常豐富的比賽與教學經驗，保證讓你收穫滿滿～ 💯
    
    <div style="display: flex; flex-wrap: wrap; gap: 20px; justify-content: center; margin-top: 15px;">
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



<style>
  .download-card:hover {
    transform: translateY(-5px);
    border-color: rgba(0,210,255,0.4) !important;
    box-shadow: 0 15px 35px rgba(0,210,255,0.15) !important;
    background: rgba(255,255,255,0.04) !important;
  }
  .download-card:hover img {
    opacity: 1 !important;
  }
  .library-download-box:hover {
    border-color: rgba(10,186,181,0.5) !important;
    box-shadow: 0 20px 45px rgba(10,186,181,0.2) !important;
    background: rgba(10,186,181,0.05) !important;
  }
  .btn-download:hover, .btn-download-lib:hover {
    transform: scale(1.03);
  }
  .btn-download:active, .btn-download-lib:active {
    transform: scale(0.98);
  }
  .func-card:hover {
    transform: translateY(-3px);
    border-color: rgba(0,210,255,0.35) !important;
    box-shadow: 0 8px 25px rgba(0,210,255,0.12) !important;
    background: rgba(255,255,255,0.04) !important;
  }

  /* Responsive Dual Buttons */
  .download-btn-group {
    display: flex;
    gap: 8px;
    width: 100%;
  }
  @media (max-width: 767px) {
    .download-btn-group {
      flex-direction: column;
    }
    .download-card .download-btn-group a,
    .library-download-box .download-btn-group a {
      padding: 12px 8px !important;
      font-size: 0.9rem !important;
    }
  }
</style>

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

