# ════════════════════════════════════════════════════════════════
#  SPIKE 六路 Servo 擴充板 Python 函式庫（官方 SPIKE App 版）
#  6-Port Servo Expansion Board Library for the official SPIKE App
#  Author: MBC robotics    Date: 2026.10
#  協議 / Protocol: Servo擴充板_上下行協議.md (v0.2)
#
#  !! 此函式庫專供官方 SPIKE App 使用 (非 Pybricks) !!
#  !! For the official SPIKE App only (not Pybricks) !!
# ════════════════════════════════════════════════════════════════
#
#  快速上手 / Quick start
#      from hub import port
#      from MBC_servo6_SPIKE_App_Lib import MBC_SERVO6
#      sv = MBC_SERVO6(port.A)
#      sv.servo_angle(1, 90)        # 定點模式 / position mode
#      sv.servo_run(2, 50)          # 速度模式 / speed mode (360° servo)
#
#  原理 / How it works
#    擴充板偽裝成超音波感測器：
#      下行 distance_sensor.show(port, [L1,L2,L3,L4])（燈光通道，每格 0~100）
#      上行 device.data(port) 讀 8 筆數值（各孔電流/狀態、總電流、板型）
#    The board poses as a distance sensor: commands go out through the light
#    channel, readings come back through device.data().
#
#  ⚠ 官方 OS 會在背景不斷重送最後一包燈光值；所有指令都是「設定成某狀態」，
#    重送無副作用。 / The OS keeps resending the last light payload; every
#    command sets a state, so resending is harmless.
#  ⚠ 每包之間至少要隔一個 OS 輪詢週期（初始化時自動量測，約 20~50ms），
#    連續控制多顆舵機時每顆各花一個週期。 / Each command waits one OS poll
#    period (auto-measured, ~20-50 ms); driving N servos takes N periods.
# ════════════════════════════════════════════════════════════════
import device, distance_sensor, time

_BOARD_R8 = 0x5301   # R8：協議版本 1 + 板型 0x53

_BIT_STALL  = 1 << 12
_BIT_MOVING = 1 << 13
_BIT_ACTIVE = 1 << 14
_BIT_FLT    = 1 << 12

_instances = []

def _default_instance():
    if not _instances:
        raise RuntimeError("請先建立擴充板物件 (例: sv = MBC_SERVO6(port.A))")
    return _instances[-1]


# ── 參數驗證 / Parameter validation（與 Pybricks 版一致）────────────
def _check_port(port_num, name="port"):
    if isinstance(port_num, bool) or not isinstance(port_num, int) or not (1 <= port_num <= 6):
        raise ValueError("%s=%r 不是有效孔位，必須是 1~6 的整數 / must be an int 1~6" % (name, port_num))

def _clamp(v, lo, hi):
    v = int(round(v))
    return lo if v < lo else (hi if v > hi else v)


# ── 封包建構 / Packet builders（與 Pybricks 版相同）──────────────────
def _build_angle(port_num, angle, speed=100):
    _check_port(port_num)
    a = _clamp(angle, 0, 399)
    return (port_num * 10, _clamp(speed, 1, 100), a // 100, a % 100)

def _build_pulse(port_num, pulse_us):
    _check_port(port_num)
    us = _clamp(pulse_us, 500, 2500)
    return (port_num * 10 + 1, 0, us // 100, us % 100)

def _build_release(port_num):
    _check_port(port_num)
    return (port_num * 10 + 2, 0, 0, 0)

def _build_calibrate(port_num, min_us, max_us, range_deg):
    _check_port(port_num)
    lo = _clamp(min_us, 0, 1000) // 10
    hi = (_clamp(max_us, 1500, 2500) - 1500) // 10
    rg = _clamp(range_deg, 10, 1000) // 10
    return (port_num * 10 + 3, lo, hi, rg)

def _build_stall(port_num, current_ma, time_ms, auto_release):
    _check_port(port_num)
    return (port_num * 10 + 4, _clamp(current_ma, 0, 1000) // 10,
            max(1, _clamp(time_ms, 50, 5000) // 50), 1 if auto_release else 0)


class MBC_SERVO6:

    def __init__(self, port_num):
        self._port = port_num
        self._last_cmd = None
        self._last_send_time = time.ticks_ms()
        self._throttle_ms = 20
        self._speed_cfg = {p: (1500, 1000) for p in range(1, 7)}
        self.connected = False

        # 1. 等 SPIKE 掛載好 LPF2 模擬感測器且收到數據（R8 恆為 0x5301）
        r = None
        for _ in range(100):
            r = device.data(self._port)
            if r and len(r) >= 8 and (r[7] & 0xFFFF) != 0:
                break
            time.sleep_ms(20)
        if not (r and len(r) >= 8 and (r[7] & 0xFFFF) != 0):
            print("[SERVO6] 未偵測到擴充板 / No servo board detected")
        else:
            self.connected = True
            if (r[7] & 0xFFFF) != _BOARD_R8:
                print("[SERVO6] 插的不是 Servo 擴充板 / Not a servo board (R8=0x%04X)" % (r[7] & 0xFFFF))

            # 2. 量測 OS 真實輪詢週期，節流間隔取 2 倍保證不掉包（同 EXP6 官方 App 庫）
            #    舵機閒置時串流內容靜止不變、量不到週期，最多只等 200ms，
            #    量不到就用保守值 60ms（EXP6 實測節流值約 53ms）。
            try:
                t0 = time.ticks_ms()
                last_d = device.data(self._port)
                changes = 0
                while changes < 10 and time.ticks_diff(time.ticks_ms(), t0) < 200:
                    cur = device.data(self._port)
                    if cur != last_d:
                        changes += 1
                        last_d = cur
                    time.sleep_ms(1)
                if changes:
                    period = time.ticks_diff(time.ticks_ms(), t0) / changes
                    self._throttle_ms = max(20, int(period * 2))
                else:
                    self._throttle_ms = 60
            except Exception:
                self._throttle_ms = 60

            # 3. 清掉上一輪程式殘留：全部放鬆、校正/堵轉設定回預設
            self._send(0, 0, 0, 0)
            time.sleep_ms(self._throttle_ms)

        _instances.append(self)

    # ── 內部：收發 ─────────────────────────────────────────────
    def _send(self, l1, l2, l3, l4):
        # 1. 去重：與上一包相同直接返回（OS 背景會持續重送，韌體端也會去重）
        cmd = [max(0, min(100, int(x))) for x in (l1, l2, l3, l4)]
        if cmd == self._last_cmd:
            return
        # 2. 節流：距上一包未滿一個 OS 輪詢週期就等，否則新包會被 OS 蓋掉
        elapsed = time.ticks_diff(time.ticks_ms(), self._last_send_time)
        if elapsed < self._throttle_ms:
            time.sleep_ms(self._throttle_ms - elapsed)
        distance_sensor.show(self._port, cmd)
        self._last_cmd = cmd
        self._last_send_time = time.ticks_ms()

    def _regs(self):
        r = device.data(self._port)
        if r and len(r) >= 8:
            return [x & 0xFFFF for x in r[:8]]
        return [0] * 8

    def _port_reg(self, port_num):
        _check_port(port_num)
        return self._regs()[port_num - 1]

    # ════════════════════════════════════════════════════════
    #  定點模式 / Position mode
    # ════════════════════════════════════════════════════════
    def servo_angle(self, port_num, angle, speed=100):
        # 轉到指定角度。speed 1~99 = 速度×6 °/s（漸進），100 = 立即到位。
        # 剛上電或放鬆後的第一次角度指令一律直接到位（板子不知道舵機實際位置）。
        # Go to angle. speed 1~99 = speed×6 °/s ramp, 100 = jump at native servo speed.
        self._send(*_build_angle(port_num, angle, speed))

    def servo_wait(self, port_num, timeout_ms=10000):
        # 等到該孔的速度斜坡走完。先等一個週期讓指令生效，避免讀到下指令前的舊狀態。
        # Wait for the ramp to finish (after one poll period so the command has landed).
        _check_port(port_num)
        time.sleep_ms(self._throttle_ms)
        t0 = time.ticks_ms()
        while time.ticks_diff(time.ticks_ms(), t0) < timeout_ms:
            if not (self._port_reg(port_num) & _BIT_MOVING):
                return
            time.sleep_ms(10)

    def servo_calibrate(self, port_num, min_us=500, max_us=2500, range_deg=180):
        # 設定脈寬與角度的對應（10µs / 10° 為單位）。min_us 0~1000、max_us 1500~2500、range_deg 10~1000。
        self._send(*_build_calibrate(port_num, min_us, max_us, range_deg))

    # ════════════════════════════════════════════════════════
    #  速度模式 / Speed mode（360° 連續旋轉舵機）
    # ════════════════════════════════════════════════════════
    def servo_run(self, port_num, speed):
        # speed -100~100，0 = 停止。實際方向依舵機而定。
        _check_port(port_num)
        stop_us, span_us = self._speed_cfg[port_num]
        us = stop_us + _clamp(speed, -100, 100) * span_us // 100
        self._send(*_build_pulse(port_num, us))

    def servo_speed_config(self, port_num, stop_us=1500, span_us=1000):
        # 速度模式的停止脈寬與滿速偏移量。舵機在 speed=0 仍緩慢轉動時，微調 stop_us。
        _check_port(port_num)
        self._speed_cfg[port_num] = (_clamp(stop_us, 500, 2500), _clamp(span_us, 0, 1000))

    # ════════════════════════════════════════════════════════
    #  共用 / Common
    # ════════════════════════════════════════════════════════
    def servo_pulse(self, port_num, pulse_us):
        # 直接輸出脈寬 500~2500µs。
        self._send(*_build_pulse(port_num, pulse_us))

    def servo_release(self, port_num):
        # 停止輸出脈衝，舵機放鬆；同時清除該孔的堵轉旗標。
        self._send(*_build_release(port_num))

    def stop_all(self):
        # 6 孔全部放鬆（保留校正 / 堵轉設定）。
        for p in range(1, 7):
            self._send(*_build_release(p))

    def servo_stall_config(self, port_num, current_ma=400, time_ms=300, auto_release=True):
        # 堵轉保護：電流超過 current_ma 持續 time_ms 判定堵轉。current_ma=0 關閉偵測。
        # 堵轉放鬆後要重新上力：送不同的角度，或先 servo_release 再送。
        self._send(*_build_stall(port_num, current_ma, time_ms, auto_release))

    # ── 讀取 / Readings ─────────────────────────────────────────
    def get_current(self, port_num):
        return self._port_reg(port_num) & 0x0FFF

    def is_stalled(self, port_num):
        return bool(self._port_reg(port_num) & _BIT_STALL)

    def is_moving(self, port_num):
        return bool(self._port_reg(port_num) & _BIT_MOVING)

    def is_active(self, port_num):
        return bool(self._port_reg(port_num) & _BIT_ACTIVE)

    def get_total_current(self):
        return self._regs()[6] & 0x0FFF

    def is_power_fault(self):
        return bool(self._regs()[6] & _BIT_FLT)


# ── 函式式 API（操作最後建立的板子）/ Function-style API (last created board) ──
def servo_angle(*a):         return _default_instance().servo_angle(*a)
def servo_wait(*a):          return _default_instance().servo_wait(*a)
def servo_calibrate(*a):     return _default_instance().servo_calibrate(*a)
def servo_run(*a):           return _default_instance().servo_run(*a)
def servo_speed_config(*a):  return _default_instance().servo_speed_config(*a)
def servo_pulse(*a):         return _default_instance().servo_pulse(*a)
def servo_release(*a):       return _default_instance().servo_release(*a)
def stop_all():              return _default_instance().stop_all()
def servo_stall_config(*a):  return _default_instance().servo_stall_config(*a)
def get_current(*a):         return _default_instance().get_current(*a)
def is_stalled(*a):          return _default_instance().is_stalled(*a)
def is_moving(*a):           return _default_instance().is_moving(*a)
def is_active(*a):           return _default_instance().is_active(*a)
def get_total_current():     return _default_instance().get_total_current()
def is_power_fault():        return _default_instance().is_power_fault()
