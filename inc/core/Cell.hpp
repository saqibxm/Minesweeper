#pragma once

#ifndef CELL_HPP
#define CELL_HPP

namespace mines
{
    struct Cell
    {
        enum State : std::uint8_t { HIDDEN, CLEARED, FLAGGED, UNDETERMINATE};

        explicit Cell(bool is_mine = false) noexcept(noexcept(proximity()));

        bool mine() const noexcept;
        bool revealed() const noexcept;
        bool flagged() const noexcept;
        bool proximal() const noexcept;
        short proximity() const noexcept;
        State state() const noexcept;
        
        void toggle_flag() noexcept;
        void mine(bool set) noexcept(noexcept(proximity()));
        void reveal() noexcept;
        void state(State state) noexcept;
        void proximity(short n);

        void open() noexcept; // clear the cell

        // explicit operator bool() const noexcept { return mine_; }
        void reset() noexcept;

        bool mine_;
        bool flag_;
        short proximity_;
        State state_ = HIDDEN;
    };
}

/* EXPERIMENTAL
bool mine_ : 1;
bool flag_ : 1;
std::uint8_t proximity_ : 4;
State state_ : 2 = HIDDEN;
*/

#endif /* CELL_HPP */