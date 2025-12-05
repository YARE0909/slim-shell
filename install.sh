set -e

echo ""
echo "--------------------------------------"
echo "     Installing Slim Shell (slim)"
echo "--------------------------------------"
echo ""

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

    # Detect package manager
    if command -v pacman >/dev/null 2>&1; then
        sudo pacman -S --needed gcc make readline ncurses git
    elif command -v apt-get >/dev/null 2>&1; then
        sudo apt-get update
        sudo apt-get install -y build-essential libreadline-dev libncurses-dev git
    elif command -v dnf >/dev/null 2>&1; then
        sudo dnf install -y gcc make readline-devel ncurses-devel git
    else
        echo "[!] Unsupported Linux distro. Install dependencies manually:"
        echo "    gcc make readline-devel ncurses-devel git"
        exit 1
    fi
}

install_deps_macos() {
    echo "[*] Installing dependencies for macOS..."
    if ! command -v brew >/dev/null 2>&1; then
        echo "[!] Homebrew not installed. Install it first:"
        echo '    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"'
        exit 1
    fi

    brew install readline git
}

install_deps_windows() {
    echo "[*] Installing dependencies for Windows (MSYS2)..."
    echo "    You must install MSYS2 from:"
    echo "    https://www.msys2.org/"
    echo ""
    echo "Then run this command from MSYS2 MinGW64 terminal:"
    echo ""
    echo "    pacman -S --needed git make mingw-w64-x86_64-gcc mingw-w64-x86_64-readline"
    echo ""

    if [[ -z "$MSYSTEM" ]]; then
        echo "[!] This script must be run inside MSYS2 terminal."
        exit 1
    fi
}

clone_repo() {
    echo "[*] Cloning Slim Shell repo..."
    if [[ -d "slim-shell" ]]; then
        rm -rf slim-shell
    fi
    git clone https://github.com/YARE0909/slim-shell.git
    cd slim-shell
}

build_slim() {
    echo "[*] Building slim..."
    make
}

install_binary_linux_macos() {
    echo "[*] Installing to /usr/local/bin/slim ..."
    sudo cp slim /usr/local/bin/slim
    sudo chmod +x /usr/local/bin/slim
}

install_binary_windows() {
    echo "[*] Copying slim.exe to a location in PATH..."

    mkdir -p "$HOME/bin"
    cp slim.exe "$HOME/bin/slim.exe"

    echo "Add this to your PowerShell profile (if needed):"
    echo '    $env:Path += ";$HOME/bin"'
}

finish() {
    echo ""
    echo "--------------------------------------"
    echo " Slim shell installed successfully! 🎉"
    echo " Run it using:"
    echo ""
    echo "      slim"
    echo ""
    echo "--------------------------------------"
}

#############################
# MAIN SCRIPT LOGIC
#############################

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
        echo "[*] Building slim.exe..."
        make
        install_binary_windows
        ;;
    *)
        echo "[!] Unknown or unsupported OS"
        exit 1
        ;;
esac

finish
