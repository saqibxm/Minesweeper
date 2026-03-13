#pragma once

#include <vector>
#include <fstream>
#include <filesystem>
#include <string>
#include <cstdint>

#include "common.h"

namespace mines
{
    /// A single recorded player action.
    struct ReplayEntry
    {
        enum class Action : uint8_t { REVEAL = 0, FLAG = 1 };

        Action   action;
        Index    row;
        Index    col;
        double   timestamp; // seconds since game start
    };

    /**
     * Stores a complete sequence of player actions for one game.
     *
     * Binary file format (.mswr):
     *   Header: "MSWR" (4 bytes) | version (1 byte = 1) | rows (2) | cols (2) | mines (2)
     *   Entries: count (4 bytes) followed by count × ReplayEntry records
     *     Each entry: action (1) | row (2) | col (2) | timestamp (8, IEEE 754 double)
     */
    class Replay
    {
    public:
        Replay() = default;

        void Record(ReplayEntry::Action action, Index row, Index col, double timestamp);
        void Clear() noexcept;

        bool Empty() const noexcept { return entries_.empty(); }
        std::size_t Size() const noexcept { return entries_.size(); }
        const std::vector<ReplayEntry>& Entries() const noexcept { return entries_; }

        bool Save(const std::filesystem::path &path) const;
        static Replay Load(const std::filesystem::path &path);

        static std::filesystem::path DefaultPath();

    private:
        std::vector<ReplayEntry> entries_;

        static constexpr const char Magic[4] = {'M', 'S', 'W', 'R'};
        static constexpr uint8_t Version = 1;
    };
}
