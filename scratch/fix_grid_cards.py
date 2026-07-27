import os
import io

docs_dir = r"docs"

def process_file(filepath):
    with io.open(filepath, "r", encoding="utf-8") as f:
        content = f.read()

    original_content = content
    
    # 1. Product Cards (like in README.md)
    # The inline style contains:
    # "background:#0a111a; border:1px solid rgba(X); border-radius:10px; padding:20px; display:flex; align-items:center; justify-content:space-between; gap:20px; overflow:hidden;"
    # We want to replace the "display:flex; align-items:center; justify-content:space-between; gap:20px; overflow:hidden;" part with the class
    # Actually, we can just search for that string and remove it, and prepend class="product-card" to the div.
    
    target_style = 'display:flex; align-items:center; justify-content:space-between; gap:20px; overflow:hidden;'
    target_style_with_spaces = 'display: flex; align-items: center; justify-content: space-between; gap: 20px; overflow: hidden;'
    
    # Simple replacement:
    if target_style in content or target_style_with_spaces in content:
        content = content.replace(target_style, '')
        content = content.replace(target_style_with_spaces, '')
        # Now we need to add the class. It's safer to just replace `<div style="background:` with `<div class="product-card" style="background:`
        content = content.replace('<div style="background:#0a111a;', '<div class="product-card" style="background:#0a111a;')

    # 2. Responsive Grids (in Pybricks/EV3 pages)
    # Target: style="display: grid; grid-template-columns: repeat(2, 1fr); gap: 12px;"
    # Or gap: 20px
    target_grid_12 = '<div style="display: grid; grid-template-columns: repeat(2, 1fr); gap: 12px;">'
    target_grid_20 = '<div style="display: grid; grid-template-columns: repeat(2, 1fr); gap: 20px; margin: 25px 0;">'
    
    content = content.replace(target_grid_12, '<div class="responsive-grid-2">')
    content = content.replace(target_grid_20, '<div class="responsive-grid-2" style="gap: 20px; margin: 25px 0;">')
    
    if content != original_content:
        with io.open(filepath, "w", encoding="utf-8") as f:
            f.write(content)
        print("Updated:", filepath)

for root, _, files in os.walk(docs_dir):
    for filename in files:
        if filename.endswith(".md"):
            process_file(os.path.join(root, filename))

print("Done scanning and updating.")
