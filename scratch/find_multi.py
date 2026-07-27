# -*- coding: utf-8 -*-
import io, os

for root, dirs, files in os.walk(r"d:\Gemini_Antigravity\mbc-pro-sensors.github.io"):
    if '.git' in dirs: dirs.remove('.git')
    for file in files:
        if file.endswith(".md"):
            filepath = os.path.join(root, file)
            with io.open(filepath, 'r', encoding='utf-8') as f:
                c = f.read()
            if c.count(u'LegoLauXiao') > 1:
                print("FOUND in", filepath)
