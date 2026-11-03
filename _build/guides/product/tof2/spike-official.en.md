### Which side is left

**Left and right are as seen from behind the sensor, looking the way it faces (left = J5, right = J3).** Facing the sensor, they are the other way round, so when you test with your hand, stand behind the sensor and look the same way it does. "Left is closer" and "turn left" in the examples follow this direction.

### Education and Robot Inventor read differently (tested 2026-11-03)

**SPIKE Prime (Education, SPIKE App 3)**: blocks and Python both pass on the real hub.

- "Color" = **left** distance in cm, "reflected light" = **right** distance in cm; in Python `color()` = left, `reflection()` = right. With nothing ahead both read **120**; the app does **not** clip them to 10 or 100.
- "Raw red / green / blue" **÷ 256** = left / right / average in cm.

**Robot Inventor blocks**: they do read the distances, but the conversion differs from Education.

| Block | What it reads | Convert |
| :--- | :--- | :--- |
| Raw color red / green / blue | left / right / average distance | **÷ 64** = cm (0~120); Education uses ÷ 256 |
| Reflected light | **left** distance in cm (0~120) | use as is; **the opposite of Education** (where `reflection()` = right) |
| Ambient light | left distance in cm | use as is, max 100 (120 = nothing ahead shows as 100) |
| Color | always −1 | cannot be used for distance |

**Robot Inventor Python**: `get_rgb_intensity()` = (left×256, right×256, average×256, 0), the same as the Education raw red / green / blue, so **÷ 256** (while Python runs the hub switches to the Education layout).

> [!NOTE]
> LEGO ended support for the Robot Inventor App on 2026-10-01; for new projects use SPIKE Prime or Pybricks.
