<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。HTML 區塊中間也不要空行。 -->
<!-- 表單程式：_build/assets/support.js；信件欄位標籤（data-label）改了要同步內部規格檔 96_銷售經營\客服信件\信件格式規格_20261104.md -->
# 🆘 Technical Support

Something not working? Spend a minute on the pages below first: many problems are solved right there. Still stuck? Fill in the form and one click opens an email with the **recipient, subject and layout already filled in**. Our engineers reply within 3 business days.

## Check these first

<div class="sp-links">
<a class="sp-link" href="/en/faq.md"><b>❓ FAQ</b><span>Compatibility, competition rules, calibration, pricing and ordering</span></a>
<a class="sp-link" href="/en/guide.md#start"><b>🚀 Already bought? Find your code</b><span>Pick your controller and software to jump to the right guide and examples</span></a>
<a class="sp-link" href="/en/learn/index.md"><b>🧩 Interactive tutorials</b><span>No hub needed: step through the code in your browser</span></a>
<a class="sp-link" href="/en/reference.md"><b>📋 Command reference</b><span>Readings and commands for every product on every platform</span></a>
<a class="sp-link" href="/en/downloads.md"><b>⬇️ Downloads</b><span>Libraries, example programs, official app helpers</span></a>
<a class="sp-link" href="/en/i2c-protocol.md"><b>🔌 Universal I2C protocol</b><span>Arduino, ESP32 and Raspberry Pi: wiring, addresses and registers</span></a>
</div>

> [!TIP]
> Every product's **Examples** page ends with a troubleshooting section (no readings, odd values, program errors). It is usually the quickest place to look.

## Email us with the form

The form **sends nothing to any server**: the buttons open an email in your own mail app (or Gmail), and nothing is sent until you review it and press Send. Fields marked <b>*</b> are required; the more you fill in, the better we can answer in one reply.

<div class="sp-form" id="support-form">
<noscript><p class="sp-status sp-warn">This form needs JavaScript. You can also email contactmbcrobot (at) gmail.com directly.</p></noscript>
<div class="sp-grid">
<label class="sp-f"><span class="sp-l">Category <b>*</b></span>
<select id="sp-cat" data-k="cat" data-label="[Category] " required>
<option value="">Choose…</option>
<option value="Usage" data-mail="Usage">Using the product (odd readings, no response, calibration)</option>
<option value="Wiring" data-mail="Wiring">Wiring (which port, cables, power)</option>
<option value="Firmware" data-mail="Firmware">Firmware and flashing (updates, versions)</option>
<option value="Software" data-mail="Software">Controller software (SPIKE App, Pybricks, EV3, Arduino, MATRIX, mBlock, micro:bit…)</option>
<option value="Examples" data-mail="Examples">Example programs (won't run, unclear, request an example)</option>
<option value="Order" data-mail="Order">Ordering and shipping (quotes, orders, delivery, warranty, replacement)</option>
<option value="Teaching" data-mail="Teaching">Teaching and competitions (lessons, WRO and other rules, strategy)</option>
<option value="Other" data-mail="Other">Other (ask anything)</option>
</select></label>
<label class="sp-f"><span class="sp-l">Product <b>*</b></span>
<select id="sp-prod" data-k="prod" data-label="[Product] " required>
<option value="">Choose…</option>
<option value="Line8">Pathfinder 8-Way (Line8)</option>
<option value="Line16">Pathfinder 16-Way (Line16)</option>
<option value="TOF2">Ranger 2-Way (TOF2)</option>
<option value="TOF8">Ranger 8-Way (TOF8)</option>
<option value="UniCam">Sharpshooter Vision</option>
<option value="IMU">IMU 9-Axis Gyro</option>
<option value="EXP6">SPIKE 6-Way Expander (EXP6)</option>
<option value="PS4">Controller PS4/PS5</option>
<option value="PS2">Controller 2.4G (PS2, Taiwan only)</option>
<option value="Multiple">Several products</option>
<option value="Pre-sale">Not bought yet (which one to choose)</option>
<option value="Other">Other / not sure</option>
</select></label>
<label class="sp-f sp-tech"><span class="sp-l">Edition</span>
<select id="sp-ed" data-k="edition" data-label="[Edition] ">
<option value="">Choose…</option>
<option>SPIKE edition</option>
<option>EV3 edition</option>
<option>Universal I2C edition (Arduino, ESP32, MATRIX, mBot, micro:bit)</option>
<option>One edition for all (Sharpshooter Vision, EXP6)</option>
<option>Not sure</option>
</select></label>
<label class="sp-f sp-tech"><span class="sp-l">Controller</span>
<select id="sp-host" data-k="host" data-label="[Controller] ">
<option value="">Choose…</option>
<option>LEGO SPIKE Prime</option>
<option>LEGO Robot Inventor</option>
<option>LEGO MINDSTORMS EV3</option>
<option>Arduino (UNO, Nano, UNO R4…)</option>
<option>ESP32</option>
<option>MATRIX Mini R4</option>
<option>MakeBlock (mBot, mBot2…)</option>
<option>micro:bit</option>
<option>Other (add under More details)</option>
</select></label>
<label class="sp-f sp-tech"><span class="sp-l">Software</span>
<select id="sp-sw" data-k="sw" data-label="[Software] ">
<option value="">Choose…</option>
<option>SPIKE App (word blocks)</option>
<option>SPIKE App (Python)</option>
<option>Pybricks (Python)</option>
<option>Pybricks (blocks)</option>
<option>EV3-G / EV3 Lab</option>
<option>EV3 Classroom</option>
<option>EV3 MicroPython / ev3dev</option>
<option>clev3r</option>
<option>Arduino IDE</option>
<option>MATRIX software</option>
<option>mBlock</option>
<option>MakeCode (micro:bit)</option>
<option>MBC block websites / BlockCode</option>
<option>Other (add under More details)</option>
</select></label>
<label class="sp-f sp-tech"><span class="sp-l">Software version</span>
<input id="sp-swver" data-k="swver" data-label="[Software version] " type="text" maxlength="80" placeholder="e.g. Pybricks 4.1, SPIKE App 3.4"></label>
<div class="sp-f sp-tech"><label class="sp-l" for="sp-fw">Sensor firmware version</label>
<input id="sp-fw" data-k="fw" data-label="[Firmware version] " type="text" maxlength="80" placeholder="e.g. 1.0, or &quot;don't know&quot;">
<details class="sp-help"><summary>Where do I find the firmware version?</summary>
<ul>
<li><b>Pybricks</b> (with the MBC library): <code>print(sensor.firmware_version())</code>, e.g. <code>(1, 0)</code>.</li>
<li><b>Arduino / ESP32 and other I2C</b>: read register <code>F2</code>, 102 = 1.2. See the <a href="/en/i2c-protocol.md">Universal I2C protocol</a>.</li>
<li><b>Sharpshooter Vision</b>: the settings menu on the camera screen.</li>
<li>Can't find it? Write "don't know" and the <b>month you bought it</b>; we will check our shipping records.</li>
</ul></details></div>
<label class="sp-f sp-tech"><span class="sp-l">How often does it happen?</span>
<select id="sp-freq" data-k="freq" data-label="[How often] ">
<option value="">Choose…</option>
<option>Every time</option>
<option>Sometimes</option>
<option>Only once</option>
<option>Not a fault, a how-to question</option>
</select></label>
<label class="sp-f sp-wide"><span class="sp-l">One-line summary <b>*</b> (goes into the subject)</span>
<input id="sp-sum" data-k="summary" data-label="[Summary] " type="text" maxlength="60" required placeholder="e.g. No readings on SPIKE port A"></label>
<label class="sp-f sp-wide"><span class="sp-l">What is the problem? <b>*</b></span>
<textarea id="sp-desc" data-k="desc" data-label="[Description]" rows="5" required></textarea></label>
<label class="sp-f sp-wide sp-tech"><span class="sp-l">What have you tried?</span>
<textarea id="sp-tried" data-k="tried" data-label="[Already tried]" rows="3" placeholder="e.g. another port, recalibrated, another cable, restarted the hub, followed the troubleshooting section"></textarea></label>
<label class="sp-f sp-tech"><span class="sp-l">What did you expect?</span>
<textarea id="sp-exp" data-k="expect" data-label="[Expected]" rows="3" placeholder="e.g. position should be 0 when the line is in the middle"></textarea></label>
<label class="sp-f sp-tech"><span class="sp-l">What actually happened?</span>
<textarea id="sp-act" data-k="actual" data-label="[Actual]" rows="3" placeholder="e.g. always -100 and the LED blinks red; paste the full error message here"></textarea></label>
<label class="sp-f sp-wide"><span class="sp-l">More details</span>
<textarea id="sp-extra" data-k="extra" data-label="[More details]" rows="3"></textarea></label>
<div class="sp-f sp-wide sp-media">
<label class="sp-check"><input id="sp-media" data-k="media" data-label="[Photos / video] " type="checkbox"> I will attach photos, screenshots or a video (highly recommended: one photo often shows the problem)</label>
<div class="sp-media-on">
<p class="sp-note">📸 Please <b>attach</b>: an overall photo of the wiring plus a close-up of the plugs, the sensor LEDs, and a screenshot of the <b>whole window</b> with your program and the error message. Program files (.llsp3, .py, .ino, .ev3, .mblock…) can be attached too.<br>🎬 Videos over 25 MB won't send: upload to YouTube (set to Unlisted) or Google Drive and paste the link below.</p>
<label class="sp-l" for="sp-link">Video / cloud link</label>
<input id="sp-link" data-k="medialink" data-label="[Video link] " type="text" maxlength="300" placeholder="https://youtu.be/…">
</div></div>
<label class="sp-f"><span class="sp-l">Your name</span>
<input id="sp-name" data-k="name" data-label="[Name] " type="text" maxlength="40" autocomplete="name" placeholder="e.g. Coach Lee"></label>
<label class="sp-f"><span class="sp-l">School / organization</span>
<input id="sp-org" data-k="org" data-label="[Organization] " type="text" maxlength="60" autocomplete="organization" placeholder="e.g. Lincoln Middle School robotics club"></label>
<label class="sp-f"><span class="sp-l">Country / region</span>
<input id="sp-country" data-k="country" data-label="[Country] " type="text" maxlength="40" autocomplete="country-name" placeholder="e.g. Singapore"></label>
</div>
<div class="sp-btns">
<a class="sp-btn sp-primary" href="#" rel="nofollow" data-act="mailto">✉️ Open in my mail app</a>
<a class="sp-btn" href="#" rel="nofollow noopener noreferrer" target="_blank" data-act="gmail">✉️ Write in Gmail</a>
<button class="sp-btn" type="button" data-act="copy">📋 Copy the email</button>
<button class="sp-btn sp-ghost" type="button" data-act="reset">Clear</button>
</div>
<p class="sp-status" role="status" aria-live="polite"></p>
<details class="sp-preview-box"><summary>Preview the email</summary>
<pre class="sp-preview"></pre>
</details>
<script type="application/json" id="support-data">{"lang":"en","tag":"[MBC Support]","yes":"Yes, attached","no":"No","empty":"(not filled in)","greet":"Hello MBC Support,","envLabel":"[Browser / OS] ","langLabel":"[Page language] ","verLabel":"[Form version] ","sep":"--------","attachNote":"(Attach photos, screenshots and program files to this email; add anything else below this line.)","to":"To: ","subj":"Subject: ","missing":"Some required fields are empty: Category, Product, One-line summary, What is the problem.","opened":"Opening your mail app. If nothing happens, use \"Write in Gmail\" or \"Copy the email\" instead.","long":"This is a long message and some mail apps cut it off. If it is incomplete, use \"Write in Gmail\" or \"Copy the email\" instead.","gmail":"Gmail opened in a new tab (you need to be signed in to Google). Remember to attach your photos before pressing Send.","copied":"Recipient, subject and message copied. Paste them into any mail app.","copyFail":"Your browser blocked copying: open \"Preview the email\" below and copy it by hand.","noTech":["Order"],"hints":{"_":{"desc":"What happened, and what were you doing? e.g. Sensor on SPIKE port A, ran example 1, the screen always shows 0.","extra":"Anything else we should know"},"Usage":{"desc":"What were you doing when it happened? What reading do you get, and what should it be? Which LED color, is it blinking?","extra":"Mat (WRO mat, home-made), lighting, sensor height above the floor, how it is mounted"},"Wiring":{"desc":"Which port and which cable? For Arduino, list where SDA / SCL / power go.","extra":"Cable length, adapters or extension cables, hub on battery or USB power"},"Firmware":{"desc":"Which firmware are you updating, and at which step does it fail? Paste the full error message.","extra":"Computer OS, flashing tool, USB cable, COM port"},"Software":{"desc":"Which software and version? What did you do, and what screen or error appeared? Paste your code.","extra":"Hub firmware version (e.g. Pybricks version, SPIKE hub firmware)"},"Examples":{"desc":"Which example (URL or file name)? How does it differ from the description, or what example would you like?","extra":"Example URL, lines you changed"},"Order":{"desc":"What would you like to buy and how many? Or describe the order / warranty / replacement issue.","extra":"Where you bought it (website, distributor name), purchase date, order number, delivery country and city"},"Teaching":{"desc":"Which competition and age group? What mission or lesson problem are you trying to solve?","extra":"Competition name and season, age group, competition date"},"Other":{"desc":"Ask anything. The more specific, the better.","extra":"Anything else we should know"}}}</script>
</div>
<script src="/assets/support.js" defer></script>

## Before you send

- **Photos help most**: an overall shot of the wiring plus a close-up of the plugs, the sensor LEDs, and the program with its error message (whole window).
- **Video**: 10–30 seconds is enough; show the robot running and the sensor LEDs. Over 25 MB, paste a YouTube (Unlisted) or cloud link.
- **Program files**: attach `.llsp3`, `.py`, `.ino`, `.ev3` or `.mblock` files, or paste the code into the email.
- **One problem per email**: separate questions get faster, more complete answers.
- Prefer not to use the form? Email us from the [Contact](/en/contact.md) page.
