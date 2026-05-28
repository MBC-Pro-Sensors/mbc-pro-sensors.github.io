# LEGO® MINDSTORMS® Robot Inventor

[🔙 回到 SPIKE 主機選單](/sensors/line8/spike-hub.md)

<div style="display: flex; align-items: center; justify-content: center; gap: 30px; margin: 30px 0; flex-wrap: wrap;">
  <div style="background: rgba(10,186,181,0.03); border: 1px solid rgba(10,186,181,0.25); border-radius: 12px; padding: 15px 35px; display: flex; align-items: center; gap: 20px; box-shadow: 0 10px 30px rgba(0,0,0,0.15);">
    <img src="../images/hubs/inventor-hub.webp" alt="MINDSTORMS Hub" style="max-height: 90px; object-fit: contain; filter: drop-shadow(0 0 15px rgba(10,186,181,0.35));" />
    <span style="font-size: 2.2rem; color: #666; font-weight: 300; line-height: 1;">+</span>
    <img src="../images/hubs/spike-mindstorms-app.webp" alt="LEGO MINDSTORMS Inventor App" style="max-height: 90px; object-fit: contain; filter: drop-shadow(0 0 15px rgba(10,186,181,0.15));" />
  </div>
</div>

> [!WARNING] 
> **請注意：您目前觀看的是 【LEGO® MINDSTORMS® Robot Inventor】 環境的教學。**
> 如果您使用的是 EV3 或是其他系統軟體，請點擊上方按鈕回到主機選單重新選擇。



本頁適用於 **LEGO® MINDSTORMS® Robot Inventor**，搭載 **LEGO MINDSTORMS Inventor App** 積木圖控模式。

在此環境下，循行者 8 路感應器會被樂高系統完美識別為官方的**「SPIKE 顏色感應器」**。

> [!CAUTION] 
> **⚠️ 智慧型主機 (51515 MINDSTORMS® Hub) 的官方軟體限制**
> 由於樂高官方 Robot Inventor App 軟體的限制，部分單一通道積木的數值**被官方嚴格限制在 `0 ~ 126`**。為了讓您能在這台主機上依然發揮循行者 8 路的極限效能，我們研發了獨家的「數據分流與壓榨」傳輸協定，將位置與反射光大數據成功繞過 126 限制，直接映射在 RGB 與反射光通道上！

---

## 📊 數據實體通道對照表

您在程式中可以直接拉取顏色感應器的 **「反射光」** 或 **「RGB 數值」** 積木來獲得以下數據：

<div class="custom-table-wrapper" style="overflow-x: auto; margin: 25px 0; border: 1px solid rgba(10,186,181,0.25); border-radius: 12px; box-shadow: 0 10px 30px rgba(0,0,0,0.3); background: #f8f9fa;">
  <table style="width: 100%; border-collapse: collapse; text-align: left; font-size: 0.95rem;">
    <thead>
      <tr style="background: rgba(10,186,181,0.12); border-bottom: 2px solid rgba(10,186,181,0.25);">
        <th style="padding: 15px 20px; color: #007a75; font-weight: bold; width: 25%;">官方積木通道</th>
        <th style="padding: 15px 20px; color: #007a75; font-weight: bold; width: 25%;">映射物理數據</th>
        <th style="padding: 15px 20px; color: #007a75; font-weight: bold; width: 50%;">數值範圍與解碼說明</th>
      </tr>
    </thead>
    <tbody>
      <tr class="table-hover-row" style="border-bottom: 1px solid rgba(0,0,0,0.06); background: rgba(0,0,0,0.01); transition: background 0.2s ease;">
        <td style="padding: 18px 20px; font-weight: bold; color: #111; vertical-align: middle;">🎨 顏色 (Color)</td>
        <td style="padding: 18px 20px; font-weight: bold; color: #333; vertical-align: middle;">特徵線寬</td>
        <td style="padding: 18px 20px; color: #444; line-height: 1.6; vertical-align: middle;">
          <span style="display: inline-block; background: rgba(10,186,181,0.15); color: #007a75; padding: 2px 8px; border-radius: 4px; font-family: monospace; font-size: 0.85rem; margin-bottom: 6px; border: 1px solid rgba(10,186,181,0.2);">0 ~ 8</span><br>
          👉 由於家用版軟體限制顏色最大只能到 10，此處特徵線寬正常在 <code>0 ~ 8</code> 之間，常用於判斷十字路口。
        </td>
      </tr>
      <tr class="table-hover-row" style="border-bottom: 1px solid rgba(0,0,0,0.06); background: rgba(0,0,0,0.01); transition: background 0.2s ease;">
        <td style="padding: 18px 20px; font-weight: bold; color: #111; vertical-align: middle;">💡 反射光 (Reflected Light)</td>
        <td style="padding: 18px 20px; font-weight: bold; color: #333; vertical-align: middle;">標準位置平移值</td>
        <td style="padding: 18px 20px; color: #444; line-height: 1.6; vertical-align: middle;">
          <span style="display: inline-block; background: rgba(217,163,0,0.15); color: #b88600; padding: 2px 8px; border-radius: 4px; font-family: monospace; font-size: 0.85rem; margin-bottom: 6px; border: 1px solid rgba(217,163,0,0.2);">0 ~ 16</span><br>
          👉 <code>8</code> 代表正中央，<code>0</code> 代表最左，<code>16</code> 代表最右。數值讀取後 -8 即為循線位置（<strong>數值為負代表黑線在左邊，數值為正代表黑線在右邊</strong>），數值穩定，完全不受 126 限制。
        </td>
      </tr>
      <tr class="table-hover-row" style="border-bottom: 1px solid rgba(0,0,0,0.06); background: rgba(0,0,0,0.01); transition: background 0.2s ease;">
        <td style="padding: 18px 20px; font-weight: bold; color: #111; vertical-align: middle;">🔴 紅色通道 (Red)</td>
        <td style="padding: 18px 20px; font-weight: bold; color: #333; vertical-align: middle;">高解析平滑位置</td>
        <td style="padding: 18px 20px; color: #444; line-height: 1.6; vertical-align: middle;">
          <span style="display: inline-block; background: rgba(255,107,53,0.15); color: #d32f2f; padding: 2px 8px; border-radius: 4px; font-family: monospace; font-size: 0.85rem; margin-bottom: 6px; border: 1px solid rgba(255,107,53,0.2);">0 ~ 200</span><br>
          👉 <code>100</code> 代表正中央，<code>0</code> 代表極左，<code>200</code> 代表極右。數值讀取後 -100 即為高精度循線位置（<strong>數值為負代表黑線在左邊，數值為正代表黑線在右邊</strong>），此通道在玩具版上<strong>完美無損</strong>，無任何縮水，可以直接帶入高階 PID 循線控制！
        </td>
      </tr>
      <tr class="table-hover-row" style="border-bottom: 1px solid rgba(0,0,0,0.06); background: rgba(0,0,0,0.01); transition: background 0.2s ease;">
        <td style="padding: 18px 20px; font-weight: bold; color: #111; vertical-align: middle;">🟢 綠色數值 (Green)</td>
        <td style="padding: 18px 20px; font-weight: bold; color: #333; vertical-align: middle;">前 4 顆光電數值</td>
        <td style="padding: 18px 20px; color: #444; line-height: 1.6; vertical-align: middle;">
          <span style="display: inline-block; background: rgba(0,180,216,0.15); color: #0077b6; padding: 2px 8px; border-radius: 4px; font-family: monospace; font-size: 0.85rem; margin-bottom: 6px; border: 1px solid rgba(0,180,216,0.2);">0 ~ 255</span><br>
          👉 <strong>前四顆光電數值</strong>：為每 2-bit 代表一個光電數值，數值範圍 <code>0 ~ 3</code>。
        </td>
      </tr>
      <tr class="table-hover-row" style="border: none; background: rgba(0,0,0,0.01); transition: background 0.2s ease;">
        <td style="padding: 18px 20px; font-weight: bold; color: #111; vertical-align: middle;">🔵 藍色數值 (Blue)</td>
        <td style="padding: 18px 20px; font-weight: bold; color: #333; vertical-align: middle;">後 4 顆光電數值</td>
        <td style="padding: 18px 20px; color: #444; line-height: 1.6; vertical-align: middle;">
          <span style="display: inline-block; background: rgba(0,180,216,0.15); color: #0077b6; padding: 2px 8px; border-radius: 4px; font-family: monospace; font-size: 0.85rem; margin-bottom: 6px; border: 1px solid rgba(0,180,216,0.2);">0 ~ 255</span><br>
          👉 <strong>後四顆光電數值</strong>：為每 2-bit 代表一個光電數值，數值範圍 <code>0 ~ 3</code>。
        </td>
      </tr>
    </tbody>
  </table>
</div>

<style>
  .table-hover-row:hover {
    background: rgba(0,0,0,0.04) !important;
  }
</style>

---

## <a href="../examples/line8/spike/line8-ri-demo.lms" target="_blank" data-ignore="true" download>📥 範例程式下載</a>

<div style="text-align: center; margin: 25px 0;">
  <img src="../images/sensors/line8/spike-ri-example.webp" alt="LEGO® MINDSTORMS® Robot Inventor App 範例程式圖控積木範例" style="max-width: 100%; border-radius: 8px; border: 1px solid rgba(10,186,181,0.25); box-shadow: 0 10px 30px rgba(0,0,0,0.5); filter: drop-shadow(0 0 15px rgba(10,186,181,0.15));" />
  <p style="font-size: 0.85rem; color: #888; margin-top: 10px;">💡 上圖為 8路循線感應器於 LEGO® MINDSTORMS® Robot Inventor App 中的經典 PID 循線程式結構示範</p>
</div>

* <a href="../examples/line8/spike/line8-ri-demo.lms" target="_blank" download="line8-ri-demo.lms" style="color: #0abab5; font-weight: bold; text-decoration: none;">📥 Robot Inventor 範例 (.lms)</a>

> 下載後用 LEGO® MINDSTORMS® Robot Inventor App 開啟即可使用。
