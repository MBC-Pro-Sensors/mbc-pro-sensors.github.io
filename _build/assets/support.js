// 技術支援頁（/support.html、/en/support.html）的寄信表單
// - 純前端：不送任何資料到伺服器，只把欄位組成一封信，交給使用者自己的郵件程式／Gmail，或複製到剪貼簿。
// - 收件信箱不寫在 HTML 裡，在這裡拼出來（擋掉只掃 HTML 的收信箱機器人）。
// - 欄位文字（中英）都在頁面上：每個欄位的 data-label 是信裡的固定標籤，data-k 是欄位代號。
// - 信件格式規格：02_MBC產品開發\96_銷售經營\客服信件\信件格式規格_20261104.md（內部檔）
(function () {
  var root = document.getElementById('support-form');
  var dataEl = document.getElementById('support-data');
  if (!root || !dataEl) return;
  var T;
  try { T = JSON.parse(dataEl.textContent); } catch (e) { return; }
  root.classList.add('sp-ready');

  var ADDR = ['contact', 'mbc', 'robot'].join('') + String.fromCharCode(64) + ['gmail', 'com'].join('.');
  var FORM_VER = 'MBC-support-v1';
  var fields = Array.prototype.slice.call(root.querySelectorAll('[data-k]'));
  var byK = {};
  fields.forEach(function (f) { byK[f.dataset.k] = f; });
  var status = root.querySelector('.sp-status');
  var preview = root.querySelector('.sp-preview');

  function val(k) {
    var f = byK[k];
    if (!f) return '';
    if (f.type === 'checkbox') return f.checked ? T.yes : T.no;
    if (f.tagName === 'SELECT') return f.value ? f.options[f.selectedIndex].dataset.mail || f.options[f.selectedIndex].text : '';
    return f.value.replace(/\r\n/g, '\n').trim();
  }
  function code(k) { var f = byK[k]; return f ? f.value : ''; }

  // 瀏覽器／系統（只放進信裡給工程師看，不會送到任何地方）
  function env() {
    var u = navigator.userAgent, os = 'Other', br = 'Other', m;
    if (/Windows NT/.test(u)) os = 'Windows'; else if (/Android/.test(u)) os = 'Android';
    else if (/iPhone|iPad/.test(u)) os = 'iOS'; else if (/CrOS/.test(u)) os = 'ChromeOS';
    else if (/Mac OS X/.test(u)) os = 'macOS'; else if (/Linux/.test(u)) os = 'Linux';
    if ((m = u.match(/Edg\/(\d+)/))) br = 'Edge ' + m[1];
    else if ((m = u.match(/OPR\/(\d+)/))) br = 'Opera ' + m[1];
    else if ((m = u.match(/Firefox\/(\d+)/))) br = 'Firefox ' + m[1];
    else if ((m = u.match(/Chrome\/(\d+)/))) br = 'Chrome ' + m[1];
    else if ((m = u.match(/Version\/(\d+).*Safari/))) br = 'Safari ' + m[1];
    return os + ' / ' + br;
  }

  function subject() {
    var s = val('summary').replace(/\s+/g, ' ');
    return T.tag + '[' + (code('cat') || '?') + '][' + (code('prod') || '?') + '] ' + s;
  }
  function body() {
    var out = [T.greet, ''];
    fields.forEach(function (f) {
      if (f.dataset.mail === 'skip') return;
      if (f.closest('.sp-hide')) return;
      var v = val(f.dataset.k);
      // 沒填的選填欄位不放進信（mailto 網址比較短；解析時「沒有這個標籤」＝沒填）
      if (!v && !f.required) return;
      var multi = f.tagName === 'TEXTAREA';
      if (multi) { out.push(f.dataset.label); out.push(v || T.empty); out.push(''); }
      else out.push(f.dataset.label + (v || T.empty));
    });
    out.push(T.envLabel + env());
    out.push(T.langLabel + T.lang);
    out.push(T.verLabel + FORM_VER);
    out.push('', T.sep, T.attachNote, '');
    return out.join('\n');
  }

  function check() {
    var bad = null;
    ['cat', 'prod', 'summary', 'desc'].forEach(function (k) {
      var f = byK[k];
      var ok = !!(f && f.value.trim());
      if (f) f.setAttribute('aria-invalid', ok ? 'false' : 'true');
      if (!ok && !bad) bad = f;
    });
    if (bad) { say(T.missing, 'err'); bad.focus(); return false; }
    return true;
  }
  function say(msg, kind) {
    status.textContent = msg;
    status.className = 'sp-status' + (kind ? ' sp-' + kind : '');
  }
  function crlf(s) { return encodeURIComponent(s.replace(/\n/g, '\r\n')); }

  function refresh() {
    preview.textContent = T.to + ADDR + '\n' + T.subj + subject() + '\n\n' + body();
  }

  // 依類別換提示文字、藏起不相關的技術欄位
  function onCat() {
    var c = byK.cat.value;
    var hint = (T.hints && T.hints[c]) || T.hints._;
    if (byK.desc) byK.desc.placeholder = hint.desc;
    if (byK.extra) byK.extra.placeholder = hint.extra;
    var tech = T.noTech.indexOf(c) < 0;
    root.querySelectorAll('.sp-tech').forEach(function (el) { el.classList.toggle('sp-hide', !tech); });
  }
  function onMedia() {
    var on = byK.media && byK.media.checked;
    root.querySelectorAll('.sp-media-on').forEach(function (el) { el.classList.toggle('sp-hide', !on); });
  }

  root.addEventListener('input', function (e) {
    if (e.target === byK.cat) onCat();
    if (e.target === byK.media) onMedia();
    if (e.target.getAttribute('aria-invalid') === 'true' && e.target.value.trim()) e.target.setAttribute('aria-invalid', 'false');
    refresh();
  });
  root.addEventListener('change', function (e) {
    if (e.target === byK.cat) onCat();
    if (e.target === byK.media) onMedia();
    refresh();
  });

  function copyText(text) {
    if (navigator.clipboard && window.isSecureContext) return navigator.clipboard.writeText(text);
    return new Promise(function (ok, no) {
      var ta = document.createElement('textarea');
      ta.value = text; ta.setAttribute('readonly', ''); ta.style.position = 'fixed'; ta.style.top = '-1000px';
      document.body.appendChild(ta); ta.select();
      try { document.execCommand('copy') ? ok() : no(); } catch (e) { no(e); }
      document.body.removeChild(ta);
    });
  }

  root.querySelectorAll('[data-act]').forEach(function (btn) {
    btn.addEventListener('click', function (e) {
      var act = btn.dataset.act;
      if (act === 'reset') {
        e.preventDefault();
        fields.forEach(function (f) { if (f.type === 'checkbox') f.checked = false; else f.value = ''; f.removeAttribute('aria-invalid'); });
        onCat(); onMedia(); refresh(); say('');
        return;
      }
      if (!check()) { e.preventDefault(); return; }
      var s = subject(), b = body();
      if (act === 'mailto') {
        var url = 'mailto:' + ADDR + '?subject=' + crlf(s) + '&body=' + crlf(b);
        btn.href = url; // 讓瀏覽器照一般連結開郵件程式
        say(url.length > 1900 ? T.long : T.opened, url.length > 1900 ? 'warn' : 'ok');
      } else if (act === 'gmail') {
        btn.href = 'https://mail.google.com/mail/?view=cm&fs=1&to=' + encodeURIComponent(ADDR) + '&su=' + encodeURIComponent(s) + '&body=' + encodeURIComponent(b);
        say(T.gmail, 'ok');
      } else if (act === 'copy') {
        e.preventDefault();
        copyText(T.to + ADDR + '\n' + T.subj + s + '\n\n' + b).then(function () { say(T.copied, 'ok'); }, function () {
          say(T.copyFail, 'warn');
          var d = root.querySelector('.sp-preview-box'); if (d) d.open = true;
        });
      }
    });
  });

  onCat(); onMedia(); refresh();
})();
