# Flatpak Guide

This guide covers how to build Lumina Engine as a Flatpak for Linux.

## Files

| File | Description |
|------|-------------|
| `flatpak/org.luminaengine.LuminaEngine.yml` | Flatpak manifest |
| `flatpak/org.luminaengine.LuminaEngine.desktop` | Desktop entry |
| `flatpak/org.luminaengine.LuminaEngine.metainfo.xml` | AppStream metadata |

## Building

### Prerequisites

```bash
# Install Flatpak and Flatpak Builder
sudo apt install flatpak flatpak-builder
# or on Arch
sudo pacman -S flatpak flatpak-builder
```

### Build

```bash
cd lumina-engine/flatpak
flatpak-builder --user --install --force-clean build org.luminaengine.LuminaEngine.yml
```

### Run

```bash
flatpak run org.luminaengine.LuminaEngine
```

### Create Distributable Package

```bash
flatpak build-bundle ~/.local/share/flatpak/repo lumina-engine.flatpak org.luminaengine.LuminaEngine
```

This creates `lumina-engine.flatpak` for distribution.

## Runtime

- **Runtime**: `org.freedesktop.Platform` 24.08
- **SDK**: `org.freedesktop.Sdk`
- **Extensions**: cmake, rust-stable

## Permissions

| Permission | Reason |
|-----------|--------|
| `--share=ipc` | IPC for Wayland/X11 |
| `--socket=x11` | X11 display |
| `--socket=wayland` | Wayland display |
| `--socket=pulseaudio` | Audio |
| `--device=dri` | Graphics |
| `--filesystem=home` | Game data |
