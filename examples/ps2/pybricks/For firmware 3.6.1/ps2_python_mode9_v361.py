from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port
from pybricks.tools import wait

class PS2:
    def __init__(self, port):
        # 初始化底層裝置
        self._dev = PUPDevice(port)
        self.raw = [0] * 7
        
    def update(self):
        """更新所有按鈕與搖桿狀態（在主迴圈開頭呼叫一次）"""
        self.raw = self._dev.read(9)
        
    # --- 搖桿 4 軸（直接讀取對應的陣列位置，數值範圍 1 ~ 9） ---
    def LX(self): return self.raw[2]
    def LY(self): return self.raw[3]
    def RX(self): return self.raw[4]
    def RY(self): return self.raw[5]

    # --- 獨立按鈕狀態（利用已分流的 raw[0] 與 raw[1] 進行遮罩拆解） ---
    def up(self):     return (self.raw[0] // 1) % 2 == 1
    def right(self):  return (self.raw[0] // 2) % 2 == 1
    def down(self):   return (self.raw[0] // 4) % 2 == 1
    def left(self):   return (self.raw[0] // 8) % 2 == 1
    def tri(self):    return (self.raw[0] // 16) % 2 == 1
    def circle(self): return (self.raw[0] // 32) % 2 == 1
    def cross(self):  return (self.raw[0] // 64) % 2 == 1
    def square(self): return (self.raw[0] // 128) % 2 == 1
    
    def l2(self):     return (self.raw[1] // 1) % 2 == 1
    def r2(self):     return (self.raw[1] // 2) % 2 == 1
    def l1(self):     return (self.raw[1] // 4) % 2 == 1
    def r1(self):     return (self.raw[1] // 8) % 2 == 1
    def select(self): return (self.raw[1] // 16) % 2 == 1
    def l3(self):     return (self.raw[1] // 32) % 2 == 1
    def r3(self):     return (self.raw[1] // 64) % 2 == 1
    def start(self):  return (self.raw[1] // 128) % 2 == 1    

# ==================== 主程式應用範例 ====================

# 初始化在連接埠 C
joy = PS2(Port.C)

while True:
    # 1. 讀取最新數據
    joy.update()
    
    # 2. 獨立條件判斷（每個按鈕都是獨立的「如果...那麼」積木）
    if joy.up():    print("按鈕：上")
    if joy.right(): print("按鈕：右")
    if joy.down():  print("按鈕：下")
    if joy.left():  print("按鈕：左")
    if joy.tri():   print("按鈕：▲")
    if joy.circle():print("按鈕：●")
    if joy.cross(): print("按鈕：Ⅹ")
    if joy.square():print("按鈕：■")
    if joy.l1():    print("按鈕：L1")
    if joy.r1():    print("按鈕：R1")
    if joy.select():print("按鈕：SELECT")
    if joy.start(): print("按鈕：START")
    if joy.l2():    print("按鈕：L2")
    if joy.r2():    print("按鈕：R2")
    if joy.l3():    print("按鈕：L3 (左搖桿壓下)")
    if joy.r3():    print("按鈕：R3 (右搖桿壓下)")
        
    # 3. 搖桿條件判斷：只要 X 或 Y 離開中心點 5，就顯示數值
    if joy.LX() != 5 or joy.LY() != 5:
        print("左搖桿 X:", joy.LX(), "Y:", joy.LY())
        
    if joy.RX() != 5 or joy.RY() != 5:
        print("右搖桿 X:", joy.RX(), "Y:", joy.RY())
        
    wait(10)