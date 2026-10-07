<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# 神攝手 ESP32CAM 視覺感應器

<div style="text-align: center;">
  <span style="display:inline-block; background:#00d2ff; color:#0a0a0a; padding:3px 12px; border-radius:20px; font-family:monospace; margin-bottom:1rem; font-size: 0.85rem;">CAM-VIS-PRO · VISION</span>
  <br>
  <img src="/images/sensors/esp32cam/esp32cam-product.webp" alt="神攝手 ESP32CAM 視覺感應器" style="max-width: 280px; margin: 1rem auto; display: block; filter: drop-shadow(0 0 30px rgba(0,210,255,0.2));" />
  <p style="font-family: monospace; color: #aaa; letter-spacing: 1px;">
    <strong>內建螢幕的樂高機器人視覺鏡頭</strong><br>
    色塊追蹤 · 座標點取色 · 動態偵測 · 機身直接設定
  </p>
</div>

---

> **讓樂高機器人真正「看得見」：在感應器的螢幕上直接設定要追的顏色，不用寫一行影像處理程式。**

## 🚀 產品概述

樂高原廠的顏色感應器只能偵測正下方的一個點，完全無法「追蹤移動中的色塊」或「判斷目標在畫面中的位置」。

神攝手 ESP32CAM 視覺感應器讓樂高機器人擁有真正的眼睛：即時找出指定顏色的色塊位置與大小、讀取畫面上任一點的顏色、偵測畫面中的動作。所有設定都在**感應器背面的彩色螢幕與按鍵**上完成，看著即時畫面調整，設定值存在機身，下次開機直接使用。

## 🎯 核心優勢

- 🖥️ **機身螢幕即時預覽**：背面 1.8 吋彩色螢幕顯示鏡頭畫面與偵測結果，所見即所得
- 🎛️ **按鍵選單直接設定**：取色位置、HSV 容許範圍、最小色塊面積，都能在感應器上調整，不需電腦
- 💾 **設定斷電保存**：顏色設定與鏡頭參數（亮度、對比、飽和度、曝光、白平衡、翻轉、鏡像）存在機身
- 🎯 **一次輸出 5 種資訊**：取色點顏色、色塊面積、色塊 X / Y 座標、動態偵測值
- 💡 **內建補光燈**：光線不足的場地也能穩定辨色
- 🔭 **可選配鏡頭視角**：依場地需求選擇窄角（看遠）或廣角（看寬）

<div style="text-align: center;">
<img src="/images/sensors/esp32cam/esp32cam-product2.webp" alt="神攝手背面的 1.8 吋彩色螢幕" style="max-width: 320px; margin: 1rem auto; display: block;" />
</div>

## 📋 產品規格

| 項目 | 規格 |
| :--- | :--- |
| 影像解析度 | 160 × 120（座標 X `0` ~ `159`、Y `0` ~ `119`） |
| 機身螢幕 | 1.8 吋 128 × 160 彩色 TFT |
| 操作按鍵 | 左、右、確認、側鍵（短按進出選單；在選單中長按 1 秒清除該組顏色） |
| 鏡頭選配 | OV2640：24° / 40° / 50° / **65°（標配）** / 68° / 75° / 85° / 120° / 160° / 200°；OV3660：65° / 110° / 120° / 160° |
| 補光燈 | 內建，在機身選單開關（設定會保存） |

## 📤 輸出資料

| 輸出 | 內容 |
| :--- | :--- |
| 取色點顏色 | 畫面上指定座標點的顏色（色相碼：`0` ~ `239` = 色相、`240` ~ `245` = 白、`246` ~ `250` = 黑） |
| 色塊面積 | `0` ~ `100`（`0` = 沒找到，此時 X / Y 無效） |
| 色塊 X 座標 | 色塊中心的水平位置 |
| 色塊 Y 座標 | 色塊中心的垂直位置 |
| 動態偵測值 | `0` ~ `100`，畫面變化越大數值越高 |

## 🧩 支援平台

| 平台 | 支援方式 |
| :--- | :--- |
| LEGO SPIKE Prime / Robot Inventor | 官方 App、Pybricks |
| LEGO MINDSTORMS EV3 | EV3 官方軟體、Pybricks |
| MATRIX Mini R4 | UART |

> [!IMPORTANT]
> 每個平台是**不同的出廠版本**，請依你的主機選購對應版本。

## 🔌 硬體接線

1. 將感應器插入 SPIKE（Port A~F）或 EV3（Port 1~4）
2. 開機後螢幕顯示鏡頭畫面即可使用
3. 短按側鍵進入設定選單，用左／右鍵選擇、確認鍵進入；設定好的顏色與鏡頭參數會自動保存
