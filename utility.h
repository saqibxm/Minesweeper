#ifndef UTILITY_H
#define UTILITY_H

#include "common.h"

#include <cstdint>
#include <utility>

namespace mines
{
    enum class Difficulty { BEGINNER, INTERMEDIATE, EXPERT, CUSTOM };
    enum Action { NONE = 0, REVEAL, FLAG };

    template <typename T> using UPair = std::pair<T, T>;
    
    inline UPair<unsigned> TranslateIndex(unsigned index, unsigned width) noexcept
    {
        auto row = index / width;
        auto col = index % width;
    
        return {row, col};
    }
}


#endif /* UTILITY_H */