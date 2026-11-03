### 左右怎麼分

**左右以感應器朝前、從背後看為準（左＝J5、右＝J3）。** 面對感應器看會剛好相反，所以用手測試時，請站到感應器後面、跟它同方向看。範例裡的「左邊比較近」「往左轉」都照這個方向。

### 上機結果（Pybricks 4.1.2）

SPIKE 版在 **Pybricks 4.1.2 實測通過（2026-11-02）**：讀值、函式庫與新手範例都在主機上跑過。

- 用 `ColorSensor` 直接讀時，2026-11 以後的感應器韌體在 Pybricks 4.x 的 `reflection()` 就是整數（例如 31）；較舊的韌體會帶 1 位小數（例如 30.8）。程式裡寫 `int(...)` 新舊韌體都通，留著就好。

<!-- tof2-cmd-examples -->

### 下命令改設定（E0～E4）

> [!TIP]
> **SPIKE Pybricks 實測通過（2026-11-02）**：E0～E4 每個命令在主機上逐條驗過：正確的參數回「完成」、超出範圍的回「失敗」且設定不變、遠距離模式讀得到 200 cm、斷電後回出廠。

用 MBC 函式庫 `MBC_laser2_obj_Lib.py` 的 `cmd()` 送命令、`wait_done()` 等它做完（True＝完成、False＝失敗或沒回應）。**官方 SPIKE App 和 Pybricks 內建 `ColorSensor` 積木送不了命令**；感應器韌體要 2026-11 以後的版本。

```python
from MBC_laser2_obj_Lib import MBC_LASER2

laser2 = MBC_LASER2(3)          # 3 = C 埠
laser2.cmd(0xE0, 1)             # 換成遠距離模式（最遠 200 cm）
if laser2.wait_done():          # True = 完成
    print('已換成遠距離')
```

| 命令 | 參數（超出範圍＝失敗） | 會做什麼 | 什麼時候用 | 範例 |
| :---: | :--- | :--- | :--- | :---: |
| `E0` 測距模式 | 0 一般（出廠，最遠 120 cm）／1 遠距離（最遠 200 cm）／2 精準（每 100 ms）／3 高速（每 20 ms） | 換模式，同時把更新時間、距離上限重設成這個模式的預設值 | 要量超過 120 cm 用遠距離；數字要穩用精準；要反應快用高速 | 1 |
| `E1` 濾波 | 0 關／1 最近 3 筆中位數（出廠）／2 最近 5 筆中位數 | 把偶爾一次的怪值濾掉 | 數字跳來跳去 → 2；比賽要最快反應 → 0 | 2 |
| `E2` 更新時間 | 20～200 ms（0＝模式預設） | 每量一次花多久：越短越快、越長越穩 | 快速避障 → 20；量遠、要穩 → 200 | 3 |
| `E3` 回出廠 | 一定要 `0xA5` | 模式 0、濾波 1、33 ms、上限 120 cm | 程式一開始先送，從乾淨的狀態開始 | 4 |
| `E4` 距離上限 | 10～模式上限 cm（一般 120、遠距離 200；0＝模式上限） | 超過上限的東西一律當成沒東西，讀到的值就是上限 | 只想對近的東西反應，不被遠處走動的人干擾 | 5 |

- **SPIKE 超出範圍＝失敗、設定不變**（`wait_done()` 回 False），不會像 Arduino I2C 那樣被夾到最近的值。
- **換模式（E0）會把更新時間和距離上限重設**：要搭配 E2、E4，先送 E0 再送它們。
- **設定只存在感應器記憶體**：拔線或主機關機就回出廠（一般模式、上限 120 cm），程式每次開始要重新送。
- **「沒東西」讀到的是上限**：一般模式 120、遠距離模式 200，用 E4 改過就是你設的上限。
- **同一個命令＋同樣參數再呼叫不會重做**（放在迴圈裡也只做一次）；要再做一次寫 `cmd(..., again=True)`（範例 4）。
- `wait_done()` 每次要多等幾十毫秒，**不要放在循跡、避障這種快速迴圈裡**；在程式開頭設定好就好。E0、E2、E3 要重新設定雷射晶片，約 0.1～0.5 秒。
- **EV3、MATRIX 版沒有命令**（只能讀）。

#### 命令範例（5 支）

每支都是：感應器接 **C 埠**，把範例和 `MBC_laser2_obj_Lib.py` 一起匯入 Pybricks Code 再執行；用主機的左、右鍵換設定，螢幕顯示目前設定、電腦畫面印讀值。[📥 打包下載（5 支範例＋函式庫）](/examples/tof2/pybricks_cmd/tof2_pybricks_cmd_examples.zip)

**1. 測距模式（E0）**

白牆或大白板放在正前方約 150 公分。一般模式讀到 120（超過上限＝沒東西），按右鍵換成 1 遠距離就讀到約 150。

[📄 下載 cmd01_long_range.py](/examples/tof2/pybricks_cmd/cmd01_long_range.py) ＋ [函式庫](/examples/tof2/pybricks_cmd/MBC_laser2_obj_Lib.py)

```python
# 測距者 2 路 命令範例 01（Pybricks，SPIKE Prime／Robot Inventor 主機）：測距模式：一般／遠距離／精準／高速
# 用主機的左、右鍵切換 4 種測距模式（命令 E0），螢幕顯示目前模式，主控台印距離：
#   0 一般（出廠，最遠 120 cm）、1 遠距離（最遠 200 cm）、2 精準（每 100 ms，最穩）、3 高速（每 20 ms，最快）
# 擺法：白牆或大白板放在左右兩顆雷射的正前方約 150 公分。一般模式會讀到 120（超過上限＝當成沒東西），換成 1 就讀到約 150
# 接線：測距者 2 路 → C 埠；跟 MBC_laser2_obj_Lib.py 一起上傳（Pybricks Code 把兩個檔放在一起）
# 命令只有 SPIKE 版能下（感應器韌體 2026-11 以後）；設定存在感應器記憶體，拔線就回出廠
#   cmd(命令碼, 參數)：送命令；wait_done()：等它做完，True = 完成、False = 失敗（參數超出範圍）或沒回應
#   命令表：E0 測距模式、E1 濾波、E2 更新時間、E3 回出廠（參數 0xA5）、E4 距離上限
from pybricks.hubs import PrimeHub
from pybricks.parameters import Button, Color
from pybricks.tools import wait, StopWatch
from MBC_laser2_obj_Lib import MBC_LASER2

hub = PrimeHub()
laser2 = MBC_LASER2(3)                   # 3 = C 埠（1=A、2=B、3=C、4=D、5=E、6=F）

mode = 0
laser2.cmd(0xE3, 0xA5)              # 先回出廠設定（一般模式）
laser2.wait_done()
hub.display.number(mode)
t = StopWatch()

while True:
    pressed = hub.buttons.pressed()    # 每 10 ms 看一次按鍵，按一下就收得到
    if Button.RIGHT in pressed or Button.LEFT in pressed:
        mode = (mode + (1 if Button.RIGHT in pressed else 3)) % 4   # 右鍵 +1、左鍵 -1
        laser2.cmd(0xE0, mode)        # 換測距模式（要重新設定雷射晶片，約 0.1～0.5 秒）
        ok = laser2.wait_done()       # 等它做完：True = 完成
        hub.display.number(mode)
        print('換成模式', mode, ['一般', '遠距離', '精準', '高速'][mode], '完成' if ok else '失敗')
        while hub.buttons.pressed():   # 等放開按鍵
            wait(10)
    if t.time() >= 300:                # 每 0.3 秒印一次距離
        t.reset()
        print('左', laser2.left(), '右', laser2.right(), 'cm｜左', laser2.left_mm(), 'mm')
    wait(10)
```

**2. 濾波（E1）**

盒子或白板放在正前方約 50 公分不要動。每秒印「1 秒內最大－最小」＝跳動幾 mm，濾波越大越穩。

[📄 下載 cmd02_filter.py](/examples/tof2/pybricks_cmd/cmd02_filter.py) ＋ [函式庫](/examples/tof2/pybricks_cmd/MBC_laser2_obj_Lib.py)

```python
# 測距者 2 路 命令範例 02（Pybricks，SPIKE Prime／Robot Inventor 主機）：濾波：讓數字比較穩
# 用主機的左、右鍵切換濾波（命令 E1）：0 關、1 最近 3 筆中位數（出廠）、2 最近 5 筆中位數
# 擺法：盒子或白板放在左右兩顆雷射的正前方約 50 公分不要動。每秒印「1 秒內最大−最小」＝數字跳動幾 mm，濾波越大越穩
# 接線：測距者 2 路 → C 埠；跟 MBC_laser2_obj_Lib.py 一起上傳（Pybricks Code 把兩個檔放在一起）
# 命令只有 SPIKE 版能下（感應器韌體 2026-11 以後）；設定存在感應器記憶體，拔線就回出廠
#   cmd(命令碼, 參數)：送命令；wait_done()：等它做完，True = 完成、False = 失敗（參數超出範圍）或沒回應
#   命令表：E0 測距模式、E1 濾波、E2 更新時間、E3 回出廠（參數 0xA5）、E4 距離上限
from pybricks.hubs import PrimeHub
from pybricks.parameters import Button, Color
from pybricks.tools import wait, StopWatch
from MBC_laser2_obj_Lib import MBC_LASER2

hub = PrimeHub()
laser2 = MBC_LASER2(3)                   # 3 = C 埠（1=A、2=B、3=C、4=D、5=E、6=F）

filt = 1
laser2.cmd(0xE3, 0xA5)              # 先回出廠設定（濾波 1）
laser2.wait_done()
wait(500)                        # 等感應器量到新的值再開始算
hub.display.number(filt)

while True:
    lo, hi = 99999, -1
    t = StopWatch()
    while t.time() < 1000:           # 1 秒內一直讀，記下最大、最小
        mm = laser2.left_mm()
        lo = min(lo, mm)
        hi = max(hi, mm)
        pressed = hub.buttons.pressed()
        if Button.RIGHT in pressed or Button.LEFT in pressed:
            filt = (filt + (1 if Button.RIGHT in pressed else 2)) % 3
            laser2.cmd(0xE1, filt)      # 換濾波（馬上生效）
            laser2.wait_done()
            hub.display.number(filt)
            print('換成濾波', filt)
            while hub.buttons.pressed():
                wait(10)
            lo, hi = 99999, -1
            t.reset()
        wait(20)
    print('濾波', filt, '｜左 mm', lo, '~', hi, '｜跳動', hi - lo, 'mm')
```

**3. 更新時間（E2）**

盒子或白板放在正前方約 50 公分不要動。每秒印「值換了幾次」（≈ 更新幾次）和跳動幾 mm：20 ms 最快、200 ms 最穩。

[📄 下載 cmd03_budget.py](/examples/tof2/pybricks_cmd/cmd03_budget.py) ＋ [函式庫](/examples/tof2/pybricks_cmd/MBC_laser2_obj_Lib.py)

```python
# 測距者 2 路 命令範例 03（Pybricks，SPIKE Prime／Robot Inventor 主機）：更新時間：快一點還是準一點
# 用主機的左、右鍵切換更新時間（命令 E2）：20、33（出廠）、100、200 ms
# 擺法：盒子或白板放在左右兩顆雷射的正前方約 50 公分不要動。每秒印「值換了幾次」（≈ 更新幾次）和「跳動幾 mm」
# 接線：測距者 2 路 → C 埠；跟 MBC_laser2_obj_Lib.py 一起上傳（Pybricks Code 把兩個檔放在一起）
# 命令只有 SPIKE 版能下（感應器韌體 2026-11 以後）；設定存在感應器記憶體，拔線就回出廠
#   cmd(命令碼, 參數)：送命令；wait_done()：等它做完，True = 完成、False = 失敗（參數超出範圍）或沒回應
#   命令表：E0 測距模式、E1 濾波、E2 更新時間、E3 回出廠（參數 0xA5）、E4 距離上限
from pybricks.hubs import PrimeHub
from pybricks.parameters import Button, Color
from pybricks.tools import wait, StopWatch
from MBC_laser2_obj_Lib import MBC_LASER2

hub = PrimeHub()
laser2 = MBC_LASER2(3)                   # 3 = C 埠（1=A、2=B、3=C、4=D、5=E、6=F）

CHOICES = [20, 33, 100, 200]
k = 1
laser2.cmd(0xE3, 0xA5)              # 先回出廠設定（33 ms）
laser2.wait_done()
laser2.cmd(0xE1, 0)                 # 關濾波，比較看得出差別
laser2.wait_done()
wait(500)                        # 等感應器量到新的值再開始算
hub.display.number(CHOICES[k])

while True:
    last, changes, lo, hi = -1, 0, 99999, -1
    t = StopWatch()
    while t.time() < 1000:
        mm = laser2.left_mm()
        if mm != last:
            changes += 1
            last = mm
        lo = min(lo, mm)
        hi = max(hi, mm)
        pressed = hub.buttons.pressed()
        if Button.RIGHT in pressed or Button.LEFT in pressed:
            k = (k + (1 if Button.RIGHT in pressed else 3)) % 4
            laser2.cmd(0xE2, CHOICES[k])    # 換更新時間（要重新設定雷射晶片）
            ok = laser2.wait_done()
            hub.display.number(CHOICES[k])
            print('換成', CHOICES[k], 'ms', '完成' if ok else '失敗')
            while hub.buttons.pressed():
                wait(10)
            last, changes, lo, hi = -1, 0, 99999, -1
            t.reset()
        wait(5)
    print(CHOICES[k], 'ms｜1 秒內換了', changes, '次｜跳動', hi - lo, 'mm')
```

**4. 回出廠設定（E3）**

白板放在正前方約 60 公分。先把上限改成 30 cm，讀數被「蓋」在 30；按中間鍵送 E3 回出廠，上限回到 120，又看得到 60 公分的白板。要停程式：按住中間鍵不放。

[📄 下載 cmd04_factory_reset.py](/examples/tof2/pybricks_cmd/cmd04_factory_reset.py) ＋ [函式庫](/examples/tof2/pybricks_cmd/MBC_laser2_obj_Lib.py)

```python
# 測距者 2 路 命令範例 04（Pybricks，SPIKE Prime／Robot Inventor 主機）：回出廠設定
# 回出廠設定（命令 E3，參數一定要是 0xA5）：先把距離上限改成 30 cm，看讀數被「蓋」在 30；
# 按中間鍵送 E3 回出廠 → 上限回到 120 cm，讀數又看得到遠的東西
# 擺法：白板放在左右兩顆雷射的正前方約 60 公分（比 30 遠、比 120 近）
# 接線：測距者 2 路 → C 埠；跟 MBC_laser2_obj_Lib.py 一起上傳（Pybricks Code 把兩個檔放在一起）
# 命令只有 SPIKE 版能下（感應器韌體 2026-11 以後）；設定存在感應器記憶體，拔線就回出廠
#   cmd(命令碼, 參數)：送命令；wait_done()：等它做完，True = 完成、False = 失敗（參數超出範圍）或沒回應
#   命令表：E0 測距模式、E1 濾波、E2 更新時間、E3 回出廠（參數 0xA5）、E4 距離上限
from pybricks.hubs import PrimeHub
from pybricks.parameters import Button, Color
from pybricks.tools import wait, StopWatch
from MBC_laser2_obj_Lib import MBC_LASER2

hub = PrimeHub()
laser2 = MBC_LASER2(3)                   # 3 = C 埠（1=A、2=B、3=C、4=D、5=E、6=F）

laser2.cmd(0xE4, 30)                # 先把距離上限改成 30 cm（超過 30 = 當成沒東西，讀到 30）
laser2.wait_done()
hub.display.number(30)
print('距離上限 30 cm：按中間鍵送 E3 回出廠')
hub.system.set_stop_button(None)   # 中間鍵拿來用（要停程式：按住中間鍵不放）

t = StopWatch()
while True:
    if Button.CENTER in hub.buttons.pressed():   # 每 10 ms 看一次按鍵
        laser2.cmd(0xE3, 0xA5, again=True)   # 回出廠；again=True：同一個命令也要再做一次
        ok = laser2.wait_done()
        hub.display.number(120)
        hub.speaker.beep()
        print('回出廠', '完成' if ok else '失敗', '：上限回到 120 cm')
        while hub.buttons.pressed():
            wait(10)
    if t.time() >= 300:                # 每 0.3 秒印一次距離
        t.reset()
        print('左', laser2.left(), '右', laser2.right(), 'cm｜左', laser2.left_mm(), 'mm')
    wait(10)
```

**5. 距離上限（E4）**

左、右鍵把上限改成 10、20…120 cm。比上限近就亮紅燈＋嗶，否則綠燈：遠處有人走動也不會誤觸。

[📄 下載 cmd05_max_distance.py](/examples/tof2/pybricks_cmd/cmd05_max_distance.py) ＋ [函式庫](/examples/tof2/pybricks_cmd/MBC_laser2_obj_Lib.py)

```python
# 測距者 2 路 命令範例 05（Pybricks，SPIKE Prime／Robot Inventor 主機）：距離上限：只看近的東西
# 距離上限（命令 E4，cm）：超過上限的東西當成「沒東西」，讀到的值就是上限 → 只對近的東西反應
# 左、右鍵把上限改成 10、20、30…120 cm（螢幕顯示），較近距離比上限近就亮紅燈＋嗶，否則綠燈
# 接線：測距者 2 路 → C 埠；跟 MBC_laser2_obj_Lib.py 一起上傳（Pybricks Code 把兩個檔放在一起）
# 命令只有 SPIKE 版能下（感應器韌體 2026-11 以後）；設定存在感應器記憶體，拔線就回出廠
#   cmd(命令碼, 參數)：送命令；wait_done()：等它做完，True = 完成、False = 失敗（參數超出範圍）或沒回應
#   命令表：E0 測距模式、E1 濾波、E2 更新時間、E3 回出廠（參數 0xA5）、E4 距離上限
from pybricks.hubs import PrimeHub
from pybricks.parameters import Button, Color
from pybricks.tools import wait, StopWatch
from MBC_laser2_obj_Lib import MBC_LASER2

hub = PrimeHub()
laser2 = MBC_LASER2(3)                   # 3 = C 埠（1=A、2=B、3=C、4=D、5=E、6=F）

limit = 30
laser2.cmd(0xE3, 0xA5)              # 先回出廠設定
laser2.wait_done()
laser2.cmd(0xE4, limit)             # 距離上限 30 cm
laser2.wait_done()
hub.display.number(limit)
t = StopWatch()

while True:
    pressed = hub.buttons.pressed()    # 每 10 ms 看一次按鍵
    if Button.RIGHT in pressed or Button.LEFT in pressed:
        limit = limit + 10 if Button.RIGHT in pressed else limit - 10
        limit = max(10, min(120, limit))
        laser2.cmd(0xE4, limit)       # 改上限（馬上生效）
        laser2.wait_done()
        hub.display.number(limit)
        while hub.buttons.pressed():
            wait(10)
    if t.time() >= 200:                # 每 0.2 秒看一次距離
        t.reset()
        cm = min(laser2.left(), laser2.right())
        if cm < limit:               # 比上限近 = 真的有東西
            hub.light.on(Color.RED)
            hub.speaker.beep(1000, 50)
        else:
            hub.light.on(Color.GREEN)
        print('上限', limit, 'cm｜', '左', laser2.left(), '右', laser2.right(), 'cm｜左', laser2.left_mm(), 'mm')
    wait(10)
```
