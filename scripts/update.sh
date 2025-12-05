set -e

INSTALL_DIR="$HOME/.slim-shell"

echo ""
echo "--------------------------------------"
echo "      Slim Shell — Updating"
echo "--------------------------------------"
echo ""

if [ ! -d "$INSTALL_DIR" ]; then
    echo "[!] Slim Shell is not installed in $INSTALL_DIR"
    exit 1
fi

cd "$INSTALL_DIR"

echo "[*] Pulling latest changes..."
git pull --rebase

echo "[*] Rebuilding..."
make

OS="$(uname -s)"

case "$OS" in
    Linux*|Darwin*)
        echo "[*] Installing updated binary..."
        sudo cp slim /usr/local/bin/slim
        sudo chmod +x /usr/local/bin/slim
        ;;
    MINGW*|MSYS*|CYGWIN*)
        echo "[*] Updating Windows binary..."
        mkdir -p "$HOME/bin"
        cp slim.exe "$HOME/bin/slim.exe"
        ;;
esac

echo ""
echo "--------------------------------------"
echo "   Slim Shell is now up to date!"
echo "--------------------------------------"
echo ""
