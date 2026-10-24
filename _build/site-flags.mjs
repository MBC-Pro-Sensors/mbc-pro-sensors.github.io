// 網站內容開關（_build/site-flags.json）：各產生器與 build.mjs 共用
import fs from 'node:fs';
import path from 'node:path';
import { fileURLToPath } from 'node:url';

const BUILD = path.dirname(fileURLToPath(import.meta.url));
export const FLAGS = JSON.parse(fs.readFileSync(path.join(BUILD, 'site-flags.json'), 'utf8'));
export const HIDE_BIG = !!FLAGS.hideMatrixBigChannel;

// 手寫頁的開關區塊：
//   <!-- hide:旗標名 -->…<!-- /hide -->  旗標為 true 時整段拿掉
//   <!-- show:旗標名 -->…<!-- /show -->  只有旗標為 true 時才顯示（隱藏後的替代說明）
export function stripHidden(src) {
  return src
    .replace(/<!-- hide:(\w+) -->[\s\S]*?<!-- \/hide -->\r?\n?/g, (all, flag) => (FLAGS[flag] ? '' : all))
    .replace(/<!-- show:(\w+) -->([\s\S]*?)<!-- \/show -->\r?\n?/g, (all, flag, body) => (FLAGS[flag] ? body.replace(/^\r?\n/, '') : ''));
}
