// MBC-Pro Sensors static site behaviour (replaces the former Docsify plugins)
(function () {
  var lang = document.documentElement.lang === 'en' ? 'en' : 'zh';

  // --- 側邊欄摺疊：預設收起，目前頁面所在的分類自動展開 ---
  var sidebarNav = document.querySelector('.sidebar-nav');
  if (sidebarNav) {
    sidebarNav.querySelectorAll('li').forEach(function (li) {
      var ul = li.querySelector('ul');
      if (!ul) return;
      li.classList.add('has-children');
      var toggleDiv = document.createElement('div');
      toggleDiv.className = 'menu-toggle';
      while (li.firstChild && li.firstChild !== ul) toggleDiv.appendChild(li.firstChild);
      li.insertBefore(toggleDiv, ul);
      toggleDiv.onclick = function (e) {
        if (e.target.tagName !== 'A') e.preventDefault();
        li.classList.toggle('closed');
      };
      if (!li.querySelector('a.active')) li.classList.add('closed');
    });
  }

  // --- 手機版選單按鈕 ---
  var toggle = document.querySelector('.sidebar-toggle');
  if (toggle) toggle.addEventListener('click', function () { document.body.classList.toggle('close'); });
  // 文件頁頂部「目錄」按鈕：手機版開關側欄（body.close = 側欄展開，沿用 docsify 的寫法）
  var toc = document.querySelector('.tn-toc');
  if (toc) toc.addEventListener('click', function () { document.body.classList.toggle('close'); });
  document.querySelectorAll('.sidebar-nav a').forEach(function (a) {
    a.addEventListener('click', function () { document.body.classList.remove('close'); });
  });

  // --- 頂部導覽（手機版漢堡選單）---
  var topnav = document.querySelector('.topnav');
  var burger = document.querySelector('.tn-burger');
  if (topnav && burger) {
    burger.addEventListener('click', function () {
      burger.setAttribute('aria-expanded', topnav.classList.toggle('open') ? 'true' : 'false');
    });
    topnav.querySelectorAll('.tn-links a').forEach(function (a) {
      a.addEventListener('click', function () { topnav.classList.remove('open'); });
    });
  }

  // --- 站內搜尋（第一次聚焦時才載入索引）---
  var input = document.querySelector('.search input');
  var panel = document.querySelector('.search .results-panel');
  var clearBtn = document.querySelector('.search .clear-button');
  var index = null;
  var loading = null;

  function loadIndex() {
    if (!loading) {
      loading = fetch('/search-index-' + lang + '.json')
        .then(function (r) { return r.json(); })
        .then(function (data) { index = data; });
    }
    return loading;
  }

  function escapeHtml(s) {
    return s.replace(/[&<>"]/g, function (c) { return { '&': '&amp;', '<': '&lt;', '>': '&gt;', '"': '&quot;' }[c]; });
  }

  function highlight(text, q) {
    var i = text.toLowerCase().indexOf(q);
    if (i < 0) return escapeHtml(text.slice(0, 80));
    var start = Math.max(0, i - 30);
    return (start ? '…' : '') + escapeHtml(text.slice(start, i)) + '<em>' + escapeHtml(text.slice(i, i + q.length)) + '</em>' + escapeHtml(text.slice(i + q.length, i + q.length + 50)) + '…';
  }

  function search() {
    var q = input.value.trim().toLowerCase();
    if (!q) { panel.classList.remove('show'); panel.innerHTML = ''; return; }
    var hits = index.filter(function (p) { return (p.t + ' ' + p.x).toLowerCase().indexOf(q) > -1; })
      .sort(function (a, b) { return (b.t.toLowerCase().indexOf(q) > -1) - (a.t.toLowerCase().indexOf(q) > -1); })
      .slice(0, 12);
    panel.innerHTML = hits.length
      ? hits.map(function (p) {
          return '<div class="matching-post"><a href="' + p.u + '"><h2>' + escapeHtml(p.t) + '</h2><p>' + highlight(p.x, q) + '</p></a></div>';
        }).join('')
      : '<div class="empty">' + (lang === 'en' ? 'No results' : '😞 找不到結果') + '</div>';
    panel.classList.add('show');
  }

  if (input && panel) {
    input.addEventListener('focus', loadIndex);
    input.addEventListener('input', function () { loadIndex().then(search); });
    if (clearBtn) clearBtn.addEventListener('click', function () { input.value = ''; search(); });
  }

  // --- 產品頁：選版本 → 詢價連結自動帶入「產品＋版本」；底部詢價列也跟著產品頁的連結 ---
  var heroQuote = document.querySelector('.ph-cta .buy-btn.line');
  var dockQuote = document.querySelector('.cta-dock .dock-line');
  if (heroQuote && dockQuote) dockQuote.href = heroQuote.href;
  document.querySelectorAll('.ph-pick button').forEach(function (btn, i, all) {
    btn.addEventListener('click', function () {
      all.forEach(function (b) { b.setAttribute('aria-checked', b === btn ? 'true' : 'false'); });
      if (heroQuote) heroQuote.href = btn.dataset.href;
      if (dockQuote) dockQuote.href = btn.dataset.href;
    });
  });

  // --- 產品頁可展開區塊：網址帶 #錨點 時自動展開該區塊 ---
  function openTarget() {
    var id = decodeURIComponent(location.hash.slice(1));
    var el = id && document.getElementById(id);
    var box = el && el.closest('details');
    if (box && !box.open) { box.open = true; el.scrollIntoView(); }
  }
  openTarget();
  window.addEventListener('hashchange', openTarget);

  // --- 詢價／下載點擊追蹤（有安裝 Google Analytics 時才送出）---
  document.addEventListener('click', function (e) {
    var a = e.target.closest && e.target.closest('a');
    if (!a || typeof window.gtag !== 'function') return;
    var href = a.getAttribute('href') || '';
    var name = /line\.me/.test(href) ? 'line_inquiry'
      : /^mailto:/.test(href) ? 'email_inquiry'
      : /\.(zip|py|pdf|llsp3|lms|lmsp|ev3|bp|ino)(\?|$)/i.test(href) ? 'file_download'
      : null;
    if (name) window.gtag('event', name, { link_url: href, page_path: location.pathname, placement: a.className || 'link' });
  });

  // --- 程式碼區塊右上角「複製」按鈕（新手照抄範例用）---
  if (navigator.clipboard) {
    document.querySelectorAll('.markdown-section pre[data-lang="cpp"], .markdown-section pre[data-lang="python"]').forEach(function (pre) {
      var btn = document.createElement('button');
      btn.type = 'button';
      btn.className = 'code-copy';
      btn.textContent = lang === 'en' ? 'Copy' : '複製';
      btn.style.cssText = 'position:absolute;top:8px;right:8px;min-height:36px;padding:6px 14px;border:1px solid rgba(10,186,181,.6);border-radius:6px;background:#0abab5;color:#fff;font-size:.85rem;font-weight:700;cursor:pointer;z-index:1';
      pre.style.position = 'relative';
      btn.addEventListener('click', function () {
        navigator.clipboard.writeText(pre.querySelector('code').innerText).then(function () {
          btn.textContent = lang === 'en' ? 'Copied ✓' : '已複製 ✓';
          setTimeout(function () { btn.textContent = lang === 'en' ? 'Copy' : '複製'; }, 2000);
        });
      });
      pre.appendChild(btn);
    });
  }

  // --- Scratch 積木圖 ---
  if (typeof renderScratchBlocks === 'function' && document.querySelector('.blocks')) renderScratchBlocks();
})();
