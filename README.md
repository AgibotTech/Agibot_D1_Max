# RobotSDK

Welcome to Robot SDK! It provides a comprehensive set of tools, libraries, and documentation designed to help you develop advanced applications for robots.

- **[中文文档](README_cn.md)**

## 🔗 SDK Version and System Platform Version Compatibility Table

| System Version | 0.0.2 | 0.0.3 | 0.0.4| 0.0.5| 0.0.6| 0.0.7
|--------------|-------|----|----|----|----|----|
| 0.0.5(V3)    | ✅    | ✅ | ❌ |❌  |❌  |❌  |
| 0.0.6(V3)    | ✅    | ✅ | ❌ |❌  |❌  |❌  |
| 0.0.7(V3)    | ✅    | ✅ | ❌ |❌  |❌  |❌  |
| 0.1.0-B(V4)  | ✅    | ✅ | ❌ |❌  |❌  |❌  |
| 0.1.0-C(V4)  | ❌    | ❌ | ✅ |✅  |✅  |✅  |
| 0.1.1-A(V4)  | ❌    | ❌ | ✅ |✅  |✅  |✅  |
| 0.1.3(V4)    | ❌    | ❌ | ✅ |✅  |✅  |✅  |
| 0.1.4(V4)    | ❌    | ❌ | ✅ |✅  |✅  |✅  |
| 0.1.5(V4)    | ❌    | ❌ | ✅ |✅  |✅  |✅  |
| 0.1.6(V4)    | ❌    | ❌ | ✅ |✅  |✅  |✅  |
| 0.1.7(V4)    | ❌    | ❌ | ✅ |✅  |✅  |✅  |


## 🌟 Features

This SDK provides high-level control interfaces that allow you to flexibly interact with robots.

## 🚀 Quick Start

Follow the steps below to set up your development environment and run the included demo programs.

### System Requirements

- **Operating System**: Ubuntu 22.04+
- **Build Tools**: CMake 3.8+, GCC 11+
- **Dependencies**: Boost 1.74+
- **Programming Language**: C++

### Installation and Setup

1. **Network Configuration**: Connect your computer to the robot's network. The default IP address of the robot is `192.168.234.1` (Wi-Fi).

3. **SDK Libraries**: C++ libraries (`.so` files) are included in the `lib/` directory and organized by architecture (`x86_64`, `aarch64`).

### Running Demo Programs

The `example/` directory contains C++ example code.

**C++ Demo:**

```bash
# Enter the C++ demo directory
cd example/

# Create build directory
mkdir build && cd build

# Configure and compile the project
cmake ..
make -j6

# Run the demo program
./data ${ip} 8081
```

## 📚 Documentation

For more in-depth information, please refer to the official documentation:

- **[API Reference](docs/en/sdk_client_api_en.md)**: Detailed descriptions of SDK functions.
- **[SDK Data Structures Documentation](docs/en/sdk_type_en.md)**: Detailed descriptions of SDK data structures.
- **[SDK State Transitions](docs/en/sdk_state_en.md)**: Detailed descriptions of SDK internal state transitions.

## 📁 Repository Structure

```
.
├── example/          # Demo applications (C++)
├── docs_user/        # Detailed documentation files
├── include/          # C++ header files for the SDK
└── lib/              # Pre-compiled library files (.so) for different architectures
```
