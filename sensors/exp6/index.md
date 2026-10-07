<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# SPIKE 專用 6 路擴充器 (EXP6)

<!-- product-hero -->

> [!WARNING]
> **⚠️ 重要限制說明**
> 請在購買前確認您的使用場景符合以下規格：
>
> | 項目 | 說明 |
> | :--- | :--- |
> | **相容主機** | SPIKE Prime 限定（不支援 Robot Inventor 與 EV3） |
> | **SPIKE 官方軟體** | 僅支援 Python 文字模式，不支援圖控積木模式 |

---

## 🚀 產品概述：突破孔位極限

SPIKE Prime 主機只有 6 個連接孔，但高階的機器人專案往往同時需要多顆感應器加上多個馬達，孔位根本不夠用。

**SPIKE 6 路擴充器 (EXP6)** 讓您用單一孔位同時連接高達 6 組感應器或馬達。這不僅僅是一個「分接頭」，而是一個內建高效能處理器的**「智慧控制器」**。它能獨立運算馬達的 PID 角度控制，大幅降低 SPIKE 主機的運算負擔；更重要的是，本產品搭載**獨立隔離供電系統**，馬達所需的大電流不會從主機抽取，**徹底解決馬達耗電導致主機當機或燒毀的致命傷！**

## 🧠 核心硬體特徵

<div class="responsive-grid-2" style="gap: 20px; margin: 25px 0;">
<div style="background: rgba(255,69,0,0.05); border: 1px solid rgba(255,69,0,0.3); border-radius: 8px; padding: 20px; display: flex; flex-direction: column; justify-content: space-between;">
<h4 style="color: #ff4500; margin-top: 0; font-size: 1.15rem; margin-bottom: 10px;">⚡ 獨立隔離供電</h4>
<p style="font-size: 0.95em; margin: 0; line-height: 1.6; color: #ccc;">馬達驅動電流完全由獨立的電池盒/變壓器供應，與 SPIKE 主機進行 100% 物理光耦隔離，保護您昂貴的主機不受異常大電流損害，保證比賽不當機。</p>
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
<h4 style="color: #00d2ff; margin-top: 0; font-size: 1.15rem; margin-bottom: 10px;">🛡️ 智慧看門狗保護</h4>
<p style="font-size: 0.95em; margin: 0; line-height: 1.6; color: #ccc;">硬體級 Watchdog 保護機制，一旦與主機斷線或程式崩潰，擴充器會自動煞車鎖死馬達防暴衝，給高階機甲競賽最穩固的安全後盾。</p>
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
2. **接入獨立電源**：擴充器必須接上額外的獨立電源（電池盒或變壓器），此電源將專門供電給馬達。
3. **連接裝置**：將您的樂高感應器（顏色、超音波、力量）與馬達，任意接入擴充器上的 1~6 號連接埠。
4. **準備寫程式！**

---

## 🎮 選擇你的軟體環境 (雙生態支援)

EXP6 擴充器最大的特色在於**「雙生態系完美支援」**。請選擇您目前使用的軟體平台，查看專屬的連線指南與 API 文件：

<div class="env-hub-grid" style="grid-template-columns: repeat(auto-fit, minmax(280px, 1fr));">
<a href="#/sensors/exp6/spike-official" class="env-card spike" style="padding: 40px 20px;">
<div style="display: flex; gap: 20px; justify-content: center; margin-bottom: 20px; min-height: 110px; align-items: flex-end;">
<img src="/images/hubs/spike-education-app.webp" alt="SPIKE App" style="max-height: 100px; object-fit: contain; filter: drop-shadow(0 0 15px rgba(242,194,0,0.3));" />
</div>
<h3 style="font-size: 1.8rem; margin-bottom: 15px !important;">官方 SPIKE App</h3>
<p style="font-size: 1.1rem;">支援官方 Python 環境。<br>標榜同步 (Sync) 執行，簡單直覺的物件導向設計，適合入門與教學！</p>
</a>

<a href="#/sensors/exp6/spike-pybricks" class="env-card pybricks" style="padding: 40px 20px; border-color: rgba(0,210,255,0.4);">
<div style="display: flex; justify-content: center; margin-bottom: 20px; min-height: 110px; align-items: flex-end;">
<img src="/images/hubs/spike-pybricks-logo.webp" alt="Pybricks" style="max-height: 100px; object-fit: contain; filter: drop-shadow(0 0 15px rgba(0,210,255,0.3));" />
</div>
<h3 style="font-size: 1.8rem; margin-bottom: 15px !important;">Pybricks 專業環境</h3>
<p style="font-size: 1.1rem;">為競賽與極限效能打造。<br>原生支援非同步多工併行 (Async/await) 與極速 RAW 通訊！</p>
</a>
</div>
