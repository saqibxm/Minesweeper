#pragma once

#ifndef CELL_HPP
#define CELL_HPP

namespace mines
{
    struct Cell
    {
        enum State : std::uint8_t { HIDDEN, CLEARED, FLAGGED, UNDETERMINATE};

        explicit Cell(bool is_mine = false);

        bool mine() const;
        bool revealed() const;
        bool flagged() const;
        bool proximal() const;
        short proximity() const;
        State state() const;
        
        void toggle_flag();
        void mine(bool set);
        void reveal();
        void state(State state);
        void proximity(short n);

        void open(); // clear the cell

        // explicit operator bool() const noexcept { return mine_; }

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