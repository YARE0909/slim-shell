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

echo "[*] Resetting local changes..."
git reset --hard HEAD
git clean -fd

echo "[*] Fetching latest..."
git fetch --all

echo "[*] Pulling latest changes..."
git reset --hard origin/main

echo "[*] Rebuilding..."
make -B   # forces rebuild, even if Make thinks it's up to date

echo ""
echo "--------------------------------------"
echo "   Slim Shell is now up to date!"
echo "--------------------------------------"
echo ""
