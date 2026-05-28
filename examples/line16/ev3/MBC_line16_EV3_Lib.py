# Python Library for MBC Line Tracer 16CH for Pybricks EV3
# Author : MBC robotics
# Date   : 2026.05
#
# !! Import this file MBC_line16_EV3_Lib.py into your Pybricks EV3 project !!
#

from pybricks.tools import StopWatch as _StopWatch

# ════════════════════════════════════════════════════════════════
#  Internal state
# ════════════════════════════════════════════════════════════════

_selected_port = None
_async_mode    = False

# ════════════════════════════════════════════════════════════════
#  Cache (Aligned with the 10ms sensor update cycle)
# ════════════════════════════════════════════════════════════════

_cache_mode0 = None
_cache_mode1 = None
_cache_mode2 = None

_time_mode0 = 0
_time_mode1 = 0
_time_mode2 = 0

_CACHE_MS = 10
_watch = _StopWatch()

async def _noop():
    pass

def _check_init():
    if _selected_port is None:
        raise RuntimeError("Sensor port not initialized. Please call line_init_port_multitask() first.")

# ════════════════════════════════════════════════════════════════
#  Hardware Read Wrappers (Sync/Async & Cache)
# ════════════════════════════════════════════════════════════════

def _read_mode(mode_num):
    global _cache_mode0, _cache_mode1, _cache_mode2
    global _time_mode0, _time_mode1, _time_mode2
    now = _watch.time()

    if mode_num == 0:
        if _cache_mode0 is None or (now - _time_mode0) >= _CACHE_MS:
            _cache_mode0 = _selected_port.device.mode(0)
            _time_mode0 = now
        return _cache_mode0
    elif mode_num == 1:
        if _cache_mode1 is None or (now - _time_mode1) >= _CACHE_MS:
            _cache_mode1 = _selected_port.device.mode(1)
            _time_mode1 = now
        return _cache_mode1
    elif mode_num == 2:
        if _cache_mode2 is None or (now - _time_mode2) >= _CACHE_MS:
            _cache_mode2 = _selected_port.device.mode(2)
            _time_mode2 = now
        return _cache_mode2

async def _async_read_mode(mode_num):
    global _cache_mode0, _cache_mode1, _cache_mode2
    global _time_mode0, _time_mode1, _time_mode2
    now = _watch.time()

    if mode_num == 0:
        if _cache_mode0 is None or (now - _time_mode0) >= _CACHE_MS:
            _cache_mode0 = _selected_port.device.mode(0)
            _time_mode0 = now
        return _cache_mode0
    elif mode_num == 1:
        if _cache_mode1 is None or (now - _time_mode1) >= _CACHE_MS:
            _cache_mode1 = _selected_port.device.mode(1)
            _time_mode1 = now
        return _cache_mode1
    elif mode_num == 2:
        if _cache_mode2 is None or (now - _time_mode2) >= _CACHE_MS:
            _cache_mode2 = _selected_port.device.mode(2)
            _time_mode2 = now
        return _cache_mode2

def _make_sensor_func(mode_num, parse_fn):
    async def _async():
        data = await _async_read_mode(mode_num)
        return parse_fn(data)

    def _func():
        _check_init()
        if _async_mode:
            return _async()
        data = _read_mode(mode_num)
        return parse_fn(data)

    return _func

# ════════════════════════════════════════════════════════════════
#  Parsers
# ════════════════════════════════════════════════════════════════

def _parse_pos16(data):
    return data[0]

def _parse_pos100(data):
    return data[0]

def _parse_width(data):
    return data[1]

def _make_ir_ch_parser(ch):
    idx = ch - 1
    byte_idx = idx // 2
    is_high = (idx % 2 == 0)
    def _parser(data):
        val = data[byte_idx]
        if is_high:
            return val // 10
        else:
            return val % 10
    return _parser

# ════════════════════════════════════════════════════════════════
#  Public Functions
# ════════════════════════════════════════════════════════════════

line_pos16  = _make_sensor_func(0, _parse_pos16)
line_pos100 = _make_sensor_func(2, _parse_pos100)
line_width  = _make_sensor_func(2, _parse_width)

_IR_CH_PARSERS = [_make_ir_ch_parser(ch) for ch in range(1, 17)]
_IR_CH_FUNCS = [_make_sensor_func(1, parser) for parser in _IR_CH_PARSERS]

def line_ir_ch(ch):
    """
    Returns IR calibration value of a single channel (1~9).
    ch    : int 1~16
    """
    if ch < 1 or ch > 16:
        raise ValueError("ch must be 1~16")
    return _IR_CH_FUNCS[ch - 1]()

def line_init_port_multitask(port, multitask=False):
    """
    Initialize sensor port.
      port      : Pybricks EV3 port object (e.g. hub.port.S1)
      multitask : bool, True for use inside async/multitask programs
    Returns an awaitable no-op so it can safely be used with
    'await line_init_port_multitask(...)'.
    """
    global _selected_port, _async_mode
    global _cache_mode0, _cache_mode1, _cache_mode2
    global _time_mode0, _time_mode1, _time_mode2
    
    _selected_port = port
    _async_mode    = bool(multitask)
    _cache_mode0 = _cache_mode1 = _cache_mode2 = None
    _time_mode0 = _time_mode1 = _time_mode2 = 0
    return _noop()
