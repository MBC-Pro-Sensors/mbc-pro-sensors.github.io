# -*- coding: utf-8 -*-
import io, os, re

repo = r"d:\Gemini_Antigravity\mbc-pro-sensors.github.io"
files_to_update = [
    r"sensors\line8\spike-pybricks.md",
    r"en\sensors\line8\spike-pybricks.md",
    r"sensors\line16\spike-pybricks.md",
    r"en\sensors\line16\spike-pybricks.md"
]

def update_line(line):
    # Regex to capture the href, extract filename, and then update both href and download attributes
    # Example: <a href="../examples/line8/pybricks/For%20firmware%203.6.1/line8_block_native.py" target="_blank" download="line8_block_native.py" ...
    
    # 3.6.1 replacements
    if 'For%20firmware%203.6.1' in line:
        line = line.replace(u'line8_block_native.py', u'line8_block_native_v361.py')
        line = line.replace(u'line8_block_with_lib.py', u'line8_block_with_lib_v361.py')
        line = line.replace(u'line8_python_native.py', u'line8_python_native_v361.py')
        line = line.replace(u'line8_python_with_lib.py', u'line8_python_with_lib_v361.py')
        line = line.replace(u'MBC_line8_Lib.py', u'MBC_line8_Lib_v361.py')
        
        line = line.replace(u'line16_block_native.py', u'line16_block_native_v361.py')
        line = line.replace(u'line16_block_with_lib.py', u'line16_block_with_lib_v361.py')
        line = line.replace(u'line16_python_native.py', u'line16_python_native_v361.py')
        line = line.replace(u'line16_python_with_lib.py', u'line16_python_with_lib_v361.py')
        line = line.replace(u'MBC_line16_Lib.py', u'MBC_line16_Lib_v361.py')
        
    # 4.0.0 replacements
    elif 'For%20firmware%204.0.0' in line:
        line = line.replace(u'line8_block_native.py', u'line8_block_native_v400.py')
        line = line.replace(u'line8_block_with_lib.py', u'line8_block_with_lib_v400.py')
        line = line.replace(u'line8_python_native.py', u'line8_python_native_v400.py')
        line = line.replace(u'line8_python_with_lib.py', u'line8_python_with_lib_v400.py')
        line = line.replace(u'MBC_line8_obj_Lib.py', u'MBC_line8_obj_Lib_v400.py')
        
        line = line.replace(u'line16_block_native.py', u'line16_block_native_v400.py')
        line = line.replace(u'line16_block_with_lib.py', u'line16_block_with_lib_v400.py')
        line = line.replace(u'line16_python_native.py', u'line16_python_native_v400.py')
        line = line.replace(u'line16_python_with_lib.py', u'line16_python_with_lib_v400.py')
        line = line.replace(u'MBC_line16_obj_Lib.py', u'MBC_line16_obj_Lib_v400.py')
        
    return line

for f in files_to_update:
    path = os.path.join(repo, f)
    if os.path.exists(path):
        with io.open(path, "r", encoding="utf-8") as file:
            lines = file.readlines()
        
        new_lines = []
        for line in lines:
            new_lines.append(update_line(line))
            
        content = u"".join(new_lines)
        
        # Text replacements (Line 8)
        content = content.replace(u'MBC_line8_Lib.py</code> 或 <code style="background: rgba(255,255,255,0.1); padding: 1px 4px; border-radius: 3px;">MBC_line8_obj_Lib.py</code>', 
                                  u'MBC_line8_Lib_v361.py</code> 或 <code style="background: rgba(255,255,255,0.1); padding: 1px 4px; border-radius: 3px;">MBC_line8_obj_Lib_v400.py</code>')
        
        content = content.replace(u'MBC_line8_Lib.py</code> or <code style="background: rgba(255,255,255,0.1); padding: 1px 4px; border-radius: 3px;">MBC_line8_obj_Lib.py</code>', 
                                  u'MBC_line8_Lib_v361.py</code> or <code style="background: rgba(255,255,255,0.1); padding: 1px 4px; border-radius: 3px;">MBC_line8_obj_Lib_v400.py</code>')

        # Text replacements (Line 16)
        content = content.replace(u'MBC_line16_Lib.py</code> 或 <code style="background: rgba(255,255,255,0.1); padding: 1px 4px; border-radius: 3px;">MBC_line16_obj_Lib.py</code>', 
                                  u'MBC_line16_Lib_v361.py</code> 或 <code style="background: rgba(255,255,255,0.1); padding: 1px 4px; border-radius: 3px;">MBC_line16_obj_Lib_v400.py</code>')
        
        content = content.replace(u'MBC_line16_Lib.py</code> or <code style="background: rgba(255,255,255,0.1); padding: 1px 4px; border-radius: 3px;">MBC_line16_obj_Lib.py</code>', 
                                  u'MBC_line16_Lib_v361.py</code> or <code style="background: rgba(255,255,255,0.1); padding: 1px 4px; border-radius: 3px;">MBC_line16_obj_Lib_v400.py</code>')

        if content != u"".join(lines):
            with io.open(path, "w", encoding="utf-8") as file:
                file.write(content)
            print("Updated " + f)
        else:
            print("No changes needed for " + f)
