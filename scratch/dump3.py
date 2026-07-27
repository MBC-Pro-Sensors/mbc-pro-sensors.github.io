import io
with io.open(r"d:\Gemini_Antigravity\mbc-pro-sensors.github.io\sensors\line8\arduino-i2c.md", "r", encoding="utf-8") as f:
    c = f.read()
idx = c.find(u"success")
with io.open(r"d:\Gemini_Antigravity\mbc-pro-sensors.github.io\scratch\dump3.txt", "w", encoding="utf-8") as out:
    out.write(c[idx-50:idx+2000])
