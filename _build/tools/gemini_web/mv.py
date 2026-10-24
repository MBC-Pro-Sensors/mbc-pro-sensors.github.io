import sys, os, glob, time, shutil
from PIL import Image
name = sys.argv[1]
ROOT = r'D:\AI_Agent_AI助理\02_MBC產品開發\95_產品網站\mbc-pro-sensors.github.io'
DL = r'C:\Users\ChenLongXiong\Downloads'
G3 = os.path.dirname(os.path.abspath(__file__))
for _ in range(30):
    fs = [f for f in glob.glob(os.path.join(DL, 'Gemini_Generated_Image_*')) if not f.endswith('.crdownload') and time.time() - os.path.getmtime(f) < 300]
    if fs: break
    time.sleep(1)
else:
    print('NO FILE'); sys.exit(1)
f = max(fs, key=os.path.getmtime)
time.sleep(1)
im = Image.open(f).convert('RGB')
out = os.path.join(ROOT, 'images', 'illust', name + '.png')
im.save(out)
os.makedirs(os.path.join(G3, 'raw'), exist_ok=True)
shutil.move(f, os.path.join(G3, 'raw', name + os.path.splitext(f)[1]))
t = im.copy(); t.thumbnail((768, 768)); t.save(os.path.join(G3, 'check.png'))
print(name, im.size, 'saved')
