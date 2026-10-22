# -*- coding: utf-8 -*-
"""插圖後製：寬 ≤1200、轉 WEBP q85、右下角疊 MBC logo（高度 = 圖高 8%）。

用法：
  py -3 _build/tools/webp_logo.py images/illust/host-spike.png            # 同名 .webp
  py -3 _build/tools/webp_logo.py images/illust/*.png --out-dir images/illust
  py -3 _build/tools/webp_logo.py --dry-run images/illust/host-spike.png   # 只印尺寸，不寫檔
選項：--max-width 1200 --quality 85 --logo images/brand/mbc-logo.png --logo-ratio 0.08 --margin 0.02 --no-logo
"""
import argparse
import glob
import os
import sys

if hasattr(sys.stdout, "reconfigure"):
    sys.stdout.reconfigure(encoding="utf-8")

from PIL import Image

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..'))
DEFAULT_LOGO = os.path.join(ROOT, 'images', 'brand', 'mbc-logo.png')


def rel(p: str) -> str:  # 跨磁碟時 relpath 會丟 ValueError，就印完整路徑
    try:
        return os.path.relpath(p, ROOT)
    except ValueError:
        return p


def process(src: str, dst: str, max_width: int, quality: int, logo_path: str, logo_ratio: float, margin: float, dry: bool) -> None:
    im = Image.open(src).convert('RGBA')
    if im.width > max_width:
        im = im.resize((max_width, round(im.height * max_width / im.width)), Image.LANCZOS)
    if logo_path:
        logo = Image.open(logo_path).convert('RGBA')
        h = max(1, round(im.height * logo_ratio))
        logo = logo.resize((round(logo.width * h / logo.height), h), Image.LANCZOS)
        m = round(im.height * margin)
        im.alpha_composite(logo, (im.width - logo.width - m, im.height - logo.height - m))
    print(f'{rel(src)} → {rel(dst)} ({im.width}×{im.height}, q{quality}{", logo" if logo_path else ""})')
    if dry:
        return
    os.makedirs(os.path.dirname(dst), exist_ok=True)
    im.convert('RGB').save(dst, 'WEBP', quality=quality, method=6)


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('inputs', nargs='+')
    ap.add_argument('--out-dir', default=None)
    ap.add_argument('--max-width', type=int, default=1200)
    ap.add_argument('--quality', type=int, default=85)
    ap.add_argument('--logo', default=DEFAULT_LOGO)
    ap.add_argument('--logo-ratio', type=float, default=0.08)
    ap.add_argument('--margin', type=float, default=0.02)
    ap.add_argument('--no-logo', action='store_true')
    ap.add_argument('--dry-run', action='store_true')
    a = ap.parse_args()

    files = []
    for pat in a.inputs:
        hits = glob.glob(pat)
        files.extend(hits if hits else [pat])
    files = [f for f in files if f.lower().endswith(('.png', '.jpg', '.jpeg', '.webp'))]
    if not files:
        print('沒有輸入圖', file=sys.stderr)
        return 2
    logo = None if a.no_logo else a.logo
    if logo and not os.path.exists(logo):
        print(f'找不到 logo：{logo}', file=sys.stderr)
        return 2
    for f in files:
        base = os.path.splitext(os.path.basename(f))[0] + '.webp'
        dst = os.path.join(a.out_dir, base) if a.out_dir else os.path.join(os.path.dirname(f), base)
        process(f, dst, a.max_width, a.quality, logo, a.logo_ratio, a.margin, a.dry_run)
    return 0


if __name__ == '__main__':
    sys.exit(main())
