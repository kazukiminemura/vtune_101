#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

constexpr size_t dataSize = 1024 * 1024 * 1000; // 10000M floats
constexpr size_t num_threads = 112;
constexpr size_t chunk_size = dataSize / num_threads;

void access_memory(char* data, size_t offset, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        data[offset + i] *= 2;
    }
}

int main() {
    std::vector<char> data(dataSize, 1);
    std::vector<std::thread> threads;

    auto start = std::chrono::high_resolution_clock::now();

    for (size_t t = 0; t < num_threads; ++t) {
        size_t offset = t * chunk_size;
        threads.emplace_back(access_memory, data.data(), offset, chunk_size);
    }

    for (auto& th : threads) {
        th.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout << "Parallel memory access completed in " << duration.count() << " seconds.\n";

    return 0;
}
