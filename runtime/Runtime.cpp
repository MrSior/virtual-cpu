#include "Runtime.h"

Runtime::Runtime(std::string bin_path) {
    bin_path_ = std::move(bin_path);

    std::ifstream in(bin_path_, std::ios::in | std::ios::binary);
    uint16_t size = 0;
    in.read(reinterpret_cast<char *>(&size), sizeof(size));

    if (!in.good()) {
        throw std::runtime_error(
            "Failed read size of program from binary file");
    }

    program_.resize(size);

    in.read(reinterpret_cast<char *>(program_.data()),
            sizeof(PolizEntry) * size);

    if (!in.good()) {
        throw std::runtime_error("Failed read program from binary file");
    }

    in.close();
}

void Runtime::run() { container::Stack<int64_t> stack; }