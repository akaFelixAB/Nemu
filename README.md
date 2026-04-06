# 🎮 Nemu

A modern, modular NES (Nintendo Entertainment System) emulator written in C++20.

## 📦 Project Structure

```
Nemu/
├── src/            # Main source code
│   └── main.cpp    # Example entry point
├── tests/          # Unit tests (Catch2 enabled by default)
│   ├── test_main.cpp
│   └── CMakeLists.txt
├── CMakeLists.txt  # Top-level CMake configuration
├── README.md       # Project documentation
└── LICENSE         # Apache 2.0 License
```

## 🚀 Features

- Modern C++20 codebase
- Clean, extensible architecture
- Unit testing with Catch2 (default, easily extendable)
- Focus on readability and best practices

## 🛠️ Requirements

- C++20 compatible compiler (recommended: g++ 10+)
- CMake >= 3.16
- [Optional] Ninja build system

## ⚡ Build Instructions

1. **Install dependencies:**
   - g++: `sudo apt install g++`
   - cmake: `sudo apt install cmake`
   - ninja (optional, recommended): `sudo apt install ninja-build`

2. **Generate build directory and compile:**
   ```sh
   mkdir build
   cd build
   cmake -G Ninja ..   # Use Ninja
   # or
   cmake ..            # Use default generator
   cmake --build .
   ```

3. **Run the main program:**
   ```sh
   ./Nemu
   ```

## 🧪 Unit Testing

- Catch2 is enabled by default and auto-integrated.
- To run tests after building:
  ```sh
  cd build
  ctest
  ```

## ⚙️ Configuration Highlights

- C++20 standard enforced via CMake
- Ninja build system supported
- Unit tests can be toggled with `NEMU_ENABLE_TESTS` (ON by default)
- Easily extendable to other test frameworks (e.g., Google Test)

## ✨ Best Practices

- Modern C++20 features
- Clean, modular project structure
- Readable, maintainable code

## 📄 License

Licensed under the [Apache License 2.0](LICENSE).

---

Feel free to customize or extend the project. For more details, refer to the CMake and Catch2 documentation.
