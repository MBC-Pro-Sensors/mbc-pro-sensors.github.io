# -*- coding: utf-8 -*-
import os
import re

directories = [
    r"d:\Gemini_Antigravity\mbc-pro-sensors.github.io\docs\sensors\line8",
    r"d:\Gemini_Antigravity\mbc-pro-sensors.github.io\docs\sensors\line16",
    r"d:\Gemini_Antigravity\mbc-pro-sensors.github.io\docs\en\sensors\line8",
    r"d:\Gemini_Antigravity\mbc-pro-sensors.github.io\docs\en\sensors\line16"
]

skip_files = {"index.md", "ev3-hub.md", "spike-hub.md"}

block_zh = """<br>

!!! success "🚀 想要學更厲害的循線控制方法嗎？"
    可以找這幾位厲害的教練上課唷！他們有非常豐富的比賽與教學經驗，保證讓你收穫滿滿～ 💯
    
    <div style="display: flex; flex-wrap: wrap; gap: 20px; justify-content: center; margin-top: 15px;">
      <div style="flex: 1; min-width: 250px; max-width: 320px; display: flex; flex-direction: column; align-items: center;">
        <h4 style="margin: 0 0 10px 0; text-align: center;">🏆 <a href="https://www.youtube.com/@LegoLauXiao" target="_blank" style="color: inherit; text-decoration: none;">LegoLauXiao 教練</a></h4>
        <div style="width: 100%; aspect-ratio: 9/16; background: #000; border-radius: 12px; overflow: hidden; border: 1px solid rgba(0,210,255,0.2); box-shadow: 0 10px 25px rgba(0,0,0,0.3);">
          <iframe src="https://www.youtube.com/embed/WgacdWLatbk" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" allowfullscreen loading="lazy" style="width: 100%; height: 100%; border: none;"></iframe>
        </div>
      </div>
      <div style="flex: 1; min-width: 250px; max-width: 320px; display: flex; flex-direction: column; align-items: center;">
        <h4 style="margin: 0 0 10px 0; text-align: center;">🏆 <a href="https://www.youtube.com/@legolaumo" target="_blank" style="color: inherit; text-decoration: none;">legolaumo 教練</a></h4>
        <div style="width: 100%; aspect-ratio: 9/16; background: #000; border-radius: 12px; overflow: hidden; border: 1px solid rgba(0,210,255,0.2); box-shadow: 0 10px 25px rgba(0,0,0,0.3);">
          <iframe src="https://www.youtube.com/embed/T9bcndBNQvQ" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" allowfullscreen loading="lazy" style="width: 100%; height: 100%; border: none;"></iframe>
        </div>
      </div>
    </div>
"""

block_en = """<br>

!!! success "🚀 Want to learn more advanced line-following control methods?"
    You can take classes from these awesome coaches! They have rich experience in competitions and teaching, guaranteeing you'll learn a lot! 💯
    
    <div style="display: flex; flex-wrap: wrap; gap: 20px; justify-content: center; margin-top: 15px;">
      <div style="flex: 1; min-width: 250px; max-width: 320px; display: flex; flex-direction: column; align-items: center;">
        <h4 style="margin: 0 0 10px 0; text-align: center;">🏆 <a href="https://www.youtube.com/@LegoLauXiao" target="_blank" style="color: inherit; text-decoration: none;">Coach LegoLauXiao</a></h4>
        <div style="width: 100%; aspect-ratio: 9/16; background: #000; border-radius: 12px; overflow: hidden; border: 1px solid rgba(0,210,255,0.2); box-shadow: 0 10px 25px rgba(0,0,0,0.3);">
          <iframe src="https://www.youtube.com/embed/WgacdWLatbk" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" allowfullscreen loading="lazy" style="width: 100%; height: 100%; border: none;"></iframe>
        </div>
      </div>
      <div style="flex: 1; min-width: 250px; max-width: 320px; display: flex; flex-direction: column; align-items: center;">
        <h4 style="margin: 0 0 10px 0; text-align: center;">🏆 <a href="https://www.youtube.com/@legolaumo" target="_blank" style="color: inherit; text-decoration: none;">Coach legolaumo</a></h4>
        <div style="width: 100%; aspect-ratio: 9/16; background: #000; border-radius: 12px; overflow: hidden; border: 1px solid rgba(0,210,255,0.2); box-shadow: 0 10px 25px rgba(0,0,0,0.3);">
          <iframe src="https://www.youtube.com/embed/T9bcndBNQvQ" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" allowfullscreen loading="lazy" style="width: 100%; height: 100%; border: none;"></iframe>
        </div>
      </div>
    </div>
"""

for d in directories:
    if not os.path.exists(d):
        print("Skipping {}, directory does not exist.".format(d))
        continue
    
    is_en = "docs\\en\\" in d
    block_to_use = block_en if is_en else block_zh

    for filename in os.listdir(d):
        if not filename.endswith(".md"):
            continue
        if filename in skip_files:
            continue
        
        filepath = os.path.join(d, filename)
        with open(filepath, "r") as f: # Removed encoding="utf-8" for python2 compat, but we can read as binary if needed. Wait, in python2 open doesn't take encoding.
            content = f.read()
            # If python2, read() returns str (bytes). We should decode to unicode.
            try:
                content = content.decode('utf-8')
            except AttributeError:
                pass # Python 3 already strings

        # Remove existing promotion block if any
        # Regex to match from <br>\n\n!!! success "🚀 to the end of the block, stopping before <style>
        # We will use a more robust regex:
        content = re.sub(r'<br>\s*!!! success "\xf0\x9f\x9a\x80.*?</div>\s*</div>\s*</div>\s*', '', content, flags=re.DOTALL)
        content = re.sub(r'<br>\s*!!! success "\xf0\x9f\x9a\x80.*?(?=<style>|$)', '', content, flags=re.DOTALL)

        # Now we want to insert block_to_use right before the first <style> tag at the bottom of the file
        # Or at the end if there is no <style>
        
        try:
            block_to_use_u = block_to_use.decode('utf-8')
        except AttributeError:
            block_to_use_u = block_to_use

        if "<style>" in content:
            parts = content.rsplit("<style>", 1)
            new_content = parts[0].rstrip() + "\n\n" + block_to_use_u + "\n<style>" + parts[1]
        else:
            new_content = content.rstrip() + "\n\n" + block_to_use_u + "\n"

        with open(filepath, "wb") as f:
            f.write(new_content.encode('utf-8'))
        print("Updated {}".format(filepath))
