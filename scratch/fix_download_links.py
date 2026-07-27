# -*- coding: utf-8 -*-
import os
import re
import io

def fix_download_links(path):
    # Regex to match markdown links starting with 📥
    # \xf0\x9f\x93\xa5 is the utf-8 encoding for 📥
    pattern = re.compile(r'\[(📥\s*[^\]]+)\]\(([^)]+)\)'.decode('utf-8'))

    for root, dirs, files in os.walk(path):
        for file in files:
            if file.endswith('.md'):
                filepath = os.path.join(root, file)
                try:
                    with io.open(filepath, 'r', encoding='utf-8') as f:
                        content = f.read()

                    # Find all matches
                    # We use u'<a href="\2" target="_blank" data-ignore="true" download>\1</a>'
                    new_content, count = pattern.subn(u'<a href="\\2" target="_blank" data-ignore="true" download>\\1</a>', content)

                    if count > 0:
                        with io.open(filepath, 'w', encoding='utf-8') as f:
                            f.write(new_content)
                        print("Fixed {} links in {}".format(count, filepath))
                except Exception as e:
                    print("Error processing {}: {}".format(filepath, e))

if __name__ == "__main__":
    fix_download_links("docs")
