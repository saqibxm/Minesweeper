#pragma once

#define interface struct

#define ABSTRACT 0

constexpr inline bool debug =
#ifdef NDEBUG
    false
#else
    true
#endif
;

#include <cstddef>

namespace mines
{
    using Index = std::size_t;
}