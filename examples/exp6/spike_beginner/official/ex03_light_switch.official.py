# EXP6 範例 3：光控開關（官方 SPIKE App・Python）
# 感應器前面亮（放一張白紙）→ 馬達轉、主機燈變綠；暗 → 馬達停、燈變紅
# 接線：EXP6 的 Host 線 → 主機 C 孔；馬達 → 擴充孔 1；顏色感應器 → 擴充孔 4；EXP6 接上電池
from hub import port, button, light
import color
import time
from MBC_exp6_SPIKE_App_Lib import MBC_EXP6

exp6 = MBC_EXP6(port.C)
time.sleep(2)

while not (button.pressed(button.LEFT) and button.pressed(button.RIGHT)):   # 左右鍵一起按住 → 結束
    bright = exp6.get_color_reflection(4)    # 擴充孔 4 的顏色感應器：反射光 0~100（越亮越大）
    print(bright)                            # 在電腦畫面印出來，方便看數字
    if bright > 50:                          # 亮 → 馬達轉
        exp6.motor_run(1, 50)
        light.color(light.POWER, color.GREEN)
    else:                                    # 暗 → 馬達停
        exp6.motor_stop(1, 2)
        light.color(light.POWER, color.RED)
    time.sleep(0.1)

exp6.stop_all(1)             # 結束前一定要全部停止
