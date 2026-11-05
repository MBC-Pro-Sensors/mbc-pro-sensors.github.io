<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# Installing the EXP6 library: SPIKE Prime (LEGO Education SPIKE App 3)

For teachers, students and first-time users: follow these steps in order to control the EXP6 with Python in the **LEGO Education SPIKE App 3**. **You install the library only once**; every Python project can then use it. Install it again after a hub firmware update.

> Using **LEGO MINDSTORMS Robot Inventor (51515)**? See the [Robot Inventor install guide](/en/sensors/exp6/install-robot-inventor.md).

## What you need

- A SPIKE Prime hub (battery charged)
- A computer or tablet with the LEGO Education SPIKE App 3
- USB cable or Bluetooth, either works
- EXP6 firmware dated 2026-11-03 or later
- [MBC_EXP6_Official_App_Lib.zip](/downloads/MBC_EXP6_Official_App_Lib.zip) from this site, unzipped

## Install steps

1. Turn the hub on and connect it in the SPIKE App by USB or Bluetooth.
2. Make a new project and choose **Python**.
3. Open the unzipped `SPIKE_Education\Install_MBC_exp6_Lib.py` in a text editor, **select all, copy**, and paste it into the App's Python editor (delete what was there).
4. Press Run (▶) and open the console at the bottom of the App. When it says "**安裝成功！(Installation Successful!)**", you are done. You can delete this install project; the library stays on the hub.

> [!TIP]
> Prefer not to copy and paste? Download [install_exp6_lib.llsp3](/examples/exp6/spike_beginner/official/install_exp6_lib.llsp3), open it in the SPIKE App with File → Open and press ▶ once. It contains the same program as `Install_MBC_exp6_Lib.py`.

## Start programming

1. Connect the EXP6 battery and plug the EXP6 Host cable into a hub port (for example port F).
2. Make a new Python project and start it like this:

```python
from hub import port
import time
from MBC_exp6_SPIKE_App_Lib import MBC_EXP6

exp = MBC_EXP6(port.F)          # EXP6 on hub port F
exp.motor_run(1, 50)            # motor on EXP6 port 1 at 50% speed
time.sleep(2)
exp.stop_all(1)                 # stop all motors at the end (on Education they do not stop by themselves)
```

3. Press Run. You can also open the 10 examples in the `examples` folder of the download pack (EXP6 in port F).

👉 Try each function in your browser first: [LEGO SPIKE App interactive tutorial](/en/learn/exp6/spike-app.html); everything is explained in the [official App guide](/en/sensors/exp6/spike/official.md).

## Remove

To remove it: make a new Python project, paste `SPIKE_Education\Uninstall_MBC_exp6_Lib.py` from the download pack and run it until you see "Uninstall Successful!".

## Troubleshooting

| What you see | What to do |
| :--- | :--- |
| `ImportError: no module named …` | Not installed, or a firmware update wiped it → install again |
| "讀不到擴充板資料" (cannot read the expander) | Check the EXP6 battery, and that the port in `MBC_EXP6(port.X)` is the one it is plugged into |
| The motors keep running after the program ends | Education has no watchdog → end your program with `exp.stop_all(1)` |
| The official App cannot connect to EXP6 | EXP6 has run a Pybricks program or been on a Robot Inventor hub → take the EXP6 battery out and put it back |
| `MemoryError` | The program is too big → split it, restart the hub and try again |

Still stuck? [Contact us](/en/contact.md) with a screenshot of the console messages.
