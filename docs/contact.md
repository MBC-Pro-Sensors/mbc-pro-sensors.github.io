<style>
  .contact-hero {
    background: linear-gradient(135deg, rgba(0,210,255,0.06) 0%, rgba(10,186,181,0.04) 50%, rgba(255,107,53,0.04) 100%);
    border: 1px solid rgba(0,210,255,0.2);
    border-radius: 16px;
    padding: 40px 30px;
    text-align: center;
    margin-bottom: 40px;
    position: relative;
    overflow: hidden;
  }
  .contact-hero::before {
    content: '';
    position: absolute;
    top: -50%;
    left: -50%;
    width: 200%;
    height: 200%;
    background: radial-gradient(ellipse at center, rgba(0,210,255,0.04) 0%, transparent 60%);
    pointer-events: none;
  }
  .contact-badge {
    display: inline-block;
    background: linear-gradient(135deg, rgba(0,210,255,0.15), rgba(10,186,181,0.1));
    border: 1px solid rgba(0,210,255,0.35);
    color: #00d2ff;
    font-size: 0.78rem;
    font-weight: bold;
    letter-spacing: 2px;
    padding: 5px 16px;
    border-radius: 20px;
    margin-bottom: 18px;
    text-transform: uppercase;
  }
  .contact-hero h1 {
    font-size: 2.2rem;
    font-weight: 800;
    margin: 0 0 12px 0;
    background: linear-gradient(135deg, #ffffff 30%, #00d2ff 100%);
    -webkit-background-clip: text;
    -webkit-text-fill-color: transparent;
    background-clip: text;
  }
  .contact-hero p {
    font-size: 1.05rem;
    color: #aaa;
    max-width: 600px;
    margin: 0 auto;
    line-height: 1.7;
  }
  .reasons-grid {
    display: grid;
    grid-template-columns: repeat(auto-fill, minmax(280px, 1fr));
    gap: 16px;
    margin: 30px 0 40px 0;
  }
  .reason-card {
    background: rgba(255,255,255,0.025);
    border: 1px solid rgba(255,255,255,0.08);
    border-radius: 12px;
    padding: 20px;
    transition: all 0.3s ease;
    position: relative;
    overflow: hidden;
  }
  .reason-card:hover {
    border-color: rgba(0,210,255,0.3);
    background: rgba(0,210,255,0.04);
    transform: translateY(-3px);
    box-shadow: 0 8px 25px rgba(0,210,255,0.1);
  }
  .reason-icon {
    font-size: 2rem;
    margin-bottom: 10px;
    display: block;
  }
  .reason-card h3 {
    margin: 0 0 8px 0;
    color: #e0e0e0;
    font-size: 1rem;
    font-weight: 700;
  }
  .reason-card p {
    margin: 0;
    font-size: 0.83rem;
    color: #888;
    line-height: 1.6;
  }
  .contact-channel-grid {
    display: grid;
    grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));
    gap: 20px;
    margin: 30px 0;
  }
  .channel-card {
    border-radius: 14px;
    padding: 28px;
    display: flex;
    align-items: flex-start;
    gap: 18px;
    transition: all 0.3s ease;
  }
  .channel-card.email {
    background: rgba(0,210,255,0.05);
    border: 1px solid rgba(0,210,255,0.25);
  }
  .channel-card.line {
    background: rgba(10,186,181,0.05);
    border: 1px solid rgba(10,186,181,0.25);
  }
  .channel-card:hover {
    transform: translateY(-3px);
    box-shadow: 0 12px 30px rgba(0,0,0,0.3);
  }
  .channel-icon-wrap {
    width: 56px;
    height: 56px;
    border-radius: 14px;
    display: flex;
    align-items: center;
    justify-content: center;
    font-size: 1.8rem;
    flex-shrink: 0;
  }
  .channel-card.email .channel-icon-wrap { background: rgba(0,210,255,0.12); }
  .channel-card.line .channel-icon-wrap { background: rgba(10,186,181,0.12); }
  .channel-info h3 {
    margin: 0 0 6px 0;
    font-size: 1.1rem;
    font-weight: bold;
  }
  .channel-card.email .channel-info h3 { color: #00d2ff; }
  .channel-card.line .channel-info h3 { color: #0abab5; }
  .channel-info p { margin: 0 0 8px 0; font-size: 0.83rem; color: #888; line-height: 1.5; }
  .channel-info code {
    font-size: 1rem;
    font-weight: bold;
    padding: 4px 10px;
    border-radius: 6px;
    display: inline-block;
  }
  .channel-card.email .channel-info code { background: rgba(0,210,255,0.1); color: #00d2ff; }
  .channel-card.line .channel-info code { background: rgba(10,186,181,0.1); color: #0abab5; }
  .support-info-box {
    background: rgba(255,107,53,0.05);
    border: 1px solid rgba(255,107,53,0.2);
    border-left: 4px solid #ff6b35;
    border-radius: 10px;
    padding: 20px 24px;
    margin: 30px 0;
  }
  .support-info-box h3 {
    color: #ff6b35;
    margin: 0 0 12px 0;
    font-size: 1.05rem;
  }
  .support-list {
    list-style: none;
    padding: 0;
    margin: 0;
  }
  .support-list li {
    padding: 7px 0;
    border-bottom: 1px solid rgba(255,255,255,0.05);
    font-size: 0.88rem;
    color: #ccc;
    display: flex;
    align-items: flex-start;
    gap: 10px;
    line-height: 1.5;
  }
  .support-list li:last-child { border-bottom: none; }
  .support-list li .step-icon { font-size: 1.1rem; flex-shrink: 0; margin-top: 1px; }
  .section-title {
    font-size: 1.4rem;
    font-weight: bold;
    color: #fff;
    margin: 40px 0 20px 0;
    padding-bottom: 10px;
    border-bottom: 1px solid rgba(255,255,255,0.08);
    display: flex;
    align-items: center;
    gap: 10px;
  }
</style>

# 聯絡我們

<div class="contact-hero">
  <div class="contact-badge">Contact & Support</div>
  <h1>我們是您最強的後盾</h1>
  <p>不論是技術協助、產品洽詢、合作提案或國際代理，我們的專業工程師與業務團隊隨時準備好了——<strong style="color:#00d2ff;">讓每一個問題都在第一時間被解決</strong>。</p>
</div>

---

<div class="section-title">💡 哪些情況可以聯絡我們？</div>

<div class="reasons-grid">
  <div class="reason-card">
    <span class="reason-icon">🛠️</span>
    <h3>技術救援與故障排查</h3>
    <p>感應器無法連線、數值異常、程式報錯？告訴我們，我們的研發工程師將遠端協助排查並調試。</p>
  </div>
  <div class="reason-card">
    <span class="reason-icon">🛒</span>
    <h3>詢價與採購洽談</h3>
    <p>無論是學校採購、社團補給或是大量訂購，請直接詢問我們取得最新的產品定價與出貨方案。</p>
  </div>
  <div class="reason-card">
    <span class="reason-icon">🤝</span>
    <h3>成為區域代理商</h3>
    <p>想在您的地區推廣最頂尖的機器人感應器嗎？歡迎加入我們的代理體系，共同拓展市場。</p>
  </div>
  <div class="reason-card">
    <span class="reason-icon">🏫</span>
    <h3>學校與教育機構合作</h3>
    <p>為課程導入頂尖硬體設備、舉辦校內機器人技術工作坊，或申請教育機構特別折扣，歡迎與我們攜手。</p>
  </div>

  <div class="reason-card">
    <span class="reason-icon">🌏</span>
    <h3>海外市場與國際合作</h3>
    <p>位於台灣以外的地區，有意引進我們的產品線？歡迎洽談跨國合作與產品輸出方案。</p>
  </div>
  <div class="reason-card">
    <span class="reason-icon">🔬</span>
    <h3>客製化產品開發</h3>
    <p>現有規格無法滿足您的特殊應用場景？我們提供客製化感應器開發服務，歡迎提出您的需求規格。</p>
  </div>
  <div class="reason-card">
    <span class="reason-icon">📣</span>
    <h3>活動贊助與媒體合作</h3>
    <p>舉辦機器人競賽、校際挑戰賽或科技展覽？歡迎尋求我們的設備贊助或品牌共同宣傳機會。</p>
  </div>
  <div class="reason-card">
    <span class="reason-icon">📝</span>
    <h3>產品使用心得與建議回饋</h3>
    <p>您的使用經驗是我們進步最直接的動力！歡迎分享任何使用心得、功能建議或改善意見。</p>
  </div>
  <div class="reason-card">
    <span class="reason-icon">💼</span>
    <h3>加入我們的團隊</h3>
    <p>對機器人感應器、韌體開發或教育科技有熱忱？歡迎主動投遞履歷，我們隨時歡迎有志之士。</p>
  </div>
</div>

---

<div class="section-title">📬 聯絡管道</div>

<div class="contact-channel-grid">
  <div class="channel-card email">
    <div class="channel-icon-wrap">✉️</div>
    <div class="channel-info">
      <h3>技術支援信箱</h3>
      <p>適合：附上程式截圖、影片或詳細文字說明的問題回報，以及採購詢價、合作提案。我們承諾於工作日 24 小時內回覆。</p>
      <code>mbc.robot89@gmail.com</code>
    </div>
  </div>
  <div class="channel-card line">
    <div class="channel-icon-wrap">💬</div>
    <div class="channel-info">
      <h3>官方 LINE 客服</h3>
      <p>適合：快速詢問、即時溝通、傳送短影片或截圖。直接與工程師即時對話，是最快的聯絡方式！</p>
      <code>@692vcvuk</code>
    </div>
  </div>
</div>

---

<div class="section-title">🆘 遠端技術支援</div>

<div class="support-info-box">
  <h3>⚡ 如何讓工程師最快速地幫您解決問題？</h3>
  <ul class="support-list">
    <li>
      <span class="step-icon">📸</span>
      <span><strong>提供影音</strong>：錄製一段車體實際運行或感應器指示燈狀態的短影片。能讓我們直觀確認訊號燈與實體運作狀況，大幅縮短排查時間。</span>
    </li>
    <li>
      <span class="step-icon">💻</span>
      <span><strong>提供程式</strong>：截圖或上傳您目前使用的圖控程式畫面，或提供專案檔（<code>.lms</code> / <code>.ev3</code> / <code>.bp</code>）及程式代碼。</span>
    </li>
    <li>
      <span class="step-icon">📋</span>
      <span><strong>描述情境</strong>：說明您正在進行的任務、目前遇到的現象，以及您已嘗試過的排除方法，有助我們快速鎖定問題。</span>
    </li>
  </ul>
</div>

> [!TIP]
> 準備好上述資訊後，透過 **LINE 客服** 傳送是最快速的方式！我們的研發工程師將在第一時間為您進行線上排查與程式調校，全力護航您的每一場競賽！ 🏆
