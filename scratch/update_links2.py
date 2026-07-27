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
        ('download="MBC_line8_Lib.py" data-ignore="true" class="btn-download-lib" style="flex: 1; text-align: center; padding: 10px 15px; background: linear-gradient(135deg, #0abab5, #00d2ff); color: #fff; text-decoration: none; border-radius: 8px; font-weight: bold; font-size: 0.95rem; box-shadow: 0 6px 20px rgba(10,186,181,0.3); transition: transform 0.2s ease, box-shadow 0.2s ease;">📥 4.0.0 版庫函數</a>',
         'download="MBC_line8_obj_Lib.py" data-ignore="true" class="btn-download-lib" style="flex: 1; text-align: center; padding: 10px 15px; background: linear-gradient(135deg, #0abab5, #00d2ff); color: #fff; text-decoration: none; border-radius: 8px; font-weight: bold; font-size: 0.95rem; box-shadow: 0 6px 20px rgba(10,186,181,0.3); transition: transform 0.2s ease, box-shadow 0.2s ease;">📥 4.0.0 版庫函數</a>')
    ],
    r"en\sensors\line8\spike-pybricks.md": [
        ('download="MBC_line8_Lib.py" data-ignore="true" class="btn-download-lib" style="flex: 1; text-align: center; padding: 10px 15px; background: linear-gradient(135deg, #0abab5, #00d2ff); color: #fff; text-decoration: none; border-radius: 8px; font-weight: bold; font-size: 0.95rem; box-shadow: 0 6px 20px rgba(10,186,181,0.3); transition: transform 0.2s ease, box-shadow 0.2s ease;">📥 v4.0.0 Library</a>',
         'download="MBC_line8_obj_Lib.py" data-ignore="true" class="btn-download-lib" style="flex: 1; text-align: center; padding: 10px 15px; background: linear-gradient(135deg, #0abab5, #00d2ff); color: #fff; text-decoration: none; border-radius: 8px; font-weight: bold; font-size: 0.95rem; box-shadow: 0 6px 20px rgba(10,186,181,0.3); transition: transform 0.2s ease, box-shadow 0.2s ease;">📥 v4.0.0 Library</a>')
    ],
    r"sensors\line16\spike-pybricks.md": [
        ('download="MBC_line16_Lib.py" data-ignore="true" class="btn-download-lib" style="flex: 1; text-align: center; padding: 10px 15px; background: linear-gradient(135deg, #0abab5, #00d2ff); color: #fff; text-decoration: none; border-radius: 8px; font-weight: bold; font-size: 0.95rem; box-shadow: 0 6px 20px rgba(10,186,181,0.3); transition: transform 0.2s ease, box-shadow 0.2s ease;">📥 4.0.0 版庫函數</a>',
         'download="MBC_line16_obj_Lib.py" data-ignore="true" class="btn-download-lib" style="flex: 1; text-align: center; padding: 10px 15px; background: linear-gradient(135deg, #0abab5, #00d2ff); color: #fff; text-decoration: none; border-radius: 8px; font-weight: bold; font-size: 0.95rem; box-shadow: 0 6px 20px rgba(10,186,181,0.3); transition: transform 0.2s ease, box-shadow 0.2s ease;">📥 4.0.0 版庫函數</a>')
    ],
    r"en\sensors\line16\spike-pybricks.md": [
        ('download="MBC_line16_Lib.py" data-ignore="true" class="btn-download-lib" style="flex: 1; text-align: center; padding: 10px 15px; background: linear-gradient(135deg, #0abab5, #00d2ff); color: #fff; text-decoration: none; border-radius: 8px; font-weight: bold; font-size: 0.95rem; box-shadow: 0 6px 20px rgba(10,186,181,0.3); transition: transform 0.2s ease, box-shadow 0.2s ease;">📥 v4.0.0 Library</a>',
         'download="MBC_line16_obj_Lib.py" data-ignore="true" class="btn-download-lib" style="flex: 1; text-align: center; padding: 10px 15px; background: linear-gradient(135deg, #0abab5, #00d2ff); color: #fff; text-decoration: none; border-radius: 8px; font-weight: bold; font-size: 0.95rem; box-shadow: 0 6px 20px rgba(10,186,181,0.3); transition: transform 0.2s ease, box-shadow 0.2s ease;">📥 v4.0.0 Library</a>')
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
