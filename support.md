<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。HTML 區塊中間也不要空行。 -->
<!-- 表單程式：_build/assets/support.js；信件欄位標籤（data-label）改了要同步內部規格檔 96_銷售經營\客服信件\信件格式規格_20261104.md -->
# 🆘 技術支援

遇到問題？先花 1 分鐘看下面幾個地方，很多問題當場就能解決。還是不行，就填下面的表單，按一下就會開好一封**收件人、主旨、內容格式都填好**的信，工程師 3 個工作天內回覆。比賽前的急件，加 [LINE @692vcvuk](https://line.me/R/ti/p/@692vcvuk) 最快。

## 先看這裡

<div class="sp-links">
<a class="sp-link" href="/faq.md"><b>❓ 常見問題</b><span>相容性、比賽規則、校正、價格與購買</span></a>
<a class="sp-link" href="/guide.md#start"><b>🚀 已經買好？找程式</b><span>選主機和軟體，直接找到對的教學與範例</span></a>
<a class="sp-link" href="/learn/index.md"><b>🧩 互動教學</b><span>不用接主機，在瀏覽器一步一步看怎麼寫</span></a>
<a class="sp-link" href="/reference.md"><b>📋 指令參考</b><span>全產品 × 全平台的讀值與指令對照</span></a>
<a class="sp-link" href="/downloads.md"><b>⬇️ 下載中心</b><span>函式庫、範例程式、官方 App 小幫手</span></a>
<a class="sp-link" href="/articles/spike-line-following-wobble.md"><b>🛤️ 循線抖動、脫線</b><span>最常見的 5 個原因和改法</span></a>
</div>

> [!TIP]
> 每個產品的「範例」頁最後都有一段**常見問題**（讀不到值、數值怪、程式報錯），先看那裡通常最快。

## 填表寄信

表單**不會把資料傳到任何伺服器**：按下按鈕後，是你自己的郵件程式（或 Gmail）開出一封信，你看過、按「傳送」才會寄出。打 <b>*</b> 的是必填，其他欄位寫越多，我們越能一次回答清楚。

<div class="sp-form" id="support-form">
<noscript><p class="sp-status sp-warn">這個表單需要開啟 JavaScript。也可以直接寄信到 contactmbcrobot（at）gmail.com，或加 LINE @692vcvuk。</p></noscript>
<div class="sp-grid">
<label class="sp-f"><span class="sp-l">問題類別 <b>*</b></span>
<select id="sp-cat" data-k="cat" data-label="【類別】" required>
<option value="">請選擇…</option>
<option value="產品使用" data-mail="產品使用">產品使用（讀值怪、沒反應、校正）</option>
<option value="接線" data-mail="接線">接線（接哪個孔、線材、供電）</option>
<option value="韌體與燒錄" data-mail="韌體與燒錄">韌體與燒錄（更新、版本）</option>
<option value="上位機軟體" data-mail="上位機軟體">上位機軟體（SPIKE 官方 App、Pybricks、EV3、Arduino、MATRIX、mBlock、micro:bit…）</option>
<option value="範例程式" data-mail="範例程式">範例程式（範例跑不動、看不懂、想要某種範例）</option>
<option value="購買與出貨" data-mail="購買與出貨">購買與出貨（報價、訂單、運送、保固、換新）</option>
<option value="教學與比賽" data-mail="教學與比賽">教學與比賽（課程、WRO 等比賽規則、策略）</option>
<option value="其他" data-mail="其他">其他（什麼都可以問）</option>
</select></label>
<label class="sp-f"><span class="sp-l">產品 <b>*</b></span>
<select id="sp-prod" data-k="prod" data-label="【產品】" required>
<option value="">請選擇…</option>
<option value="Line8">循行者 8 路 (Line8)</option>
<option value="Line16">循行者 16 路 (Line16)</option>
<option value="TOF2">測距者 2 路 (TOF2)</option>
<option value="TOF8">測距者 8 路 (TOF8)</option>
<option value="UniCam">神攝手</option>
<option value="IMU">陀螺儀 IMU 九軸</option>
<option value="EXP6">SPIKE 6 路擴充器 (EXP6)</option>
<option value="PS4">掌控者 PS4/PS5</option>
<option value="PS2">掌控者 PS2</option>
<option value="多項">好幾個產品</option>
<option value="選購">還沒買（選購諮詢）</option>
<option value="其他">其他／不確定</option>
</select></label>
<label class="sp-f sp-tech"><span class="sp-l">產品版本</span>
<select id="sp-ed" data-k="edition" data-label="【產品版本】">
<option value="">請選擇…</option>
<option>SPIKE 版</option>
<option>EV3 版</option>
<option>通用 I2C 版（Arduino、ESP32、MATRIX、mBot、micro:bit）</option>
<option>不分版（神攝手、EXP6）</option>
<option>不確定</option>
</select></label>
<label class="sp-f sp-tech"><span class="sp-l">主機</span>
<select id="sp-host" data-k="host" data-label="【主機】">
<option value="">請選擇…</option>
<option>LEGO SPIKE Prime</option>
<option>LEGO Robot Inventor</option>
<option>LEGO MINDSTORMS EV3</option>
<option>Arduino（UNO、Nano、UNO R4…）</option>
<option>ESP32</option>
<option>MATRIX Mini R4</option>
<option>MakeBlock（mBot、mBot2…）</option>
<option>micro:bit</option>
<option>其他（寫在補充資訊）</option>
</select></label>
<label class="sp-f sp-tech"><span class="sp-l">軟體</span>
<select id="sp-sw" data-k="sw" data-label="【軟體】">
<option value="">請選擇…</option>
<option>SPIKE 官方 App（積木）</option>
<option>SPIKE 官方 App（Python）</option>
<option>Pybricks（Python）</option>
<option>Pybricks（積木）</option>
<option>EV3-G／EV3 Lab</option>
<option>EV3 Classroom</option>
<option>EV3 MicroPython／ev3dev</option>
<option>clev3r</option>
<option>Arduino IDE</option>
<option>MATRIX 軟體</option>
<option>mBlock</option>
<option>MakeCode（micro:bit）</option>
<option>MBC 積木網站／BlockCode</option>
<option>其他（寫在補充資訊）</option>
</select></label>
<label class="sp-f sp-tech"><span class="sp-l">軟體版本</span>
<input id="sp-swver" data-k="swver" data-label="【軟體版本】" type="text" maxlength="80" placeholder="例：Pybricks 4.1、SPIKE App 3.4"></label>
<div class="sp-f sp-tech"><label class="sp-l" for="sp-fw">感應器韌體版本</label>
<input id="sp-fw" data-k="fw" data-label="【韌體版本】" type="text" maxlength="80" placeholder="例：1.0，或寫「不知道」">
<details class="sp-help"><summary>韌體版本在哪裡看？</summary>
<ul>
<li><b>Pybricks</b>（用 MBC 函式庫）：<code>print(感應器.firmware_version())</code>，例如印出 <code>(1, 0)</code>。</li>
<li><b>Arduino／ESP32 等 I2C</b>：讀暫存器 <code>F2</code>，102＝1.2，見 <a href="/i2c-protocol.md">通用 I2C 協議</a>。</li>
<li><b>神攝手</b>：相機螢幕的設定選單。</li>
<li>看不到也沒關係：寫「不知道」，再寫<b>購買月份</b>，我們用出貨紀錄對。</li>
</ul></details></div>
<label class="sp-f sp-tech"><span class="sp-l">發生頻率</span>
<select id="sp-freq" data-k="freq" data-label="【發生頻率】">
<option value="">請選擇…</option>
<option>每次都會</option>
<option>有時候會</option>
<option>只發生過一次</option>
<option>不是故障，是想問用法</option>
</select></label>
<label class="sp-f sp-wide"><span class="sp-l">一句話摘要 <b>*</b>（會放進信件主旨）</span>
<input id="sp-sum" data-k="summary" data-label="【一句話摘要】" type="text" maxlength="60" required placeholder="例：插 SPIKE 的 A 孔讀不到值"></label>
<label class="sp-f sp-wide"><span class="sp-l">問題描述 <b>*</b></span>
<textarea id="sp-desc" data-k="desc" data-label="【問題描述】" rows="5" required></textarea></label>
<label class="sp-f sp-wide sp-tech"><span class="sp-l">已經試過什麼</span>
<textarea id="sp-tried" data-k="tried" data-label="【已經試過】" rows="3" placeholder="例：換過孔位、重新校正、換過線、重開主機、照範例頁的常見問題做過"></textarea></label>
<label class="sp-f sp-tech"><span class="sp-l">期待的結果</span>
<textarea id="sp-exp" data-k="expect" data-label="【期待結果】" rows="3" placeholder="例：線在中間時位置值應該是 0"></textarea></label>
<label class="sp-f sp-tech"><span class="sp-l">實際的結果</span>
<textarea id="sp-act" data-k="actual" data-label="【實際結果】" rows="3" placeholder="例：一直是 -100，燈號紅燈閃；錯誤訊息全文貼這裡"></textarea></label>
<label class="sp-f sp-wide"><span class="sp-l">補充資訊</span>
<textarea id="sp-extra" data-k="extra" data-label="【補充資訊】" rows="3"></textarea></label>
<div class="sp-f sp-wide sp-media">
<label class="sp-check"><input id="sp-media" data-k="media" data-label="【照片／影片】" type="checkbox"> 我會附上照片、截圖或影片（強烈建議，常常一張照片就能看出問題）</label>
<div class="sp-media-on">
<p class="sp-note">📸 寄信時請<b>附上</b>：接線全景＋接頭特寫照片、感應器燈號、程式畫面與錯誤訊息的<b>整個視窗</b>截圖；程式檔（.llsp3、.py、.ino、.ev3、.mblock…）也可以直接附。<br>🎬 影片超過 25 MB 寄不出去：請上傳 YouTube（設「不公開」）或 Google 雲端硬碟，把連結貼在下面。</p>
<label class="sp-l" for="sp-link">影片／雲端連結</label>
<input id="sp-link" data-k="medialink" data-label="【影片連結】" type="text" maxlength="300" placeholder="https://youtu.be/…">
</div></div>
<label class="sp-f"><span class="sp-l">怎麼稱呼你</span>
<input id="sp-name" data-k="name" data-label="【稱呼】" type="text" maxlength="40" autocomplete="name" placeholder="例：王老師"></label>
<label class="sp-f"><span class="sp-l">學校／單位</span>
<input id="sp-org" data-k="org" data-label="【學校／單位】" type="text" maxlength="60" autocomplete="organization" placeholder="例：○○國小機器人社、○○機器人教室"></label>
<label class="sp-f"><span class="sp-l">國家／地區</span>
<input id="sp-country" data-k="country" data-label="【國家／地區】" type="text" maxlength="40" placeholder="例：台灣"></label>
</div>
<div class="sp-btns">
<a class="sp-btn sp-primary" href="#" rel="nofollow" data-act="mailto">✉️ 用郵件程式寄信</a>
<a class="sp-btn" href="#" rel="nofollow noopener noreferrer" target="_blank" data-act="gmail">✉️ 用 Gmail 網頁寫信</a>
<button class="sp-btn" type="button" data-act="copy">📋 複製信件內容</button>
<button class="sp-btn sp-ghost" type="button" data-act="reset">清除</button>
</div>
<p class="sp-status" role="status" aria-live="polite"></p>
<details class="sp-preview-box"><summary>預覽信件內容</summary>
<pre class="sp-preview"></pre>
</details>
<script type="application/json" id="support-data">{"lang":"zh-TW","tag":"[MBC 支援]","yes":"會附上","no":"沒有","empty":"（未填）","greet":"MBC 技術支援您好：","envLabel":"【瀏覽器／系統】","langLabel":"【網頁語言】","verLabel":"【表單版本】","sep":"--------","attachNote":"（照片、截圖、程式檔請直接附在這封信；可以在這行下面繼續補充）","to":"收件人：","subj":"主旨：","missing":"還有必填欄位沒填：問題類別、產品、一句話摘要、問題描述。","opened":"正在開啟你的郵件程式。沒反應的話，請改用「用 Gmail 網頁寫信」或「複製信件內容」。","long":"內容比較長，有些郵件程式會截斷。沒有完整帶入的話，請改用「用 Gmail 網頁寫信」或「複製信件內容」。","gmail":"已在新分頁開啟 Gmail（要先登入 Google 帳號）。記得附上照片再按傳送。","copied":"已複製收件人、主旨和內容，貼到任何郵件程式就能寄。","copyFail":"瀏覽器不讓複製：請打開下面的「預覽信件內容」自己選取複製。","noTech":["購買與出貨"],"hints":{"_":{"desc":"發生了什麼事？在做什麼的時候發生？例：把感應器插在 SPIKE 的 A 孔，執行範例 1，畫面一直顯示 0。","extra":"其他想讓我們知道的事"},"產品使用":{"desc":"在做什麼的時候發生？讀值是多少、應該是多少？燈號是什麼顏色、有沒有閃？","extra":"場地（WRO 地圖、自製場地）、光線、感應器離地高度、感應器怎麼裝在車上"},"接線":{"desc":"接在主機的哪個孔、用哪條線？Arduino 請寫 SDA／SCL／電源各接到哪一腳。","extra":"線材長度、有沒有用轉接板或延長線、主機用電池還是 USB 供電"},"韌體與燒錄":{"desc":"要更新哪個韌體？做到哪一步出問題？錯誤訊息全文貼上。","extra":"電腦作業系統、用的燒錄工具、USB 線、COM 埠"},"上位機軟體":{"desc":"用哪個軟體、哪個版本？做了什麼操作、出現什麼畫面或錯誤訊息？程式碼可以直接貼上。","extra":"主機韌體版本（例如 Pybricks 版本、SPIKE 主機韌體）"},"範例程式":{"desc":"哪一個範例（網址或檔名）？跑起來跟說明哪裡不一樣？或想要什麼樣的範例？","extra":"範例網址、你改過哪幾行"},"購買與出貨":{"desc":"想買什麼、數量多少？或訂單／保固／換新的狀況。","extra":"購買管道（官網、LINE、經銷商名稱）、購買日期、訂單編號、收件國家與城市"},"教學與比賽":{"desc":"是哪個比賽、組別？想解決什麼任務或課程問題？","extra":"比賽名稱與年度、組別（國小／國中／高中）、比賽日期"},"其他":{"desc":"什麼都可以問，寫得越具體越好。","extra":"其他想讓我們知道的事"}}}</script>
</div>
<script src="/assets/support.js" defer></script>

## 寄信前的小提醒

- **照片最有用**：接線全景＋接頭特寫、感應器燈號、程式畫面加錯誤訊息（整個視窗）。
- **影片**：10～30 秒就夠，拍到車子實際跑和感應器燈號。超過 25 MB 請貼 YouTube（不公開）或雲端連結。
- **程式檔**：直接附 `.llsp3`、`.py`、`.ino`、`.ev3`、`.mblock` 檔，或把程式碼貼進信裡。
- **一封信一個問題**：問題不一樣就分開寄，回覆比較快、也不會漏。
- 不想用表單也可以：到 [聯絡我們](/contact.md) 直接寄信，或加 [LINE @692vcvuk](https://line.me/R/ti/p/@692vcvuk)。
