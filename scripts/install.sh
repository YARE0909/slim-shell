#!/bin/sh
set -e

echo ""
echo "--------------------------------------"
echo "     Installing Slim Shell (slim)"
echo "--------------------------------------"
echo ""

INSTALL_DIR="$HOME/.slim-shell"

detect_os() {
    case "$(uname -s)" in
        Linux*)   OS="linux" ;;
        Darwin*)  OS="macos" ;;
        MINGW*|MSYS*|CYGWIN*) OS="windows" ;;
        *)        OS="unknown" ;;
    esac
}

install_deps_linux() {
    echo "[*] Installing dependencies for Linux..."

    if command -v pacman >/dev/null 2>&1; then
        sudo pacman -S --needed gcc make readline ncurses git
    elif command -v apt-get >/dev/null 2>&1; then
        sudo apt-get update
        sudo apt-get install -y build-essential libreadline-dev libncurses-dev git
    elif command -v dnf >/dev/null 2>&1; then
        sudo dnf install -y gcc make readline-devel ncurses-devel git
    else
        echo "[!] Unsupported Linux distro. Install manually:"
        echo "    gcc make readline-devel ncurses-devel git"
        exit 1
    fi
}

install_deps_macos() {
    echo "[*] Installing dependencies for macOS..."
    if ! command -v brew >/dev/null 2>&1; then
        echo "[!] Homebrew not installed!"
        echo 'Install it using:'
        echo '/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"'
        exit 1
    fi

    brew install readline git
}

install_deps_windows() {
    echo "[*] Installing dependencies for Windows (MSYS2)..."
    echo ""
    echo "Run this in MSYS2 MinGW64:"
    echo "    pacman -S --needed git make mingw-w64-x86_64-gcc mingw-w64-x86_64-readline"
    echo ""

    if [ -z "$MSYSTEM" ]; then
        echo "[!] This script must be run in MSYS2."
        exit 1
    fi
}

clone_repo() {
    echo "[*] Installing into: $INSTALL_DIR"

    rm -rf "$INSTALL_DIR"
    git clone https://github.com/YARE0909/slim-shell.git "$INSTALL_DIR"
    cd "$INSTALL_DIR"
}

build_slim() {
    echo "[*] Building Slim Shell..."
    make
}

install_binary_linux_macos() {
    echo "[*] Installing slim to /usr/local/bin ..."

    sudo cp slim /usr/local/bin/slim
    sudo chmod +x /usr/local/bin/slim
}

install_binary_windows() {
    echo "[*] Installing slim.exe..."

    mkdir -p "$HOME/bin"
    cp slim.exe "$HOME/bin/slim.exe"

    echo ""
    echo "If slim is not recognized, add this to PowerShell profile:"
    echo '    $env:Path += ";$HOME/bin"'
    echo ""
}

finish() {
    echo ""
    echo "--------------------------------------"
    echo "Slim Shell installed successfully!"
    echo ""
    echo "Run it with:"
    echo "    slim"
    echo ""
    echo "Install directory:"
    echo "    $INSTALL_DIR"
    echo "--------------------------------------"
}

#################################
# MAIN SCRIPT LOGIC
#################################

detect_os
echo "[*] Detected OS: $OS"

case "$OS" in
    linux)
        install_deps_linux
        clone_repo
        build_slim
        install_binary_linux_macos
        ;;
    macos)
        install_deps_macos
        clone_repo
        build_slim
        install_binary_linux_macos
        ;;
    windows)
        install_deps_windows
        clone_repo
        make
        install_binary_windows
        ;;
    *)
        echo "[!] Unsupported OS!"
        exit 1
        ;;
esac

finish
