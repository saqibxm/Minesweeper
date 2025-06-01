#ifndef UTILITY_H
#define UTILITY_H

#include "common.h"

#include <cstdint>
#include <utility>

namespace mines
{
    enum class Difficulty { BEGINNER, INTERMEDIATE, EXPERT, CUSTOM };
    enum Action { NONE = 0, REVEAL, FLAG };

    struct DifficultyConfig
    {
        friend bool operator==(const DifficultyConfig &lhs, const DifficultyConfig &rhs)
        {
            return lhs.level == rhs.level
            && lhs.rows == rhs.rows
            && lhs.cols == rhs.cols
            && lhs.mines == rhs.mines;
        }
        Difficulty level;
        std::size_t rows, cols;
        unsigned mines;
    };

    inline DifficultyConfig DiffToDC(Difficulty diff) noexcept
    {
        DifficultyConfig cfg;
        cfg.level = diff;

        switch(diff)
        {
        case Difficulty::BEGINNER:
            cfg.rows = 7;
            cfg.cols = 7;
            cfg.mines = 10;
            break;

        case Difficulty::INTERMEDIATE:
            cfg.rows = 15;
            cfg.cols = 15;
            cfg.mines = 35;
            break;
        
        case Difficulty::EXPERT:
            cfg.rows = 35;
            cfg.cols = 16;
            cfg.mines = 99;
            break;
        
        default:
            cfg.rows = 0;
            cfg.cols = 0;
            cfg.mines = 0;
            break;
        }
        return cfg;
    }

    template <typename T> using UPair = std::pair<T, T>;
    
    inline UPair<unsigned> TranslateIndex(unsigned index, unsigned width) noexcept
    {
        auto row = index / width;
        auto col = index % width;
    
        return {row, col};
    }
}


#endif /* UTILITY_H */