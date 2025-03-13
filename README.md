# 📂 SizeExplorer
**SizeExplorer** is a fast and multi-threaded C++ command-line tool for analyzing disk usage. It helps you find the **largest files and folders** in a given directory.

## 🚀 Features
- 🔍 **Find largest files or folders** in a directory.
- ⚡ **Multi-threaded** for fast folder size calculation.
- ✅ **Cross-platform**: Works on **Windows** and **Linux**.
- 🏗 **Makefile for Linux**, `build.bat` for Windows.
- 📄 **Simple CLI usage** with command-line arguments.

---

## 📦 Installation

### **Windows**
1. Install [MinGW-w64](https://www.mingw-w64.org/downloads/) if not installed.
2. Clone this repository:
   ```sh
   git clone https://github.com/yaabdulkadir/SizeExplorer.git
   cd SizeExplorer
   ```
3. Build the executable:
   ```sh
   build.bat
   ```

### **Linux**
1. Install `g++` if not installed:
   ```sh
   sudo apt update && sudo apt install g++
   ```
2. Clone the repository:
   ```sh
   git clone https://github.com/yaabdulkadir/SizeExplorer.git
   cd SizeExplorer
   ```
3. Build using `make`:
   ```sh
   make
   ```

---

## 🛠 Usage
After building, you can use `SizeExplorer` from the terminal.

### **Find the Largest Files in a Directory**
```sh
.\bin\SizeExplorer "C:\Users\YourName" 10 files   # Windows
./bin/SizeExplorer "/home/user/Documents" 10 files   # Linux
```

### **Find the Largest Folders in a Directory**
```sh
.\bin\SizeExplorer "C:\Users\YourName" 10 folders   # Windows
./bin/SizeExplorer "/home/user/Documents" 10 folders   # Linux
```

### **Display Help**
```sh
SizeExplorer --help
```

### **Check Version**
```sh
SizeExplorer --version
```

## 📜 License
This project is licensed under the **MIT License**. See the [LICENSE](LICENSE) file for details.

---
