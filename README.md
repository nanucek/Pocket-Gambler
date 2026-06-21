# Pocket Gambler OS – Release Notes (v0.2.1)

Pocket Gambler OS v0.2.1 transitions the platform from a simple minigame collection into a full-fledged, handheld operating system featuring an integrated RPG progression ecosystem, live Wi-Fi data streaming, and advanced wealth analytics.

**A quick note of apology:** We are very sorry, but unfortunately, previous versions of this software may no longer be available or accessible. We appreciate your understanding and support as we continue to push the project forward with this latest build!
<img width="1024" height="1024" alt="Pocket Gambler logo" src="https://github.com/user-attachments/assets/93959f5e-1e2c-4e8b-a174-9a7b2cfc36ee" />

## 🚀 New Features & Applications

* **Live Crypto Exchange (Binance Terminal):** Integrated a dynamic Wi-Fi Scanner and text prompter for on-device password entry. The OS can now actively scan surrounding airwaves, connect securely to local Wi-Fi, and stream live Bitcoin prices directly from the Binance API. Players can speculate on market movements, buy simulated $100 fiat blocks, and track their net Profit/Loss (P/L) in real-time.
* **Player Card & RPG Leveling:** Implemented a system-wide progression mechanic. Players earn XP for every single dollar wagered or lost across the ecosystem. The OS automatically calculates Level Ups, increments base stats, and saves progress permanently.
* **Dynamic Theme System:** Directly accessible from the Player Card by pressing the `t` key. Switch the entire visual design of the OS on the fly. Features 3 custom color profiles:
* *Neon Cyberpunk* (The classic cyan-magenta retro aesthetic)
* *Matrix Hacker* (A clean, pure-green command-line terminal layout)
* *Blood Casino* (A deep crimson profile accented with premium high-roller gold tones)


* **Wealth Analytics Charting:** Implemented a real-time line graph renderer within the Player Card. The OS caches the last 50 financial adjustments, dynamically maps the boundaries, and plots a smooth vector graph tracking the history of your wealth over time.
* **Hardcore Vault Heist (Mafia Bank):** A massive overhaul of the banking application. Alongside standard loan shark options (borrowing $1000 at a 10% interest rate), players can now engage in a terminal hacking minigame inspired by *Fallout*.
* *The Logic:* Players must crack the safe by deciphering a 6-letter cipher chosen randomly from a pool of 12 tactical tech words.
* *High Stakes:* You have **only 3 attempts** to log the exact matches based on character positions. Success yields a flat $5000 payload; failure trips the silent alarms, resulting in an immediate 50% confiscation of your entire balance.



## 🛠️ Bug Fixes & UI Optimizations

* **Input Calibration Fix (Lowercase Key Matching):** Calibrated all gaming and system inputs to intercept lowercase characters (`t`, `l`, `p`, `h`, `b`, `s`, `u`, `c`). This resolves a hardware quirk on the M5Cardputer where the keyboard matrix defaults to lowercase keycodes, which previously caused the OS to ignore inputs.
* **Scrolling Menu & Selection Frame Overhaul:** Fixed a critical bug where the highlight selection frame drifted off the screen when browsing through long lists. The algorithm now maps relative positions based on the visible screen window (rows 0-3), ensuring buttery-smooth scrolling navigation across all 15 index items.
* **Header Geometry Corrections:** Shortened the OS title text to "OS" to reclaim screen real estate. The battery level indicator has been centered inward, completely resolving visual overlap bugs with the cash balance counter in the upper-right corner.
* **Atomic Save System (Anti-Loss Protection):** Expanded the `save.txt` structure on the SD card to securely serialize the 50-point history array, active debt structures, and crypto wallet sizes. Saving triggers atomically upon every transaction to safeguard user progress against abrupt power cuts.

## 🏗️ Under the Hood Changes (For Developers)

* **Linker Collision Bypass (Monolithic App Header Design):** Due to PlatformIO compiler instability and recurrent linker errors (`undefined reference to vtable`), the architectural pattern for the custom apps directory (`src/apps/`) was refactored into inline header structures. This removes object compilation linking phases, packing the system directly into `main.cpp` cleanly.
* **Memory and Scope Safety:** Eliminated universal macros like `RED` or `NEON_GREEN` inside game headers to prevent cross-contamination. Critical game layouts now communicate via raw Hex statements or through the global `UI::C_PRIM` theme variables.
* **Battery Conservation Logic:** The Wi-Fi radio module activates exclusively after entering the Binance Terminal and committing an Enter sequence. Pressing Backspace safely terminates all network sockets and turns off the radio, preventing battery drain while enjoying localized games.

---

*Developer Note: Due to structural layout changes in the data serialization of `save.txt`, booting v0.2.1 for the first time will automatically reinitialize old save data back to the base $1000 threshold.*

---

### ⚠️ LEGAL DISCLAIMER

> **This software is entirely a fictional, educational game simulation.** The "Live Crypto Exchange" and "Mafia Bank" modules connect to a public API for real-time market data purely for entertainment and simulation purposes. **No real money, real cryptocurrency, or real financial transactions are executed, stored, or processed by this software.** The developer assumes **zero responsibility or liability** for any financial losses, hardware damage, network security issues, connectivity failures, or data corruption that may occur while using this software. Use at your own risc.
