#pragma once

#include <string>

#include "common.h"
#include "utility"
#include "Observer.hpp"

namespace mines
{
    class DifficultyConfig {};
    class Controller;

    interface IView : public IObserver
    {
        virtual ~IView() = default;
        // virtual void GrantControl(Controller&) = ABSTRACT;
        virtual DifficultyConfig SelectDifficulty() = ABSTRACT;
        virtual void ShowMessage(const std::string &) = ABSTRACT;
        virtual void Display() = ABSTRACT; // Draw the board
        virtual bool ShouldClose() const = ABSTRACT;
    };
} // namespace mines
