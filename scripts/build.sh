#!/bin/bash
set -e

# Automatic build script for Lumina Engine
echo "=== Starting compilation ==="

cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel

echo "=== Compilation completed. Executable in build/bin/ ==="