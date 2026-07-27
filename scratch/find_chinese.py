import os
import io
import re

def contains_chinese(text):
    if re.search(u'[\u4e00-\u9fff]', text):
        return True
    return False

def scan_dir(path):
    with io.open("chinese_results.txt", "w", encoding="utf-8") as out:
        for root, dirs, files in os.walk(path):
            for file in files:
                if file.endswith('.md') or file.endswith('.html'):
                    filepath = os.path.join(root, file)
                    try:
                        with io.open(filepath, 'r', encoding='utf-8') as f:
                            lines = f.readlines()
                            for i, line in enumerate(lines):
                                if contains_chinese(line):
                                    out.write(u"Found Chinese in {}:{}, line: {}\n".format(filepath, i+1, line.strip()))
                                    break # Just report first occurrence per file
                    except Exception as e:
                        out.write(u"Error reading {}: {}\n".format(filepath, e))

if __name__ == "__main__":
    scan_dir("docs/en")
