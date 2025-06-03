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
    // commands.reserve(100);
}

Controller::~Controller()
{
}

void Controller::RevealRequested(Index r, Index c)
{
    auto cmd = std::make_unique<RevealCommand>(r, c);
    HandleCommand(std::move(cmd));
}

void Controller::FlagRequested(Index r, Index c)
{
    auto cmd = std::make_unique<FlagCommand>(r, c);
    HandleCommand(std::move(cmd));
}

void Controller::NewGameRequested(const DifficultyConfig &cfg)
{
    auto cmd = std::make_unique<NewCustomGameCommand>(cfg);
    HandleCommand(std::move(cmd));
}

void Controller::NewGameRequested()
{
    auto cmd = std::make_unique<NewGameCommand>();
    HandleCommand(std::move(cmd));
}


void Controller::HandleCommand(std::unique_ptr<ICommand> cmd)
{
    bool success = cmd->Execute(model);

    if (success)
        commands.push(std::move(cmd));

    // commands.top()->Execute(model);
}