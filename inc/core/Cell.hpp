#pragma once

#ifndef CELL_HPP
#define CELL_HPP

namespace mines
{
    struct Cell
    {
        enum State { HIDDEN, CLEARED, FLAGGED, UNDETERMINATE};

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

#endif /* CELL_HPP */