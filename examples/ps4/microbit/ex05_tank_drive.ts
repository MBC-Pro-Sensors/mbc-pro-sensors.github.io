// 掌控者 PS4／PS5 × micro:bit 範例 5：雙搖桿坦克＋安全啟動（綜合）
// 做什麼：左搖桿管左輪、右搖桿管右輪，推越多越快。一開始要兩支搖桿都放開（在中間）才會「解鎖」開車；斷線就鎖回去。按 A 顯示手把電量；電量低於 20% 燈條變紅。
// 接線：同範例 4（接收器 P19／P20、外接 5 V；左輪 P1、右輪 P2、伺服外接電池、共地）。
// 準備：兩輪小車。
// 看到什麼：放開兩支搖桿 → 打勾（解鎖）、燈條綠；兩支都往前推 → 前進；一前一後 → 原地轉；放開 → 停；關手把 → X 並鎖住。

const DEAD = 15                                           // 死區：搖桿在 ±15 以內當成 0
let armed = false
let lowBattery = false

// 左輪伺服 P1、右輪伺服 P2（連續旋轉伺服）；left／right = −100~100，正 = 前進
function drive(left: number, right: number): void {
    left = Math.constrain(left, -100, 100)
    right = Math.constrain(right, -100, 100)
    pins.servoWritePin(AnalogPin.P1, Math.round(90 + left * 0.9))    // 90 = 停、180 = 全速前進、0 = 全速後退
    pins.servoWritePin(AnalogPin.P2, Math.round(90 - right * 0.9))   // 右輪裝的方向相反，所以用減的
}

function deadZone(v: number): number {                    // 搖桿在中間附近就當 0，放開手車子才會停
    if (Math.abs(v) < DEAD) {
        return 0
    }
    return v
}

input.onButtonPressed(Button.A, function () {
    basic.showNumber(mbc.padBattery())                    // 手把電量 0~100（PS2 手把恆 0）
})

basic.forever(function () {
    if (!mbc.padConnected()) {
        drive(0, 0)
        armed = false                                     // 斷線後要重新解鎖
        basic.showIcon(IconNames.No, 0)
        basic.pause(200)
        return
    }
    let ly = deadZone(mbc.padStick(MbcPadAxis.LeftY))     // 左搖桿上下 −100~+100
    let ry = deadZone(mbc.padStick(MbcPadAxis.RightY))    // 右搖桿上下
    if (!armed) {
        if (ly == 0 && ry == 0) {                         // 兩支都放開才解鎖
            armed = true
            basic.showIcon(IconNames.Yes, 0)
            mbc.padLight(0x00FF00)                        // 燈條綠色 = 可以開了
        }
        basic.pause(50)
        return
    }
    drive(ly, ry)                                         // 左搖桿 → 左輪、右搖桿 → 右輪
    let low = mbc.padBattery() < 20
    if (low != lowBattery) {                              // 電量狀態變了才改燈條
        lowBattery = low
        mbc.padLight(low ? 0xFF0000 : 0x00FF00)           // 低電量紅、正常綠
    }
    basic.pause(20)
})
