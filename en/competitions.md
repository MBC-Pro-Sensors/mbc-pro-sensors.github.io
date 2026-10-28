<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# 🏁 Competition Guide: Which Robotics Competitions Allow MBC Sensors?

Not every robotics competition allows third-party sensors. This page lists competitions in **Taiwan, Asia and worldwide** that are open to other brands, checks each one against its official rules, and shows which MBC products you can use. Last checked: 2026-10-14.

> [!IMPORTANT]
> This page is based on the **2026** rules. Most Taiwan events for 2026 have already taken place, and we will update the page when the 2027 rules are published. Rules change every year, so **always check the organizer's latest rules before you register**. For items marked ⚠️, ask the organizer first.

## At a Glance: Products × Competitions

<style>.markdown-section table th{white-space:nowrap}.markdown-section table td{white-space:nowrap}.markdown-section table td:first-child{white-space:normal;min-width:9em}</style>

✅ Allowed　⚠️ Ask the organizer　❌ Not allowed

| Competition | Pathfinder | Ranger | Vision | EXP6 | Controller | Servo6 |
| :--- | :---: | :---: | :---: | :---: | :---: | :---: |
| [WRO RoboMission](/en/wro-rules.md) | ✅ | ✅⁴ | ✅² | ✅ | ❌ | ✅ |
| [WRO Future Engineers](#wro-future-engineers) | ✅ | ✅ | ✅ | ✅ | ❌ | ✅ |
| [Robofest](#robofest) | ✅ | ✅ | ✅ | ✅ | ❌ | ✅ |
| [RoboCupJunior](#robocupjunior) Rescue | ❌ | ⚠️ | ⚠️ | ⚠️ | ❌ | ⚠️ |
| [Robotex](#robotex-international) | ⚠️ | ⚠️ | ⚠️ | ⚠️ | ⚠️ | ⚠️ |
| [AERC](#aerc-taiwan) Group A (LEGO) | ✅ | ❌ | ❌ | ❌ | ❌ | ❌ |
| [AERC](#aerc-taiwan) Group D (any brand) | ✅ | ✅ | ✅ | ✅ | ✅³ | ✅ |
| [GRG](#grg-global-robotics-games-taiwan) AI Maker, Open | ✅ | ✅ | ✅¹ | ✅ | ❌ | ✅ |
| [MRC](#mrc-minoan-robotsports-competition-taiwan) | ✅ | ✅ | ✅ | ❌ | ✅³ | ❌ |
| [JBC](#jbc-junior-botball-challenge-taiwan) | ✅ | ✅ | ✅ | ✅ | ❌ | ✅ |
| [FLL Challenge](#competitions-where-mbc-sensors-cannot-be-used) | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ |

¹ One camera per robot　² Cameras in Junior and Senior only　³ For remote-control events　Servo6 = SPIKE 6-Port Servo Expander (coming soon; treated like EXP6: a programmable expansion device, servos count as motors)　⁴ Ranger 8-Way: Elementary and Junior teams should check with the organizer first

Pathfinder = 8 / 16-way line sensor, Ranger = 2 / 8-way laser ranging, Vision = Sharpshooter Vision camera sensor (in development), EXP6 = SPIKE 6-Way Expander (SPIKE™ Prime only), Controller = PS4/PS5 receiver.

## Worldwide

### WRO Future Engineers

A self-driving car challenge for high-school students and older: build a car that drives itself around a randomly configured track. **Teams choose their own hardware, electronics, controllers and programming language**, within the technical rules. Ranger works well for wall detection and Sharpshooter Vision for recognizing the red and green traffic signs.

Official information: [WRO Association](https://wro-association.org/)

### WRO RoboMission

Open to any brand since 2025, but cameras are allowed only in the Junior and Senior categories, and lasers need an eye-safety statement. See the full [WRO Rules Check](/en/wro-rules.md).

### Robofest

An international competition run by Lawrence Technological University (LTU), with the World Championship held in the USA every May. Robots must be fully autonomous and programmed from scratch by the team. **Any small robot platform and any programming language are allowed.**

Official information: [Robofest](https://www.robofest.net/)

### RoboCupJunior

The youth leagues of RoboCup. The 2026 world championship is in Incheon, Korea, with regional events such as RoboCup Asia-Pacific. Platforms are open, but the Rescue rules **ban commercial modules designed or marketed to solve a single primary task** ("task-solvers"). Ready-made line-following modules fall into this group, so **Pathfinder cannot be used in Rescue Line**. Ranger and Sharpshooter Vision are general-purpose sensors, but the organizer decides, so ask first.

Official information: [RoboCupJunior](https://junior.robocup.org/)

### Robotex International

An international robotics festival from Estonia, held in Seoul, Korea for the first time in 2026. National qualifiers run both LEGO and custom-robot categories (line following, sumo and more). Categories and hardware rules differ by country, so check your national rules.

Official information: [Robotex International](https://robotex.international/)

## Taiwan

### AERC (Taiwan)

The Asia Educational Robot Competition (亞洲智慧型機器人大賽), held for the 38th time in 2026, runs three venues in Taichung, Taoyuan and Tainan for primary school through university.

- **Group A (LEGO)**: LEGO® (including SPIKE™) controllers, sensors and motors only, but the rules **name "MBC Pathfinder – LEGO edition" as an allowed sensor**, together with Mindsensors and HiTechnic.
- **Group D (any brand)**: Any brand of components, as long as the event rules are met. All MBC products can be used.
- **Matching events**: line following (Pathfinder), programmed obstacle avoidance (Ranger), remote-controlled obstacle avoidance (Controller, Group D) and AI vision line following (Sharpshooter Vision, Group D).

Official information: [AERC website](http://www.aerc1988.com.tw)｜[2026 rules (Chinese)](https://jr.hs.ntnu.edu.tw/wp-content/uploads/2026/03/第38屆AERC亞洲智慧型機器人大賽競賽辦法.pdf)

### GRG (Global Robotics Games) Taiwan

Organized in Taiwan by the Taiwan Smart Programming AI Robotics & Knowledge Association (SPARK). Hardware rules differ by category:

- **AI Maker Series**: Any building materials and any controller (Arduino, Raspberry Pi, SPIKE™ Prime and more), and more than one controller is allowed. **One camera maximum**; primary-school teams must use block-based coding. Cars pre-programmed by the manufacturer to follow a line or route are not allowed.
- **Open Category**: Project-based, no hardware restriction (24 V maximum).
- **Mobile Robotics 1–3**: **Official LEGO® parts only**, so MBC products cannot be used.
- Bluetooth and Wi-Fi must be off during matches; no remote control.

Official information: [GRG Taiwan](https://global-robotics-games-taiwan.github.io/)

### MRC (Minoan Robotsports Competition) Taiwan

The Taiwan qualifier, organized by SPARK, has more than 20 robot sports such as soccer, hockey, sumo and marathon, many of them remote-controlled.

- The original controller of an educational kit (LEGO, mBot, Maqueen and others) must be used.
- **Third-party sensors are allowed** if they are electrically compatible with the controller and unmodified. Pathfinder, Ranger and Sharpshooter Vision can all be used.
- **Why Sharpshooter Vision and the Controller are allowed**: the ban on added microcontrollers such as Arduino or ESP32 stops teams from controlling the robot with a second control board. Sharpshooter Vision and the Controller are a sensor and a remote receiver, and the robot is still controlled by the educational kit's hub, so they can be used (as explained by the Taiwan organizer).
- **Remote-control events**: The Controller can be used.
- **Not allowed**: programmable expansion units and independent power sources, so **EXP6 cannot be used**.

Official information: [MRC Taiwan](https://minoan-robotsports-competition-taiwan.github.io/)

### JBC (Junior Botball Challenge) Taiwan

Organized in Taiwan by SPARK. JBC has always been open to **any robot platform**: bring your own kit and follow the parts rules for your level. Robots run autonomously, so the Controller is not used.

Official information: [JBC Taiwan](https://junior-botball-challenge-taiwan.github.io/)

## Competitions Where MBC Sensors Cannot Be Used

| Competition | Reason |
| :--- | :--- |
| FIRST® LEGO® League Challenge | Official LEGO parts only |
| MIT STEAM Educational Robot Sports Competition (Taiwan) | Controllers must be designed in Taiwan; smart sensors with a built-in microcontroller are not allowed |
| Single-brand competitions | Competitions that require one brand's kit |

## Your Competition Isn't Listed?

Email [contactmbcrobot@gmail.com](mailto:contactmbcrobot@gmail.com) or message us on [LINE](https://line.me/R/ti/p/@692vcvuk) with the competition name and category, and our engineers will check the rules for you. We can also provide a laser eye-safety statement for the Ranger sensors.
