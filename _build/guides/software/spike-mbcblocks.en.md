### What you need

- **Sensor**: {{product}} ({{edition}}).
- **Hub**: SPIKE Prime or Robot Inventor.
- **Computer**: open the website in Chrome. Nothing to install.
- **Hub firmware**: must be switched to **Pybricks firmware** first (step 2 below, done from the website).

### Install the software

1. Open the [MBC SPIKE Blocks website](https://mbc-spike-blocks.mbcrobot.workers.dev) (blocks with a Chinese interface, nothing to install).
2. Switch the firmware: top menu "**SPIKE Hub → Install/Update Pybricks firmware**", follow the screen, about **3 minutes**. The website flashes Pybricks **v4.1.0**.
3. To go back to the official LEGO App later, switch back to the original LEGO firmware with the SPIKE App.

### Make the software see the sensor

No library needed: the MBC sensor blocks are built into the website, one block per value.

- To open an example: top menu "**File → Open**" (or press **Ctrl+O**) → pick the downloaded `.abp`.
- The Blocks website reads more data than the official App (no need to split numbers yourself), but example 6, marked "Pybricks only", uses a new mode that the Blocks website cannot read either.

### Wiring

1. Plug the sensor into hub **port {{port}}** with the cable (any of A~F works; choose the same port in the blocks).
2. Latch up, push the plug in gently.
3. When the hub is on, the light on the sensor comes on.

### First test

1. Connect the website to the hub (the hub must be running Pybricks firmware).
2. Open the `.abp` of example 1 on this page and press Run.
3. If the number changes with what is in front of the sensor, everything is connected.

### FAQ

- **The website can't connect to the hub?** The hub still has the original LEGO firmware: do step 2 of "Install the software" to switch to Pybricks firmware.
- **The official App can't connect any more?** The hub now runs Pybricks firmware: switch back to the original LEGO firmware with the SPIKE App.
- **Nothing happens when I plug it in?** Check in order: cable firmly in → another port → another cable → another hub.
