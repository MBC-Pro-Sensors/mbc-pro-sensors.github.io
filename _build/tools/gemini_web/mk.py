import json, sys, os
ROOT = r'D:\AI_Agent_AI助理\02_MBC產品開發\95_產品網站\mbc-pro-sensors.github.io'
sys.path.insert(0, os.path.join(ROOT, '_build', 'tools'))
from gemini_image import build_prompt
OUT = os.path.dirname(os.path.abspath(__file__))
TPL = open(os.path.join(OUT, 'tpl.js'), encoding='utf-8').read()
for x in json.load(open(os.path.join(ROOT, '_build/figures/illust_prompts.json'), encoding='utf-8')):
    name = os.path.splitext(os.path.basename(x['out']))[0]
    js = TPL.replace('__PROMPT__', json.dumps(build_prompt(x['prompt']))).replace('__RATIO__', json.dumps(x['ratio']))
    open(os.path.join(OUT, f'job_{name}.js'), 'w', encoding='utf-8').write(js)
print('ok')
