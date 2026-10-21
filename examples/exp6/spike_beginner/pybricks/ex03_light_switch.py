# EXP6 範例 3：光控開關（Pybricks）
# 感應器前面亮（放一張白紙）→ 馬達轉、主機燈變綠；暗 → 馬達停、燈變紅
# 接線：EXP6 的 Host 線 → 主機 C 孔；馬達 → 擴充孔 1；顏色感應器 → 擴充孔 4；EXP6 接上電池
from pybricks.hubs import PrimeHub
from pybricks.parameters import Button, Color
from MBC_exp6_obj_Lib import MBC_EXP6

hub = PrimeHub()
exp6 = MBC_EXP6(3, False)    # 3 = C 孔

while exp6.get_device_id(1) != 5:        # 等擴充孔 1 的馬達準備好
    exp6.keep_alive_wait(100)

while not (Button.LEFT in hub.buttons.pressed() and Button.RIGHT in hub.buttons.pressed()):   # 左右鍵一起按住 → 結束
    bright = exp6.get_color_reflection(4)    # 擴充孔 4 的顏色感應器：反射光 0~100（越亮越大）
    print(bright)
    if bright > 50:                          # 亮 → 馬達轉
        exp6.motor_run(1, 50)
        hub.light.on(Color.GREEN)
    else:                                    # 暗 → 馬達停
        exp6.motor_stop(1, 2)
        hub.light.on(Color.RED)
    exp6.keep_alive_wait(100)

exp6.stop_all(1)
