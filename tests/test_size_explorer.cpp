#include <iostream>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <cassert>

namespace fs = std::filesystem;

// Function to create test files
void createTestFiles(const std::string& dir) {
    fs::create_directories(dir + "/subfolder");

    // Create a 5 MB file
    std::ofstream file1(dir + "/file1.txt", std::ios::binary);
    file1.seekp(5 * 1024 * 1024 - 1 );
    file1.put(0);
    file1.close();

    // Create a 10 MB file
    std::ofstream file2(dir + "/file2.txt", std::ios::binary);
    file2.seekp(10 * 1024 * 1024 - 1);
    file2.put(0);
    file2.close();

    // Create a 15 MB file inside subfolder
    std::ofstream file3(dir + "/subfolder/file3.txt", std::ios::binary);
    file3.seekp(15 * 1024 * 1024 - 1);
    file3.put(0);
    file3.close();
}

void testExecution() {
    std::cout << "Running: Basic execution test...\n";
    int result = std::system("./bin/SizeExplorer ./tests/test_data 3 files > /dev/null 2>&1");
    assert(result == 0);
    std::cout << "✅ Passed: Basic execution test\n";
}


void testFindingLargeFiles() {
    std::cout << "Running: Find largest files test...\n";
    std::string output = "output.txt";
    std::string command = "./bin/SizeExplorer ./tests/test_data 3 files > " + output;
    std::system(command.c_str());

    std::ifstream inFile(output);
    std::string line;
    bool foundLargeFile = false;

    while (std::getline(inFile, line)) {
        if (line.find("file3.txt") != std::string::npos) {
            foundLargeFile = true;
            break;
        }
    }

    assert(foundLargeFile);
    std::cout << "✅ Passed: Find largest files test\n";
}

void testInvalidDirectory() {
    std::cout << "Running: Invalid directory test...\n";
    int result = std::system("./bin/SizeExplorer ./invalid_directory 3 files > /dev/null 2>&1");
    assert(result != 0);  // Program should return an error code
    std::cout << "✅ Passed: Invalid directory test\n";
}

int main() {
    std::cout << "🏃 Running SizeExplorer Tests...\n";

    std::string testDir = "./tests/test_data";
    
    // Create test files if they don't exist
    if (!fs::exists(testDir)) {
        createTestFiles(testDir);
    }

    testExecution();
    testFindingLargeFiles();
    testInvalidDirectory();

    std::cout << "\n🎉 All tests passed!\n";
    return 0;
}