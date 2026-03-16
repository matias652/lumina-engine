# Flatpak Guide

This guide covers how to build and distribute Lumina Engine as a Flatpak for Linux.

## Overview

Flatpak is a next-generation Linux sandboxed packaging format. Lumina Engine includes complete Flatpak configuration for easy distribution.

## Files

The Flatpak configuration consists of:

| File | Description |
|------|-------------|
| `flatpak/org.luminaengine.LuminaEngine.yml` | Main Flatpak manifest |
| `flatpak/org.luminaengine.LuminaEngine.desktop` | Desktop entry file |
| `flatpak/org.luminaengine.LuminaEngine.metainfo.xml` | AppStream metadata |

## Building Locally

### Prerequisites

```bash
# Install Flatpak and Flatpak Builder
sudo apt install flatpak flatpak-builder
# or on Arch
sudo pacman -S flatpak flatpak-builder

# Add Flathub repository (if not already added)
flatpak remote-add --user --if-not-exists flathub https://flathub.org/repo/flathub.flatpakrepo
```

### Build the Flatpak

```bash
cd lumina-engine/flatpak

# Build the Flatpak
flatpak-builder --user --install build org.luminaengine.LuminaEngine.yml

# Or build without installing
flatpak-builder build org.luminaengine.LuminaEngine.yml
```

### Running

```bash
flatpak run org.luminaengine.LuminaEngine
```

### Troubleshooting Build

If you encounter build issues:

```bash
# Clean the build directory
rm -rf build

# Try again with verbose output
flatpak-builder -v --force-clean build org.luminaengine.LuminaEngine.yml
```

## Submitting to Flathub

To publish on Flathub:

### 1. Fork Flathub

Fork the [Flathub repository](https://github.com/flathub/flathub) on GitHub.

### 2. Create Your Branch

```bash
git checkout -b lumina-engine
```

### 3. Add the Application

Copy the manifest to your fork:

```bash
mkdir -p games/org.luminaengine.LuminaEngine
cp flatpak/org.luminaengine.LuminaEngine.yml games/org.luminaengine.LuminaEngine/
cp flatpak/org.luminaengine.LuminaEngine.desktop games/org.luminaengine.LuminaEngine/
cp flatpak/org.luminaengine.LuminaEngine.metainfo.xml games/org.luminaengine.LuminaEngine/
```

### 4. Build Verification

Test your manifest locally before submitting:

```bash
flatpak-builder --user --install --force-clean build games/org.luminaengine.LuminaEngine/org.luminaengine.LuminaEngine.yml
```

### 5. Submit Pull Request

Create a pull request to the Flathub repository with:
- The application files in a new directory
- A brief description of your application

## Runtime Requirements

The Flatpak uses:

- **Runtime**: `org.freedesktop.Platform` (version 24.08)
- **SDK**: `org.freedesktop.Sdk`
- **Extensions**:
  - `org.freedesktop.Sdk.Extension.cmake`
  - `org.freedesktop.Sdk.Extension.rust-stable`

## Permissions

The following permissions are requested:

| Permission | Reason |
|-----------|--------|
| `--share=ipc` | IPC for Wayland/X11 |
| `--socket=x11` | X11 display server |
| `--socket=wayland` | Wayland display server |
| `--socket=pulseaudio` | Audio output |
| `--device=dri` | Hardware graphics acceleration |
| `--filesystem=home` | Access game data |
| `--env=SDL_VIDEODRIVER=wayland` | Use Wayland for SDL |

## CI/CD

For automated builds, create a GitHub Actions workflow:

```yaml
name: Flatpak

on:
  release:
    types: [published]

jobs:
  flatpak:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
      
      - name: Build Flatpak
        uses: flatpak/flatpak-github-actions/flatpak-builder@v6
        with:
          bundle: lumina-engine.flatpak
          manifest-path: flatpak/org.luminaengine.LuminaEngine.yml
          cache-key: "{{ github.sha }}"
      
      - name: Upload artifact
        uses: actions/upload-artifact@v4
        with:
          name: lumina-engine.flatpak
          path: lumina-engine.flatpak
```

## Troubleshooting

### SDL3 Not Found

The manifest builds SDL3 from source since the runtime may not include SDL3. If build fails:

1. Check the SDL3 tag is valid
2. Ensure network access for fetching sources

### Missing Dependencies

If runtime dependencies are missing, add them to the manifest:

```yaml
- name: some-package
  buildsystem: simple
  build-commands:
    - install -Dm755 some-bin /app/bin/some-bin
  sources:
    - type: archive
      url: https://example.com/some-bin.tar.gz
      sha256: ...
```

### Icon Not Showing

Ensure the icon is properly configured:
1. PNG icon at 64x64, 128x128, 256x256
2. Desktop file references correct icon name
3. Metainfo has launchable type set
