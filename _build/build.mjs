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

const require = createRequire(import.meta.url);
const marked = require('marked'); // v1.2.9 — same version Docsify 4.13 bundles
const Prism = require('prismjs');
require('prismjs/components/prism-python');

const BUILD = path.dirname(fileURLToPath(import.meta.url));
const ROOT = path.resolve(BUILD, '..');
const OUT = path.join(ROOT, '_site');
const SITE = 'https://mbc-pro-sensors.github.io';
const DEFAULT_OG = '/images/brand/og-image.jpg';

const git = (args) => execSync(`git ${args}`, { cwd: ROOT, encoding: 'utf8' });
const tracked = git('-c core.quotepath=off ls-files').split('\n').filter(Boolean);

// ---------------------------------------------------------------- URL mapping
const isPage = (f) => f.endsWith('.md') && (f === 'README.md' || f === 'contact.md' || /^(en\/)?(README|contact)\.md$/.test(f) || /^(en\/)?sensors\//.test(f));
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
    const t = stripEmoji(stripTags(c));
    if (t.length >= 20) return t.length > 155 ? t.slice(0, 152) + '…' : t;
  }
  return '';
}

function lastmod(file) {
  try { return git(`log -1 --format=%cs -- "${file}"`).trim() || new Date().toISOString().slice(0, 10); }
  catch { return new Date().toISOString().slice(0, 10); }
}

// Product names from the sidebars: "- **循行者 8 路 (Line8)**" followed by its index link
function productNames(sidebarFile) {
  const names = {};
  const lines = fs.readFileSync(path.join(ROOT, sidebarFile), 'utf8').split('\n');
  lines.forEach((line, i) => {
    const m = line.match(/^\s+- \*\*(.+?)\*\*\s*$/);
    if (!m) return;
    const next = lines.slice(i + 1, i + 3).join('\n').match(/\/sensors\/([^/]+)\/index\.md/);
    if (next) names[next[1]] = m[1];
  });
  return names;
}
const PRODUCTS = { zh: productNames('_sidebar.md'), en: productNames('en/_sidebar.md') };

function ogImageFor(product) {
  const p = product && `/images/brand/og/${product}.jpg`;
  return p && fs.existsSync(path.join(ROOT, p)) ? p : DEFAULT_OG;
}

// ------------------------------------------------------------------ sidebar
function renderSidebar(lang, currentUrl) {
  const file = lang === 'en' ? 'en/_sidebar.md' : '_sidebar.md';
  const renderer = new marked.Renderer();
  renderer.link = (href, title, text) => {
    const mapped = mapHref(href, file) || href;
    const active = mapped === currentUrl ? ' class="active"' : '';
    return `<a href="${mapped}"${active} title="${escAttr(stripTags(text))}">${text}</a>`;
  };
  let html = marked(fs.readFileSync(path.join(ROOT, file), 'utf8'), { renderer, gfm: true });
  html = html.replace(/<li>((?:(?!<li>)[\s\S])*?class="active")/, '<li class="active">$1');
  html = html.replace(/<img(?![^>]*\balt=)/g, '<img alt=""'); // decorative hub icons
  return html;
}

// ----------------------------------------------------------------- template
const HASH_REDIRECT = `(function(){function go(){var h=location.hash;if(h.indexOf('#/')!==0)return;var p=h.slice(2),f='',q=p.search(/[?#]/);if(q>-1){var m=p.slice(q).match(/id=([^&]+)/);f=m?'#'+m[1]:'';p=p.slice(0,q);}p=p.replace(/\\.md$/,'');var u;if(p===''||p==='README')u='/';else if(p==='en'||p==='en/'||p==='en/README')u='/en/';else if(p.slice(-1)==='/')u='/'+p;else if(p==='index'||/\\/index$/.test(p))u='/'+p.slice(0,-5);else u='/'+p+'.html';location.replace(u+f);}go();addEventListener('hashchange',go);})();`;

const UI = {
  zh: { search: '🔍 搜尋文件', lang: 'EN', home: '首頁', cta: { h: '想購買或詢價？', p: '個人購買、學校 / 社團大量採購、代理洽談都歡迎，工程師工作日 24 小時內回覆。', line: 'LINE 立即詢價', mail: 'Email 詢價', more: '更多聯絡方式', contact: '/contact.html' } },
  en: { search: '🔍 Search docs', lang: '中文', home: 'Home', cta: { h: 'Interested in this sensor?', p: 'Ask us for pricing, bulk / school orders or distributor info. Our engineers reply within 24 hours on weekdays.', line: 'Chat on LINE', mail: 'Email us', more: 'All contact options', contact: '/en/contact.html' } },
};

function ctaHtml(lang) {
  const t = UI[lang].cta;
  return `<div class="buy-cta"><h3>${t.h}</h3><p>${t.p}</p><div class="buy-cta-btns">` +
    `<a class="buy-btn line" href="https://line.me/R/ti/p/@692vcvuk" target="_blank" rel="noopener">💬 ${t.line}</a>` +
    `<a class="buy-btn mail" href="mailto:mbc.robot89@gmail.com">✉️ ${t.mail}</a>` +
    `<a class="buy-btn more" href="${t.contact}">${t.more} →</a></div></div>`;
}

function page({ lang, url, title, description, ogImage, alternates, jsonLd, extraHead = '', sidebar, body, langHref }) {
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
  <meta name="robots" content="index, follow, max-image-preview:large, max-snippet:-1, max-video-preview:-1">
  <link rel="canonical" href="${abs}">
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
  <script type="application/ld+json">${JSON.stringify(jsonLd)}</script>
  <link rel="preconnect" href="https://fonts.googleapis.com">
  <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
  <link rel="stylesheet" href="https://fonts.googleapis.com/css2?family=Nunito:wght@400;700&family=Orbitron:wght@500;700&display=swap">
  <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/docsify@4.13.1/lib/themes/dark.css">
  <link rel="stylesheet" href="/assets/site.css?v=${BUILD_ID}">
</head>
<body class="ready sticky">
<main>
<button class="sidebar-toggle" aria-label="Menu"><div class="sidebar-toggle-button"><span></span><span></span><span></span></div></button>
<aside class="sidebar">
<div class="search"><div class="input-wrap"><input type="search" value="" aria-label="Search text" placeholder="${UI[lang].search}"></div><div class="results-panel"></div></div>
<h1 class="app-name"><a class="app-name-link" href="${lang === 'en' ? '/en/' : '/'}">MBC-Pro-Sensors</a></h1>
<div class="sidebar-nav">${sidebar}</div>
</aside>
<section class="content"><article class="markdown-section" id="main">
${body}
</article></section>
</main>
<a id="lang-toggle-btn" href="${langHref}" hreflang="${lang === 'en' ? 'zh-TW' : 'en'}"><span>🌐</span> <span>${UI[lang].lang}</span></a>
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
  const src = fs.readFileSync(path.join(ROOT, file), 'utf8').replace(/^﻿/, '');
  let html = renderMarkdown(src, file, lang);
  for (const tag of ['div', 'section']) {
    const open = (html.match(new RegExp(`<${tag}[\\s>]`, 'g')) || []).length;
    const close = (html.match(new RegExp(`</${tag}>`, 'g')) || []).length;
    if (open !== close) console.warn(`WARN ${file}: ${open} <${tag}> vs ${close} </${tag}> — page layout will break`);
  }

  const h1 = (html.match(/<h1[^>]*>([\s\S]*?)<\/h1>/) || [])[1];
  const h1Text = stripEmoji(stripTags(h1 || '')) || 'MBC-Pro Sensors';
  const product = (file.match(/sensors\/([^/]+)\//) || [])[1];
  const isProductIndex = /sensors\/[^/]+\/index\.md$/.test(file);
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
  let title = home ? home.title : `${h1Text}${productName && !isProductIndex ? ' – ' + productName : ''} | MBC-Pro Sensors`;
  let description = home ? home.description : describe(html) || homeHead[lang === 'en' ? '/en/' : '/'].description;

  // breadcrumbs
  const crumbs = [{ name: UI[lang].home, url: lang === 'en' ? '/en/' : '/' }];
  if (product && !isProductIndex && productName) crumbs.push({ name: productName, url: urlFor(`${lang === 'en' ? 'en/' : ''}sensors/${product}/index.md`) });
  if (!home) crumbs.push({ name: h1Text, url });
  if (crumbs.length > 2) {
    const trail = crumbs.slice(0, -1).map((c) => `<a href="${c.url}">${escAttr(c.name)}</a>`).join('<span>›</span>');
    html = html.replace(/<\/h1>/, `</h1><nav class="breadcrumbs" aria-label="breadcrumb">${trail}</nav>`);
  }
  if (isProductIndex) html += ctaHtml(lang);

  const jsonLd = home ? home.jsonLd : {
    '@context': 'https://schema.org',
    '@graph': [
      {
        '@type': product && !isProductIndex ? 'TechArticle' : 'WebPage',
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
    ],
  };

  const out = page({
    lang, url, title, description, ogImage: ogImageFor(product), alternates, jsonLd,
    extraHead: home && home.keywords ? `<meta name="keywords" content="${escAttr(home.keywords)}">` : '',
    sidebar: renderSidebar(lang, url), body: html, langHref,
  });
  const outFile = path.join(OUT, url.endsWith('/') ? url + 'index.html' : url);
  fs.mkdirSync(path.dirname(outFile), { recursive: true });
  fs.writeFileSync(outFile, out);

  searchIndex[lang].push({ t: h1Text + (productName && !isProductIndex ? ` – ${productName}` : ''), u: url, x: stripEmoji(stripTags(html.replace(/<h1[\s\S]*?<\/h1>/, ''))).slice(0, 4000) });
  sitemap.push({ url, lastmod: lastmod(file), alternates, priority: home ? '1.0' : isProductIndex ? '0.9' : '0.7' });
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
fs.writeFileSync(path.join(OUT, '404.html'), notFound);

console.log(`Built ${pages.length} pages -> ${path.relative(ROOT, OUT)}/`);
