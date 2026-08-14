# About

Snap package of Nvidia's official GeForce NOW game streaming client for Linux, repacked from Nvidia's flatpak distribution.

## Disclaimer

This project and its contributors are not affiliated with Nvidia, nor it's GeForce NOW product. This snap repacks Nvidia's official GeForce NOW Linux client, which Nvidia otherwise only distributes as a flatpak, so it can be installed and updated through the Snap Store.

# Installation

[![Get it from the Snap Store](https://raw.githubusercontent.com/snapcore/snap-store-badges/master/EN/%5BEN%5D-snap-store-white.png)](https://snapcraft.io/geforcenow)

## Manual Installation

 - Go to the [latest release](https://github.com/kenvandine/geforcenow/releases/latest).
 - Download the specific file that best fits your disto.

# Usage

 - [Basic usage](https://github.com/kenvandine/geforcenow/wiki/Basic-usage)
   - [Keyboard shortcuts](https://github.com/kenvandine/geforcenow/wiki/Basic-usage#keyboard-shortcuts)
   - [Command-line arguments](https://github.com/kenvandine/geforcenow/wiki/Basic-usage#command-line-arguments)
   - [Changing the keyboard layout](https://github.com/kenvandine/geforcenow/wiki/Basic-usage#changing-the-keyboard-layout)
   - [Directly launching a game from the desktop](https://github.com/kenvandine/geforcenow/wiki/Basic-usage#directly-launching-a-game-from-the-desktop)
 - [Troubleshooting](https://github.com/kenvandine/geforcenow/wiki/Troubleshooting)
   - [Gamepad controls are not detected](https://github.com/kenvandine/geforcenow/wiki/Troubleshooting#gamepad-controls-are-not-detected)
   - [Steam Deck controls are not detected](https://github.com/kenvandine/geforcenow/wiki/Troubleshooting#steam-deck-controls-are-not-detected)

# Building from source

## Requirements

You will need [snapcraft](https://snapcraft.io/snapcraft) to build the snap. `snapcraft` pulls Nvidia's official GeForce NOW client directly from their flatpak distribution (an OSTree repository) at build time, so a build requires network access to `international.download.nvidia.com`.

## Building

```bash
git clone https://github.com/kenvandine/geforcenow.git
cd geforcenow
snapcraft
```

This always pulls whatever Nvidia currently has published on the `master` branch of their GeForce NOW flatpak repo, so re-running `snapcraft` picks up their latest release.

# Links
 - [GeForce NOW](https://nvidia.com/en-eu/geforce-now)
 - [Basic usage](https://github.com/kenvandine/geforcenow/wiki/Basic-usage)
 - [Troubleshooting](https://github.com/kenvandine/geforcenow/wiki/Troubleshooting)
