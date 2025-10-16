#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <mutex>

std::mutex mtx; // Mutex for synchronizing access to shared data

// Simple Run-Length Encoding (RLE) compression function
std::string compress_chunk(const std::string& chunk) {
    if (chunk.empty()) {
        return "";
    }
    std::string compressed;
    int count = 1;
    char current_char = chunk[0];
    for (size_t i = 1; i < chunk.length(); ++i) {
        if (chunk[i] == current_char && count < 255) {
            count++;
        } else {
            compressed += static_cast<char>(count);
            compressed += current_char;
            current_char = chunk[i];
            count = 1;
        }
    }
    compressed += static_cast<char>(count);
    compressed += current_char;
    return compressed;
}

// Simple RLE decompression function
std::string decompress(const std::string& data) {
    std::string decompressed;
    for (size_t i = 0; i < data.length(); i += 2) {
        int count = static_cast<unsigned char>(data[i]);
        char character = data[i + 1];
        decompressed.append(count, character);
    }
    return decompressed;
}

// Function executed by each thread to compress its chunk
void compress_worker(const std::string& chunk, std::string& result) {
    result = compress_chunk(chunk);
}

// Main function to perform multithreaded compression
void compress_file(const std::string& input_filename, const std::string& output_filename) {
    std::ifstream inFile(input_filename, std::ios::binary);
    if (!inFile) {
        std::cerr << "Error opening input file." << std::endl;
        return;
    }

    // Read the whole file into a string
    std::string data((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
    inFile.close();

    if (data.empty()) {
        std::cout << "Input file is empty. Nothing to compress." << std::endl;
        return;
    }

    auto start_time = std::chrono::high_resolution_clock::now();

    // Determine the number of threads and chunk size
    const int num_threads = std::thread::hardware_concurrency(); // Use hardware-supported thread count
    const size_t chunk_size = data.length() / num_threads;
    std::vector<std::thread> threads;
    std::vector<std::string> results(num_threads);

    // Launch threads
    for (int i = 0; i < num_threads; ++i) {
        size_t start = i * chunk_size;
        size_t end = (i == num_threads - 1) ? data.length() : start + chunk_size;
        threads.emplace_back(compress_worker, data.substr(start, end - start), std::ref(results[i]));
    }

    // Join threads
    for (auto& t : threads) {
        t.join();
    }

    // Combine results and write to output file
    std::ofstream outFile(output_filename, std::ios::binary);
    for (const auto& res : results) {
        outFile << res;
    }
    outFile.close();

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end_time - start_time;

    std::cout << "File compressed successfully with " << num_threads << " threads." << std::endl;
    std::cout << "Time taken: " << diff.count() << " seconds." << std::endl;
}

// Main function to perform single-threaded decompression
void decompress_file(const std::string& input_filename, const std::string& output_filename) {
    std::ifstream inFile(input_filename, std::ios::binary);
    if (!inFile) {
        std::cerr << "Error opening input file." << std::endl;
        return;
    }
    std::string data((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
    inFile.close();

    std::string decompressed_data = decompress(data);

    std::ofstream outFile(output_filename, std::ios::binary);
    outFile << decompressed_data;
    outFile.close();

    std::cout << "File decompressed successfully." << std::endl;
}


int main() {
    std::cout << "Multithreaded File Compression/Decompression Tool" << std::endl;
    std::cout << "1. Compress File" << std::endl;
    std::cout << "2. Decompress File" << std::endl;
    std::cout << "Enter your choice: ";
    int choice;
    std::cin >> choice;

    std::string input_file, output_file;
    if (choice == 1) {
        std::cout << "Enter input filename to compress: ";
        std::cin >> input_file;
        std::cout << "Enter output filename: ";
        std::cin >> output_file;
        compress_file(input_file, output_file);
    } else if (choice == 2) {
        std::cout << "Enter input filename to decompress: ";
        std::cin >> input_file;
        std::cout << "Enter output filename: ";
        std::cin >> output_file;
        decompress_file(input_file, output_file);
    } else {
        std::cout << "Invalid choice." << std::endl;
    }

    return 0;
}
