<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# Pybricks 專業環境專區 (EXP6)

[🔙 回到 EXP6 首頁](/sensors/exp6/index.md)

<div style="display: flex; align-items: center; justify-content: center; gap: 30px; margin: 30px 0; flex-wrap: wrap;">
<div style="background: rgba(0,210,255,0.03); border: 1px solid rgba(0,210,255,0.25); border-radius: 12px; padding: 15px 35px; display: flex; align-items: center; gap: 20px; box-shadow: 0 10px 30px rgba(0,0,0,0.15);">
<img src="/images/hubs/spike-hub.webp" alt="SPIKE Prime Hub" style="max-height: 90px; object-fit: contain; filter: drop-shadow(0 0 15px rgba(242,194,0,0.35));" />
<span style="font-size: 2.2rem; color: #666; font-weight: 300; line-height: 1;">+</span>
<img src="/images/hubs/spike-pybricks-logo.webp" alt="Pybricks" style="max-height: 90px; object-fit: contain; filter: drop-shadow(0 0 15px rgba(0,210,255,0.25));" />
</div>
</div>

> [!WARNING] 
> **請注意：您目前觀看的是 【Pybricks 專業環境】 教學。**
> 適用於刷入 Pybricks 韌體的 SPIKE Prime。如果您使用的是官方 SPIKE App，請點擊 [這裡切換至官方教學](/sensors/exp6/spike-official.md)。

Pybricks 環境不僅提供了毫秒級的極速通訊，更完美支援**「非同步多工併行 (Async/await)」**。EXP6 的專屬庫利用此特性打造了**「背景看門狗與心跳保活機制」**，讓機器人在執行複雜任務時，永遠不會斷線。

---

## 📥 下載專屬程式庫與範例

<div style="display: flex; gap: 15px; margin-top: 15px; flex-wrap: wrap;">
<a href="/downloads/MBC_EXP6_Pybricks_Lib.zip" class="btn-download" style="flex: 1; min-width: 200px; text-align: center; background: linear-gradient(135deg, #00d2ff, #007a75); color: #fff; padding: 12px; border-radius: 8px; text-decoration: none; font-weight: bold; font-size: 1rem; box-shadow: 0 4px 15px rgba(0,210,255,0.25); transition: transform 0.2s ease; display: inline-block;">📥 下載 Pybricks 程式庫與範例 (.zip)</a>
</div>

*(注意：請將下載的 `MBC_exp6_obj_Lib.py` 放進 Pybricks 專案中，方可使用。)*

---

<div style="margin: 30px 0; text-align: center;">
<img src="/images/sensors/exp6/exp6_pybricks_example.webp" alt="Pybricks 程式介面對照圖" style="max-width: 100%; border-radius: 12px; box-shadow: 0 10px 30px rgba(0,0,0,0.3); border: 1px solid rgba(0,210,255,0.3);" />
</div>

<style>
  .code-example { background: rgba(255,255,255,0.02); border: 1px solid rgba(0,210,255,0.2); border-radius: 12px; padding: 1.5rem; margin-bottom: 2rem; }
  .code-example h4 { font-size: 1.1rem; margin-bottom: 1rem; display: flex; align-items: center; gap: 0.5rem; color: #00d2ff; }
  .code-block-wrapper { margin-bottom: 1.5rem; border-radius: 8px; overflow: hidden; background: #1e1e1e; border: 1px solid rgba(0,210,255,0.2); }
  .code-block-wrapper:last-child { margin-bottom: 0; }
  .code-header { background: rgba(0,210,255,0.1); padding: 0.5rem 1rem; font-size: 0.85rem; color: #00d2ff; display: flex; justify-content: space-between; align-items: center; border-bottom: 1px solid rgba(0,210,255,0.2); font-weight: 600; }
  .code-badge { padding: 0.2rem 0.6rem; border-radius: 4px; font-size: 0.75rem; letter-spacing: 0.5px; }
  .code-badge.sync { background: rgba(59, 130, 246, 0.2); color: #60a5fa; }
  .code-badge.async { background: rgba(139, 92, 246, 0.2); color: #c084fc; }
  .blocks { display: flex; justify-content: center; align-items: center; padding: 1.5rem; background: rgba(0,0,0,0.2); border-radius: 8px; margin-bottom: 1.5rem; overflow-x: auto; border: 1px dashed rgba(255,255,255,0.1); }
  .block-label { font-size: 0.85rem; font-weight: 700; color: #aaa; margin-bottom: 0.5rem; margin-left: 0.5rem; }
  .blocks svg .sb3-input-string, .blocks svg .sb3-input-dropdown, .blocks svg .sb-input-string, .blocks svg .sb-input-dropdown, .blocks svg .sb-obsolete.sb-darker { fill: #D9576D !important; }
  .blocks svg .sb3-input-string + text.sb3-label, .blocks svg .sb3-input-dropdown + text.sb3-label, .blocks svg .sb3-input-dropdown ~ text.sb3-label, .blocks svg .sb-input-string + text.sb-label, .blocks svg .sb-input-dropdown + text.sb-label, .blocks svg .sb-input-dropdown ~ text.sb-label { fill: #FFFFFF !important; }
  .blocks svg .sb3-dropdown-arrow, .blocks svg .sb-input-dropdown polygon { fill: #FFFFFF !important; opacity: 1 !important; }
</style>
<section id="intro">
<div style="background-color: rgba(76, 175, 80, 0.15); border-left: 4px solid #4CAF50; padding: 1rem; margin-bottom: 2rem; border-radius: 4px;">
<strong style="color: #4CAF50; font-size: 1.1rem;">⚠️ 注意：此說明文件針對 pyBricks 4.0.0 版本以上使用者</strong>
</div>
<h1>📚 1. 快速開始 (Quick Start)</h1>
<p>歡迎使用 <b>EXP6 擴充板</b>！這份說明書將教你如何使用 <code>MBC_expObj_Lib</code> 函式庫來控制馬達與讀取感測器。</p>
<p>我們的程式庫主要支援「<b>物件式（Object-Oriented）</b>」寫法，這就像是你平常拖拉圖形積木一樣直覺。此外，為了因應各種挑戰，每個指令都分為 <b>一般模式</b> 與 <b>多工模式</b> 兩種寫法：</p>
<ul>
<li><b>一般模式</b>：一行一行乖乖排隊執行，最簡單直覺，適合基礎教學。</li>
<li><b>多工模式</b>：每個指令前面必須加上 <code>await</code> 魔法單字，可以讓機器人「同時」做很多件事情，適合高階機器人。</li>
</ul>
<h3>🤖 初始化擴充板與啟動任務 (Setup &amp; Run Task)</h3>
<p>在使用任何功能前，必須先在 <code>set up</code> 階段建立擴充板物件，並根據你選擇的模式（一般/多工）決定是否需要引入 <code>run_task</code> 啟動任務。假設擴充板接在主機的 <b>Port C (孔 3)</b>：</p>
<ul>
<li><b>一般模式</b>：建立物件時將 multitask 設為 <code>false</code>，即可直接在 <code>program</code> 中開始下達控制指令。</li>
<li><b>多工模式</b>：建立物件時將 multitask 設為 <code>true</code>，並且<b>必須</b>在 <code>set up</code> 階段從 <code>MBC_exp6_obj_Lib</code> 引入擴充板專屬的 <code>run_task</code>。在圖形介面中，你只需要將程式放在 <code>program</code> 積木下方，轉成 Python 程式碼時，系統會自動將它包裝成 <code>main()</code> 並呼叫 <code>run_task(main())</code> 啟動。專屬的 <code>run_task</code> 內部除了執行程式外，還會同時啟動「背景心跳機制」來維持連線，並在程式結束時安全停機。</li>
</ul>
<div class="code-example">
<h4>初始化積木 <code>MBC_EXP6</code> 與啟動任務</h4>
<div class="block-label">一般積木</div>
<pre v-pre class="blocks">@turnRight set up :: #eeb014
from [MBC_exp6_obj_Lib] import [MBC_EXP6] :: #FF6680
@greenFlag [exp6] is a [MBC_EXP6] (3) (false :: #59C059) :: #FF6680

@turnLeft program :: #eeb014
@greenFlag [exp6 v] [motor_power] (6) (100) ‹ › :: #FF6680
@greenFlag [exp6 v] [keep_alive_wait] (5000) ‹ › :: #FF6680</pre>
<div class="code-block-wrapper">
<div class="code-header">
<span>基礎一般寫法</span>
<span class="code-badge sync">一般</span>
</div>


```python
from MBC_exp6_obj_Lib import MBC_EXP6

# Set up.
exp6 = MBC_EXP6(3, False)

exp6.motor_power(6, 100)
exp6.keep_alive_wait(5000)

```


</div>
<div class="block-label" style="margin-top: 1rem;">多工積木</div>
<pre v-pre class="blocks">@turnRight set up :: #eeb014
from [MBC_exp6_obj_Lib] import [MBC_EXP6] :: #FF6680
from [MBC_exp6_obj_Lib] import [run_task] :: #FF6680
@greenFlag [exp6] is a [MBC_EXP6] (3) (true :: #59C059) :: #FF6680

@turnLeft program :: #eeb014
@greenFlag [exp6 v] [await v] [motor_power] (6) (100) ‹ › :: #FF6680
@greenFlag [exp6 v] [await v] [keep_alive_wait] (5000) ‹ › :: #FF6680</pre>
<div class="code-block-wrapper">
<div class="code-header">
<span>進階多工寫法 (需 await)</span>
<span class="code-badge async">多工</span>
</div>


```python
from pybricks.tools import multitask, run_task, wait
from MBC_exp6_obj_Lib import MBC_EXP6, run_task

# Set up.
exp6 = MBC_EXP6(3, True)

async def main():
    await exp6.motor_power(6, 100)
    await exp6.keep_alive_wait(5000)

run_task(main())

```


</div>
</div>
</section>

<pre v-pre class="blocks">@greenFlag [exp6 v] [call v] [motor_run] (1) (30) ‹ › :: #FF6680</pre>
<div class="code-block-wrapper">
<div class="code-header"><span>一般模式</span><span class="code-badge sync">一般</span></div>


```python
# 讓擴充板孔 1 的馬達維持 30 的穩定速度轉動
exp6.motor_run(1, 30)
```


</div>
<div class="block-label" style="margin-top: 1rem;">多工積木</div>
<pre v-pre class="blocks">@greenFlag [exp6 v] [await v] [motor_run] (1) (30) ‹ › :: #FF6680</pre>
<div class="code-block-wrapper">
<div class="code-header"><span>多工模式</span><span class="code-badge async">多工</span></div>


```python
await exp6.motor_run(1, 30)
```


</div>
</div>
<!-- Angle Control -->
<h3>🎯 角度精準控制 (Angle Control)</h3>
<p>設定馬達旋轉特定的相對角度，或是移動到指定的絕對角度。擴充板內建的 PID 控制與角度累積器可以確保動作精準。</p>
<div class="code-example">
<h4><code>exp6.motor_run_degrees(port, speed, degrees, stop)</code> 與 <code>exp6.motor_track_target(port, speed, angle, stop)</code></h4>
<ul>
<li><code>port</code>: 擴充板上的孔位 (1~6)</li>
<li><code>speed</code>: 目標速度 (-100 ~ 100)。正數順時針，負數逆時針。</li>
<li><code>degrees</code>: 移動的相對角度（例如 360 就是往前轉一圈）。範圍為 -2499 ~ 2499。</li>
<li><code>angle</code>: 移動到指定的絕對角度 (0~359，會自動找最短路徑)。</li>
<li><code>stop</code>: 填寫數字決定到達目標後的行為。
&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;<ul>
<li><code>1</code>: 滑行 (STOP_COAST)</li>
<li><code>2</code>: 煞車 (STOP_BRAKE)</li>
<li><code>3</code>: 鎖死 (STOP_HOLD)</li>
<li><code>4</code>: 續轉 (STOP_CONTINUE)</li>
</ul>
</li>
</ul>
<p><i>備註：舊版 <code>motor_run_target</code> 已更名為 <code>motor_track_target</code>，但舊名仍可相容使用。</i></p>
<div class="block-label">一般積木</div>
<pre v-pre class="blocks">@greenFlag [exp6 v] [call v] [motor_run_degrees] (1) (30) (360) (3) ‹ › :: #FF6680
@greenFlag [exp6 v] [call v] [motor_track_target] (1) (30) (90) (3) ‹ › :: #FF6680</pre>
<div class="code-block-wrapper">
<div class="code-header"><span>一般模式</span><span class="code-badge sync">一般</span></div>


```python
# 讓孔 1 馬達以速度 30 轉動 360 度 (相對角度)，到達後鎖死 (HOLD)
exp6.motor_run_degrees(1, 30, 360, 3)

# 讓孔 1 馬達以速度 30 轉動到 90 度的位置 (絕對角度)，到達後鎖死
exp6.motor_track_target(1, 30, 90, 3)
```


</div>
<div class="block-label" style="margin-top: 1rem;">多工積木</div>
<pre v-pre class="blocks">@greenFlag [exp6 v] [await v] [motor_run_degrees] (1) (30) (360) (3) ‹ › :: #FF6680
@greenFlag [exp6 v] [await v] [motor_track_target] (1) (30) (90) (3) ‹ › :: #FF6680</pre>
<div class="code-block-wrapper">
<div class="code-header"><span>多工模式</span><span class="code-badge async">多工</span></div>


```python
await exp6.motor_run_degrees(1, 30, 360, 3)
await exp6.motor_track_target(1, 30, 90, 3)
```


</div>
</div>
<!-- Motor PID -->
<h3>🎛️ 設定 PID 參數 (Set PID)</h3>
<p>進階功能，用於手動調整馬達閉迴路控制的 P (比例)、I (積分)、D (微分) 參數，優化轉動的穩定性。<b>請注意：50 是系統預設的馬達控制參數基準值。</b>以 50 為基準，往上調整就是等比例放大，往下調整就是等比例縮小。</p>
<div class="code-example">
<h4><code>exp6.motor_set_pid(port, p, i, d)</code></h4>
<div class="block-label">一般積木</div>
<pre v-pre class="blocks">@greenFlag [exp6 v] [call v] [motor_set_pid] (1) (50) (50) (50) ‹ › :: #FF6680</pre>
<div class="code-block-wrapper">
<div class="code-header"><span>一般模式</span><span class="code-badge sync">一般</span></div>


```python
# 設定孔 1 的馬達 PID 參數
exp6.motor_set_pid(1, 50, 50, 50)
```


</div>
<div class="block-label" style="margin-top: 1rem;">多工積木</div>
<pre v-pre class="blocks">@greenFlag [exp6 v] [await v] [motor_set_pid] (1) (50) (50) (50) ‹ › :: #FF6680</pre>
<div class="code-block-wrapper">
<div class="code-header"><span>多工模式</span><span class="code-badge async">多工</span></div>


```python
await exp6.motor_set_pid(1, 50, 50, 50)
```


</div>
</div>
<!-- Motor Stop & Brake -->
<h3>🛑 停止與煞車 (Stop &amp; Brake)</h3>
<p>你可以選擇讓馬達自然滑行停止，或是用力咬死煞車 (鎖定維持在當前角度)。</p>
<div class="code-example">
<h4><code>exp6.motor_stop(port, stop)</code> 與 <code>exp6.stop_all(stop)</code></h4>
<ul>
<li><code>port</code>: 擴充板上的孔位 (1~6)</li>
<li><code>stop</code>: 填寫數字決定停止模式。
&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;<ul>
<li><code>1</code>: 滑行 (STOP_COAST)</li>
<li><code>2</code>: 煞車 (STOP_BRAKE)</li>
<li><code>3</code>: 鎖死 (STOP_HOLD)</li>
</ul>
</li>
</ul>
<div class="block-label">一般積木</div>
<pre v-pre class="blocks">@greenFlag [exp6 v] [call v] [motor_stop] (1) (1) ‹ › :: #FF6680
@greenFlag [exp6 v] [call v] [motor_stop] (1) (3) ‹ › :: #FF6680
@greenFlag [exp6 v] [call v] [stop_all] (1) ‹ › :: #FF6680</pre>
<div class="code-block-wrapper">
<div class="code-header"><span>一般模式</span><span class="code-badge sync">一般</span></div>


```python
exp6.motor_stop(1, 1)  # 馬達斷電，自然滑行停止
exp6.motor_stop(1, 3)   # 咬死煞車，鎖定目前位置
exp6.stop_all(1)       # 緊急停止！讓擴充板上所有馬達滑行停止
```


</div>
<div class="block-label" style="margin-top: 1rem;">多工積木</div>
<pre v-pre class="blocks">@greenFlag [exp6 v] [await v] [motor_stop] (1) (1) ‹ › :: #FF6680
@greenFlag [exp6 v] [await v] [motor_stop] (1) (3) ‹ › :: #FF6680
@greenFlag [exp6 v] [await v] [stop_all] (1) ‹ › :: #FF6680</pre>
<div class="code-block-wrapper">
<div class="code-header"><span>多工模式</span><span class="code-badge async">多工</span></div>


```python
await exp6.motor_stop(1, 1)
await exp6.motor_stop(1, 3)
await exp6.stop_all(1)
```


</div>
</div>
<!-- Motor Inverted -->
<h3>🔄 設定馬達轉向 (Set Inverted)</h3>
<p>設定馬達的預設旋轉方向。當機構設計導致馬達安裝反向時，可以直接在程式初始階段將其反轉，後續所有指令（包含底盤控制）的邏輯就會自動匹配，不需手動修改數值正負號。</p>
<div class="code-example">
<h4><code>exp6.set_motor_inverted(port, inverted)</code></h4>
<ul>
<li><code>port</code>: 擴充板上的孔位 (1~6)</li>
<li><code>inverted</code>: 布林值 (<code>True</code>/<code>False</code>)。<code>True</code> 表示反轉，預設不填為 <code>True</code>。</li>
</ul>
<div class="block-label">一般積木</div>
<pre v-pre class="blocks">@greenFlag [exp6 v] [call v] [set_motor_inverted] (1) ‹ › :: #FF6680</pre>
<div class="code-block-wrapper">
<div class="code-header"><span>一般模式</span><span class="code-badge sync">一般</span></div>


```python
# 將 1 號孔的馬達方向反轉
exp6.set_motor_inverted(1, True)
```


</div>
<div class="block-label" style="margin-top: 1rem;">多工積木</div>
<pre v-pre class="blocks">@greenFlag [exp6 v] [await v] [set_motor_inverted] (1) ‹ › :: #FF6680</pre>
<div class="code-block-wrapper">
<div class="code-header"><span>多工模式</span><span class="code-badge async">多工</span></div>


```python
await exp6.set_motor_inverted(1, True)
```


</div>
</div>
<!-- Double Motor Drive -->
<h3>🚗 雙馬達同步控制 (Drive)</h3>
<p>同時精準控制兩顆馬達，非常適合用來製作底盤輪型機器人（例如循線車）。</p>
<div class="code-example">
<h4><code>exp6.drive(left_port, right_port, left_speed, right_speed)</code></h4>
<div class="block-label">一般積木</div>
<pre v-pre class="blocks">@greenFlag [exp6 v] [call v] [drive] (1) (2) (50) (40) ‹ › :: #FF6680
@greenFlag [exp6 v] [call v] [drive_stop] (1) (2) ‹ › :: #FF6680</pre>
<div class="code-block-wrapper">
<div class="code-header"><span>一般模式</span><span class="code-badge sync">一般</span></div>


```python
# 讓孔 1 與孔 2 的馬達同時以 50 與 40 的速度前進
exp6.drive(1, 2, 50, 40)

# 停止雙馬達 (滑行停止)
exp6.drive_stop(1, 2)
```


</div>
<div class="block-label" style="margin-top: 1rem;">多工積木</div>
<pre v-pre class="blocks">@greenFlag [exp6 v] [await v] [drive] (1) (2) (50) (40) ‹ › :: #FF6680
@greenFlag [exp6 v] [await v] [drive_stop] (1) (2) ‹ › :: #FF6680</pre>
<div class="code-block-wrapper">
<div class="code-header"><span>多工模式</span><span class="code-badge async">多工</span></div>


```python
await exp6.drive(1, 2, 50, 40)
await exp6.drive_stop(1, 2)
```


</div>
</div>
</section>
<!-- SECTION 3: Sensor -->
<section id="sensor">
<h1>📡 3. 感測器讀取 (Sensors)</h1>
<p>讀取擴充板上的感測器數據與馬達狀態。擴充板會自動判斷感測器類型並回傳對應的數值。</p>
<!-- Motor State -->
<h3>📐 馬達角度與速度</h3>
<p>取得馬達當前的累積角度、絕對角度或是速度。擴充板內建 32-bit 累積器，不用擔心角度溢位問題！</p>
<ul>
<li><b>角度歸零</b>：<code>reset_angle(port)</code></li>
<li><b>讀取累積角度</b>：<code>get_motor_angle(port)</code></li>
<li><b>讀取絕對角度</b>：<code>get_motor_abs_angle(port)</code></li>
<li><b>讀取速度</b>：<code>get_motor_speed(port)</code></li>
</ul>
<div class="code-example">
<ul>
<li><code>port</code>: 擴充板上的孔位 (1~6)</li>
</ul>
<div class="block-label">一般積木</div>
<pre v-pre class="blocks">@greenFlag [exp6 v] [call v] [reset_angle] (1) ‹ › :: #FF6680
(@greenFlag [exp6 v] [call v] [get_motor_angle] (1) ‹ › :: #FF6680)
(@greenFlag [exp6 v] [call v] [get_motor_abs_angle] (1) ‹ › :: #FF6680)
(@greenFlag [exp6 v] [call v] [get_motor_speed] (1) ‹ › :: #FF6680)</pre>
<div class="code-block-wrapper">
<div class="code-header"><span>一般模式</span><span class="code-badge sync">一般</span></div>


```python
# 將孔 1 馬達的角度歸零
exp6.reset_angle(1)

# 讀取累積角度、絕對角度與速度deg/s
angle = exp6.get_motor_angle(1)
abs_angle = exp6.get_motor_abs_angle(1)
speed = exp6.get_motor_speed(1)
print("累積角度:", angle, "絕對角度:", abs_angle, "速度:", speed)
```


</div>
<div class="block-label" style="margin-top: 1rem;">多工積木</div>
<pre v-pre class="blocks">@greenFlag [exp6 v] [await v] [reset_angle] (1) ‹ › :: #FF6680
(@greenFlag [exp6 v] [await v] [get_motor_angle] (1) ‹ › :: #FF6680)
(@greenFlag [exp6 v] [await v] [get_motor_abs_angle] (1) ‹ › :: #FF6680)
(@greenFlag [exp6 v] [await v] [get_motor_speed] (1) ‹ › :: #FF6680)</pre>
<div class="code-block-wrapper">
<div class="code-header"><span>多工模式</span><span class="code-badge async">多工</span></div>


```python
await exp6.reset_angle(1)

angle = await exp6.get_motor_angle(1)
abs_angle = await exp6.get_motor_abs_angle(1)
speed = await exp6.get_motor_speed(1)
print(angle, abs_angle, speed)
```


</div>
</div>
<!-- Color Sensor -->
<h3>🎨 顏色感測器 (Color Sensor)</h3>
<p>取得 SPIKE 顏色感測器的辨識結果與反射值。</p>
<div class="code-example">
<h4>顏色代碼 <code>get_color_color(port)</code> 與 反射值 <code>get_color_reflection(port)</code></h4>
<ul>
<li><code>port</code>: 擴充板上的孔位 (1~6)</li>
</ul>
<div class="block-label">一般積木</div>
<pre v-pre class="blocks">(@greenFlag [exp6 v] [call v] [get_color_color] (3) ‹ › :: #FF6680)
(@greenFlag [exp6 v] [call v] [get_color_reflection] (3) ‹ › :: #FF6680)</pre>
<div class="code-block-wrapper">
<div class="code-header"><span>一般模式</span><span class="code-badge sync">一般</span></div>


```python
# 讀取顏色代碼（-1 為無顏色，其他對應樂高標準色碼）
color_code = exp6.get_color_color(3)

# 讀取光線反射值 (0~100，適合循線)
reflection = exp6.get_color_reflection(3)
```


</div>
<div class="block-label" style="margin-top: 1rem;">多工積木</div>
<pre v-pre class="blocks">(@greenFlag [exp6 v] [await v] [get_color_color] (3) ‹ › :: #FF6680)
(@greenFlag [exp6 v] [await v] [get_color_reflection] (3) ‹ › :: #FF6680)</pre>
<div class="code-block-wrapper">
<div class="code-header"><span>多工模式</span><span class="code-badge async">多工</span></div>


```python
color_code = await exp6.get_color_color(3)
reflection = await exp6.get_color_reflection(3)
```


</div>
</div>
<!-- Advanced Color (RGB/HSV) -->
<h3>🌈 進階色彩通道 (RGB / HSV)</h3>
<p>除了基本的顏色代碼，你也可以讀取完整的 RGB 或 HSV 數值，甚至單獨擷取其中一個通道（例如只抓取紅色或色相）。這在進階影像識別或色彩過濾時非常實用。</p>
<div class="code-example">
<h4>讀取完整 RGB 與 HSV 元組 <code>get_color_rgb(port)</code> 與 <code>get_color_hsv(port)</code></h4>
<ul>
<li><code>port</code>: 擴充板上的孔位 (1~6)</li>
</ul>
<div class="block-label">一般積木</div>
<pre v-pre class="blocks">(@greenFlag [exp6 v] [call v] [get_color_rgb] (3) ‹ › :: #FF6680)
(@greenFlag [exp6 v] [call v] [get_color_hsv] (3) ‹ › :: #FF6680)</pre>
<div class="code-block-wrapper">
<div class="code-header"><span>一般模式</span><span class="code-badge sync">一般</span></div>


```python
# 讀取 RGB，回傳 (R, G, B)，範圍為 0~100
rgb = exp6.get_color_rgb(3)

# 讀取 HSV，回傳 (H, S, V)，H 範圍 0~359，S 與 V 範圍 0~100
hsv = exp6.get_color_hsv(3)
```


</div>
<div class="block-label" style="margin-top: 1rem;">多工積木</div>
<pre v-pre class="blocks">(@greenFlag [exp6 v] [await v] [get_color_rgb] (3) ‹ › :: #FF6680)
(@greenFlag [exp6 v] [await v] [get_color_hsv] (3) ‹ › :: #FF6680)</pre>
<div class="code-block-wrapper">
<div class="code-header"><span>多工模式</span><span class="code-badge async">多工</span></div>


```python
rgb = await exp6.get_color_rgb(3)
hsv = await exp6.get_color_hsv(3)
```


</div>
</div>
<div class="code-example">
<h4>單一通道擷取 <code>get_color_red(port)</code> 等</h4>
<ul>
<li><code>port</code>: 擴充板上的孔位 (1~6)</li>
</ul>
<div class="block-label">一般積木</div>
<pre v-pre class="blocks">(@greenFlag [exp6 v] [call v] [get_color_red] (3) ‹ › :: #FF6680)
(@greenFlag [exp6 v] [call v] [get_color_hue] (3) ‹ › :: #FF6680)</pre>
<div class="code-block-wrapper">
<div class="code-header"><span>一般模式</span><span class="code-badge sync">一般</span></div>


```python
# 單獨讀取 RGB 通道 (範圍 0~100)
r = exp6.get_color_red(3)
g = exp6.get_color_green(3)
b = exp6.get_color_blue(3)

# 單獨讀取 HSV 通道 (H 範圍 0~359, S/V 範圍 0~100)
h = exp6.get_color_hue(3)
s = exp6.get_color_sat(3)
v = exp6.get_color_val(3)
```


</div>
<div class="block-label" style="margin-top: 1rem;">多工積木</div>
<pre v-pre class="blocks">(@greenFlag [exp6 v] [await v] [get_color_red] (3) ‹ › :: #FF6680)
(@greenFlag [exp6 v] [await v] [get_color_hue] (3) ‹ › :: #FF6680)</pre>
<div class="code-block-wrapper">
<div class="code-header"><span>多工模式</span><span class="code-badge async">多工</span></div>


```python
r = await exp6.get_color_red(3)
g = await exp6.get_color_green(3)
b = await exp6.get_color_blue(3)

h = await exp6.get_color_hue(3)
s = await exp6.get_color_sat(3)
v = await exp6.get_color_val(3)
```


</div>
</div>
<!-- Ultrasonic & Force -->
<h3>📏 超音波與觸碰感測器</h3>
<div class="code-example">
<h4>距離 <code>get_ultrasonic_distance(port)</code> 與 深度 <code>get_touch_force(port)</code></h4>
<ul>
<li><code>port</code>: 擴充板上的孔位 (1~6)</li>
</ul>
<div class="block-label">一般積木</div>
<pre v-pre class="blocks">(@greenFlag [exp6 v] [call v] [get_ultrasonic_distance] (4) ‹ › :: #FF6680)
(@greenFlag [exp6 v] [call v] [get_touch_force] (5) ‹ › :: #FF6680)</pre>
<div class="code-block-wrapper">
<div class="code-header"><span>一般模式</span><span class="code-badge sync">一般</span></div>


```python
# 讀取超音波距離 (單位：毫米 mm)
dist = exp6.get_ultrasonic_distance(4)

# 讀取觸碰感測器按壓深度 (0~100)
force = exp6.get_touch_force(5)
```


</div>
<div class="block-label" style="margin-top: 1rem;">多工積木</div>
<pre v-pre class="blocks">(@greenFlag [exp6 v] [await v] [get_ultrasonic_distance] (4) ‹ › :: #FF6680)
(@greenFlag [exp6 v] [await v] [get_touch_force] (5) ‹ › :: #FF6680)</pre>
<div class="code-block-wrapper">
<div class="code-header"><span>多工模式</span><span class="code-badge async">多工</span></div>


```python
dist = await exp6.get_ultrasonic_distance(4)
force = await exp6.get_touch_force(5)
```


</div>
</div>
<!-- System State & Raw -->
<h3>🔋 系統與進階狀態 (System &amp; Raw)</h3>
<p>你可以隨時檢查目前擴充板上插了什麼設備、電池電壓，或是抓取底層的 Raw Data 進行開發除錯。</p>
<div class="code-example">
<h4>取得 ID <code>get_device_id(port)</code>、Raw Data <code>get_port_raw(port)</code> 與 電壓 <code>get_voltage()</code></h4>
<ul>
<li><code>port</code>: 擴充板上的孔位 (1~6)</li>
</ul>
<div class="block-label">一般積木</div>
<pre v-pre class="blocks">(@greenFlag [exp6 v] [call v] [get_device_id] (1) ‹ › :: #FF6680)
(@greenFlag [exp6 v] [call v] [get_voltage] ‹ › :: #FF6680)
(@greenFlag [exp6 v] [call v] [get_port_raw] (1) ‹ › :: #FF6680)</pre>
<div class="code-block-wrapper">
<div class="code-header"><span>一般模式</span><span class="code-badge sync">一般</span></div>


```python
# 回傳裝置 ID: 0=無, 1=觸碰, 3=顏色, 4=超音波, 5=馬達
dev_id = exp6.get_device_id(1)

# 回傳當前電池電壓 (例如 8.25 V)
volts = exp6.get_voltage()

# 取得孔位 1 的原始未解析資料 (Raw Data, uint16)
raw_data = exp6.get_port_raw(1)
```


</div>
<div class="block-label" style="margin-top: 1rem;">多工積木</div>
<pre v-pre class="blocks">(@greenFlag [exp6 v] [await v] [get_device_id] (1) ‹ › :: #FF6680)
(@greenFlag [exp6 v] [await v] [get_voltage] ‹ › :: #FF6680)
(@greenFlag [exp6 v] [await v] [get_port_raw] (1) ‹ › :: #FF6680)</pre>
<div class="code-block-wrapper">
<div class="code-header"><span>多工模式</span><span class="code-badge async">多工</span></div>


```python
dev_id = await exp6.get_device_id(1)
volts = await exp6.get_voltage()
raw_data = await exp6.get_port_raw(1)
```


</div>
</div>
</section>

<section id="heartbeat">
<h1>💓 4. 系統心跳與維持 (Heartbeat)</h1>
<p>EXP6 擴充板需要與主控端維持通訊的「心跳訊號」。如果您在初始化時設定了 <code>multitask=True</code>，系統會在背景自動發送心跳，您不需要使用以下指令。但如果您使用一般模式 (<code>multitask=False</code>)，請務必使用以下指令來維持通訊，否則擴充板0.5秒內沒有收到心跳訊號會自動關閉馬達電力輸出。</p>
<!-- keep_alive_wait -->
<h3>⏳ 心跳等待 (Wait)</h3>
<p>在一般模式下，請使用這個指令來取代原本系統的 <code>wait</code> 或 <code>sleep</code>，它會在等待期間自動幫你維持與擴充板的通訊。</p>
<div class="code-example">
<h4><code>exp6.keep_alive_wait(ms)</code></h4>
<ul>
<li><code>ms</code>: 等待時間 (毫秒)</li>
</ul>
<div class="block-label">一般積木</div>
<pre v-pre class="blocks">@greenFlag [exp6 v] [call v] [keep_alive_wait] (1000) ‹ › :: #FF6680</pre>
<div class="code-block-wrapper">
<div class="code-header"><span>一般模式</span><span class="code-badge sync">一般</span></div>


```python
# 等待 1 秒鐘 (1000 毫秒)，期間會自動發送心跳訊號
exp6.keep_alive_wait(1000)
```


</div>
</div>
<!-- keep_alive -->
<h3>💓 手動維持心跳 (Keep Alive)</h3>
<p>如果您在程式中寫了耗時的 <code>while</code> 或 <code>for</code> 迴圈，請在迴圈內部呼叫這個指令，以確保擴充板不會因為太久沒收到通訊而斷線。</p>
<div class="code-example">
<h4><code>exp6.keep_alive()</code></h4>
<div class="block-label">一般積木</div>
<pre v-pre class="blocks">@greenFlag [exp6 v] [call v] [keep_alive] ‹ › :: #FF6680</pre>
<div class="code-block-wrapper">
<div class="code-header"><span>一般模式</span><span class="code-badge sync">一般</span></div>


```python
# 在耗時的迴圈中手動補足心跳
while True:
    # 執行一些繁重的計算或感測任務...
    
    # 確保通訊不中斷
    exp6.keep_alive()
```


</div>
</div>
</section>

<section id="advanced">
<h1>🧠 5. 進階探索 (Advanced)</h1>
<p>這部分是留給想要深入了解程式庫底層運作原理的進階開發者。</p>
<h3>函數式寫法 (Functional API)</h3>
<p>其實，<code>MBC_EXP6</code> 物件底層呼叫的是一系列全域的函數。如果你不喜歡建立物件，你可以直接引入這些函數。函數的名稱通常就是物件方法加上 <code>exp_</code> 前綴：</p>
<div class="code-example">
<h4>引入與使用函數式 API</h4>
<div class="code-block-wrapper">
<div class="code-header"><span>同步寫法</span></div>


```python
from MBC_uart_Lib import exp_init, exp_motor_power, exp_get_motor_angle

# 直接初始化 (port=3, multitask=False)
exp_init(3, False)

# 啟動孔位 5 的馬達
exp_motor_power(5, 50)

# 讀取孔位 5 的角度
angle = exp_get_motor_angle(5)

```


</div>
</div>
</section>
</div>
</section>
