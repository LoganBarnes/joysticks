# Joysticks

A simple C++ GUI app to detect and display joystick inputs on Windows, MacOS, & Linux.

## Building

From the repo root directory:

```bash
# make build directory and enter it.
mkdir build
cd build
# Configure the project and download packages
cmake -DCMAKE_BUILD_TYPE=Release ..
# Build the project
cmake --build . --config Release --target joysticks --parallel
# Run the app (.\Release\joystick.exe on Windows)
./joystick
```
