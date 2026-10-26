# ════════════════════════════════════════════════════════════════
#  SPIKE 六路 Servo 擴充板 Python 函式庫（Pybricks 版）
#  6-Port Servo Expansion Board Library for Pybricks SPIKE Prime
#  Author: MBC robotics    Date: 2026.10
#  協議 / Protocol: Servo擴充板_上下行協議.md (v0.2)
# ════════════════════════════════════════════════════════════════
#
#  快速上手 / Quick start
#  ────────────────────────────────────────────────────────────
#      from MBC_servo6_Lib import MBC_SERVO6
#      sv = MBC_SERVO6(port, multitask)
#
#  port：1~6，對應 Port.A~F。 / port: 1-6, maps to Port.A~F.
#
#  ① multitask=False（同步 sync）— 直接呼叫，不用 await
#         sv.servo_angle(1, 90)
#         sv.servo_run(2, 50)
#         sv.stop_all()            # 程式結束前放鬆所有舵機 / release all before exit
#
#  ② multitask=True（非同步 async）— 每個呼叫前加 await，用 run_task(main()) 執行。
#     要「結束時自動放鬆所有舵機」，請這樣匯入（排在 pybricks.tools 之後）：
#     For auto-release on exit, import run_task like this (AFTER pybricks.tools):
#         from pybricks.tools import run_task
#         from MBC_servo6_Lib import MBC_SERVO6, run_task
#         async def main():
#             await sv.servo_angle(1, 90)
#         run_task(main())
#
#  ⚠ 第一次執行會在主控台看到：
#      "Custom electronics may damage your LEGO hub ... Press Y to proceed."
#    請按 Y（只需一次，之後會記住）。這是 Pybricks 要讓擴充板從 M1/M2 取得舵機電源的
#    安全確認；按 N 舵機不會有電。需要 Pybricks 韌體 v4.0 以上。
#  ⚠ On first run the console asks "Press Y to proceed." — press Y once (remembered).
#    It lets the hub power the servos through M1/M2. Requires Pybricks firmware v4.0+.
#
#  兩種舵機模式 / Two servo modes
#  ────────────────────────────────────────────────────────────
#      定點模式 / position : servo_angle(port, angle, speed)
#      速度模式 / speed    : servo_run(port, speed)   ← 360° 連續旋轉舵機
#
#  不需要心跳 / keep_alive：擴充板沒有看門狗。
#  No heartbeat / keep_alive needed: the board has no watchdog.
# ════════════════════════════════════════════════════════════════
from pybricks.iodevices import UARTDevice
from pybricks.parameters import Port
from pybricks.tools import StopWatch as _StopWatch
from pybricks.tools import wait
from pybricks.tools import run_task as _pb_run_task

# ── raw UART 協議常數 ──────────────────────────────────────
_CMD_HEADER     = 0xA5      # 下行 [0xA5,L1,L2,L3,L4,xor]
_STREAM_HEADER  = 0x5A      # 上行 [0x5A][16][R1~R8 int16 LE][xor]
_STREAM_PAYLOAD = 16
_BAUD           = 115200
_POWER_PIN_P1   = 1         # 電池電壓送到 P1(M1)；板上有整流橋，極性不拘
_BOARD_ID       = 0x53      # R8 bit8~14
_MUTINY_PKT     = bytes([0xA5, 0x01, 0x00, 0x00, 0x00, 0xA4])   # L1=1 空操作，喚醒板子切 RAW

_PROBE_TIMEOUT_MS  = 1000   # 開機偵測擴充板的時限
_PROBE_KICK_MS     = 50     # 偵測期間補送策反包的間隔
_RECONNECT_KICK_MS = 100    # 未連線時補送策反包的間隔
_DISCONNECT_MS     = 400    # 超過此時間沒收到上行串流 → 判定斷線（板子每 10ms 送一包）
_ERR_REPEAT_MS     = 1000   # 未連線提示限流
_CACHE_MS          = 10     # 讀取快取有效時間（= 串流週期）
_STALE_MS          = 50     # 超過這麼久沒讀，UART 緩衝裡可能堆著舊封包 → 清掉只收新的
_FRESH_TIMEOUT_MS  = 40     # 等新封包的上限（板子每 10ms 送一包）
_CMD_SETTLE_MS     = 15     # 下指令後等板子處理完、送出反映新狀態的封包
_RELINK_WAIT_MS    = 60     # 下指令時發現斷線：送完策反包後最多等這麼久看板子有沒有回來
_STOP_REPEAT       = 3      # 結束時全部放鬆指令重送次數

# 上行狀態位元 / status bits in R1~R6
_BIT_STALL  = 1 << 12
_BIT_MOVING = 1 << 13
_BIT_ACTIVE = 1 << 14
# R7
_BIT_FLT = 1 << 12
_BIT_EN  = 1 << 13

_PORT_MAP = {1: Port.A, 2: Port.B, 3: Port.C, 4: Port.D, 5: Port.E, 6: Port.F}
_PORT_LETTER = {1: 'A', 2: 'B', 3: 'C', 4: 'D', 5: 'E', 6: 'F'}

_watch = _StopWatch()


# ── 參數驗證 / Parameter validation ───────────────────────────
# _check_* 拋 ValueError：孔位錯了會打到別的指令，沒有安全預設可退。
# _clamp   不拋錯：數值夾到範圍邊界就是合理結果（全速、最大角度…）。
def _check_port(port_num, name="port"):
    if isinstance(port_num, bool) or not isinstance(port_num, int) or not (1 <= port_num <= 6):
        raise ValueError("%s=%r 不是有效孔位，必須是 1~6 的整數 / must be an int 1~6" % (name, port_num))

def _clamp(v, lo, hi):
    v = int(round(v))
    return lo if v < lo else (hi if v > hi else v)

def _cmd_packet(l1, l2, l3, l4):
    x = _CMD_HEADER ^ l1 ^ l2 ^ l3 ^ l4
    return bytes([_CMD_HEADER, l1, l2, l3, l4, x])


# ── 封包建構（純函式，參數已驗證）/ Packet builders ─────────────
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


_instances = []

def _default_instance():
    if not _instances:
        raise RuntimeError("請先建立擴充板物件 / Create the board first: sv = MBC_SERVO6(port, multitask)")
    return _instances[-1]

def run_task(coro):
    # Pybricks run_task 的包裝：主程式結束 / 報錯 / 按停止鍵時，放鬆所有舵機。
    # Wraps Pybricks run_task: releases all servos when main() ends, errors, or is stopped.
    try:
        return _pb_run_task(coro)
    finally:
        for inst in _instances:
            inst._final_stop()


class MBC_SERVO6:

    def __init__(self, port, multitask=False):
        if port not in _PORT_MAP:
            raise ValueError("port 必須為 1~6（對應 Port.A~F），收到：{} / port must be 1~6 (Port.A~F)".format(port))
        self._port = port
        self._async_mode = bool(multitask)
        self._rx = bytearray()
        self._cache = None
        self._cache_time = 0
        self._pkt_seq = 0          # 每解析到一包新封包 +1，用來判斷「清空緩衝後是否收到新包」
        self._uart_busy = False
        self._err_last_ms = -_ERR_REPEAT_MS
        self._reconnect_last_ms = -_RECONNECT_KICK_MS
        # 使用者設定的校正 / 堵轉參數：熱插拔重連時重送給板子
        self._calib = {}
        self._stall = {}
        # 速度模式：停止脈寬、滿速偏移量（µs）
        self._speed_cfg = {p: (1500, 1000) for p in range(1, 7)}

        letter = _PORT_LETTER[port]
        self._err_msg = "[SERVO6] Port {L}（孔{n}）未偵測到擴充板 / No servo board detected".format(L=letter, n=port)
        self._reconn_msg = "[SERVO6] Port {L}（孔{n}）已重新連線 / Servo board reconnected".format(L=letter, n=port)
        self._disc_msg = "[SERVO6] Port {L}（孔{n}）連線中斷 / Servo board disconnected".format(L=letter, n=port)

        try:
            self._uart = UARTDevice(_PORT_MAP[port], baudrate=_BAUD, power_pin=_POWER_PIN_P1)
        except TypeError:
            raise RuntimeError("需要 Pybricks 韌體 v4.0 以上才能供電給舵機 / Pybricks firmware v4.0+ required")
        self._uart.clear()
        self._uart.write(_MUTINY_PKT)

        self.connected = self._probe_board()
        if self.connected:
            r8 = self._reg(7)
            if (r8 >> 8) & 0x7F != _BOARD_ID:
                print("[SERVO6] Port {} 插的不是 Servo 擴充板（板型 0x{:02X}）/ Not a servo board".format(
                    letter, (r8 >> 8) & 0x7F))
            # 清掉上一輪程式殘留的狀態：全部放鬆、校正/堵轉設定回預設
            self._send_sync(0, 0, 0, 0)
        else:
            self._warn_no_board()
        _instances.append(self)

    # ── 內部：連線 ─────────────────────────────────────────────
    def _probe_board(self, timeout_ms=_PROBE_TIMEOUT_MS):
        # 每 _PROBE_KICK_MS 補送策反包，收到任一合法上行封包即視為擴充板存在。
        self._rx = bytearray()
        self._cache = None
        t0 = _watch.time()
        last_kick = t0 - _PROBE_KICK_MS
        while _watch.time() - t0 < timeout_ms:
            if _watch.time() - last_kick >= _PROBE_KICK_MS:
                self._uart.write(_MUTINY_PKT)
                last_kick = _watch.time()
            self._drain_and_parse()
            if self._cache is not None:
                return True
            wait(5)
        return False

    def _warn_no_board(self):
        now = _watch.time()
        if now - self._err_last_ms >= _ERR_REPEAT_MS:
            print(self._err_msg)
            self._err_last_ms = now

    def _resend_config(self):
        pkts = [_build_calibrate(p, *c) for p, c in self._calib.items()]
        pkts += [_build_stall(p, *s) for p, s in self._stall.items()]
        return pkts

    def _try_reconnect_sync(self):
        now = _watch.time()
        if now - self._reconnect_last_ms >= _RECONNECT_KICK_MS:
            try:
                self._uart.write(_MUTINY_PKT)
            except OSError:
                pass
            self._reconnect_last_ms = now
        self._drain_and_parse()
        if self._cache is None:
            return False
        self.connected = True
        print(self._reconn_msg)
        for pkt in self._resend_config():
            self._send_sync(*pkt)
        return True

    async def _try_reconnect_async(self):
        now = _watch.time()
        if now - self._reconnect_last_ms >= _RECONNECT_KICK_MS:
            await self._uart_write_async(_MUTINY_PKT)
            self._reconnect_last_ms = now
        self._drain_and_parse()
        if self._cache is None:
            return False
        self.connected = True
        print(self._reconn_msg)
        for pkt in self._resend_config():
            await self._send_async(*pkt)
        return True

    def _check_disconnect(self):
        # 只在「剛 drain 過」之後呼叫：串流中的板子會更新 _cache_time。
        if self.connected and (_watch.time() - self._cache_time) > _DISCONNECT_MS:
            self._mark_disconnected()

    def _mark_disconnected(self):
        self.connected = False
        self._cache = None   # 清掉舊快取，否則重連判斷會被這包舊資料騙過
        self._reconnect_last_ms = -_RECONNECT_KICK_MS
        self._err_last_ms = _watch.time()
        print(self._disc_msg)

    # 只下指令、從不讀值的程式也要發現斷線：板子插回去會重開、停在樂高模式等策反包，
    # 設定也回到預設。所以下指令前，若一陣子沒收到串流，就確認一次板子還在送；
    # 沒在送 → 判定斷線、馬上送策反包重連，連上後 _try_reconnect_* 會重送校正／堵轉設定。
    def _link_sync(self):
        if self.connected and _watch.time() - self._cache_time > _STALE_MS:
            seq = self._flush_rx()
            t0 = _watch.time()
            while self._pkt_seq == seq and _watch.time() - t0 < _FRESH_TIMEOUT_MS:
                wait(2)
                self._drain_and_parse()
            if self._pkt_seq == seq:
                self._mark_disconnected()
        if self.connected:
            return True
        kicked = _watch.time() - self._reconnect_last_ms >= _RECONNECT_KICK_MS
        t0 = _watch.time()
        while True:
            if self._try_reconnect_sync():
                return True
            if not kicked or _watch.time() - t0 >= _RELINK_WAIT_MS:
                return False
            wait(5)

    async def _link_async(self):
        if self.connected and _watch.time() - self._cache_time > _STALE_MS:
            seq = self._flush_rx()
            t0 = _watch.time()
            while self._pkt_seq == seq and _watch.time() - t0 < _FRESH_TIMEOUT_MS:
                await wait(2)
                self._drain_and_parse()
            if self._pkt_seq == seq:
                self._mark_disconnected()
        if self.connected:
            return True
        kicked = _watch.time() - self._reconnect_last_ms >= _RECONNECT_KICK_MS
        t0 = _watch.time()
        while True:
            if await self._try_reconnect_async():
                return True
            if not kicked or _watch.time() - t0 >= _RELINK_WAIT_MS:
                return False
            await wait(5)

    # ── 內部：raw UART 收發 ────────────────────────────────────
    async def _uart_write_async(self, data):
        while self._uart_busy:
            await wait(0)
        self._uart_busy = True
        try:
            await self._uart.write(data)
        finally:
            self._uart_busy = False

    def _send_sync(self, l1, l2, l3, l4):
        self._uart.write(_cmd_packet(l1, l2, l3, l4))

    async def _send_async(self, l1, l2, l3, l4):
        await self._uart_write_async(_cmd_packet(l1, l2, l3, l4))

    def _drain_and_parse(self):
        data = self._uart.read_all()
        if data:
            self._rx.extend(data)
        rx = self._rx
        n = len(rx)
        i = 0
        latest = None
        while True:
            while i < n and rx[i] != _STREAM_HEADER:
                i += 1
            if n - i < 2:
                break
            if rx[i + 1] != _STREAM_PAYLOAD:
                i += 1
                continue
            total = 2 + _STREAM_PAYLOAD + 1
            if n - i < total:
                break
            x = 0
            for k in range(i + 1, i + 2 + _STREAM_PAYLOAD):
                x ^= rx[k]
            # 雙重確認：checksum 只有 1 byte，資料中巧合出現 0x5A 可能誤判；
            # 真封包首尾相連，緊接的下一 byte 也必須是同步頭（或已到緩衝區結尾）。
            next_i = i + total
            if x == rx[i + total - 1] and (next_i >= n or rx[next_i] == _STREAM_HEADER):
                latest = bytes(rx[i + 2:i + 2 + _STREAM_PAYLOAD])
                i = next_i
            else:
                i += 1
        self._rx = bytearray(rx[i:])
        if latest is not None:
            self._cache = latest
            self._cache_time = _watch.time()
            self._pkt_seq += 1

    def _flush_rx(self):
        # 清掉 UART 緩衝與半包，回傳目前封包序號；之後序號變了才代表收到清空後的新封包。
        # 保留 _cache：等不到新包時仍可回傳最後已知值，不會突然變 0。
        self._uart.clear()
        self._rx = bytearray()
        return self._pkt_seq

    def _read(self):
        now = _watch.time()
        if self._cache is not None and (now - self._cache_time) < _CACHE_MS:
            return self._cache
        if self._cache is None or (now - self._cache_time) > _STALE_MS:
            seq = self._flush_rx()
            t0 = _watch.time()
            while self._pkt_seq == seq and _watch.time() - t0 < _FRESH_TIMEOUT_MS:
                wait(2)
                self._drain_and_parse()
        else:
            self._drain_and_parse()
        self._check_disconnect()
        return self._cache

    async def _read_async(self):
        now = _watch.time()
        if self._cache is not None and (now - self._cache_time) < _CACHE_MS:
            return self._cache
        if self._cache is None or (now - self._cache_time) > _STALE_MS:
            seq = self._flush_rx()
            t0 = _watch.time()
            while self._pkt_seq == seq and _watch.time() - t0 < _FRESH_TIMEOUT_MS:
                await wait(2)
                self._drain_and_parse()
        else:
            self._drain_and_parse()
        self._check_disconnect()
        return self._cache

    def _reg(self, idx):
        c = self._cache
        return (c[2 * idx] | (c[2 * idx + 1] << 8)) if c else 0

    def _final_stop(self):
        # 關機善後：直接寫 UART 並吞掉 EBUSY 重試（async 寫入被取消時底層可能短暫殘留 busy）。
        pkt = _cmd_packet(0, 0, 0, 0)
        sent = guard = 0
        while sent < _STOP_REPEAT and guard < _STOP_REPEAT + 30:
            guard += 1
            try:
                self._uart.write(pkt)
                sent += 1
                wait(5)
            except OSError:
                wait(2)

    # ── 內部：sync / async 共用派工 ─────────────────────────────
    # 參數驗證在 _cmd 裡先做完（不論走 sync 或 async），async 模式一定回傳 coroutine。
    def _cmd(self, pkts):
        if self._async_mode:
            return self._cmd_async(pkts)
        if not self._link_sync():
            self._warn_no_board()
            return None
        for pkt in pkts:
            self._send_sync(*pkt)
        return None

    async def _cmd_async(self, pkts):
        if not await self._link_async():
            self._warn_no_board()
            return
        for pkt in pkts:
            await self._send_async(*pkt)

    def _get(self, fn):
        if self._async_mode:
            return self._get_async(fn)
        if not self.connected and not self._try_reconnect_sync():
            self._warn_no_board()
            return fn(None)
        return fn(self._read())

    async def _get_async(self, fn):
        if not self.connected and not await self._try_reconnect_async():
            self._warn_no_board()
            return fn(None)
        return fn(await self._read_async())

    def _port_reg(self, port_num):
        _check_port(port_num)
        return lambda c: self._reg(port_num - 1) if c else 0

    # ════════════════════════════════════════════════════════
    #  定點模式 / Position mode
    # ════════════════════════════════════════════════════════
    def servo_angle(self, port, angle, speed=100):
        # 轉到指定角度。speed 1~99 = 速度×6 °/s（漸進），100 = 立即到位（舵機原生速度）。
        # 剛上電或放鬆後的第一次角度指令一律直接到位（板子不知道舵機實際位置）。
        # Go to angle. speed 1~99 = speed×6 °/s ramp, 100 = jump at native servo speed.
        return self._cmd([_build_angle(port, angle, speed)])

    def servo_wait(self, port, timeout_ms=10000):
        # 等到該孔的速度斜坡走完（speed=100 立即到位時不需等）。
        # Wait until the port's speed ramp finishes.
        # 先等板子處理完剛送的指令，再清空緩衝：只採用清空後收到的封包，
        # 否則會讀到下指令前的舊狀態（漸進中位元還是 0）而立刻返回。
        _check_port(port)
        if self._async_mode:
            return self._servo_wait_async(port, timeout_ms)
        wait(_CMD_SETTLE_MS)
        seq = self._flush_rx()
        t0 = _watch.time()
        while _watch.time() - t0 < timeout_ms:
            if not self.connected and not self._try_reconnect_sync():
                self._warn_no_board()
                return
            self._drain_and_parse()
            self._check_disconnect()   # 拔線時約 0.4 秒就返回，不會等滿 timeout
            if self._pkt_seq != seq and not (self._reg(port - 1) & _BIT_MOVING):
                return
            wait(10)

    async def _servo_wait_async(self, port, timeout_ms):
        await wait(_CMD_SETTLE_MS)
        seq = self._flush_rx()
        t0 = _watch.time()
        while _watch.time() - t0 < timeout_ms:
            if not self.connected and not await self._try_reconnect_async():
                self._warn_no_board()
                return
            self._drain_and_parse()
            self._check_disconnect()   # 拔線時約 0.4 秒就返回，不會等滿 timeout
            if self._pkt_seq != seq and not (self._reg(port - 1) & _BIT_MOVING):
                return
            await wait(10)

    def servo_calibrate(self, port, min_us=500, max_us=2500, range_deg=180):
        # 設定脈寬與角度的對應（10µs / 10° 為單位）。min_us 0~1000、max_us 1500~2500、range_deg 10~1000。
        # Map pulse width to angle (10 µs / 10° steps).
        pkt = _build_calibrate(port, min_us, max_us, range_deg)
        self._calib[port] = (min_us, max_us, range_deg)
        return self._cmd([pkt])

    # ════════════════════════════════════════════════════════
    #  速度模式 / Speed mode（360° 連續旋轉舵機）
    # ════════════════════════════════════════════════════════
    def servo_run(self, port, speed):
        # speed -100~100，0 = 停止。實際方向依舵機而定。
        # speed -100..100, 0 = stop. Actual direction depends on the servo.
        _check_port(port)
        stop_us, span_us = self._speed_cfg[port]
        us = stop_us + _clamp(speed, -100, 100) * span_us // 100
        return self._cmd([_build_pulse(port, us)])

    def servo_speed_config(self, port, stop_us=1500, span_us=1000):
        # 速度模式的停止脈寬與滿速偏移量。舵機在 speed=0 仍緩慢轉動時，微調 stop_us。
        # Stop pulse and full-speed offset for speed mode; tune stop_us if the servo creeps at 0.
        _check_port(port)
        self._speed_cfg[port] = (_clamp(stop_us, 500, 2500), _clamp(span_us, 0, 1000))
        return self._cmd([]) if self._async_mode else None

    # ════════════════════════════════════════════════════════
    #  共用 / Common
    # ════════════════════════════════════════════════════════
    def servo_pulse(self, port, pulse_us):
        # 直接輸出脈寬 500~2500µs。 / Output a raw pulse width, 500-2500 µs.
        return self._cmd([_build_pulse(port, pulse_us)])

    def servo_release(self, port):
        # 停止輸出脈衝，舵機放鬆；同時清除該孔的堵轉旗標。
        # Stop the pulse (servo goes limp) and clear the port's stall flag.
        return self._cmd([_build_release(port)])

    def stop_all(self):
        # 6 孔全部放鬆（保留校正 / 堵轉設定）。 / Release all 6 ports (keeps settings).
        return self._cmd([_build_release(p) for p in range(1, 7)])

    def servo_stall_config(self, port, current_ma=400, time_ms=300, auto_release=True):
        # 堵轉保護：電流超過 current_ma 持續 time_ms 判定堵轉。current_ma=0 關閉偵測。
        # auto_release=True 時自動放鬆。堵轉放鬆後要重新上力：送不同的角度，或先 servo_release 再送。
        # Stall protection: current > current_ma for time_ms → stalled. current_ma=0 disables.
        # After an auto-release, send a different command (or servo_release first) to re-arm.
        pkt = _build_stall(port, current_ma, time_ms, auto_release)
        self._stall[port] = (current_ma, time_ms, auto_release)
        return self._cmd([pkt])

    # ── 讀取 / Readings ─────────────────────────────────────────
    def get_current(self, port):
        # 該孔電流 mA。 / Port current in mA.
        f = self._port_reg(port)
        return self._get(lambda c: f(c) & 0x0FFF)

    def is_stalled(self, port):
        f = self._port_reg(port)
        return self._get(lambda c: bool(f(c) & _BIT_STALL))

    def is_moving(self, port):
        # 速度斜坡尚未走完。 / Speed ramp still in progress.
        f = self._port_reg(port)
        return self._get(lambda c: bool(f(c) & _BIT_MOVING))

    def is_active(self, port):
        # 該孔正在輸出脈衝（舵機有出力）。 / Port is outputting pulses.
        f = self._port_reg(port)
        return self._get(lambda c: bool(f(c) & _BIT_ACTIVE))

    def get_total_current(self):
        # 6 孔總電流 mA（eFuse 量測，限流約 750mA）。 / Total current in mA (eFuse limit ≈ 750 mA).
        return self._get(lambda c: self._reg(6) & 0x0FFF if c else 0)

    def is_power_fault(self):
        # eFuse 故障（過流 / 過熱，會自動重試）。 / eFuse fault (over-current / thermal, auto-retry).
        return self._get(lambda c: bool(self._reg(6) & _BIT_FLT) if c else False)


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
