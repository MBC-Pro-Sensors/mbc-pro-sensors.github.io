# Python Library for MBC Line Tracer 8CH for Pybricks SPIKE Prime
# Author : MBC robotics
# Date   : 2026.05
#
# !! Import this file MBC_line8_Lib.py into your Pybricks editor !!
#
# Port mapping (port= argument):
#   1 → Port.A,  2 → Port.B,  3 → Port.C,
#   4 → Port.D,  5 → Port.E,  6 → Port.F

from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port
from pybricks.tools import StopWatch as _StopWatch

# ════════════════════════════════════════════════════════════════
#  Port number → Port object mapping (SPIKE Prime: 1~6 = A~F)
# ════════════════════════════════════════════════════════════════

_PORT_MAP = {
    1: Port.A,
    2: Port.B,
    3: Port.C,
    4: Port.D,
    5: Port.E,
    6: Port.F,
}

_PORT_LETTER = {1: 'A', 2: 'B', 3: 'C', 4: 'D', 5: 'E', 6: 'F'}

# ════════════════════════════════════════════════════════════════
#  Packet layout (mode 9, 16 bytes raw → 8 int16 from Pybricks)
#
#  word[0] : buf[0]=IR_calib(ch1),  buf[1]=IR_calib(ch2)    → 0~100 each
#  word[1] : buf[2]=IR_calib(ch3),  buf[3]=IR_calib(ch4)
#  word[2] : buf[4]=IR_calib(ch5),  buf[5]=IR_calib(ch6)
#  word[3] : buf[6]=IR_calib(ch7),  buf[7]=IR_calib(ch8)
#  word[4] : buf[8]=linePos8+8,     buf[9]=lineWidth
#  word[5] : buf[10]=linePos100+100, buf[11]=binGroupCount
#  word[6] : buf[12]=binRaw(8-bit), buf[13]=0(unused)
#  word[7] : buf[14]=0(unused),     buf[15]=0(unused)
#
#  Note: Unlike the 16ch version, IR calibration values are raw 0~100
#  (1 byte per channel, NOT 4-bit packed). Values never exceed 127,
#  so Pybricks int16 sign conversion is not needed for ch data.
# ════════════════════════════════════════════════════════════════

_MODE = 9

def _u16(word):
    """Convert signed int16 (from Pybricks) to unsigned uint16."""
    return word & 0xFFFF if word < 0 else word

def _lo(word):
    return _u16(word) & 0xFF

def _hi(word):
    return (_u16(word) >> 8) & 0xFF

# ════════════════════════════════════════════════════════════════
#  Pre-build PUPDevice for all ports at the module level
#  ★ Must be created here; calling PUPDevice() after run_task() is prohibited
# ════════════════════════════════════════════════════════════════

_devices = {}
for _port_num, _port_obj in _PORT_MAP.items():
    try:
        _devices[_port_num] = PUPDevice(_port_obj)
    except Exception:
        _devices[_port_num] = None

# ════════════════════════════════════════════════════════════════
#  Internal state
# ════════════════════════════════════════════════════════════════

_selected_dev  = None
_async_mode    = False
_selected_port = None

_JUNCTION_NAMES = {0: "none", 1: "line", 2: "junction"}

# ════════════════════════════════════════════════════════════════
#  Cache (Aligned with the 10ms sensor update cycle)
# ════════════════════════════════════════════════════════════════

_cache      = None
_cache_time = 0
_CACHE_MS   = 10
_watch      = _StopWatch()

async def _noop():
    pass

def _check_init():
    if _selected_dev is None:
        if _selected_port is None:
            raise RuntimeError(
                "Sensor not found. Please connect the sensor to the correct port."
            )
        letter = _PORT_LETTER.get(_selected_port, str(_selected_port))
        raise RuntimeError(
            "Sensor not found on Port {} (port={}). Please check the connection.".format(
                letter, _selected_port
            )
        )

# ════════════════════════════════════════════════════════════════
#  Parse helpers (Pure computation, shared by sync / async)
# ════════════════════════════════════════════════════════════════

def _parse_ir_calibration(p):
    """
    Unpack 8 IR calibration values (0~100 each) from words 0~3.
    Returns list of 8 integers.
    Index 0 = channel 1 (leftmost), index 7 = channel 8 (rightmost).

    Packet layout: each word holds 2 channels, lo=lower ch, hi=upper ch.
      word[0]: ch1=lo, ch2=hi
      word[1]: ch3=lo, ch4=hi
      word[2]: ch5=lo, ch6=hi
      word[3]: ch7=lo, ch8=hi
    """
    values = []
    for word_idx in range(4):
        values.append(_lo(p[word_idx]))  # odd channel  (ch1, ch3, ch5, ch7)
        values.append(_hi(p[word_idx]))  # even channel (ch2, ch4, ch6, ch8)
    return values

def _parse_line_pos8(p):
    """linePos8: buf[8]-8, range -8~+8 (center=0). Negative=right, Positive=left."""
    return _lo(p[4]) - 8

def _parse_line_width(p):
    """lineWidth: buf[9], number of triggered sensors (0~8)."""
    return _hi(p[4])

def _parse_line_pos100(p):
    """linePos100: buf[10]-100, range -100~+100 (center=0). High-resolution interpolated position."""
    return _lo(p[5]) - 100

def _parse_junction(p):
    """binGroupCount: buf[11]. 0=no line, 1=single line, 2+=junction/fork."""
    return _hi(p[5])

def _parse_bin_raw(p):
    """binRaw 8-bit: buf[12]. Bit 0=rightmost sensor (ch1), bit 7=leftmost sensor (ch8)."""
    return _lo(p[6])

def _parse_on_sensors(p):
    """List of 1-indexed channel numbers where line is detected (bit0=ch1 ... bit7=ch8)."""
    raw = _parse_bin_raw(p)
    return [i + 1 for i in range(8) if raw & (1 << i)]

def _parse_junction_name(p):
    j = _parse_junction(p)
    return _JUNCTION_NAMES.get(min(j, 2), "junction")

def _parse_read_all(p):
    return {
        "ir_calibration": _parse_ir_calibration(p),
        "line_pos8":      _parse_line_pos8(p),
        "line_width":     _parse_line_width(p),
        "line_pos100":    _parse_line_pos100(p),
        "bin_raw":        _parse_bin_raw(p),
        "on_sensors":     _parse_on_sensors(p),
        "junction":       _parse_junction(p),
        "junction_name":  _parse_junction_name(p),
    }

# ★ Specific to line_ir_ch: 8 parsers are pre-created when the module is loaded, not at call time
def _make_ir_ch_parser(ch):
    def _parser(p):
        return _parse_ir_calibration(p)[ch - 1]
    return _parser

_IR_CH_PARSERS = [_make_ir_ch_parser(ch) for ch in range(1, 9)]  # index 0 = ch1

# ════════════════════════════════════════════════════════════════
#  Factory function: Pass in a parser to automatically generate a dual-mode + cache public function
#  sync mode  → returns the result directly
#  async mode → returns a coroutine (requires await)
#  cache      → reads hardware only once for multiple calls within 10ms
# ════════════════════════════════════════════════════════════════

def _make_sensor_func(parse_fn):
    async def _async():
        global _cache, _cache_time
        now = _watch.time()
        if _cache is None or (now - _cache_time) >= _CACHE_MS:
            _cache      = await _selected_dev.read(_MODE)
            _cache_time = now
        return parse_fn(_cache)

    def _func():
        global _cache, _cache_time
        _check_init()
        if _async_mode:
            return _async()
        now = _watch.time()
        if _cache is None or (now - _cache_time) >= _CACHE_MS:
            _cache      = _selected_dev.read(_MODE)
            _cache_time = now
        return parse_fn(_cache)

    return _func

# ════════════════════════════════════════════════════════════════
#  Public sensor functions
# ════════════════════════════════════════════════════════════════

line_ir_calibration = _make_sensor_func(_parse_ir_calibration)
line_pos8           = _make_sensor_func(_parse_line_pos8)
line_width          = _make_sensor_func(_parse_line_width)
line_pos100         = _make_sensor_func(_parse_line_pos100)
line_bin_raw        = _make_sensor_func(_parse_bin_raw)
line_on_sensors     = _make_sensor_func(_parse_on_sensors)
line_junction       = _make_sensor_func(_parse_junction)
line_junction_name  = _make_sensor_func(_parse_junction_name)
line_read_all       = _make_sensor_func(_parse_read_all)

# ★ Specific to line_ir_ch: 8 functions are pre-created when the module is loaded
_IR_CH_FUNCS = [_make_sensor_func(parser) for parser in _IR_CH_PARSERS]  # index 0 = ch1

def line_ir_ch(ch):
    """
    Returns IR calibration value of a single channel (0~100).
    ch    : int 1~8  (1=leftmost, 8=rightmost)
    sync  : line_ir_ch(4)
    async : await line_ir_ch(4)
    ★ Shares cache; reads hardware only once for multiple calls within 10ms
    ★ Function objects are pre-created when the module is loaded; no new objects are created at call time
    """
    if ch < 1 or ch > 8:
        raise ValueError("ch must be 1~8, got {}".format(ch))
    return _IR_CH_FUNCS[ch - 1]()

# ════════════════════════════════════════════════════════════════
#  Initialization functions
# ════════════════════════════════════════════════════════════════

def line_init_port_multitask(port, multitask=False):
    """
    Initialize sensor and select mode.
      port      : int 1~6  (1=A, 2=B, 3=C, 4=D, 5=E, 6=F)
      multitask : bool, True for use inside async/multitask programs
    Returns an awaitable no-op so it can safely be used with
    'await line_init_port_multitask(...)'.
    """
    global _selected_dev, _async_mode, _selected_port
    global _cache, _cache_time
    _selected_port = port
    _selected_dev  = _devices.get(port)
    _async_mode    = bool(multitask)
    _cache         = None   # ★ Cache resets upon port switch
    _cache_time    = 0
    return _noop()