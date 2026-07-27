import io
c = io.open(r"d:\Gemini_Antigravity\mbc-pro-sensors.github.io\sensors\line8\spike-pybricks.md", "r", encoding="utf-8").read()
idx = c.find(u"WgacdWLatbk")
with io.open(r"d:\Gemini_Antigravity\mbc-pro-sensors.github.io\scratch\dump4.txt", "w", encoding="utf-8") as f:
    f.write(c[idx-500:idx+200])
