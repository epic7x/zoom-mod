# Zoom

`Zoom` is a Geode mod for Geometry Dash that lets you move and inspect the level while the game is paused.

This fork is maintained by `epic7x` and targets modern Geode / Geometry Dash 2.2 builds.

**It is based on the now-obsolete `Zoooom!` mod by [BobbyShmurner](https://github.com/BobbyShmurner), originally published here: [Zoooom!](https://github.com/BobbyShmurner/Zoom).**

This project was also developed with light AI assistance during parts of the refactoring and maintenance process.

The mod is open source, and the source code is available in this repository for review. It does not include malicious functionality, hidden network behavior, or intentionally harmful code.

## What It Does

When you pause the game, the mod allows you to:

- zoom in and out of the current scene
- move the camera around while paused
- hide or show the pause menu while zoomed in
- use touch gestures on mobile to inspect the level more comfortably

The goal is simple: make paused-level inspection feel smooth and practical on both desktop and mobile.

## Compatibility

Current target versions:

- Geode `5.5.3`
- Geometry Dash `2.2081`
- Platforms: Windows, Android, macOS, and iOS

Desktop support is the most polished path right now. Apple platforms are supported, but may still need more real-device testing.

## Controls

### Desktop

- `Mouse Wheel`: zoom in / zoom out
- `Middle Mouse Button`: pan the camera while paused
- `Home`: toggle the pause menu while zoomed

The keybind can be changed from the mod settings in Geode.

### Mobile

- Open the pause menu
- Tap the magnifying glass button
- Use pinch gestures to zoom
- Drag with one finger to move the camera
- Use the back button in the top-left corner to exit zoom mode

## Settings

The mod includes the following configurable options:

- `Zoom Rate`: changes how fast zooming reacts on desktop
- `Hide Menu Automatically`: hides the pause menu when zooming in
- `Show Menu Automatically`: shows the pause menu again when zooming back out
- `Alt Key Stops Zoom`: disables zoom input while `Alt` is held
- `Toggle Pause Screen`: desktop keybind for showing or hiding the pause menu

## Project Structure

Important files and folders:

- `mod.json`: mod metadata, settings, versions, and links
- `src/`: source code
- `resources/`: UI sprites used by the mod
- `CMakeLists.txt`: build configuration
- `build.ps1`: helper script for local builds

Source layout:

- `src/zoom_shared.cpp`: shared camera / zoom behavior
- `src/desktop.cpp`: desktop hooks and input flow
- `src/mobile.cpp`: mobile zoom layer and gesture logic
- `src/settings.cpp`: runtime settings sync

## Building

To build the project locally, make sure you have:

- Geode CLI
- CMake
- Visual Studio Build Tools / MSVC on Windows
- a valid Geode SDK path

Example PowerShell session from the project root:

```powershell
$env:PATH="C:\Program Files\CMake\bin;$env:PATH"
$env:GEODE_SDK="C:\Users\agust\Documents\Zoom-main\.geode-sdk"
geode build -p win
```

If `geode` is not available in your `PATH`, run the CLI using its full installation path.

## Notes

- This repository focuses on the maintained `epic7x` version of the mod.
- Build artifacts and local SDK files are intentionally ignored in version control.
- If you change the mod ID, Geode will treat it as a different mod installation.

## License / Attribution

If you plan to redistribute or publish changes, make sure the project keeps whatever attribution and licensing obligations apply to the code and assets you are using.
