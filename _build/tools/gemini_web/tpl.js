async () => {
  const P = __PROMPT__, R = __RATIO__;
  const sleep = ms => new Promise(r => setTimeout(r, ms));
  const lab = e => (e.getAttribute('aria-label') || e.textContent || '').trim();
  const q = (sel, t) => [...document.querySelectorAll(sel)].find(e => lab(e).startsWith(t));
  const waitFor = async (fn, ms = 15000) => { const t = Date.now(); while (Date.now() - t < ms) { const v = fn(); if (v) return v; await sleep(300); } return null; };
  const log = [];
  await waitFor(() => document.querySelector('[role=textbox]'));
  if (!q('button', 'close 圖片')) {
    const tb = await waitFor(() => q('button', '上傳與工具')); if (!tb) return { err: 'no tools btn' };
    tb.click();
    const mi = await waitFor(() => [...document.querySelectorAll('[role=menuitemcheckbox]')].find(e => e.textContent.includes('建立圖像')));
    if (!mi) return { err: 'no image menu' };
    mi.click(); log.push('tool on'); await sleep(1000);
  }
  const rb = await waitFor(() => q('button', '顯示比例')); if (!rb) return { err: 'no ratio btn', log };
  if (!lab(rb).includes(R)) {
    rb.click();
    const rr = await waitFor(() => [...document.querySelectorAll('[role=menuitemradio]')].find(e => e.textContent.trim() === R));
    if (!rr) return { err: 'no ratio item', log };
    rr.click(); await sleep(600);
    document.body.dispatchEvent(new KeyboardEvent('keydown', { key: 'Escape', bubbles: true }));
    await sleep(400);
  }
  log.push('ratio=' + lab(q('button', '顯示比例') || document.body).slice(0, 20));
  const box = document.querySelector('[role=textbox]'); box.focus();
  document.execCommand('insertText', false, P); await sleep(800);
  const before = document.querySelectorAll('model-response img').length;
  const send = await waitFor(() => [...document.querySelectorAll('button')].find(b => /傳送|Send/.test(b.getAttribute('aria-label') || '') && !b.disabled), 5000);
  if (!send) return { err: 'no send btn', log };
  send.click(); log.push('sent');
  const img = await waitFor(() => [...document.querySelectorAll('model-response img')].filter(i => i.naturalWidth >= 256).length > before ? 1 : 0, 150000);
  if (!img) { const mr = [...document.querySelectorAll('model-response')].pop(); return { err: 'no image', log, url: location.href, reply: mr ? mr.innerText.slice(0, 400) : '' }; }
  await sleep(1500);
  const dl = [...document.querySelectorAll('button')].filter(b => (b.getAttribute('aria-label') || b.textContent).includes('下載原尺寸')).pop();
  if (!dl) return { err: 'no download btn', log, url: location.href };
  dl.click(); log.push('download clicked');
  const im = [...document.querySelectorAll('model-response img')].filter(i => i.naturalWidth >= 256).pop();
  return { ok: true, log, url: location.href, w: im.naturalWidth, h: im.naturalHeight };
}
