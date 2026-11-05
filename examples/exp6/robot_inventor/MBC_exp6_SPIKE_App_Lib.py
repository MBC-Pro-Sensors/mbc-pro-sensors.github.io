# Python Library for EXP6 Smart Expansion Board for LEGO MINDSTORMS Robot Inventor (51515)
# Author : MBC robotics
#
# !! 此函式庫專供 LEGO MINDSTORMS Robot Inventor 10.x／SPIKE App 2.x（玩具版）使用 !!
# !! This library is designed for MINDSTORMS Robot Inventor 10.x / SPIKE App 2.x !!
#
# ⚠ 這個檔由 05_上機測試\實際App測試\make_toy_lib.py 從教育版正本自動產生，不要手改；要改請改教育版再重新產生。
# ⚠ Generated from the SPIKE App 3 (education) library by make_toy_lib.py. Do not edit by hand.
#
# 安裝：在 App 執行一次 Install_MBC_exp6_Lib.py；程式用 from projects.mbc_exp6.MBC_exp6_SPIKE_App_Lib import *
# 需搭配 EXP6 韌體 2026-11-03 以後。函式名稱、用法跟教育版完全一樣。
# 通訊走原始協定（跟 Pybricks 版同一套）：玩具版主機的樂高模式會把 0~2500 以外的讀值變成 None，所以孔切成全雙工 UART。
#   心跳由背景計時器自動送；函式庫 10 秒沒被呼叫就停心跳，EXP6 看門狗會讓馬達停（程式結束後忘了停的馬達不會一直轉）。
#   要等超過 10 秒、馬達又要一直轉，請用 exp.keep_alive_wait(ms)。EXP6 進入原始模式後，要拔電池重開才回樂高模式。
# Uses the raw protocol (same as the Pybricks library): in LEGO mode the toy hub turns readings outside 0~2500 into None.
#   A background timer sends the heartbeat; after 10 s without library calls it stops, so motors stop after the program ends.
#   Use exp.keep_alive_wait(ms) for longer waits with motors running. Power-cycle EXP6 to return it to LEGO mode.
#   電池超過 9 V（比 2S 鋰電池高）時 EXP6 韌體會停止對樂高顏色／超音波感應器供電，函式庫會印一次中英文提示。

_instances = []

import hub, utime as time
from machine import Timer
from hub import port

_CMD_HEADER     = 0xA5    # 下行 [0xA5, L1, L2, L3, L4, xor]
_STREAM_HEADER  = 0x5A    # 上行 [0x5A, 32, R1~R8 各 int32 LE, xor]
_STREAM_PAYLOAD = 32
_HB_PKT         = bytes([0xA5, 0x01, 0x00, 0x00, 0x00, 0xA4])   # L1=1 心跳（只餵看門狗）
_HB_PERIOD_MS   = 100     # 背景心跳週期（EXP6 看門狗 500 ms）
_HB_IDLE_MS     = 10000   # 函式庫超過這麼久沒被呼叫就停心跳 → 看門狗讓馬達停；要等更久請用 keep_alive_wait(ms)
_STALE_MS       = 25      # 超過這麼久沒讀，主機緩衝區（約 139 bytes）裡就是舊資料 → 清掉等新的
_FRESH_MS       = 40      # 清掉之後最多等這麼久拿新的一包（板子每 10 ms 送一包）
_CONNECT_MS     = 3000    # 建物件時等 EXP6 回資料的上限

_hb_timer = None          # 全部 EXP6 物件共用一個背景心跳計時器


def _hb_tick(t):
    now = time.ticks_ms()
    for e in _instances:
        try:
            if time.ticks_diff(now, e._last_use_ms) > _HB_IDLE_MS:
                continue            # 主程式很久沒用函式庫（多半已經結束）：不再替它維持連線
            if time.ticks_diff(now, e._last_tx_ms) >= _HB_PERIOD_MS - 10:
                e._uart.write(_HB_PKT)
                e._last_tx_ms = time.ticks_ms()
        except Exception:
            pass


def _start_heartbeat():
    global _hb_timer
    if _hb_timer is None:
        _hb_timer = Timer(-1)
        _hb_timer.init(period=_HB_PERIOD_MS, mode=Timer.PERIODIC, callback=_hb_tick)


def _stop_heartbeat():
    global _hb_timer
    if _hb_timer is not None:
        try:
            _hb_timer.deinit()
        except Exception:
            pass
        _hb_timer = None


def _mbc_cls(name):
    # MBC 感應器類別：單一檔案時就在這裡；安裝到主機時拆成另外的檔，用到才載入（主機記憶體小，一次只編譯一小份）：
    #   MBC_exp6_mbc（循行者、測距者、掌控者）、MBC_exp6_cam（神攝手）
    g = globals()
    if name in g:
        return g[name]
    import gc, sys
    mod = 'projects.mbc_exp6.' + ('MBC_exp6_cam' if name == 'EXP6_UNICAM' else 'MBC_exp6_mbc')
    for _ in range(2):
        gc.collect()
        m = __import__(mod, None, None, ('x',))
        if hasattr(m, name):
            return getattr(m, name)
        # 上次載入到一半就記憶體不夠，主機留著半成品模組（11-05 上機 AttributeError）→ 丟掉重載
        sys.modules.pop(mod, None)
    raise MemoryError('EXP6：載入 %s 時記憶體不夠，請把程式縮小或重開主機 / not enough memory to load %s' % (mod, mod))


def _cmd_packet(l1, l2, l3, l4):
    return bytes([_CMD_HEADER, l1, l2, l3, l4, _CMD_HEADER ^ l1 ^ l2 ^ l3 ^ l4])

def _default_instance():
    if not _instances:
        raise RuntimeError("請先建立擴充板物件 (例: exp = MBC_EXP6(port.F)) / Create the board first, e.g. exp = MBC_EXP6(port.F)")
    return _instances[-1]

DEV_NONE = 0
DEV_FORCE = 1
DEV_COLOR = 3          # 跟韌體／Pybricks 庫一致（get_device_id 回傳的顏色感應器代碼）
DEV_ULTRASONIC = 4
DEV_MOTOR = 5

DIR_HOLD = 0
DIR_CW = 1
DIR_CCW = 2
DIR_COAST = 3
DIR_BRAKE = 4

# 停止狀態（motor_stop / stop_all / motor_run_degrees / motor_track_target 的必填參數；
# 可傳字串常數或整數碼 1~4，大小寫不拘）
# 整數碼(1-based，與 Pybricks 庫一致)：1=coast 滑行、2=brake 煞車、3=hold 鎖死、4=continue 續轉/維持現狀。
# continue 在不同函式家族的意義不同：
#   - motor_run_degrees/motor_track_target(走角度)：到位後不切換停止模式，維持原速度續轉。
#   - motor_stop/drive_stop/stop_all(純停止指令)：不送出任何指令，讓馬達維持呼叫前的狀態
#     (還在轉就繼續轉、已經停了就繼續停著)——這是刻意合法的選項，不是無法辨識的錯誤。
# ⚠ 傳入真的無法辨識的值（範圍外的整數、亂打的字串等）才會印一行提示、自動改用 brake
# 煞車，不會中斷程式──brake 介於「滑行(可能讓機構掉落)」與「鎖死(可能卡住機構)」之間，
# 是誤用時最安全的預設。
STOP_COAST    = "coast"     # 滑行           (= 1)
STOP_BRAKE    = "brake"     # 被動煞車       (= 2)（傳 "break" 亦視為 brake）
STOP_HOLD     = "hold"      # 位置鎖死       (= 3)
STOP_CONTINUE = "continue"  # 走角度：到位後續轉；純停止指令：不送指令、維持現狀 (= 4)

# 停止狀態字串 → 韌體協議碼。兩種情境的編碼不同：
#   motor_stop / stop_all：單馬達指令 L2（coast=3, hold=4, brake=5）
#   motor_run_degrees：走角度 Method C 的 stop 欄位（coast=0, brake=1, hold=2, continue=3）
_STOP_L2         = {"coast": 3, "hold": 4, "brake": 5}
_STOP_COMPLETION = {"coast": 0, "brake": 1, "hold": 2, "continue": 3}
# 整數碼 → 停止狀態名。採 1-based（與 Pybricks 庫一致，跟孔號 1~6 風格相同，較直覺）：
#   1=coast 滑行、2=brake 煞車、3=hold 鎖死、4=continue 續轉（continue 僅走角度可用）。
# （韌體走角度 Method C 內部仍是 0-based，_stop_to_completion 會做轉換，使用者不用管。）
# 讓使用者傳字串或整數皆可；早期只吃字串時，傳整數會被 str() 成 "1" 之類、查無此鍵而
# 靜默退回預設（走角度→hold、停止→coast），造成「四種停止狀態全部沒作用」的假象。
_STOP_INT        = {1: "coast", 2: "brake", 3: "hold", 4: "continue"}
_VALID_STOP_NAMES = ("coast", "brake", "hold", "continue")

def _norm_stop(stop):
    # 接受 STOP_* 字串常數、"coast"/"brake"/"hold"/"continue"（大小寫不拘、"break" 亦當 brake），
    # 或整數碼 1~4（含數字字串 "1"~"4"）。任何無法辨識的值印一行提示、改用 brake 煞車，不拋錯──
    # brake 是誤用時最安全的中間狀態(見上方 STOP_* 常數區塊說明)，讓程式能繼續跑、不因打錯
    # 一個停止碼就整段程式中斷。
    if isinstance(stop, bool):        # True/False 先轉 1/0，避免被 str() 成 "true"/"false"
        stop = int(stop)
    if isinstance(stop, int):
        if stop in _STOP_INT:
            return _STOP_INT[stop]
    else:
        s = str(stop).strip().lower()
        if s == "break":
            s = "brake"
        if s in ("1", "2", "3", "4"):     # 數字字串比照整數碼（1-based）
            return _STOP_INT[int(s)]
        if s in _VALID_STOP_NAMES:
            return s
    print("[EXP6] stop=%r 不符合定義範圍(1:coast, 2:brake, 3:hold, 4:continue)，"
          "已改用 brake 模式 / invalid stop value, using brake" % (stop,))
    return "brake"

def _stop_to_l2(stop):
    # motor_stop / drive_stop / stop_all 共用：continue 代表「維持現狀、不送指令」，回傳
    # None 讓呼叫端跳過整次傳送(不當成無法辨識的錯誤處理，也不印提示)。coast/brake/hold
    # 三個合法名稱 _norm_stop 保證都在 _STOP_L2 裡，不會有 KeyError。
    name = _norm_stop(stop)
    if name == "continue":
        return None
    return _STOP_L2[name]

def _stop_to_completion(stop):
    return _STOP_COMPLETION[_norm_stop(stop)]

MODE_SPEED = 0
MODE_ANGLE = 1
MODE_ABS_ANGLE = 2
MODE_RESET = 3
MODE_COLOR = 10
MODE_RGB = 11
MODE_HSV = 12
MODE_DISTANCE = 20
MODE_FORCE = 30

# ── 參數驗證副程式 / Parameter validation helpers ──────────────────
# 分兩類（與 Pybricks 版邏輯一致）：
#   _check_*  會拋 ValueError──用在「超出範圍就是靜默做錯事」的參數：孔位超界／重複會
#             撞上協議算式，別名成完全不相關的指令；mode 是離散合法值集合，沒有「最接近」
#             可猜。這三種沒有安全的預設值可以退，只能讓呼叫方立刻修正。
#   _clamp*   不拋錯──用在「夾到範圍邊界就是合理結果」的參數（speed/power/PID 倍率夾到
#             極值＝全速/全開，符合直覺）。_clamp_warn 額外印一行提示，只用在「靜默夾範圍
#             會嚴重改變行為」的參數（degrees 被截斷＝提早觸發煞車動作）。
def _check_port(port_num, name="port"):
    if isinstance(port_num, bool) or not isinstance(port_num, int) or not (1 <= port_num <= 6):
        raise ValueError("%s=%r 不是有效孔位，必須是 1~6 的整數 / must be an integer 1~6" % (name, port_num))

def _check_distinct_ports(port_a, port_b):
    if port_a == port_b:
        raise ValueError(
            "left_port 與 right_port 不能是同一孔位(%r)，會被協議的 left*10+right 算式"
            "誤解成單馬達 PID 指令(port*11) / left_port and right_port must be different ports" % (port_a,))

_VALID_MODES = (MODE_SPEED, MODE_ANGLE, MODE_ABS_ANGLE, MODE_RESET,
                MODE_COLOR, MODE_RGB, MODE_HSV, MODE_DISTANCE, MODE_FORCE)

def _check_mode(mode):
    if mode not in _VALID_MODES:
        raise ValueError("mode=%r 不是合法的 MODE_* 常數 / is not a valid MODE_* constant" % (mode,))

def _clamp(v, lo, hi, name):
    v = int(v)
    if v < lo:
        return lo
    if v > hi:
        return hi
    return v

def _clamp_warn(v, lo, hi, name):
    v = int(v)
    if v < lo or v > hi:
        c = lo if v < lo else hi
        print("[EXP6] %s=%d 超出範圍，已限制在 %d~%d 之間 / out of range, clamped" % (name, v, lo, hi))
        return c
    return v

def _cache_validated(build_fn):
    # 參數暫存器：若本次呼叫參數跟上次完全相同，直接複用上次組好的封包內容，不重新驗證、
    # 不重新做角度/PID 換算。參數不同（或這是第一次呼叫）才真正跑 build_fn；build_fn 內部
    # 驗證失敗會直接拋出，失敗的呼叫不會被寫入快取，下次同樣的錯誤參數仍會重新驗證並報錯。
    cache = [None, None]   # [上次的參數 tuple, 上次的結果]
    def wrapper(*args):
        if cache[0] == args:
            return cache[1]
        result = build_fn(*args)
        cache[0], cache[1] = args, result
        return result
    return wrapper

# ── 封包組建副程式 / Packet-building helpers ──────────────────────
# 純函式：只依參數算出 (l1,l2,l3,l4)，不碰 self、不送封包。跟 Pybricks 版的 _build_* 系列
# 對應，讓兩個庫的驗證/組包邏輯保持一致；實際傳送仍由各 method 呼叫 self._send()。
def _build_motor_run(port_num, speed):
    _check_port(port_num)
    speed = _clamp(speed, -100, 100, "speed")
    return (port_num * 11, DIR_CW if speed >= 0 else DIR_CCW, abs(speed), 0)

def _build_motor_power(port_num, power):
    _check_port(port_num)
    power = _clamp(power, -100, 100, "power")
    return (port_num * 10, DIR_CW if power >= 0 else DIR_CCW, abs(power), 0)

def _build_motor_stop(port_num, stop):
    _check_port(port_num)
    l2 = _stop_to_l2(stop)
    if l2 is None:              # stop=continue：維持現狀，不送任何指令
        return None
    return (port_num * 10, l2, 0, 0)

def _build_motor_set_pid(port_num, p, i, d):
    _check_port(port_num)
    p = _clamp(p, 0, 100, "p")
    i = _clamp(i, 0, 100, "i")
    d = _clamp(d, 0, 100, "d")
    return (80 + port_num, p, i, d)

def _build_motor_run_degrees(port_num, speed, degrees, stop):
    _check_port(port_num)
    speed = _clamp(speed, -100, 100, "speed")
    # degrees 被截斷會讓 stop 動作提早觸發(見上方 _clamp_warn 說明)，所以夾範圍時要印提示；
    # Method C：停止狀態佔用 L3 高位，故最大角度只能到 2499。
    degrees = _clamp_warn(degrees, -2499, 2499, "degrees")
    deg = abs(int(degrees))
    # 方向 = 速度正負 × 角度正負（跟 Pybricks 庫一致）：兩者符號相同→正轉(CW,8)，相異→反轉(CCW,9)。
    op = 9 if (speed < 0) != (degrees < 0) else 8
    l3 = _stop_to_completion(stop) * 25 + deg // 100   # L3 = stop*25 + 百位(0~24)
    return (port_num * 10 + op, abs(int(speed)), l3, deg % 100)

def _build_motor_track_target(port_num, speed, angle, stop):
    _check_port(port_num)
    speed = _clamp(speed, -100, 100, "speed")
    # Method C(比照 motor_run_degrees)：目標角度只到 360，百位數只需 0~3，用「除4」
    # 把停止狀態疊進 L3(stop*4 + 百位，最大 3*4+3=15，遠低於 100)。
    # angle 本質上是週期性的(540°等於180°)，直接取模即可，不需要額外驗證/夾範圍。
    t = int(angle) % 360
    l3 = _stop_to_completion(stop) * 4 + t // 100
    return (port_num * 10 + 7, abs(int(speed)), l3, t % 100)

def _build_set_inverted(port_num, inverted):
    _check_port(port_num)
    return (76, port_num, 1 if inverted else 0, 0)

def _build_drive(left_port, right_port, left_speed, right_speed):
    _check_port(left_port, "left_port")
    _check_port(right_port, "right_port")
    _check_distinct_ports(left_port, right_port)
    left_speed = _clamp(left_speed, -100, 100, "left_speed")
    right_speed = _clamp(right_speed, -100, 100, "right_speed")
    l2 = (0 if left_speed >= 0 else 2) | (0 if right_speed >= 0 else 1)
    return (left_port * 10 + right_port, l2, abs(left_speed), abs(right_speed))

def _build_drive_stop(left_port, right_port, stop):
    _check_port(left_port, "left_port")
    _check_port(right_port, "right_port")
    _check_distinct_ports(left_port, right_port)
    l2 = _stop_to_l2(stop)
    if l2 is None:              # stop=continue：維持現狀，不送任何指令
        return None
    return (left_port * 10 + right_port, l2, 0, 0)

# 參數暫存器：對每個 _build_* 分別套用獨立快取，同一指令連續呼叫相同參數時（例如控制
# 迴圈每 tick 都送同樣的 drive(...)），第二次以後直接跳過驗證與組包運算。
_build_motor_run          = _cache_validated(_build_motor_run)
_build_motor_power        = _cache_validated(_build_motor_power)
_build_motor_stop         = _cache_validated(_build_motor_stop)
_build_motor_set_pid      = _cache_validated(_build_motor_set_pid)
_build_motor_run_degrees  = _cache_validated(_build_motor_run_degrees)
_build_motor_track_target = _cache_validated(_build_motor_track_target)
_build_set_inverted       = _cache_validated(_build_set_inverted)
_build_drive              = _cache_validated(_build_drive)
_build_drive_stop         = _cache_validated(_build_drive_stop)

_SENSOR_CUT_V = 9.0   # 電池超過這個電壓，EXP6 韌體停止對樂高顏色／超音波感應器供電（韌體 SENSOR_SUPPLY_CUT_MV）

class MBC_EXP6:
    DEV_NONE = DEV_NONE
    DEV_FORCE = DEV_FORCE
    DEV_COLOR = DEV_COLOR
    DEV_ULTRASONIC = DEV_ULTRASONIC
    DEV_MOTOR = DEV_MOTOR

    DIR_HOLD = DIR_HOLD
    DIR_CW = DIR_CW
    DIR_CCW = DIR_CCW
    DIR_COAST = DIR_COAST
    DIR_BRAKE = DIR_BRAKE

    STOP_COAST = STOP_COAST
    STOP_BRAKE = STOP_BRAKE
    STOP_HOLD = STOP_HOLD
    STOP_CONTINUE = STOP_CONTINUE

    MODE_SPEED = MODE_SPEED
    MODE_ANGLE = MODE_ANGLE
    MODE_ABS_ANGLE = MODE_ABS_ANGLE
    MODE_RESET = MODE_RESET
    MODE_COLOR = MODE_COLOR
    MODE_RGB = MODE_RGB
    MODE_HSV = MODE_HSV
    MODE_DISTANCE = MODE_DISTANCE
    MODE_FORCE = MODE_FORCE

    def __init__(self, port_num, multitask=False):
        if isinstance(port_num, str):          # 'C' 這種字母也可以
            port_num = getattr(port, port_num)
        elif isinstance(port_num, int):        # 0~5 對應 A~F（教育版的 port.F 是 5）
            port_num = getattr(port, 'ABCDEF'[port_num])
        self._port = port_num
        self.v = 0
        self.d = [0] * 6
        self._la = [0] * 6
        self._aa = [0] * 6
        self._port_mode_cache = {}
        self._mbc_win = {}        # 各孔目前要求的 (mode, 起始 byte)
        self._mbc_warn_ms = {}    # 讀不到感應器提示的上次時間（限流）
        self._batt_warned = False # 電池超過 2S 安全電壓的提示只印一次
        self._inverted = {i: False for i in range(1, 7)}   # 各孔正/反裝，預設全部正裝
        self._last_send_time = time.ticks_ms()
        self._last_cmd = None
        self._throttle_ms = 20

        # 1. 孔切成全雙工 UART，送心跳讓 EXP6 進入原始模式，等第一包上行資料（最多 3 秒，不會卡住）
        self._raw = None          # 最新一包上行的 32 bytes（R1~R8 各 int32 LE）
        self._rx = b''
        self._last_drain_ms = self._last_tx_ms = self._last_use_ms = time.ticks_ms()
        for e in list(_instances):          # 同一個孔上一支程式留下的物件先移除，免得兩個一起送心跳
            if e._port is self._port:
                _instances.remove(e)
        self._uart = self._port
        self._uart.mode(hub.port.MODE_FULL_DUPLEX)
        time.sleep_ms(300)
        self._uart.baud(115200)
        time.sleep_ms(50)
        t0 = time.ticks_ms()
        while self._raw is None and time.ticks_diff(time.ticks_ms(), t0) < _CONNECT_MS:
            self._write(_HB_PKT)
            time.sleep_ms(50)
            self._drain()
        if self._raw is None:
            print("[EXP6] 讀不到擴充板資料（請確認 EXP6 接在這個孔且已開電）/ No data from EXP6 (check the port and power)")
        else:
            self.v = ((self._raw[24] | (self._raw[25] << 8)) & 0xFFF) / 100.0
            self._check_battery()       # 只開馬達、不讀感應器的程式也會提示電池
        _instances.append(self)
        _start_heartbeat()

        # 2. 發送首次硬體重置 (L1=0, L2=L3=L4=0)
        self._send(0, 0, 0, 0)
        time.sleep(0.1)
        self.update() # 強制更新一次緩衝區獲取真實電壓

        # 將所有孔位預設切換至角度模式 (1)
        for i in range(1, 7):
            self.set_port_mode(i, self.MODE_ANGLE)

        time.sleep(0.05)
        self.update()
        for i in range(1, 7):
            self._la[i-1] = self.d[i-1]
            self._aa[i-1] = 0

    # ── 原始協定收送（玩具版） ─────────────────────────────────
    def _write(self, data):
        self._uart.write(data)
        self._last_tx_ms = self._last_use_ms = time.ticks_ms()

    def _drain(self):
        # 讀光主機緩衝區，找出最新一包合法的上行封包存進 self._raw
        rx = self._rx
        while True:
            b = self._uart.read(256)
            if not b:
                break
            rx += b
        i, n, latest = 0, len(rx), None
        while i < n:
            if rx[i] != _STREAM_HEADER:
                i += 1
                continue
            if n - i < _STREAM_PAYLOAD + 3:
                break
            if rx[i + 1] != _STREAM_PAYLOAD:
                i += 1
                continue
            x = 0
            for k in range(i + 1, i + _STREAM_PAYLOAD + 2):
                x ^= rx[k]
            nxt = i + _STREAM_PAYLOAD + 3
            # 檢查碼只有 1 byte：再確認下一包也是同步頭（或已到結尾），避免巧合對上
            if x == rx[i + _STREAM_PAYLOAD + 2] and (nxt >= n or rx[nxt] == _STREAM_HEADER):
                latest = rx[i + 2:i + 2 + _STREAM_PAYLOAD]
                i = nxt
            else:
                i += 1
        self._rx = rx[i:]
        self._last_drain_ms = time.ticks_ms()
        if latest is not None:
            self._raw = latest
        return latest

    def _frame(self):
        # 回傳最新一包資料轉成的 8 個 16-bit 值（排列跟教育版樂高模式 mode 0 相同），沒資料回 None
        self._last_use_ms = time.ticks_ms()
        if time.ticks_diff(time.ticks_ms(), self._last_drain_ms) > _STALE_MS:
            # 緩衝區滿了會留住舊資料：先清掉，再等一包新的
            while self._uart.read(256):
                pass
            self._rx = b''
            t0 = time.ticks_ms()
            got = None
            while got is None and time.ticks_diff(time.ticks_ms(), t0) < _FRESH_MS:
                time.sleep_ms(2)
                got = self._drain()
        else:
            self._drain()
        b = self._raw
        if b is None:
            return None
        return [b[k] | (b[k + 1] << 8) for k in range(0, _STREAM_PAYLOAD, 4)]

    def close(self):
        # 停止所有馬達並停背景心跳。程式結束前可呼叫，不呼叫也沒關係（10 秒後心跳自己停）。
        self.stop_all("coast")
        if self in _instances:
            _instances.remove(self)
        if not _instances:
            _stop_heartbeat()

    def update(self):
        r = self._frame()
        if r and len(r) >= 8:
            new_d = []
            for x in r[:6]:
                val = x & 0xFFFF
                if val >= 32768:
                    val -= 65536
                new_d.append(val)

            # 更新累積角度
            for i in range(6):
                if self._port_mode_cache.get(i+1) == self.MODE_ANGLE:
                    c = new_d[i]
                    d = (c - self._la[i]) & 0xFFFF
                    if d > 32767:
                        d -= 65536
                    self._aa[i] += d
                    self._la[i] = c

            self.d = new_d
            self.v = (r[6] & 0xFFF) / 100.0
            self._check_battery()
        return self.d

    def _send(self, l1, l2, l3, l4):
        # 1. 指令狀態去重 (Deduplication)：相同狀態直接無視，0延遲返回！
        cmd = [max(0, min(100, int(x))) for x in (l1, l2, l3, l4)]
        if cmd == self._last_cmd:
            return

        # 2. 自適應智慧節流閥 (Adaptive Smart Throttle)
        now = time.ticks_ms()
        try:
            elapsed = time.ticks_diff(now, self._last_send_time)
        except AttributeError:
            self._last_send_time = now
            elapsed = self._throttle_ms

        if elapsed < self._throttle_ms:
            time.sleep((self._throttle_ms - elapsed) / 1000.0)

        self._write(_cmd_packet(cmd[0], cmd[1], cmd[2], cmd[3]))

        # 3. 更新快取與最後發送時間
        self._last_cmd = cmd
        self._last_send_time = time.ticks_ms()

    def _send_oneshot(self, l1, l2, l3, l4):
        self._send(l1, l2, l3, l4)
        # 動態等待，確保指令已發出 (給予 2.5 倍安全餘裕)
        time.sleep(self._throttle_ms * 2.5 / 1000.0)
        # 替換為空包彈，把擴充板的快取洗掉 (防止最後一個指令被 RTOS 無限重傳)
        self._send(100, 0, 0, 0) # 換成絕對安全的 Dummy Payload，清空擴充板快取

    # ==========================
    # 核心控制指令
    # ==========================
    def motor_run(self, port_num, speed):
        self._send(*_build_motor_run(port_num, speed))

    def motor_power(self, port_num, power):
        self._send(*_build_motor_power(port_num, power))

    def motor_stop(self, port_num, stop):
        pkt = _build_motor_stop(port_num, stop)
        if pkt is None:   # stop=continue：維持現狀，不送任何指令
            return
        self._send(*pkt)

    def motor_set_pid(self, port_num, p, i, d):
        self._send(*_build_motor_set_pid(port_num, p, i, d))

    def motor_run_degrees(self, port_num, speed, degrees, stop):
        self._send_oneshot(*_build_motor_run_degrees(port_num, speed, degrees, stop))

    def motor_track_target(self, port_num, speed, angle, stop):
        self._send_oneshot(*_build_motor_track_target(port_num, speed, angle, stop))
    motor_run_target = motor_track_target   # 相容別名（舊名）

    def set_motor_inverted(self, port_num, inverted=True):
        pkt = _build_set_inverted(port_num, bool(inverted))   # 先驗證 port_num，通過才更新本地狀態
        self._inverted[port_num] = bool(inverted)
        self._send(*pkt)

    def set_port_mode(self, port_num, mode):
        _check_port(port_num)
        _check_mode(mode)
        if mode == self.MODE_RESET:
            self._send(69 + port_num, mode, 0, 0)
            time.sleep(0.05)
            self._send(100, 0, 0, 0) # Oneshot 防背景重送
            # 歸零指令會自動切回角度模式，這裡更新快取
            self._port_mode_cache[port_num] = self.MODE_ANGLE
        else:
            self._send(69 + port_num, mode, 0, 0)
            self._port_mode_cache[port_num] = mode
            time.sleep(0.02)

    def drive(self, left_port, right_port, left_speed, right_speed):
        self._send(*_build_drive(left_port, right_port, left_speed, right_speed))

    def drive_stop(self, left_port, right_port, stop):
        pkt = _build_drive_stop(left_port, right_port, stop)
        if pkt is None:   # stop=continue：維持現狀，不送任何指令
            return
        self._send(*pkt)

    def stop_all(self, stop):
        # 逐孔送停止指令以支援 coast/brake/hold（不再用 (0,0,0,0) 硬重置，避免歸零角度/重置格式的副作用）。
        l2 = _stop_to_l2(stop)
        if l2 is None:   # stop=continue：維持所有馬達現狀，不送任何指令
            return
        for port_num in range(1, 7):
            self._send(port_num * 10, l2, 0, 0)

    def reset_angle(self, port_num):
        self.set_port_mode(port_num, self.MODE_RESET)
        self.set_port_mode(port_num, self.MODE_ANGLE)

        # 強制等待 LPF2 回傳的真實角度歸零，否則會發生巨大的角度跳變
        for _ in range(20):
            self.update()
            if abs(self.d[port_num - 1]) < 5:
                break
            time.sleep(0.02)

        self.update()
        self._la[port_num - 1] = self.d[port_num - 1]
        self._aa[port_num - 1] = 0

    # ==========================
    # 狀態讀取指令
    # ==========================
    def _ensure_mode(self, port_num, mode):
        if self._port_mode_cache.get(port_num) != mode:
            self.set_port_mode(port_num, mode)
            time.sleep(0.05) # 確保模式切換且資料回傳

    def get_port_raw(self, port_num):
        # 孔位驗證放這裡：get_ultrasonic_distance/get_touch_force/get_motor_abs_angle/
        # get_motor_speed/_get_rgb(顏色系列) 全都經由這裡讀值，一次驗證全部涵蓋。
        _check_port(port_num)
        self.update()
        return self.d[port_num - 1]

    def get_ultrasonic_distance(self, port_num):
        self._ensure_mode(port_num, self.MODE_DISTANCE)
        return self.get_port_raw(port_num)

    def get_touch_force(self, port_num):
        self._ensure_mode(port_num, self.MODE_FORCE)
        return self.get_port_raw(port_num)

    def get_motor_angle(self, port_num):
        _check_port(port_num)
        self._ensure_mode(port_num, self.MODE_ANGLE)
        self.update()
        return self._aa[port_num - 1]

    def get_motor_abs_angle(self, port_num):
        # 韌體送的是 -180~179；% 360 換成 0~359（跟 Pybricks 庫一致：-180→180、-1→359）
        self._ensure_mode(port_num, self.MODE_ABS_ANGLE)
        return self.get_port_raw(port_num) % 360

    def get_motor_speed(self, port_num):
        self._ensure_mode(port_num, self.MODE_SPEED)
        return self.get_port_raw(port_num)

    def _color_packet(self, port_num):
        # 原始協定的顏色封包（fmt=10，跟 Pybricks 版相同）：顏色碼 4 bit＋反射 7 bit＋R、G、B 各 7 bit（0~100）
        self._ensure_mode(port_num, self.MODE_COLOR)
        self.update()
        b = self._raw
        if b is None:
            return 0, 0, 0, 0, 0
        o = (port_num - 1) * 4
        b0, b1, b2, b3 = b[o], b[o + 1], b[o + 2], b[o + 3]
        cc = b3 >> 4
        refl = ((b3 & 0x0F) << 3) | (b2 >> 5)
        r = ((b2 & 0x1F) << 2) | (b1 >> 6)
        g = ((b1 & 0x3F) << 1) | (b0 >> 7)
        return cc, refl, r, g, b0 & 0x7F

    def _get_rgb(self, port_num):
        p = self._color_packet(port_num)
        return (p[2], p[3], p[4])

    def get_color_color(self, port_num):
        # 樂高顏色感應器自己判斷的顏色碼（0 黑、1 洋紅、3 藍、4 天藍、5 綠、7 黃、9 紅、10 白）；看不出顏色回 -1
        cc = self._color_packet(port_num)[0]
        return cc if cc <= 10 else -1

    def get_color_reflection(self, port_num):
        return self._color_packet(port_num)[1]

    def get_color_rgb(self, port_num):
        return self._get_rgb(port_num)

    def get_color_hsv(self, port_num):
        r, g, b = self._get_rgb(port_num)
        cmax = max(r, g, b)
        cmin = min(r, g, b)
        delta = cmax - cmin
        h = 0
        if delta == 0: h = 0
        elif cmax == r: h = 60 * (((g - b) / delta) % 6)
        elif cmax == g: h = 60 * (((b - r) / delta) + 2)
        elif cmax == b: h = 60 * (((r - g) / delta) + 4)
        s = 0 if cmax == 0 else (delta / cmax) * 100
        v = cmax
        return (int(h), int(s), int(v))

    def get_color_hue(self, port_num): return self.get_color_hsv(port_num)[0]
    def get_color_sat(self, port_num): return self.get_color_hsv(port_num)[1]
    def get_color_val(self, port_num): return self.get_color_hsv(port_num)[2]
    def get_color_red(self, port_num): return self._get_rgb(port_num)[0]
    def get_color_green(self, port_num): return self._get_rgb(port_num)[1]
    def get_color_blue(self, port_num): return self._get_rgb(port_num)[2]

    def get_device_id(self, port_num):
        # 從上行第 7、8 筆取裝置代碼：R7 = 電壓 12-bit + 孔1 代碼；R8 = 孔2~6 代碼各 3-bit。
        # 0 無、1 力量、3 顏色、4 超音波、5 馬達、7 其他（跟 Pybricks 庫一致）。讀不到資料回 0。
        _check_port(port_num)
        r = self._frame()
        if not r or len(r) < 8:
            return self.DEV_NONE
        if port_num == 1:
            return ((r[6] & 0xFFFF) >> 12) & 0x7
        return ((r[7] & 0xFFFF) >> ((port_num - 2) * 3)) & 0x7

    # ==========================
    # MBC 感應器 / MBC sensors（需搭配 EXP6 韌體 2026-11-03 以後）
    # ==========================
    # 宣告感應器插在 EXP6 的哪一孔，回傳該感應器的物件（感應器直接插上就能用）：
    #     line8 = exp.line8(2)      # 循行者 8 路插在 EXP6 的 2 號孔
    #     print(line8.pos100())
    # 可用：line8 line16 laser2 laser8 ps2 ps4 unicam（神攝手，mode 要跟相機選單一樣：exp.unicam(3, 13)）
    def line8(self, port_num):      return _mbc_cls('EXP6_LINE8')(self, port_num)
    def line16(self, port_num):     return _mbc_cls('EXP6_LINE16')(self, port_num)
    def laser2(self, port_num):     return _mbc_cls('EXP6_LASER2')(self, port_num)
    def laser8(self, port_num):     return _mbc_cls('EXP6_LASER8')(self, port_num)
    def ps2(self, port_num):        return _mbc_cls('EXP6_PS')(self, port_num)
    def ps4(self, port_num):        return _mbc_cls('EXP6_PS')(self, port_num)
    def unicam(self, port_num, mode=13): return _mbc_cls('EXP6_UNICAM')(self, port_num, mode)

    def _mbc_id(self, r, port_num):
        # R7 = 電壓 12-bit + 孔1 裝置碼；R8 = 孔2~6 裝置碼各 3-bit
        if port_num == 1:
            return ((r[6] & 0xFFFF) >> 12) & 0x7
        return ((r[7] & 0xFFFF) >> ((port_num - 2) * 3)) & 0x7

    # 讀第 port_num 孔感應器 mode 的第 ofs、ofs+1 個 byte，回傳 (lo, hi)；讀不到回傳 None。
    # 官方 App 每孔只有 int16，放不下「回應的欄位」，所以換欄位後固定等 2 個 Hub 輪詢週期。
    def _mbc_read(self, port_num, mode, ofs):
        _mbc_check_port(port_num)
        r = self._mbc_frame()
        if (r is None or self._mbc_id(r, port_num) != _DEV_MBC
                or self._mbc_win.get(port_num) != (mode, ofs)):
            self._last_cmd = None   # 允許重送同一筆：感應器拔插後 EXP6 已把這孔的格式打回預設
            self._send(69 + port_num, _FMT_MBC_WINDOW, mode, ofs)
            self._port_mode_cache[port_num] = _FMT_MBC_WINDOW   # 樂高感應器的讀值函式下次會重送自己的格式
            self._mbc_win[port_num] = (mode, ofs)
            t0 = time.ticks_ms()
            time.sleep(self._throttle_ms * 2 / 1000.0)
            while True:
                r = self._mbc_frame()
                if r is not None and self._mbc_id(r, port_num) == _DEV_MBC:
                    break
                if time.ticks_diff(time.ticks_ms(), t0) > _MBC_TIMEOUT_MS:
                    self._mbc_win.pop(port_num, None)
                    self._mbc_warn(port_num)
                    return None
                time.sleep(0.01)
        v = r[port_num - 1] & 0xFFFF
        return v & 0xFF, v >> 8

    def _mbc_warn(self, port_num):
        now = time.ticks_ms()
        last = self._mbc_warn_ms.get(port_num)
        if last is None or time.ticks_diff(now, last) >= _MBC_WARN_MS:
            print("[EXP6] 孔{n} 讀不到 MBC 感應器（確認已插好）/ No MBC sensor on EXP6 port {n} (check the cable)".format(n=port_num))
            self._mbc_warn_ms[port_num] = now


    def _mbc_frame(self):
        r = self._frame()
        return r if (r and len(r) >= 8) else None

    def _check_battery(self):
        # 電池超過 2S 鋰電池的安全電壓：EXP6 韌體 2026-11-04 起停止對樂高感應器供電，印一次提示
        if self._batt_warned or self.v <= _SENSOR_CUT_V:
            return
        self._batt_warned = True
        print("[EXP6] 電池電壓 %.1f V 超過 2S 鋰電池的安全電壓（%.0f V）：已停止對樂高顏色／超音波感應器供電，保護感應器（馬達照常）。請改用 2S 鋰電池（7.4 V）。/ Battery %.1f V is above the 2S Li-ion safe voltage (%.0f V): power to LEGO color/ultrasonic sensors is cut to protect them (motors still work). Please use a 2S Li-ion battery (7.4 V)." % (self.v, _SENSOR_CUT_V, self.v, _SENSOR_CUT_V))

    def get_voltage(self):
        self.update()
        return self.v

    # 跟 Pybricks 版同名。玩具版由背景計時器自動送心跳（函式庫 10 秒沒被呼叫就停，讓程式結束後馬達會停）：
    # keep_alive() 補送一次並重新計時；要等超過 10 秒、馬達又要一直轉，請用 keep_alive_wait(ms)。
    def keep_alive(self):
        self._write(_HB_PKT)

    def keep_alive_wait(self, ms):
        t0 = time.ticks_ms()
        while True:
            left = ms - time.ticks_diff(time.ticks_ms(), t0)
            if left <= 0:
                break
            self._last_use_ms = time.ticks_ms()
            time.sleep_ms(int(min(left, 1000)))


# ==========================
# MBC 感應器物件（由 exp.line8(port) 這類方法建立）
# ==========================
# 函式名稱與回傳值跟各感應器直插 Hub 的函式庫相同（一次回傳整組資料的函式除外）。
# 每個函式讀 1 次「2 bytes 視窗」；同一個視窗的函式（例如 pos8 與 width）連續呼叫最快，
# 換到別的視窗第一次約多 2~3 個 Hub 輪詢週期。讀不到感應器時回傳 None 並印提示。
_DEV_MBC        = 6     # EXP6 回報「MBC 感應器視窗已生效」的裝置碼
_FMT_MBC_WINDOW = 100   # EXP6 fmt=100：L3 = 感應器 mode、L4 = 起始 byte
_MBC_TIMEOUT_MS = 400
_MBC_WARN_MS    = 2000

def _mbc_check_port(port_num):
    if isinstance(port_num, bool) or not isinstance(port_num, int) or not (1 <= port_num <= 6):
        raise ValueError("port=%r 不是有效孔位，必須是 1~6 的整數 / must be an integer 1~6" % (port_num,))

def _s8(b):
    return b - 256 if b >= 128 else b

def _check_ch_range(ch, n):
    if isinstance(ch, bool) or not isinstance(ch, int) or not (1 <= ch <= n):
        raise ValueError("ch must be 1~{}, got {}".format(n, ch))

_JUNCTION_NAMES = {0: "none", 1: "line", 2: "junction"}

class _EXP6_SENSOR:
    _MODE = 9   # MBC 感應器的完整資料 mode

    def __init__(self, exp, port_num):
        _mbc_check_port(port_num)
        self._exp = exp
        self._port = port_num
        self._sq = None     # 下命令：最後一個序號（1~99）
        self._ck = None     # 　　　　最後一個 (命令碼, 參數…)
        self._pk = None     # 　　　　最後送出的封包（重送用）
        self._ct = 0        # 　　　　最後送出的時間

    def _get(self, ofs, fn):
        b = self._exp._mbc_read(self._port, self._MODE, ofs)
        return None if b is None else fn(b[0], b[1])

    # ── 下命令 / commands（2026-11-05；跟 Pybricks 版同名、同用法；需要 EXP6 韌體 2026-11-03 以後）──
    # seq = sensor.cmd(0xE2)          # 命令碼 0xE0~0xEE，參數最多 5 個（-9999~9999），回傳序號
    # ok = sensor.wait_done()         # 等做完：True 完成；False 失敗、拒絕或超過 timeout 毫秒
    # sensor.cmd_state()              # 0 完成、1 執行中、2 失敗、3 拒絕；感應器還沒收到 = None
    # 命令碼與參數見各感應器的說明（跟直插主機時一樣）。EXP6 用 L1=91~96 寫參數、L1=97 送出（值都 ≤100）。
    def _cmd_pkts(self, code, p, seq):
        L1 = 90 + self._port
        out = [(L1, 0, 0, code - 0xE0)]
        for i, x in enumerate(p):
            x = int(x)
            ax = -x if x < 0 else x
            out.append((L1, i + 1 + (10 if x < 0 else 0), ax // 100, ax % 100))
        out.append((97, self._port, seq, 0))
        return out

    def _st(self):
        # 模式 8 的第 6、7 個 byte = (命令狀態, 感應器最後收下的序號)；讀不到 = None
        return self._exp._mbc_read(self._port, 8, 6)

    def _send_pk(self, pk):
        for x in pk:
            self._exp._last_cmd = None      # 命令封包每筆都要送（不去重）
            self._exp._send(*x)
        self._exp._mbc_win.pop(self._port, None)   # 下過命令：之後讀值重新要求視窗

    def cmd(self, code, *p, again=False):
        if isinstance(code, bool) or not isinstance(code, int) or not 0xE0 <= code <= 0xEE:
            raise ValueError('cmd(): 命令碼要 0xE0~0xEE / code must be 0xE0~0xEE')
        if len(p) > 5:
            raise ValueError('cmd(): 參數最多 5 個 / at most 5 parameters')
        for x in p:
            if not -9999 <= int(x) <= 9999:
                raise ValueError('cmd(): EXP6 參數要 -9999~9999 / parameters must be -9999~9999')
        k = (code,) + tuple(int(x) for x in p)
        if k == self._ck and not again:
            return self._sq                 # 同命令同參數不重送（要重做請加 again=True）
        if self._sq is None:
            f = self._exp._mbc_read(self._port, 8, 2)   # 第 2、3 個 byte：第 3 個 bit0 = 支援命令
            if f is None:
                return None
            if not (f[1] & 1):
                raise ValueError('cmd(): 感應器韌體太舊，不支援命令，請更新韌體 / sensor firmware too old for commands')
            s = self._st()
            if s is None:
                return None
            self._sq = s[1]                 # 從感應器最後收下的序號 +1 開始，才不會跟上一個程式撞號
        self._sq = self._sq % 99 + 1
        self._ck = k
        self._pk = self._cmd_pkts(code, p, self._sq)
        self._ct = time.ticks_ms()
        self._send_pk(self._pk)
        return self._sq

    def cmd_state(self):
        s = self._st()
        if s is None or self._sq is None or s[1] != self._sq:
            return None
        return s[0]

    def busy(self):
        if self._sq is None:
            return False
        s = self.cmd_state()
        return s is None or s == 1

    def wait_done(self, timeout=10000):
        if self._sq is None:
            return True
        t0 = time.ticks_ms()
        while True:
            s = self.cmd_state()
            if s is not None and s != 1:
                return s == 0
            if time.ticks_diff(time.ticks_ms(), t0) >= timeout:
                return False
            if s is None and time.ticks_diff(time.ticks_ms(), self._ct) >= 300 and self._pk:
                self._ct = time.ticks_ms()  # 300 ms 還沒收到：同一個序號重送（感應器不會重做）
                self._send_pk(self._pk)
            self._exp.keep_alive_wait(20)


# 循行者 8 路：pos8() -8~+8、width() 0~8、pos100() -100~+100、junction() 0~2+、
# junction_name() "none"/"line"/"junction"、bin_raw() 0~255、on_sensors() [int]、ir_ch(ch) 0~100
class EXP6_LINE8(_EXP6_SENSOR):
    def pos8(self):          return self._get(8,  lambda lo, hi: lo - 8)
    def width(self):         return self._get(8,  lambda lo, hi: hi)
    def pos100(self):        return self._get(10, lambda lo, hi: lo - 100)
    def junction(self):      return self._get(10, lambda lo, hi: hi)
    def junction_name(self): return self._get(10, lambda lo, hi: _JUNCTION_NAMES.get(min(hi, 2), "junction"))
    def bin_raw(self):       return self._get(12, lambda lo, hi: lo)
    def on_sensors(self):
        return self._get(12, lambda lo, hi: [i + 1 for i in range(8) if lo & (1 << i)])
    def ir_ch(self, ch):
        _check_ch_range(ch, 8)
        idx = ch - 1
        return self._get(idx & ~1, lambda lo, hi: hi if idx & 1 else lo)


# 循行者 16 路：pos16() -16~+16、width()、pos100()、junction()、junction_name()、
# bin_raw() 0~65535、on_sensors()、ir_ch(ch) 0~15（ch = 1~16）
class EXP6_LINE16(_EXP6_SENSOR):
    def pos16(self):         return self._get(8,  lambda lo, hi: lo - 16)
    def width(self):         return self._get(8,  lambda lo, hi: hi)
    def pos100(self):        return self._get(10, lambda lo, hi: lo - 100)
    def junction(self):      return self._get(10, lambda lo, hi: hi)
    def junction_name(self): return self._get(10, lambda lo, hi: _JUNCTION_NAMES.get(min(hi, 2), "junction"))
    def bin_raw(self):       return self._get(12, lambda lo, hi: lo | (hi << 8))
    def on_sensors(self):
        return self._get(12, lambda lo, hi: [i + 1 for i in range(16) if (lo | (hi << 8)) & (1 << i)])
    def ir_ch(self, ch):
        _check_ch_range(ch, 16)
        idx = ch - 1
        byte_i = idx >> 1
        shift = 4 if idx & 1 else 0
        return self._get(byte_i & ~1, lambda lo, hi: ((hi if byte_i & 1 else lo) >> shift) & 0x0F)


# 兩路雷射 TOF2：left() right() average() min_distance()（0~120 cm，120 = 沒目標）、
# min_side()（-1 左較近、+1 右較近、0 相等）、diff()（左 - 右）、ch(ch)、is_ok(ch)
class EXP6_LASER2(_EXP6_SENSOR):
    def left(self):         return self._get(0, lambda lo, hi: lo)
    def right(self):        return self._get(0, lambda lo, hi: hi)
    def average(self):      return self._get(0, lambda lo, hi: (lo + hi) >> 1)
    def min_distance(self): return self._get(0, lambda lo, hi: min(lo, hi))
    def min_side(self):     return self._get(0, lambda lo, hi: -1 if lo < hi else (1 if hi < lo else 0))
    def diff(self):         return self._get(0, lambda lo, hi: lo - hi)
    def ch(self, ch):
        _check_ch_range(ch, 2)
        return self._get(0, lambda lo, hi: hi if ch == 2 else lo)
    def is_ok(self, ch):
        _check_ch_range(ch, 2)
        bit = 1 << (ch - 1)
        return self._get(2, lambda lo, hi: (lo & bit) != 0 and (hi & bit) == 0)


# 八方位雷射 TOF8：ch(ch) 0~120（ch = 1~8）、min_direction() -8~+8、min_distance()、
# max_direction()、max_distance()、init_status() [bool]*8、fault_status() [bool]*8、is_ok(ch)
class EXP6_LASER8(_EXP6_SENSOR):
    def ch(self, ch):
        _check_ch_range(ch, 8)
        idx = ch - 1
        return self._get(idx & ~1, lambda lo, hi: hi if idx & 1 else lo)
    def min_direction(self): return self._get(8,  lambda lo, hi: lo - 8)
    def min_distance(self):  return self._get(8,  lambda lo, hi: hi)
    def max_direction(self): return self._get(10, lambda lo, hi: lo - 8)
    def max_distance(self):  return self._get(10, lambda lo, hi: hi)
    def init_status(self):   return self._get(12, lambda lo, hi: [(lo & (1 << i)) != 0 for i in range(8)])
    def fault_status(self):  return self._get(12, lambda lo, hi: [(hi & (1 << i)) != 0 for i in range(8)])
    def is_ok(self, ch):
        _check_ch_range(ch, 8)
        bit = 1 << (ch - 1)
        return self._get(12, lambda lo, hi: (lo & bit) != 0 and (hi & bit) == 0)


# PS2 遙控接收器 / PS4 藍牙遙控：buttons() 16-bit（PSX 排列：b15 ■ b14 Ⅹ b13 ● b12 ▲ b11 R1
# b10 L1 b9 R2 b8 L2 b7 左 b6 下 b5 右 b4 上 b3 START b2 R3 b1 L3 b0 SELECT）、
# pressed(name)、pressed_buttons()、lx() ly() rx() ry() 1~9（5 = 中心）
_PS_BUTTONS = {
    "square": 15, "cross": 14, "circle": 13, "triangle": 12,
    "r1": 11, "l1": 10, "r2": 9, "l2": 8,
    "left": 7, "down": 6, "right": 5, "up": 4,
    "start": 3, "r3": 2, "l3": 1, "select": 0,
}

def _ps_word(lo, hi):
    return ((hi >> 4) << 12) | ((lo & 0x0F) << 8) | ((hi & 0x0F) << 4) | (lo >> 4)

class EXP6_PS(_EXP6_SENSOR):
    _MODE = 0
    def buttons(self): return self._get(0, _ps_word)
    def pressed(self, name):
        key = str(name).strip().lower()
        if key not in _PS_BUTTONS:
            raise ValueError("unknown button {!r}, use one of {}".format(name, sorted(_PS_BUTTONS)))
        bit = 1 << _PS_BUTTONS[key]
        return self._get(0, lambda lo, hi: (_ps_word(lo, hi) & bit) != 0)
    def pressed_buttons(self):
        return self._get(0, lambda lo, hi: [k for k, b in _PS_BUTTONS.items() if _ps_word(lo, hi) & (1 << b)])
    def lx(self): return self._get(2, lambda lo, hi: (lo | (hi << 8)) // 10)
    def ly(self): return self._get(2, lambda lo, hi: (lo | (hi << 8)) % 10)
    def rx(self): return self._get(4, lambda lo, hi: (lo | (hi << 8)) // 10)
    def ry(self): return self._get(4, lambda lo, hi: (lo | (hi << 8)) % 10)


# 神攝手通用版/ Sharpshooter Vision：cam = exp.unicam(port, mode=13)，函式同 Pybricks 版 UniCam
#   mode＝相機模式 ×10＋子選項，要跟相機選單切的一樣（SPIKE 讀不到相機目前的模式）
#   11 顏色追蹤 最多色  12 多色  13 標準 XY＋面積  14 精細 XYWH   21 座標取色 色相  22 色相＋飽和
#   23 色相＋飽和＋明度   30 照順序給色號   41 循線 黑線  42 白線  43 色線   51 物體清單  52 物體詳細
#   每個函式只讀它要的 byte（相鄰 2 個一次）；hsv()／device_id() 讀 2 次，order() 讀 7 次（較慢）。
#   座標像素 X 0~159、Y 0~119（上＝小），(0, 0)＝沒找到；色相碼 0~255；循線偏移線在左＝負。
_UC_MODES = (11, 12, 13, 14, 21, 22, 23, 30, 41, 42, 43, 51, 52)
_UC_NCOL = {11: 14, 12: 7, 13: 4, 14: 2}
_UC_NPT = {21: 14, 22: 7, 23: 4}
_UC_NOBJ = {51: 4, 52: 2}
_UC_LINE = (41, 42, 43)
SHAPES = ('other', 'circle', 'ellipse', 'triangle', 'square', 'rectangle', 'pentagon', 'hexagon', 'star', 'trapezoid')
SHAPES_ZH = ('其他', '圓', '橢圓', '三角', '正方', '長方', '五邊', '六邊', '星', '梯形')

# 色相碼 → 'red' 'orange' 'yellow' 'green' 'cyan' 'blue' 'purple' 'white' 'black'（None＝沒有）
def color_name(h):
    if h is None:
        return None
    if h > 248:
        return 'black'
    if h > 240:
        return 'white'
    if h < 12 or h >= 220:
        return 'red'
    if h < 28:
        return 'orange'
    if h < 56:
        return 'yellow'
    if h < 100:
        return 'green'
    if h < 140:
        return 'cyan'
    if h < 180:
        return 'blue'
    return 'purple'

# 形狀編號 0~9 → 名稱；zh=True 回中文
def shape_name(s, zh=False):
    if s is None or not 0 <= s <= 9:
        return None
    return SHAPES_ZH[s] if zh else SHAPES[s]

def _uc_windows(idxs):
    ks = []
    for i in sorted(set(idxs)):
        if not ks or i > ks[-1] + 1:
            ks.append(i)
    return ks

def _uc_xy_idx(m, c):
    if m == 11:
        return (c - 1,)
    k = {12: 2, 13: 3, 14: 6}[m] * (c - 1)
    return (k, k + 1)

def _uc_xy(b, m, c):
    if m == 11:
        g = b[c - 1]
        if not g:
            return (0, 0)
        return (((g - 1) % 16) * 10 + 5, ((g - 1) // 16) * 10 + 5)
    k = {12: 2, 13: 3, 14: 6}[m] * (c - 1)
    return (b[k], b[k + 1])

def _uc_count6_idx(m, c):
    if m == 11:
        bit = 42 if c == 0 else 3 * (c - 1)
        return (bit // 8, (bit + 3) // 8) if c == 0 else (bit // 8, (bit + 2) // 8)
    return (3,) if c == 0 else ((c - 1) // 2,)

def _uc_count6(b, m, c):
    if m == 11:
        bits = 0
        for i in range(6):
            bits |= b[i] << (8 * i)
        return (bits >> (3 * (c - 1))) & 7 if c else (bits >> 42) & 15
    if c == 0:
        return b[3] & 15
    x = b[(c - 1) // 2]
    return x >> 4 if c % 2 else x & 15

def _uc_hsv_idx(m, i):
    if m == 21:
        return (i - 1,)
    if m == 22:
        return (2 * i - 2, 2 * i - 1)
    return (3 * i - 3, 3 * i - 2, 3 * i - 1)

def _uc_hsv(b, m, i):
    if m == 21:
        return (b[i - 1], None, None)
    if m == 22:
        return (b[2 * i - 2], b[2 * i - 1], None)
    return (b[3 * i - 3], b[3 * i - 2], b[3 * i - 1])

def _uc_ob_code(b, m, k):
    if m == 51:
        return b[k]
    return b[k] * 10 + b[k + 1] if b[k] else 0

class EXP6_UNICAM(_EXP6_SENSOR):
    def __init__(self, exp, port_num, mode=13):
        if mode not in _UC_MODES:
            raise ValueError('mode 要填 %s / mode must be one of %s' % (_UC_MODES, _UC_MODES))
        _EXP6_SENSOR.__init__(self, exp, port_num)
        self._m = mode

    # 讀感應器 smode 的 idxs 這些 byte，湊成 16 bytes 交給 fn(b)；任何一次讀不到回傳 None
    def _uc(self, smode, idxs, fn):
        b = [0] * 16
        for k in _uc_windows(idxs):
            r = self._exp._mbc_read(self._port, smode, k)
            if r is None:
                return None
            b[k], b[k + 1] = r
        return fn(b)

    def _need(self, name, ok):
        if self._m not in ok:
            raise ValueError('%s(): 模式 %d 沒有這個值，要用模式 %s / not available in mode %d, use mode %s'
                             % (name, self._m, ok, self._m, ok))

    def _col(self, c, n):
        if not 1 <= c <= n:
            raise ValueError('第幾色／第幾點要填 1~%d / colour or spot must be 1~%d' % (n, n))

    def _obj(self, name, i, ok=(51, 52)):
        self._need(name, ok)
        n = _UC_NOBJ[self._m]
        if not 1 <= i <= n:
            raise ValueError('第幾個物體要填 1~%d / object must be 1~%d' % (n, n))
        return 3 * i - 2 if self._m == 51 else 7 * (i - 1)

    # 共用：mode_number() sensor_ok() device_id()→(0x4C, hw0, 韌體版字) is_unicam() firmware_version()→(主, 次)
    def mode_number(self):
        return self._m

    def sensor_ok(self):
        return self._uc(9, (0,), lambda b: True) is not None

    def device_id(self):
        return self._uc(8, (0, 1, 4, 5), lambda b: (b[0], b[1], b[4] | (b[5] << 8)))

    def is_unicam(self):
        return self._uc(8, (0, 1), lambda b: (b[0] | (b[1] << 8)) == 0x034C)

    def firmware_version(self):
        return self._uc(8, (4, 5), lambda b: ((b[4] | (b[5] << 8)) // 100, (b[4] | (b[5] << 8)) % 100))

    # 相機模式 1 顏色追蹤（11~14）：max_color() xy(c) x(c) y(c) found(c) cell(c) area(c) size(c) count(c) colors_found()
    def max_color(self):
        self._need('max_color', (11, 12, 13, 14))
        return _UC_NCOL[self._m]

    def _cxy(self, name, color, j):
        self._need(name, (11, 12, 13, 14))
        self._col(color, _UC_NCOL[self._m])
        m = self._m
        return self._uc(9, _uc_xy_idx(m, color), lambda b: j(_uc_xy(b, m, color)))

    def xy(self, color):
        return self._cxy('xy', color, lambda p: p)

    def x(self, color):
        return self._cxy('x', color, lambda p: p[0])

    def y(self, color):
        return self._cxy('y', color, lambda p: p[1])

    def found(self, color):
        return self._cxy('found', color, lambda p: p[0] > 0)

    def cell(self, color):
        self._need('cell', (11,))
        self._col(color, 14)
        i = color - 1
        return self._uc(9, (i,), lambda b: (0, 0) if not b[i] else ((b[i] - 1) % 16 + 1, (b[i] - 1) // 16 + 1))

    def area(self, color):
        self._need('area', (13, 14))
        self._col(color, _UC_NCOL[self._m])
        i = 3 * color - 1 if self._m == 13 else 6 * color - 1
        return self._uc(9, (i,), lambda b: b[i])

    def size(self, color):
        self._need('size', (14,))
        self._col(color, 2)
        i = 6 * color - 4
        return self._uc(9, (i, i + 1), lambda b: (b[i], b[i + 1]))

    def count(self, color):
        self._need('count', (11, 12, 13, 14))
        self._col(color, _UC_NCOL[self._m])
        m = self._m
        if m in (11, 12):
            return self._uc(6, _uc_count6_idx(m, color), lambda b: _uc_count6(b, m, color))
        if m == 13:
            i = 12 + (color - 1) // 2
            return self._uc(9, (i,), lambda b: b[i] >> 4 if color % 2 else b[i] & 15)
        i = 6 * color - 2
        return self._uc(9, (i,), lambda b: b[i])

    def colors_found(self):
        self._need('colors_found', (11, 12, 14))
        m = self._m
        if m == 14:
            return self._uc(9, (12,), lambda b: b[12])
        return self._uc(6, _uc_count6_idx(m, 0), lambda b: _uc_count6(b, m, 0))

    # 相機模式 2 座標取色（21~23）：max_point() hue(i) sat(i) val(i) hsv(i) color(i)
    def max_point(self):
        self._need('max_point', (21, 22, 23))
        return _UC_NPT[self._m]

    def _pt(self, name, ok, point, sel, j):
        self._need(name, ok)
        self._col(point, _UC_NPT[self._m])
        m = self._m
        return self._uc(9, sel(_uc_hsv_idx(m, point)), lambda b: j(_uc_hsv(b, m, point)))

    def hue(self, point):
        return self._pt('hue', (21, 22, 23), point, lambda t: t[:1], lambda h: h[0])

    def sat(self, point):
        return self._pt('sat', (22, 23), point, lambda t: t[1:2], lambda h: h[1])

    def val(self, point):
        return self._pt('val', (23,), point, lambda t: t[2:3], lambda h: h[2])

    def hsv(self, point):
        return self._pt('hsv', (21, 22, 23), point, lambda t: t, lambda h: h)

    def color(self, point):
        return self._pt('color', (21, 22, 23), point, lambda t: t[:1], lambda h: color_name(h[0]))

    # 相機模式 3 照順序給色號（30）：order() 由左到右的色相碼（最多 13 塊，讀 7 次）、total() 總塊數
    def order(self):
        self._need('order', (30,))
        return self._uc(9, range(14), lambda b: b[:min(13, b[13])])

    def total(self):
        self._need('total', (30,))
        return self._uc(9, (13,), lambda b: b[13])

    # 相機模式 4 循線（41~43）：near() mid() far() angle() width() flags() junction() junction_dist()
    #   edges() far_width() marker() lost()（line_all() 拿掉，每個值都有自己的函式）
    def _li(self, name, i, signed=False):
        self._need(name, _UC_LINE)
        return self._uc(9, (i,), lambda b: _s8(b[i]) if signed else b[i])

    def near(self):
        return self._li('near', 0, True)

    def mid(self):
        return self._li('mid', 1, True)

    def far(self):
        return self._li('far', 2, True)

    def angle(self):
        return self._li('angle', 3, True)

    def width(self):
        return self._li('width', 4)

    def flags(self):
        return self._li('flags', 5)

    def junction(self):
        return self._li('junction', 6)

    def junction_dist(self):
        return self._li('junction_dist', 7)

    def edges(self):
        self._need('edges', _UC_LINE)
        return self._uc(9, (8, 9), lambda b: (_s8(b[8]), _s8(b[9])))

    def far_width(self):
        return self._li('far_width', 10)

    def marker(self):
        self._need('marker', _UC_LINE)
        return self._uc(9, (11, 12), lambda b: (b[11], b[12]))

    def lost(self):
        return self._li('lost', 13)

    # 相機模式 5 物體偵測（51、52）：max_object() object_count() object(i) object_found(i) object_color(i)
    #   object_shape(i) object_xy(i) object_size(i) object_area(i)；物體 1＝面積最大，只看目標色 1~8
    def max_object(self):
        self._need('max_object', (51, 52))
        return _UC_NOBJ[self._m]

    def object_count(self):
        self._need('object_count', (51,))
        return self._uc(9, (0,), lambda b: b[0])

    def _code(self, name, i, j):
        k = self._obj(name, i)
        m = self._m
        return self._uc(9, (k,) if m == 51 else (k, k + 1), lambda b: j(_uc_ob_code(b, m, k)))

    def object(self, i):
        return self._code('object', i, lambda c: c)

    def object_found(self, i):
        return self._code('object_found', i, lambda c: c > 0)

    def object_color(self, i):
        return self._code('object_color', i, lambda c: c // 10)

    def object_shape(self, i):
        return self._code('object_shape', i, lambda c: c % 10)

    def object_xy(self, i):
        k = self._obj('object_xy', i) + (1 if self._m == 51 else 2)
        return self._uc(9, (k, k + 1), lambda b: (b[k], b[k + 1]))

    def object_size(self, i):
        k = self._obj('object_size', i, (52,)) + 4
        return self._uc(9, (k, k + 1), lambda b: (b[k], b[k + 1]))

    def object_area(self, i):
        k = self._obj('object_area', i, (52,)) + 6
        return self._uc(9, (k,), lambda b: b[k])


# ==========================
# 程式導向 API (與 Pybricks 版 100% 相容)
# ==========================
def exp_motor_run(*a):         return _default_instance().motor_run(*a)
def exp_motor_power(*a):       return _default_instance().motor_power(*a)
def exp_motor_stop(*a):         return _default_instance().motor_stop(*a)
def exp_motor_set_pid(*a):      return _default_instance().motor_set_pid(*a)
def exp_motor_run_degrees(*a):  return _default_instance().motor_run_degrees(*a)
def exp_motor_track_target(*a): return _default_instance().motor_track_target(*a)
def exp_motor_run_target(*a):   return _default_instance().motor_run_target(*a)   # 相容別名 / alias
def exp_set_motor_inverted(*a): return _default_instance().set_motor_inverted(*a)
def exp_set_port_mode(*a):     return _default_instance().set_port_mode(*a)
def exp_drive(*a):             return _default_instance().drive(*a)
def exp_drive_stop(*a):        return _default_instance().drive_stop(*a)
def exp_stop_all(*a):          return _default_instance().stop_all(*a)
def exp_reset_angle(*a):       return _default_instance().reset_angle(*a)

def exp_get_port_raw(*a):            return _default_instance().get_port_raw(*a)
def exp_get_ultrasonic_distance(*a): return _default_instance().get_ultrasonic_distance(*a)
def exp_get_touch_force(*a):         return _default_instance().get_touch_force(*a)

def exp_get_motor_angle(*a):     return _default_instance().get_motor_angle(*a)
def exp_get_motor_abs_angle(*a): return _default_instance().get_motor_abs_angle(*a)
def exp_get_motor_speed(*a):     return _default_instance().get_motor_speed(*a)

def exp_get_color_color(*a):      return _default_instance().get_color_color(*a)
def exp_get_color_reflection(*a): return _default_instance().get_color_reflection(*a)
def exp_get_color_rgb(*a):        return _default_instance().get_color_rgb(*a)
def exp_get_color_hsv(*a):        return _default_instance().get_color_hsv(*a)
def exp_get_color_hue(*a):        return _default_instance().get_color_hue(*a)
def exp_get_color_sat(*a):        return _default_instance().get_color_sat(*a)
def exp_get_color_val(*a):        return _default_instance().get_color_val(*a)
def exp_get_color_red(*a):        return _default_instance().get_color_red(*a)
def exp_get_color_green(*a):      return _default_instance().get_color_green(*a)
def exp_get_color_blue(*a):       return _default_instance().get_color_blue(*a)

def exp_get_device_id(*a): return _default_instance().get_device_id(*a)
def exp_get_voltage():     return _default_instance().get_voltage()

def exp_run_task(coro):
    raise NotImplementedError("SPIKE App 版本不支援 async 任務。/ async tasks are not supported in the SPIKE App version.")
def run_task(coro):
    raise NotImplementedError("SPIKE App 版本不支援 async 任務。/ async tasks are not supported in the SPIKE App version.")
def keep_alive():
    if _instances:
        _instances[-1].keep_alive()
def keep_alive_wait(ms):
    if _instances:
        _instances[-1].keep_alive_wait(ms)
    else:
        time.sleep(ms / 1000.0)

__all__ = [
    "exp_run_task",
    "run_task", "keep_alive", "keep_alive_wait",
    "DEV_NONE", "DEV_FORCE", "DEV_COLOR", "DEV_ULTRASONIC", "DEV_MOTOR",
    "DIR_HOLD", "DIR_CW", "DIR_CCW", "DIR_COAST", "DIR_BRAKE",
    "STOP_COAST", "STOP_BRAKE", "STOP_HOLD", "STOP_CONTINUE",
    "MODE_SPEED", "MODE_ANGLE", "MODE_ABS_ANGLE", "MODE_RESET",
    "MODE_COLOR", "MODE_RGB", "MODE_HSV", "MODE_DISTANCE", "MODE_FORCE",
    "exp_motor_run", "exp_motor_power",
    "exp_motor_stop",
    "exp_motor_set_pid",
    "exp_motor_run_degrees", "exp_motor_track_target", "exp_motor_run_target",
    "exp_set_motor_inverted",
    "exp_drive", "exp_drive_stop",
    "exp_stop_all", "exp_reset_angle",
    "exp_get_port_raw",
    "exp_get_motor_angle", "exp_get_motor_abs_angle", "exp_get_motor_speed",
    "exp_get_ultrasonic_distance",
    "exp_get_touch_force",
    "exp_get_color_color", "exp_get_color_reflection",
    "exp_get_color_rgb",   "exp_get_color_hsv",
    "exp_get_color_hue",   "exp_get_color_sat",   "exp_get_color_val",
    "exp_get_color_red",   "exp_get_color_green", "exp_get_color_blue",
    "exp_get_device_id",
    "exp_get_voltage",
    "exp_set_port_mode",
    "MBC_EXP6",
]
