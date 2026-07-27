# -*- coding: utf-8 -*-
import io, os, re

repo = r"d:\Gemini_Antigravity\mbc-pro-sensors.github.io"
files_to_update = [
    r"sensors\line8\spike-pybricks.md",
    r"en\sensors\line8\spike-pybricks.md",
    r"sensors\line16\spike-pybricks.md",
    r"en\sensors\line16\spike-pybricks.md"
]

for f in files_to_update:
    path = os.path.join(repo, f)
    if os.path.exists(path):
        with io.open(path, "r", encoding="utf-8") as file:
            content = file.read()
        
        orig_content = content
        
        # Line 8 3.6.1
        content = content.replace(u'For%20firmware%203.6.1/line8_block_native.py', u'For%20firmware%203.6.1/line8_block_native_v361.py')
        content = content.replace(u'download="line8_block_native.py"', u'download="line8_block_native_v361.py"')
        
        content = content.replace(u'For%20firmware%203.6.1/line8_block_with_lib.py', u'For%20firmware%203.6.1/line8_block_with_lib_v361.py')
        content = content.replace(u'download="line8_block_with_lib.py"', u'download="line8_block_with_lib_v361.py"')
        
        content = content.replace(u'For%20firmware%203.6.1/line8_python_native.py', u'For%20firmware%203.6.1/line8_python_native_v361.py')
        content = content.replace(u'download="line8_python_native.py"', u'download="line8_python_native_v361.py"')
        
        content = content.replace(u'For%20firmware%203.6.1/line8_python_with_lib.py', u'For%20firmware%203.6.1/line8_python_with_lib_v361.py')
        content = content.replace(u'download="line8_python_with_lib.py"', u'download="line8_python_with_lib_v361.py"')
        
        content = content.replace(u'For%20firmware%203.6.1/MBC_line8_Lib.py', u'For%20firmware%203.6.1/MBC_line8_Lib_v361.py')
        content = content.replace(u'download="MBC_line8_Lib.py"', u'download="MBC_line8_Lib_v361.py"')

        # Line 8 4.0.0
        content = content.replace(u'For%20firmware%204.0.0/line8_block_native.py', u'For%20firmware%204.0.0/line8_block_native_v400.py')
        content = content.replace(u'download="line8_block_native.py"', u'download="line8_block_native_v400.py"')
        
        content = content.replace(u'For%20firmware%204.0.0/line8_block_with_lib.py', u'For%20firmware%204.0.0/line8_block_with_lib_v400.py')
        content = content.replace(u'download="line8_block_with_lib.py"', u'download="line8_block_with_lib_v400.py"')
        
        content = content.replace(u'For%20firmware%204.0.0/line8_python_native.py', u'For%20firmware%204.0.0/line8_python_native_v400.py')
        content = content.replace(u'download="line8_python_native.py"', u'download="line8_python_native_v400.py"')
        
        content = content.replace(u'For%20firmware%204.0.0/line8_python_with_lib.py', u'For%20firmware%204.0.0/line8_python_with_lib_v400.py')
        content = content.replace(u'download="line8_python_with_lib.py"', u'download="line8_python_with_lib_v400.py"')
        
        content = content.replace(u'For%20firmware%204.0.0/MBC_line8_obj_Lib.py', u'For%20firmware%204.0.0/MBC_line8_obj_Lib_v400.py')
        content = content.replace(u'download="MBC_line8_obj_Lib.py"', u'download="MBC_line8_obj_Lib_v400.py"')


        # Line 16 3.6.1
        content = content.replace(u'For%20firmware%203.6.1/line16_block_native.py', u'For%20firmware%203.6.1/line16_block_native_v361.py')
        content = content.replace(u'download="line16_block_native.py"', u'download="line16_block_native_v361.py"')
        
        content = content.replace(u'For%20firmware%203.6.1/line16_block_with_lib.py', u'For%20firmware%203.6.1/line16_block_with_lib_v361.py')
        content = content.replace(u'download="line16_block_with_lib.py"', u'download="line16_block_with_lib_v361.py"')
        
        content = content.replace(u'For%20firmware%203.6.1/line16_python_native.py', u'For%20firmware%203.6.1/line16_python_native_v361.py')
        content = content.replace(u'download="line16_python_native.py"', u'download="line16_python_native_v361.py"')
        
        content = content.replace(u'For%20firmware%203.6.1/line16_python_with_lib.py', u'For%20firmware%203.6.1/line16_python_with_lib_v361.py')
        content = content.replace(u'download="line16_python_with_lib.py"', u'download="line16_python_with_lib_v361.py"')
        
        content = content.replace(u'For%20firmware%203.6.1/MBC_line16_Lib.py', u'For%20firmware%203.6.1/MBC_line16_Lib_v361.py')
        content = content.replace(u'download="MBC_line16_Lib.py"', u'download="MBC_line16_Lib_v361.py"')

        # Line 16 4.0.0
        content = content.replace(u'For%20firmware%204.0.0/line16_block_native.py', u'For%20firmware%204.0.0/line16_block_native_v400.py')
        content = content.replace(u'download="line16_block_native.py"', u'download="line16_block_native_v400.py"')
        
        content = content.replace(u'For%20firmware%204.0.0/line16_block_with_lib.py', u'For%20firmware%204.0.0/line16_block_with_lib_v400.py')
        content = content.replace(u'download="line16_block_with_lib.py"', u'download="line16_block_with_lib_v400.py"')
        
        content = content.replace(u'For%20firmware%204.0.0/line16_python_native.py', u'For%20firmware%204.0.0/line16_python_native_v400.py')
        content = content.replace(u'download="line16_python_native.py"', u'download="line16_python_native_v400.py"')
        
        content = content.replace(u'For%20firmware%204.0.0/line16_python_with_lib.py', u'For%20firmware%204.0.0/line16_python_with_lib_v400.py')
        content = content.replace(u'download="line16_python_with_lib.py"', u'download="line16_python_with_lib_v400.py"')
        
        content = content.replace(u'For%20firmware%204.0.0/MBC_line16_obj_Lib.py', u'For%20firmware%204.0.0/MBC_line16_obj_Lib_v400.py')
        content = content.replace(u'download="MBC_line16_obj_Lib.py"', u'download="MBC_line16_obj_Lib_v400.py"')


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
        
        # Note: if there are any `download="..."` attributes that match the old names WITHOUT replacing the link, they will be handled properly.
        # Wait, my replacement for `download="line8_block_native.py"` comes AFTER the `href` replacement, but wait...
        # Wait, if I replace `download="line8_block_native.py"` first, it doesn't matter, it's global!
        # But wait! I only replaced `download="..."` globally, so BOTH 3.6.1 and 4.0.0 will be replaced.
        # Ah! `download="line8_block_native.py"` appears MULTIPLE times for BOTH versions!
        # If I replace `download="line8_block_native.py"` with `..._v361.py` first, then for 4.0.0 it won't match anymore!
        # This is a bug!
        
        with io.open(r"d:\Gemini_Antigravity\mbc-pro-sensors.github.io\scratch\temp_updated_content.md", "w", encoding="utf-8") as f:
            f.write(content)
            
        if content != orig_content:
            with io.open(path, "w", encoding="utf-8") as file:
                file.write(content)
            print("Updated " + f)
        else:
            print("No changes needed for " + f)
