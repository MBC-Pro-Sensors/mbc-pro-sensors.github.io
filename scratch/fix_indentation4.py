# -*- coding: utf-8 -*-
import os
import io
import re

def fix(filepath):
    with io.open(filepath, 'r', encoding='utf-8') as f:
        content = f.read()

    # Pattern starts with the specific flex div and ends with the 3rd </div> after legolaumo's iframe.
    # Note: Using strict regex to not over-match
    pattern = r'[ \t]*<div style="display: flex; flex-wrap: wrap; gap: 20px;.*?legolaumo.*?</iframe[ \t]*>\s*</div>\s*</div>\s*</div>'
    
    changed = False
    new_content = content
    
    matches = re.finditer(pattern, content, flags=re.DOTALL | re.IGNORECASE)
    for match in matches:
        original_block = match.group(0)
        
        # Double check it actually contains the coaches
        if u"LegoLauXiao" in original_block and u"legolaumo" in original_block:
            lines = original_block.split(u'\n')
            new_lines = [line.lstrip() for line in lines]
            new_block = u'\n'.join(new_lines)
            
            if new_block != original_block:
                new_content = new_content.replace(original_block, new_block)
                changed = True

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
