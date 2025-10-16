# Connect-Four

A C++ implementation of Connect Four with two playable targets:

- **`terminal.cpp`** — terminal/console-based Connect Four (local two-player or vs simple AI).
- **`main.cpp`** — GUI version (player vs computer) using SFML.

This repo is a small project showcase demonstrating game logic, a basic AI, and a lightweight SFML-based GUI.

---

## Contents (high level)

- `terminal.cpp` — console/terminal gameplay entry point.
- `main.cpp` — GUI entry point (requires SFML).
- `CMakeLists.txt` — build configuration.
- Other `.cpp`/`.h` files — game engine, board logic, AI, helpers.
- Assets (if present) — images/sounds used by GUI.

---

## Requirements

- C++ compiler with C++17 support (`g++`, `clang++`, or MSVC).
- CMake (optional, recommended).
- **SFML** (only required if you want the GUI target).

## Build with CMake (recommended for GUI)

Create a build directory and configure:

```bash
# Clone the repo
git clone https://github.com/Jimmyjohn23/Connect-Four.git
cd Connect-Four

# Create and enter a build directory
rm -rf build
mkdir build
cd build
cmake ..
cmake --build . -j$(nproc)

# === Run the game ===
./connect4
