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

# Contact Us

<div class="contact-hero">
  <div class="contact-badge">Contact & Support</div>
  <h1>Your Strongest Partner</h1>
  <p>Whether it's technical assistance, product inquiries, partnership proposals, or becoming a regional distributor — our professional engineering and business team is always ready. <strong style="color:#00d2ff;">Every question answered, at the speed you need.</strong></p>
</div>

---

<div class="section-title">💡 When Should You Contact Us?</div>

<div class="reasons-grid">
  <div class="reason-card">
    <span class="reason-icon">🛠️</span>
    <h3>Technical Rescue & Troubleshooting</h3>
    <p>Sensor won't connect, abnormal readings, program errors? Tell us — our R&D engineers will remotely assist you in diagnosing and fine-tuning your setup.</p>
  </div>
  <div class="reason-card">
    <span class="reason-icon">🛒</span>
    <h3>Product Pricing & Purchasing</h3>
    <p>Whether it's for a school program, team supply, or bulk order — contact us directly to get the latest pricing and shipping options.</p>
  </div>
  <div class="reason-card">
    <span class="reason-icon">🤝</span>
    <h3>Become a Regional Distributor</h3>
    <p>Want to bring the world's top competition-grade sensors to your region? Join our distributor network and grow with us.</p>
  </div>
  <div class="reason-card">
    <span class="reason-icon">🏫</span>
    <h3>School & Educational Institution Partnerships</h3>
    <p>Bring cutting-edge hardware into your curriculum, host on-campus robotics workshops, or apply for special educational pricing.</p>
  </div>


  <div class="reason-card">
    <span class="reason-icon">🌏</span>
    <h3>International Markets & Cross-Border Collaboration</h3>
    <p>Located outside of Taiwan and interested in introducing our product lines? Let's discuss cross-national cooperation and export arrangements.</p>
  </div>
  <div class="reason-card">
    <span class="reason-icon">🔬</span>
    <h3>Custom Product Development</h3>
    <p>Do our standard specs fall short for your special application? We offer custom sensor development services — just bring us your requirements.</p>
  </div>
  <div class="reason-card">
    <span class="reason-icon">📣</span>
    <h3>Event Sponsorship & Media Partnerships</h3>
    <p>Organizing a robotics tournament, inter-school challenge, or tech expo? Reach out about equipment sponsorship or co-branding opportunities.</p>
  </div>
  <div class="reason-card">
    <span class="reason-icon">📝</span>
    <h3>Product Feedback & Feature Suggestions</h3>
    <p>Your hands-on experience is our most direct driver of improvement! Share any user insights, feature requests, or improvement suggestions anytime.</p>
  </div>
  <div class="reason-card">
    <span class="reason-icon">💼</span>
    <h3>Join Our Team</h3>
    <p>Passionate about robotics sensors, firmware development, or education technology? Send us your resume — we welcome talented, motivated individuals at any time.</p>
  </div>
</div>

---

<div class="section-title">📬 Contact Channels</div>

<div class="contact-channel-grid">
  <div class="channel-card email">
    <div class="channel-icon-wrap">✉️</div>
    <div class="channel-info">
      <h3>Technical Support Email</h3>
      <p>Best for: Detailed problem reports with screenshots or videos attached, purchase inquiries, and partnership proposals. We promise to reply within 24 business hours.</p>
      <code>mbc.robot89@gmail.com</code>
    </div>
  </div>
  <div class="channel-card line">
    <div class="channel-icon-wrap">💬</div>
    <div class="channel-info">
      <h3>Official LINE Support</h3>
      <p>Best for: Quick questions, real-time communication, sending short clips or screenshots. Chat directly with our engineers — the fastest way to get help!</p>
      <code>@692vcvuk</code>
    </div>
  </div>
</div>

---

<div class="section-title">🆘 Remote Technical Support</div>

<div class="support-info-box">
  <h3>⚡ How to Help Our Engineers Solve Your Problem as Fast as Possible?</h3>
  <ul class="support-list">
    <li>
      <span class="step-icon">📸</span>
      <span><strong>Provide a Video</strong>: Record a short clip of your robot running or the sensor indicator LEDs. A visual reference dramatically speeds up diagnosis.</span>
    </li>
    <li>
      <span class="step-icon">💻</span>
      <span><strong>Share Your Program</strong>: Screenshot your block program, or upload your project file (<code>.lms</code> / <code>.ev3</code> / <code>.bp</code>) and source code.</span>
    </li>
    <li>
      <span class="step-icon">📋</span>
      <span><strong>Describe the Context</strong>: Tell us what you were trying to do, what happened, and what you've already tried — this helps us pinpoint the issue fast.</span>
    </li>
  </ul>
</div>

> [!TIP]
> Once you have the above ready, sending via **LINE** is the fastest path to resolution! Our R&D engineers will get on it immediately — fully committed to powering your success in every competition. 🏆
