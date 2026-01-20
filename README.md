# 🔍 Grep

**A high-performance implementation of the Unix `grep` utility, built from scratch with Modern C++ (C++20).**

This project is a deep dive into **System Programming**, focusing on efficient file I/O, recursion, and concurrency.

To ensure reliability, I integrated a **GitHub Actions CI/CD workflow** that automatically verifies compilation on Linux, macOS, and Windows using CMake and Ninja.

---

## 🚀 Features

* **⚡ Multi-Threaded Search:** Uses a "Chunking" strategy to split file lists across available CPU cores for maximum throughput.
* **📂 Recursive Search (`-r`):** Automatically dives into subdirectories using `std::filesystem`.
* **🔡 Case Insensitivity (`-i`):** Supports case-insensitive pattern matching.
* **📄 Multiple File Support:** Can accept specific lists of files or wildcard inputs.
* **📍 Context Awareness:** Prints filenames and line numbers for every match.
* **✅ Cross-Platform:** Verified builds on Linux, macOS, and Windows.

## 🛠️ Technical Implementation

### The "Chunking" Strategy
Unlike simple thread pools, this engine uses **Static Partitioning** to minimize synchronization overhead.
1.  **Discovery Phase:** The engine recursively scans directories to build a complete manifest of target files.
2.  **Partitioning:** The file list is mathematically divided based on `std::thread::hardware_concurrency()`.
3.  **Parallel Execution:** Each thread is assigned a dedicated "chunk" (start/end index) of the vector, allowing them to search completely independently without fighting for locks.

### Modern C++ (C++20)
The project leverages modern standards including:
* `<filesystem>` for robust path handling.
* `<thread>` & `<mutex>` for safe concurrency.
* **RAII** principles for resource management.

## 📦 Installation & Build

**Prerequisites:**
* **C++20 compliant compiler** (GCC 10+, Clang 10+, or MSVC 19.29+)
* **CMake** (3.10 or higher)
* **Ninja** build system

**Build Instructions:**

1.  **Clone the repository:**
    ```bash
    git clone https://github.com/Lovishg10/Grep.git
    cd Grep
    ```

2.  **Generate the project:**
    
    ```bash
    mkdir build && cd build
    cmake -G Ninja ..
    ```

3.  **Compile:**
    ```bash
    ninja
    ```

4.  **Run:**
    * *Linux / macOS:* `./mygrep "pattern" filename.txt`
    * *Windows:* `mygrep.exe "pattern" filename.txt`

## 💻 Usage

The syntax follows standard command-line conventions:

```bash
./mygrep [options] "pattern" <files...>
```

## ⚠️ A Note on Code Quality

**The "DRY" (Don't Repeat Yourself) principle was harmed in the making of this project.**

You may notice some code blocks that look suspiciously identical to others. For this project I subscribed to the philosophy of *"If it works, don't touch it,"* and honestly, `Ctrl+C` / `Ctrl+V` was just faster than writing helper functions. Refactoring is currently scheduled for the year 2030.

