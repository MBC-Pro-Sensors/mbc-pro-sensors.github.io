# -*- coding: utf-8 -*-
"""Gemini 情境插圖產生器（REST v1beta generateContent；D7）。

用法：
  py -3 _build/tools/gemini_image.py --out images/illust/host-spike.png --prompt "SPIKE Prime robot on a WRO mat" [--dry-run]
  py -3 _build/tools/gemini_image.py --batch _build/illust.json [--dry-run]      # [{out, prompt, alt}]
  py -3 _build/tools/gemini_image.py --retry-pending                              # 重試 images/illust/_pending.json 裡失敗的圖
選項：
  --key-file   金鑰檔（預設 D:\\AI_Agent_AI助理\\02_MBC產品開發\\95_產品網站\\MBC粉專API\\.secrets\\gemini_api_key.txt）
  --model      先試的模型（預設 gemini-2.5-flash-image，失敗再 gemini-3-pro-image）
  --dry-run    不呼叫 API，只印出 prompt 與輸出路徑（本輪驗證用）
  --placeholder-only  不呼叫 API，直接產 SVG 佔位＋寫 _pending.json（測一張就 429 時用；之後 --retry-pending）
行為：429／503 依序退避 10／30／60 秒；全部失敗就寫 images/illust/_pending.json 並產同名 .svg 佔位，不擋上線。
產圖後請跑 webp_logo.py 轉 webp 並疊 logo。
"""
import argparse
import base64
import json
import os
import sys

if hasattr(sys.stdout, "reconfigure"):
    sys.stdout.reconfigure(encoding="utf-8")
import time
import urllib.error
import urllib.request

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..'))
DEFAULT_KEY = r'D:\AI_Agent_AI助理\02_MBC產品開發\95_產品網站\MBC粉專API\.secrets\gemini_api_key.txt'
MODELS = ['gemini-2.5-flash-image', 'gemini-3-pro-image']
PENDING = os.path.join(ROOT, 'images', 'illust', '_pending.json')
BACKOFF = [10, 30, 60]

# 品牌語氣（product-marketing.md：專業、白話、像教練）＋畫面規則
STYLE = (
    'Illustration for an educational robotics website. Tone: professional, plain and friendly, like a coach explaining to students. '
    'Subject: a LEGO-compatible robot sensor module made by a third party (black PCB, teal accents), used in a classroom or on a robotics competition mat. '
    'Light background, clean flat/vector style with soft shadows, bright but not garish colours. '
    'No text, no letters, no numbers, no logos, no LEGO trademark or minifigures, no brand names. '
)


def build_prompt(subject: str) -> str:
    return STYLE + 'Scene: ' + subject.strip()


def load_key(path: str) -> str:
    with open(path, 'r', encoding='utf-8') as f:
        return f.read().strip()


def call_gemini(model: str, key: str, prompt: str, ratio: str = '') -> bytes:
    url = f'https://generativelanguage.googleapis.com/v1beta/models/{model}:generateContent'
    gen_cfg = {'responseModalities': ['IMAGE']}
    if ratio:  # 長寬比（1:1、16:9、4:3…）；批次 JSON 的 ratio 欄位
        gen_cfg['imageConfig'] = {'aspectRatio': ratio}
    body = json.dumps({
        'contents': [{'parts': [{'text': prompt}]}],
        'generationConfig': gen_cfg,
    }).encode('utf-8')
    req = urllib.request.Request(url, data=body, method='POST', headers={'Content-Type': 'application/json', 'x-goog-api-key': key})
    with urllib.request.urlopen(req, timeout=120) as resp:
        data = json.loads(resp.read().decode('utf-8'))
    for cand in data.get('candidates', []):
        for part in cand.get('content', {}).get('parts', []):
            inline = part.get('inlineData') or part.get('inline_data')
            if inline and inline.get('data'):
                return base64.b64decode(inline['data'])
    raise RuntimeError('回應裡沒有圖片：' + json.dumps(data)[:300])


def generate(prompt: str, key: str, first_model: str, ratio: str = '') -> bytes:
    models = [first_model] + [m for m in MODELS if m != first_model]
    last = None
    for model in models:
        for attempt in range(len(BACKOFF) + 1):
            try:
                return call_gemini(model, key, prompt, ratio)
            except urllib.error.HTTPError as e:
                last = f'{model} HTTP {e.code}'
                if e.code in (429, 503) and attempt < len(BACKOFF):
                    print(f'  {last}，{BACKOFF[attempt]} 秒後重試', flush=True)
                    time.sleep(BACKOFF[attempt])
                    continue
                print(f'  {last}，換下一個模型', flush=True)
                break
            except Exception as e:  # noqa: BLE001
                last = f'{model}: {e}'
                print(f'  {last}', flush=True)
                break
    raise RuntimeError(last or '未知錯誤')


def placeholder_svg(path: str, alt: str, ratio: str = '') -> None:
    w, h = 1200, 675
    if ratio and ':' in ratio:  # 依長寬比出佔位（1:1 → 1200×1200）
        a, b = ratio.split(':')
        h = round(1200 * float(b) / float(a))
    svg = (
        f'<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 {w} {h}" width="{w}" height="{h}">'
        f'<rect width="{w}" height="{h}" fill="#f6f8fa"/>'
        f'<rect x="40" y="40" width="{w - 80}" height="{h - 80}" rx="24" fill="none" stroke="#cfd8de" stroke-width="4" stroke-dasharray="18 12"/>'
        f'<text x="{w // 2}" y="{h // 2 + 12}" font-family="Nunito, Arial, sans-serif" font-size="36" fill="#8a97a3" text-anchor="middle">{alt}</text>'
        '</svg>\n'
    )
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, 'w', encoding='utf-8') as f:
        f.write(svg)


def load_pending() -> list:
    if os.path.exists(PENDING):
        with open(PENDING, 'r', encoding='utf-8') as f:
            return json.load(f)
    return []


def save_pending(items: list) -> None:
    os.makedirs(os.path.dirname(PENDING), exist_ok=True)
    with open(PENDING, 'w', encoding='utf-8') as f:
        json.dump(items, f, ensure_ascii=False, indent=2)


def run_jobs(jobs: list, key: str, model: str, dry: bool, placeholder_only: bool = False) -> int:
    pending = [p for p in load_pending() if p.get('out') not in {j['out'] for j in jobs}]
    failed = 0
    for job in jobs:
        out = os.path.join(ROOT, job['out'])
        prompt = build_prompt(job['prompt'])
        alt = job.get('alt', os.path.splitext(os.path.basename(job['out']))[0])
        print(f'→ {job["out"]}')
        if dry:
            print('  [dry-run] prompt: ' + prompt)
            continue
        if placeholder_only:  # 額度已知用完：不呼叫 API，直接佔位＋記 pending
            failed += 1
            print('  [placeholder-only] 產 SVG 佔位並寫入 _pending.json')
            placeholder_svg(os.path.splitext(out)[0] + '.svg', alt, job.get('ratio', ''))
            pending.append({'out': job['out'], 'prompt': job['prompt'], 'alt': alt, 'ratio': job.get('ratio', ''), 'error': 'skipped: quota exhausted (429) on first test image', 'time': time.strftime('%Y-%m-%d %H:%M:%S')})
            continue
        try:
            png = generate(prompt, key, model, job.get('ratio', ''))
            os.makedirs(os.path.dirname(out), exist_ok=True)
            with open(out, 'wb') as f:
                f.write(png)
            print(f'  已存 {len(png)} bytes')
        except Exception as e:  # noqa: BLE001
            failed += 1
            print(f'  失敗：{e}；寫入 _pending.json 並產 SVG 佔位')
            placeholder_svg(os.path.splitext(out)[0] + '.svg', alt, job.get('ratio', ''))
            pending.append({'out': job['out'], 'prompt': job['prompt'], 'alt': alt, 'ratio': job.get('ratio', ''), 'error': str(e), 'time': time.strftime('%Y-%m-%d %H:%M:%S')})
    if not dry:
        save_pending(pending)
    return failed


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--out', help='輸出 png（相對 repo 根）')
    ap.add_argument('--prompt', help='場景描述（會自動加品牌語氣與畫面規則）')
    ap.add_argument('--alt', default=None)
    ap.add_argument('--batch', help='JSON 清單 [{out, prompt, alt}]')
    ap.add_argument('--retry-pending', action='store_true')
    ap.add_argument('--key-file', default=DEFAULT_KEY)
    ap.add_argument('--model', default=MODELS[0])
    ap.add_argument('--dry-run', action='store_true')
    ap.add_argument('--placeholder-only', action='store_true', help='不呼叫 API，只產 SVG 佔位並寫 _pending.json（額度用完時）')
    a = ap.parse_args()

    if a.retry_pending:
        jobs = load_pending()
        if not jobs:
            print('沒有待重試的圖')
            return 0
    elif a.batch:
        with open(a.batch, 'r', encoding='utf-8') as f:
            jobs = json.load(f)
    elif a.out and a.prompt:
        jobs = [{'out': a.out, 'prompt': a.prompt, 'alt': a.alt or ''}]
    else:
        ap.error('要給 --out 與 --prompt，或 --batch，或 --retry-pending')

    key = ''
    if not a.dry_run and not a.placeholder_only:
        if not os.path.exists(a.key_file):
            print(f'找不到金鑰檔 {a.key_file}', file=sys.stderr)
            return 2
        key = load_key(a.key_file)
    else:
        print(f'[dry-run] 金鑰檔：{a.key_file}（{"存在" if os.path.exists(a.key_file) else "不存在"}）；模型順序：{[a.model] + [m for m in MODELS if m != a.model]}')
    failed = run_jobs(jobs, key, a.model, a.dry_run, a.placeholder_only)
    print(f'完成：{len(jobs) - failed} 成功、{failed} 失敗')
    return 1 if failed else 0


if __name__ == '__main__':
    sys.exit(main())
