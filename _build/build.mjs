// Static site builder for mbc-pro-sensors.github.io
//
// Renders every Markdown page into its own HTML file (same look as the former
// Docsify site) so search engines can index each product / tutorial page.
//
//   README.md                 -> /
//   en/README.md              -> /en/
//   sensors/line8/index.md    -> /sensors/line8/
//   sensors/line8/foo.md      -> /sensors/line8/foo.html
//
// Usage:  cd _build && npm ci && node build.mjs      (output: ../_site)
import fs from 'node:fs';
import path from 'node:path';
import { execSync } from 'node:child_process';
import { createRequire } from 'node:module';
import { fileURLToPath } from 'node:url';
import { stripHidden } from './site-flags.mjs';

const require = createRequire(import.meta.url);
const marked = require('marked'); // v1.2.9 — same version Docsify 4.13 bundles
const Prism = require('prismjs');
require('prismjs/components/prism-python');
require('prismjs/components/prism-c');
require('prismjs/components/prism-cpp');
require('prismjs/components/prism-typescript'); // micro:bit MakeCode 片段
require('prismjs/components/prism-basic');      // clev3r 片段

const BUILD = path.dirname(fileURLToPath(import.meta.url));
const ROOT = path.resolve(BUILD, '..');
const OUT = path.join(ROOT, '_site');
const SITE = 'https://mbc-pro-sensors.github.io';
const DEFAULT_OG = '/images/brand/og-image.jpg';
// Google Analytics (GA4): set gaMeasurementId in _build/site-config.json to enable
const SITE_CONFIG = JSON.parse(fs.readFileSync(path.join(BUILD, 'site-config.json'), 'utf8'));
const GA_ID = (SITE_CONFIG.gaMeasurementId || '').trim();
const GA_SNIPPET = GA_ID ? `<script async src="https://www.googletagmanager.com/gtag/js?id=${GA_ID}"></script>
  <script>window.dataLayer=window.dataLayer||[];function gtag(){dataLayer.push(arguments);}gtag('js',new Date());gtag('config','${GA_ID}');</script>` : '';

const git = (args) => execSync(`git ${args}`, { cwd: ROOT, encoding: 'utf8' });
const tracked = git('-c core.quotepath=off ls-files --cached --others --exclude-standard').split('\n').filter(Boolean);

// ---------------------------------------------------------------- URL mapping
const isPage = (f) => f.endsWith('.md') && (f === 'README.md' || f === 'contact.md' || /^(en\/)?(README|contact|i2c-protocol|makeblock|guide|privacy|wro-rules|competitions|faq|where-to-buy|distributors|reference|matrix|microbit|downloads)\.md$/.test(f) || /^(en\/)?(sensors|articles)\//.test(f) || /^(en\/)?learn\/index\.md$/.test(f));
const pages = tracked.filter(isPage);

function urlFor(mdPath) {
  let p = mdPath.replace(/^\/+/, '').replace(/\.md$/, '');
  if (p === '' || p === 'README') return '/';
  if (p === 'en' || p === 'en/' || p === 'en/README') return '/en/';
  if (p.endsWith('/')) return '/' + p;
  if (p === 'index' || p.endsWith('/index')) return '/' + p.slice(0, -'index'.length);
  return '/' + p + '.html';
}

// Map any internal link (Docsify hash route, .md path, relative .md) to a static URL
function mapHref(href, fromFile) {
  let m = href.match(/^#\/(.*)$/);
  let target, frag = '';
  if (m) {
    target = m[1];
  } else if (/^[^:#?]+\.md([?#].*)?$/.test(href)) {
    target = href.startsWith('/') ? href : path.posix.join(path.posix.dirname(fromFile), href);
  } else {
    return null;
  }
  const q = target.search(/[?#]/);
  if (q > -1) {
    const id = target.slice(q).match(/id=([^&]+)/) || target.slice(q).match(/^#(.+)/);
    if (id) frag = '#' + id[1];
    target = target.slice(0, q);
  }
  return urlFor(target) + frag;
}

// ------------------------------------------------------------ Docsify slugify
function makeSlugger() {
  const cache = {};
  return (str) => {
    let slug = str.trim().replace(/[A-Z]+/g, (s) => s.toLowerCase()).replace(/<[^>]+>/g, '')
      .replace(/[ -⁯⸀-⹿\\'!"#$%&()*+,./:;<=>?@[\]^`{|}~]/g, '')
      .replace(/\s/g, '-').replace(/-+/g, '-').replace(/^(\d)/, '_$1');
    const count = Object.prototype.hasOwnProperty.call(cache, slug) ? cache[slug] + 1 : 0;
    cache[slug] = count;
    return count ? `${slug}-${count}` : slug;
  };
}

// ------------------------------------------------------------------ Markdown
const CALLOUT_LABEL = {
  zh: { NOTE: '注意', TIP: '提示', IMPORTANT: '重要', WARNING: '警告', CAUTION: '危險' },
  en: { NOTE: 'Note', TIP: 'Tip', IMPORTANT: 'Important', WARNING: 'Warning', CAUTION: 'Caution' },
};

function renderMarkdown(src, file, lang) {
  const slug = makeSlugger();
  const renderer = new marked.Renderer();
  renderer.heading = (text, level) => {
    const id = slug(text);
    return `<h${level} id="${id}"><a href="#${encodeURI(id)}" data-id="${id}" class="anchor"><span>${text}</span></a></h${level}>`;
  };
  renderer.code = (code, info) => {
    const lang = (info || '').trim().split(/\s+/)[0] || 'markup';
    const grammar = Prism.languages[lang] || Prism.languages.markup;
    return `<pre data-lang="${lang}"><code class="lang-${lang}">${Prism.highlight(code, grammar, lang)}</code></pre>`;
  };
  renderer.link = (href, title, text) => {
    const mapped = href && mapHref(href, file);
    const t = title ? ` title="${title}"` : '';
    if (mapped) return `<a href="${mapped}"${t}>${text}</a>`;
    if (/^https?:/.test(href)) return `<a href="${href}"${t} target="_blank" rel="noopener">${text}</a>`;
    return `<a href="${href}"${t}>${text}</a>`;
  };
  renderer.image = (href, title, text) =>
    `<img src="${href}" alt="${text || ''}"${title ? ` title="${title}"` : ''}>`;

  // MkDocs-style admonition (!!! success "Title" + one line) -> GitHub-style tip callout
  src = src.replace(/^!!! \w+ "([^"]+)"\r?\n([^\r\n]+)/gm, (all, title, body) => `> [!TIP]\n> **${title}**\n> ${body}`);
  let html = marked(src, { renderer, gfm: true });

  // Raw-HTML links inside the Markdown (e.g. product cards on the home page)
  html = html.replace(/href="([^"]+)"/g, (all, href) => {
    const mapped = mapHref(href, file);
    return mapped ? `href="${mapped}"` : all;
  });
  // GitHub-style alerts: > [!NOTE] ...
  html = html.replace(/<blockquote>\s*<p>\[!(NOTE|TIP|IMPORTANT|WARNING|CAUTION)\]\s*/g, (all, kind) =>
    `<blockquote class="callout callout-${kind.toLowerCase()}"><p class="callout-title">${CALLOUT_LABEL[lang][kind]}</p><p>`);
  // Lazy-load everything except the first image (likely the hero / LCP image)
  let imgCount = 0;
  html = html.replace(/<img(?![^>]*\bloading=)/g, (all) => (imgCount++ === 0 ? all : '<img loading="lazy" decoding="async"'));
  html = html.replace(/<iframe(?![^>]*\bloading=)/g, '<iframe loading="lazy"');
  // privacy-enhanced YouTube embeds (no tracking cookies)
  html = html.replace(/https:\/\/www\.youtube\.com\/embed\//g, 'https://www.youtube-nocookie.com/embed/');
  return html;
}

// ------------------------------------------------------------------- icons
// Emoji used as icons (headings, buttons, card icons) -> inline Lucide SVG line icons (_build/icons.json, ISC).
// Status marks (✅ ❌ 🟢 🟡) stay as emoji: their color is the meaning.
const ICONS = JSON.parse(fs.readFileSync(path.join(BUILD, 'icons.json'), 'utf8'));
const icon = (name, cls = 'i') => `<svg class="${cls}" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" aria-hidden="true" focusable="false">${ICONS[name]}</svg>`;
const EMOJI_ICON = Object.fromEntries(Object.entries({
  '🏆': 'trophy', '🥈': 'medal', '🧩': 'puzzle', '🚀': 'rocket', '🎯': 'target', '🧱': 'blocks', '📋': 'clipboard-list',
  '🔌': 'plug', '📊': 'chart-column', '📤': 'upload', '🐍': 'code', '📥': 'download', '📡': 'radar', '🧠': 'brain',
  '📐': 'ruler', '📏': 'ruler', '⚠️': 'triangle-alert', '🔄': 'refresh-cw', '⚙️': 'settings', '⚡': 'zap', '❓': 'circle-help',
  '💻': 'laptop', '🎨': 'palette', '📦': 'package', '💓': 'activity', '🤖': 'bot', '🛡️': 'shield', '🎮': 'gamepad-2',
  '🛠️': 'wrench', '🛑': 'octagon-x', '🚗': 'car', '🔬': 'microscope', '📺': 'monitor', '🚦': 'traffic-cone', '🌐': 'languages',
  '🌏': 'earth', '📚': 'book-open', '🔋': 'battery', '🏁': 'flag', '🤝': 'handshake', '🧭': 'compass', '🗂️': 'folder',
  '🧾': 'receipt', '🔒': 'lock', '🎛️': 'sliders-horizontal', '🌈': 'rainbow', '⏳': 'hourglass', '🔗': 'link',
  '💡': 'lightbulb', '🛒': 'shopping-cart', '🎉': 'party-popper', '📝': 'notebook-pen', '🗑️': 'trash-2', '🛤️': 'route',
  '💬': 'message-circle', '✉️': 'mail', '🔍': 'search', '📄': 'file-text', '📖': 'book-open', '🔙': 'arrow-left', '📬': 'inbox',
  '🆘': 'life-buoy', '📈': 'trending-up', '👉': 'arrow-right', '📍': 'map-pin', '🔦': 'flashlight', '🔀': 'shuffle', '👁️': 'eye', '🏫': 'school', '💼': 'briefcase', '📣': 'megaphone', '📸': 'camera', '📞': 'phone', '📌': 'pin', '🧪': 'flask-conical',
}).map(([e, n]) => [e.replace(/️/g, ''), n]));
function iconize(html) {
  const swap = (all, pre, e) => (EMOJI_ICON[e] ? pre + icon(EMOJI_ICON[e]) : all);
  return html
    .replace(/(<(?:div|span) class="(?:lp-ico|lp-noimg|section-title|reason-icon|step-icon|channel-icon-wrap)">)(\p{Extended_Pictographic})️?/gu, swap)
    .replace(/(<h[1-4][^>]*>(?:<a [^>]*>)?(?:<span>)?|<a\b[^>]*>|<(?:p|li)\b[^>]*>(?:<strong>)?|<summary>)(\p{Extended_Pictographic})️?[  ]*/gu, swap);
}

// Intrinsic image size (PNG / JPEG / GIF / WebP) so every <img> reserves its space (no layout shift)
const sizeCache = new Map();
function imageSize(src) {
  if (sizeCache.has(src)) return sizeCache.get(src);
  let size = null;
  try {
    const b = fs.readFileSync(path.join(ROOT, decodeURI(src)));
    if (/\.svg$/i.test(src)) {
      // SVG：先讀根元素的 width/height，沒有就用 viewBox 的寬高（產生器出的圖兩者都有）
      const root = (b.toString('utf8').match(/<svg\b[^>]*>/) || [''])[0];
      const wh = [root.match(/\swidth="(\d+(?:\.\d+)?)(?:px)?"/), root.match(/\sheight="(\d+(?:\.\d+)?)(?:px)?"/)];
      const vb = root.match(/viewBox="\s*[-\d.]+[\s,]+[-\d.]+[\s,]+([\d.]+)[\s,]+([\d.]+)\s*"/);
      if (wh[0] && wh[1]) size = [Math.round(+wh[0][1]), Math.round(+wh[1][1])];
      else if (vb) size = [Math.round(+vb[1]), Math.round(+vb[2])];
    } else if (b.toString('ascii', 1, 4) === 'PNG') size = [b.readUInt32BE(16), b.readUInt32BE(20)];
    else if (b.toString('ascii', 0, 3) === 'GIF') size = [b.readUInt16LE(6), b.readUInt16LE(8)];
    else if (b.toString('ascii', 0, 4) === 'RIFF' && b.toString('ascii', 8, 12) === 'WEBP') {
      const kind = b.toString('ascii', 12, 16);
      if (kind === 'VP8 ') size = [b.readUInt16LE(26) & 0x3fff, b.readUInt16LE(28) & 0x3fff];
      else if (kind === 'VP8L') { const n = b.readUInt32LE(21); size = [(n & 0x3fff) + 1, ((n >> 14) & 0x3fff) + 1]; }
      else if (kind === 'VP8X') size = [b.readUIntLE(24, 3) + 1, b.readUIntLE(27, 3) + 1];
    } else if (b[0] === 0xff && b[1] === 0xd8) {
      for (let i = 2; i < b.length - 9;) {
        const m = b[i + 1];
        if (m >= 0xc0 && m <= 0xcf && m !== 0xc4 && m !== 0xc8 && m !== 0xcc) { size = [b.readUInt16BE(i + 7), b.readUInt16BE(i + 5)]; break; }
        i += 2 + b.readUInt16BE(i + 2);
      }
    }
  } catch { /* missing or remote image: leave as is */ }
  sizeCache.set(src, size);
  return size;
}
const sizeImages = (html) => html.replace(/<img\b[^>]*>/g, (tag) => {
  if (!/\balt=/.test(tag)) tag = tag.replace('<img', '<img alt=""');
  if (/\bwidth=/.test(tag) || /style="(?:[^"]*[;\s])?(?:width|height)\s*:/.test(tag)) return tag; // inline size wins: adding width/height would distort it
  const src = (tag.match(/\bsrc="(\/[^"?#]+)"/) || [])[1];
  const s = src && imageSize(src);
  return s ? tag.replace('<img', `<img width="${s[0]}" height="${s[1]}"`) : tag;
});
// YouTube embeds -> click-to-load facade: self-hosted thumbnail (images/yt/<ID>.webp, from _build/tools/yt_thumbs.py),
// nothing is fetched from YouTube until the visitor clicks; without JS the link opens the video on YouTube.
const ytMissing = new Set();
const ytLite = (html, en) => html.replace(/<iframe\b([^>]*?)\bsrc="https:\/\/www\.youtube(?:-nocookie)?\.com\/embed\/([A-Za-z0-9_-]{11})[^"]*"([^>]*)>\s*<\/iframe>/g, (all, a1, id, a2) => {
  const attrs = a1 + a2;
  const title = (attrs.match(/\btitle="([^"]*)"/) || [])[1] || '';
  const style = (attrs.match(/\bstyle="([^"]*)"/) || [])[1];
  if (!fs.existsSync(path.join(ROOT, 'images', 'yt', id + '.webp'))) ytMissing.add(id);
  const label = (en ? 'Play video' : '播放影片') + (title && !/^YouTube video player$/i.test(title) ? (en ? ': ' : '：') + title : '');
  return `<a class="yt-lite" href="https://www.youtube.com/watch?v=${id}" data-yt="${id}" data-title="${escAttr(title || label)}" target="_blank" rel="noopener" aria-label="${escAttr(label)}"${style ? ` style="${style}"` : ''}>` +
    `<img src="/images/yt/${id}.webp" alt="" loading="lazy" decoding="async"><span class="yt-play" aria-hidden="true"></span></a>`;
});
// Page-level post-processing of everything inside <body>
const finishPage = (doc) => {
  const i = doc.indexOf('<body');
  return doc.slice(0, i) + sizeImages(iconize(ytLite(doc.slice(i), /<html lang="en/.test(doc))));
};

// ------------------------------------------------------------------- helpers
const stripTags = (html) => html
  .replace(/<(style|script)[\s\S]*?<\/\1>/g, ' ')
  .replace(/<[^>]+>/g, ' ')
  .replace(/&nbsp;/g, ' ').replace(/&amp;/g, '&').replace(/&lt;/g, '<').replace(/&gt;/g, '>').replace(/&quot;/g, '"').replace(/&#39;/g, "'")
  .replace(/\s+/g, ' ').trim();
const escAttr = (s) => s.replace(/&/g, '&amp;').replace(/"/g, '&quot;').replace(/</g, '&lt;');
const stripEmoji = (s) => s.replace(/[\p{Extended_Pictographic}️‍]/gu, '').replace(/\s+/g, ' ').trim();

function describe(html) {
  // first meaningful paragraph / blockquote text after the H1
  const body = html.replace(/^[\s\S]*?<\/h1>/, '');
  const withoutCallouts = body.replace(/<blockquote class="callout[\s\S]*?<\/blockquote>/g, '');
  const chunks = withoutCallouts.match(/<(p|blockquote)[^>]*>[\s\S]*?<\/\1>/g) || [];
  for (const c of chunks) {
    // skip navigation-only paragraphs such as "← Back to 8-Way Line Follower Home"
    if (/^<p[^>]*>\s*<a [^>]*>[\s\S]*?<\/a>\s*<\/p>$/.test(c)) continue;
    const t = stripEmoji(stripTags(c));
    if (/^(←|Back to|返回)/.test(t)) continue;
    if (t.length >= 20) return t.length > 155 ? t.slice(0, 152) + '…' : t;
  }
  return '';
}

function firstCommit(file) {
  try { return git(`log --diff-filter=A --format=%cs -- "${file}"`).trim().split('\n').pop() || lastmod(file); }
  catch { return lastmod(file); }
}

function lastmod(file) {
  try { return git(`log -1 --format=%cs -- "${file}"`).trim() || new Date().toISOString().slice(0, 10); }
  catch { return new Date().toISOString().slice(0, 10); }
}

// Product names from the sidebars: "- [循行者 8 路 (Line8)](/sensors/line8/index.md)" (one line per product)
function productNames(sidebarFile) {
  const names = {};
  const lines = fs.readFileSync(path.join(ROOT, sidebarFile), 'utf8').split('\n');
  for (const line of lines) {
    const m = line.match(/^\s+- \[(?:\*\*)?(.+?)(?:\*\*)?\]\((?:\/en)?\/sensors\/([^/]+)\/index\.md\)\s*$/);
    if (m) names[m[2]] = m[1];
  }
  return names;
}
const PRODUCTS = { zh: productNames('_sidebar.md'), en: productNames('en/_sidebar.md') };

function ogImageFor(product) {
  const p = product && `/images/brand/og/${product}.jpg`;
  return p && fs.existsSync(path.join(ROOT, p)) ? p : DEFAULT_OG;
}

// ------------------------------------------------------------------ sidebar
// 產品頁與第 2、3 層頁面：在目前產品那一行底下展開這款的主機（目前主機再展開各軟體）
function sidebarTree(src, lang, g) {
  const gp = g && GI.products[g.key];
  if (!gp) return src;
  const pre = lang === 'en' ? '/en' : '';
  const kids = [];
  for (const ah of gp.hosts) {
    const h = GHOST[ah.id];
    const one = ah.sws.length === 1;
    kids.push(`    - [${h.name[lang]}](${one ? `${pre}/sensors/${g.key}/${ah.id}/${ah.sws[0]}.md` : `${pre}/sensors/${g.key}/${ah.id}/index.md`})`);
    if (!one && ah.id === g.host) for (const sw of ah.sws) kids.push(`      - [${L_(gSw(ah.id, sw).name, lang)}](${pre}/sensors/${g.key}/${ah.id}/${sw}.md)`);
  }
  const re = new RegExp(`^(\\s+- \\[[^\\]]+\\]\\(${pre}/sensors/${g.key}/index\\.md\\))[ \\t]*$`, 'm');
  return src.replace(re, (a) => `${a}\n${kids.join('\n')}`);
}
function renderSidebar(lang, currentUrl, g = null) {
  const file = lang === 'en' ? 'en/_sidebar.md' : '_sidebar.md';
  const renderer = new marked.Renderer();
  renderer.link = (href, title, text) => {
    const mapped = mapHref(href, file) || href;
    const active = mapped === currentUrl ? ' class="active"' : '';
    return `<a href="${mapped}"${active} title="${escAttr(stripTags(text))}">${text}</a>`;
  };
  let html = marked(sidebarTree(stripHidden(fs.readFileSync(path.join(ROOT, file), 'utf8')), lang, g), { renderer, gfm: true });
  html = html.replace(/<li>((?:(?!<li>)[\s\S])*?class="active")/, '<li class="active">$1');
  html = html.replace(/<img(?![^>]*\balt=)/g, '<img alt=""'); // decorative hub icons
  return html;
}

// ----------------------------------------------------------------- template
const HASH_REDIRECT = `(function(){function go(){var h=location.hash;if(h.indexOf('#/')!==0)return;var p=h.slice(2),f='',q=p.search(/[?#]/);if(q>-1){var m=p.slice(q).match(/id=([^&]+)/);f=m?'#'+m[1]:'';p=p.slice(0,q);}p=p.replace(/\\.md$/,'');var u;if(p===''||p==='README')u='/';else if(p==='en'||p==='en/'||p==='en/README')u='/en/';else if(p.slice(-1)==='/')u='/'+p;else if(p==='index'||/\\/index$/.test(p))u='/'+p.slice(0,-5);else u='/'+p+'.html';location.replace(u+f);}go();addEventListener('hashchange',go);})();`;

const LINE_URL = 'https://line.me/R/ti/p/@692vcvuk';
// Overseas buyers rarely use LINE: English pages ask for a quote by email instead
const QUOTE_MAIL = 'mailto:contactmbcrobot@gmail.com?subject=MBC-Pro%20Sensors%20quote%20request';
const quoteLink = (lang, cls, label) => lang === 'en'
  ? `<a class="${cls}" href="${QUOTE_MAIL}">✉️ ${label}</a>`
  : `<a class="${cls}" href="${LINE_URL}" target="_blank" rel="noopener">💬 ${label}</a>`;
const UI = {
  zh: { search: '搜尋文件', toc: '目錄', lang: 'EN', home: '首頁', line: 'LINE 詢價', guide: '選購指南', guideHref: '/guide.html', choose: '選擇版本：', guides: '各版本教學：',
    nav: [['產品', '/#products'], ['互動教學', '/learn/'], ['比賽對照', '/competitions.html'], ['教學文章', '/articles/'], ['選購指南', '/guide.html'], ['購買管道', '/where-to-buy.html'], ['聯絡我們', '/contact.html']], cta: { h: '想購買或詢價？', p: '個人購買、學校 / 社團大量採購、代理洽談都歡迎，工程師工作日 24 小時內回覆。', line: 'LINE 立即詢價', mail: 'Email 詢價', more: '更多聯絡方式', contact: '/contact.html' } },
  en: { search: 'Search docs', toc: 'Contents', lang: '中文', home: 'Home', line: 'Email for a quote', guide: 'Buying Guide', guideHref: '/en/guide.html', choose: 'Choose edition:', guides: 'Setup guides:',
    nav: [['Products', '/en/#products'], ['Tutorials', '/en/learn/'], ['Competitions', '/en/competitions.html'], ['Videos', '/en/#videos'], ['Buying Guide', '/en/guide.html'], ['Where to Buy', '/en/where-to-buy.html'], ['Contact', '/en/contact.html']], cta: { h: 'Interested in this sensor?', p: 'Ask us for pricing, bulk / school orders or distributor info. Our engineers reply within 24 hours on weekdays.', line: 'Chat on LINE', mail: 'Email us', more: 'All contact options', contact: '/en/contact.html' } },
};

// Site-wide footer: company name + trademark disclaimer (LEGO Fair Play guidelines)
const FOOTER = {
  zh: `LEGO®、SPIKE™ 與 MINDSTORMS® 是 LEGO Group 的商標，LEGO Group 並未贊助、授權或認可本網站及本公司產品。MBC-Pro Sensors 為少伯創意機器人有限公司（MBC Robot）之獨立產品，其他商標為各自所有人所有。`,
  en: `LEGO®, SPIKE™ and MINDSTORMS® are trademarks of the LEGO Group, which does not sponsor, authorize or endorse this site or our products. MBC-Pro Sensors is an independent product line of Shaobo Creative Robotics Company Limited (MBC Robot). Other trademarks belong to their respective owners.`,
};
const SOCIAL = [
  ['YouTube', 'https://www.youtube.com/@mbc_robot'],
  ['Instagram', 'https://www.instagram.com/mbc.pro.sensors/'],
  ['Threads', 'https://www.threads.com/@mbc.pro.sensors'],
  ['Facebook', 'https://www.facebook.com/mbcprosensors/'],
  ['LINE', LINE_URL],
];
const socialHtml = `<p class="sf-social">${SOCIAL.map(([n, u]) => `<a href="${u}" target="_blank" rel="noopener me">${n}</a>`).join(' · ')}</p>`;
const footerHtml = (lang) => `<footer class="site-footer">${socialHtml}<p>${FOOTER[lang]}</p><p>© ${new Date().getFullYear()} ${lang === 'en' ? 'Shaobo Creative Robotics Company Limited (MBC Robot)' : '少伯創意機器人有限公司（MBC Robot）'}</p></footer>`;

function ctaHtml(lang) {
  const t = UI[lang].cta;
  return `<div class="buy-cta"><h3>${t.h}</h3><p>${t.p}</p><div class="buy-cta-btns">` +
    `<a class="buy-btn line" href="https://line.me/R/ti/p/@692vcvuk" target="_blank" rel="noopener">💬 ${t.line}</a>` +
    `<a class="buy-btn mail" href="mailto:contactmbcrobot@gmail.com">✉️ ${t.mail}</a>` +
    `<a class="buy-btn more" href="${t.contact}">${t.more} →</a></div></div>`;
}

// Spec summary under the price: editions, power, optional size/weight/box (shown only when filled in products.json), warranty
const SPEC_LABEL = {
  zh: { editions: '支援版本', power: '供電', size: '尺寸', weight: '重量', box: '包裝內容', warranty: '保固', warrantyValue: '6 個月' },
  en: { editions: 'Editions', power: 'Power', size: 'Size', weight: 'Weight', box: 'In the box', warranty: 'Warranty', warrantyValue: '6 months' },
};
function productSpecs(p, lang) {
  const L = SPEC_LABEL[lang];
  const s = (p.specs || {})[lang] || {};
  const rows = [[L.editions, p[lang].editions.map(([label]) => label).join(lang === 'en' ? ', ' : '、')]];
  if (s.power) rows.push([L.power, s.power]);
  for (const r of s.extra || []) rows.push(r);
  for (const k of ['size', 'weight', 'box']) if (s[k]) rows.push([L[k], s[k]]);
  rows.push([L.warranty, L.warrantyValue]);
  return `<table class="ph-specs"><tbody>${rows.map(([k, v]) => `<tr><th>${k}</th><td>${v}</td></tr>`).join('')}</tbody></table>`;
}

// Quote link with the product (and edition) already filled in: LINE opens a chat with the message typed, email gets subject + body
const LINE_ID = '%40692vcvuk';
function prefilledQuote(lang, name, edition) {
  if (lang === 'en') {
    const what = edition ? `${name} (${edition} edition)` : name;
    return `mailto:contactmbcrobot@gmail.com?subject=${encodeURIComponent('Quote request: ' + what)}&body=${encodeURIComponent(`Hello,\n\nI'd like a quote for ${what}.\nQuantity:\nCountry:\n`)}`;
  }
  return `https://line.me/R/oaMessage/${LINE_ID}/?${encodeURIComponent(`您好，我想詢價：${name}${edition ? `（${edition}）` : ''}，數量：`)}`;
}
// EXP6 "editions" are software environments, not hardware versions: no edition picker
const NO_EDITION_PICK = new Set(['exp6']);

// Sales block at the top of each product page, generated from _build/products.json
const PRODUCT_DATA = JSON.parse(fs.readFileSync(path.join(BUILD, 'products.json'), 'utf8'));
function productHero(key, lang, file, name) {
  const p = PRODUCT_DATA[key];
  if (!p) return '';
  const t = p[lang];
  const u = UI[lang];
  const media = [];
  if (p.image) media.push(`<div class="ph-img"><img src="${p.image}" alt="${escAttr(name)}" style="filter: drop-shadow(0 0 28px ${p.color}aa);"></div>`);
  if (p.video) media.push(`<div class="ph-video"><iframe src="https://www.youtube-nocookie.com/embed/${p.video}" title="${escAttr(name)}" loading="lazy" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" allowfullscreen></iframe></div>`);
  const editions = t.editions.map(([label, href]) => `<a href="${mapHref(href, file) || href}">${label}</a>`).join('');
  return `<div class="product-hero" style="--pc: ${p.color};">` +
    `<span class="ph-badge">${t.badge}</span>` +
    (media.length ? `<div class="ph-media${media.length === 1 ? ' single' : ''}">${media.join('')}</div>` : '') +
    `<p class="ph-pitch">${t.pitch}</p>` +
    // price + quote button right under the pitch (on phones CSS also moves the video below them)
    (p.msrp ? `<p class="ph-price">${lang === 'en' ? (p.msrp.usd ? `MSRP <b>US$${p.msrp.usd}</b> <small>(shipping not included)</small>` : `<b>Available in Taiwan only</b> <small>(not sold outside Taiwan)</small>`) : `建議售價 <b>NT$${p.msrp.twd.toLocaleString('en-US')}</b>`}</p>` : '') +
    // edition picker: choosing one rewrites the quote link (site.js) so the message names the edition
    (NO_EDITION_PICK.has(key) ? '' : `<div class="ph-pick" role="radiogroup" aria-label="${u.choose}"><span>${u.choose}</span>${t.editions.map(([label]) =>
      `<button type="button" role="radio" aria-checked="false" data-href="${escAttr(prefilledQuote(lang, name, label))}">${label}</button>`).join('')}</div>`) +
    `<div class="ph-cta"><a class="buy-btn line" href="${escAttr(prefilledQuote(lang, name))}"${lang === 'en' ? '' : ' target="_blank" rel="noopener"'}>${lang === 'en' ? '✉️' : '💬'} ${u.line}</a><a class="buy-btn more" href="${u.guideHref}">🧭 ${u.guide}</a></div>` +
    `<div class="ph-stats">${t.stats.map(([v, l]) => `<div class="ph-stat"><b>${v}</b><span>${l}</span></div>`).join('')}</div>` +
    `<div class="ph-editions"><span>${u.guides}</span>${editions}</div>` +
    productSpecs(p, lang) +
    (p.wro ? `<p class="ph-wro">🏆 ${p.wro[lang]}<br><a href="${lang === 'en' ? '/en/wro-rules.html' : '/wro-rules.html'}">${lang === 'en' ? 'WRO rules check' : 'WRO 規則對照'}</a> · <a href="${lang === 'en' ? '/en/competitions.html' : '/competitions.html'}">${lang === 'en' ? 'Other competitions' : '其他比賽'}</a> · <a href="${lang === 'en' ? '/en/where-to-buy.html' : '/where-to-buy.html'}">${lang === 'en' ? 'Where to buy' : '購買管道'}</a></p>` : '') +
    `</div>`;
}

// ------------------------------------------------- 三層導覽：產品 → 主機 → 軟體（2026-10-22；設計見 _build/guides/DESIGN.md）
// 資料：_build/generated/guides-index.json（node _build/gen-guides.mjs 產生）
const GI = JSON.parse(fs.readFileSync(path.join(BUILD, 'generated', 'guides-index.json'), 'utf8'));
const GHOST = Object.fromEntries(GI.hosts.map((h) => [h.id, h]));
const gpre = (lang) => (lang === 'en' ? '/en' : '');
const gProductUrl = (lang, k) => `${gpre(lang)}/sensors/${k}/`;
const gHostUrl = (lang, k, h) => `${gpre(lang)}/sensors/${k}/${h}/`;
const gSwUrl = (lang, k, h, s) => `${gpre(lang)}/sensors/${k}/${h}/${s}.html`;
const gSw = (h, s) => GHOST[h].software.find((x) => x.id === s);
const gHostOf = (k, h) => ((GI.products[k] && GI.products[k].hosts) || []).find((x) => x.id === h);
// 進某個主機的網址：想保留的軟體這裡也有 → 那個軟體頁；只有一種軟體 → 軟體頁；其他 → 平台頁
function gEnter(lang, k, h, keepSw) {
  const ah = gHostOf(k, h);
  if (!ah) return null;
  if (keepSw && ah.sws.includes(keepSw)) return gSwUrl(lang, k, h, keepSw);
  if (ah.sws.length === 1) return gSwUrl(lang, k, h, ah.sws[0]);
  return gHostUrl(lang, k, h);
}
const GL = {
  zh: { product: '產品', host: '主機', sw: '軟體', pickSw: '選軟體', overview: '平台總覽：版本與接線', switcher: '切換產品、主機、軟體', none: (h) => `沒有 ${h} 版`,
    learn: '不用接線、不用主機，先在網頁上玩互動教學。', pickHost: '① 選你的主機', pickHostSub: '只列出這款產品能用的主機；選好主機再選軟體，之後整頁都只講你的軟體。', pickSwTitle: '選你用的軟體', cont: '繼續用', step1: '選產品', step2: '選主機', step3: '選軟體', go: '前往', result: '選好產品、主機和軟體，這裡就會出現入口', table: '所有產品 × 主機對照表', noHost: '這款沒有這個主機', moved: '這頁內容已整合到新的說明頁：', title: '找到你的程式' },
  en: { product: 'Product', host: 'Host', sw: 'Software', pickSw: 'Pick software', overview: 'Host overview: edition and wiring', switcher: 'Switch product, host or software', none: (h) => `no ${h} edition`,
    learn: 'No wiring, no hub: try the interactive tutorial in your browser first.', pickHost: '① Pick your host', pickHostSub: 'Only hosts this product supports are listed; pick one, then your software, and the page covers just that software.', pickSwTitle: 'Pick your software', cont: 'Continue with', step1: 'Pick a product', step2: 'Pick a host', step3: 'Pick software', go: 'Go to', result: 'Pick a product, a host and software to get the link', table: 'All products × hosts', noHost: 'Not available for this product', moved: 'This page has moved into the new guide: ', title: 'Find your program' },
};
const fileOk = (p) => p && p.startsWith('/') && fs.existsSync(path.join(ROOT, decodeURI(p)));
// 主機圖：images/illust/host-*.svg（插畫）優先，沒有就用 hubs 照片或 Lucide 圖示
function gHostImg(h) {
  const webp = h.illust && h.illust.replace(/.svg$/, ".webp");
  if (fileOk(webp)) return `<img src="${webp}" alt="" loading="lazy">`;
  if (fileOk(h.illust)) return `<img src="${h.illust}" alt="" loading="lazy">`;
  if (fileOk(h.icon)) return `<img src="${h.icon}" alt="" loading="lazy">`;
  return ICONS[h.icon] ? icon(h.icon, 'i gi') : '';
}
function gSwImg(s) {
  if (fileOk(s.icon)) return `<img src="${s.icon}" alt="" loading="lazy">`;
  return ICONS[s.icon] ? icon(s.icon, 'i gi') : '';
}
const swNames = (lang, h, sws) => sws.map((s) => L_(gSw(h, s).name, lang)).join(lang === 'en' ? ' · ' : '・');
function L_(o, lang) { return (o && (o[lang] || o.zh)) || ''; }

// 互動教學（learn/<教學>/<種類>.html，英文在 en/learn/；由互動教學技能的 wrap_learn.js 產生）
// 網站產品 → 教學（神攝手分學校版、WRO 版；PS2／PS4 共用一套）；主機 → 軟體 → 教學種類。只列實際有檔的頁。
const LEARN = { line8: ['line8'], line16: ['line16'], tof2: ['laser2'], tof8: ['laser8'], ps2: ['gamepad'], ps4: ['gamepad'], esp32cam: ['cam_school', 'hue12'], exp6: ['exp6'], servo6: ['servo6'], imu: ['imu'] };
const LEARN_KINDS = {
  spike: { pybricks: ['micropython', 'pybricks-blocks'], mbcblocks: ['mbc-blocks'], official: ['spike-app'] },
  ev3: { pybricks: ['ev3-pybricks'], clev3r: ['clev3r'], classroom: ['ev3-classroom'], ev3g: ['ev3g'] },
  arduino: { ide: ['arduino'] },
  mblock: { mblock5: ['mblock'] },
  matrix: { matrixblock: ['matrix'], mvision: ['matrix'] },
  microbit: { makecode: ['makecode'] },
};
const LEARN_HOST_NAME = { spike: 'SPIKE', ev3: 'EV3', arduino: 'Arduino', mblock: 'mBlock', matrix: 'MATRIX', microbit: 'micro:bit' };
const LK = {
  zh: { title: '互動教學', sub: '不用接主機，先在網頁上點點看：左邊選功能，右邊的程式或積木跟著長出來，還能拖拉模擬感應器看數值。',
    swSub: '第一次用？先跟著互動教學在網頁上做一遍，再照下面的步驟接主機。', all: '全部產品的互動教學', guide: '產品說明',
    kinds: { micropython: 'MicroPython（Pybricks）', 'pybricks-blocks': 'Pybricks 積木', 'mbc-blocks': 'MBC 積木網站', 'spike-app': '樂高官方 SPIKE App',
      'ev3-pybricks': 'Pybricks／EV3 MicroPython', clev3r: 'clev3r', 'ev3-classroom': 'EV3 Classroom', ev3g: 'EV3-G 官方軟體',
      arduino: 'Arduino IDE', mblock: 'mBlock 5', matrix: 'MATRIXblock', makecode: 'MakeCode' },
    eds: { cam_school: '學校版', hue12: 'WRO 版' }, gamepad: '掌控者 PS2／PS4' },
  en: { title: 'Interactive tutorial', sub: 'No hub needed: click through it in your browser. Pick a function on the left, watch the code or blocks build up on the right, and drag the simulated sensor to see the values.',
    swSub: 'First time? Try the interactive tutorial in your browser first, then follow the steps below with your hub.', all: 'Interactive tutorials for all products', guide: 'Product guide',
    kinds: { micropython: 'MicroPython (Pybricks)', 'pybricks-blocks': 'Pybricks blocks', 'mbc-blocks': 'MBC Blocks website', 'spike-app': 'LEGO SPIKE App',
      'ev3-pybricks': 'Pybricks / EV3 MicroPython', clev3r: 'clev3r', 'ev3-classroom': 'EV3 Classroom', ev3g: 'EV3-G (LEGO software)',
      arduino: 'Arduino IDE', mblock: 'mBlock 5', matrix: 'MATRIXblock', makecode: 'MakeCode' },
    eds: { cam_school: 'School edition', hue12: 'WRO edition' }, gamepad: 'Controller PS2 / PS4' },
};
const learnHub = (lang) => `${gpre(lang)}/learn/`;
// [{ t: 教學, host, items: [{ kind, url }] }]；host 不給 = 全部主機，sws 不給 = 該主機全部軟體
function learnOf(lang, key, host, sws) {
  const out = [];
  for (const h of host ? [host] : Object.keys(LEARN_KINDS)) {
    const kinds = [...new Set((sws || Object.keys(LEARN_KINDS[h])).flatMap((s) => LEARN_KINDS[h][s] || []))];
    for (const t of LEARN[key] || []) {
      const items = kinds.map((kind) => ({ kind, url: `${learnHub(lang)}${t}/${kind}.html` })).filter((x) => fileOk(x.url));
      if (items.length) out.push({ t, host: h, items });
    }
  }
  return out;
}
function learnLinks(lang, groups, showHost) {
  const K = LK[lang];
  return groups.map((g) => {
    const label = [showHost ? LEARN_HOST_NAME[g.host] : '', K.eds[g.t] || ''].filter(Boolean).join(' ');
    return `<p class="lb-links">${label ? `<span class="lb-ed">${label}</span>` : ''}${g.items.map((x) => `<a href="${x.url}">${K.kinds[x.kind]}</a>`).join('')}</p>`;
  }).join('');
}
// 產品頁（主機格子下面，列全部主機）、平台頁（軟體格子下面，只列這個主機）、軟體頁（最上面，只列這個軟體）共用的入口框
function learnBox(lang, key, host, sws, mode) {
  const groups = learnOf(lang, key, host, sws);
  if (!groups.length) return '';
  const K = LK[lang];
  return `<div class="learn-box${mode === 'sw' ? ' lb-sw' : ''}"><p class="lb-title">${icon('puzzle')}<b>${K.title}</b>${host ? `<span class="lb-tag">${LEARN_HOST_NAME[host]}</span>` : ''}</p>` +
    `<p class="lb-sub">${mode === 'sw' ? K.swSub : K.sub}</p>${learnLinks(lang, groups, !host)}` +
    (mode === 'sw' ? '' : `<p class="lb-all"><a href="${learnHub(lang)}">${K.all} →</a></p>`) + `</div>`;
}
// learn/index.md 的 <!-- learn-hub -->：每款產品一張卡（PS2／PS4 合一張），卡內每個主機一排
function learnHubHtml(lang) {
  const K = LK[lang], seen = new Set(), cards = [];
  for (const [k, p] of Object.entries(GI.products)) {
    for (const t of LEARN[k] || []) {
      if (seen.has(t)) continue;
      seen.add(t);
      const groups = learnOf(lang, k, null, null).filter((g) => g.t === t);
      if (!groups.length) continue;
      const name = t === 'gamepad' ? K.gamepad : escAttr(L_(p.short, lang)) + (K.eds[t] ? ` ${K.eds[t]}` : '');
      cards.push(`<div class="learn-card" id="${t}" style="--pc:${p.color}">${p.image ? `<img src="${p.image}" alt="" loading="lazy">` : '<span class="ft-noimg"></span>'}` +
        `<div><h3>${name}</h3>${groups.map((g) => `<p class="lc-row"><span class="lc-host">${LEARN_HOST_NAME[g.host]}</span>${g.items.map((x) => `<a href="${x.url}">${K.kinds[x.kind]}</a>`).join('')}</p>`).join('')}` +
        `<a class="lc-guide" href="${gProductUrl(lang, k)}">${K.guide} →</a></div></div>`);
    }
  }
  return `<div class="learn-hub">${cards.join('')}</div>`;
}

// 第 2、3 層頁面頂端的切換列：產品 ▾ › 主機 ▾ › 軟體 ▾（<details> 下拉：不用 JS 也能用、鍵盤可操作；連結在建站時算好）
function switcherHtml(lang, g) {
  const T_ = GL[lang];
  const cur = GI.products[g.key];
  const curHost = gHostOf(g.key, g.host);
  const seg = (cls, k, v, items) => `<details class="gsw-seg ${cls}"><summary><span class="gsw-k">${k}</span><span class="gsw-v">${v}</span></summary><ul class="gsw-menu">${items.map(([href, text, on, sub]) =>
    `<li><a href="${href}"${on ? ' aria-current="page"' : ''}>${text}${sub ? `<small>${sub}</small>` : ''}</a></li>`).join('')}</ul></details>`;
  const prodItems = Object.entries(GI.products).map(([k, p]) => {
    const has = gHostOf(k, g.host);
    const href = !has ? gProductUrl(lang, k) : g.sw ? gEnter(lang, k, g.host, g.sw) : gHostUrl(lang, k, g.host);
    return [href, escAttr(L_(p.short, lang)), k === g.key, has ? '' : T_.none(GHOST[g.host].name[lang])];
  });
  const hostItems = cur.hosts.map((ah) => [gEnter(lang, g.key, ah.id, g.sw), GHOST[ah.id].name[lang], ah.id === g.host, '']);
  const swItems = curHost.sws.map((s) => [gSwUrl(lang, g.key, g.host, s), L_(gSw(g.host, s).name, lang), s === g.sw, ''])
    .concat([[gHostUrl(lang, g.key, g.host), T_.overview, !g.sw, '']]);
  return `<nav class="gsw gsw-${g.host}" aria-label="${T_.switcher}" data-p="${g.key}" data-h="${g.host}"${g.sw ? ` data-s="${g.sw}"` : ''}><div class="gsw-in">` +
    seg('gsw-p', T_.product, escAttr(L_(cur.short, lang)), prodItems) + '<span class="gsw-sep" aria-hidden="true">›</span>' +
    seg(`gsw-h pg-${g.host}`, T_.host, GHOST[g.host].name[lang], hostItems) + '<span class="gsw-sep" aria-hidden="true">›</span>' +
    seg('gsw-s', T_.sw, g.sw ? L_(gSw(g.host, g.sw).name, lang) : T_.pickSw, swItems) +
    `</div></nav>`;
}

// 平台頁 <!-- sw-grid -->：② 選你用的軟體（大格子）
function swGridHtml(lang, key, host) {
  const ah = gHostOf(key, host);
  if (!ah) return '';
  return `<div class="pick-grid pick-sw pg-${host}">${ah.sws.map((s) => {
    const d = gSw(host, s);
    return `<a class="pick-tile" href="${gSwUrl(lang, key, host, s)}" data-sw="${s}"><span class="pt-img">${gSwImg(d)}</span><span class="pt-name">${L_(d.name, lang)}</span><span class="pt-tag">${L_(d.tagline, lang)}</span></a>`;
  }).join('')}</div>`;
}

// 產品頁 <!-- platform-grid -->：① 選你的主機（只列可用主機；只有一種軟體的主機直接連軟體頁）＋「繼續用 X › Y →」（site.js 讀 localStorage）
function platformGrid(key, lang) {
  const gp = GI.products[key];
  if (!gp) return '';
  const T_ = GL[lang];
  const data = {};
  const tiles = gp.hosts.map((ah) => {
    const h = GHOST[ah.id];
    data[ah.id] = { name: h.name[lang], url: gEnter(lang, key, ah.id), sws: Object.fromEntries(ah.sws.map((s) => [s, { name: L_(gSw(ah.id, s).name, lang), url: gSwUrl(lang, key, ah.id, s) }])) };
    return `<a class="pick-tile pg-${ah.id}" href="${gEnter(lang, key, ah.id)}" data-host="${ah.id}"><span class="pt-img">${gHostImg(h)}</span><span class="pt-name">${h.name[lang]}</span><span class="pt-tag">${L_(h.tagline, lang)}</span><span class="pt-sw">${swNames(lang, ah.id, ah.sws)}</span></a>`;
  });
  // 產品頁頂圖：images/illust/ref-<產品>.webp（G3 產的 16:9 插畫；沒有就用 svg，都沒有就不放）
  const ref = [`/images/illust/ref-${key}.webp`, `/images/illust/ref-${key}.svg`].find(fileOk);
  return `<div class="pick" id="pick-host">${ref ? `<img class="pick-banner" src="${ref}" alt="" loading="lazy">` : ""}<h2 class="pick-title">${T_.pickHost}</h2><p class="pick-sub">${T_.pickHostSub}</p>` +
    `<p class="pick-continue" hidden></p>` +
    `<div class="pick-grid pick-hosts">${tiles.join('')}</div>` + learnBox(lang, key, null, null, 'product') +
    `<script type="application/json" class="pick-data">${JSON.stringify({ cont: T_.cont, hosts: data }).replace(/</g, '\\u003c')}</script></div>`;
}

// 首頁 <!-- finder -->：三步（產品 → 主機 → 軟體）直接到軟體頁；沒有 JS 時是對照表
function finderHtml(lang) {
  const T_ = GL[lang];
  const prods = Object.entries(GI.products);
  const data = { labels: { go: T_.go, result: T_.result, noHost: T_.noHost }, hosts: {}, products: {} };
  for (const h of GI.hosts) data.hosts[h.id] = { name: h.name[lang], sws: Object.fromEntries(h.software.map((s) => [s.id, { name: L_(s.name, lang), tag: L_(s.tagline, lang) }])) };
  for (const [k, p] of prods) data.products[k] = { name: L_(p.short, lang), hosts: Object.fromEntries(p.hosts.map((ah) => [ah.id, { url: gEnter(lang, k, ah.id), sws: Object.fromEntries(ah.sws.map((s) => [s, gSwUrl(lang, k, ah.id, s)])) }])) };
  const prodTiles = prods.map(([k, p]) => `<button type="button" role="radio" aria-checked="false" data-key="${k}" style="--pc:${p.color}">${p.image ? `<img src="${p.image}" alt="" loading="lazy">` : '<span class="ft-noimg"></span>'}<span>${escAttr(L_(p.short, lang))}</span></button>`).join('');
  const hostTiles = GI.hosts.map((h) => `<button type="button" role="radio" aria-checked="false" data-host="${h.id}" class="ft-${h.id}" disabled><span>${h.name[lang]}</span></button>`).join('');
  const rows = prods.map(([k, p]) => `<tr><td><a href="${gProductUrl(lang, k)}">${escAttr(L_(p.short, lang))}</a></td>${GI.hosts.map((h) => {
    const ah = gHostOf(k, h.id);
    return `<td>${ah ? ah.sws.map((s) => `<a href="${gSwUrl(lang, k, h.id, s)}">${L_(gSw(h.id, s).name, lang)}</a>`).join('<br>') : '—'}</td>`;
  }).join('')}</tr>`).join('');
  return `<div class="finder" id="finder-box">` +
    `<div class="finder-pick">` +
    `<p class="finder-step"><b>1</b>${T_.step1}</p><div class="finder-tiles finder-prods" role="radiogroup" aria-label="${T_.step1}">${prodTiles}</div>` +
    `<p class="finder-step"><b>2</b>${T_.step2}</p><div class="finder-tiles finder-hosts" role="radiogroup" aria-label="${T_.step2}">${hostTiles}</div>` +
    `<p class="finder-step"><b>3</b>${T_.step3}</p><div class="finder-tiles finder-sws" role="radiogroup" aria-label="${T_.step3}"></div>` +
    `<div class="finder-result" aria-live="polite"><p>${T_.result}</p></div>` +
    `</div>` +
    `<details class="finder-table" open><summary>${T_.table}</summary><table><thead><tr><th>${T_.product}</th>${GI.hosts.map((h) => `<th>${h.name[lang]}</th>`).join('')}</tr></thead><tbody>${rows}</tbody></table></details>` +
    `<p class="finder-learn">${icon('puzzle')}<span>${T_.learn}</span><a href="${learnHub(lang)}">${LK[lang].all} →</a></p>` +
    `<script type="application/json" id="finder-data">${JSON.stringify(data).replace(/</g, '\\u003c')}</script>` +
    `</div>`;
}

// 舊頁（<!-- moved: --> 或 guides-index 的 moved 對照）頁首提示條的文字
function movedLabel(target) {
  const m = target.match(/^\/(en\/)?sensors\/([^/]+)\/(?:([^/]+)\/)?([^/]+)\.md$/);
  if (!m || !GI.products[m[2]]) return target;
  const lang = m[1] ? 'en' : 'zh';
  const parts = [L_(GI.products[m[2]].short, lang)];
  if (m[3]) { parts.push(GHOST[m[3]].name[lang]); if (m[4] !== 'index') parts.push(L_(gSw(m[3], m[4]).name, lang)); }
  return parts.join(' › ');
}

// ------------------------------------------------- 「安裝與設定」include 與「範例」抽取（gen-guides.mjs 產的頁面裡的指令，建站時展開）
const readSrc = (rel) => stripHidden(fs.readFileSync(path.join(ROOT, rel), 'utf8').replace(/^﻿/, ''));
// 標題整體往下移，最高一層變成 minLevel（include 進來的內容放在 ## 段落底下）
function demote(lines, fence, minLevel) {
  let top = 7;
  lines.forEach((l, i) => { const m = !fence[i] && l.match(/^(#{1,6})\s/); if (m) top = Math.min(top, m[1].length); });
  const shift = top > 6 ? 0 : Math.max(0, minLevel - top);
  return lines.map((l, i) => (!fence[i] && /^#{1,6}\s/.test(l) ? '#'.repeat(Math.min(6, l.match(/^#+/)[0].length + shift)) + l.replace(/^#+/, '') : l));
}
function fenceMap(lines) {
  let inF = false;
  return lines.map((l) => { const f = /^\s*(```|~~~)/.test(l); const r = inF || f; if (f) inF = !inF; return r; });
}
// 相對 .md 連結改成以來源檔為準的絕對路徑
const absLinks = (text, srcRel) => text
  .replace(/\]\((?!https?:|\/|#|mailto:)([^)\s]+\.md(?:[#?][^)\s]*)?)\)/g, (a, h) => `](/${path.posix.join(path.posix.dirname(srcRel), h)})`)
  .replace(/href="(?!https?:|\/|#|mailto:)([^"]+\.md(?:[#?][^"]*)?)"/g, (a, h) => `href="/${path.posix.join(path.posix.dirname(srcRel), h)}"`);
const stripMoved = (t) => t.replace(/^<!-- moved: .* -->\r?\n(?:<p class="moved-note">.*<\/p>\r?\n)?/m, '');

function expandIncludes(src) {
  let resolved = 0;
  const fillVars = (text, v) => text.replace(/\{\{(\w+)\}\}/g, (a, k) => (v[k] != null && typeof v[k] !== 'boolean' ? v[k] : a));
  const prep = (rel, v) => {
    const lines = fillVars(readSrc(rel), v).split(/\r?\n/);
    return absLinks(demote(lines, fenceMap(lines), 3).join('\n'), rel);
  };
  // <!-- setup: 通用.md + 產品.md {vars} -->：產品專屬內文插在通用內文「### 接線」之後、「### 第一次測試」之前；skipWiring 拿掉通用的「接線」節
  src = src.replace(/^<!-- setup: (\S+) \+ (\S+)(?: (\{.*\}))? -->[ \t]*$/gm, (all, swRel, pRel, vars) => {
    const v = vars ? JSON.parse(vars) : {};
    const hasSw = fs.existsSync(path.join(ROOT, swRel));
    const hasP = fs.existsSync(path.join(ROOT, pRel));
    if (!hasSw && !hasP) return '';
    resolved++;
    const prod = hasP ? prep(pRel, v) : '';
    if (!hasSw) return prod + '\n';
    let lines = prep(swRel, v).split('\n');
    const isH = (l, re) => /^###\s/.test(l) && re.test(l);
    if (v.skipWiring) {
      const a = lines.findIndex((l) => isH(l, /接線|Wiring/i));
      if (a > -1) { let b = a + 1; while (b < lines.length && !/^###\s/.test(lines[b])) b++; lines.splice(a, b - a); }
    }
    if (prod) {
      let at = lines.findIndex((l) => isH(l, /第一次測試|First test/i));
      if (at < 0) at = lines.findIndex((l) => isH(l, /常見問題|FAQ/i));
      if (at < 0) at = lines.length;
      lines.splice(at, 0, prod, '');
    }
    return lines.join('\n') + '\n';
  });
  src = src.replace(/^<!-- include: (\S+)(?: (\{.*\}))? -->[ \t]*$/gm, (all, rel, vars) => {
    if (!fs.existsSync(path.join(ROOT, rel))) return '';
    resolved++;
    return prep(rel, vars ? JSON.parse(vars) : {}) + '\n';
  });
  return src.replace(/<!-- include-fallback -->\r?\n([\s\S]*?)<!-- \/include-fallback -->/g, (all, body) => (resolved ? '' : body));
}

// 範例頁 → 只留這個軟體：每個「## 範例 N」段保留共同段落，<details> 只留 summary 符合本軟體的（加 open），下載表／下載連結只留本軟體那欄
const EX_LABEL = { zh: { only: '下面這些範例沒有這個軟體的版本：', }, en: { only: 'These examples have no version for this software: ' } };
function extractExamples(srcRel, host, sw, whole, lang) {
  const text = stripMoved(readSrc(srcRel));
  let lines = text.split(/\r?\n/);
  if (whole) {
    const h1 = lines.findIndex((l) => /^# /.test(l));
    lines = lines.slice(h1 + 1).filter((l) => !/^\[🔙/.test(l));
    return absLinks(demote(lines, fenceMap(lines), 3).join('\n'), srcRel);
  }
  const H = GHOST[host];
  const reOf = (s, k) => (s[k] && s[k][lang] ? new RegExp(s[k][lang], 'i') : null);
  const mine = H.software.find((s) => s.id === sw);
  const sumRe = reOf(mine, 'summaryMatch');
  const colRes = H.software.map((s) => [s.id, reOf(s, 'colMatch')]).filter(([, r]) => r);
  const fence = fenceMap(lines);
  // 切成 ## 段
  const secs = [];
  lines.forEach((l, i) => { if (!fence[i] && /^## /.test(l)) secs.push({ start: i, lines: [] }); if (secs.length) secs[secs.length - 1].lines.push(i); });
  const out = [];
  const skipped = [];
  for (const s of secs) {
    const head = lines[s.start];
    if (!/^##\s.*?(範例|Example)\s*\d+/i.test(head)) continue;
    const idx = s.lines;
    const keep = [];
    let had = 0, kept = 0;
    for (let j = 0; j < idx.length; j++) {
      const i = idx[j];
      const l = lines[i];
      if (!fence[i] && /^<details\b/.test(l)) {
        // 找對應的 </details>（巢狀計數，程式框內不算）
        let depth = 0, k = j;
        for (; k < idx.length; k++) {
          const t = lines[idx[k]];
          if (fence[idx[k]] && k !== j) continue;
          depth += (t.match(/<details\b/g) || []).length - (t.match(/<\/details>/g) || []).length;
          if (depth <= 0) break;
        }
        const block = idx.slice(j, k + 1).map((x) => lines[x]);
        const sum = (block.join('\n').match(/<summary>([\s\S]*?)<\/summary>/) || [])[1] || '';
        had++;
        if (sumRe && sumRe.test(sum)) { kept++; block[0] = block[0].replace(/^<details(?![^>]*\bopen\b)/, '<details open'); keep.push(...block); }
        j = k;
        continue;
      }
      keep.push(fence[i] ? l : l);
    }
    if (had && !kept) { skipped.push(head.replace(/^##\s+/, '')); continue; }
    out.push(...filterDownloads(keep, colRes, sw));
  }
  const ofence = fenceMap(out);
  let res = demote(out, ofence, 3).join('\n');
  if (skipped.length) res += `\n\n> [!NOTE]\n> ${EX_LABEL[lang].only}${skipped.join(lang === 'en' ? '; ' : '；')}\n`;
  return absLinks(res, srcRel);
}
// 下載表（表頭有 2 個以上軟體欄）只留本軟體那欄；「[⬇ A] ｜ [⬇ B]」連結列只留本軟體
function filterDownloads(lines, colRes, sw) {
  const fence = fenceMap(lines);
  const which = (t) => colRes.filter(([, r]) => r.test(t)).map(([id]) => id);
  const cells = (l) => l.trim().replace(/^\|/, '').replace(/\|$/, '').split(/(?<!\\)\|/);
  const out = [];
  for (let i = 0; i < lines.length; i++) {
    const l = lines[i];
    if (!fence[i] && /^\|.*\|\s*$/.test(l) && i + 1 < lines.length && /^\|\s*:?-{3,}/.test(lines[i + 1])) {
      const hc = cells(l);
      const tags = hc.map(which);
      if (tags.filter((t) => t.length).length >= 2 || (tags.some((t) => t.length) && !tags.some((t) => t.includes(sw)))) {
        const keepCol = tags.map((t) => !t.length || t.includes(sw));
        let j = i;
        const rows = [];
        while (j < lines.length && /^\|.*\|\s*$/.test(lines[j])) { rows.push(lines[j]); j++; }
        if (keepCol.some((k, c) => k && tags[c].length)) for (const r of rows) out.push('| ' + cells(r).filter((c, n) => keepCol[n]).map((c) => c.trim()).join(' | ') + ' |');
        i = j - 1;
        continue;
      }
    }
    if (!fence[i] && /^\[[^\]]+\]\([^)]+\)(\s*｜\s*\[[^\]]+\]\([^)]+\))+\s*$/.test(l)) {
      const segs = l.split(/\s*｜\s*/);
      const tags = segs.map((s) => which((s.match(/^\[([^\]]+)\]/) || [])[1] || ''));
      if (tags.filter((t) => t.length).length >= 2) {
        const kept = segs.filter((s, n) => !tags[n].length || tags[n].includes(sw));
        if (kept.length) out.push(kept.join(' ｜ '));
        continue;
      }
    }
    out.push(l);
  }
  return out;
}
function expandExamples(src, lang) {
  return src.replace(/^<!-- examples: (\S+) host=(\S+) sw=(\S+)( whole)? -->[ \t]*$/gm, (all, rel, host, sw, whole) =>
    (fs.existsSync(path.join(ROOT, rel)) ? extractExamples(rel, host, sw, !!whole, lang) : ''));
}

// ------------------------------------------------- 右側目錄（h2／h3；≥1200px 固定在右，小螢幕變 <details>）
// 目錄放進文章：有切換列（產品 › 主機 › 軟體）就接在它後面，沒有就放文章最前面
function withToc(body, toc) {
  if (!toc) return body;
  const m = body.match(/<nav class="gsw[\s\S]*?<\/nav>/);
  if (!m) return toc + body;
  const i = m.index + m[0].length;
  return body.slice(0, i) + toc + body.slice(i);
}

function tocHtml(html, lang) {
  const items = [];
  for (const m of html.matchAll(/<h([23]) id="([^"]+)">(?:<a [^>]*><span>)?([\s\S]*?)(?:<\/span><\/a>)?<\/h\1>/g)) {
    const text = stripEmoji(stripTags(m[3])).replace(/\s+/g, ' ').trim();
    if (text) items.push({ level: +m[1], id: m[2], text });
  }
  if (items.length < 3) return '';
  const li = items.map((it) => `<li class="toc-h${it.level}"><a href="#${encodeURI(it.id)}">${escAttr(it.text)}</a></li>`).join('');
  // 目錄 HTML 直接放在文章開頭（有切換列就放在切換列下方），預設收起＝手機版的樣子；桌機（≥1200px）由 <details> 裡的小腳本在解析當下
  // 打開並搬到右側欄。以前是先畫在右側／上方、等 site.js 才搬，慢網路下整頁往上跳（Lighthouse 手機 CLS 0.3）。
  const early = `<script>(function(d){if(!matchMedia('(min-width: 1200px)').matches)return;d.open=true;var n=d.parentNode,w=n.closest('.doc-wrap');if(w)w.appendChild(n);})(document.currentScript.parentNode)</script>`;
  return `<nav class="toc" aria-label="${UI[lang].toc}"><details><summary>${UI[lang].toc}</summary>${early}<ol>${li}</ol></details></nav>`;
}

// Product pages: every <h2> section below the sales block becomes a collapsible <details> (first one open),
// so the page stays short; the text is still in the HTML for search engines and find-in-page.
function collapseSections(html, file) {
  const start = html.search(/<h2 id=/);
  if (start < 0) return html;
  const sections = html.slice(start).split(/(?=<h2 id=)/);
  const count = (s, re) => (s.match(re) || []).length;
  const unbalanced = sections.find((s) => ['div', 'section', 'details', 'table'].some((t) =>
    count(s, new RegExp(`<${t}[\\s>]`, 'g')) !== count(s, new RegExp(`</${t}>`, 'g'))));
  if (unbalanced) { console.warn(`WARN ${file}: section HTML not balanced, sections left open`); return html; }
  return html.slice(0, start) + sections.map((s, i) => {
    const [, h2, rest] = s.match(/^(<h2[^>]*>[\s\S]*?<\/h2>)([\s\S]*)$/);
    // a link inside <summary> would swallow the click: drop the heading's anchor link
    const head = h2.replace(/<a href="#[^"]*" data-id="[^"]*" class="anchor"><span>([\s\S]*?)<\/span><\/a>/, '<span>$1</span>');
    return `<details class="acc"${i === 0 ? ' open' : ''}><summary>${head}</summary><div class="acc-body">${rest}</div></details>`;
  }).join('');
}

// schema.org Product + Offer for product index pages (TWD on zh pages, USD on en pages)
function productLd(key, lang, url, name, description) {
  const p = PRODUCT_DATA[key];
  if (!p || !p.msrp) return null;
  const en = lang === 'en';
  if (en && !p.msrp.usd) return null; // Taiwan-only product: no USD offer
  return {
    '@type': 'Product',
    '@id': SITE + url + '#product',
    name,
    description,
    sku: p[lang].badge.split(' · ')[0],
    image: SITE + (p.image || DEFAULT_OG),
    brand: { '@type': 'Brand', name: 'MBC-Pro Sensors' },
    manufacturer: { '@id': SITE + '/#organization' },
    offers: {
      '@type': 'Offer',
      url: SITE + url,
      price: String(en ? p.msrp.usd : p.msrp.twd),
      priceCurrency: en ? 'USD' : 'TWD',
      availability: 'https://schema.org/' + (p.zh.badge.includes('即將推出') ? 'PreOrder' : 'InStock'),
      seller: { '@id': SITE + '/#organization' },
    },
  };
}

// schema.org FAQPage from a faq.md page: each <h3> is a question, the text up to the next heading is its answer
function faqLd(html) {
  return {
    '@type': 'FAQPage',
    mainEntity: html.split(/<h3[^>]*>/).slice(1).map((part) => {
      const [q, rest] = part.split('</h3>');
      return { '@type': 'Question', name: stripTags(q), acceptedAnswer: { '@type': 'Answer', text: stripTags(rest.split(/<h[23][^>]*>/)[0]) } };
    }),
  };
}

function ctaDock(lang) {
  const u = UI[lang];
  return `<div class="cta-dock">${quoteLink(lang, 'dock-line', u.line)}<a class="dock-guide" href="${u.guideHref}">🧭 ${u.guide}</a></div>`;
}

// docs pages add a "contents" button that opens the sidebar on phones (replaces docsify's floating toggle)
function topNav(lang, langHref, docs = false) {
  const u = UI[lang];
  const home = lang === 'en' ? '/en/' : '/';
  return `<header class="topnav"><a class="tn-logo" href="${home}"><img src="/images/brand/mbc-logo.png" alt="MBC Robot" width="56" height="39"><span>MBC-Pro Sensors</span></a>` +
    (docs ? `<button class="tn-toc" aria-label="${u.toc}">${icon('list-tree')}<span>${u.toc}</span></button>` : '') +
    `<button class="tn-burger" aria-label="Menu" aria-expanded="false">${icon('menu')}</button>` +
    `<nav class="tn-links">${u.nav.map(([l, h]) => `<a href="${h}">${l}</a>`).join('')}` +
    `<a class="tn-lang" href="${langHref}" hreflang="${lang === 'en' ? 'zh-TW' : 'en'}">🌐 ${u.lang}</a>` +
    `${quoteLink(lang, 'tn-line', u.line)}</nav></header>`;
}

function page({ lang, url, title, description, ogImage, alternates, jsonLd, extraHead = '', sidebar, body, langHref, layout = 'docs', noindex = false, toc = '', canonical = null }) {
  const htmlLang = lang === 'en' ? 'en' : 'zh-TW';
  const abs = SITE + url;
  const alt = alternates.map((a) => `<link rel="alternate" hreflang="${a.hreflang}" href="${SITE + a.url}">`).join('\n  ');
  return `<!DOCTYPE html>
<html lang="${htmlLang}">
<head>
  <meta charset="UTF-8">
  <script>${HASH_REDIRECT}</script>
  <title>${escAttr(title)}</title>
  <meta name="viewport" content="width=device-width, initial-scale=1.0, minimum-scale=1.0">
  <meta name="description" content="${escAttr(description)}">
  <meta name="robots" content="${noindex ? 'noindex, follow' : 'index, follow, max-image-preview:large, max-snippet:-1, max-video-preview:-1'}">
  <link rel="canonical" href="${canonical ? SITE + canonical : abs}">
  ${alt}
  <link rel="icon" href="/favicon.ico" sizes="48x48">
  <link rel="apple-touch-icon" href="/images/brand/apple-touch-icon.png">
  <meta property="og:type" content="website">
  <meta property="og:url" content="${abs}">
  <meta property="og:title" content="${escAttr(title)}">
  <meta property="og:description" content="${escAttr(description)}">
  <meta property="og:image" content="${SITE + ogImage}">
  <meta property="og:image:width" content="1200">
  <meta property="og:image:height" content="630">
  <meta property="og:site_name" content="MBC-Pro Sensors">
  <meta property="og:locale" content="${lang === 'en' ? 'en_US' : 'zh_TW'}">
  <meta name="twitter:card" content="summary_large_image">
  <meta name="twitter:title" content="${escAttr(title)}">
  <meta name="twitter:description" content="${escAttr(description)}">
  <meta name="twitter:image" content="${SITE + ogImage}">
  ${extraHead}
  ${GA_SNIPPET}
  <script type="application/ld+json">${JSON.stringify(jsonLd)}</script>
  <link rel="preconnect" href="https://fonts.googleapis.com">
  <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
  <link rel="stylesheet" href="https://fonts.googleapis.com/css2?family=Nunito:wght@400;700&family=Noto+Sans+TC:wght@400;700&family=JetBrains+Mono:wght@400;700&family=Orbitron:wght@500;700&display=swap">
  <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/docsify@4.13.1/lib/themes/${layout === 'landing' ? 'dark' : 'vue'}.css">
  <link rel="stylesheet" href="/assets/site.css?v=${BUILD_ID}">
</head>
${layout === 'landing' ? `<body class="ready landing theme-light">
${topNav(lang, langHref)}
<main class="landing-main"><article class="markdown-section" id="main">
${body}
</article>${footerHtml(lang)}</main>` : `<body class="ready sticky docs theme-light">
${topNav(lang, langHref, true)}
<main>
<aside class="sidebar">
<div class="search"><div class="input-wrap"><input type="search" value="" aria-label="Search text" placeholder="${UI[lang].search}"></div><div class="results-panel"></div></div>
<h1 class="app-name"><a class="app-name-link" href="${lang === 'en' ? '/en/' : '/'}">MBC-Pro-Sensors</a></h1>
<div class="sidebar-nav">${sidebar}</div>
</aside>
<section class="content"><div class="doc-wrap${toc ? ' has-toc' : ''}"><article class="markdown-section" id="main">
${withToc(body, toc)}
</article></div>${footerHtml(lang)}</section>
</main>`}
${ctaDock(lang)}
<script src="https://cdn.jsdelivr.net/npm/scratchblocks@3.6.4/build/scratchblocks.min.js" defer></script>
<script src="/scratchblocks-init.js" defer></script>
<script src="/assets/site.js?v=${BUILD_ID}" defer></script>
</body>
</html>
`;
}

// -------------------------------------------------------------------- build
const BUILD_ID = Date.now().toString(36);
fs.rmSync(OUT, { recursive: true, force: true });
fs.mkdirSync(OUT, { recursive: true });

// 1. copy static assets (everything tracked except sources / tooling)
const SKIP = [/\.md$/, /^_build\//, /^\.github\//, /^\.agents\//, /^\.gitignore$/, /^index\.html$/, /^sitemap\.xml$/];
for (const f of tracked) {
  if (SKIP.some((re) => re.test(f))) continue;
  const src = path.join(ROOT, f);
  if (!fs.existsSync(src)) continue;
  const dst = path.join(OUT, f);
  fs.mkdirSync(path.dirname(dst), { recursive: true });
  fs.copyFileSync(src, dst);
}
fs.cpSync(path.join(BUILD, 'assets'), path.join(OUT, 'assets'), { recursive: true });
fs.writeFileSync(path.join(OUT, '.nojekyll'), '');

// 2. render pages
const pageSet = new Set(pages);
const homeHead = JSON.parse(fs.readFileSync(path.join(BUILD, 'home.json'), 'utf8'));
const searchIndex = { zh: [], en: [] };
const sitemap = [];

for (const file of pages) {
  const lang = file.startsWith('en/') ? 'en' : 'zh';
  const url = urlFor(file);
  let src = stripHidden(fs.readFileSync(path.join(ROOT, file), 'utf8').replace(/^﻿/, ''));
  // 三層導覽：<!-- guide: 產品/主機(/軟體) -->；「安裝與設定」include 與「範例」抽取在這裡展開
  const gm = src.match(/^<!-- guide: ([^/\s]+)\/([^/\s]+)(?:\/([^/\s]+))? -->/m);
  const guide = gm ? { key: gm[1], host: gm[2], sw: gm[3] || null } : null;
  if (guide) src = expandExamples(expandIncludes(src), lang);
  // 舊頁：<!-- moved: 新頁 -->（gen-guides 補的標記；標記被別的產生器蓋掉時用 guides-index 的對照）
  const movedMd = (src.match(/^<!-- moved: (\S+) -->/m) || [])[1] || GI.moved[file] || null;
  const movedUrl = movedMd ? mapHref(movedMd, file) : null;
  let html = renderMarkdown(src, file, lang);
  if (movedUrl) {
    html = html.replace(/<p class="moved-note">[\s\S]*?<\/p>\s*/, '');
    html = `<div class="moved-bar" role="note">${icon('arrow-right')}<span>${GL[lang].moved}<a href="${movedUrl}">${escAttr(movedLabel(movedMd))}</a></span></div>` + html;
  }
  for (const tag of ['div', 'section']) {
    const open = (html.match(new RegExp(`<${tag}[\\s>]`, 'g')) || []).length;
    const close = (html.match(new RegExp(`</${tag}>`, 'g')) || []).length;
    if (open !== close) console.warn(`WARN ${file}: ${open} <${tag}> vs ${close} </${tag}> — page layout will break`);
  }

  const h1 = (html.match(/<h1[^>]*>([\s\S]*?)<\/h1>/) || [])[1];
  const h1Text = stripEmoji(stripTags(h1 || '')) || 'MBC-Pro Sensors';
  const product = (file.match(/sensors\/([^/]+)\//) || [])[1];
  const isProductIndex = /sensors\/[^/]+\/index\.md$/.test(file);
  const isArticle = /^(en\/)?articles\/(?!index\.md$)/.test(file);
  const productName = product && PRODUCTS[lang][product];

  // counterpart in the other language
  const otherFile = lang === 'en' ? file.slice(3) : 'en/' + file;
  const hasOther = pageSet.has(otherFile);
  const zhUrl = lang === 'zh' ? url : (hasOther ? urlFor(otherFile) : null);
  const enUrl = lang === 'en' ? url : (hasOther ? urlFor(otherFile) : null);
  const alternates = [];
  if (zhUrl) alternates.push({ hreflang: 'zh-TW', url: zhUrl });
  if (enUrl) alternates.push({ hreflang: 'en', url: enUrl });
  if (zhUrl) alternates.push({ hreflang: 'x-default', url: zhUrl });
  const langHref = hasOther ? urlFor(otherFile) : (lang === 'en' ? '/' : '/en/');

  // title / description
  const home = homeHead[url];
  // 第 2、3 層頁面的 H1 已經含產品名，不再重複
  let title = home ? home.title : `${h1Text}${productName && !isProductIndex && !h1Text.includes(productName) ? ' – ' + productName : ''} | MBC-Pro Sensors`;
  const pitch = isProductIndex && PRODUCT_DATA[product] && PRODUCT_DATA[product][lang].pitch;
  let description = home ? home.description : pitch || describe(html) || homeHead[lang === 'en' ? '/en/' : '/'].description;
  // product sub-pages (setup guides) share opening text across products: lead with the product name so each is unique
  if (product && !isProductIndex && productName && !description.includes(productName)) {
    description = `${productName}${lang === 'en' ? ': ' : '：'}${description}`;
    if (description.length > 155) description = description.slice(0, 152) + '…';
  }

  // breadcrumbs
  const crumbs = [{ name: UI[lang].home, url: lang === 'en' ? '/en/' : '/' }];
  if (isArticle) crumbs.push({ name: lang === 'en' ? 'Articles' : '教學文章', url: lang === 'en' ? '/en/articles/' : '/articles/' });
  if (product && !isProductIndex && productName) crumbs.push({ name: productName, url: urlFor(`${lang === 'en' ? 'en/' : ''}sensors/${product}/index.md`) });
  if (guide && guide.sw) crumbs.push({ name: GHOST[guide.host].name[lang], url: gHostUrl(lang, guide.key, guide.host) });
  if (!home) crumbs.push({ name: h1Text, url });
  if (crumbs.length > 2) {
    const trail = crumbs.slice(0, -1).map((c) => `<a href="${c.url}">${escAttr(c.name)}</a>`).join('<span>›</span>');
    html = html.replace(/<\/h1>/, `</h1><nav class="breadcrumbs" aria-label="breadcrumb">${trail}</nav>`);
  }
  if (isProductIndex) {
    html = html.replace('<!-- product-hero -->', productHero(product, lang, file, h1Text));
    html = html.replace('<!-- platform-grid -->', platformGrid(product, lang));
    html = collapseSections(html, file);
    html += ctaHtml(lang);
  }
  if (html.includes('<!-- learn-hub -->')) html = html.replace('<!-- learn-hub -->', learnHubHtml(lang));
  if (html.includes('<!-- finder -->')) html = html.replace('<!-- finder -->', finderHtml(lang));
  if (guide) {
    html = html.replace('<!-- sw-grid -->', swGridHtml(lang, guide.key, guide.host) + learnBox(lang, guide.key, guide.host, null, 'host'));
    // 軟體頁：互動教學入口放最上面（標題、麵包屑之後）
    if (guide.sw) {
      const box = learnBox(lang, guide.key, guide.host, [guide.sw], 'sw');
      if (box) html = html.replace(/<\/h1>(?:<nav class="breadcrumbs"[\s\S]*?<\/nav>)?/, (m) => m + box);
    }
    html = switcherHtml(lang, guide) + html;
  }
  const layout = src.includes('<!-- layout: landing -->') ? 'landing' : 'docs';
  // edition chooser pages (SPIKE / EV3 hub) are navigation only: keep them out of the index and the sitemap;
  // micro:bit 範例頁在擴充上機驗證前也先不收錄
  // 只有一種軟體的平台頁（產品頁直接連軟體頁）標 <!-- noindex -->
  const noindex = /(spike|ev3)-hub\.md$/.test(file) || /microbit-examples\.md$/.test(file) || src.includes('<!-- noindex -->');
  // 右側目錄：文件頁（非產品 index，產品 index 的 h2 已摺成 details）
  const toc = layout === 'docs' && !isProductIndex ? tocHtml(html, lang) : '';

  const jsonLd = home ? home.jsonLd : {
    '@context': 'https://schema.org',
    '@graph': [
      {
        '@type': isArticle ? 'Article' : product && !isProductIndex ? 'TechArticle' : 'WebPage',
        ...(isArticle ? { headline: h1Text, datePublished: firstCommit(file), author: { '@id': SITE + '/#organization' } } : {}),
        '@id': SITE + url + '#webpage',
        url: SITE + url,
        name: title,
        description,
        inLanguage: lang === 'en' ? 'en' : 'zh-TW',
        image: SITE + ogImageFor(product),
        dateModified: lastmod(file),
        isPartOf: { '@id': SITE + '/#website' },
        publisher: { '@id': SITE + '/#organization' },
      },
      {
        '@type': 'BreadcrumbList',
        itemListElement: crumbs.map((c, i) => ({ '@type': 'ListItem', position: i + 1, name: c.name, item: SITE + c.url })),
      },
      ...(isProductIndex ? [productLd(product, lang, url, h1Text, description)].filter(Boolean) : []),
      ...(/(^|\/)faq\.md$/.test(file) ? [faqLd(html)] : []),
    ],
  };

  const out = page({
    lang, url, title, description, ogImage: ogImageFor(product), alternates, jsonLd,
    extraHead: home && home.keywords ? `<meta name="keywords" content="${escAttr(home.keywords)}">` : '',
    sidebar: renderSidebar(lang, url, guide || (isProductIndex ? { key: product, host: null } : null)), body: html, langHref, layout, noindex, toc,
    canonical: movedUrl ? movedUrl.replace(/#.*$/, '') : null,
  });
  const outFile = path.join(OUT, url.endsWith('/') ? url + 'index.html' : url);
  fs.mkdirSync(path.dirname(outFile), { recursive: true });
  fs.writeFileSync(outFile, finishPage(out));

  // 舊頁（已整合到新頁）不進站內搜尋與 sitemap
  if (!movedUrl) searchIndex[lang].push({ t: h1Text + (productName && !isProductIndex ? ` – ${productName}` : ''), u: url, x: stripEmoji(stripTags(html.replace(/<h1[\s\S]*?<\/h1>/, ''))).slice(0, 4000) });
  if (!noindex && !movedUrl) sitemap.push({ url, lastmod: lastmod(file), alternates, priority: home ? '1.0' : isProductIndex ? '0.9' : '0.7' });
}

fs.writeFileSync(path.join(OUT, 'search-index-zh.json'), JSON.stringify(searchIndex.zh));
fs.writeFileSync(path.join(OUT, 'search-index-en.json'), JSON.stringify(searchIndex.en));

// 3. sitemap.xml
const sm = ['<?xml version="1.0" encoding="UTF-8"?>',
  '<urlset xmlns="http://www.sitemaps.org/schemas/sitemap/0.9" xmlns:xhtml="http://www.w3.org/1999/xhtml">'];
for (const s of sitemap) {
  sm.push('  <url>', `    <loc>${SITE + s.url}</loc>`, `    <lastmod>${s.lastmod}</lastmod>`, `    <priority>${s.priority}</priority>`);
  for (const a of s.alternates) sm.push(`    <xhtml:link rel="alternate" hreflang="${a.hreflang}" href="${SITE + a.url}"/>`);
  sm.push('  </url>');
}
sm.push('</urlset>', '');
fs.writeFileSync(path.join(OUT, 'sitemap.xml'), sm.join('\n'));

// 4. 404 page — also rescues old .md and Docsify hash URLs
const notFound = page({
  lang: 'zh', url: '/404.html', title: '找不到頁面 | MBC-Pro Sensors', description: '找不到此頁面，請回到首頁或使用左側選單。',
  ogImage: DEFAULT_OG, alternates: [], jsonLd: { '@context': 'https://schema.org', '@type': 'WebPage', name: 'Not found' },
  extraHead: `<meta name="robots" content="noindex"><script>(function(){var p=location.pathname;if(/\\.md$/.test(p)){var u=p.replace(/\\.md$/,'');if(/\\/README$/.test(u))u=u.slice(0,-6);else if(/\\/index$/.test(u))u=u.slice(0,-5);else u+='.html';location.replace(u);}})();</script>`,
  sidebar: renderSidebar('zh', ''),
  body: '<h1>404 — 找不到頁面 / Page not found</h1><p>這個網址可能已經更新。請使用左側選單，或 <a href="/">回到首頁</a> / <a href="/en/">English home</a>。</p>',
  langHref: '/en/',
});
fs.writeFileSync(path.join(OUT, '404.html'), finishPage(notFound));

if (ytMissing.size) console.warn(`WARN YouTube 縮圖缺 ${[...ytMissing].join(' ')}：跑 py -3 _build/tools/yt_thumbs.py ${[...ytMissing].join(' ')}`);
console.log(`Built ${pages.length} pages -> ${path.relative(ROOT, OUT)}/`);
