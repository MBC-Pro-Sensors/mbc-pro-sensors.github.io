# 循行者16路感應器 (LineSensor16)

<div style="text-align: center; margin-bottom: 2rem;">
  <span style="display:inline-block; background:#ff6b35; color:#fff; padding:3px 12px; border-radius:20px; font-family:monospace; margin-bottom:1rem; font-size: 0.85rem;">LNS-16-PRO · 競賽級</span>
  
  <div style="display: flex; flex-wrap: wrap; align-items: center; justify-content: center; gap: 30px; margin: 20px 0;">
    <div style="flex: 1; min-width: 250px; text-align: center;">
      <img src="../images/sensors/line16/line16-product-spike.webp" alt="循行者16路感應器" style="max-width: 280px; width: 100%; display: block; margin: 0 auto; filter: drop-shadow(0 0 20px rgba(10,186,181,0.65)) drop-shadow(0 0 40px rgba(10,186,181,0.35));" />
    </div>
    <div style="flex: 1; min-width: 300px; max-width: 480px; aspect-ratio: 16/9; background: #000; border-radius: 12px; overflow: hidden; border: 1px solid rgba(10,186,181,0.2); box-shadow: 0 10px 25px rgba(0,0,0,0.5);">
      <iframe src="https://www.youtube.com/embed/MuG9kp2-8FY" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" allowfullscreen loading="lazy" style="width: 100%; height: 100%; border: none;"></iframe>
    </div>
  </div>

  <p style="font-family: monospace; color: #aaa; letter-spacing: 1px;">
    <strong>16 通道高密度光反射陣列</strong><br>
    一鍵校準 · 專款專用 (分 SPIKE / EV3 / I2C 版本) · 循線競賽終極方案
  </p>
</div>

> [!IMPORTANT]
> **⚠️ 購買與相容性重要聲明 (本感應器非跨系統通用！)**
> 本感應器為**「專款專用」**硬體設計，出廠時已針對特定系統進行物理插座與電氣協議的封裝，**不同主機版本之間並不互通**：
> 1. **🧱 SPIKE 專屬版**：採用 LPF2 (Lego 6-pin) 扁平插座，僅相容於 SPIKE Prime, SPIKE Essential 與 Robot Inventor 主機。
> 2. **🧱 EV3 專屬版**：採用 RJ12 (樂高專用 6-pin 水晶頭) 插座，僅相容於 EV3 主機。
> 3. **∿ 通用 I2C 開發版**：採用標準排針/XH2.54 通訊埠，僅適用於 Arduino、ESP32、樹莓派等自行開發系統。
> *請務必依據您的主機系統選購對應的硬體版本。各版本物理接頭完全不同，請勿強行連接以免造成硬體損壞！*

---

## 🚀 產品概述 與 硬體優勢

MBC 16路循線感應器是一款專為樂高 (LEGO) 及各式高階機器人競賽設計的旗艦級第三方感應器。
* **🚀 極速競賽級嵌入式核心**：板載 high-frequency 智慧處理晶片，運算效能強勁。專為毫秒級的高頻控制所設計，能在極瞬時間內完成 16 通道的海量數據濾波與幾何位置解算，為機器人注入頂級大腦。
* **⚡ 零延遲平行採樣架構 (Zero-Latency Sampling)**：採用領先的硬體級資料並行通道技術， 16 路訊號在底層背景獨立、高速採樣，完全不佔用主機通訊帶寬與執行緒，確保數據刷新零延遲，指令隨傳隨到。
* **📐 高密度幾何光學佈局**：精密排布 16 組超靈敏紅外線收發陣列（右至左通道 0 ~ 15）。超高密度的物理布局，能完美捕捉賽道上的所有微小弧度變化與交錯線條，提供最細緻的賽道還原度。

## 🧠 核心資料特徵與演算法

為了讓您的循線車達到比賽級的極致平滑與穩定，底層硬體會將 16 路原始反射訊號在背景進行即時校準、二值化與連續性濾波，產出以下四個核心數據特徵：

<div class="responsive-grid-2" style="gap: 20px; margin: 25px 0;">
  <div style="background: rgba(10,186,181,0.05); border: 1px solid rgba(10,186,181,0.3); border-radius: 8px; padding: 20px; display: flex; flex-direction: column; justify-content: space-between;">
    <h4 style="color: var(--theme-color); margin-top: 0; font-size: 1.15rem; margin-bottom: 10px;">📊 絕對物理光值 (0 ~ 100)</h4>
    <p style="font-size: 0.95em; margin: 0; line-height: 1.6; color: #ccc;">反應最真實的物理反射光強度。<strong>100 代表純白高反光，0 代表純黑無反光</strong>。此數值保持物理真實，不隨黑白線模式切換而反轉，非常適合開發即時長條圖視覺化儀表板。</p>
  </div>
  <div style="background: rgba(242,194,0,0.05); border: 1px solid rgba(242,194,0,0.3); border-radius: 8px; padding: 20px; display: flex; flex-direction: column; justify-content: space-between;">
    <h4 style="color: var(--accent-color); margin-top: 0; font-size: 1.15rem; margin-bottom: 10px;">🎯 次像素高解析平滑位置 (0 ~ 200)</h4>
    <p style="font-size: 0.95em; margin: 0; line-height: 1.6; color: #ccc;">透過相鄰感測器的連續反射強度進行高精度幾何差值運算，輸出 <strong>0 ~ 200 的超平滑連續位置 (100 為正中央)</strong>，專供高階 PID 控制演算法使用，能徹底消除車體微小抖動。</p>
  </div>
  <div style="background: rgba(255,107,53,0.05); border: 1px solid rgba(255,107,53,0.3); border-radius: 8px; padding: 20px; display: flex; flex-direction: column; justify-content: space-between;">
    <h4 style="color: #ff6b35; margin-top: 0; font-size: 1.15rem; margin-bottom: 10px;">📐 特徵線寬 與 二值化矩陣</h4>
    <p style="font-size: 0.95em; margin: 0; line-height: 1.6; color: #ccc;">指出目前觸發的感測器數量（即線寬，可用於精準判斷十字路口、丁字路口或直角），以及 16 顆感測器是否觸發的 0 與 1 二值化狀態矩陣，方便還原精細軌跡。</p>
  </div>
  <div style="background: rgba(0,210,255,0.05); border: 1px solid rgba(0,210,255,0.3); border-radius: 8px; padding: 20px; display: flex; flex-direction: column; justify-content: space-between;">
    <h4 style="color: #00d2ff; margin-top: 0; font-size: 1.15rem; margin-bottom: 10px;">🛡️ 邊界防禦與防掉線記憶</h4>
    <p style="font-size: 0.95em; margin: 0; line-height: 1.6; color: #ccc;">系統會即時監控最左側與最右側感應器的物理光強度，能極速捕捉賽道分岔或車頭意外衝出軌跡，搭配方向記憶演算法，實現高速過彎不脫線！</p>
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

## 🔌 硬體接線

就像連接樂高感應器一樣！循行者使用標準的連接線，直接插上 SPIKE 或 EV3 主機，跟接樂高感應器完全一樣。

1. **找到連接孔**：感應器上的連接孔在側邊，跟樂高感應器一樣。
2. **拿出連接線**：使用內附的標準連接線（或樂高原廠線）。
3. **插入主機**：SPIKE 插任一 **A 至 F 連接埠**，EV3 插 **1 至 4 號連接埠**。
4. **確認燈號**：通電後感應器上的綠色心跳指示燈 (靠近插座) 會開始閃爍。

> **⚠ 小提醒**：接線時不要用力硬插！連接頭有方向性，卡榫朝上，輕輕推進去就好。

---

## 🎯 校準教學

每一種地面（白紙、木板、比賽場地圖）反射的光量都不同。校準就是「讓感應器認識你的場地」，告訴它：「這個亮度是白色地面，那個亮度是黑色線。」

### 📺 官方一鍵校準影音教學
為了提供最直覺的操作指南，建議您觀看下方的實體校準操作示範影片：

<div style="max-width: 480px; aspect-ratio: 16/9; margin: 20px auto; background: #000; border-radius: 12px; overflow: hidden; border: 1px solid rgba(10,186,181,0.2); box-shadow: 0 10px 25px rgba(0,0,0,0.5);">
  <iframe src="https://www.youtube.com/embed/TKi7b20x8UY" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" allowfullscreen loading="lazy" style="width: 100%; height: 100%; border: none;"></iframe>
</div>

### 校準步驟（超簡單！）

1. **按下校準按鈕**：按住感應器上的校準按鈕 **0.5 秒**，直到 **🟢 綠色校準指示燈 (靠近校準按鈕)** 保持亮起。
2. **開始掃描**：指示燈亮起後，將感應器在場地黑線與白色區塊上來回移動。
3. **保持高度**：感應器距離地面 **0.5 ~ 1.2 公分**，不要太高也不要貼地。
4. **等待完成**：5 秒後 **🟢 綠色校準指示燈 (靠近校準按鈕)** 熄滅，校準即完成！數值已自動儲存。

> **⚠ 校準注意事項**
> - 掃描時要同時覆蓋 **最黑的黑線** 和 **最白的白色區域**。
> - 高度請維持在 0.5 ~ 1.2 公分之間，太高會讀不準。
> - 校準完成後，數值會自動存入感應器記憶體，下次開機不用重做（除非換場地）。

---

## 🚦 硬體指示燈與按鈕

為了保證極佳的視覺閱讀對比度，並適應不同底色主題，我們為感應器狀態元件設計了色彩清晰、對比極高的高規格狀態看板。**請注意：心跳燈與校準燈是物理上分開的兩個獨立指示燈！**

<div style="display: flex; flex-direction: column; gap: 12px; margin: 20px 0;">
  <div style="display: flex; align-items: center; gap: 15px; background: rgba(0, 255, 100, 0.06); border: 1px solid rgba(0, 255, 100, 0.25); border-radius: 8px; padding: 12px 15px;">
    <span style="font-size: 1.5rem; filter: drop-shadow(0 0 5px rgba(0,255,100,0.5)); line-height: 1;">🟢</span>
    <div style="flex: 1;">
      <strong style="color: #00ff64; font-size: 1.05rem;">綠色心跳指示燈 (靠近插座)</strong>
      <div style="color: #ddd; font-size: 0.95rem; margin-top: 4px; line-height: 1.5;">
        <strong>心跳閃爍</strong>：每秒閃爍一次，表示感應器供電正常且系統正常運作中。
      </div>
    </div>
  </div>

  <div style="display: flex; align-items: center; gap: 15px; background: rgba(0, 255, 100, 0.06); border: 1px solid rgba(0, 255, 100, 0.25); border-radius: 8px; padding: 12px 15px;">
    <span style="font-size: 1.5rem; filter: drop-shadow(0 0 5px rgba(0,255,100,0.5)); line-height: 1;">🟢</span>
    <div style="flex: 1;">
      <strong style="color: #00ff64; font-size: 1.05rem;">綠色校準指示燈 (靠近校準按鈕)</strong>
      <div style="color: #ddd; font-size: 0.95rem; margin-top: 4px; line-height: 1.5;">
        <strong>校準常亮</strong>：按住按鈕 0.5 秒後此綠燈會持續亮起 5 秒，代表校準進行中；熄滅表示校準順利完成。
      </div>
    </div>
  </div>

  <div style="display: flex; align-items: center; gap: 15px; background: rgba(255, 50, 50, 0.06); border: 1px solid rgba(255, 50, 50, 0.25); border-radius: 8px; padding: 12px 15px;">
    <span style="font-size: 1.5rem; filter: drop-shadow(0 0 5px rgba(255,50,50,0.5)); line-height: 1;">🔴</span>
    <div style="flex: 1;">
      <strong style="color: #ff4a4a; font-size: 1.05rem;">紅色指示燈 (通訊除錯燈)</strong>
      <div style="color: #ddd; font-size: 0.95rem; margin-top: 4px; line-height: 1.5;">
        <strong>正常狀態</strong>：熄滅。<br>
        <strong>紅色 LED 燈長亮狀態</strong>：表示與主控板通訊失敗。請檢查連接線是否插緊，或更換主機連接埠 / 連接線進行交叉測試。
      </div>
    </div>
  </div>

  <div style="display: flex; align-items: center; gap: 15px; background: rgba(242, 194, 0, 0.06); border: 1px solid rgba(242, 194, 0, 0.25); border-radius: 8px; padding: 12px 15px;">
    <span style="font-size: 1.5rem; filter: drop-shadow(0 0 5px rgba(242,194,0,0.5)); line-height: 1;">🔘</span>
    <div style="flex: 1;">
      <strong style="color: #f2c200; font-size: 1.05rem;">一鍵校準按鈕</strong>
      <div style="color: #ddd; font-size: 0.95rem; margin-top: 4px; line-height: 1.5;">
        按住 <strong>0.5 秒</strong> 放開，即可進入自動來回校準模式（綠色校準指示燈持續亮起 5 秒）。
      </div>
    </div>
  </div>

  <div style="display: flex; align-items: center; gap: 15px; background: rgba(0, 210, 255, 0.06); border: 1px solid rgba(0, 210, 255, 0.25); border-radius: 8px; padding: 12px 15px;">
    <span style="font-size: 1.5rem; filter: drop-shadow(0 0 5px rgba(0,210,255,0.5)); line-height: 1;">💡</span>
    <div style="flex: 1;">
      <strong style="color: #00d2ff; font-size: 1.05rem;">板載 16 路狀態 LED 陣列</strong>
      <div style="color: #ddd; font-size: 0.95rem; margin-top: 4px; line-height: 1.5;">
        <strong>即時顯示觸發狀態</strong>：燈亮 = 該通道偵測到黑線（循黑模式），可極其直觀地在車底看到黑線相對通道的絕對位置。
      </div>
    </div>
  </div>
</div>

---

## 🎮 選擇你的主機

請先選擇您使用的控制器系統，進入後再選擇您的軟體環境：

<div class="env-hub-grid" style="grid-template-columns: repeat(auto-fit, minmax(280px, 1fr));">
  <a href="#/sensors/line16/spike-hub" class="env-card spike" style="padding: 40px 20px;">
    <div style="display: flex; gap: 20px; justify-content: center; margin-bottom: 20px; min-height: 110px; align-items: flex-end;">
      <img src="../images/hubs/spike-hub.webp" alt="SPIKE Prime Hub" style="max-height: 100px; object-fit: contain; filter: drop-shadow(0 0 15px rgba(242,194,0,0.3));" />
      <img src="../images/hubs/inventor-hub.webp" alt="Robot Inventor Hub" style="max-height: 100px; object-fit: contain; filter: drop-shadow(0 0 15px rgba(10,186,181,0.3));" />
    </div>
    <h3 style="font-size: 1.8rem; margin-bottom: 15px !important;">🧱 SPIKE 系列主機</h3>
    <p style="font-size: 1.1rem;">教育版 (SPIKE 3) / 家用版 (Robot Inventor) / pyBricks</p>
  </a>
  
  <a href="#/sensors/line16/ev3-hub" class="env-card ev3" style="padding: 40px 20px;">
    <div style="display: flex; justify-content: center; margin-bottom: 20px; min-height: 110px; align-items: flex-end;">
      <img src="../images/hubs/ev3-hub.webp" alt="EV3 Hub" style="max-height: 100px; object-fit: contain; filter: drop-shadow(0 0 15px rgba(10,186,181,0.3));" />
    </div>
    <h3 style="font-size: 1.8rem; margin-bottom: 15px !important;">🧱 EV3 系列主機</h3>
    <p style="font-size: 1.1rem;">官方軟體 / Classroom / clev3r / pyBricks</p>
  </a>

  <a href="#/sensors/line16/arduino-i2c" class="env-card mcu" style="padding: 40px 20px;">
    <div style="display: flex; justify-content: center; margin-bottom: 20px; min-height: 110px; align-items: flex-end;">
      <svg viewBox="0 0 100 40" style="width: 120px; height: 50px; fill: none; stroke: #00d2ff; stroke-width: 4; stroke-linecap: round; stroke-linejoin: round; filter: drop-shadow(0 0 15px rgba(0, 210, 255, 0.6));">
        <path d="M 5,25 H 25 V 5 H 45 V 35 H 65 V 5 H 85 V 35 H 95" />
      </svg>
    </div>
    <h3 style="font-size: 1.8rem; margin-bottom: 15px !important;">∿ 通用 I2C 版本</h3>
    <p style="font-size: 1.1rem;">Arduino / ESP32 / 樹莓派 / 各式自主開發主控板</p>
  </a>
</div>

---

## ⚙️ 通用提示

### 🛠️ 一鍵自動來回校準

為了讓 16 路光電感測器獲得最佳的比對增益，本產品提供了簡便的高效自動校準：
*   **觸發方式**：按住感測器上的 **🔘 校準按鈕 0.5 秒** 直到 **🟢 綠色校準指示燈 (靠近校準按鈕)** 保持亮起。
*   **操作步驟**：在綠燈亮起的 **5 秒鐘內**，將車頭在黑線與白色區域之間來回擺動（確保所有感應器都踩過黑線與白地）。
*   **完成狀態**：5 秒後 **🟢 綠色校準指示燈 (靠近校準按鈕)** 熄滅，代表校準已完成，數據會自動非同步存入底層記憶體。

### 🎨 切換追蹤黑線或白線
感應器上電啟動時，**按住校準按鈕再放開**，即可切換追蹤模式：
預設為 **黑線模式**（追蹤深色線），切換後變為 **白線模式**（追蹤淺色線）。

> **💡 數據極性同步切換**：切換追蹤模式後，感測器輸出的反射光強度與高精度連續位置數據**也會自動完成極性翻轉**。因此您的程式邏輯（如 PID 循線演算法）完全不需要做任何修改即可直接完美套用！
> 此設定會保留到下次重新上電。重新上電後會恢復為預設的黑線模式。

---

## ❓ 常見問題 FAQ

- **❓ 感應器插上去沒反應？**
  → 先確認 **🟢 綠色心跳指示燈 (靠近插座)** 有沒有在閃爍。如果沒有任何閃爍，請依序進行以下排除步驟：
  1. 檢查連接線兩端是否確實插緊。
  2. 嘗試更換主控板的其他連接埠 (Port)。
  3. 更換一條連接線進行交叉測試。
  4. **將感應器更換到另一台主機 (Hub) 連接測試**，以排除單一主機接口故障。

- **❓ 機器人沿著線走一直晃來晃去？**
  → 最常見原因：**還沒校準**，或者校準未成功。請回到校準步驟重新校準。
  *若已校準但還是晃，請檢查：*
  1. 感應器高度是否在 **0.5 至 1.2 公分** 之間。

- **❓ 校準完還是讀不到線？**
  → 確認校準時有掃到「最黑的黑線」和「最白的白色區域」。檢查場地光線是否均勻，並確認感應器高度在 0.5 至 1.2 公分。
- **❓ 不同 App 讀到的數值不一樣？**
  → 教育版（黃色主機）和家用版（藍綠色主機）的回傳格式不同。請確認您使用的是哪個版本，再參考對應的平台說明頁。
- **❓ 紅色 LED 燈長亮辦？**
  → **🔴 紅色指示燈長亮** 代表與主控板通訊失敗。請檢查連接線是否插緊、換埠測試、或更換另一條連接線。若換孔、換線及換主機測試後仍長亮紅燈，請聯繫我們的技術支援。

---

### 🌐 聯繫專屬線上支援

如果您嘗試了上述所有步驟仍無法順利排除問題，請隨時聯絡我們！為了讓技術專家能以最快的速度幫您找出癥結點，請協助提供以下資訊：
1. **📸 提供影音**：錄製一段 **車體實際運行或感應器指示燈狀態的短影片**（讓我們能直觀確認訊號燈與實體運作狀態）。
2. **💻 提供程式**：提供您目前使用的 **圖控程式截圖、專案檔（.lms / .ev3）或是程式代碼**。
3. **💬 聯絡方式**：將上述資訊寄送至我們的技術支援信箱，或直接傳送至官方 LINE 客服。

專業的研發工程師將會在第一時間為您進行線上排查與程式調校，全力護航您的每一場競賽！
