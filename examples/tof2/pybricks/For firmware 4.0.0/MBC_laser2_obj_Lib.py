# Python Object Library for MBC Laser 2CH ToF Sensor for Pybricks SPIKE Prime
# Author : MBC robotics
# Date   : 2026.09
#
# !! Import this file MBC_laser2_obj_Lib.py into your Pybricks editor !!
#
# Port mapping (port= argument):
#   1 → Port.A,  2 → Port.B,  3 → Port.C,
#   4 → Port.D,  5 → Port.E,  6 → Port.F

# ════════════════════════════════════════════════════════════════
#  【API 函式說明庫】 (可用於主程式的參考)
#
#  距離單位一律為「公分」0~120；120 代表無目標或超過 120cm。
#  CH1 = 左雷射，CH2 = 右雷射。
#
#  1. 初始化：
#     laser2_init_port_multitask(port, multitask)
#       - 說明：初始化感測器並指定連接埠。
#       - 參數：port 填寫 1~6，分別對應 SPIKE 主機的 A~F 埠。
#       - 參數：multitask 填寫 True/False，分別對應 多工任務與非多工任務。
#       - ★ multitask=True 時，必須在 run_task(main()) 之前（最外層）呼叫。
#
#  2. 距離讀取：
#     laser2_left()          - 左雷射距離，0~120（公分）。
#     laser2_right()         - 右雷射距離，0~120（公分）。
#     laser2_ch(ch)          - 指定通道距離，ch 填 1(左) 或 2(右)。
#     laser2_average()       - 左右平均距離，0~120（公分）。
#     laser2_min_distance()  - 左右兩者中較近的距離，0~120（公分）。
#
#  3. 方向判斷：
#     laser2_min_side()
#       - 說明：較近的目標在哪一側。
#       - 回傳：-1 = 左邊較近，+1 = 右邊較近，0 = 一樣近或兩邊都沒有目標。
#
#     laser2_diff()
#       - 說明：左距離減右距離（可直接用於沿牆 / 對正的比例控制）。
#       - 回傳：整數 -120~+120；正值代表右邊比較近。
#
#  4. 狀態檢查：
#     laser2_is_ok(ch)
#       - 說明：檢查指定通道是否「開機初始化成功，且目前沒有故障」。
#       - 回傳：True (正常) / False (初始化失敗、I2C 錯誤或逾時無回應)。
#
#     laser2_read_all()
#       - 說明：一次取得所有解析完畢的綜合狀態字典 (dict)，只讀一次硬體。
#
#  5. 物件式寫法（圖形化工具的 Setup 區使用）：
#     laser2 = MBC_LASER2(port, multitask)，方法名稱去掉 laser2_ 前綴，
#     例如 laser2.left()、laser2.diff()，詳見檔案最下方說明。
# ════════════════════════════════════════════════════════════════

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
#  Packet layout (mode 9, 7 x DATA16 = 14 bytes → 7 int16 from Pybricks)
#
#  word[0] : buf[0]=dist(left, ch1),  buf[1]=dist(right, ch2)  → 0~120 cm each
#  word[1] : buf[2]=initOk bitmap,    buf[3]=fault bitmap      (bit0=left, bit1=right)
#  word[2~6] : 0 (unused)
#
#  Average / min / side / diff are computed here, not in the firmware.
# ════════════════════════════════════════════════════════════════

_MODE      = 9
_NO_TARGET = 120

def _u16(word):
    """Convert signed int16 (from Pybricks) to unsigned uint16."""
    return word & 0xFFFF if word < 0 else word

def _lo(word):
    return _u16(word) & 0xFF

def _hi(word):
    return (_u16(word) >> 8) & 0xFF

# ════════════════════════════════════════════════════════════════
#  PUPDevice is built lazily, only for the port actually passed to
#  laser2_init_port_multitask() -- never touches the other 5 ports.
#  ★ For multitask=True, call laser2_init_port_multitask() at the top level,
#    BEFORE run_task(main()) -- not from inside async def main(). Building
#    a device object after run_task() has started is not safe (same rule
#    as MBC_LASER2). For multitask=False (sync) this restriction does not
#    apply -- call it anywhere.
# ════════════════════════════════════════════════════════════════

_devices = {}

# ════════════════════════════════════════════════════════════════
#  Internal state
# ════════════════════════════════════════════════════════════════

_selected_dev  = None
_async_mode    = False
_selected_port = None

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

def _check_ch(ch):
    if ch < 1 or ch > 2:
        raise ValueError("ch must be 1~2 (1=left, 2=right), got {}".format(ch))

# ════════════════════════════════════════════════════════════════
#  Parse helpers (Pure computation, shared by sync / async)
# ════════════════════════════════════════════════════════════════

def _parse_left(p):
    """Left laser (ch1) distance 0~120 cm."""
    return _lo(p[0])

def _parse_right(p):
    """Right laser (ch2) distance 0~120 cm."""
    return _hi(p[0])

def _parse_average(p):
    """Average of left and right, 0~120 cm."""
    return (_lo(p[0]) + _hi(p[0])) >> 1

def _parse_min_distance(p):
    """Closer of left and right, 0~120 cm."""
    return min(_lo(p[0]), _hi(p[0]))

def _parse_min_side(p):
    """-1 = left is closer, +1 = right is closer, 0 = equal or no target on either side."""
    left, right = _lo(p[0]), _hi(p[0])
    if left < right:
        return -1
    if right < left:
        return 1
    return 0

def _parse_diff(p):
    """left - right, -120~+120. Positive = right side is closer."""
    return _lo(p[0]) - _hi(p[0])

def _parse_init_status(p):
    """List of 2 booleans [left, right]: initialized successfully at power-up."""
    mask = _lo(p[1])
    return [(mask & 1) != 0, (mask & 2) != 0]

def _parse_fault_status(p):
    """List of 2 booleans [left, right]: currently faulty (I2C error or timeout)."""
    mask = _hi(p[1])
    return [(mask & 1) != 0, (mask & 2) != 0]

def _parse_read_all(p):
    return {
        "left":         _parse_left(p),
        "right":        _parse_right(p),
        "average":      _parse_average(p),
        "min_distance": _parse_min_distance(p),
        "min_side":     _parse_min_side(p),
        "diff":         _parse_diff(p),
        "init_status":  _parse_init_status(p),
        "fault_status": _parse_fault_status(p),
    }

# ★ Specific to laser2_is_ok: 2 parsers are pre-created when the module is loaded
def _make_is_ok_parser(ch):
    bit = 1 << (ch - 1)
    def _parser(p):
        return (_lo(p[1]) & bit) != 0 and (_hi(p[1]) & bit) == 0
    return _parser

_CH_PARSERS    = [_parse_left, _parse_right]                    # index 0 = ch1 (left)
_IS_OK_PARSERS = [_make_is_ok_parser(ch) for ch in range(1, 3)]  # index 0 = ch1 (left)

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

laser2_left         = _make_sensor_func(_parse_left)
laser2_right        = _make_sensor_func(_parse_right)
laser2_average      = _make_sensor_func(_parse_average)
laser2_min_distance = _make_sensor_func(_parse_min_distance)
laser2_min_side     = _make_sensor_func(_parse_min_side)
laser2_diff         = _make_sensor_func(_parse_diff)
laser2_read_all     = _make_sensor_func(_parse_read_all)

# ★ Pre-created channel specific functions (no new objects at call time)
_CH_FUNCS    = [laser2_left, laser2_right]
_IS_OK_FUNCS = [_make_sensor_func(parser) for parser in _IS_OK_PARSERS]

def laser2_ch(ch):
    """
    Returns distance of a single channel (0~120 cm, 120 = no target).
    ch    : int 1=left, 2=right
    sync  : laser2_ch(1)
    async : await laser2_ch(1)
    """
    _check_ch(ch)
    return _CH_FUNCS[ch - 1]()

def laser2_is_ok(ch):
    """
    Returns True if the channel (1=left, 2=right) initialized successfully AND has no current fault.
    sync  : laser2_is_ok(1)
    async : await laser2_is_ok(1)
    """
    _check_ch(ch)
    return _IS_OK_FUNCS[ch - 1]()

# ════════════════════════════════════════════════════════════════
#  Initialization functions
# ════════════════════════════════════════════════════════════════

def laser2_init_port_multitask(port, multitask=False):
    """
    Initialize sensor and select mode.
      port      : int 1~6  (1=A, 2=B, 3=C, 4=D, 5=E, 6=F)
      multitask : bool, True for use inside async/multitask programs

    ★ For multitask=True, call this at the top level BEFORE run_task(main()),
    not from inside async def main() -- building the port's device object
    must happen before the event loop starts (same rule as MBC_LASER2(...)).
    Only the requested port is touched; the other 5 ports are left alone.
    Returns an awaitable no-op so it can safely be used with
    'await laser2_init_port_multitask(...)'.
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
#  Object-style API (MBC_LASER2 class)
#
#  Graphical block tools place object construction ("object block") in
#  the Setup canvas, before run_task(), while the free functions above are
#  called from an "imported function" block that only lands inside
#  async def main() (after run_task() has already started). Building a
#  PUPDevice there is too late/unsafe.
#
#  MBC_LASER2(port, multitask) builds a PUPDevice for ONLY that one port,
#  in __init__ (before run_task()) -- it never touches the other 5 ports.
#
#  Kept fully independent of the module-level globals above so mixing the
#  function-style calls with this class in the same program can't cause
#  cross-talk.
# ════════════════════════════════════════════════════════════════

class MBC_LASER2:
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

    def left(self):
        return self._get(_parse_left)

    def right(self):
        return self._get(_parse_right)

    def average(self):
        return self._get(_parse_average)

    def min_distance(self):
        return self._get(_parse_min_distance)

    def min_side(self):
        return self._get(_parse_min_side)

    def diff(self):
        return self._get(_parse_diff)

    def read_all(self):
        return self._get(_parse_read_all)

    def ch(self, ch):
        """
        Returns distance of a single channel (0~120 cm, 120 = no target).
        ch : int 1=left, 2=right
        """
        _check_ch(ch)
        return self._get(_CH_PARSERS[ch - 1])

    def is_ok(self, ch):
        """
        Returns True if the channel (1=left, 2=right) initialized successfully AND has no current fault.
        """
        _check_ch(ch)
        return self._get(_IS_OK_PARSERS[ch - 1])

# ════════════════════════════════════════════════════════════════
#  MBC_LASER2 -- callable methods (object-style API)
#
#  laser2 = MBC_LASER2(port, multitask)   -- call BEFORE run_task(main())
#    port      : int 1~6  (1=A, 2=B, 3=C, 4=D, 5=E, 6=F)
#    multitask : False -> sync (call methods directly)
#                True  -> async (await every method call)
#
#  laser2.left()            0~120    left laser distance in cm
#  laser2.right()           0~120    right laser distance in cm
#  laser2.ch(ch)            0~120    single channel distance in cm, ch=1(left)/2(right)
#  laser2.average()         0~120    average of left and right in cm
#  laser2.min_distance()    0~120    closer of left and right in cm
#  laser2.min_side()        -1/0/+1  -1=left closer, +1=right closer, 0=equal/no target
#  laser2.diff()            -120~+120  left - right (positive = right side closer)
#  laser2.is_ok(ch)         bool     channel initialized and currently fault-free
#  laser2.read_all()        dict     all of the above (+ init/fault status) in one hardware read
#
#  Distance 120 means no target or farther than 120 cm.
#
#  Example (sync):
#      laser2 = MBC_LASER2(3, False)
#      print(laser2.left(), laser2.right())
#
#  Example (async / multitask):
#      laser2 = MBC_LASER2(3, True)
#      async def main():
#          print(await laser2.left(), await laser2.right())
#      run_task(main())
# ════════════════════════════════════════════════════════════════
