// ============================================================
// MBC_TOF8.h
// 測距者 8 路（TOF8）通用 I2C 版 簡易函式庫（header-only）
// MBC 通用 I2C 協議 v1，裝置位址 0x29
// 說明：MBC 產品網站「MBC 通用 I2C 協議」頁與「測距者 8 路」產品頁
//
// 讀取方式：主機先寫 1 byte「暫存器編號」，再 requestFrom 讀回資料。
// 多位元組數值一律高位元組在前（MSB-first）。
// 距離一律 uint16、單位 mm：1200 = 範圍內沒有目標、0xFFFF = 該顆故障。
//
//   0x07        → 8 bytes ：型號字串（"LASER8"）
//   0x11        → 6 bytes ：最近方位 int8、最近距離 mm、最遠方位 int8、最遠距離 mm
//   0x21        → 16 bytes：8 顆距離 mm（第 0 顆 = 最左、第 7 顆 = 最右）
//   0x22 ~ 0x29 → 2 bytes ：第 0 ~ 7 顆距離 mm
//   0x32        → 1 byte  ：bit0 = 就緒、bit1 = 有任一顆故障
//   0x34 / 0x35 / 0x36 → 1 byte：初始化成功／通訊錯誤／逾時 位元圖（bit i = 第 i 顆）
//   方位：-8（最左）~ +8（最右），0 = 正前方
// ============================================================
#ifndef MBC_TOF8_H
#define MBC_TOF8_H

#include <Wire.h>

class MBC_TOF8 {
  public:
    static const uint16_t NO_TARGET = 1200;   // 範圍內沒有目標
    static const uint16_t FAULT     = 0xFFFF; // 該顆故障

    explicit MBC_TOF8(uint8_t i2cAddr = 0x29) : _addr(i2cAddr) {}

    void begin() {
      Wire.begin(); // Uno/Nano 固定用 A4=SDA, A5=SCL
    }

    // 8 顆初始化完成（開機約 2~4 秒）才回 true
    bool isReady() {
      uint8_t s;
      if (!readReg(0x32, &s, 1)) return false;
      return (s & 0x01) != 0;
    }

    // 讀型號字串（model 至少 9 bytes）
    bool readModel(char model[9]) {
      uint8_t b[8];
      if (!readReg(0x07, b, 8)) return false;
      for (uint8_t i = 0; i < 8; i++) model[i] = (char)b[i];
      model[8] = '\0';
      return true;
    }

    // 讀取全部 8 顆距離（mm）
    bool readAll(uint16_t mm[8]) {
      uint8_t b[16];
      if (!readReg(0x21, b, 16)) return false;
      for (uint8_t i = 0; i < 8; i++) mm[i] = be16(&b[2 * i]);
      return true;
    }

    // 讀取單一顆（ch: 0~7，0 = 最左）距離（mm）
    bool readChannel(uint8_t ch, uint16_t &mm) {
      if (ch > 7) return false;
      uint8_t b[2];
      if (!readReg(0x22 + ch, b, 2)) return false;
      mm = be16(b);
      return true;
    }

    // 讀取最近／最遠目標的方位（-8 ~ +8）與距離（mm）
    bool readMinMax(int8_t &minDir, uint16_t &minMm, int8_t &maxDir, uint16_t &maxMm) {
      uint8_t b[6];
      if (!readReg(0x11, b, 6)) return false;
      minDir = (int8_t)b[0];
      minMm  = be16(&b[1]);
      maxDir = (int8_t)b[3];
      maxMm  = be16(&b[4]);
      return true;
    }

    // 讀取初始化成功／通訊錯誤／逾時位元圖（bit i = 第 i 顆）
    bool readStatus(uint8_t &initOk, uint8_t &commErr, uint8_t &timeout) {
      uint8_t b[3];
      if (!readReg(0x34, &b[0], 1)) return false;
      if (!readReg(0x35, &b[1], 1)) return false;
      if (!readReg(0x36, &b[2], 1)) return false;
      initOk = b[0];
      commErr = b[1];
      timeout = b[2];
      return true;
    }

  private:
    uint8_t _addr;

    static uint16_t be16(const uint8_t *p) {
      return (uint16_t)(((uint16_t)p[0] << 8) | p[1]);
    }

    // 選定暫存器後讀回 n bytes（n ≤ 32）；false = 裝置沒回應
    bool readReg(uint8_t reg, uint8_t *buf, uint8_t n) {
      Wire.beginTransmission(_addr);
      Wire.write(reg);
      if (Wire.endTransmission() != 0) return false; // 非 0 = 沒收到 ACK
      if (Wire.requestFrom(_addr, n) != n) return false;
      for (uint8_t i = 0; i < n; i++) buf[i] = Wire.read();
      return true;
    }
};

#endif
