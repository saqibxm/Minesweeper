#include <cassert>

#include "Controller.hpp"
#include "Graphics/GraphicalView.hpp"

using namespace mines;

/*
Controller::Controller(Game *p_model, bool graphical)
: model(p_model)
{
    view.reset(new Graphics(*model));
    assert(model && view);
    
    model->Attach(view.get());
}
*/

Controller::Controller(Game &gameModel) : model(gameModel)
{
}

Controller::~Controller()
{
}

void Controller::HandleCommand(std::unique_ptr<ICommand> cmd)
{
    commands.push(std::move(cmd));

    commands.top()->Execute(model);
}