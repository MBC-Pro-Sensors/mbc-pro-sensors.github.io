import io
c = io.open(r"d:\Gemini_Antigravity\mbc-pro-sensors.github.io\sensors\line8\spike-pybricks.md", "r", encoding="utf-8").read()
idx1 = c.find(u"success")
idx2 = c.find(u"success", idx1+1)
with io.open(r"d:\Gemini_Antigravity\mbc-pro-sensors.github.io\scratch\dump5.txt", "w", encoding="utf-8") as f:
    f.write(u"FIRST:\n" + c[idx1-50:idx1+200] + u"\n\nSECOND:\n" + c[idx2-50:idx2+200])
