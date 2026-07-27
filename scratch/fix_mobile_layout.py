import os
import io

files_to_update = [
    r"docs\sensors\line8\spike-pybricks.md",
    r"docs\sensors\line16\spike-pybricks.md",
    r"docs\en\sensors\line8\spike-pybricks.md",
    r"docs\en\sensors\line16\spike-pybricks.md",
]

css_to_add = u"""
  /* Responsive Dual Buttons */
  .download-btn-group {
    display: flex;
    gap: 8px;
    width: 100%;
  }
  @media (max-width: 767px) {
    .download-btn-group {
      flex-direction: column;
    }
    .download-card .download-btn-group a,
    .library-download-box .download-btn-group a {
      padding: 12px 8px !important;
      font-size: 0.9rem !important;
    }
  }
"""

for fpath in files_to_update:
    if not os.path.exists(fpath):
        print("File not found:", fpath)
        continue
    
    with io.open(fpath, "r", encoding="utf-8") as f:
        content = f.read()
    
    # Replace inline div
    content = content.replace(u'<div style="display: flex; gap: 8px;">', u'<div class="download-btn-group">')
    
    # Inject CSS
    if u".download-btn-group" not in content:
        content = content.replace(u"</style>", css_to_add + u"</style>")
        
    with io.open(fpath, "w", encoding="utf-8") as f:
        f.write(content)
        
    print("Updated:", fpath)
