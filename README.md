# M5PaperOS

I'm improving on the [m5stack/M5Paper_FactoryTest](https://github.com/m5stack/M5Paper_FactoryTest), fixing bugs and adding functionality.


## Improvements so far

- Moved Wi-Fi configuration into Settings app
- Added Wi-Fi indicator to status bar
- Added DejaVu-Sans in order to see utf-8 icons
- Extended Wi-Fi connection timeout to establish connection more reliably
- Preventing Wi-Fi from disconnecting
- Automatically syncing time after Wi-Fi connection was established
- Removed unused button from keyboard and enlarged number-alpha switch
- Increased back arrow size to prevent labels from beeing cut off
- Made some visual improvements to the UI


## Bugfixes so far

- Fixed bootup sequence to automatically connect to saved Wi-Fi network
- Added missing dependency to `platformio.ini`
- Fixed typos and misspellings
- Fixed label centering issues


## Other changes

- Removed poorly implemented language selection
- Renamed `WLAN` to `Wi-Fi`