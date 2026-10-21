# Python Object Library for MBC Laser 8CH ToF Sensor for Pybricks SPIKE Prime
# Author : MBC robotics
# Date   : 2026.09
#
# !! Import this file MBC_laser8_obj_Lib.py into your Pybricks editor !!
#
# Port mapping (port= argument):
#   1 → Port.A,  2 → Port.B,  3 → Port.C,
#   4 → Port.D,  5 → Port.E,  6 → Port.F

# ════════════════════════════════════════════════════════════════
#  【API 函式說明庫】 (可用於主程式的參考)
#
#  距離單位一律為「公分」0~120；120 代表無目標或超過 120cm。
#  方向一律為 -8~+8；0 = 正前方，負值 = CH1 側，正值 = CH8 側。
#
#  1. 初始化：
#     laser8_init_port_multitask(port, multitask)
#       - 說明：初始化感測器並指定連接埠。
#       - 參數：port 填寫 1~6，分別對應 SPIKE 主機的 A~F 埠。
#       - 參數：multitask 填寫 True/False，分別對應 多工任務與非多工任務。
#       - ★ multitask=True 時，必須在 run_task(main()) 之前（最外層）呼叫。
#
#  2. 獨立通道讀取：
#     laser8_ch(ch)
#       - 說明：讀取指定單一通道的距離。
#       - 參數：ch 填寫 1~8，代表第 1~8 號感測器。
#       - 回傳：整數 0~120（公分）。
#
#     laser8_is_ok(ch)
#       - 說明：檢查指定通道是否「開機初始化成功，且目前沒有故障」。
#       - 回傳：True (正常) / False (初始化失敗、I2C 錯誤或逾時無回應)。
#
#  3. 整體狀態分析：
#     laser8_min_direction()
#       - 說明：最近目標所在的方向。
#       - 回傳：整數 -7~+7；若周圍都沒有目標，回傳 -8 或 +8（上次目標偏向的那一側）。
#
#     laser8_min_distance()
#       - 說明：最近目標的距離。
#       - 回傳：整數 0~120（公分）。
#
#     laser8_max_direction()
#       - 說明：最遠（最空曠）方向。
#       - 回傳：整數 -7~+7。
#
#     laser8_max_distance()
#       - 說明：最遠方向的距離。
#       - 回傳：整數 0~120（公分）。
#
#     laser8_distances()
#       - 說明：一次取得 8 顆感測器的距離陣列。
#       - 回傳：長度為 8 的整數 list，例如 [120, 50, 120, ...]。
#
#     laser8_init_status()
#       - 說明：8 顆感測器開機初始化是否成功。
#       - 回傳：長度為 8 的布林 list，例如 [True, True, False, ...]。
#
#     laser8_fault_status()
#       - 說明：8 顆感測器目前是否故障（I2C 錯誤或逾時無回應）。
#       - 回傳：長度為 8 的布林 list，True = 故障。
#
#     laser8_read_all()
#       - 說明：一次取得所有解析完畢的綜合狀態字典 (dict)，只讀一次硬體。
#
#  4. 物件式寫法（圖形化工具的 Setup 區使用）：
#     laser8 = MBC_LASER8(port, multitask)，方法名稱去掉 laser8_ 前綴，
#     例如 laser8.ch(3)、laser8.min_distance()，詳見檔案最下方說明。
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
#  word[0] : buf[0]=dist(ch1),     buf[1]=dist(ch2)     → 0~120 cm each
#  word[1] : buf[2]=dist(ch3),     buf[3]=dist(ch4)
#  word[2] : buf[4]=dist(ch5),     buf[5]=dist(ch6)
#  word[3] : buf[6]=dist(ch7),     buf[7]=dist(ch8)
#  word[4] : buf[8]=minDirection,  buf[9]=minDistance   (direction 0~16, 8=front)
#  word[5] : buf[10]=maxDirection, buf[11]=maxDistance
#  word[6] : buf[12]=initOk bitmap, buf[13]=fault bitmap (bit0=ch1 ... bit7=ch8)
#
#  Note: word[6] may exceed 0x7FFF, so Pybricks returns it as a negative
#  int16 -- _u16() converts it back before splitting bytes.
# ════════════════════════════════════════════════════════════════

_MODE          = 9
_BEARING_CENTER = 8

def _u16(word):
    """Convert signed int16 (from Pybricks) to unsigned uint16."""
    return word & 0xFFFF if word < 0 else word

def _lo(word):
    return _u16(word) & 0xFF

def _hi(word):
    return (_u16(word) >> 8) & 0xFF

# ════════════════════════════════════════════════════════════════
#  PUPDevice is built lazily, only for the port actually passed to
#  laser8_init_port_multitask() -- never touches the other 5 ports.
#  ★ For multitask=True, call laser8_init_port_multitask() at the top level,
#    BEFORE run_task(main()) -- not from inside async def main(). Building
#    a device object after run_task() has started is not safe (same rule
#    as MBC_LASER8). For multitask=False (sync) this restriction does not
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
    if ch < 1 or ch > 8:
        raise ValueError("ch must be 1~8, got {}".format(ch))

# ════════════════════════════════════════════════════════════════
#  Parse helpers (Pure computation, shared by sync / async)
# ════════════════════════════════════════════════════════════════

def _parse_distances(p):
    """
    Unpack 8 distances (0~120 cm each) from words 0~3.
    Index 0 = channel 1, index 7 = channel 8. 120 = no target.
    """
    values = []
    for word_idx in range(4):
        values.append(_lo(p[word_idx]))  # odd channel  (ch1, ch3, ch5, ch7)
        values.append(_hi(p[word_idx]))  # even channel (ch2, ch4, ch6, ch8)
    return values

def _parse_min_direction(p):
    """Closest target direction -8~+8 (0=front). ±8 = no target, last seen side."""
    return _lo(p[4]) - _BEARING_CENTER

def _parse_min_distance(p):
    """Closest target distance 0~120 cm."""
    return _hi(p[4])

def _parse_max_direction(p):
    """Farthest (most open) direction -8~+8 (0=front)."""
    return _lo(p[5]) - _BEARING_CENTER

def _parse_max_distance(p):
    """Farthest distance 0~120 cm."""
    return _hi(p[5])

def _parse_init_status(p):
    """List of 8 booleans: channel initialized successfully at power-up."""
    mask = _lo(p[6])
    return [(mask & (1 << i)) != 0 for i in range(8)]

def _parse_fault_status(p):
    """List of 8 booleans: channel currently faulty (I2C error or timeout)."""
    mask = _hi(p[6])
    return [(mask & (1 << i)) != 0 for i in range(8)]

def _parse_read_all(p):
    return {
        "distances":     _parse_distances(p),
        "min_direction": _parse_min_direction(p),
        "min_distance":  _parse_min_distance(p),
        "max_direction": _parse_max_direction(p),
        "max_distance":  _parse_max_distance(p),
        "init_status":   _parse_init_status(p),
        "fault_status":  _parse_fault_status(p),
    }

# ★ Specific to laser8_ch / laser8_is_ok: 8 parsers each are pre-created when the module is loaded
def _make_ch_parser(ch):
    word_idx = (ch - 1) // 2
    is_hi    = (ch - 1) % 2 != 0
    def _parser(p):
        return _hi(p[word_idx]) if is_hi else _lo(p[word_idx])
    return _parser

def _make_is_ok_parser(ch):
    bit = 1 << (ch - 1)
    def _parser(p):
        return (_lo(p[6]) & bit) != 0 and (_hi(p[6]) & bit) == 0
    return _parser

_CH_PARSERS    = [_make_ch_parser(ch) for ch in range(1, 9)]     # index 0 = ch1
_IS_OK_PARSERS = [_make_is_ok_parser(ch) for ch in range(1, 9)]  # index 0 = ch1

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

laser8_distances     = _make_sensor_func(_parse_distances)
laser8_min_direction = _make_sensor_func(_parse_min_direction)
laser8_min_distance  = _make_sensor_func(_parse_min_distance)
laser8_max_direction = _make_sensor_func(_parse_max_direction)
laser8_max_distance  = _make_sensor_func(_parse_max_distance)
laser8_init_status   = _make_sensor_func(_parse_init_status)
laser8_fault_status  = _make_sensor_func(_parse_fault_status)
laser8_read_all      = _make_sensor_func(_parse_read_all)

# ★ Pre-created channel specific functions (no new objects at call time)
_CH_FUNCS    = [_make_sensor_func(parser) for parser in _CH_PARSERS]
_IS_OK_FUNCS = [_make_sensor_func(parser) for parser in _IS_OK_PARSERS]

def laser8_ch(ch):
    """
    Returns distance of a single channel (0~120 cm, 120 = no target).
    ch    : int 1~8
    sync  : laser8_ch(1)
    async : await laser8_ch(1)
    """
    _check_ch(ch)
    return _CH_FUNCS[ch - 1]()

def laser8_is_ok(ch):
    """
    Returns True if the channel (1~8) initialized successfully AND has no current fault.
    sync  : laser8_is_ok(1)
    async : await laser8_is_ok(1)
    """
    _check_ch(ch)
    return _IS_OK_FUNCS[ch - 1]()

# ════════════════════════════════════════════════════════════════
#  Initialization functions
# ════════════════════════════════════════════════════════════════

def laser8_init_port_multitask(port, multitask=False):
    """
    Initialize sensor and select mode.
      port      : int 1~6  (1=A, 2=B, 3=C, 4=D, 5=E, 6=F)
      multitask : bool, True for use inside async/multitask programs

    ★ For multitask=True, call this at the top level BEFORE run_task(main()),
    not from inside async def main() -- building the port's device object
    must happen before the event loop starts (same rule as MBC_LASER8(...)).
    Only the requested port is touched; the other 5 ports are left alone.
    Returns an awaitable no-op so it can safely be used with
    'await laser8_init_port_multitask(...)'.
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
#  Object-style API (MBC_LASER8 class)
#
#  Graphical block tools place object construction ("object block") in
#  the Setup canvas, before run_task(), while the free functions above are
#  called from an "imported function" block that only lands inside
#  async def main() (after run_task() has already started). Building a
#  PUPDevice there is too late/unsafe.
#
#  MBC_LASER8(port, multitask) builds a PUPDevice for ONLY that one port,
#  in __init__ (before run_task()) -- it never touches the other 5 ports.
#
#  Kept fully independent of the module-level globals above so mixing the
#  function-style calls with this class in the same program can't cause
#  cross-talk.
# ════════════════════════════════════════════════════════════════

class MBC_LASER8:
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

    def distances(self):
        return self._get(_parse_distances)

    def min_direction(self):
        return self._get(_parse_min_direction)

    def min_distance(self):
        return self._get(_parse_min_distance)

    def max_direction(self):
        return self._get(_parse_max_direction)

    def max_distance(self):
        return self._get(_parse_max_distance)

    def init_status(self):
        return self._get(_parse_init_status)

    def fault_status(self):
        return self._get(_parse_fault_status)

    def read_all(self):
        return self._get(_parse_read_all)

    def ch(self, ch):
        """
        Returns distance of a single channel (0~120 cm, 120 = no target).
        ch : int 1~8
        """
        _check_ch(ch)
        return self._get(_CH_PARSERS[ch - 1])

    def is_ok(self, ch):
        """
        Returns True if the channel (1~8) initialized successfully AND has no current fault.
        """
        _check_ch(ch)
        return self._get(_IS_OK_PARSERS[ch - 1])

# ════════════════════════════════════════════════════════════════
#  MBC_LASER8 -- callable methods (object-style API)
#
#  laser8 = MBC_LASER8(port, multitask)   -- call BEFORE run_task(main())
#    port      : int 1~6  (1=A, 2=B, 3=C, 4=D, 5=E, 6=F)
#    multitask : False -> sync (call methods directly)
#                True  -> async (await every method call)
#
#  laser8.ch(ch)            0~120    single channel distance in cm, ch=1~8
#  laser8.distances()       [int]*8  all 8 channels' distance in cm
#  laser8.min_direction()   -8~+8    closest target direction (0=front, ±8=no target)
#  laser8.min_distance()    0~120    closest target distance in cm
#  laser8.max_direction()   -8~+8    farthest / most open direction (0=front)
#  laser8.max_distance()    0~120    farthest distance in cm
#  laser8.is_ok(ch)         bool     channel initialized and currently fault-free
#  laser8.init_status()     [bool]*8 initialized at power-up, per channel
#  laser8.fault_status()    [bool]*8 currently faulty (I2C error / timeout), per channel
#  laser8.read_all()        dict     all of the above in one dict, one hardware read
#
#  Distance 120 means no target or farther than 120 cm.
#  Negative direction = CH1 side, positive direction = CH8 side.
#
#  Example (sync):
#      laser8 = MBC_LASER8(3, False)
#      print(laser8.min_direction(), laser8.min_distance())
#
#  Example (async / multitask):
#      laser8 = MBC_LASER8(3, True)
#      async def main():
#          print(await laser8.min_direction(), await laser8.min_distance())
#      run_task(main())
# ════════════════════════════════════════════════════════════════
