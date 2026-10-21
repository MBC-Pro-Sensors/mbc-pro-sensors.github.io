# Python Library for Laser 8CH ToF Sensor for Pybricks SPIKE Prime
# Author : MBC robotics
# Date   : 2026.06
#
# !! Import this file MBC_laser8_Lib.py into your Pybricks editor !!
#
# Port mapping (port= argument):
#   1 → Port.A,  2 → Port.B,  3 → Port.C,
#   4 → Port.D,  5 → Port.E,  6 → Port.F

# ════════════════════════════════════════════════════════════════
#  【API 函式說明庫】 (可用於主程式的參考)
#
#  1. 初始化：
#     laser_init_port_multitask(port, multitask)
#       - 說明：初始化感測器並指定連接埠。
#       - 參數：port 填寫 1~6，分別對應 SPIKE 主機的 A~F 埠。
#       - 參數：multitask 填寫 true/false，分別對應 多工任務與非多工任務。
#
#  2. 獨立通道讀取：
#     laser_ch(ch)
#       - 說明：讀取指定單一通道的距離數值。
#       - 參數：ch 填寫 1~8，代表第 1~8 號感測器。
#       - 回傳：整數 0~120（公分，數值越小代表越近，120 代表無目標或超過 120cm）。
#
#     laser_is_ok(ch)
#       - 說明：檢查指定通道的感測器硬體是否正常連線與初始化。
#       - 回傳：True (正常) / False (異常、損壞或排線鬆脫)。
#
#  3. 整體狀態分析：
#     laser_min_direction()
#       - 說明：回傳當前距離「最近」的目標落在哪個感測器方向。
#       - 回傳：整數 0~16（方位，8 = 正前方，越小越偏左、越大越偏右）。
#
#     laser_min_distance()
#       - 說明：回傳當前「最近目標」的距離數值。
#       - 回傳：整數 0~120（公分）。
#
#     laser_distances()
#       - 說明：一次取得所有 8 顆感測器的距離陣列。
#       - 回傳：長度為 8 的整數陣列 (list)，例如 [120, 50, 120, ...]。
#
#     laser_init_status()
#       - 說明：一次取得所有 8 顆感測器的連線狀態陣列。
#       - 回傳：長度為 8 的布林值陣列 (list)，例如 [True, True, False, ...]。
#
#     laser_read_all()
#       - 說明：一次取得所有解析完畢的綜合狀態字典 (Dictionary)。
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
#  Packet layout (mode 9, 14 bytes raw → 7 int16 from Pybricks)
#
#  word[0] : lo=sensorData[0], hi=sensorData[1]
#  word[1] : lo=sensorData[2], hi=sensorData[3]
#  word[2] : lo=sensorData[4], hi=sensorData[5]
#  word[3] : lo=sensorData[6], hi=sensorData[7]
#  word[4] : lo=minDirection,  hi=minDistance
#  word[5] : lo=maxDirection,  hi=maxDistance
#  word[6] : lo=sensorInitOk,  hi=fault bitmap
# ════════════════════════════════════════════════════════════════

_MODE = 9

def _lo(word):
    """取得低位元組"""
    return word & 0xFF

def _hi(word):
    """取得高位元組"""
    return (word >> 8) & 0xFF

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

def _parse_distances(p):
    """Returns list of 8 integers representing distances of channel 1~8 (0~120 cm)."""
    return [
        _lo(p[0]), _hi(p[0]),
        _lo(p[1]), _hi(p[1]),
        _lo(p[2]), _hi(p[2]),
        _lo(p[3]), _hi(p[3])
    ]

def _parse_min_direction(p):
    """Closest target direction 0~16 (8 = front)."""
    return _lo(p[4])

def _parse_min_distance(p):
    """Closest target distance 0~120 cm."""
    return _hi(p[4])

def _parse_init_status(p):
    """Returns a list of booleans indicating initialization status of channels 1~8."""
    init_mask = _lo(p[6])
    return [(init_mask & (1 << i)) != 0 for i in range(8)]

def _parse_read_all(p):
    return {
        "distances":       _parse_distances(p),
        "min_direction":   _parse_min_direction(p),
        "min_distance":    _parse_min_distance(p),
        "init_status":     _parse_init_status(p),
    }

# ★ Specific to laser_ch: 8 parsers are pre-created when the module is loaded
def _make_laser_ch_parser(ch):
    word_idx = (ch - 1) // 2
    is_hi    = (ch - 1) % 2 != 0
    def _parser(p):
        return _hi(p[word_idx]) if is_hi else _lo(p[word_idx])
    return _parser

_LASER_CH_PARSERS = [_make_laser_ch_parser(ch) for ch in range(1, 9)]

# ════════════════════════════════════════════════════════════════
#  Factory function: Pass in a parser to automatically generate a dual-mode + cache public function
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

laser_distances         = _make_sensor_func(_parse_distances)
laser_min_direction     = _make_sensor_func(_parse_min_direction)
laser_min_distance      = _make_sensor_func(_parse_min_distance)
laser_init_status       = _make_sensor_func(_parse_init_status)
laser_read_all          = _make_sensor_func(_parse_read_all)

# ★ Pre-created channel specific functions
_LASER_CH_FUNCS = [_make_sensor_func(parser) for parser in _LASER_CH_PARSERS]

def laser_ch(ch):
    """
    Returns distance (0~120 cm) of a single channel.
    ch    : int 1~8
    sync  : laser_ch(1)
    async : await laser_ch(1)
    """
    if ch < 1 or ch > 8:
        raise ValueError("ch must be 1~8, got {}".format(ch))
    return _LASER_CH_FUNCS[ch - 1]()

def laser_is_ok(ch):
    """
    Returns True if the specified channel (1~8) was initialized successfully.
    """
    if ch < 1 or ch > 8:
        raise ValueError("ch must be 1~8, got {}".format(ch))
    def _parser(p):
        return (_lo(p[6]) & (1 << (ch - 1))) != 0
    return _make_sensor_func(_parser)()

# ════════════════════════════════════════════════════════════════
#  Initialization functions
# ════════════════════════════════════════════════════════════════

def laser_init_port_multitask(port, multitask=False):
    """
    Initialize sensor and select mode.
      port      : int 1~6  (1=A, 2=B, 3=C, 4=D, 5=E, 6=F)
      multitask : bool, True for use inside async/multitask programs
    """
    global _selected_dev, _async_mode, _selected_port
    global _cache, _cache_time
    _selected_port = port
    _selected_dev  = _devices.get(port)
    _async_mode    = bool(multitask)
    _cache         = None   # Cache resets upon port switch
    _cache_time    = 0
    return _noop()
