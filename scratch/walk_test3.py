import os

count = 0
for root, dirs, files in os.walk(r"d:\Gemini_Antigravity\mbc-pro-sensors.github.io"):
    for file in files:
        if file.endswith(".md"):
            count += 1
            print(os.path.join(root, file))
print("Total MD files found:", count)
