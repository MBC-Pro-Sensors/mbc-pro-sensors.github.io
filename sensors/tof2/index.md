# 測距者 2 路雷射測距 (TOF2)

<div style="text-align: center;">
  <span style="display:inline-block; background:#0ABAB5; color:#fff; padding:3px 12px; border-radius:20px; font-family:monospace; margin-bottom:1rem; font-size: 0.85rem;">TOF-02-LSR · ADVANCED</span>
  <br>
  <img src="../images/sensors/tof2/tof2-product.webp" alt="測距者2路雷射測距" style="max-width: 280px; margin: 1rem auto; display: block; filter: drop-shadow(0 0 30px rgba(10,186,181,0.2));" />
  <p style="font-family: monospace; color: #aaa; letter-spacing: 1px;">
    <strong>雙光束雷射測距模組</strong><br>
    輕巧且敏銳 · 精準捕捉物體左右動向 · 告別笨重超音波
  </p>
</div>

---

> **輕巧且敏銳：告別笨重超音波，精準捕捉物體左右動向的微型雷射。**

## 🚀 產品概述

傳統超音波感應器不只體積大，更有反應速度慢、容易受到回波干擾（產生鬼影數據）的致命缺陷，在高速機器人應用場景中完全力不從心。

測距者 2 路採用**雷射飛行時間技術**，以光速完成測距，反應速度遠超超音波。體積極致輕巧，適合安裝在機器人的各種位置。更獨特的是：兩個雷射點的配置，讓機器人能精準判斷物體是**從左側離開還是右側離開**，這是超音波完全無法做到的功能。

## 🎯 核心優勢

- ⚡ **雷射級極速反應**：不受聲波傳播速度限制，偵測反應遠快於超音波
- 🎯 **方向判斷**：獨特雙光束設計，能判斷物體最後是從左側還是右側離開感測範圍
- 📦 **極致輕巧**：體積遠小於超音波，不佔用機器人寶貴的安裝空間
- 🔌 **即插即用**：直接插上 SPIKE / EV3，不需額外電源或複雜設定
- 🔼 **無縫升級路徑**：保留與 TOF8 相容的雙 0 度設計，日後升級至 TOF8 無需修改核心程式邏輯

## 📊 適用場景對比

| 場景 | 推薦機型 |
| :--- | :--- |
| 簡單防撞、物體距離偵測、方向判斷 | ✅ **TOF2（本款）** |
| 相撲對戰、全方位 180 度避障 | ➡️ TOF8（8路全視角） |

## 🔌 硬體接線

1. 使用標準連接線插上 SPIKE（Port A~F）或 EV3（Port 1~4）
2. 通電後確認感應器指示燈正常亮起即完成

## 💡 方向判斷說明

感應器的兩個雷射點並排排列。當物體離開感測範圍時：
- **左側雷射先失去訊號** → 物體從**右側**離開
- **右側雷射先失去訊號** → 物體從**左側**離開

這讓您的機器人能對「目標消失後往哪個方向轉」做出智能判斷。
