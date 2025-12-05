# Slim Shell

Slim Shell is a lightweight, minimal, personal shell built for fun.
It uses modern slang, custom colors, a stylized banner, and simple built-in commands.
The goal of this project is not to replace a full Unix shell, but to experiment, learn, and have a good time building something from scratch.

Slim Shell supports Linux, macOS, and Windows (via MSYS2).
It includes a custom prompt, command history, builtins, external command execution, and an update mechanism that lets users refresh the shell from inside the shell itself.

---

## Features

* Custom colored prompt with time and working directory
* ASCII art banner at startup
* Built-in commands (`cd`, `exit`, `update`, and more)
* External command execution via `execvp`
* Readline support for history and editing
* Auto-update system: run `update` inside the shell to pull and rebuild the latest version
* Simple, clean codebase organized into multiple modules

---

## Installation

To install Slim Shell, run the following command:

```
curl -fsSL https://raw.githubusercontent.com/YARE0909/slim-shell/main/scripts/install.sh | bash
```

The installer will:

* Detect your operating system
* Install necessary dependencies
* Clone the Slim Shell repository into `~/.slim-shell`
* Build the shell
* Install the binary as `slim`

Once installed, run:

```
slim
```

---

## Updating

Slim Shell includes its own update command.

Inside the shell, run:

```
update
```

This will:

* Pull the latest code from GitHub
* Rebuild the project
* Restart the shell from the newly built binary

---

## Uninstallation

To remove Slim Shell:

```
rm -rf ~/.slim-shell
sudo rm -f /usr/local/bin/slim
```