import io, os

filepath = r"d:\Gemini_Antigravity\mbc-pro-sensors.github.io\sensors\line8\arduino-i2c.md"
with io.open(filepath, 'r', encoding='utf-8') as f:
    c = f.read()

idx = c.find(u'WgacdWLatbk')
print("idx:", idx)
start_tag = u'<div style="display: flex; flex-wrap: wrap; gap: 20px;'
start_idx = c.rfind(start_tag, 0, idx)
print("start_idx:", start_idx)

vid2_idx = c.find(u'T9bcndBNQvQ', idx)
print("vid2_idx:", vid2_idx)

end_idx = c.find(u'</div>', vid2_idx)
end_idx = c.find(u'</div>', end_idx + 1)
end_idx = c.find(u'</div>', end_idx + 1)
end_idx += len(u'</div>')

print("end_idx:", end_idx)

line_start = c.rfind(u'\n', 0, start_idx)
if line_start == -1: line_start = 0
else: line_start += 1

block = c[line_start:end_idx]
print("BLOCK LENGTH:", len(block))
lines = block.split(u'\n')
new_block = u'\n'.join([line.lstrip() for line in lines])

c = c[:line_start] + new_block + c[end_idx:]

with io.open(filepath + ".test", 'w', encoding='utf-8') as f:
    f.write(c)

print("done")
