| Id | R/W | Name | Meaning |
| :--- | :---: | :--- | :--- |
| <span id="rf0">`F0`</span> | R | device ID | low byte = factory I2C address (unchanged after re-addressing), high byte = hw[0] (channels / sensors / variant) |
| <span id="rf1">`F1`</span> | R | protocol version | 2 for this release (same number reported in SPIKE mode 8 v[1])（2） |
| <span id="rf2">`F2`</span> | R | firmware version | major×100 + minor (e.g. 102 = 1.2) |
| <span id="rf3">`F3`</span> | R | status flags | b0 = ready; other bits per product (see flags)（bit） |
| <span id="rf4">`F4`</span> | R | update counter | +1 every 10 ms snapshot, wraps 0~255 (2.56 s per cycle, not usable for reboot detection; frozen = module busy)（0~255） |
| <span id="rf5">`F5`</span> | R | uptime seconds | seconds since boot, saturates at 32767 (~9 h); a drop means the module rebooted（0~32767） |
| <span id="rf8">`F8`</span> | W | change I2C address | [F8][new 0x08~0x77][~new], saved to EEPROM, takes effect after reboot; [F8][0x00][0xFF] = back to factory address；params：new address 0x08~0x77, ~new address |
| <span id="ref">`EF`</span> | W | reboot | [EF][0xA5]; mBlock sends ACK first, then reboots；params：magic 0xA5 |
