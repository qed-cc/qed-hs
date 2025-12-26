#!/bin/bash
# Build script for qed-hs (Hidden Service Network Layer)
# Copyright 2025 Rhett Creighton - Apache License 2.0

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build"

# Check for required tools
command -v autoreconf >/dev/null 2>&1 || { echo "autoreconf required but not found"; exit 1; }

cd "$SCRIPT_DIR"

# Generate configure script if needed
if [ ! -f configure ]; then
    echo "[qed-hs] Running autoreconf..."
    autoreconf -i
fi

# Create build directory
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Configure if needed
if [ ! -f Makefile ]; then
    echo "[qed-hs] Configuring..."
    ../configure --disable-asciidoc --disable-manpage --disable-html-manual
fi

# Build
echo "[qed-hs] Building..."
make -j$(nproc)

echo "[qed-hs] Build complete"
echo "  Binary: ${BUILD_DIR}/src/app/qed-hs/qed-hs"
echo "  Library: ${BUILD_DIR}/src/core/libqed_hs_core.a"
