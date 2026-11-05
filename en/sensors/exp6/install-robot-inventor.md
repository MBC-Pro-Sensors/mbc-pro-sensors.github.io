<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# Installing the EXP6 library: MINDSTORMS Robot Inventor (51515)

For teachers, students and first-time users: follow these steps in order and the **LEGO MINDSTORMS Robot Inventor (51515) hub** can control the EXP6 with Python. **You install the library only once**; every Python project can then use it. Install it again after a hub firmware update.

> Using **LEGO Education SPIKE Prime** (SPIKE App 3)? See the [Education install guide](/en/sensors/exp6/install-spike-app.md).

## What you need

- A Robot Inventor hub (battery charged)
- **A USB cable that carries data** (the one that came with the hub works; charge-only cables do not)
- A computer or tablet with the LEGO MINDSTORMS App (Robot Inventor App 10.x)
- EXP6 firmware dated 2026-11-03 or later
- [MBC_EXP6_Robot_Inventor_Lib.zip](/downloads/MBC_EXP6_Robot_Inventor_Lib.zip) from this site, unzipped

## Why a USB cable

The install sends a fairly large file to the hub, and over Bluetooth the hub may switch off halfway. **You only need the USB cable for this one install**; afterwards you can program over Bluetooth as usual.

## Install steps

1. Connect the hub to the computer with the USB cable and press the centre button to turn it on.
2. Open the LEGO MINDSTORMS App and connect to the hub (the App shows it is connected).
3. Make a new project and choose **Python**.
4. Open the unzipped `Install_MBC_exp6_Lib.py` in a text editor, **select all and copy**.
5. Delete what is in the App's Python editor and **paste** what you copied.
6. Press the App's Run button (▶).
7. Open the console at the bottom of the App. Within about 10 seconds you will see:

```text
MBC_exp6_SPIKE_App_Lib.py … B OK
MBC_exp6_mbc.py … B OK
MBC_exp6_cam.py … B OK
安裝成功！ (Installed OK)
```

   "Installed OK" means you are done. You can delete this install project; the library stays on the hub (`/projects/mbc_exp6/`, kept after power-off).
8. You can unplug the USB cable now and connect over Bluetooth from then on.

> [!TIP]
> If you see "Install failed, please run again": press Run once more. If it still fails, make sure you are connected by USB.

## Start programming

1. Connect the EXP6 battery and plug the EXP6 Host cable into a hub port (for example port F).
2. Make a new Python project and start it like this:

```python
from hub import port
import time
from projects.mbc_exp6.MBC_exp6_SPIKE_App_Lib import MBC_EXP6

exp = MBC_EXP6(port.F)          # EXP6 on hub port F
exp.motor_run(1, 50)            # motor on EXP6 port 1 at 50% speed
time.sleep(2)
exp.motor_stop(1, "brake")      # brake to a stop
```

3. Press Run. You can also open the 10 examples in the `examples` folder of the download pack (EXP6 in port F, motors in EXP6 ports 1 and 2; they print their messages in Chinese).

👉 Try each function in your browser first: [Robot Inventor App interactive tutorial](/en/learn/exp6/robot-inventor.html); everything is explained in the [Robot Inventor section of the official App guide](/en/sensors/exp6/spike/official.md).

## 3 things to know on Robot Inventor

1. **After running a program, the EXP6 only goes back to normal when you take out its battery**: until then the App's device screen does not show the EXP6, which is normal. Before moving it to another hub (for example an Education SPIKE Prime), take the EXP6 battery out and put it back.
2. **The motors stop by themselves within 10 seconds after the program ends**: to keep a motor turning longer than 10 seconds while doing nothing else, wait with `exp.keep_alive_wait(ms)`, for example `exp.keep_alive_wait(15000)` for 15 seconds.
3. **The import line differs from the Education version**: on Robot Inventor it is `from projects.mbc_exp6.MBC_exp6_SPIKE_App_Lib import …`; a wrong line gives an `ImportError`.

## Remove

To remove it: make a new Python project, paste `Uninstall_MBC_exp6_Lib.py` from the download pack and run it until you see "Uninstall Successful!".

## Troubleshooting

| What you see | What to do |
| :--- | :--- |
| `ImportError: no module named …` | Not installed, or a firmware update wiped it → install again; also check the import line has `projects.mbc_exp6.` |
| "讀不到擴充板資料" (cannot read the expander) | Check the EXP6 battery, and that the port in `MBC_EXP6(port.X)` is the one it is plugged into |
| The hub switches off during install | Connect by USB and install again |
| The motors stop after a while | Nothing called the library for 10 seconds → wait with `exp.keep_alive_wait(ms)` |
| The App's device screen does not show EXP6 | Take the EXP6 battery out and put it back |
| `MemoryError` | The program is too big → split it, restart the hub and try again |

Still stuck? [Contact us](/en/contact.md) with a screenshot of the console messages.
