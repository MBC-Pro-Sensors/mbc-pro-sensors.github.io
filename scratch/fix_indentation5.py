# -*- coding: utf-8 -*-
import os
import io

def fix(filepath):
    with io.open(filepath, 'r', encoding='utf-8') as f:
        content = f.read()

    start_tag = u'<div style="display: flex; flex-wrap: wrap; gap: 20px;'
    
    start_idx = 0
    changed = False
    new_content = u""
    
    while True:
        idx = content.find(start_tag, start_idx)
        if idx == -1:
            new_content += content[start_idx:]
            break
            
        vid_idx = content.find(u'T9bcndBNQvQ', idx)
        if vid_idx != -1 and vid_idx - idx < 2000:
            end_idx = content.find(u'</div>', vid_idx)
            end_idx = content.find(u'</div>', end_idx + 1)
            end_idx = content.find(u'</div>', end_idx + 1)
            if end_idx != -1:
                end_idx += len(u'</div>')
                line_start_idx = content.rfind(u'\n', start_idx, idx)
                if line_start_idx == -1:
                    line_start_idx = 0
                else:
                    line_start_idx += 1
                    
                new_content += content[start_idx:line_start_idx]
                
                original_block = content[line_start_idx:end_idx]
                lines = original_block.split(u'\n')
                new_lines = [line.lstrip() for line in lines]
                new_block = u'\n'.join(new_lines)
                
                if new_block != original_block:
                    changed = True
                new_content += new_block
                start_idx = end_idx
                continue
                
        new_content += content[start_idx:idx+len(start_tag)]
        start_idx = idx + len(start_tag)
        
    if changed:
        if u"AI \u6ce8\u610f" not in new_content:
            ai_warning = u"<!-- AI \u6ce8\u610f\uff1a\u5728\u6b64\u5c08\u6848\u7684 Markdown \u6a94\u6848\u4e2d\uff0c\u76f4\u63a5\u63d2\u5165\u7684 HTML \u7a0b\u5f0f\u78bc\u8acb\u52ff\u4f7f\u7528\u4efb\u4f55\u7e2e\u6392\uff0c\u5426\u5247\u6703\u88ab Markdown \u89e3\u6790\u5668\u8aa4\u8a8d\u70ba\u7a0b\u5f0f\u78bc\u5340\u584a\uff08\u8b8a\u6210\u6587\u672c\u8996\u7a97\uff09\u3002 -->\n"
            new_content = ai_warning + new_content
        with io.open(filepath, 'w', encoding='utf-8') as f:
            f.write(new_content)
        print("Fixed: " + filepath)

def walk_dir(directory):
    for root, dirs, files in os.walk(directory):
        if 'scratch' in root or '.git' in root:
            continue
        for file in files:
            if file.endswith(".md"):
                fix(os.path.join(root, file))

if __name__ == "__main__":
    workspace = r"d:\Gemini_Antigravity\mbc-pro-sensors.github.io"
    walk_dir(workspace)
