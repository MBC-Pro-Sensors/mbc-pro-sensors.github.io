# -*- coding: utf-8 -*-
"""YouTube 縮圖存到 images/yt/<ID>.webp（build 把影片換成「點了才載入」，縮圖放自己網站，不連 YouTube）。
用法：py -3 _build/tools/yt_thumbs.py            # 掃 _site 裡用到的影片，缺的才下載
     py -3 _build/tools/yt_thumbs.py ID1 ID2    # 指定影片
新增影片後跑一次、commit 圖檔；build 遇到沒有縮圖的影片會印 WARN。"""
import io, os, re, sys, glob, urllib.request
from PIL import Image
ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
OUT = os.path.join(ROOT, "images", "yt")
ids = sys.argv[1:]
if not ids:
    pat = re.compile(r"youtube(?:-nocookie)?\.com/embed/([A-Za-z0-9_-]{11})|data-yt=\"([A-Za-z0-9_-]{11})\"")
    found = set()
    for f in glob.glob(os.path.join(ROOT, "_site", "**", "*.html"), recursive=True):
        for m in pat.finditer(open(f, encoding="utf-8").read()):
            found.add(m.group(1) or m.group(2))
    ids = sorted(i for i in found if not os.path.exists(os.path.join(OUT, i + ".webp")))
os.makedirs(OUT, exist_ok=True)
def get(url):
    try:
        with urllib.request.urlopen(url, timeout=20) as r:
            return r.read()
    except Exception:
        return None

for vid in ids:
    # Shorts（直式）有 oardefault 1080×1920 → 存 360×640；一般影片存 640×360（4:3 縮圖的上下黑邊裁掉）
    data = get(f"https://i.ytimg.com/vi/{vid}/oardefault.jpg")
    size = (360, 640) if data else (640, 360)
    for name in ("maxresdefault", "sddefault", "hqdefault"):
        if data: break
        data = get(f"https://i.ytimg.com/vi/{vid}/{name}.jpg")
    if not data:
        print("下載失敗", vid); continue
    im = Image.open(io.BytesIO(data)).convert("RGB")
    w, h = im.size
    tw, th = size
    if w * th > h * tw:                       # 比目標寬 → 裁左右
        nw = round(h * tw / th); im = im.crop(((w - nw) // 2, 0, (w - nw) // 2 + nw, h))
    else:                                     # 比目標高 → 裁上下
        nh = round(w * th / tw); im = im.crop((0, (h - nh) // 2, w, (h - nh) // 2 + nh))
    im = im.resize(size, Image.LANCZOS)
    p = os.path.join(OUT, vid + ".webp")
    im.save(p, "WEBP", quality=72, method=6)
    print(vid, size, os.path.getsize(p) // 1024, "KB")
