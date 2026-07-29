#pragma once

#include <type_traits>

#include "source/data/input_output/bin/bin_file/bin_file.hpp"

namespace WRITER_FACTORY_DETAIL {
    template <class T>
    struct is_bin_file: std::false_type {};

    template <class U>
    struct is_bin_file<BINFile<U>>: std::true_type {};

    template <class T>
    struct bin_value_type {};

    template <class U>
    struct bin_value_type<BINFile<U>> {
        using type = U;
    };
}