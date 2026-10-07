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

  // --- 首頁頂部導覽（手機版漢堡選單）---
  var topnav = document.querySelector('.topnav');
  var burger = document.querySelector('.tn-burger');
  if (topnav && burger) {
    burger.addEventListener('click', function () { topnav.classList.toggle('open'); });
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

  // --- 詢價／下載點擊追蹤（有安裝 Google Analytics 時才送出）---
  document.addEventListener('click', function (e) {
    var a = e.target.closest && e.target.closest('a');
    if (!a || typeof window.gtag !== 'function') return;
    var href = a.getAttribute('href') || '';
    var name = /line\.me/.test(href) ? 'line_inquiry'
      : /^mailto:/.test(href) ? 'email_inquiry'
      : /\.(zip|py|pdf|llsp3|lms|lmsp|ev3|bp)(\?|$)/i.test(href) ? 'file_download'
      : null;
    if (name) window.gtag('event', name, { link_url: href, page_path: location.pathname, placement: a.className || 'link' });
  });

  // --- Scratch 積木圖 ---
  if (typeof renderScratchBlocks === 'function' && document.querySelector('.blocks')) renderScratchBlocks();
})();
