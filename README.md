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
