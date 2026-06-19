# 神攝手高階視覺感應器

<div style="text-align: center;">
  <span style="display:inline-block; background:#00d2ff; color:#0a0a0a; padding:3px 12px; border-radius:20px; font-family:monospace; margin-bottom:1rem; font-size: 0.85rem;">CAM-VIS-PRO · VISION</span>
  <br>
  <img src="../images/sensors/esp32cam/esp32cam-product.webp" alt="神攝手視覺感應器" style="max-width: 280px; margin: 1rem auto; display: block; filter: drop-shadow(0 0 30px rgba(0,210,255,0.2));" />
  <p style="font-family: monospace; color: #aaa; letter-spacing: 1px;">
    <strong>高階機器人視覺鏡頭模組</strong><br>
    賦予機器人真實視界 · 精準色塊追蹤 · 座標定位
  </p>
</div>

---

> **賦予機器人真實視界：專精於色塊追蹤與精準座標定位的高階視覺鏡頭。**

## 🚀 產品概述

樂高原廠的顏色感應器只能偵測感應器正下方的單一點顏色，完全無法進行「追蹤移動中的色塊」或「判斷目標在畫面中的位置」。

神攝手高階視覺感應器讓您的樂高機器人真正擁有「眼睛」，能即時偵測指定顏色的色塊在畫面中的位置與大小，並將座標數據直接傳遞給主機。讓機器人能夠主動追蹤目標、執行精準的對準任務，開啟全新的機器人應用可能性。

## 🎯 核心優勢

- 👁️ **真實視覺追蹤**：偵測指定顏色色塊在畫面中的 X/Y 座標與面積
- 🎯 **精準座標定位**：透過查詢特定座標點的顏色，實現精準的位置辨識
- 🔌 **即插即用**：直接插上 SPIKE / EV3，官方積木直接讀取視覺數據
- 🚀 **零程式障礙**：無需學習影像處理，複雜的視覺演算法全在感應器內部完成

## 📋 主要功能

### 色塊追蹤
設定目標顏色後，感應器會持續輸出：
- **X 座標**：色塊中心的水平位置
- **Y 座標**：色塊中心的垂直位置
- **面積**：色塊的大小（可用於估算距離）

### 座標點查色
查詢畫面中特定座標點的顏色，適合用於：
- 辨識地標顏色
- 確認物品擺放位置

## 🔌 硬體接線

1. 將感應器插入 SPIKE（Port A~F）或 EV3（Port 1~4）
2. 通電後感應器指示燈亮起即完成，無需任何額外設定
