#include "source/data/utils.h"

size_t FILES::get_file_size(std::ifstream& is) {
    std::streampos current_position = is.tellg();

    is.seekg(0, std::ios::end);
    std::streampos end_position = is.tellg();
    is.seekg(current_position, std::ios::beg);

    return static_cast<size_t>(end_position);
}

size_t FILES::get_file_size(std::ofstream& os) {
    std::streampos current_position = os.tellp();

    os.seekp(0, std::ios::end);
    std::streampos end_position = os.tellp();
    os.seekp(current_position, std::ios::beg);

    return static_cast<size_t>(end_position);
}

bool NUM::is_number(const std::string& value, double& out) {
    if (value.empty()) {
        return false;
    }

    std::istringstream iss(value);
    iss >> out;

    return !iss.fail() && iss.eof();
}