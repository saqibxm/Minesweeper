#pragma once

#define interface struct

#define ABSTRACT 0
#include <cstdint>


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
    using Index = std::uint_fast16_t;
}