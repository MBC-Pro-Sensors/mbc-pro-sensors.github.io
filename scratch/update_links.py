# -*- coding: utf-8 -*-
import io, os

repo = r"d:\Gemini_Antigravity\mbc-pro-sensors.github.io"
files_to_update = [
    r"sensors\line8\spike-pybricks.md",
    r"en\sensors\line8\spike-pybricks.md",
    r"sensors\line16\spike-pybricks.md",
    r"en\sensors\line16\spike-pybricks.md"
]

replacements = {
    r"sensors\line8\spike-pybricks.md": [
        ("4.0.0/MBC_line8_Lib.py", "4.0.0/MBC_line8_obj_Lib.py"),
        ('您<strong>必須將此庫檔案 (<code style="background: rgba(255,255,255,0.1); padding: 1px 4px; border-radius: 3px;">MBC_line8_Lib.py</code>) 與您的主程式放在同一個專案列表下</strong>',
         '您<strong>必須將下載的庫檔案（依韌體版本而定，例如 <code style="background: rgba(255,255,255,0.1); padding: 1px 4px; border-radius: 3px;">MBC_line8_Lib.py</code> 或 <code style="background: rgba(255,255,255,0.1); padding: 1px 4px; border-radius: 3px;">MBC_line8_obj_Lib.py</code>）與您的主程式放在同一個專案列表下</strong>')
    ],
    r"en\sensors\line8\spike-pybricks.md": [
        ("4.0.0/MBC_line8_Lib.py", "4.0.0/MBC_line8_obj_Lib.py"),
        ('You <strong>MUST place this library file (<code style="background: rgba(255,255,255,0.1); padding: 1px 4px; border-radius: 3px;">MBC_line8_Lib.py</code>) in the same project directory as your main program</strong>',
         'You <strong>MUST place the downloaded library file (depending on your firmware version, e.g., <code style="background: rgba(255,255,255,0.1); padding: 1px 4px; border-radius: 3px;">MBC_line8_Lib.py</code> or <code style="background: rgba(255,255,255,0.1); padding: 1px 4px; border-radius: 3px;">MBC_line8_obj_Lib.py</code>) in the same project directory as your main program</strong>')
    ],
    r"sensors\line16\spike-pybricks.md": [
        ("4.0.0/MBC_line16_Lib.py", "4.0.0/MBC_line16_obj_Lib.py"),
        ('您<strong>必須將此庫檔案 (<code style="background: rgba(255,255,255,0.1); padding: 1px 4px; border-radius: 3px;">MBC_line16_Lib.py</code>) 與您的主程式放在同一個專案列表下</strong>',
         '您<strong>必須將下載的庫檔案（依韌體版本而定，例如 <code style="background: rgba(255,255,255,0.1); padding: 1px 4px; border-radius: 3px;">MBC_line16_Lib.py</code> 或 <code style="background: rgba(255,255,255,0.1); padding: 1px 4px; border-radius: 3px;">MBC_line16_obj_Lib.py</code>）與您的主程式放在同一個專案列表下</strong>')
    ],
    r"en\sensors\line16\spike-pybricks.md": [
        ("4.0.0/MBC_line16_Lib.py", "4.0.0/MBC_line16_obj_Lib.py"),
        ('You <strong>MUST place this library file (<code style="background: rgba(255,255,255,0.1); padding: 1px 4px; border-radius: 3px;">MBC_line16_Lib.py</code>) in the same project directory as your main program</strong>',
         'You <strong>MUST place the downloaded library file (depending on your firmware version, e.g., <code style="background: rgba(255,255,255,0.1); padding: 1px 4px; border-radius: 3px;">MBC_line16_Lib.py</code> or <code style="background: rgba(255,255,255,0.1); padding: 1px 4px; border-radius: 3px;">MBC_line16_obj_Lib.py</code>) in the same project directory as your main program</strong>')
    ]
}

for f in files_to_update:
    path = os.path.join(repo, f)
    if os.path.exists(path):
        with io.open(path, "r", encoding="utf-8") as file:
            content = file.read()
        
        orig_content = content
        for old, new in replacements[f]:
            content = content.replace(old.decode("utf-8"), new.decode("utf-8"))
        
        if content != orig_content:
            with io.open(path, "w", encoding="utf-8") as file:
                file.write(content)
            print("Updated " + f)
        else:
            print("No changes needed for " + f)
