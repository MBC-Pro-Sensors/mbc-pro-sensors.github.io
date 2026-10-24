async () => {
  const P = __PROMPT__;
  const sleep = ms => new Promise(r => setTimeout(r, ms));
  const waitFor = async (fn, ms = 15000) => { const t = Date.now(); while (Date.now() - t < ms) { const v = fn(); if (v) return v; await sleep(300); } return null; };
  const imgs = () => [...document.querySelectorAll('model-response img')].filter(i => i.naturalWidth >= 256);
  await waitFor(() => imgs().length > 0, 20000);
  const before = imgs().length;
  const box = await waitFor(() => document.querySelector('[role=textbox]')); box.focus();
  document.execCommand('insertText', false, P); await sleep(800);
  const send = await waitFor(() => [...document.querySelectorAll('button')].find(b => /傳送|Send/.test(b.getAttribute('aria-label') || '') && !b.disabled), 5000);
  if (!send) return { err: 'no send btn', before };
  send.click();
  const ok = await waitFor(() => imgs().length > before, 150000);
  if (!ok) { const mr = [...document.querySelectorAll('model-response')].pop(); return { err: 'no new image', before, reply: mr ? mr.innerText.slice(0, 400) : '' }; }
  await sleep(1500);
  const dl = [...document.querySelectorAll('button')].filter(b => (b.getAttribute('aria-label') || b.textContent).includes('下載原尺寸')).pop();
  if (!dl) return { err: 'no download btn' };
  dl.click();
  return { ok: true, before, after: imgs().length, url: location.href };
}
