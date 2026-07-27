# Python Object Library for MBC Line Tracer 8CH for Pybricks SPIKE Prime
# Author : MBC robotics
# Date   : 2026.07
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
#  PUPDevice is built lazily, only for the port actually passed to
#  line_init_port_multitask() -- never touches the other 5 ports.
#  ★ For multitask=True, call line_init_port_multitask() at the top level,
#    BEFORE run_task(main()) -- not from inside async def main(). Building
#    a device object after run_task() has started is not safe (same rule
#    as MBC_LINE8/MBC_EXP6). For multitask=False (sync) this restriction
#    does not apply -- call it anywhere.
# ════════════════════════════════════════════════════════════════

_devices = {}

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

    ★ For multitask=True, call this at the top level BEFORE run_task(main()),
    not from inside async def main() -- building the port's device object
    must happen before the event loop starts (same rule as MBC_LINE8(...)).
    Only the requested port is touched; the other 5 ports are left alone.
    Returns an awaitable no-op so it can safely be used with
    'await line_init_port_multitask(...)'.
    """
    global _selected_dev, _async_mode, _selected_port
    global _cache, _cache_time
    _selected_port = port
    _async_mode    = bool(multitask)
    _cache         = None   # ★ Cache resets upon port switch
    _cache_time    = 0
    if port not in _devices:
        try:
            _devices[port] = PUPDevice(_PORT_MAP[port])
        except Exception:
            _devices[port] = None
    _selected_dev = _devices.get(port)
    return _noop()

# ════════════════════════════════════════════════════════════════
#  Object-style API (MBC_LINE8 class)
#
#  Same reason MBC_EXP6 is a class: graphical block tools place object
#  construction ("object block") in the Setup canvas, before run_task(),
#  while the free functions above are called from an "imported function"
#  block that only lands inside async def main() (after run_task() has
#  already started). Building a PUPDevice there is too late/unsafe.
#
#  MBC_LINE8(port, multitask) builds a PUPDevice for ONLY that one port,
#  in __init__ (before run_task()) -- it never touches the other 5 ports,
#  so it can't collide with another device (e.g. an EXP6 board) that is
#  already streaming on one of them.
#
#  Kept fully independent of the module-level globals above so mixing the
#  old function-style calls with this class in the same program can't
#  cause cross-talk.
# ════════════════════════════════════════════════════════════════

class MBC_LINE8:
    def __init__(self, port, multitask=False):
        self._selected_port = port
        self._async_mode    = bool(multitask)
        self._cache         = None
        self._cache_time    = 0
        try:
            self._selected_dev = PUPDevice(_PORT_MAP[port])
        except Exception:
            self._selected_dev = None

    def _check_init(self):
        if self._selected_dev is None:
            letter = _PORT_LETTER.get(self._selected_port, str(self._selected_port))
            raise RuntimeError(
                "Sensor not found on Port {} (port={}). Please check the connection.".format(
                    letter, self._selected_port
                )
            )

    async def _read_async(self):
        now = _watch.time()
        if self._cache is None or (now - self._cache_time) >= _CACHE_MS:
            self._cache      = await self._selected_dev.read(_MODE)
            self._cache_time = now
        return self._cache

    def _read_sync(self):
        now = _watch.time()
        if self._cache is None or (now - self._cache_time) >= _CACHE_MS:
            self._cache      = self._selected_dev.read(_MODE)
            self._cache_time = now
        return self._cache

    def _get(self, parse_fn):
        self._check_init()
        if self._async_mode:
            return self._get_async(parse_fn)
        return parse_fn(self._read_sync())

    async def _get_async(self, parse_fn):
        return parse_fn(await self._read_async())

    def ir_calibration(self):
        return self._get(_parse_ir_calibration)

    def pos8(self):
        return self._get(_parse_line_pos8)

    def width(self):
        return self._get(_parse_line_width)

    def pos100(self):
        return self._get(_parse_line_pos100)

    def bin_raw(self):
        return self._get(_parse_bin_raw)

    def on_sensors(self):
        return self._get(_parse_on_sensors)

    def junction(self):
        return self._get(_parse_junction)

    def junction_name(self):
        return self._get(_parse_junction_name)

    def read_all(self):
        return self._get(_parse_read_all)

    def ir_ch(self, ch):
        """
        Returns IR calibration value of a single channel (0~100).
        ch : int 1~8  (1=leftmost, 8=rightmost)
        """
        if ch < 1 or ch > 8:
            raise ValueError("ch must be 1~8, got {}".format(ch))
        return self._get(_IR_CH_PARSERS[ch - 1])

# ════════════════════════════════════════════════════════════════
#  MBC_LINE8 -- callable methods (object-style API)
#
#  line8 = MBC_LINE8(port, multitask)   -- call BEFORE run_task(main())
#    port      : int 1~6  (1=A, 2=B, 3=C, 4=D, 5=E, 6=F)
#    multitask : False -> sync (call methods directly)
#                True  -> async (await every method call)
#
#  line8.pos8()             -8~+8    line position, coarse (center=0)
#  line8.pos100()           -100~+100  line position, high-resolution (center=0)
#  line8.width()            0~8      number of triggered sensors
#  line8.bin_raw()          0~255    raw 8-bit sensor bitmap
#  line8.on_sensors()       [int]    1-indexed channels currently on the line
#  line8.junction()         0~2+     0=no line, 1=single line, 2+=junction
#  line8.junction_name()    str      "none" / "line" / "junction"
#  line8.ir_calibration()   [int]*8  all 8 channels' calibrated IR value (0~100)
#  line8.ir_ch(ch)          0~100    single channel's calibrated IR value, ch=1~8
#  line8.read_all()         dict     all of the above in one dict, one hardware read
#
#  Example (sync):
#      line8 = MBC_LINE8(3, False)
#      print(line8.pos8(), line8.width())
#
#  Example (async / multitask):
#      line8 = MBC_LINE8(3, True)
#      async def main():
#          print(await line8.pos8(), await line8.width())
#      run_task(main())
# ════════════════════════════════════════════════════════════════