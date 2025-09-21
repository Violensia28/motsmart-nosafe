
# MOTSMART NOSAFE Pack (v0.1-ns)

Adds a **NOSAFE** profile to your MOTSMART firmware with **notify-only** safety and CI artifacts.

### Key Points
- **Non-blocking safety**: SD/sensor failures only produce notifications; **welding is never blocked**.
- **Sensors aggregator** included for **DS18B20 (1-Wire)**, **PC817 (optocoupler input)**, and **ACS712** (current sensor).
- **Notification period**: 5 seconds (configurable).
- **GitHub Actions**: builds `nosafe-minimal` and `nosafe-full`, uploads `.bin` artifacts and logs; creates release on tag.
- **Termux guide**: build & (options to) flash from Android (see `HOWTO_Termux.md`).

See `HOWTO_NOSAFE.md` for quick integration.
