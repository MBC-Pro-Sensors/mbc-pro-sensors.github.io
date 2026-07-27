# -*- coding: utf-8 -*-
import os
import re
import io

def fix_html_download_links(path):
    for root, dirs, files in os.walk(path):
        for file in files:
            if file.endswith('.md'):
                filepath = os.path.join(root, file)
                try:
                    with io.open(filepath, 'r', encoding='utf-8') as f:
                        content = f.read()

                    # Find all matches
                    # Only replace if the class string doesn't already have data-ignore next to it
                    # It's easier to just do a naive replace of `class="btn-download"` with `data-ignore="true" class="btn-download"`
                    # But make sure we don't duplicate it.
                    
                    changed = False
                    if u'data-ignore="true" class="btn-download"' not in content and u'class="btn-download"' in content:
                        content = content.replace(u'class="btn-download"', u'data-ignore="true" class="btn-download"')
                        changed = True
                    if u'data-ignore="true" class="btn-download-lib"' not in content and u'class="btn-download-lib"' in content:
                        content = content.replace(u'class="btn-download-lib"', u'data-ignore="true" class="btn-download-lib"')
                        changed = True

                    if changed:
                        with io.open(filepath, 'w', encoding='utf-8') as f:
                            f.write(content)
                        print("Fixed HTML links in {}".format(filepath))
                except Exception as e:
                    print("Error processing {}: {}".format(filepath, e))

if __name__ == "__main__":
    fix_html_download_links("docs")
