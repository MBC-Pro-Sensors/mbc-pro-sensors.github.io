# Python Library for MBC Line Tracer 8CH for Pybricks EV3
# Author : MBC robotics
# Date   : 2026.05
#
# !! Import this file MBC_line8_EV3_Lib.py into your Pybricks EV3 project !!
#
# Port mapping (port= argument):
#   1 -> Port.S1,  2 -> Port.S2,  3 -> Port.S3,  4 -> Port.S4

from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port
from pybricks.tools import StopWatch as _StopWatch

# ════════════════════════════════════════════════════════════════
#  Port number -> Port object mapping (EV3: 1~4 = S1~S4)
# ════════════════════════════════════════════════════════════════

_PORT_MAP = {
    1: Port.S1,
    2: Port.S2,
    3: Port.S3,
    4: Port.S4,
}

_PORT_LETTER = {1: 'S1', 2: 'S2', 3: 'S3', 4: 'S4'}

# ════════════════════════════════════════════════════════════════
#  Mode layout
#
#  mode 0: data[0] = pos8,    range -8~+8  (negative=right, positive=left)
#  mode 1: data[0~7] = 8 raw IR calibration values, one per channel
#          data[0]=ch1 (rightmost), data[7]=ch8 (leftmost), range 0~100 each
#  mode 2: data[0] = pos100,  range -100~+100
#          data[1] = width,   range 0~8
#          data[2] = ch1,     range 0~100  (rightmost boundary)
#          data[3] = ch8,     range 0~100  (leftmost  boundary)
# ════════════════════════════════════════════════════════════════

# ════════════════════════════════════════════════════════════════
#  Pre-build PUPDevice for all ports at module import time.
#  ★ Must be done here. PUPDevice() is forbidden after run_task().
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

# ════════════════════════════════════════════════════════════════
#  Cache (aligned with sensor update cycle of 10ms)
#  Each mode has its own cache since they return different data.
# ════════════════════════════════════════════════════════════════

_cache_mode0 = None
_cache_mode1 = None
_cache_mode2 = None

_time_mode0  = 0
_time_mode1  = 0
_time_mode2  = 0

_CACHE_MS    = 10
_watch       = _StopWatch()

async def _noop():
    pass

def _check_init():
    if _selected_dev is None:
        if _selected_port is None:
            raise RuntimeError(
                "Sensor not found. Please connect the sensor to the correct port."
            )
        label = _PORT_LETTER.get(_selected_port, str(_selected_port))
        raise RuntimeError(
            "Sensor not found on Port {} (port={}). Please check the connection.".format(
                label, _selected_port
            )
        )

# ════════════════════════════════════════════════════════════════
#  Hardware read with cache (sync and async per mode)
# ════════════════════════════════════════════════════════════════

def _read_mode0():
    global _cache_mode0, _time_mode0
    now = _watch.time()
    if _cache_mode0 is None or (now - _time_mode0) >= _CACHE_MS:
        _cache_mode0 = _selected_dev.read(0)
        _time_mode0  = now
    return _cache_mode0

def _read_mode1():
    global _cache_mode1, _time_mode1
    now = _watch.time()
    if _cache_mode1 is None or (now - _time_mode1) >= _CACHE_MS:
        _cache_mode1 = _selected_dev.read(1)
        _time_mode1  = now
    return _cache_mode1

def _read_mode2():
    global _cache_mode2, _time_mode2
    now = _watch.time()
    if _cache_mode2 is None or (now - _time_mode2) >= _CACHE_MS:
        _cache_mode2 = _selected_dev.read(2)
        _time_mode2  = now
    return _cache_mode2

async def _async_read_mode0():
    global _cache_mode0, _time_mode0
    now = _watch.time()
    if _cache_mode0 is None or (now - _time_mode0) >= _CACHE_MS:
        _cache_mode0 = await _selected_dev.read(0)
        _time_mode0  = now
    return _cache_mode0

async def _async_read_mode1():
    global _cache_mode1, _time_mode1
    now = _watch.time()
    if _cache_mode1 is None or (now - _time_mode1) >= _CACHE_MS:
        _cache_mode1 = await _selected_dev.read(1)
        _time_mode1  = now
    return _cache_mode1

async def _async_read_mode2():
    global _cache_mode2, _time_mode2
    now = _watch.time()
    if _cache_mode2 is None or (now - _time_mode2) >= _CACHE_MS:
        _cache_mode2 = await _selected_dev.read(2)
        _time_mode2  = now
    return _cache_mode2

# ════════════════════════════════════════════════════════════════
#  Factory function: takes read functions and a parser,
#  produces a dual-mode (sync/async) public function with cache.
# ════════════════════════════════════════════════════════════════

def _make_sensor_func(read_sync_fn, read_async_fn, parse_fn):
    async def _async():
        return parse_fn(await read_async_fn())

    def _func():
        _check_init()
        if _async_mode:
            return _async()
        return parse_fn(read_sync_fn())

    return _func

# ════════════════════════════════════════════════════════════════
#  Parse helpers (pure calculation, shared by sync and async)
# ════════════════════════════════════════════════════════════════

def _parse_pos8(data):
    """mode 0: line position, range -8~+8."""
    return data[0]

def _parse_pos100(data):
    """mode 2: high-resolution line position, range -100~+100."""
    return data[0]

def _parse_width(data):
    """mode 2: line width in channel count, range 0~8."""
    return data[1]

def _parse_edge_ch1(data):
    """mode 2: leftmost channel (ch1) IR value, range 0~100."""
    return data[2]

def _parse_edge_ch8(data):
    """mode 2: rightmost channel (ch8) IR value, range 0~100."""
    return data[3]

def _make_ir_ch_parser(ch):
    """mode 1: single channel raw IR value, range 0~100."""
    idx = ch - 1
    def _parser(data):
        return data[idx]
    return _parser

# ════════════════════════════════════════════════════════════════
#  Public sensor functions
# ════════════════════════════════════════════════════════════════

line_pos8  = _make_sensor_func(_read_mode0, _async_read_mode0, _parse_pos8)
line_pos100 = _make_sensor_func(_read_mode2, _async_read_mode2, _parse_pos100)
line_width  = _make_sensor_func(_read_mode2, _async_read_mode2, _parse_width)
line_edge_l = _make_sensor_func(_read_mode2, _async_read_mode2, _parse_edge_ch1)
line_edge_r = _make_sensor_func(_read_mode2, _async_read_mode2, _parse_edge_ch8)

# ★ line_ir_ch: 8 parsers and functions pre-built at import time.
#   No new objects are created on each call.
_IR_CH_PARSERS = [_make_ir_ch_parser(ch) for ch in range(1, 9)]
_IR_CH_FUNCS   = [
    _make_sensor_func(_read_mode1, _async_read_mode1, parser)
    for parser in _IR_CH_PARSERS
]

def line_ir_ch(ch):
    """
    Returns raw IR calibration value of a single channel (0~100).
    ch    : int 1~8  (1=rightmost, 8=leftmost)
    sync  : line_ir_ch(4)
    async : await line_ir_ch(4)
    ★ Shares cache. Hardware is read at most once per 10ms window.
    """
    if ch < 1 or ch > 8:
        raise ValueError("ch must be 1~8, got {}".format(ch))
    return _IR_CH_FUNCS[ch - 1]()

# ════════════════════════════════════════════════════════════════
#  Initialization function
# ════════════════════════════════════════════════════════════════

def line_init_port_multitask(port, multitask=False):
    """
    Initialize sensor and select mode.
      port      : int 1~4  (1=S1, 2=S2, 3=S3, 4=S4)
      multitask : bool, True for use inside async/multitask programs
    Returns an awaitable no-op so it can safely be used with
    'await line_init_port_multitask(...)'.
    """
    global _selected_dev, _async_mode, _selected_port
    global _cache_mode0, _cache_mode1, _cache_mode2
    global _time_mode0,  _time_mode1,  _time_mode2

    _selected_port = port
    _selected_dev  = _devices.get(port)
    _async_mode    = bool(multitask)

    # Reset cache on port change
    _cache_mode0 = _cache_mode1 = _cache_mode2 = None
    _time_mode0  = _time_mode1  = _time_mode2  = 0

    return _noop()