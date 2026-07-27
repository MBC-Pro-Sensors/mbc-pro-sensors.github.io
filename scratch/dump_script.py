import io
with io.open(r"d:\Gemini_Antigravity\mbc-pro-sensors.github.io\sensors\line8\arduino-i2c.md", "r", encoding="utf-8") as f:
    c = f.read()

idx = c.find(u"WgacdWLatbk")
start = max(0, idx - 500)
end = min(len(c), idx + 500)

with io.open(r"d:\Gemini_Antigravity\mbc-pro-sensors.github.io\scratch\dump.txt", "w", encoding="utf-8") as f:
    f.write(c[start:end])
