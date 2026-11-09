### 接線與供電

1. 把 EXP6 的 **Host 通訊線**插進 SPIKE Prime 任一孔（Port A~F）。
2. EXP6 一定要接 **2S 鋰電池**（XT30 接頭）供電給馬達和樂高感應器；不要接 3S 以上（超過 9 V 會停止供電給樂高顏色／超音波感應器）。
3. 樂高感應器與馬達接在擴充器的 1～6 號孔。

> [!WARNING]
> 這個寫法的 EXP6 只支援 **SPIKE Prime** 主機（Robot Inventor（51515）主機請用 [Robot Inventor App 的 Python](/learn/exp6/robot-inventor.html)；不支援 EV3）。**換另一種寫法之前，先讓 EXP6 斷電一次**（關主機或拔掉 Host 線）：EXP6 一旦跑過 Pybricks 程式，就會一直用 Pybricks 的方式溝通，直到重新開機。
