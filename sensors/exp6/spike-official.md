<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# 官方 SPIKE App 專區 (EXP6)

[🔙 回到 EXP6 首頁](/sensors/exp6/index.md)

<div style="display: flex; align-items: center; justify-content: center; gap: 30px; margin: 30px 0; flex-wrap: wrap;">
<div style="background: rgba(242,194,0,0.03); border: 1px solid rgba(242,194,0,0.25); border-radius: 12px; padding: 15px 35px; display: flex; align-items: center; gap: 20px; box-shadow: 0 10px 30px rgba(0,0,0,0.15);">
<img src="/images/hubs/spike-hub.webp" alt="SPIKE Prime Hub" style="max-height: 90px; object-fit: contain; filter: drop-shadow(0 0 15px rgba(242,194,0,0.35));" />
<span style="font-size: 2.2rem; color: #666; font-weight: 300; line-height: 1;">+</span>
<img src="/images/hubs/spike-education-app.webp" alt="SPIKE App" style="max-height: 90px; object-fit: contain; filter: drop-shadow(0 0 15px rgba(242,194,0,0.25));" />
</div>
</div>

> [!WARNING] 
> **請注意：您目前觀看的是 【官方 SPIKE App (Python 模式)】 教學。**
> 如果您使用的是 Pybricks 專業環境，請點擊 [這裡切換至 Pybricks 教學](/sensors/exp6/spike-pybricks.md)。

本頁適用於在 **官方 SPIKE App** (SPIKE 3) 中使用 Python 進行開發的使用者。
在官方環境中，EXP6 程式庫採用純 Python 撰寫（不支援多工作業 async），所有指令皆採用**「同步 (Sync) 且直覺的物件導向設計」**，非常適合循序漸進的程式教學。

---

## 📥 下載專屬程式庫與範例

<div style="display: flex; gap: 15px; margin-top: 15px; flex-wrap: wrap;">
<a href="/downloads/MBC_EXP6_Official_App_Lib.zip" class="btn-download" style="flex: 1; min-width: 200px; text-align: center; background: linear-gradient(135deg, #f2c200, #ff8c00); color: #fff; padding: 12px; border-radius: 8px; text-decoration: none; font-weight: bold; font-size: 1rem; box-shadow: 0 4px 15px rgba(242,194,0,0.25); transition: transform 0.2s ease; display: inline-block;">📥 下載官方版程式庫與範例 (.zip)</a>
</div>

<div style="display: flex; gap: 20px; margin: 20px 0; flex-wrap: wrap;">

<div style="flex: 1; min-width: 300px; background: rgba(76, 175, 80, 0.08); border: 1px solid rgba(76, 175, 80, 0.3); padding: 20px; border-radius: 12px;">
<h4 style="margin-top: 0; color: #4CAF50; display: flex; align-items: center; gap: 8px;">📥 安裝函式庫 (Install)</h4>
<p style="font-size: 0.95rem; line-height: 1.6; margin-bottom: 0;">在執行任何擴充板程式前，請務必先將下載包中的 <code>Install_MBC_exp6_Lib.py</code> 程式碼貼入 SPIKE App 中並執行<strong>一次</strong>。腳本會自動將核心程式庫寫入到主機韌體中。安裝成功後，您就可以在任何新專案中直接 <code>from MBC_exp6_SPIKE_App_Lib import MBC_EXP6</code>，不需再複製貼上一長串的原始碼！</p>
</div>

<div style="flex: 1; min-width: 300px; background: rgba(236, 89, 89, 0.08); border: 1px solid rgba(236, 89, 89, 0.3); padding: 20px; border-radius: 12px;">
<h4 style="margin-top: 0; color: #EC5959; display: flex; align-items: center; gap: 8px;">🗑️ 解除安裝 (Uninstall)</h4>
<p style="font-size: 0.95rem; line-height: 1.6; margin-bottom: 0;">如果未來不再需要使用擴充板，或是想要清除主機空間，請執行下載包中的 <code>Uninstall_MBC_exp6_Lib.py</code> 腳本。執行後即可將擴充板程式庫從 SPIKE 主機的內部韌體中完全且乾淨地移除。</p>
</div>

</div>

<div style="margin: 15px 0 25px 0; text-align: center;">
<img src="/images/sensors/exp6/exp6_official_app_install.webp" alt="官方軟體安裝操作教學" style="max-width: 100%; border-radius: 12px; box-shadow: 0 10px 30px rgba(0,0,0,0.3); border: 1px solid rgba(242,194,0,0.3);" />
</div>

---

## 🚀 基礎初始化

在使用任何功能前，必須先在 setup 階段建立擴充板物件。假設您的 EXP6 擴充板接在主機的 **Port F**：

<div class="category-section" style="margin: 30px 0 40px 0;">
<div class="category-header" style="display: flex; align-items: center; gap: 12px; margin-bottom: 20px; padding: 14px 22px; background: linear-gradient(135deg, rgba(242,194,0,0.08), rgba(242,194,0,0.02)); border: 1px solid rgba(242,194,0,0.25); border-radius: 10px;">
<span style="font-size: 1.6rem;">⚙️</span>
<div>
<h3 style="margin: 0; color: #f2c200; font-size: 1.2rem; font-weight: bold;">步驟一：建立連線物件</h3>
<p style="margin: 4px 0 0 0; font-size: 0.82rem; color: #999; line-height: 1.4;">在主程式中引入函式庫，並使用 port.A ~ port.F 來指定擴充板實際連接的孔位。</p>
</div>
</div>

<div style="background: #1e1e1e; border: 1px solid rgba(242,194,0,0.3); border-radius: 8px; overflow: hidden; margin-bottom: 20px;">
<div style="background: rgba(242,194,0,0.1); padding: 8px 15px; border-bottom: 1px solid rgba(242,194,0,0.2);">
<span style="color: #f2c200; font-family: monospace; font-size: 0.85rem; font-weight: bold;">Python 代碼 (官方 App 專用)</span>
</div>
<div style="padding: 15px; overflow-x: auto;">
<pre style="margin: 0; font-family: 'Consolas', monospace; font-size: 0.9rem; line-height: 1.5;"><code style="color: #d4d4d4;"><span style="color: #c586c0;">from</span> hub <span style="color: #c586c0;">import</span> port
<span style="color: #c586c0;">import</span> time
<span style="color: #c586c0;">from</span> MBC_exp6_SPIKE_App_Lib <span style="color: #c586c0;">import</span> MBC_EXP6

<span style="color: #6a9955;"># 建立擴充板連線 (假設擴充板接在主機的 Port F)</span>
exp6 = MBC_EXP6(port.F)
time.sleep(<span style="color: #b5cea8;">1</span>) <span style="color: #6a9955;"># 建議等待 1 秒讓擴充板就緒</span>

<span style="color: #6a9955;"># 開始下達馬達與感測器指令...</span>
exp6.motor_power(<span style="color: #b5cea8;">1</span>, <span style="color: #b5cea8;">50</span>)
exp6.stop_all()
</code></pre>
</div>
</div>
</div>

---

## 🎯 核心控制與讀取指令

在官方 App 中，所有的控制指令都是**同步 (Sync)** 執行的。以下是完整的 API 參考手冊。

### ⚙️ 馬達控制 (Motor Control)

控制接在 EXP6 擴充板上的 SPIKE 馬達，支援指定轉速、角度、PID 閉迴路控制、煞車以及雙馬達同步驅動。

<div class="responsive-grid-2" style="gap: 20px;">

<div class="func-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(242,194,0,0.2); border-radius: 10px; padding: 16px;">
<h4 style="color: #f2c200; margin-top: 0; margin-bottom: 10px; font-size: 1.05rem;">⚡ 啟動馬達 (Power)</h4>
<p style="font-size: 0.9em; color: #ccc;">使用純粹的電力 (開迴路 PWM) 驅動馬達。沒有速度補償，但反應最直接快速。</p>
<pre style="margin: 0; background: #1a1a1a; padding: 10px; border-radius: 6px; font-size: 0.85rem; overflow-x: auto;"><code style="color: #ccc;"><span style="color: #6a9955;"># exp6.motor_power(孔位, 動力 -100~100)</span>
exp6.motor_power(<span style="color: #b5cea8;">1</span>, <span style="color: #b5cea8;">50</span>)</code></pre>
</div>

<div class="func-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(242,194,0,0.2); border-radius: 10px; padding: 16px;">
<h4 style="color: #f2c200; margin-top: 0; margin-bottom: 10px; font-size: 1.05rem;">🔄 閉迴路轉動 (Run)</h4>
<p style="font-size: 0.9em; color: #ccc;">使用 PID 閉迴路控制，擴充板會自動根據負載調整力量，確保轉速維持在設定值。</p>
<pre style="margin: 0; background: #1a1a1a; padding: 10px; border-radius: 6px; font-size: 0.85rem; overflow-x: auto;"><code style="color: #ccc;"><span style="color: #6a9955;"># exp6.motor_run(孔位, 速度 -100~100)</span>
exp6.motor_run(<span style="color: #b5cea8;">2</span>, <span style="color: #b5cea8;">75</span>)</code></pre>
</div>

<div class="func-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(242,194,0,0.2); border-radius: 10px; padding: 16px; grid-column: 1 / -1;">
<h4 style="color: #f2c200; margin-top: 0; margin-bottom: 10px; font-size: 1.05rem;">🎯 角度精準控制 (Angle Control)</h4>
<p style="font-size: 0.9em; color: #ccc;">設定馬達旋轉特定的相對角度，或是移動到指定的絕對角度。擴充板內建的 PID 控制與角度累積器可以確保動作精準。參數 <code>stop</code> 填寫 <code>1</code>(滑行), <code>2</code>(煞車), <code>3</code>(鎖死), <code>4</code>(續轉)。</p>
<pre style="margin: 0; background: #1a1a1a; padding: 10px; border-radius: 6px; font-size: 0.85rem; overflow-x: auto;"><code style="color: #ccc;"><span style="color: #6a9955;"># 相對角度：讓孔 1 馬達以速度 30 轉動 360 度，到達後鎖死 (3)</span>
exp6.motor_run_degrees(<span style="color: #b5cea8;">1</span>, <span style="color: #b5cea8;">30</span>, <span style="color: #b5cea8;">360</span>, <span style="color: #b5cea8;">3</span>)

<span style="color: #6a9955;"># 絕對角度：讓孔 1 馬達以速度 30 轉動到 90 度的位置，到達後鎖死</span>
exp6.motor_track_target(<span style="color: #b5cea8;">1</span>, <span style="color: #b5cea8;">30</span>, <span style="color: #b5cea8;">90</span>, <span style="color: #b5cea8;">3</span>)</code></pre>
</div>

<div class="func-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(242,194,0,0.2); border-radius: 10px; padding: 16px;">
<h4 style="color: #f2c200; margin-top: 0; margin-bottom: 10px; font-size: 1.05rem;">🛑 停止與煞車 (Stop)</h4>
<p style="font-size: 0.9em; color: #ccc;">選擇讓馬達自然滑行停止，或是用力咬死煞車。</p>
<pre style="margin: 0; background: #1a1a1a; padding: 10px; border-radius: 6px; font-size: 0.85rem; overflow-x: auto;"><code style="color: #ccc;">exp6.motor_stop(<span style="color: #b5cea8;">1</span>, <span style="color: #b5cea8;">1</span>)  <span style="color: #6a9955;"># 1: 滑行</span>
exp6.motor_stop(<span style="color: #b5cea8;">1</span>, <span style="color: #b5cea8;">3</span>)  <span style="color: #6a9955;"># 3: 鎖定目前位置</span>
exp6.stop_all(<span style="color: #b5cea8;">1</span>)      <span style="color: #6a9955;"># 全域緊急滑行停止</span></code></pre>
</div>

<div class="func-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(242,194,0,0.2); border-radius: 10px; padding: 16px;">
<h4 style="color: #f2c200; margin-top: 0; margin-bottom: 10px; font-size: 1.05rem;">🚗 雙輪底盤同步 (Drive Base)</h4>
<p style="font-size: 0.9em; color: #ccc;">保證兩顆馬達精準同步啟動，不會出現一快一慢導致車子偏向的問題。</p>
<pre style="margin: 0; background: #1a1a1a; padding: 10px; border-radius: 6px; font-size: 0.85rem; overflow-x: auto;"><code style="color: #ccc;"><span style="color: #6a9955;"># 直行 (左輪 孔1, 右輪 孔2)</span>
exp6.drive(<span style="color: #b5cea8;">1</span>, <span style="color: #b5cea8;">2</span>, <span style="color: #b5cea8;">50</span>, <span style="color: #b5cea8;">50</span>)
<span style="color: #6a9955;"># 雙馬達煞車鎖定 (3)</span>
exp6.drive_stop(<span style="color: #b5cea8;">1</span>, <span style="color: #b5cea8;">2</span>, <span style="color: #b5cea8;">3</span>)</code></pre>
</div>

<div class="func-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(242,194,0,0.2); border-radius: 10px; padding: 16px; grid-column: 1 / -1;">
<h4 style="color: #f2c200; margin-top: 0; margin-bottom: 10px; font-size: 1.05rem;">📐 讀取角度與設定轉向</h4>
<p style="font-size: 0.9em; color: #ccc;">隨時讀取馬達內部的編碼器資訊，或是反轉硬體安裝方向。</p>
<pre style="margin: 0; background: #1a1a1a; padding: 10px; border-radius: 6px; font-size: 0.85rem; overflow-x: auto;"><code style="color: #ccc;">exp6.get_motor_angle(<span style="color: #b5cea8;">1</span>)     <span style="color: #6a9955;"># 累積角度</span>
exp6.get_motor_abs_angle(<span style="color: #b5cea8;">1</span>) <span style="color: #6a9955;"># 絕對角度 0~359</span>
exp6.get_motor_speed(<span style="color: #b5cea8;">1</span>)     <span style="color: #6a9955;"># 目前轉速 deg/s</span>
exp6.reset_angle(<span style="color: #b5cea8;">1</span>)         <span style="color: #6a9955;"># 累積角度歸零</span>
exp6.set_motor_inverted(<span style="color: #b5cea8;">1</span>, <span style="color: #569cd6;">True</span>) <span style="color: #6a9955;"># 將馬達方向反轉</span></code></pre>
</div>

</div>

### 📡 感測器讀取 (Sensor)

所有讀取指令都會從擴充板即時取得感測器狀態，非常適合用於回圈中做邏輯判斷。

<div class="responsive-grid-2" style="gap: 20px;">

<div class="func-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(242,194,0,0.2); border-radius: 10px; padding: 16px;">
<h4 style="color: #f2c200; margin-top: 0; margin-bottom: 10px; font-size: 1.05rem;">📏 超音波與力量感測器</h4>
<pre style="margin: 0; background: #1a1a1a; padding: 10px; border-radius: 6px; font-size: 0.85rem; overflow-x: auto;"><code style="color: #ccc;"><span style="color: #6a9955;"># 讀取前方障礙物距離 (毫米 mm)</span>
dist = exp6.get_ultrasonic_distance(<span style="color: #b5cea8;">3</span>)

<span style="color: #6a9955;"># 讀取力量感測器按壓百分比 (0~100%)</span>
force = exp6.get_touch_force(<span style="color: #b5cea8;">4</span>)</code></pre>
</div>

<div class="func-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(242,194,0,0.2); border-radius: 10px; padding: 16px;">
<h4 style="color: #f2c200; margin-top: 0; margin-bottom: 10px; font-size: 1.05rem;">🎨 顏色感測器進階讀取</h4>
<pre style="margin: 0; background: #1a1a1a; padding: 10px; border-radius: 6px; font-size: 0.85rem; overflow-x: auto;"><code style="color: #ccc;"><span style="color: #6a9955;"># 官方 0~10 顏色代碼 與 反射光</span>
color_id = exp6.get_color_color(<span style="color: #b5cea8;">5</span>)
reflection = exp6.get_color_reflection(<span style="color: #b5cea8;">5</span>)

<span style="color: #6a9955;"># RGB 三原色讀取 (0~1023)</span>
r, g, b = exp6.get_color_rgb(<span style="color: #b5cea8;">5</span>)

<span style="color: #6a9955;"># HSV 色彩空間 (色相, 飽和度, 明度)</span>
h, s, v = exp6.get_color_hsv(<span style="color: #b5cea8;">5</span>)</code></pre>
</div>

</div>

### 💓 系統心跳維持與其他功能

<div class="responsive-grid-2" style="gap: 20px;">

<div class="func-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(242,194,0,0.2); border-radius: 10px; padding: 16px;">
<h4 style="color: #f2c200; margin-top: 0; margin-bottom: 10px; font-size: 1.05rem;">💓 心跳訊號 (Heartbeat)</h4>
<p style="font-size: 0.9em; color: #ccc;">EXP6 擴充板需要與主控端維持「心跳訊號」。如果在 0.5 秒內沒有收到心跳訊號，擴充板會自動關閉馬達電力輸出以保證安全。</p>
<pre style="margin: 0; background: #1a1a1a; padding: 10px; border-radius: 6px; font-size: 0.85rem; overflow-x: auto;"><code style="color: #ccc;"><span style="color: #6a9955;"># 取代 time.sleep，等待期間自動發送心跳</span>
exp6.keep_alive_wait(<span style="color: #b5cea8;">3000</span>) <span style="color: #6a9955;"># 毫秒</span>

<span style="color: #6a9955;"># 在自行撰寫的 while 迴圈內主動維持心跳</span>
exp6.keep_alive()</code></pre>
</div>

<div class="func-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(242,194,0,0.2); border-radius: 10px; padding: 16px;">
<h4 style="color: #f2c200; margin-top: 0; margin-bottom: 10px; font-size: 1.05rem;">⚙️ 系統數據</h4>
<pre style="margin: 0; background: #1a1a1a; padding: 10px; border-radius: 6px; font-size: 0.85rem; overflow-x: auto;"><code style="color: #ccc;"><span style="color: #6a9955;"># 取得擴充板的即時電池電壓</span>
v = exp6.get_voltage()

<span style="color: #6a9955;"># 取得連接在該孔位的裝置代碼 (馬達=5)</span>
id = exp6.get_device_id(<span style="color: #b5cea8;">1</span>)</code></pre>
</div>

</div>
