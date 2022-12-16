# About

Unofficial client for Nvidia's GeForce NOW game streaming service, providing a native Linux desktop experince and some additional features such as Discord rich presence.

## Disclaimer

This project and its contributors are not affiliated with Nvidia, nor it's GeForce NOW product. This repository does not contain any Nvidia / GeForce NOW software. It is simply an Electron wrapper that loads the official GFN web application page, just as it would in a regular web browser.

# Installation

[![Get it from the Snap Store](https://raw.githubusercontent.com/snapcore/snap-store-badges/master/EN/%5BEN%5D-snap-store-white.png)](https://snapcraft.io/geforcenow)

***Note**: The main version of this project, which receives the most support, is the flatpak version hosted on FlatHub!*

## Manual Installation

 - Go to the [latest release](https://github.com/kenvandine/geforcenow/releases/latest).
 - Download the specific file that best fits your disto.

***Note**: Manual installations are possible but not supported. Please use the flatpak version if you have any trouble with the manual installation!*

# Usage

 - [Basic usage](https://github.com/kenvandine/geforcenow/wiki/Basic-usage)
   - [Keyboard shortcuts](https://github.com/kenvandine/geforcenow/wiki/Basic-usage#keyboard-shortcuts)
   - [Command-line arguments](https://github.com/kenvandine/geforcenow/wiki/Basic-usage#command-line-arguments)
   - [Changing the keyboard layout](https://github.com/kenvandine/geforcenow/wiki/Basic-usage#changing-the-keyboard-layout)
   - [Directly launching a game from the desktop](https://github.com/kenvandine/geforcenow/wiki/Basic-usage#directly-launching-a-game-from-the-desktop)
 - [Integrations](https://github.com/kenvandine/geforcenow/wiki/Integrations)
   - [Discord](https://github.com/kenvandine/geforcenow/wiki/Integrations#discord)
     - [Using native GFN + flatpak Discord](https://github.com/kenvandine/geforcenow/wiki/Integrations#using-native-gfn--flatpak-discord)
     - [Disabling the Discord RPC](https://github.com/kenvandine/geforcenow/wiki/Integrations#disabling-the-discord-rpc)
 - [Troubleshooting](https://github.com/kenvandine/geforcenow/wiki/Troubleshooting)
   - [Gamepad controls are not detected](https://github.com/kenvandine/geforcenow/wiki/Troubleshooting#gamepad-controls-are-not-detected)
   - [Steam Deck controls are not detected](https://github.com/kenvandine/geforcenow/wiki/Troubleshooting#steam-deck-controls-are-not-detected)

# Building from source

## Requirements

You will need to install [npm](https://www.npmjs.com/), the Node.js package manager. On most distributions, the package is simply called `npm`.

## Cloning the source code

Once you have npm, clone the wrapper to a convenient location:

```bash
git clone https://github.com/kenvandine/geforcenow.git
```

## Building

```bash
npm install
npm start
```

On subsequent runs, `npm start` will be all that's required.

## Updating the source code

Simply pull the latest version of master and install any changed dependencies:

```bash
git checkout master
git pull
npm install
```

# Links
 - [GeForce NOW](https://nvidia.com/en-eu/geforce-now)
 - [Basic usage](https://github.com/kenvandine/geforcenow/wiki/Basic-usage)
 - [Troubleshooting](https://github.com/kenvandine/geforcenow/wiki/Troubleshooting)
