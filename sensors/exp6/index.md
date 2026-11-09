<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# SPIKE 專用 6 路擴充器 (EXP6)

<!-- product-hero -->

<!-- platform-grid -->


> [!WARNING]
> **⚠️ 重要限制說明**
> 請在購買前確認您的使用場景符合以下規格：
>
> | 項目 | 說明 |
> | :--- | :--- |
> | **相容主機** | SPIKE Prime；Robot Inventor（51515）只能用 Robot Inventor App 的 Python（👉 [說明](/sensors/exp6/spike/official.md)、[互動教學](/learn/exp6/robot-inventor.html)）；不支援 EV3 |
> | **SPIKE 官方軟體** | 僅支援 Python 文字模式，不支援圖控積木模式（👉 [SPIKE 新手範例：官方 App／MBC 積木網站／Pybricks 各 5 個](/sensors/exp6/spike-examples.md)） |

---

## 🚀 產品概述：突破孔位極限

SPIKE Prime 主機只有 6 個連接孔，但高階的機器人專案往往同時需要多顆感應器加上多個馬達，孔位根本不夠用。

**SPIKE 6 路擴充器 (EXP6)** 讓您用單一孔位同時連接高達 6 組感應器或馬達。這不僅僅是一個「分接頭」，而是一個內建高效能處理器的**「智慧控制器」**。它能獨立運算馬達的 PID 角度控制，大幅降低 SPIKE 主機的運算負擔；更重要的是，本產品搭載**獨立隔離供電系統**，馬達所需的大電流不會從主機抽取，**徹底解決馬達耗電導致主機當機或燒毀的致命傷！**

## 🧠 核心硬體特徵

<div class="responsive-grid-2" style="gap: 20px; margin: 25px 0;">
<div style="background: rgba(255,69,0,0.05); border: 1px solid rgba(255,69,0,0.3); border-radius: 8px; padding: 20px; display: flex; flex-direction: column; justify-content: space-between;">
<h4 style="color: #ff4500; margin-top: 0; font-size: 1.15rem; margin-bottom: 10px;">⚡ 獨立隔離供電</h4>
<p style="font-size: 0.95em; margin: 0; line-height: 1.6; color: #ccc;">馬達驅動電流完全由 EXP6 自己的 2S 鋰電池供應，與 SPIKE 主機的訊號線經過隔離晶片完全電氣隔離，保護您昂貴的主機不受異常大電流損害，保證比賽不當機。</p>
</div>
<div style="background: rgba(0,255,100,0.05); border: 1px solid rgba(0,255,100,0.3); border-radius: 8px; padding: 20px; display: flex; flex-direction: column; justify-content: space-between;">
<h4 style="color: #00ff64; margin-top: 0; font-size: 1.15rem; margin-bottom: 10px;">🧠 板載邊緣運算</h4>
<p style="font-size: 0.95em; margin: 0; line-height: 1.6; color: #ccc;">內建高效能微控制器，在背景默默為您執行 6 路馬達的 PID 絕對角度運算與馬達同步。SPIKE 主機只需下一道指令，剩下的精準控制全由擴充器代勞。</p>
</div>
<div style="background: rgba(242,194,0,0.05); border: 1px solid rgba(242,194,0,0.3); border-radius: 8px; padding: 20px; display: flex; flex-direction: column; justify-content: space-between;">
<h4 style="color: #f2c200; margin-top: 0; font-size: 1.15rem; margin-bottom: 10px;">🎯 一線控六路極速通訊</h4>
<p style="font-size: 0.95em; margin: 0; line-height: 1.6; color: #ccc;">採用特製的高速資料流協議，主機能在單一週期內接收所有 6 顆感應器的 32-bit 資料封包，實現近乎零延遲的多感測器採樣。</p>
</div>
<div style="background: rgba(0,210,255,0.05); border: 1px solid rgba(0,210,255,0.3); border-radius: 8px; padding: 20px; display: flex; flex-direction: column; justify-content: space-between;">
<h4 style="color: #00d2ff; margin-top: 0; font-size: 1.15rem; margin-bottom: 10px;">🛡️ 看門狗保護（Pybricks）</h4>
<p style="font-size: 0.95em; margin: 0; line-height: 1.6; color: #ccc;">在 Pybricks 環境使用時，擴充器約 0.5 秒沒收到主機訊號（斷線或程式當掉），就會讓馬達<b>滑行停止</b>，避免暴衝。<b>官方 SPIKE App 沒有這個功能</b>，程式最後一定要寫 <code>stop_all(1)</code> 讓馬達停止。</p>
</div>
</div>

<style>
/* 行動裝置響應式：寬度小於 768px 時自動降為單排 1x4 */
@media (max-width: 768px) {
div[style*="grid-template-columns: repeat(2, 1fr)"] {
grid-template-columns: 1fr !important;
}
}
</style>

---

## 🔌 硬體接線與啟動

1. **主機通訊連線**：將擴充器的「Host 通訊線」插入 SPIKE 主機的任一連接埠 (Port A~F)。
2. **接上 2S 鋰電池**：擴充器一定要接 **2S 鋰電池**（7.4 V，充飽 8.4 V，XT30 接頭），馬達和樂高感應器的電都來自這顆電池，不是主機。**不要接 3S 以上或變壓器**：超過 9 V 時，EXP6 會停止供電給樂高顏色／超音波感應器。
3. **連接裝置**：將您的樂高感應器（顏色、超音波、力量）與馬達，任意接入擴充器上的 1~6 號連接埠。
4. **準備寫程式！**

---

## 💡 板上燈號

**電量燈**（4 顆，由上往下是 LED1、LED2、LED3、紅燈）：電量**由下往上**亮，緊鄰紅燈的 LED3 是第 1 格。電量燈照 **2S 鋰電池**算；電壓約 4 秒平滑一次，往上升一格要再多 0.1 V，燈號不會來回跳。

| 電池電壓（2S） | 紅燈 | LED3 | LED2 | LED1（最上） |
| :--- | :--- | :--- | :--- | :--- |
| 6.9 V 以下 | 慢閃（快沒電，請充電） | 暗 | 暗 | 看通訊狀態 |
| 6.9～7.2 V | 亮 | 暗 | 暗 | 看通訊狀態 |
| 7.2～7.5 V | 暗 | 亮 | 暗 | 看通訊狀態 |
| 7.5～7.8 V | 暗 | 亮 | 亮 | 看通訊狀態 |
| 7.8 V 以上 | 暗 | 亮 | 亮 | 亮 |

**LED1 兼通訊燈**（通訊狀態優先於電量）：

- **快閃**（0.1 秒）＝等主機握手。Pybricks 要等程式跑到 `MBC_EXP6(...)` 才會連上，所以**只開機、還沒跑程式時一直快閃是正常的**。
- **慢閃**（0.5 秒）＝Pybricks 模式下超過 0.5 秒沒收到主機資料（程式結束、按了停止鍵）；這時看門狗已讓馬達滑行停止。
- 其他時候＝電量第 3 格（7.8 V 以上亮）。

**各孔燈**（6 顆，第 k 號孔亮第 k 顆）：

| 孔燈 | 意思 |
| :--- | :--- |
| 熄 | 空孔 |
| 快閃 | 跟裝置握手中 |
| 常亮 | 已連線 |
| 慢閃 | 這個孔的馬達正在執行動作 |

---

## 🎮 選擇你的軟體環境 (雙生態支援)

EXP6 擴充器最大的特色在於**「雙生態系完美支援」**。請選擇您目前使用的軟體平台，查看專屬的連線指南與 API 文件：

<div class="env-hub-grid" style="grid-template-columns: repeat(auto-fit, minmax(280px, 1fr));">
<a href="/sensors/exp6/spike-official.md" class="env-card spike" style="padding: 40px 20px;">
<div style="display: flex; gap: 20px; justify-content: center; margin-bottom: 20px; min-height: 110px; align-items: flex-end;">
<img src="/images/hubs/spike-education-app.webp" alt="SPIKE App" style="max-height: 100px; object-fit: contain; filter: drop-shadow(0 0 15px rgba(242,194,0,0.3));" />
</div>
<h3 style="font-size: 1.8rem; margin-bottom: 15px !important;">官方 SPIKE App</h3>
<p style="font-size: 1.1rem;">支援官方 Python 環境。<br>標榜同步 (Sync) 執行，簡單直覺的物件導向設計，適合入門與教學！</p>
</a>

<a href="/sensors/exp6/spike-pybricks.md" class="env-card pybricks" style="padding: 40px 20px; border-color: rgba(0,210,255,0.4);">
<div style="display: flex; justify-content: center; margin-bottom: 20px; min-height: 110px; align-items: flex-end;">
<img src="/images/hubs/spike-pybricks-logo.webp" alt="Pybricks" style="max-height: 100px; object-fit: contain; filter: drop-shadow(0 0 15px rgba(0,210,255,0.3));" />
</div>
<h3 style="font-size: 1.8rem; margin-bottom: 15px !important;">Pybricks 專業環境</h3>
<p style="font-size: 1.1rem;">為競賽與極限效能打造。<br>原生支援非同步多工併行 (Async/await) 與極速 RAW 通訊！</p>
</a>

</div>
