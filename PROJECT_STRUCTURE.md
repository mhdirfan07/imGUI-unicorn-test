# ImGUI Unicorn - Project Structure

Struktur proyek telah diorganisir dengan baik untuk memisahkan concerns (kepedulian) antara berbagai komponen. Berikut adalah penjelasan folder dan file:

## 📁 Struktur Folder

```
.
├── src/
│   ├── main.cpp                    # Entry point aplikasi
│   ├── core/                       # Logika bisnis & sistem
│   │   ├── system_monitor.h/.cpp   # Monitoring CPU & RAM
│   │   ├── database_manager.h/.cpp # Manajemen database SQLite
│   ├── ui/                         # Komponen User Interface
│   │   ├── camera_view.h/.cpp      # View untuk kamera & MOILDEV
│   │   ├── database_view.h/.cpp    # View untuk database logs
│   │   ├── widget_gallery_view.h/.cpp  # View untuk widget demo
│   └── utils/                      # Utilitas umum
│       └── graphics_utils.h/.cpp   # Fungsi konversi OpenCV → OpenGL
├── imgui/                          # Library ImGui (tidak diubah)
├── moil/                           # Header MOILDEV library
├── lib/                            # Binary libraries
├── CMakeLists.txt                  # Build configuration
└── main.cpp                        # File lama (bisa dihapus)
```

## 📚 Penjelasan Setiap Folder

### `src/main.cpp` - Entry Point

- Inisialisasi GLFW, OpenGL, dan ImGui
- Setup fonts dan style
- Main loop rendering
- Manajemen state aplikasi global

### `src/core/` - Logika Bisnis

- **system_monitor.cpp**: Fungsi membaca CPU & RAM usage dari `/proc/stat`
- **database_manager.cpp**: Kelas untuk manajemen database (CRUD operations)

### `src/ui/` - Komponen UI

- **camera_view.cpp**: Logika kamera, MOILDEV processing, dan rendering video
- **database_view.cpp**: Form input dan tabel database
- **widget_gallery_view.cpp**: Demo ImGui widgets (buttons, sliders, dll)

### `src/utils/` - Fungsi Utility

- **graphics_utils.cpp**: Konversi cv::Mat → GLuint texture

## 🔧 Cara Menggunakan Struktur Ini

### Menambah View Baru

1. Buat file `src/ui/my_view.h` dan `src/ui/my_view.cpp`
2. Define class dengan method `Render()` dan `Update()` (jika perlu)
3. Include di `src/main.cpp` dan tambahkan ke enum `AppView`
4. Update CMakeLists.txt: tambah file `.cpp` baru ke `add_executable`

### Menambah Logic Baru

1. Buat module di `src/core/` atau `src/utils/`
2. Buat `.h` untuk header, `.cpp` untuk implementasi
3. Include di `main.cpp` atau di view yang membutuhkan
4. Update CMakeLists.txt

## 🏗️ Arsitektur Pattern

Proyek ini menggunakan pattern **Model-View** sederhana:

- **Model/Core**: Database, System Monitoring (data & logika)
- **View**: UI Components (presentasi)
- **Main**: Orchestrator (mengkoordinasikan semua)

```
┌─────────────────┐
│   main.cpp      │ ← Orchestrator
├─────────────────┤
│  UI Views       │ ← Presentasi
├─────────────────┤
│  Core Logic     │ ← Data & Logika
└─────────────────┘
```

## 📦 Build & Run

```bash
cd /home/irfan/imGUI-unicorn-test
mkdir build && cd build
cmake ..
make
./unicorn
```

## ✅ Keuntungan Struktur Baru

✓ **Maintainability**: Kode lebih mudah dibaca dan dipahami  
✓ **Scalability**: Mudah menambah fitur atau module baru  
✓ **Reusability**: Fungsi dapat digunakan dari berbagai views  
✓ **Testability**: Setiap modul dapat ditest secara terpisah  
✓ **Collaboration**: Developer bisa bekerja di folder berbeda tanpa conflict

Selamat menggunakan struktur yang lebih baik! 🚀
