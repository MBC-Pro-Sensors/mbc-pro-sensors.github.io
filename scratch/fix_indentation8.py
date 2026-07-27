# -*- coding: utf-8 -*-
import io, os

def fix(filepath):
    with io.open(filepath, 'r', encoding='utf-8') as f:
        c = f.read()
    orig = c
    
    idx = c.find(u'WgacdWLatbk')
    if idx != -1:
        start_tag = u'<div style="display: flex; flex-wrap: wrap; gap: 20px;'
        start_idx = c.rfind(start_tag, 0, idx)
        if start_idx == -1:
            print("Could not find start_tag in " + filepath)
        
        vid2_idx = c.find(u'T9bcndBNQvQ', idx)
        if vid2_idx == -1:
            print("Could not find T9bcndBNQvQ in " + filepath)
            
        if start_idx != -1 and vid2_idx != -1:
            end_idx = c.find(u'</div>', vid2_idx)
            end_idx = c.find(u'</div>', end_idx + 1)
            end_idx = c.find(u'</div>', end_idx + 1)
            if end_idx != -1:
                end_idx += len(u'</div>')
                line_start = c.rfind(u'\n', 0, start_idx)
                if line_start == -1: line_start = 0
                else: line_start += 1
                
                block = c[line_start:end_idx]
                lines = block.split(u'\n')
                new_block = u'\n'.join([line.lstrip() for line in lines])
                
                if block != new_block:
                    c = c[:line_start] + new_block + c[end_idx:]
                    if u"AI \u6ce8\u610f" not in c:
                        ai_warning = u"<!-- AI \u6ce8\u610f\uff1a\u5728\u6b64\u5c08\u6848\u7684 Markdown \u6a94\u6848\u4e2d\uff0c\u76f4\u63a5\u63d2\u5165\u7684 HTML \u7a0b\u5f0f\u78bc\u8acb\u52ff\u4f7f\u7528\u4efb\u4f55\u7e2e\u6392\uff0c\u5426\u5247\u6703\u88ab Markdown \u89e3\u6790\u5668\u8aa4\u8a8d\u70ba\u7a0b\u5f0f\u78bc\u5340\u584a\uff08\u8b8a\u6210\u6587\u672c\u8996\u7a97\uff09\u3002 -->\n"
                        c = ai_warning + c
                    with io.open(filepath, 'w', encoding='utf-8') as f:
                        f.write(c)
                    print("Fixed: " + filepath)
                else:
                    print("Already unindented: " + filepath)
            else:
                print("Could not find end </div> in " + filepath)

for root, dirs, files in os.walk(r"d:\Gemini_Antigravity\mbc-pro-sensors.github.io"):
    if 'scratch' in root or '.git' in root: continue
    for file in files:
        if file.endswith(".md"): fix(os.path.join(root, file))
