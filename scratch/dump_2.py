import io
c = io.open(r"d:\Gemini_Antigravity\mbc-pro-sensors.github.io\sensors\line8\arduino-i2c.md", "r", encoding="utf-8").read()
idx1 = c.find(u"LegoLauXiao")
idx2 = c.find(u"LegoLauXiao", idx1+1)
with io.open(r"d:\Gemini_Antigravity\mbc-pro-sensors.github.io\scratch\dump_2.txt", "w", encoding="utf-8") as f:
    f.write(c[idx2-200:idx2+200])
