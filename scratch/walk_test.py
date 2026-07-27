import os
count = 0
for root, dirs, files in os.walk(r"d:\Gemini_Antigravity\mbc-pro-sensors.github.io"):
    if 'scratch' in root or '.git' in root: continue
    for file in files:
        if file.endswith(".md"):
            count += 1
print("Total MD files found:", count)
