> [!TIP]
> **🧩 New to the Ranger 8-Way? Follow the step-by-step interactive tutorial:** [LEGO SPIKE App](/en/learn/laser8/spike-app.html). Pick options on the left and watch the blocks build up on the right; every function has an example, and a simulated sensor lets you try the values.

### Wait 2~4 seconds after power-on

The Ranger 8-Way needs no calibration; plug it in and go. **After power-on, wait 2~4 seconds** for the 8 lasers to get ready; meanwhile everything reads 120 (cm). Start your program with a short wait before reading distances.

| Channel | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |
| :--- | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| Facing | left | left-front 60° | left-front 30° | front | front | right-front 30° | right-front 60° | right |

The table assumes the sensor faces forward with the components up. Put your hand on the robot's left: example 1 should show a **negative** direction. If it shows a positive one, the sensor is mounted the other way, and "turn left" / "turn right" in the examples must be swapped too.

> [!WARNING]
> The official App readings are worked out from the sensor firmware and **have not yet been checked one by one on a real hub**. The home edition (Robot Inventor) uses a different format, so the official App examples on this page do not apply to it. If official-App example 1 always shows -7 or odd numbers wherever your hand is, switch to the MBC Blocks website or Pybricks and let us know.
