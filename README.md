# stock-ticker

## Prerequisites

- C++20 compiler (e.g., g++)
- CMake >= 3.10
- Qt6 (Core, Widgets, Network)
- GoogleTest and GoogleMock (system installed, e.g., libgtest-dev, libgmock-dev on Ubuntu)

## Build Instructions

```bash
mkdir -p build
cd build
cmake ..
cmake --build .
```

## Running Tests

```bash
cd build
ctest
# or run the test binary directly
./stock-ticker-tests
```
