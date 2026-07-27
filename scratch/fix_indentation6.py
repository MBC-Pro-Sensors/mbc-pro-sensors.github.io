# -*- coding: utf-8 -*-
import os
import io
import re

def fix(filepath):
    with io.open(filepath, 'r', encoding='utf-8') as f:
        c = f.read()
    orig_c = c
    
    pattern = r'[ \t]*<div style="display: flex; flex-wrap: wrap; gap: 20px;[^>]*>.*?LegoLauXiao.*?legolaumo.*?</iframe[ \t]*>\r?\n[ \t]*</div>\r?\n[ \t]*</div>\r?\n[ \t]*</div>'
    
    matches = re.findall(pattern, c, flags=re.DOTALL | re.IGNORECASE)
    for m in matches:
        new_m = u'\n'.join([line.lstrip() for line in m.split(u'\n')])
        c = c.replace(m, new_m)
        
    if c != orig_c:
        if u"AI \u6ce8\u610f" not in c:
            ai_warning = u"<!-- AI \u6ce8\u610f\uff1a\u5728\u6b64\u5c08\u6848\u7684 Markdown \u6a94\u6848\u4e2d\uff0c\u76f4\u63a5\u63d2\u5165\u7684 HTML \u7a0b\u5f0f\u78bc\u8acb\u52ff\u4f7f\u7528\u4efb\u4f55\u7e2e\u6392\uff0c\u5426\u5247\u6703\u88ab Markdown \u89e3\u6790\u5668\u8aa4\u8a8d\u70ba\u7a0b\u5f0f\u78bc\u5340\u584a\uff08\u8b8a\u6210\u6587\u672c\u8996\u7a97\uff09\u3002 -->\n"
            c = ai_warning + c
        with io.open(filepath, 'w', encoding='utf-8') as f:
            f.write(c)
        print("Fixed: " + filepath)

for root, dirs, files in os.walk(r"d:\Gemini_Antigravity\mbc-pro-sensors.github.io"):
    if 'scratch' in root or '.git' in root: continue
    for file in files:
        if file.endswith(".md"): fix(os.path.join(root, file))
