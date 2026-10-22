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
      btn.style.cssText = 'position:absolute;top:8px;right:8px;min-height:44px;padding:6px 14px;border:1px solid rgba(10,186,181,.6);border-radius:6px;background:#0abab5;color:#fff;font-size:.85rem;font-weight:700;cursor:pointer;z-index:1';
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

// ===== 2026-10-21 #67：指令參考 ?host=、首頁選擇器、右側目錄 =====
(function () {
  document.documentElement.classList.add('js');
  var reduced = window.matchMedia && window.matchMedia('(prefers-reduced-motion: reduce)').matches;

  // --- 指令參考：網址 ?host=spike 時只展開那個平台的 <details>（其他收起）；記住上次選的平台 ---
  var HOSTS = ['spike', 'ev3', 'arduino', 'mblock', 'matrix', 'microbit'];
  var host = (new URLSearchParams(location.search).get('host') || '').toLowerCase();
  if (HOSTS.indexOf(host) < 0) { try { host = localStorage.getItem('mbc-host') || ''; } catch (e) { host = ''; } }
  var plats = document.querySelectorAll('details.plat[data-host]');
  if (plats.length && HOSTS.indexOf(host) > -1 && document.querySelector('details.plat[data-host="' + host + '"]')) {
    plats.forEach(function (d) { d.open = d.dataset.host === host; });
  }
  plats.forEach(function (d) {
    d.addEventListener('toggle', function () { if (d.open) { try { localStorage.setItem('mbc-host', d.dataset.host); } catch (e) {} } });
  });

  // --- 首頁兩步選擇器：產品 × 主機 → 入口連結（照 ph-pick 的 radio 行為） ---
  var finder = document.getElementById('finder');
  var dataEl = document.getElementById('finder-data');
  if (finder && dataEl) {
    var data = JSON.parse(dataEl.textContent);
    var picked = { key: null, host: null };
    try { picked.key = localStorage.getItem('mbc-finder-key'); picked.host = localStorage.getItem('mbc-host'); } catch (e) {}
    var result = finder.querySelector('.finder-result');
    var table = finder.querySelector('.finder-table');
    if (table) table.removeAttribute('open');
    function pick(group, attr, value) {
      finder.querySelectorAll(group + ' button').forEach(function (b) { b.setAttribute('aria-checked', b.dataset[attr] === value ? 'true' : 'false'); });
    }
    function render() {
      var L = data.labels;
      if (!picked.key || !picked.host || !data.products[picked.key]) { result.innerHTML = '<p>' + L.result + '</p>'; return; }
      var p = data.products[picked.key];
      var h = p.hosts[picked.host];
      var hostName = finder.querySelector('.finder-hosts button[data-host="' + picked.host + '"] span').textContent;
      var html = '<h3>' + p.name + ' × ' + hostName + '</h3>';
      if (!h) html += '<p>' + L.none + '</p><div class="fr-links"><a class="ghost" href="' + p.index + '">' + p.name + '</a></div>';
      else {
        html += '<div class="fr-links">';
        var seen = {};
        ['start', 'ref', 'lib'].forEach(function (k, i) {
          if (!h[k] || seen[h[k]]) return;
          seen[h[k]] = 1;
          html += '<a class="' + (i ? 'ghost' : '') + '" href="' + h[k] + '"' + (/^https?:/.test(h[k]) ? ' target="_blank" rel="noopener"' : '') + '>' + L[k] + '</a>';
        });
        html += '</div>';
      }
      result.innerHTML = html;
    }
    finder.querySelectorAll('.finder-prods button').forEach(function (b) {
      b.addEventListener('click', function () { picked.key = b.dataset.key; pick('.finder-prods', 'key', picked.key); try { localStorage.setItem('mbc-finder-key', picked.key); } catch (e) {} render(); });
    });
    finder.querySelectorAll('.finder-hosts button').forEach(function (b) {
      b.addEventListener('click', function () { picked.host = b.dataset.host; pick('.finder-hosts', 'host', picked.host); try { localStorage.setItem('mbc-host', picked.host); } catch (e) {} render(); });
    });
    if (picked.key && data.products[picked.key]) pick('.finder-prods', 'key', picked.key); else picked.key = null;
    if (picked.host && HOSTS.indexOf(picked.host) > -1) pick('.finder-hosts', 'host', picked.host); else picked.host = null;
    render();
  }

  // --- 右側目錄：小螢幕收起；捲動時標示目前段落（IntersectionObserver）；reduced-motion 不平滑捲 ---
  var toc = document.querySelector('.toc');
  if (toc) {
    var det = toc.querySelector('details');
    var wide = window.matchMedia('(min-width: 1200px)');
    function fit() { if (det) det.open = wide.matches; }
    fit();
    if (wide.addEventListener) wide.addEventListener('change', fit);
    var links = toc.querySelectorAll('a[href^="#"]');
    var map = {};
    links.forEach(function (a) {
      var id = decodeURIComponent(a.getAttribute('href').slice(1));
      var el = document.getElementById(id);
      if (el) map[id] = a;
      a.addEventListener('click', function (e) {
        if (!el) return;
        e.preventDefault();
        var box = el.closest('details'); if (box && !box.open) box.open = true;
        el.scrollIntoView(reduced ? {} : { behavior: 'smooth', block: 'start' });
        history.replaceState(null, '', '#' + encodeURIComponent(id));
        if (!wide.matches && det) det.open = false;
      });
    });
    if ('IntersectionObserver' in window) {
      var current = null;
      var io = new IntersectionObserver(function (entries) {
        entries.forEach(function (en) { if (en.isIntersecting) current = en.target.id; });
        if (current && map[current]) { links.forEach(function (a) { a.classList.remove('active'); }); map[current].classList.add('active'); }
      }, { rootMargin: '-70px 0px -70% 0px', threshold: 0 });
      Object.keys(map).forEach(function (id) { io.observe(document.getElementById(id)); });
    }
  }
})();
