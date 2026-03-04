# Moildev ImGui - Build & Run Guide

Panduan lengkap untuk build dan menjalankan aplikasi Moildev ImGui dengan struktur folder yang terorganisir.

## 🚀 Quick Start

### Menggunakan Script (Recommended)

```bash
# Navigate ke project directory
cd /home/irfan/imGUI-unicorn-test

# Option 1: Build dan run sekaligus
./build.sh build-run

# Option 2: Build saja
./build.sh build

# Option 3: Run existing build
./build.sh run

# Option 4: Clean rebuild
./build.sh rebuild

# Lihat semua options
./build.sh help
```

### Menggunakan CMake Langsung

```bash
cd /home/irfan/imGUI-unicorn-test
mkdir -p build && cd build

# Configure dengan CMake
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build
make -j4

# Run dengan cmake target
cmake --build . --target run
```

### Menggunakan Make (Simple)

```bash
cd /home/irfan/imGUI-unicorn-test/build
make -j4          # Build only
./unicorn         # Run application
```

## 📋 Available Commands

| Command                | Deskripsi                        |
| ---------------------- | -------------------------------- |
| `./build.sh build`     | Compile project                  |
| `./build.sh run`       | Run application (sudah compiled) |
| `./build.sh build-run` | Build + Run dalam satu command   |
| `./build.sh rebuild`   | Clean + Build dari scratch       |
| `./build.sh clean`     | Hapus folder build               |
| `./build.sh help`      | Tampilkan bantuan                |

## 🔧 CMake Targets

Setelah `cd build`:

```bash
# Build dan run
cmake --build . --target run

# Build, run, dan clean (rebuild)
cmake --build . --target rebuild

# Build dan run sekaligus
cmake --build . --target build-run
```

## 📁 Project Structure

```
.
├── src/
│   ├── main.cpp              # Entry point
│   ├── core/                 # Business logic
│   ├── ui/                   # UI components
│   └── utils/                # Utilities
├── CMakeLists.txt            # Build config
├── build.sh                  # Build script (recommended)
├── build/                    # Build output (auto-created)
└── imgui/                    # ImGui library
```

## ⚙️ Requirements

- CMake >= 3.10
- C++17 compiler (g++, clang)
- OpenCV 4.0+
- GLFW 3.3+
- OpenGL 3.3+
- OpenCL
- SQLite3
- MOILDEV library

## 🐛 Troubleshooting

### Build gagal?

```bash
# Clean dan rebuild
./build.sh rebuild

# Atau manual
rm -rf build
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make clean
make -j4
```

### Runtime error?

```bash
# Check dengan verbose output
cd build
./unicorn  # Lihat error messages
```

### Font tidak loading?

Aplikasi akan fallback ke system fonts. Jika ingin custom fonts, copy ke project root:

- `Roboto-Medium.ttf`
- `Roboto-Bold.ttf`

## 💡 Tips & Tricks

### Build lebih cepat dengan parallel jobs

```bash
cd build
make -j$(nproc)  # Gunakan semua CPU cores
```

### Incremental compilation

```bash
# Hanya compile file yang berubah
cd build
make -j4
```

### Generate compile_commands.json untuk IDE

```bash
cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
```

Ini akan generate `build/compile_commands.json` untuk IDE IntelliSense.

## 🎯 Development Workflow

```bash
# 1. Initial setup
./build.sh build

# 2. Edit code
# ... make changes ...

# 3. Rebuild dan test
cd build && make -j4 && ./unicorn

# 4. Full rebuild if needed
./build.sh rebuild
```

## 📊 Performance

- Build time (clean): ~30-45 seconds
- Build time (incremental): ~5-10 seconds
- Parallel compilation: Gunakan `make -j4` atau lebih

---

**Selamat menggunakan Moildev ImGui!** 🚀
