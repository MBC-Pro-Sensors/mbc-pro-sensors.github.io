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
    // cpp／python 之外，MakeCode（typescript）、clev3r（basic、vb）也要能複製；text 是序列埠輸出範例，不加
    document.querySelectorAll(['cpp', 'python', 'typescript', 'basic', 'vb'].map(function (l) { return '.markdown-section pre[data-lang="' + l + '"]'; }).join(', ')).forEach(function (pre) {
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

  // --- 互動教學總覽：產品 ▾ ＋ 主機 ▾ → 寫法按鈕（網址 #教學 先選那款；否則用上次選的 mbc.learn，或首頁選過的產品／主機） ---
  var lhData = document.getElementById('learn-hub-data');
  if (lhData) {
    var LH = JSON.parse(lhData.textContent), lhBox = document.getElementById('learn-hub-box');
    var lhP = document.getElementById('lh-p'), lhH = document.getElementById('lh-h');
    var lhKs = lhBox.querySelector('.lh-ks'), lhImg = lhBox.querySelector('.lh-img'), lhGuide = lhBox.querySelector('.lh-guide a');
    var lhProd = function () { return LH[lhP.selectedIndex]; };
    var lhDraw = function () {
      var p = lhProd(), h = p.hosts[lhH.selectedIndex];
      lhKs.textContent = '';
      h.ks.forEach(function (k) { var a = document.createElement('a'); a.href = k.u; a.title = k.n; a.textContent = k.s; lhKs.appendChild(a); });
      if (p.img) { lhImg.src = p.img; lhImg.hidden = false; } else lhImg.hidden = true;
      lhGuide.href = p.guide;
      try { localStorage.setItem('mbc.learn', p.t + '|' + h.h); } catch (e) {}
      lhFit();
    };
    // 一橫列放不下（超出視窗右緣）就改成換行
    var lhBar = lhBox.querySelector('.lh-bar');
    var lhFit = function () { lhBar.classList.remove('lh-wrap'); if (lhBar.getBoundingClientRect().right > document.documentElement.clientWidth - 16) lhBar.classList.add('lh-wrap'); };
    window.addEventListener('resize', lhFit);
    var lhHosts = function (keep) {
      var p = lhProd(), at = 0;
      lhH.textContent = '';
      p.hosts.forEach(function (h, i) { var o = document.createElement('option'); o.value = h.h; o.textContent = h.name; lhH.appendChild(o); if (h.h === keep) at = i; });
      lhH.selectedIndex = at;
      lhDraw();
    };
    var lhPick = function (t, host) {
      for (var i = 0; i < LH.length; i++) if (LH[i].t === t || LH[i].sites.indexOf(t) > -1) { lhP.selectedIndex = i; lhHosts(host); return true; }
      return false;
    };
    var saved = '', site = '', siteHost = '';
    try { saved = localStorage.getItem('mbc.learn') || ''; site = localStorage.getItem('mbc.product') || ''; siteHost = localStorage.getItem('mbc.host') || ''; } catch (e) {}
    var hashT = location.hash.slice(1);
    if (!(hashT && lhPick(hashT, saved.split('|')[1])) && !(saved && lhPick(saved.split('|')[0], saved.split('|')[1])) && !(site && lhPick(site, siteHost))) lhHosts(lhH.value);
    lhP.addEventListener('change', function () { lhHosts(lhH.value); });
    lhH.addEventListener('change', lhDraw);
  }

  // --- 首頁三步選擇器：產品 → 主機 → 軟體 → 直接到軟體頁（記住 mbc.product／mbc.host／mbc.sw） ---
  var finder = document.getElementById('finder-box');
  var dataEl = document.getElementById('finder-data');
  if (finder && dataEl) {
    var data = JSON.parse(dataEl.textContent);
    var picked = { key: null, host: null, sw: null };
    try { picked.key = localStorage.getItem('mbc.product'); picked.host = localStorage.getItem('mbc.host'); picked.sw = localStorage.getItem('mbc.sw'); } catch (e) {}
    var result = finder.querySelector('.finder-result');
    var swBox = finder.querySelector('.finder-sws');
    var table = finder.querySelector('.finder-table');
    if (table) table.removeAttribute('open');
    var save = function (k, v) { try { localStorage.setItem(k, v); } catch (e) {} };
    var mark = function (group, attr, value) {
      finder.querySelectorAll(group + ' button').forEach(function (b) { b.setAttribute('aria-checked', b.dataset[attr] === value ? 'true' : 'false'); });
    };
    function render() {
      var p = data.products[picked.key];
      // 第 2 步：只開這款有的主機
      finder.querySelectorAll('.finder-hosts button').forEach(function (b) {
        var ok = !!(p && p.hosts[b.dataset.host]);
        b.disabled = !ok;
        b.title = ok || !p ? '' : data.labels.noHost;
      });
      if (!p || !p.hosts[picked.host]) picked.host = null;
      mark('.finder-hosts', 'host', picked.host);
      // 第 3 步：這個主機的軟體；只有一種就直接選
      var h = p && picked.host ? p.hosts[picked.host] : null;
      swBox.innerHTML = '';
      if (h) {
        var ids = Object.keys(h.sws);
        if (ids.indexOf(picked.sw) < 0) picked.sw = ids.length === 1 ? ids[0] : null;
        ids.forEach(function (id) {
          var s = data.hosts[picked.host].sws[id];
          var b = document.createElement('button');
          b.type = 'button'; b.setAttribute('role', 'radio'); b.dataset.sw = id;
          b.className = 'ft-' + picked.host;
          b.setAttribute('aria-checked', id === picked.sw ? 'true' : 'false');
          b.innerHTML = '<span></span><small></small>';
          b.firstChild.textContent = s.name; b.lastChild.textContent = s.tag;
          b.addEventListener('click', function () { picked.sw = id; save('mbc.sw', id); render(); });
          swBox.appendChild(b);
        });
      } else picked.sw = null;
      if (p && h && picked.sw) {
        var hostName = data.hosts[picked.host].name, swName = data.hosts[picked.host].sws[picked.sw].name;
        result.innerHTML = '<div class="fr-links"><a></a></div>';
        var a = result.querySelector('a');
        a.href = h.sws[picked.sw];
        a.textContent = data.labels.go + ' ' + p.name + ' › ' + hostName + ' › ' + swName + ' →';
      } else result.innerHTML = '<p>' + data.labels.result + '</p>';
    }
    finder.querySelectorAll('.finder-prods button').forEach(function (b) {
      b.addEventListener('click', function () { picked.key = b.dataset.key; mark('.finder-prods', 'key', picked.key); save('mbc.product', picked.key); render(); });
    });
    finder.querySelectorAll('.finder-hosts button').forEach(function (b) {
      b.addEventListener('click', function () { if (b.disabled) return; picked.host = b.dataset.host; save('mbc.host', picked.host); render(); });
    });
    if (!data.products[picked.key]) picked.key = null;
    mark('.finder-prods', 'key', picked.key);
    render();
  }

  // --- 切換列（第 2、3 層頁面）：一次只開一個下拉；Esc 關閉；點外面關閉；記住目前的產品／主機／軟體 ---
  var gsw = document.querySelector('.gsw');
  if (gsw) {
    var segs = gsw.querySelectorAll('details.gsw-seg');
    segs.forEach(function (d) {
      d.addEventListener('toggle', function () {
        if (!d.open) return;
        segs.forEach(function (o) { if (o !== d) o.open = false; });
        var cur = d.querySelector('a[aria-current="page"]') || d.querySelector('a');
        if (cur && d.matches(':focus-within')) cur.focus();
      });
      d.addEventListener('keydown', function (e) {
        if (e.key === 'Escape' && d.open) { d.open = false; d.querySelector('summary').focus(); }
      });
    });
    document.addEventListener('click', function (e) { if (!gsw.contains(e.target)) segs.forEach(function (o) { o.open = false; }); });
    try {
      localStorage.setItem('mbc.product', gsw.dataset.p);
      localStorage.setItem('mbc.host', gsw.dataset.h);
      if (gsw.dataset.s) localStorage.setItem('mbc.sw', gsw.dataset.s);
    } catch (e) {}
  }

  // --- 產品頁「① 選你的主機」：上次選過的主機／軟體這款也有 → 顯示「繼續用 X › Y →」 ---
  document.querySelectorAll('.pick').forEach(function (box) {
    var el = box.querySelector('.pick-data');
    var cont = box.querySelector('.pick-continue');
    if (!el || !cont) return;
    var d = JSON.parse(el.textContent);
    var h = null, s = null;
    try { h = localStorage.getItem('mbc.host'); s = localStorage.getItem('mbc.sw'); } catch (e) {}
    var host = h && d.hosts[h];
    if (!host) return;
    var sw = s && host.sws[s];
    var a = document.createElement('a');
    a.href = sw ? sw.url : host.url;
    a.textContent = d.cont + ' ' + host.name + (sw ? ' › ' + sw.name : '') + ' →';
    cont.appendChild(a);
    cont.hidden = false;
    var tile = box.querySelector('.pick-tile[data-host="' + h + '"]');
    if (tile) tile.classList.add('last');
  });

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

// 目錄位置：手機（<1200px）放文章開頭、有切換列（產品 › 主機 › 軟體）就放它下方；桌機放右側欄（.doc-wrap 最後）。
// 2026-10-24：build.mjs 已直接把目錄寫在手機位置，桌機由目錄裡的小腳本在解析當下搬過去；這裡只處理之後視窗寬度改變
(function () {
  var toc = document.querySelector('nav.toc'), wrap = toc && toc.closest('.doc-wrap'), art = wrap && wrap.querySelector('article');
  if (!toc || !wrap || !art) return;
  var sw = art.querySelector('nav.gsw');
  var mq = window.matchMedia('(min-width: 1200px)');
  function place() {
    if (mq.matches) { if (toc.parentElement !== wrap) wrap.appendChild(toc); }
    else if (sw) { if (toc.previousElementSibling !== sw) sw.insertAdjacentElement('afterend', toc); }
    else if (art.firstElementChild !== toc) art.insertBefore(toc, art.firstChild);
  }
  place();
  if (mq.addEventListener) mq.addEventListener('change', place); else if (mq.addListener) mq.addListener(place);
})();

// YouTube：點縮圖才換成影片（youtube-nocookie、自動播放）；按 Ctrl／Shift／中鍵照常開新分頁
document.addEventListener('click', function (e) {
  var a = e.target.closest && e.target.closest('a.yt-lite');
  if (!a || e.ctrlKey || e.metaKey || e.shiftKey || e.button) return;
  e.preventDefault();
  var f = document.createElement('iframe');
  f.src = 'https://www.youtube-nocookie.com/embed/' + a.getAttribute('data-yt') + '?autoplay=1&rel=0';
  f.title = a.getAttribute('data-title') || 'YouTube';
  f.allow = 'accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share';
  f.allowFullscreen = true;
  f.className = 'yt-on';
  if (a.getAttribute('style')) f.setAttribute('style', a.getAttribute('style'));
  a.replaceWith(f);
  f.focus();
});
