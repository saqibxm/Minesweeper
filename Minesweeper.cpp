#include "Minesweeper.hpp"

#include "Graphics/GraphicalView.hpp"


Minesweeper::Minesweeper() : controller(game), view(std::make_unique<mines::Graphics>(controller))
{
}

void Minesweeper::Run()
{
    auto diff = view->SelectDifficulty();
    controller.NewGameRequested(diff);
    game.Attach(view.get());

    while (!view->ShouldClose())
    {
        view->Display();
        controller.Update();
    }
}
