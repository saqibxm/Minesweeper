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

#include <cstdint>

namespace mines
{
    using Index = std::size_t;
}