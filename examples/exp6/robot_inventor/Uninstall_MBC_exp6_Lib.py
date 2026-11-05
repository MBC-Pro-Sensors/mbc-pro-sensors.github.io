# MBC EXP6 Library Uninstaller（玩具版：LEGO MINDSTORMS Robot Inventor 10.x／SPIKE App 2.x）
# 在 App 執行此程式，把 /projects/mbc_exp6/ 裡的 EXP6 函式庫移除。
# Run this in the app to remove the EXP6 library from /projects/mbc_exp6/.
import os

DIR = "/projects/mbc_exp6"
print("=========================================")
try:
    names = os.listdir(DIR)
except OSError:
    names = None
if names is None:
    print("沒有安裝 EXP6 函式庫。 (Not installed.)")
else:
    for n in names:
        try:
            os.remove(DIR + "/" + n)
            print("移除 / removed", n)
        except Exception as e:
            print("移除失敗 / failed", n, e)
    try:
        os.rmdir(DIR)
    except Exception:
        pass
    print("移除完成！ (Uninstall Successful!)")
# 2026-11-05 以前的版本裝在 /lib
try:
    os.remove("/lib/MBC_exp6_SPIKE_App_Lib.py")
    print("也移除了舊版 /lib/MBC_exp6_SPIKE_App_Lib.py")
except OSError:
    pass
print("=========================================")
