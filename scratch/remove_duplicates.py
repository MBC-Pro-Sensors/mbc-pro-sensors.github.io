# -*- coding: utf-8 -*-
import io, os, re

count = 0
for root, dirs, files in os.walk(r"d:\Gemini_Antigravity\mbc-pro-sensors.github.io"):
    if '.git' in dirs: dirs.remove('.git')
    for file in files:
        if file.endswith(".md"):
            filepath = os.path.join(root, file)
            with io.open(filepath, 'r', encoding='utf-8') as f:
                c = f.read()
            
            orig_c = c
            
            pattern = re.compile(u'<br>\\s*!!! success.*?WgacdWLatbk.*?T9bcndBNQvQ.*?</div>\\s*</div>\\s*</div>\\s*', re.DOTALL)
            matches = list(pattern.finditer(c))
            
            if len(matches) > 1:
                print("Fixing {} ({} matches)".format(filepath, len(matches)))
                # Remove all but the last match
                # We do this by replacing from the end to not mess up indices
                # Actually, simpler: replace all matches with a unique placeholder, then replace the last placeholder with the original text (or just the cleaned block), and other placeholders with empty string.
                
                # Let's just construct a new string
                new_c = c[:matches[0].start()]
                for i in range(len(matches) - 1):
                    # add text between this match and next
                    new_c += c[matches[i].end():matches[i+1].start()]
                # Add the last match and the rest of the string
                new_c += c[matches[-1].start():]
                
                with io.open(filepath, 'w', encoding='utf-8') as f:
                    f.write(new_c)
                count += 1

print("Updated {} files".format(count))
