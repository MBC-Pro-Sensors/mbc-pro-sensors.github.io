<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# EXP6 函式庫安裝教學：SPIKE Prime（教育版 SPIKE App 3）

這頁寫給老師、學生和第一次用的人：照順序一步一步做，就能在 **LEGO Education SPIKE App 3** 用 Python 控制 EXP6。**函式庫只要裝一次**，之後每個 Python 專案都能用；主機韌體更新後要再裝一次。

> 用的是 **LEGO MINDSTORMS Robot Inventor（51515）**？請看 [玩具版安裝教學](/sensors/exp6/install-robot-inventor.md)。

## 你需要

- SPIKE Prime 主機（電池充好電）
- 電腦或平板，裝好 LEGO Education SPIKE App 3
- USB 線或藍牙都可以
- EXP6 韌體 2026-11-03 以後
- 從網站下載的 [MBC_EXP6_Official_App_Lib.zip](/downloads/MBC_EXP6_Official_App_Lib.zip)，先解壓縮

## 安裝步驟

1. 主機開機，用 USB 線或藍牙在 SPIKE App 連上主機。
2. 新增一個專案，類型選 **Python**。
3. 用記事本打開解壓縮出來的 `SPIKE_Education\Install_MBC_exp6_Lib.py`，**全選、複製**，貼到 App 的 Python 編輯區（原本的內容刪掉）。
4. 按「執行」（▶），打開 App 下方的主控台（Console），出現「**安裝成功！(Installation Successful!)**」就完成了。這個安裝專案可以刪掉，函式庫已經存在主機裡。

> [!TIP]
> 不想複製貼上：也可以下載 [install_exp6_lib.llsp3](/examples/exp6/spike_beginner/official/install_exp6_lib.llsp3)，在 SPIKE App 用「檔案」→「開啟」打開它、按 ▶ 執行一次，裡面的程式跟 `Install_MBC_exp6_Lib.py` 一樣。

## 開始寫程式

1. EXP6 接上電池，用 EXP6 的主機線（Host 線）接到主機的某一個孔（例如 F 孔）。
2. 新增一個 Python 專案，開頭這樣寫：

```python
from hub import port
import time
from MBC_exp6_SPIKE_App_Lib import MBC_EXP6

exp = MBC_EXP6(port.F)          # EXP6 插在主機的 F 孔
exp.motor_run(1, 50)            # EXP6 1 號孔的馬達以 50% 速度轉
time.sleep(2)
exp.stop_all(1)                 # 最後讓全部馬達停下（教育版程式結束後馬達不會自己停）
```

3. 按執行。也可以打開下載包裡 `examples` 資料夾的 10 支範例來試（EXP6 插 F 孔）。

👉 想先在網頁上點點看每個功能怎麼寫：[官方 SPIKE App 互動教學](/learn/exp6/spike-app.html)；全部說明在 [官方 App 說明頁](/sensors/exp6/spike/official.md)。

## 移除

不要用了：新增一個 Python 專案，貼上下載包裡的 `SPIKE_Education\Uninstall_MBC_exp6_Lib.py` 執行，看到「移除成功！」就好。

## 常見問題

| 看到什麼 | 怎麼辦 |
| :--- | :--- |
| `ImportError: no module named …` | 還沒安裝，或主機韌體更新後被清掉 → 再裝一次 |
| 「讀不到擴充板資料」 | 確認 EXP6 有接電池、`MBC_EXP6(port.X)` 的孔跟實際插的一樣 |
| 程式結束後馬達還在轉 | 教育版沒有看門狗 → 程式最後寫 `exp.stop_all(1)` |
| 官方 App 連不上 EXP6 | EXP6 跑過 Pybricks 程式或接過玩具版主機 → 拔掉 EXP6 電池再插回 |
| `MemoryError` | 程式太大 → 把程式拆小、重開主機再試 |

還有問題？到 [聯絡我們](/contact.md)，附上主控台的訊息截圖。
