#pragma once

#include <SFML/Graphics.hpp>

#include "common.h"
#include "utility.h"

namespace mines::impl {

class DifficultySelectorDelegate
{
public:
    DifficultySelectorDelegate(sf::Font &font_);

    DifficultyConfig PromptSelection();

private:
    sf::RenderWindow prompt;
    sf::Font font;

    std::array<unsigned, 3> PromptCustomDifficulty();
};

}
