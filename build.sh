#!/usr/bin/env bash

GREEN="\033[1;32m"
RED="\033[1;31m"
ILC="\033[3m"
ORG="\033[1;33m"
RST="\033[0m"
BOLD="\033[1m"

function _error()
{
    echo -e "${RED}${BOLD}[❌] ERROR:\n${RST}\t$1\n\t${ILC}\"$2\"${RST}"
    exit 84
}

function _success()
{
    echo -e "${GREEN}[✅] SUCCESS:\t${RST}${ILC}$1${RST}"
}

function _info()
{
    echo -e "${ORG}[🚧] RUNNING:\t${RST}${ILC}$1${RST}"
}

function _check_dep()
{
    local dep="$1"
    command -v "$dep" &>/dev/null || _error "Missing dependency" "$dep is not installed or not in PATH"
}

function _check_pkg()
{
    pkg-config --exists "$1" || _error "Missing package" "$1 (pkg-config could not find it)"
}

_info "Checking build dependencies..."

for bin in cmake ninja pkg-config glslangValidator; do
    _check_dep "$bin"
done

_check_pkg "vulkan"

_success "All required tools and libraries are available"

_info "Updating git submodules..."
git submodule update -j16 --init --recursive
_success "Git submodules updated successfully"

_info "Rebuilding shader cache..."
for item in $(find assets/shaders \( -name "*.vert" -o -name "*.frag" \)); do
    _info "Compiling shader: $item"
    glslangValidator -V "$item" -o "${item%.*}.spv" || _error "Shader compilation failed" "$item"
    _success "Shader compiled successfully: ${item%.*}.spv"
done
_success "Built shader cache"

BUILD_DIR="build"

_info "Creating build directory \"$BUILD_DIR\"..."
mkdir -p "$BUILD_DIR" || _error "Failed to create build directory" "$BUILD_DIR"

cd "$BUILD_DIR" || _error "Failed to enter build directory" "$BUILD_DIR"

_info "Configuring CMake project with Ninja generator..."
cmake -G Ninja .. || _error "CMake configuration failed" "Check CMakeLists.txt or missing dependencies"

_info "Building project with Ninja..."
ninja || _error "Build failed" "Fix compilation errors above ⬆"

_success "Build completed successfully"
