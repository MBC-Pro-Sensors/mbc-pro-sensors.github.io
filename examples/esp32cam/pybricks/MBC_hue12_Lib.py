# MBC 神攝手 WRO 版（HUE12）Pybricks 函式庫
# 下面的 HUE12Sensor 類別一字不改照抄官方 Hue12_Sensor_For_pyBricks_python.py，
# 只拿掉原檔最後的示範主程式（原檔是完整程式，import 會卡在它自己的無限迴圈）。
# 需要 Pybricks 韌體 v4.0.0b5 以上（MBC 積木網站燒的 v4.1.0 可以）。
#
#   from MBC_hue12_Lib import HUE12Sensor
#   cam = HUE12Sensor(Port.C)                 # HUE12 插在 C 孔
#   cam.update()                              # 讀一次最新資料
#   name, hue, sat = cam.get_slot(1)          # 第 1 格：顏色名稱、色相、飽和度
#
# 色相：0~239 彩色、240~245 白（Wht）、246~250 黑（Blk）、255 = 這一格沒設定（NONE）
from pybricks.iodevices import PUPDevice

class HUE12Sensor:
    # Initialize the sensor on the specified port
    def __init__(self, port):
        # Create a PUPDevice object for UART communication
        self.device = PUPDevice(port)
        # Display the connected device ID for verification
        print("Connected to ID:", self.device.info()['id'])
        # Initialize 12 data slots with default values (Hue=255, Sta=0, Name="NONE")
        self.slots = [(255, 0, "NONE")] * 12

    # Internal helper to map numeric Hue values to color names
    def _get_color_name(self, hue):
        # 255 indicates no object detected or inactive slot
        if hue == 255: return "NONE"
        # Special ranges for grayscale (Black and White)
        if 246 <= hue <= 250: return "Blk"
        if 240 <= hue <= 245: return "Wht"

        # Map hue ranges to specific color names
        if hue >= 230 or hue < 10: return "Red"
        if 10 <= hue < 30:         return "Org"
        if 30 <= hue < 50:         return "Yel"
        if 50 <= hue < 70:         return "Lim"
        if 70 <= hue < 90:         return "Grn"
        if 90 <= hue < 110:        return "Mnt"
        if 110 <= hue < 130:       return "Cyn"
        if 130 <= hue < 150:       return "Azr"
        if 150 <= hue < 170:       return "Blu"
        if 170 <= hue < 190:       return "Prp"
        if 190 <= hue < 210:       return "Mag"
        if 210 <= hue < 230:       return "Pnk"
        # Return "Unk" if value is outside defined ranges
        return "Unk"

    # Fetch and decode data from the sensor
    def update(self):
        try:
            # Read raw data from mode 0
            raw_data = self.device.read(0)
        except OSError:
            # Return False if communication fails
            return False

        # Check if received data is valid and contains enough elements
        if not raw_data or len(raw_data) < 8:
            return False

        # List to store the unpacked 8-bit bytes
        unpacked_bytes = []

        # Decoding Logic: Unpack 32-bit integers into bytes
        for k in range(8):
            # Get the k-th 32-bit integer
            val = raw_data[k]
            # Extract first byte (Hue)
            b0 = val & 0xFF
            # Extract second byte (Status/Sta)
            b1 = (val >> 8) & 0xFF
            # Extract third byte (Next Hue)
            b2 = (val >> 16) & 0xFF
            # Add extracted bytes to the list
            unpacked_bytes.extend([b0, b1, b2])

        # Parse data for each of the 12 slots
        for i in range(12):
            # Calculate index for Hue value
            idx_h = i * 2
            # Calculate index for Saturation/Status value
            idx_s = i * 2 + 1

            # Ensure index is within data bounds
            if idx_s < len(unpacked_bytes):
                # Retrieve Hue value
                h = unpacked_bytes[idx_h]
                # Retrieve Saturation value
                s = unpacked_bytes[idx_s]
                # Get color name based on Hue
                name = self._get_color_name(h)
                # Save data to local cache
                self.slots[i] = (h, s, name)
        
        # Return True for successful update
        return True

    # Retrieve cached data for a specific slot ID (1-12)
    def get_slot(self, slot_id):
        # Validate slot ID range
        if 1 <= slot_id <= 12:
            # Return cached data (adjust for 0-based list)
            h, s, name = self.slots[slot_id - 1]
            return name, h, s
        else:
            # Return error values if ID is out of range
            return "Err", 0, 0
