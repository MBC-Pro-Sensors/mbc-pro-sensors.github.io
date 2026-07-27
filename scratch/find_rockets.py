import io, os

for root, dirs, files in os.walk(r"d:\Gemini_Antigravity\mbc-pro-sensors.github.io"):
    for file in files:
        if file.endswith(".md"):
            filepath = os.path.join(root, file)
            with io.open(filepath, 'r', encoding='utf-8') as f:
                c = f.read()
            if c.count(u'🚀') > 1:
                print(filepath, c.count(u'🚀'))
