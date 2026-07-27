# -*- coding: utf-8 -*-
import os
import io

def fix_file(filepath):
    with io.open(filepath, 'r', encoding='utf-8') as f:
        lines = f.readlines()

    new_lines = []
    in_target_block = False
    block_lines_collected = []
    changed = False

    for line in lines:
        if u'<div style="display: flex; flex-wrap: wrap; gap: 20px; justify-content: center; margin-top: 15px;' in line:
            # We found the start of our flex block
            # If we were already in a block, something is weird, just flush it
            if in_target_block:
                new_lines.extend(block_lines_collected)
            in_target_block = True
            block_lines_collected = [line]
        elif in_target_block:
            block_lines_collected.append(line)
            # check if it's our target block by looking for the video IDs
            joined_block = u"".join(block_lines_collected)
            
            # Count the divs to see if the block is complete
            div_open = joined_block.count(u'<div')
            div_close = joined_block.count(u'</div')
            
            if div_close >= div_open and div_open > 0:
                # The div is closed. Is it our Lego block?
                if u'WgacdWLatbk' in joined_block and u'T9bcndBNQvQ' in joined_block:
                    # Yes! Unindent all lines in block_lines_collected
                    for b_line in block_lines_collected:
                        new_lines.append(b_line.lstrip())
                    changed = True
                else:
                    # Not our block, keep as is
                    new_lines.extend(block_lines_collected)
                
                in_target_block = False
                block_lines_collected = []
        else:
            new_lines.append(line)
            
    if in_target_block:
        new_lines.extend(block_lines_collected)

    if changed:
        content = u"".join(new_lines)
        if u"AI \u6ce8\u610f" not in content:
            ai_warning = u"<!-- AI \u6ce8\u610f\uff1a\u5728\u6b64\u5c08\u6848\u7684 Markdown \u6a94\u6848\u4e2d\uff0c\u76f4\u63a5\u63d2\u5165\u7684 HTML \u7a0b\u5f0f\u78bc\u8acb\u52ff\u4f7f\u7528\u4efb\u4f55\u7e2e\u6392\uff0c\u5426\u5247\u6703\u88ab Markdown \u89e3\u6790\u5668\u8aa4\u8a8d\u70ba\u7a0b\u5f0f\u78bc\u5340\u584a\uff08\u8b8a\u6210\u6587\u672c\u8996\u7a97\uff09\u3002 -->\n"
            content = ai_warning + content
        with io.open(filepath, 'w', encoding='utf-8') as f:
            f.write(content)
        print("Fixed: " + filepath)

def walk_dir(directory):
    for root, dirs, files in os.walk(directory):
        if 'scratch' in root or '.git' in root:
            continue
        for file in files:
            if file.endswith(".md"):
                fix_file(os.path.join(root, file))

if __name__ == "__main__":
    workspace = r"d:\Gemini_Antigravity\mbc-pro-sensors.github.io"
    walk_dir(workspace)
