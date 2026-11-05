<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# EXP6 函式庫安裝教學：MINDSTORMS Robot Inventor（玩具版 51515）

這頁寫給老師、學生和第一次用的人：照順序一步一步做，就能讓 **LEGO MINDSTORMS Robot Inventor（51515）主機**用 Python 控制 EXP6。**函式庫只要裝一次**，之後每個 Python 專案都能用；主機韌體更新後要再裝一次。

> 用的是 **LEGO Education SPIKE Prime**（SPIKE App 3）？請看 [教育版安裝教學](/sensors/exp6/install-spike-app.md)。

## 你需要

- Robot Inventor 主機（電池充好電）
- **USB 線或藍牙都可以**（建議用 USB 線：主機附的那條就可以，只能充電的線不行）
- 電腦或平板，裝好 LEGO MINDSTORMS App（Robot Inventor App 10.x）
- EXP6 韌體 2026-11-03 以後
- 從網站下載的 [MBC_EXP6_Robot_Inventor_Lib.zip](/downloads/MBC_EXP6_Robot_Inventor_Lib.zip)，先解壓縮

## USB 線還是藍牙

兩種我們都在 App 裡實際裝過，都成功（藍牙約 3 秒裝完）。**建議用 USB 線**：安裝時要傳一個比較大的檔案，接線比較穩。手邊沒有可以傳資料的線，用藍牙也可以。

> LEGO 已在 2026-10-01 停止 Robot Inventor App 的更新與支援；2026-11-05 用 App 10.5.4 實測，安裝和執行都正常。

## 安裝步驟

1. 用 USB 線把主機接到電腦（或用藍牙），按主機中間的按鈕開機。
2. 打開 LEGO MINDSTORMS App，連上主機（App 會顯示已連線）。
3. 新增一個專案，類型選 **Python**。
4. 用記事本打開解壓縮出來的 `Install_MBC_exp6_Lib.py`，**全選、複製**。
5. 把 App 的 Python 編輯區原本的內容刪掉，**貼上**剛才複製的內容。
6. 按 App 的「執行」（▶）。
7. 打開 App 下方的主控台（Console），大約 10 秒內會看到：

```text
MBC_exp6_SPIKE_App_Lib.py … B OK
MBC_exp6_mbc.py … B OK
MBC_exp6_cam.py … B OK
安裝成功！ (Installed OK)
```

   看到「安裝成功」就完成了。這個安裝專案可以刪掉，函式庫已經存在主機裡（`/projects/mbc_exp6/`，關機不會不見）。
8. 用 USB 線裝的，現在拔掉也沒關係，之後用藍牙連線就可以。

> [!TIP]
> 看到「安裝失敗，請再執行一次」：再按一次執行。還是不行，請改用 USB 線連線再裝一次。

## 開始寫程式

1. EXP6 接上電池，用 EXP6 的主機線（Host 線）接到主機的某一個孔（例如 F 孔）。
2. 新增一個 Python 專案，開頭這樣寫：

```python
from hub import port
import time
from projects.mbc_exp6.MBC_exp6_SPIKE_App_Lib import MBC_EXP6

exp = MBC_EXP6(port.F)          # EXP6 插在主機的 F 孔
exp.motor_run(1, 50)            # EXP6 1 號孔的馬達以 50% 速度轉
time.sleep(2)
exp.motor_stop(1, "brake")      # 煞車停
```

3. 按執行。也可以直接打開下載包裡 `examples` 資料夾的 10 支範例來試（EXP6 插 F 孔、馬達接 EXP6 的 1／2 號孔）。

👉 想先在網頁上點點看每個功能怎麼寫：[Robot Inventor App 互動教學](/learn/exp6/robot-inventor.html)；全部說明在 [官方 App 說明頁的 Robot Inventor 段](/sensors/exp6/spike/official.md)。

## 玩具版要知道的 3 件事

1. **EXP6 執行過程式後，要拔電池重開才會回到一般模式**：這段時間 App 的裝置頁看不到 EXP6 是正常的。要換到別台主機（例如教育版 SPIKE Prime）前，先拔掉 EXP6 電池再插回。
2. **程式結束後，馬達會在 10 秒內自動停**：要讓馬達轉超過 10 秒、中間又不做別的事，用 `exp.keep_alive_wait(毫秒)` 來等，例如 `exp.keep_alive_wait(15000)` 等 15 秒。
3. **匯入那一行跟教育版不一樣**：玩具版是 `from projects.mbc_exp6.MBC_exp6_SPIKE_App_Lib import …`，寫錯會出現 `ImportError`。

## 移除

不要用了：新增一個 Python 專案，貼上下載包裡的 `Uninstall_MBC_exp6_Lib.py` 執行，看到「移除完成！」就好。

## 常見問題

| 看到什麼 | 怎麼辦 |
| :--- | :--- |
| `ImportError: no module named …` | 還沒安裝，或主機韌體更新後被清掉 → 再裝一次；也確認匯入那行有 `projects.mbc_exp6.` |
| 「讀不到擴充板資料」 | 確認 EXP6 有接電池、`MBC_EXP6(port.X)` 的孔跟實際插的一樣 |
| 安裝時主機自己關機 | 改用 USB 線連線再裝一次 |
| 馬達跑一陣子自己停 | 超過 10 秒沒呼叫函式 → 用 `exp.keep_alive_wait(毫秒)` |
| App 裝置頁看不到 EXP6 | 拔 EXP6 電池再插回 |
| `MemoryError` | 程式太大 → 把程式拆小、重開主機再試 |

還有問題？到 [聯絡我們](/contact.md)，附上主控台的訊息截圖。
