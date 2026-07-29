#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace FILES {
    size_t get_file_size(std::ifstream& is);
    size_t get_file_size(std::ofstream& os);
}

namespace NUM {
    bool is_number(const std::string& value, double& out);
}