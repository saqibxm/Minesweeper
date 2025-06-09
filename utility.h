#ifndef UTILITY_H
#define UTILITY_H

#include "common.h"

#include <cstdint>
#include <utility>
#include <string_view>

namespace mines
{
    enum class Difficulty : std::uint8_t { BEGINNER = 0, INTERMEDIATE, EXPERT, CUSTOM };
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
        static std::string_view DiffToString(const Difficulty &diff)
        {
            switch(diff)
            {
            case Difficulty::BEGINNER:
                return "Beginner";
            case Difficulty::INTERMEDIATE:
                return "Intermediate";
            case Difficulty::EXPERT:
                return "Expert";
            case Difficulty::CUSTOM:
                return "Custom";
            default:
                break;
            }

            return "Unknown";
        }
        static inline DifficultyConfig From(Difficulty diff) noexcept
        {
            DifficultyConfig cfg;
            cfg.level = diff;

            switch(diff)
            {
                case Difficulty::BEGINNER:
                    cfg.rows = 9;
                    cfg.cols = 9;
                    cfg.mines = 10;
                    break;

                case Difficulty::INTERMEDIATE:
                    cfg.rows = 16;
                    cfg.cols = 16;
                    cfg.mines = 40;
                    break;

                case Difficulty::EXPERT:
                    cfg.rows = 16;
                    cfg.cols = 30;
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

        explicit operator bool() const noexcept { return *this != DifficultyConfig(); }

        Difficulty level = Difficulty::CUSTOM;
        std::size_t rows = 0, cols = 0;
        unsigned mines = 0;
    };

    template <typename T> using UPair = std::pair<T, T>;
    
    inline UPair<unsigned> TranslateIndex(unsigned index, unsigned width) noexcept
    {
        auto row = index / width;
        auto col = index % width;
    
        return {row, col};
    }
}


#endif /* UTILITY_H */