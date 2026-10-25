> [!TIP]
> **🧩 掌控者 第一次用？跟著互動教學一步一步做：**[MicroPython 版](/learn/gamepad/micropython.html)・[Pybricks 積木版](/learn/gamepad/pybricks-blocks.html)。左邊點選、右邊程式跟著長出來，每個功能都有範例，還有模擬感應器可以操作看數值。

### 配對手把（第一次用或要換手把）

1. 接收器接好、主機開機。**插上去的時候不要按住接收器上的按鍵**（會進入燒錄模式，接收器就不會動作；放開按鍵、拔掉重插就好）。
2. **第一次使用**：接收器還沒記住任何手把時，一開機就會自動開放 **30 秒**配對（綠燈快速閃）。**要換手把、或 30 秒已經過了**：按一下接收器上的按鍵（Reset），會清掉舊的手把、重新開放 30 秒。
3. 在這 30 秒內讓手把進入配對模式，直到手把的燈條快速閃爍：**PS4 手把**同時長按 **SHARE＋PS**；**PS5 手把**同時長按 **建立（Create）＋PS**。
4. 連上之後接收器會**記住這支手把**，也不會再連別人的手把。之後開機，只要按手把的 **PS 鍵**就會自動連上。

| 接收器的綠燈 | 意思 |
| :--- | :--- |
| 快速閃 | 正在等手把配對 |
| 一直亮 | 沒有連上手把（按手把 PS 鍵重新連線） |
| 熄掉 | 已經連上手把 |

> [!TIP]
> 比賽現場請在自己的位置完成配對，避免 30 秒內有別隊手把也在配對模式。綁錯手把就再按一次 Reset 重新配對。

### 函式庫與範例下載（Pybricks 4.1）

- [📥 全部下載（.zip）](/examples/ps4/ps4_pybricks_spike_v4.x.zip)：`MBC_sensors.py`、`MBC_gamepad_Lib.py` 和 6 個新手範例
- 單獨下載函式庫：[MBC_sensors.py](/examples/ps4/pybricks/MBC_sensors.py)（新專案用這個，一個檔適用全部 MBC 感應器，寫 `from MBC_sensors import MBC_GAMEPAD`）・[MBC_gamepad_Lib.py](/examples/ps4/pybricks/MBC_gamepad_Lib.py)（本頁範例與舊程式用）

> [!NOTE]
> Pybricks 3.6.1 版範例已不再更新，舊檔放在[下載專區](/downloads.md)的「舊版（不再更新）」。
