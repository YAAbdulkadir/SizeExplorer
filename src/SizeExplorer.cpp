#include <iostream>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cstdlib>
#include <unordered_map>
#include <future>
#include <mutex>

namespace fs = std::filesystem;

const std::string VERSION = "SizeExplorer v1.1.0";
std::mutex folderMutex; // Mutex for thread-safe folder size storage

// Struct to store file/folder size information
struct EntryInfo {
    std::string path;
    uintmax_t size;

    bool operator<(const EntryInfo& other) const {
        return size > other.size; // Sort descending
    }
};

// Function to display help message
void showHelp(const std::string& programName) {
    std::cout << "SizeExplorer - Analyze Disk Usage\n"
              << "Usage: " << programName << " <directory_path> [topN] [files|folders]\n"
              << "Options:\n"
              << "  -h, --help                            Show this help message\n"
              << "  --version                             Show version information\n\n"
              << "Arguments:\n"
              << "  <directory_path>                      The directory to scan\n"
              << "  [topN]                                Number of results to display (default: 10)\n"
              << "  [files|folders]                       Show largest files or folders (default: files)\n"
              << "\nExamples:\n"
              << "  " << programName << " C:\\Users\\YourName 5 files\n"
              << "  " << programName << " C:\\Users\\YourName 5 folders\n";
}

// Function to calculate folder size recursively
uintmax_t calculateFolderSize(const fs::path& folder) {
    uintmax_t totalSize = 0;
    try {
        for (const auto& entry : fs::recursive_directory_iterator(folder, fs::directory_options::skip_permission_denied)) {
            if (entry.is_regular_file()) {
                totalSize += entry.file_size();
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error calculating folder size for " << folder << ": " << e.what() << '\n';
    }
    return totalSize;
}

// Function to find the largest files
std::vector<EntryInfo> findLargestFiles(const fs::path& directory, size_t topN) {
    std::vector<EntryInfo> files;

    try {
        for (const auto& entry : fs::recursive_directory_iterator(directory, fs::directory_options::skip_permission_denied)) {
            if (entry.is_regular_file()) {
                files.push_back({ entry.path().string(), entry.file_size() });
            }
        }

        std::sort(files.begin(), files.end());
        if (files.size() > topN) {
            files.resize(topN);
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
    }
    return files;
}

// Function to find the largest folders (multi-threaded)
std::vector<EntryInfo> findLargestFolders(const fs::path& directory, size_t topN) {
    std::unordered_map<std::string, uintmax_t> folderSizes;
    std::vector<std::future<void>> futures;

    try {
        for (const auto& entry : fs::directory_iterator(directory, fs::directory_options::skip_permission_denied)) {
            if (entry.is_directory()) {
                // Launch a thread to calculate folder size asynchronously
                futures.push_back(std::async(std::launch::async, [&folderSizes, entry]() {
                    uintmax_t size = calculateFolderSize(entry.path());
                    std::lock_guard<std::mutex> lock(folderMutex);
                    folderSizes[entry.path().string()] = size;
                }));
            }
        }

        // Wait for all threads to complete
        for (auto& f : futures) {
            f.get();
        }

        // Convert map to vector
        std::vector<EntryInfo> folders;
        for (const auto& [path, size] : folderSizes) {
            folders.push_back({ path, size });
        }

        std::sort(folders.begin(), folders.end());
        if (folders.size() > topN) {
            folders.resize(topN);
        }
        return folders;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
    }
    return {};
}

int main(int argc, char* argv[]) {
    // Check if help or version flag is provided
    if (argc == 2) {
        std::string arg = argv[1];
        if (arg == "-h" || arg == "--help") {
            showHelp(argv[0]);
            return 0;
        }
        if (arg == "--version") {
            std::cout << "Largest Files/Folders Finder v" << VERSION << "\n";
            return 0;
        }
    }
    if (argc < 2 || argc > 4) {
        std::cerr << "Invalid arguments! Use -h or --help for usage instructions.\n";
        return 1;
    }

    fs::path directory = argv[1];

    if (!fs::exists(directory) || !fs::is_directory(directory)) {
        std::cerr << "Error: Invalid directory!\n";
        return 1;
    }

    size_t topN = 10;
    if (argc >= 3) {
        topN = std::atoi(argv[2]);
        if (topN <= 0) {
            std::cerr << "Error: topN must be a positive integer!\n";
            return 1;
        }
    }

    std::string mode = "files"; // Default mode
    if (argc == 4) {
        mode = argv[3];
        if (mode != "files" && mode != "folders") {
            std::cerr << "Error: Mode must be 'files' or 'folders'!\n";
            return 1;
        }
    }

    std::vector<EntryInfo> results;

    if (mode == "files") {
        results = findLargestFiles(directory, topN);
        std::cout << "\nTop " << topN << " Largest Files in " << directory << ":\n";
    }
    else {
        results = findLargestFolders(directory, topN);
        std::cout << "\nTop " << topN << " Largest Folders in " << directory << ":\n";
    }

    std::cout << std::setw(10) << "Size (MB)" << " | Path\n";
    std::cout << "---------------------------------------\n";
    for (const auto& entry : results) {
        std::cout << std::setw(10) << entry.size / (1024 * 1024) << " MB | " << entry.path << '\n';
    }

    return 0;
    
}