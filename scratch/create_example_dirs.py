import os

dirs = [
    "docs/examples/line8/ev3",
    "docs/examples/line8/spike",
    "docs/examples/line8/pybricks",
    "docs/examples/line16/ev3",
    "docs/examples/line16/spike",
    "docs/examples/line16/pybricks"
]

for d in dirs:
    try:
        os.makedirs(d)
    except OSError:
        pass
    with open(os.path.join(d, ".gitkeep"), "w") as f:
        f.write("")

print("Created example directories and .gitkeep files.")
