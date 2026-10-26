# ════════════════════════════════════════════════════════════════
#  EXP6 智能擴充板 Python 函式庫（Pybricks SPIKE Prime 版）
#  EXP6 Smart Expansion Board Library for Pybricks SPIKE Prime
#  Author: MBC robotics    Date: 2026.06
# ════════════════════════════════════════════════════════════════
#
#  快速上手 / Quick start
#  ────────────────────────────────────────────────────────────
#      from MBC_exp6_auto_Lib import MBC_EXP6
#      exp6 = MBC_EXP6(port, multitask)
#
#  port：1~6，對應 Port.A~F。
#  port: 1-6, maps to Port.A~F.
#
#  multitask 決定下面兩種寫法 / multitask picks one of two styles：
#
#  ① multitask=False（同步 sync）— 直接呼叫，不用 await
#     Call functions directly, no "await" needed.
#         exp6.motor_power(6, 100)
#         exp6.keep_alive_wait(5000)
#
#  ② multitask=True（非同步 async）— 每個呼叫前加 await，
#     程式整體用 run_task(main()) 執行。
#     Every call needs "await"; run the program with run_task(main()).
#
#     ⚠ 想要「背景自動保活 + 結束自動停馬達」，務必這樣匯入
#       （這行要排在 pybricks.tools 的 run_task 匯入之後）：
#     ⚠ To get automatic background keep-alive AND auto-stop-on-exit,
#       import run_task like this (AFTER the pybricks.tools import):
#         from pybricks.tools import run_task
#         from MBC_exp6_auto_Lib import MBC_EXP6, run_task
#       漏了這行，run_task() 會變成 Pybricks 原生版，不會有上述保護。
#       Without it, run_task() silently falls back to plain Pybricks
#       and you lose that protection.
#         async def main():
#             await exp6.motor_power(6, 100)
#             await exp6.keep_alive_wait(5000)
#         run_task(main())
#
#  多塊 EXP6 / Multiple boards
#  ────────────────────────────────────────────────────────────
#      可以同時建立多個 MBC_EXP6 物件（分別插在 hub 不同孔位），
#      每個物件各自持有獨立的連線狀態，互不干擾。
#      You can create multiple MBC_EXP6 objects at once (one per hub
#      port); each keeps its own independent connection state.
#
#  長時間等待 / 長時間運算時請用以下兩個函式維持連線：
#  During a long wait or long computation, use these to stay connected:
#      keep_alive_wait(ms)   取代 wait(ms) / replaces wait(ms)
#      keep_alive()          長運算中間呼叫一次 / call once mid-computation
#  （async 模式下背景已自動處理，不需手動呼叫）
#  (Handled automatically in the background in async mode.)
#
#  為什麼會自動停馬達？/ Why do motors auto-stop?
#  擴充板內建看門狗：太久沒收到任何資料就會自動讓馬達滑行停止。這是安全
#  機制（保護程式當機、按停止鍵、USB 斷線等情況），不是 bug，無法關閉。
#  The board has a built-in watchdog: if it stops hearing from the hub
#  for too long, it auto-coasts the motors to a stop. This is a safety
#  feature (covers crashes, the stop button, USB disconnects, etc.) —
#  not a bug, and it cannot be turned off.
#
#  協議細節與設計權衡（例如看門狗逾時數值怎麼來的）另見：
#  Protocol details and design rationale (e.g. how the watchdog
#  timeout value was chosen) are documented separately in:
#      新版上下行協議.md
# ════════════════════════════════════════════════════════════════

from pybricks.iodevices import UARTDevice
from pybricks.parameters import Port
from pybricks.tools import StopWatch as _StopWatch
from pybricks.tools import wait
# 底線別名：避免跟本檔對外覆寫版的 run_task / multitask 同名衝突
from pybricks.tools import run_task as _pb_run_task
from pybricks.tools import multitask as _multitask

# ── raw UART 協議常數 ──────────────────────────────────────
_CMD_HEADER    = 0xA5      # 下行指令封包同步頭（v4.0 6-byte 格式：[0xA5,L1,L2,L3,L4,xor]）
_STREAM_HEADER = 0x5A      # 上行串流封包同步頭
_L1_HEARTBEAT  = 0x01      # L1=1：純心跳，只餵看門狗，不觸發任何指令動作
_BAUD          = 115200
_STREAM_PAYLOAD = 32       # 內層 payload 固定 32 bytes（R1~R8，各 int32 little-endian）
_HEARTBEAT_MS   = 100      # 距上次送出超過此值就搭便車補心跳

_L1_MODE_BASE = 69

_PORT_MAP = {
    1: Port.A,
    2: Port.B,
    3: Port.C,
    4: Port.D,
    5: Port.E,
    6: Port.F,
}

_PORT_LETTER = {1: 'A', 2: 'B', 3: 'C', 4: 'D', 5: 'E', 6: 'F'}

DEV_NONE       = 0
DEV_FORCE      = 1
DEV_COLOR      = 3
DEV_ULTRASONIC = 4
DEV_MOTOR      = 5

DIR_HOLD  = 0
DIR_CW    = 1
DIR_CCW   = 2
DIR_COAST = 3
DIR_BRAKE = 4

# 停止狀態（motor_stop / stop_all / motor_run_degrees / motor_track_target 的必填參數；
# 可傳字串常數或整數碼 1~4，大小寫不拘）
# Stop state (required arg for motor_stop / stop_all / motor_run_degrees / motor_track_target;
# string constant OR int 1~4)
# 整數碼(1-based)：1=coast 滑行、2=brake 煞車、3=hold 鎖死、4=continue 續轉/維持現狀。
# continue 在不同函式家族的意義不同：
#   - motor_run_degrees/motor_track_target(走角度)：到位後不切換停止模式，維持原速度續轉。
#   - motor_stop/drive_stop/stop_all(純停止指令)：不送出任何指令，讓馬達維持呼叫前的狀態
#     (還在轉就繼續轉、已經停了就繼續停著)——這是刻意合法的選項，不是無法辨識的錯誤。
# ⚠ 傳入真的無法辨識的值（範圍外的整數、亂打的字串等）才會印一行提示、自動改用 brake
# 煞車，不會中斷程式──brake 介於「滑行(可能讓機構掉落)」與「鎖死(可能卡住機構)」之間，
# 是誤用時最安全的預設。
# ⚠ Passing a genuinely unrecognized value (out-of-range int, garbage string, etc.) prints a
# notice and falls back to brake instead of raising — brake sits between "coast" (could let a
# mechanism drop) and "hold" (could jam one), making it the safest default on misuse. Note
# "continue" itself is NOT an unrecognized value: for pure-stop functions (motor_stop/
# drive_stop/stop_all) it means "send nothing, leave the motor exactly as it is."
STOP_COAST    = "coast"     # 滑行：切斷動力自由轉動 / coast            (= 1)
STOP_BRAKE    = "brake"     # 被動煞車：短路馬達兩端阻尼（傳 "break" 亦視為 brake）/ passive brake (= 2)
STOP_HOLD     = "hold"      # 位置鎖死：主動 PID 咬住當前角度 / actively hold position (= 3)
STOP_CONTINUE = "continue"  # 走角度：到位後續轉；純停止指令：不送指令、維持現狀 (= 4)

MODE_SPEED     = 0
MODE_ANGLE     = 1
MODE_ABS_ANGLE = 2
MODE_RESET     = 3
MODE_COLOR     = 10
MODE_RGB       = 11
MODE_HSV       = 12
MODE_DISTANCE  = 20
MODE_FORCE     = 30

# 多塊 EXP6 共用的時鐘與快取門檻（純數值/無狀態，可安全跨物件共用）
# Shared clock + cache threshold across boards (stateless, safe to share)
_cache_time = 0
_CACHE_MS   = 10
_watch      = _StopWatch()

def _clip(v):
    return max(0, min(100, int(v)))

def _is_near_zero(v, tol=1):
    return -tol <= v <= tol

# ── 參數驗證副程式 / Parameter validation helpers ──────────────────
# 分兩類：
#   _check_*  會拋 ValueError──用在「超出範圍就是靜默做錯事」的參數：孔位超界／
#             重複會撞上協議算式，別名成完全不相關的指令；mode 是離散合法值集合，
#             沒有「最接近」可猜。這三種沒有安全的預設值可以退，只能讓呼叫方立刻修正。
#   _clamp*   不拋錯──用在「夾到範圍邊界就是合理結果」的參數（speed/power/PID 倍率
#             夾到極值＝全速/全開，符合直覺）。_clamp_warn 額外印一行提示，只用在
#             「靜默夾範圍會嚴重改變行為」的參數（degrees 被截斷＝提早觸發煞車動作）。
#
# _check_* raises ValueError — for params where an out-of-range value silently does the
# wrong thing: duplicate/out-of-range ports alias into a totally unrelated command via the
# protocol's arithmetic encoding; mode is a discrete legal set with no "nearest" guess. None
# of these three have a safe fallback, so the caller must fix the input immediately.
# _clamp* never raises — for params where saturating at the boundary is a sensible result
# (speed/power/PID ratio clamped to the extreme = full speed/full gain, matches intuition).
# _clamp_warn additionally prints a notice, reserved for params where silent clamping would
# meaningfully change behavior (degrees getting truncated means the stop action fires early).
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
    # Parameter register: if this call's args exactly match the previous call's, reuse the
    # previously built packet instead of re-validating/re-computing. A failed call (raises
    # inside build_fn) is never cached, so the same bad input keeps failing on every call.
    cache = [None, None]   # [上次的參數 tuple, 上次的結果 / last args tuple, last result]
    def wrapper(*args):
        if cache[0] == args:
            return cache[1]
        result = build_fn(*args)
        cache[0], cache[1] = args, result
        return result
    return wrapper

# 停止狀態字串 → 韌體協議碼。兩種情境的編碼不同：
#   motor_stop / stop_all：單馬達指令的 L2（coast=3, hold=4, brake=5，見協議 L2 狀態）
#   motor_run_degrees：走角度 Method C 的 stop 欄位（coast=0, brake=1, hold=2, continue=3）
_STOP_L2         = {"coast": 3, "hold": 4, "brake": 5}
_STOP_COMPLETION = {"coast": 0, "brake": 1, "hold": 2, "continue": 3}
# 整數碼 → 停止狀態名。pyBricks 版採 1-based（跟本庫孔號 1~6 風格一致，較直覺）：
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

def _cmd_packet(l1, l2, l3, l4):
    l1, l2, l3, l4 = _clip(l1), _clip(l2), _clip(l3), _clip(l4)
    x = _CMD_HEADER ^ l1 ^ l2 ^ l3 ^ l4
    return bytes([_CMD_HEADER, l1, l2, l3, l4, x])

def _heartbeat_packet():
    x = _CMD_HEADER ^ _L1_HEARTBEAT
    return bytes([_CMD_HEADER, _L1_HEARTBEAT, 0, 0, 0, x])

def _parse_port_s32(raw, port_num):
    if raw is None or len(raw) < 32:
        return 0
    o = (port_num - 1) * 4
    low24 = raw[o] | (raw[o+1]<<8) | (raw[o+2]<<16)
    hi8 = raw[o+3]
    if hi8 >= 128:
        hi8 -= 256
    try:
        return low24 + hi8 * 16777216
    except OverflowError:
        # 某些裝置(如顏色感測器)把 32-bit 全部位元當「位元欄位」塞爆用(不是單純的有號整數量值)，
        # 重組出來的數值可能落在部分 MicroPython port(無 big-int 支援時)小整數(small int)代表
        # 範圍之外，純數學運算本身就會丟 OverflowError——不是我們邏輯錯，是平台整數位寬限制，
        # 且無法用改寫運算式(改用位元運算/拆步驟等)繞過，因為問題在「結果數值本身」，不是算法。
        # 馬達角度/距離/力量等真實場景數值都遠低於此門檻，只有顏色感測器這種刻意塞滿32-bit的
        # 封包才會踩到。get_port_raw() 是通用除錯用途，這裡犧牲最高位元組換取「不當機」：
        # 退回 24-bit 無號值(0~16777215)。顏色/超音波/力量各有專用函式(get_color_*/
        # get_ultrasonic_distance/get_touch_force)走各自獨立解析路徑，不受此限制影響，資料
        # 完全正確、不打折扣。
        return low24

def _parse_voltage(raw):
    if raw is None or len(raw) < 32:
        return 0.0
    r7_low16 = raw[24] | (raw[25] << 8)
    return (r7_low16 & 0x0FFF) / 100.0

def _parse_device_id(raw, port_num):
    if raw is None or len(raw) < 32:
        return DEV_NONE
    if port_num == 1:
        r7_low16 = raw[24] | (raw[25] << 8)
        return (r7_low16 >> 12) & 0x7
    else:
        r8_low16 = raw[28] | (raw[29] << 8)
        return (r8_low16 >> ((port_num - 2) * 3)) & 0x7

def _make_port_parser(dev_id, extract_fn, default):
    def _parser(raw, port_num):
        if dev_id is not None and _parse_device_id(raw, port_num) != dev_id:
            return default
        return extract_fn(raw, port_num)
    return _parser

def _parse_distance_logic(raw, port_num):
    dist = _parse_port_s32(raw, port_num)
    return 2000 if dist in (65535, -1) else dist

def _parse_abs_angle_logic(raw, port_num):
    # 韌體回傳的絕對角度 apos 是 LEGO 原生「-180~179」格式（CALIB 路徑也刻意
    # 對齊成這個範圍，見 STM32_exp6_dma.ino 第 1147-1149 行）。但本庫對外
    # 承諾的是 0~359 的「硬體朝向」（像羅盤），故在此統一換算。
    # Python 的 % 對負數取非負餘數：-180→180、-1→359、0→0、179→179，剛好
    # 把 -180~179 折到 0~359；而對「本來就是 0~359」的值則是恆等映射，所以
    # 不管韌體走 CALIB 還是 fallback 路徑，這個換算都正確。
    return _parse_port_s32(raw, port_num) % 360

_parse_motor_s32     = _make_port_parser(DEV_MOTOR,      _parse_port_s32, 0)
_parse_motor_abs_s32 = _make_port_parser(DEV_MOTOR,      _parse_abs_angle_logic, 0)
_parse_distance_s32  = _make_port_parser(DEV_ULTRASONIC, _parse_distance_logic, 0)
_parse_force_s32     = _make_port_parser(DEV_FORCE,      _parse_port_s32, 0)
_parse_raw_s32       = _make_port_parser(None,           _parse_port_s32, 0)

# ── 顏色封包（fmt=10，v3.0 RAW 32-bit bytes解析）──────────────────────
def _parse_color_packet(raw, port_num):
    if raw is None or len(raw) < 32:
        return 0, 0, 0, 0, 0
    o = (port_num - 1) * 4
    b0, b1, b2, b3 = raw[o], raw[o+1], raw[o+2], raw[o+3]
    cc = b3 >> 4
    refl = ((b3 & 0x0F) << 3) | (b2 >> 5)
    r = ((b2 & 0x1F) << 2) | (b1 >> 6)
    g = ((b1 & 0x3F) << 1) | (b0 >> 7)
    b = b0 & 0x7F
    return cc, refl, r, g, b

def _rgb_to_hsv(r, g, b):
    r_n, g_n, b_n = r / 100.0, g / 100.0, b / 100.0
    cmax = max(r_n, g_n, b_n)
    cmin = min(r_n, g_n, b_n)
    delta = cmax - cmin
    if delta == 0:
        h = 0
    elif cmax == r_n:
        h = 60 * (((g_n - b_n) / delta) % 6)
    elif cmax == g_n:
        h = 60 * (((b_n - r_n) / delta) + 2)
    else:
        h = 60 * (((r_n - g_n) / delta) + 4)
    s = 0 if cmax == 0 else (delta / cmax) * 100
    v = cmax * 100
    return int(h), int(s), int(v)

def _parse_color_code(raw, port_num): return _parse_color_packet(raw, port_num)[0]
def _parse_reflection(raw, port_num): return _parse_color_packet(raw, port_num)[1]
def _parse_rgb(raw, port_num):
    _, _, r, g, b = _parse_color_packet(raw, port_num)
    return (r, g, b)
def _parse_hsv(raw, port_num):
    _, _, r, g, b = _parse_color_packet(raw, port_num)
    return _rgb_to_hsv(r, g, b)
def _parse_color_red(raw, port_num): return _parse_color_packet(raw, port_num)[2]
def _parse_color_green(raw, port_num): return _parse_color_packet(raw, port_num)[3]
def _parse_color_blue(raw, port_num): return _parse_color_packet(raw, port_num)[4]
def _parse_color_hue(raw, port_num): return _parse_hsv(raw, port_num)[0]
def _parse_color_sat(raw, port_num): return _parse_hsv(raw, port_num)[1]
def _parse_color_val(raw, port_num): return _parse_hsv(raw, port_num)[2]

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
    # 方向 = 速度正負 × 角度正負：兩者符號相同→正轉(CW,8)，相異→反轉(CCW,9)。
    # (speed 正/deg 正)與(speed 負/deg 負)皆 CW；(speed 負/deg 正)與(speed 正/deg 負)皆 CCW。
    op  = 9 if (speed < 0) != (degrees < 0) else 8
    l3  = _stop_to_completion(stop) * 25 + deg // 100   # L3 = stop*25 + 百位(0~24)
    return (port_num * 10 + op, _clip(abs(int(speed))), l3, deg % 100)

def _build_motor_track_target(port_num, speed, angle, stop):
    _check_port(port_num)
    speed = _clamp(speed, -100, 100, "speed")
    # Method C(比照 motor_run_degrees)：目標角度只到 360，百位數只需 0~3，用「除4」
    # 把停止狀態疊進 L3(stop*4 + 百位，最大 3*4+3=15，遠低於 100)，不影響原本可表達範圍。
    # angle 本質上是週期性的(540°等於180°)，直接取模即可，不需要額外驗證/夾範圍。
    t = int(angle) % 360
    l3 = _stop_to_completion(stop) * 4 + t // 100
    return (port_num * 10 + 7, _clip(abs(int(speed))), l3, t % 100)

def _build_set_inverted(port_num, inverted):
    _check_port(port_num)
    return (76, port_num, 1 if inverted else 0, 0)

def _build_set_port_mode(port_num, mode):
    _check_port(port_num)
    _check_mode(mode)
    return (_L1_MODE_BASE + port_num, mode, 0, 0)

def _build_drive(left_port, right_port, left_speed, right_speed):
    _check_port(left_port, "left_port")
    _check_port(right_port, "right_port")
    _check_distinct_ports(left_port, right_port)
    left_speed = _clamp(left_speed, -100, 100, "left_speed")
    right_speed = _clamp(right_speed, -100, 100, "right_speed")
    l2 = (0 if left_speed  >= 0 else 2) | (0 if right_speed >= 0 else 1)
    return (left_port * 10 + right_port, l2, abs(left_speed), abs(right_speed))

def _build_drive_stop(left_port, right_port, stop):
    _check_port(left_port, "left_port")
    _check_port(right_port, "right_port")
    _check_distinct_ports(left_port, right_port)
    l2 = _stop_to_l2(stop)
    if l2 is None:              # stop=continue：維持現狀，不送任何指令
        return None
    return (left_port * 10 + right_port, l2, 0, 0)

# 參數暫存器：對每個 _build_* 分別套用獨立快取(見 _cache_validated 說明)，同一指令連續
# 呼叫相同參數時(例如控制迴圈每 tick 都送同樣的 drive(...))，第二次以後直接跳過驗證與組包運算。
_build_motor_run          = _cache_validated(_build_motor_run)
_build_motor_power        = _cache_validated(_build_motor_power)
_build_motor_stop         = _cache_validated(_build_motor_stop)
_build_motor_set_pid      = _cache_validated(_build_motor_set_pid)
_build_motor_run_degrees  = _cache_validated(_build_motor_run_degrees)
_build_motor_track_target = _cache_validated(_build_motor_track_target)
_build_set_inverted       = _cache_validated(_build_set_inverted)
_build_set_port_mode      = _cache_validated(_build_set_port_mode)
_build_drive              = _cache_validated(_build_drive)
_build_drive_stop         = _cache_validated(_build_drive_stop)

# ── 以下工廠函式產生的是「self 為第一參數」的方法，直接掛在
#    MBC_EXP6 類別上（見類別內 method = _make_xxx(...) 的寫法）。
#    每個物件各自持有獨立的連線狀態，工廠只負責產生共用邏輯。
# ── The factories below build methods (self as first arg) that get
#    attached directly onto MBC_EXP6 (see "method = _make_xxx(...)"
#    inside the class). Each instance keeps its own connection state;
#    the factories just supply the shared logic.

def _make_sensor_func(parse_fn):
    async def _async(self, port_num):
        if not self.connected and not await self._try_reconnect_async():
            self._warn_no_board()
            return parse_fn(None, port_num)   # None → parser 回傳正常型別的預設值
        raw = await self._read_async()
        return parse_fn(raw, port_num)

    def _func(self, port_num):
        _check_port(port_num)
        self._check_init()
        if self._async_mode:
            return _async(self, port_num)
        if not self.connected and not self._try_reconnect_sync():
            self._warn_no_board()
            return parse_fn(None, port_num)
        return parse_fn(self._read_sync(), port_num)

    return _func

def _make_mode_gated_func(required_mode, parse_fn):
    async def _async(self, port_num):
        if not self.connected and not await self._try_reconnect_async():
            self._warn_no_board()
            return parse_fn(None, port_num)
        if self._port_mode_cache.get(port_num) != required_mode:
            await self._send(*_build_set_port_mode(port_num, required_mode))
            self._port_mode_cache[port_num] = required_mode
            # 模式還沒真正切換前讀到的封包，是用「舊格式」編碼的，拿新格式
            # 的 parser 去解會是亂碼，所以必須先等到確實有新封包抵達。
            await self._wait_until_async(lambda raw: True, timeout_ms=100)
        raw = await self._read_async()
        return parse_fn(raw, port_num)

    def _func(self, port_num):
        _check_port(port_num)
        self._check_init()
        if self._async_mode:
            return _async(self, port_num)
        if not self.connected and not self._try_reconnect_sync():
            self._warn_no_board()
            return parse_fn(None, port_num)
        if self._port_mode_cache.get(port_num) != required_mode:
            self._send_sync(*_build_set_port_mode(port_num, required_mode))
            self._port_mode_cache[port_num] = required_mode
            self._wait_until(lambda raw: True, timeout_ms=100)
        return parse_fn(self._read_sync(), port_num)

    return _func

def _make_device_id_func():
    async def _async(self, port_num):
        if not self.connected and not await self._try_reconnect_async():
            self._warn_no_board()
            return _parse_device_id(None, port_num)
        return _parse_device_id(await self._read_async(), port_num)

    def _func(self, port_num):
        _check_port(port_num)
        self._check_init()
        if self._async_mode:
            return _async(self, port_num)
        if not self.connected and not self._try_reconnect_sync():
            self._warn_no_board()
            return _parse_device_id(None, port_num)
        return _parse_device_id(self._read_sync(), port_num)

    return _func

def _make_voltage_func():
    async def _async(self):
        if not self.connected and not await self._try_reconnect_async():
            self._warn_no_board()
            return _parse_voltage(None)
        return _parse_voltage(await self._read_async())

    def _func(self):
        self._check_init()
        if self._async_mode:
            return _async(self)
        if not self.connected and not self._try_reconnect_sync():
            self._warn_no_board()
            return _parse_voltage(None)
        return _parse_voltage(self._read_sync())

    return _func

def _make_send_func(build_fn):
    # 驗證一定要在 _func 裡先做完（不論最後走 sync 還是 async 路徑），不能延後到 _async
    # 內部才做：如果放在「還沒連線就 return」的分支之後，未連線時（例如板子還沒插上就先
    # 呼叫指令做離線測試）錯誤的參數會被整個跳過、完全不會被抓到。
    #
    # async 模式下 _func 一定要回傳「_async(...) 這個 coroutine」，不能直接回傳 None——
    # 呼叫端永遠是 await exp6.motor_stop(...) 這種寫法，若 _func 提早 return None，
    # await None 在 Pybricks/MicroPython 會直接拋 TypeError('NoneType' object isn't
    # iterable)。所以 pkt is None(stop=continue，維持現狀不送指令) 這個判斷要挪到
    # _async 內部做，讓 async 模式永遠拿到一個可以 await 的 coroutine。
    async def _async(self, pkt):
        if pkt is None:   # stop=continue（motor_stop/drive_stop）：維持現狀，不送任何指令
            return
        if not self.connected and not await self._try_reconnect_async():
            self._warn_no_board()
            return
        await self._send(*pkt)

    def _func(self, *args):
        self._check_init()
        pkt = build_fn(*args)
        if self._async_mode:
            return _async(self, pkt)
        if pkt is None:
            return
        if not self.connected and not self._try_reconnect_sync():
            self._warn_no_board()
            return
        self._send_sync(*pkt)

    return _func

def _make_stop_all_func():
    # 注意：刻意不送 (0,0,0,0)。協議上 L1:0（L2=L3=L4=0）是「全板硬重置」，
    # 除了讓馬達滑行，還會把所有孔的回傳格式打回 255(自動) 並把所有馬達
    # 的相對角度歸零（新版上下行協議.md 第 21-23 行）。本地的
    # _port_mode_cache 不會跟著這個重置同步，下次讀 MODE_SPEED /
    # MODE_ABS_ANGLE 的孔會誤用舊快取、把相對角度的 bytes 當成別的格式解，
    # 讀出錯誤數值且不會報錯；歸零角度也會打掉使用者自己在外部累計的里程。
    # stop_all() 是給使用者當「暫停鍵」用的，不該有這些副作用，所以改成
    # 對 6 個孔個別送單孔停止指令 coast/brake/hold（跟 motor_stop(port) 同一條路），
    # 這個指令範圍在協議上跟 L1:0 是分開的，沒有歸零角度/重置格式的問題。
    # （(0,0,0,0) 仍保留給 __init__ 開機、_emergency_stop 關機善後使用，
    # 那兩個時機本來就是想要打回乾淨已知狀態。）
    async def _async(self, stop):
        if _stop_to_l2(stop) is None:   # continue：維持所有馬達現狀，不送任何指令
            return
        if not self.connected and not await self._try_reconnect_async():
            self._warn_no_board()
            return
        for _ in range(3):
            for port_num in range(1, 7):
                await self._send(*_build_motor_stop(port_num, stop))

    def _func(self, stop):
        self._check_init()
        if self._async_mode:
            return _async(self, stop)
        if _stop_to_l2(stop) is None:
            return
        if not self.connected and not self._try_reconnect_sync():
            self._warn_no_board()
            return
        for _ in range(3):
            for port_num in range(1, 7):
                self._send_sync(*_build_motor_stop(port_num, stop))

    return _func

def _make_reset_angle_func():
    async def _async(self, pkt, port_num):
        if not self.connected and not await self._try_reconnect_async():
            self._warn_no_board()
            return
        await self._send(*pkt)
        self._port_mode_cache[port_num] = MODE_ANGLE   # 韌體歸零後自動切到 fmt=1，本地快取同步，避免下次多送一次 set_mode
        await self._wait_until_async(lambda raw: _is_near_zero(_parse_motor_s32(raw, port_num)))

    def _func(self, port_num):
        self._check_init()
        pkt = _build_set_port_mode(port_num, MODE_RESET)   # 先驗證 port_num，未連線時也不例外
        if self._async_mode:
            return _async(self, pkt, port_num)
        if not self.connected and not self._try_reconnect_sync():
            self._warn_no_board()
            return
        self._send_sync(*pkt)
        self._port_mode_cache[port_num] = MODE_ANGLE
        self._wait_until(lambda raw: _is_near_zero(_parse_motor_s32(raw, port_num)))

    return _func

def _make_set_inverted_func():
    # set_motor_inverted(port, inverted=True)：設定馬達正/反裝。先記住設定（斷線重連時據此重送），
    # 再送 L1=76 給韌體。韌體是反轉的單一真理來源，Python 只負責 API 與熱插拔復原。
    async def _async(self, pkt):
        if not self.connected and not await self._try_reconnect_async():
            self._warn_no_board()
            return
        await self._send(*pkt)

    def _func(self, port_num, inverted=True):
        self._check_init()
        inverted = bool(inverted)
        pkt = _build_set_inverted(port_num, inverted)   # 先驗證 port_num，通過才更新本地狀態
        self._inverted[port_num] = inverted   # 即使當下未連線也記住，重連時重送
        if self._async_mode:
            return _async(self, pkt)
        if not self.connected and not self._try_reconnect_sync():
            self._warn_no_board()
            return
        self._send_sync(*pkt)

    return _func

_STOP_DRAIN_MS = 30
_STOP_GAP_MS   = 2
_FEED_MS    = 50
_SILENCE_MS = 100

# 開機偵測擴充板：策反成功的板子會每 10ms 送一包上行串流，在此時間內
# 收到任一合法封包即視為板子存在。板子通常早已上電、幾十~上百 ms 內就
# 回應，500ms 已有充足餘裕；只有真的沒板子才會等滿。
_PROBE_TIMEOUT_MS = 500
_PROBE_KICK_MS    = 50    # 偵測期間補送策反包的間隔（確保冷啟動也能被策反）

# 未連線時，讀取/設定都會跳出錯誤訊息，但限流為最多每 _ERR_REPEAT_MS 印一次，
# 避免使用者在 50ms 輪詢迴圈裡被同一行洗版。讀取回傳值維持正常型別的預設值。
_ERR_REPEAT_MS = 1000

# 6-byte v4.0 策反包（喚醒 legoEmulator 切到 PROTO_PYBRICKS_RAW）。單獨定義成常數，
# 供 _open_uart 開機、_probe_board 偵測、_try_reconnect_* 熱插拔恢復共用。
# 內容等同 _heartbeat_packet()（L1=1，無副作用）：legoEmulator 兩處策反偵測
# （EV3UARTEmulationHard.cpp 的 CONN_WAIT_SYNC / CONN_CONNECTED）已同步改成
# 辨識 6-byte 格式 [0xA5,L1,L2,L3,L4,XOR]，任何合法 v4.0 封包皆可觸發策反，
# 不再需要 7-byte 特製握手包。
_MUTINY_PKT = bytes([0xA5, 0x01, 0x00, 0x00, 0x00, 0xA4])

# 熱插拔自動恢復：開機時沒插板子（connected=False）後，若使用者中途才把板子插上，
# 每次讀取/設定（async 另含背景保活迴圈）會週期性補送策反包喚醒它，收到上行串流
# 即翻回 connected。kick 間隔 100ms：板子 CONN_WAIT_SYNC 窗約 500ms，每窗可收到
# 多次 kick，可靠觸發策反；且非阻塞，不會拖慢 50ms 輪詢。
_RECONNECT_KICK_MS = 100

# 斷線偵測：connected 時，若「主動 drain 後」仍超過此時間沒收到任何新的合法上行
# 串流，判定板子被拔掉、翻回 connected=False。板子每 10ms 送一包，400ms=40 包沒到，
# 遠超任何短暫延遲，門檻夠寬避免誤判；同時比韌體看門狗 500ms 早一步反應。
_DISCONNECT_MS = 400

# 所有已建立的 MBC_EXP6 物件 / All MBC_EXP6 instances created so far.
# run_task() 用它來同時餵所有板子的心跳、收尾時同時緊急停止所有板子；
# 舊版「函式式」API（exp_motor_run 等）也用它來找「目前的預設板子」。
_instances = []

def _default_instance():
    if not _instances:
        raise RuntimeError(
            "EXP6 尚未初始化。請先建立 MBC_EXP6(port, multitask) 物件。/ EXP6 is not set up: create MBC_EXP6(port, multitask) first."
        )
    return _instances[-1]

def exp_run_task(coro):
    return run_task(coro)

_loop_running = False

def run_task(coro):
    global _loop_running

    async def _user_wrapper():
        global _loop_running
        try:
            await coro
        finally:
            _loop_running = False              # main 結束 → 通知心跳收手

    async def _runner():
        global _loop_running
        _loop_running = True
        loops = [inst._keep_alive_loop() for inst in _instances]
        await _multitask(_user_wrapper(), *loops)   # gather，不用 race

    try:
        return _pb_run_task(_runner())
    finally:
        _loop_running = False
        for inst in _instances:
            inst._uart_busy = False
        if _instances:
            wait(_STOP_DRAIN_MS)
            for inst in _instances:
                inst._emergency_stop()

def keep_alive():
    return _default_instance().keep_alive()

def keep_alive_wait(ms):
    return _default_instance().keep_alive_wait(ms)


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

    def __init__(self, port, multitask=False):
        self._port = port
        self._multitask = bool(multitask)

        # per-instance 連線狀態：每個物件各自一份，互不影響
        # per-instance connection state, isolated from other boards
        self._uart          = None
        self._rx             = bytearray()
        self._last_tx_ms     = 0
        self._selected_port  = None
        self._async_mode     = False
        self._cache          = None
        self._cache_time     = 0
        self._port_mode_cache = {}
        self._inverted        = {i: False for i in range(1, 7)}   # 各孔正/反裝，預設全部正裝；斷線重連時據此重送給韌體
        self._uart_busy      = False
        self.connected       = True    # 開機偵測結果；偵測不到擴充板時設 False（見下）
        self._err_last_ms    = -_ERR_REPEAT_MS   # 上次印錯誤訊息的時間（限流用；初值確保首次立即印）
        self._reconnect_last_ms = -_RECONNECT_KICK_MS  # 上次補送策反包喚醒熱插板子的時間

        # 未連線提示 / 重新連線提示：不論開機時是否偵測到都先建好，讓熱插拔恢復
        # 機制隨時可用（開機沒插→中途插上→自動翻回 connected 並印「已重新連線」）。
        letter = _PORT_LETTER.get(port, str(port))
        self._err_msg = ("[EXP6] Port {L}（孔{n}）未偵測到擴充板 / "
                         "No EXP6 board detected".format(L=letter, n=port))
        self._reconn_msg = ("[EXP6] Port {L}（孔{n}）已重新連線 / "
                            "EXP6 board reconnected".format(L=letter, n=port))
        self._disc_msg = ("[EXP6] Port {L}（孔{n}）連線中斷（板子被拔掉？）/ "
                          "EXP6 board disconnected".format(L=letter, n=port))

        self._open_uart(port, self._multitask)

        # 開機偵測：確認這個孔真的插著 EXP6 擴充板。策反成功的板子會持續送
        # 上行串流封包，_probe_board 在 timeout 內收到任一合法封包即視為存在。
        # 偵測不到時：印一次中英文提示、不中斷程式。之後每個讀取/設定函式會先嘗試
        # 熱插拔重連（_try_reconnect_*），失敗才呼叫 _warn_no_board() 跳出提示（限流）、
        # 讀取回傳正常型別的預設值（0 / 0.0 / (0,0,0)…）；一旦板子被插上並策反成功，
        # 自動翻回 connected 並恢復正常運作。
        self.connected = self._probe_board()
        if not self.connected:
            self._warn_no_board()

        if self.connected:
            # 開機歸零：上位機啟動當下，擴充板上的馬達很可能已經被手動轉動過，
            # 故藉由全停指令把 6 個孔位的相對角度歸零，避免帶著舊角度啟動。
            #
            # 注意：歸零完全交給這行 (0,0,0,0) 來做，不要額外對每個孔逐一送
            # MODE_RESET (L2=3)！韌體端 (0,0,0,0) 的處理本來就會把所有孔的
            # 回傳格式打回 255(自動)，並且只對「目前偵測到接著馬達」的孔位
            # 做角度歸零（STM32_exp6_dma.ino case 0：逐孔檢查 get_port_id==5
            # 才歸零），不會動到其他孔的格式。
            # 如果改成逐孔送 MODE_RESET，韌體那邊的 case 70~75 對 L2==3 的處理
            # 「不論裝置種類」都會把該孔的回傳格式強制設成 1(角度)（同檔案
            # case 70~75）；超音波/力量感測器的孔會因此被切到角度格式，而
            # get_ultrasonic_distance / get_touch_force 是用 _make_sensor_func
            # 做的，沒有 mode-gating 自我修復機制，會永久讀到 0，直到使用者
            # 自己手動呼叫 set_port_mode 校正回來為止。曾經在這裡加過這段逐孔
            # 迴圈，已確認是錯的，刪掉。
            self._send_sync(0, 0, 0, 0)

            # 等待韌體真正完成歸零（見 _wait_until 的說明）；沒接馬達的孔位
            # 在 _parse_motor_s32 的裝置 ID 閘門下天生讀到 0，直接視同通過。
            self._wait_until(
                lambda raw: all(_is_near_zero(_parse_motor_s32(raw, p)) for p in range(1, 7))
            )

        _instances.append(self)

    # ── 內部：初始化 / 連線狀態檢查 ─────────────────────────────
    def _open_uart(self, port, multitask):
        if port not in _PORT_MAP:
            raise ValueError("port 必須為 1~6（對應 Port.A~F），收到：{} / port must be 1~6 (Port.A~F)".format(port))
        self._selected_port = port
        self._async_mode    = bool(multitask)
        self._uart          = UARTDevice(_PORT_MAP[port], baudrate=_BAUD)
        self._rx             = bytearray()
        self._cache          = None
        self._cache_time     = 0
        self._last_tx_ms     = 0
        self._uart_busy      = False
        self._port_mode_cache = {}
        self._uart.clear()

        # =========================================================
        # 🚀 【SDK 實體盲讀策反補丁】
        # 送出 6-byte v4.0 策反包 [0xA5,L1,L2,L3,L4,XOR]（等同一包心跳）。
        # legoEmulator（EV3UARTEmulationHard.cpp 的 CONN_WAIT_SYNC / CONN_CONNECTED
        # 兩處策反偵測）已同步改成辨識 6-byte XOR 校驗：0xA5^L1^L2^L3^L4==XOR，
        # 任何合法 v4.0 封包皆可觸發策反。策反包由 legoEmulator 消費，不進
        # comm_poll_command，後續所有指令仍用 v4.0 6-byte 格式，不需改動。
        # =========================================================
        self._uart.write(_MUTINY_PKT)
        # =========================================================

    def _check_init(self):
        if self._uart is None:
            letter = _PORT_LETTER.get(self._selected_port, str(self._selected_port)) if self._selected_port else "?"
            raise RuntimeError(
                "EXP6 未初始化（Port {}）。請先建立 MBC_EXP6(port, multitask) 物件。/ EXP6 is not set up: create MBC_EXP6(port, multitask) first.".format(letter)
            )

    def _probe_board(self, timeout_ms=_PROBE_TIMEOUT_MS):
        # 偵測這個孔是否真的插著 EXP6 擴充板：策反成功的板子會每 10ms 送一包
        # 上行串流封包 [0x5A][32][...][xor]；沒插板子（或插了別的裝置、策反未
        # 成功）則永遠收不到。收到任一合法封包即回傳 True，逾時回傳 False。
        #
        # 期間每 _PROBE_KICK_MS 補送一次策反包，確保韌體冷啟動、仍卡在
        # legoEmulator 握手（CONN_WAIT_SYNC）時有機會被策反。策反成功後再送這
        # 包也無害：RAW 模式下 comm_poll_command 會因 XOR 不符直接丟棄，不影響
        # 任何狀態。__init__ 不能 await，故此處用同步 wait/_drain_and_parse。
        self._uart.clear()
        self._rx = bytearray()
        self._cache = None
        t0 = _watch.time()
        last_kick = t0 - _PROBE_KICK_MS   # 讓第一輪立即補送一次
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
        # 未連線時的統一警示：跳出一行中英文錯誤訊息。為避免 50ms 輪詢迴圈把
        # 同一行洗版，限流成最多每 _ERR_REPEAT_MS 印一次（首次一定會印）。
        # 讀取函式呼叫本方法後仍回傳正常型別的預設值，不改變回傳型別。
        now = _watch.time()
        if now - self._err_last_ms >= _ERR_REPEAT_MS:
            print(self._err_msg)
            self._err_last_ms = now

    def _on_reconnected(self):
        # 熱插拔恢復：偵測到板子回來（收到合法上行串流）時呼叫。翻回 connected、
        # 清掉 _port_mode_cache（板子剛策反、韌體把所有孔格式打回自動，本地快取
        # 已失效，須重問模式）。
        self.connected = True
        self._port_mode_cache = {}
        print(self._reconn_msg)

    def _resend_inverted_sync(self):
        # 熱插拔恢復後，把使用者設過的反轉狀態重新送給剛策反的板子（韌體重置後 is_inverted 全 false）。
        for p, inv in self._inverted.items():
            if inv:
                self._send_sync(*_build_set_inverted(p, True))

    async def _resend_inverted_async(self):
        for p, inv in self._inverted.items():
            if inv:
                await self._send(*_build_set_inverted(p, True))

    def _on_disconnected(self):
        # 斷線偵測：connected 期間偵測到板子被拔掉（超過 _DISCONNECT_MS 沒新串流）
        # 時呼叫。翻回 connected=False、印一次中斷提示。之後讀取/設定走未連線路徑
        # （回預設值 + 週期補送策反包嘗試重連），板子重插上即自動恢復。
        self.connected = False
        # 關鍵：清掉舊快取。否則 _cache 仍保留斷線前最後一包，_try_reconnect_* 的
        # 「if self._cache is not None」會被這包舊資料誤判成「已收到串流→已重連」，
        # 拔掉後下一次讀取就假性恢復。清成 None 後，重連只有真的 drain 到新封包才成立。
        self._cache = None
        self._reconnect_last_ms = -_RECONNECT_KICK_MS   # 讓重連立即開始 kick
        self._err_last_ms = _watch.time()               # 剛印過中斷提示，暫緩 no-board 洗版
        print(self._disc_msg)

    def _check_disconnect(self):
        # 只能在「剛主動 drain 過」之後呼叫才有意義——串流中的板子會在該次 drain
        # 更新 _cache_time，故 now-_cache_time 很小；只有真的沒串流才會超過門檻。
        # （若在沒 drain 的情況下呼叫，cache 老只是因為沒去讀，會誤判。）
        if self.connected and (_watch.time() - self._cache_time) > _DISCONNECT_MS:
            self._on_disconnected()

    def _try_reconnect_sync(self):
        # 未連線時的輕量非阻塞重連嘗試（同步版，供 sync 模式讀取/設定守衛用）。
        # 週期性補送策反包喚醒後來才插上的板子，並 drain 一次；收到合法
        # 上行串流即視為板子回來、翻回 connected 回傳 True。沒有 wait()，跨多次
        # 呼叫累積 kick，不會拖慢輪詢。
        now = _watch.time()
        if now - self._reconnect_last_ms >= _RECONNECT_KICK_MS:
            try:
                self._uart.write(_MUTINY_PKT)
            except OSError:
                pass
            self._reconnect_last_ms = now
        self._drain_and_parse()
        if self._cache is not None:
            self._on_reconnected()
            self._resend_inverted_sync()
            return True
        return False

    async def _try_reconnect_async(self):
        # 同 _try_reconnect_sync，但 kick 走 async 寫入路徑（帶 _uart_busy 鎖），
        # 避免與背景保活迴圈/使用者送指令的 await 寫入交錯。
        now = _watch.time()
        if now - self._reconnect_last_ms >= _RECONNECT_KICK_MS:
            await self._uart_write_async(_MUTINY_PKT)
            self._reconnect_last_ms = now
        self._drain_and_parse()
        if self._cache is not None:
            self._on_reconnected()
            await self._resend_inverted_async()
            return True
        return False

    # ── 內部：raw UART 收發 ────────────────────────────────────
    async def _uart_write_async(self, data):
        while self._uart_busy:
            await wait(0)
        self._uart_busy = True
        try:
            await self._uart.write(data)
            self._last_tx_ms = _watch.time()
        finally:
            self._uart_busy = False

    def _send_sync(self, l1, l2, l3, l4):
        if self._uart is None:
            return
        self._uart.write(_cmd_packet(l1, l2, l3, l4))
        self._last_tx_ms = _watch.time()

    async def _send(self, l1, l2, l3, l4):
        if self._uart is None:
            return
        await self._uart_write_async(_cmd_packet(l1, l2, l3, l4))

    def _drain_and_parse(self):
        data = self._uart.read_all()
        if data:
            self._rx.extend(data)
        rx = self._rx
        i = 0
        n = len(rx)
        latest = None
        while True:
            while i < n and rx[i] != _STREAM_HEADER:
                i += 1
            if i >= n:
                self._rx = bytearray()
                if latest is not None:
                    self._cache = latest
                    self._cache_time = _watch.time()
                return
            if n - i < 2:
                break
            length = rx[i + 1]
            total = 2 + length + 1
            if length != _STREAM_PAYLOAD:
                i += 1
                continue
            if n - i < total:
                break
            x = 0
            for k in range(i + 1, i + 1 + length + 1):
                x ^= rx[k]
            if x == rx[i + 2 + length]:
                # 雙重確認：checksum 只有 1 byte，巧合對上的機率不是 0
                # （角度資料中剛好出現 0x5A、剛好下一 byte 等於 32、剛好
                # checksum 也對上）。真封包彼此首尾相連，所以再驗證緊接
                # 在這包後面的下一個 byte 是否也是同步頭（或乾脆已經是
                # 緩衝區結尾，代表這就是目前最新一包）；通不過就視為巧
                # 合誤判，不採信、只往後移 1 byte 重新對齊，避免把這 35
                # bytes 整包當真吃掉、誤跳過真正的下一包。
                next_i = i + total
                if next_i >= n or rx[next_i] == _STREAM_HEADER:
                    latest = bytes(rx[i + 2 : i + 2 + length])
                    i = next_i
                else:
                    i += 1
            else:
                i += 1
        self._rx = bytearray(rx[i:]) if i > 0 else rx
        if latest is not None:
            self._cache = latest
            self._cache_time = _watch.time()

    def _maybe_heartbeat_sync(self):
        if _watch.time() - self._last_tx_ms > _HEARTBEAT_MS:
            self._uart.write(_heartbeat_packet())
            self._last_tx_ms = _watch.time()

    async def _maybe_heartbeat_async(self):
        if _watch.time() - self._last_tx_ms > _HEARTBEAT_MS:
            await self._uart_write_async(_heartbeat_packet())

    async def _read_async(self):
        now = _watch.time()
        if self._cache is not None and (now - self._cache_time) < _CACHE_MS:
            await self._maybe_heartbeat_async()
            return self._cache
        self._drain_and_parse()
        self._check_disconnect()   # 剛 drain 過，可靠判斷板子是否還在串流
        await self._maybe_heartbeat_async()
        return self._cache

    def _read_sync(self):
        now = _watch.time()
        if self._cache is not None and (now - self._cache_time) < _CACHE_MS:
            self._maybe_heartbeat_sync()
            return self._cache
        self._drain_and_parse()
        self._check_disconnect()   # 剛 drain 過，可靠判斷板子是否還在串流
        self._maybe_heartbeat_sync()
        return self._cache

    # ── 內部：送出會改變孔位回傳格式的指令（mode 切換 / 角度歸零）之後，
    #    用來等待韌體「真正套用完成」的共用邏輯。
    #
    #    背景：韌體在指令送達、處理完成之前，仍會持續用「舊格式/舊數值」發
    #    串流封包；而 SPIKE 端硬體 UART 緩衝區容量有限，滿了之後新進（已經
    #    生效的）封包反而會被丟棄、卡在最前面的是舊封包。所以不能單純
    #    wait() 賭時間就去讀——必須先讓出 min_wait_ms 讓韌體確實處理完，
    #    再清空緩衝區、只看「清空之後才抵達」的新封包是否符合 predicate。
    #
    #    注意執行順序：一定要「先 wait 再 clear」，不能「先 clear 再在迴圈裡
    #    用 elapsed >= min_wait_ms 當作放行條件」——後者有個邏輯漏洞：迴圈
    #    一開始就可能抓到一包還沒切換完成的舊封包存進 self._cache，若接下來
    #    幾輪剛好沒有新資料進來（self._cache 不會被覆寫，因為
    #    _drain_and_parse 只在「真的抓到新封包」時才更新它），等
    #    elapsed >= min_wait_ms 成立的那一刻，predicate 拿到的其實還是最早
    #    那包舊資料——min_wait_ms 形同虛設，舊資料被「偷渡」過關。
    #    （這個寫法在串流頻率夠快、樣本夠密集時通常不會踩到，但只要某次
    #    輪詢剛好卡在新資料抵達的空檔，就會誤判通過。）
    #    timeout_ms 是 min_wait_ms 之後再給的安全上限，逾時就放棄等待、
    #    直接讓呼叫端使用當下最新的資料，不會卡死（總耗時上限約為
    #    min_wait_ms + timeout_ms）。
    #
    #    Shared logic for waiting until firmware has actually applied a
    #    mode-switch / angle-reset command, used after any port command
    #    that changes what a port streams back.
    #
    #    Why: between sending the command and firmware applying it, the
    #    board keeps streaming OLD data. SPIKE's hardware UART buffer is
    #    small, so once it's full, newly-arrived (already-updated) packets
    #    get dropped while stale ones sit at the front. So instead of
    #    gambling on wait(), we first yield min_wait_ms for firmware to
    #    actually finish, THEN clear the buffers, and only accept a packet
    #    that arrived after that clear and satisfies `predicate`.
    #
    #    Ordering matters: it must be "wait, then clear" — not "clear, then
    #    gate on elapsed >= min_wait_ms inside the loop". The latter has a
    #    logic bug: the very first iteration can capture a not-yet-switched
    #    packet into self._cache, and if no further bytes happen to arrive
    #    for the next few polls (self._cache is only overwritten when
    #    _drain_and_parse finds an actual new packet), `predicate` ends up
    #    evaluating that same stale packet the moment elapsed crosses
    #    min_wait_ms — min_wait_ms becomes a no-op and stale data sneaks
    #    through. `timeout_ms` is the safety bound applied after the
    #    min_wait_ms settle period, so a never-satisfied predicate can't
    #    hang forever (total worst case is roughly min_wait_ms + timeout_ms).
    def _wait_until(self, predicate, timeout_ms=200, poll_ms=5, min_wait_ms=15):
        if min_wait_ms > 0:
            wait(min_wait_ms)
        self._uart.clear()
        self._rx = bytearray()
        self._cache = None
        t0 = _watch.time()
        while True:
            elapsed = _watch.time() - t0
            if elapsed >= timeout_ms:
                return False
            self._drain_and_parse()
            if self._cache is not None and predicate(self._cache):
                return True
            wait(poll_ms)

    async def _wait_until_async(self, predicate, timeout_ms=200, poll_ms=5, min_wait_ms=15):
        if min_wait_ms > 0:
            await wait(min_wait_ms)
        self._uart.clear()
        self._rx = bytearray()
        self._cache = None
        t0 = _watch.time()
        while True:
            elapsed = _watch.time() - t0
            if elapsed >= timeout_ms:
                return False
            self._drain_and_parse()
            if self._cache is not None and predicate(self._cache):
                return True
            await wait(poll_ms)

    async def _feed_async(self):
        if self._uart is None:
            return
        await self._uart_write_async(_heartbeat_packet())

    def _emergency_stop(self, times=10, gap_ms=_STOP_GAP_MS):
        # 注意：這裡直接寫 _uart、不走 _send_sync，是為了在這個「關機善後」路徑
        # 單獨吞掉 EBUSY 並重試。EBUSY 來源：async 背景心跳的 await _uart.write()
        # 在 run_task 收尾(race=True)被取消時，Python 層 _uart_busy 會於 finally 清掉，
        # 但底層 UART 裝置短暫殘留 busy 還沒釋放；緊接著的同步 _uart.write() 就撞上
        # [Errno 16] EBUSY。該殘留通常要等「下一次實際對 UART 動作」才會被推進釋放，
        # 純 wait 不會觸發，所以這裡用「重試」而非「等更久」。裝置數毫秒內就會放開。
        if self._uart is None:
            return
        pkt   = _cmd_packet(0, 0, 0, 0)
        sent  = 0
        guard = 0
        while sent < times and guard < times + 30:   # guard：上限保護，永不無限迴圈
            guard += 1
            try:
                self._uart.write(pkt)
                self._last_tx_ms = _watch.time()
                sent += 1
                if gap_ms and sent < times:
                    wait(gap_ms)
            except OSError:
                wait(2)        # 裝置仍 busy（剛被取消的心跳寫入未釋放）；讓底層收尾後再試

    async def _keep_alive_loop(self, check_ms=_FEED_MS):
        while _loop_running:
            try:
                if self._uart is not None:
                    if not self.connected:
                        # 背景熱插拔恢復：即使主程式沒在讀取，也持續嘗試把後來
                        # 插上的板子策反回來，收到串流即自動翻回 connected。
                        await self._try_reconnect_async()
                    elif (_watch.time() - self._last_tx_ms) >= _SILENCE_MS:
                        await self._feed_async()
            except Exception:
                pass
            await wait(check_ms)

    # ── keep-alive（手動保活）──────────────────────────────────
    def keep_alive(self):
        if self._async_mode:
            return self._feed_async()
        if self._uart is None:
            return None
        self._uart.write(_heartbeat_packet())
        self._last_tx_ms = _watch.time()
        return None

    def _keep_alive_wait_sync(self, ms):
        if self._uart is None:
            wait(ms)
            return
        self.keep_alive()
        end = _watch.time() + ms
        while True:
            remain = end - _watch.time()
            if remain <= 0:
                return
            wait(remain if remain < _FEED_MS else _FEED_MS)
            self.keep_alive()

    async def _keep_alive_wait_async(self, ms):
        if self._uart is None:
            await wait(ms)
            return
        await self._feed_async()
        end = _watch.time() + ms
        while True:
            remain = end - _watch.time()
            if remain <= 0:
                return
            await wait(remain if remain < _FEED_MS else _FEED_MS)
            await self._feed_async()

    def keep_alive_wait(self, ms):
        if self._async_mode:
            return self._keep_alive_wait_async(ms)
        self._keep_alive_wait_sync(ms)
        return None

    def run_task(self, coro):
        return exp_run_task(coro)

    # 馬達指令 / Motor commands
    #   motor_run(port, speed)             開迴路定速 -100~100 / open-loop run, -100~100
    #   motor_power(port, power)           PWM 直驅 -100~100 / direct PWM drive, -100~100
    #   motor_stop(port, stop)             停止（stop 必填：1滑行/2煞車/3鎖死/4維持現狀(不送指令)，或字串 STOP_*）
    #                                      stop (required): 1=coast / 2=brake / 3=hold / 4=continue(no-op, sends nothing)
    #   motor_set_pid(port, p, i, d)       PID 倍率 0~100（50=原廠值,0=關閉,100=2倍）
    #                                      PID ratio 0~100 (50=factory default, 0=off, 100=2x)
    #   motor_run_degrees(port, speed, degrees, stop)  相對角度 -2499~2499，stop 必填
    #                                      stop：1滑行/2煞車/3鎖死/4續轉（或 STOP_COAST/BRAKE/HOLD/CONTINUE）
    #   motor_track_target(port, speed, angle, stop)  絕對角度(最短路徑) 0~360，stop 必填
    #                                      stop：1滑行/2煞車/3鎖死/4續轉（或 STOP_COAST/BRAKE/HOLD/CONTINUE）
    #                                      （舊名 motor_run_target 仍可用）/ (old name motor_run_target still works)
    #   set_motor_inverted(port, inverted) 設定正/反裝（反裝時指令與回傳自動反向）/ set motor direction
    #   set_port_mode(port, mode)          設定該孔回傳格式 / set this port's data format
    #   drive(l_port, r_port, l_speed, r_speed)  雙馬達同步驅動 / synchronized dual-motor drive
    #   drive_stop(l_port, r_port, stop)   雙馬達停止，stop 必填，同 motor_stop 的 4 種狀態
    motor_run          = _make_send_func(_build_motor_run)
    motor_power        = _make_send_func(_build_motor_power)
    motor_stop         = _make_send_func(_build_motor_stop)
    motor_set_pid      = _make_send_func(_build_motor_set_pid)
    motor_run_degrees  = _make_send_func(_build_motor_run_degrees)
    motor_track_target = _make_send_func(_build_motor_track_target)
    motor_run_target   = motor_track_target   # 相容別名（舊名）/ backward-compat alias
    set_motor_inverted = _make_set_inverted_func()
    set_port_mode      = _make_send_func(_build_set_port_mode)
    drive              = _make_send_func(_build_drive)
    drive_stop         = _make_send_func(_build_drive_stop)

    # stop_all(stop)：全部馬達緊急停止，stop 必填，同 motor_stop 的 4 種狀態 / emergency-stop all motors
    stop_all    = _make_stop_all_func()
    # reset_angle(port)：將該孔馬達的相對角度歸零 / zero this port's motor relative angle
    reset_angle = _make_reset_angle_func()

    # 感測器讀值 / Sensor readings
    #   get_port_raw(port)            原始數值（依該孔回傳格式）/ raw value (per port's format)
    #   get_ultrasonic_distance(port) 距離 mm / distance in mm
    #   get_touch_force(port)         力量感應器讀值 / touch force sensor reading
    get_port_raw            = _make_sensor_func(_parse_raw_s32)
    get_ultrasonic_distance = _make_sensor_func(_parse_distance_s32)
    get_touch_force         = _make_sensor_func(_parse_force_s32)

    # 顏色感應器 / Color sensor（數值皆 0~100，顏色色碼 0~10 為硬體原生分類）
    # Color sensor (all values 0~100; color code 0~10 is the hardware's built-in classification)
    get_color_color      = _make_mode_gated_func(MODE_COLOR, _parse_color_code)
    get_color_reflection = _make_mode_gated_func(MODE_COLOR, _parse_reflection)

    get_color_rgb = _make_mode_gated_func(MODE_COLOR, _parse_rgb)
    get_color_hsv = _make_mode_gated_func(MODE_COLOR, _parse_hsv)

    get_color_hue = _make_mode_gated_func(MODE_COLOR, _parse_color_hue)
    get_color_sat = _make_mode_gated_func(MODE_COLOR, _parse_color_sat)
    get_color_val = _make_mode_gated_func(MODE_COLOR, _parse_color_val)

    get_color_red   = _make_mode_gated_func(MODE_COLOR, _parse_color_red)
    get_color_green = _make_mode_gated_func(MODE_COLOR, _parse_color_green)
    get_color_blue  = _make_mode_gated_func(MODE_COLOR, _parse_color_blue)

    # get_device_id(port)：該孔裝置類型 / device type on this port
    # get_voltage()：擴充板電壓 (V) / expansion board voltage (V)
    get_device_id = _make_device_id_func()
    get_voltage   = _make_voltage_func()

    # 馬達讀值 / Motor readings
    #   get_motor_angle(port)      相對角度，可用 reset_angle 歸零 / relative angle, zero it with reset_angle
    #   get_motor_abs_angle(port)  絕對角度 0~359，馬達硬體朝向，不受 reset_angle 影響
    #                              absolute angle 0~359, the motor's physical heading, unaffected by reset_angle
    #   get_motor_speed(port)      目前轉速 / current speed
    get_motor_angle     = _make_mode_gated_func(MODE_ANGLE,     _parse_motor_s32)
    get_motor_abs_angle = _make_mode_gated_func(MODE_ABS_ANGLE, _parse_motor_abs_s32)
    get_motor_speed     = _make_mode_gated_func(MODE_SPEED,     _parse_motor_s32)


# ── 舊版「函式式」相容層 / Legacy procedural-API compatibility layer ──
# 這些 exp_* 自由函式只認「最後建立的那個 MBC_EXP6 物件」，僅供單板、
# 舊腳本相容使用。若同時操作多塊板子，請改用各物件自己的方法
# （exp6_a.motor_run(...) / exp6_b.motor_run(...)），不要混用這些函式。
# These exp_* free functions always act on "the most recently created
# MBC_EXP6 instance" — single-board legacy use only. For multiple boards,
# call methods on each object directly instead of these free functions.
def exp_motor_run(*a):          return _default_instance().motor_run(*a)
def exp_motor_power(*a):        return _default_instance().motor_power(*a)
def exp_motor_stop(*a):         return _default_instance().motor_stop(*a)
def exp_motor_set_pid(*a):      return _default_instance().motor_set_pid(*a)
def exp_motor_run_degrees(*a):  return _default_instance().motor_run_degrees(*a)
def exp_motor_track_target(*a): return _default_instance().motor_track_target(*a)
def exp_motor_run_target(*a):   return _default_instance().motor_run_target(*a)   # 相容別名 / alias
def exp_set_motor_inverted(*a): return _default_instance().set_motor_inverted(*a)
def exp_set_port_mode(*a):      return _default_instance().set_port_mode(*a)
def exp_drive(*a):              return _default_instance().drive(*a)
def exp_drive_stop(*a):         return _default_instance().drive_stop(*a)
def exp_stop_all(*a):           return _default_instance().stop_all(*a)
def exp_reset_angle(*a):        return _default_instance().reset_angle(*a)

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

# ════════════════════════════════════════════════════════════════
#  MBC_EXP6 物件可用函數列表 / Available Object Methods
# ════════════════════════════════════════════════════════════════
#
# 【系統與連線 / System & Connection】
#   keep_alive_wait(ms)
#       - 說明：在一般模式下取代 wait()，等待期間會自動維持連線心跳。
#       - Desc: Replaces wait() in sync mode, automatically sending heartbeats.
#       - 參數 / Params：
#           ms: 等待時間，單位毫秒 / Wait time in milliseconds.
#
#   keep_alive()
#       - 說明：在耗時迴圈中手動補足心跳。
#       - Desc: Manually send a heartbeat in long loops to maintain connection.
#       - 參數 / Params：無 / None.
#
#   run_task(coro)
#       - 說明：啟動背景多工心跳任務 (僅多工模式使用)。
#       - Desc: Starts background multitask heartbeat (for async mode only).
#       - 參數 / Params：
#           coro: 協程任務函數 / Coroutine task function.
#
# 【馬達控制 / Motor Control】
#   motor_power(port, power)
#       - 說明：直接以指定的 PWM 比例驅動馬達 (開迴路)。
#       - Desc: Drive the motor directly with PWM power (open-loop).
#       - 參數 / Params：
#           port: 孔位 1~6 / Port 1~6.
#           power: 動力 -100~100 (正數順時針) / Power -100~100 (positive for CW).
#
#   motor_run(port, speed)
#       - 說明：以指定的目標速度轉動馬達 (PID 閉迴路)。
#       - Desc: Run the motor at a specific target speed (closed-loop PID).
#       - 參數 / Params：
#           port: 孔位 1~6 / Port 1~6.
#           speed: 目標速度 -100~100 (正數順時針) / Target speed -100~100 (positive for CW).
#
#   motor_stop(port, stop)
#       - 說明：停止馬達。stop 為必填，決定停止方式。
#       - Desc: Stop the motor. `stop` is required and selects the stop mode.
#       - 參數 / Params：
#           port: 孔位 1~6 / Port 1~6.
#           stop: 1=STOP_COAST 滑行 / 2=STOP_BRAKE 被動煞車 / 3=STOP_HOLD 位置鎖死 /
#                 4=STOP_CONTINUE 維持現狀(不送任何指令，馬達繼續做原本在做的事)
#                 (整數 1~4、STOP_* 常數、或字串 "coast"/"brake"/"hold"/"continue" 皆可)。
#
#   stop_all(stop)
#       - 說明：緊急停止所有 1~6 孔的馬達。stop 為必填（同 motor_stop）。
#       - Desc: Emergency stop all motors on ports 1~6. `stop` required (same as motor_stop).
#       - 參數 / Params：
#           stop: 1=STOP_COAST / 2=STOP_BRAKE / 3=STOP_HOLD / 4=STOP_CONTINUE(維持現狀，不送任何指令)。
#
#   motor_run_degrees(port, speed, degrees, stop)
#       - 說明：以指定速度轉動相對角度。stop 為必填，決定到位後的動作。
#       - Desc: Run a relative angle at a set speed. `stop` required: what to do on arrival.
#       - 參數 / Params：
#           port: 孔位 1~6 / Port 1~6.
#           speed: 速度 -100~100 / Speed -100~100.
#           degrees: 相對角度 -2499~2499 (可正可負) / Relative angle -2499~2499.
#           ★ 轉向 = 速度正負 × 角度正負：符號相同→正轉(CW)，相異→反轉(CCW)。
#             (speed=+,deg=+) 與 (speed=-,deg=-) 皆 CW；(speed=-,deg=+) 與 (speed=+,deg=-) 皆 CCW。
#             轉動圈數只看 |degrees|、速度大小只看 |speed|。
#           Direction = sign(speed) × sign(degrees): same sign→CW, opposite→CCW.
#           stop: 1=STOP_COAST 滑行 / 2=STOP_BRAKE 煞車 / 3=STOP_HOLD 鎖死 / 4=STOP_CONTINUE 到位後續轉
#                 (整數 1~4、STOP_* 常數、或字串皆可)。
#
#   motor_track_target(port, speed, angle, stop)   （舊名 motor_run_target 仍可用 / old name still works）
#       - 說明：以指定速度轉動到絕對角度 (以最短路徑)。stop 為必填，決定到位後的動作。
#       - Desc: Run the motor to an absolute target angle via shortest path. `stop` required.
#       - 參數 / Params：
#           port: 孔位 1~6 / Port 1~6.
#           speed: 速度 -100~100 / Speed -100~100.
#           angle: 目標絕對角度 0~359 / Target absolute angle 0~359.
#           stop: 1=STOP_COAST 滑行 / 2=STOP_BRAKE 煞車 / 3=STOP_HOLD 鎖死 / 4=STOP_CONTINUE 到位後續轉
#                 (整數 1~4、STOP_* 常數、或字串皆可)。
#
#   set_motor_inverted(port, inverted=True)
#       - 說明：設定馬達正/反裝。反裝時該孔的控制指令方向與回傳數值都自動反向，
#               不影響雙馬達同步的底層編碼器運算。預設全部正裝。
#       - Desc: Set motor mounting direction. When inverted, this port's commands and
#               readings auto-reverse; the dual-motor sync low-level loop is unaffected.
#       - 參數 / Params：
#           port: 孔位 1~6 / Port 1~6.
#           inverted: True 反裝 / False 正裝 (預設 True) / True=inverted, False=normal.
#
#   drive(left_port, right_port, left_speed, right_speed)
#       - 說明：雙馬達同步驅動，適用於車體底盤。
#       - Desc: Synchronized dual-motor drive for chassis.
#       - 參數 / Params：
#           left_port: 左馬達孔位 1~6 / Left motor port 1~6.
#           right_port: 右馬達孔位 1~6 / Right motor port 1~6.
#           left_speed: 左馬達速度 -100~100 / Left motor speed.
#           right_speed: 右馬達速度 -100~100 / Right motor speed.
#
#   drive_stop(left_port, right_port, stop)
#       - 說明：停止雙馬達。stop 為必填，跟 motor_stop 同一套 4 種狀態。
#       - Desc: Stop dual motors. `stop` is required, same 4 states as motor_stop.
#       - 參數 / Params：
#           left_port: 左馬達孔位 1~6 / Left motor port 1~6.
#           right_port: 右馬達孔位 1~6 / Right motor port 1~6.
#           stop: 1=STOP_COAST / 2=STOP_BRAKE / 3=STOP_HOLD / 4=STOP_CONTINUE(維持現狀，不送任何指令)。
#
#   motor_set_pid(port, p, i, d)
#       - 說明：設定馬達的 PID 參數基準倍率。
#       - Desc: Set the PID ratio for the motor.
#       - 參數 / Params：
#           port: 孔位 1~6 / Port 1~6.
#           p, i, d: PID 倍率，預設 50，0為關閉，100為2倍 / PID ratio (0~100, 50=default).
#
# 【感測器與狀態 / Sensors & States】
#   reset_angle(port)
#       - 說明：將該孔馬達的相對累積角度歸零。
#       - Desc: Reset the relative cumulative angle of the motor to zero.
#       - 參數 / Params：
#           port: 孔位 1~6 / Port 1~6.
#
#   get_motor_angle(port)
#       - 說明：讀取該孔馬達的相對累積角度。
#       - Desc: Get the relative cumulative angle of the motor.
#       - 參數 / Params：
#           port: 孔位 1~6 / Port 1~6.
#
#   get_motor_abs_angle(port)
#       - 說明：讀取馬達絕對角度 (0~359)。
#       - Desc: Get the absolute angle of the motor (0~359).
#       - 參數 / Params：
#           port: 孔位 1~6 / Port 1~6.
#
#   get_motor_speed(port)
#       - 說明：讀取馬達目前轉速。
#       - Desc: Get the current speed of the motor.
#       - 參數 / Params：
#           port: 孔位 1~6 / Port 1~6.
#
#   get_ultrasonic_distance(port)
#       - 說明：讀取超音波距離 (毫米)。
#       - Desc: Get ultrasonic distance in mm.
#       - 參數 / Params：
#           port: 孔位 1~6 / Port 1~6.
#
#   get_touch_force(port)
#       - 說明：讀取觸碰感應器按壓力量 (0~100)。
#       - Desc: Get touch sensor force (0~100).
#       - 參數 / Params：
#           port: 孔位 1~6 / Port 1~6.
#
#   get_color_color(port)
#       - 說明：讀取顏色代碼 (-1 為無顏色，0~10 對應標準顏色)。
#       - Desc: Get color code (-1 for none, 0~10 for standard colors).
#       - 參數 / Params：
#           port: 孔位 1~6 / Port 1~6.
#
#   get_color_reflection(port)
#       - 說明：讀取顏色感測器光線反射值 (0~100)。
#       - Desc: Get color sensor reflection value (0~100).
#       - 參數 / Params：
#           port: 孔位 1~6 / Port 1~6.
#
#   get_color_rgb(port) / get_color_hsv(port)
#       - 說明：讀取顏色感測器 RGB / HSV 元組。
#       - Desc: Get RGB / HSV tuple from color sensor.
#       - 參數 / Params：
#           port: 孔位 1~6 / Port 1~6.
#
#   get_color_red(port) / get_color_green(port) / get_color_blue(port)
#       - 說明：單獨讀取顏色感測器 R / G / B 通道 (0~100)。
#       - Desc: Get individual R / G / B channels from color sensor (0~100).
#       - 參數 / Params：
#           port: 孔位 1~6 / Port 1~6.
#
#   get_color_hue(port) / get_color_sat(port) / get_color_val(port)
#       - 說明：單獨讀取顏色感測器 H (0~359) / S (0~100) / V (0~100) 通道。
#       - Desc: Get individual H / S / V channels from color sensor.
#       - 參數 / Params：
#           port: 孔位 1~6 / Port 1~6.
#
#   get_device_id(port)
#       - 說明：取得該孔連接裝置的 ID (0=無, 1=觸碰, 3=顏色, 4=超音波, 5=馬達)。
#       - Desc: Get the device ID connected to the port.
#       - 參數 / Params：
#           port: 孔位 1~6 / Port 1~6.
#
#   get_voltage()
#       - 說明：讀取擴充板電池電壓 (V)。
#       - Desc: Get the expansion board battery voltage in Volts.
#       - 參數 / Params：無 / None.
#
#   get_port_raw(port)
#       - 說明：讀取該孔未解析的原始數值。⚠ 顏色感測器的封包把 32-bit 全部位元當位元欄位塞滿，
#               若剛好落在 MicroPython 小整數(small int)代表範圍外，會退回 24-bit 無號值(捨棄
#               最高位元組)以避免當機；請改用 get_color_*() 系列取得完整正確的顏色資料。
#               馬達/超音波/力量感測器的真實數值遠低於此門檻，不受影響。
#       - Desc: Get the raw, unparsed value of the port. ⚠ Color sensor packets use all 32 bits
#               as bit-packed fields; if the reconstructed value falls outside MicroPython's
#               small-int range, this falls back to a 24-bit unsigned value (top byte dropped)
#               to avoid a crash — use get_color_*() for correct, full color data instead.
#               Motor/ultrasonic/force values are always well within the safe range.
#       - 參數 / Params：
#           port: 孔位 1~6 / Port 1~6.
# ════════════════════════════════════════════════════════════════

# ════════════════════════════════════════════════════════════════
#  MBC_EXP6 快速查閱列表 / Quick Reference List
# ════════════════════════════════════════════════════════════════
# keep_alive_wait(ms)
# keep_alive()
# run_task(coro)
# motor_power(port, power)
# motor_run(port, speed)
# motor_stop(port, stop)                          stop: 1滑行 2煞車 3鎖死 4維持現狀(不送指令)
# stop_all(stop)                                  stop: 1滑行 2煞車 3鎖死 4維持現狀(不送指令)
# motor_run_degrees(port, speed, degrees, stop)   degrees -2499~2499; stop: 1滑行 2煞車 3鎖死 4續轉
#   （停止碼字串常數：STOP_COAST=1 STOP_BRAKE=2 STOP_HOLD=3 STOP_CONTINUE=4；續轉只有走角度指令才會真的轉動，
#     純停止指令(motor_stop/drive_stop/stop_all)收到 4 是「不送任何指令、維持現狀」的意思）
# motor_track_target(port, speed, angle, stop)    stop: 1滑行 2煞車 3鎖死 4續轉（舊名 motor_run_target 仍可用）
# set_motor_inverted(port, inverted=True)         設定正/反裝
# drive(left_port, right_port, left_speed, right_speed)
# drive_stop(left_port, right_port, stop)         stop: 同 motor_stop 的 4 種狀態
# motor_set_pid(port, p, i, d)
# reset_angle(port)
# get_motor_angle(port)
# get_motor_abs_angle(port)
# get_motor_speed(port)
# get_ultrasonic_distance(port)
# get_touch_force(port)
# get_color_color(port)
# get_color_reflection(port)
# get_color_rgb(port)
# get_color_hsv(port)
# get_color_red(port)
# get_color_green(port)
# get_color_blue(port)
# get_color_hue(port)
# get_color_sat(port)
# get_color_val(port)
# get_device_id(port)
# get_voltage()
# get_port_raw(port)
# ════════════════════════════════════════════════════════════════
