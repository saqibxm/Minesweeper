#include <array>
#include <string>
#include <cctype>
#include <optional>

#include "Graphics/DifficultySelector.hpp"

using namespace mines;
using namespace mines::impl;

DifficultySelector::DifficultySelector(sf::Font &font_) : font(font_)
{
}


DifficultyConfig DifficultySelector::PromptSelection()
{
    constexpr std::size_t sz = 4;
    sf::RectangleShape buttons[sz];
    std::vector<sf::Text> labels;

    if (prompt.isOpen()) prompt.close();
    prompt.create(sf::VideoMode({240, 240}), "Select Difficulty", sf::Style::Close);

    // Predefined difficulty levels for button mapping
    constexpr Difficulty levels[sz] = {
        Difficulty::BEGINNER,
        Difficulty::INTERMEDIATE,
        Difficulty::EXPERT,
        Difficulty::CUSTOM
    };

    // Layout and initialize buttons and labels
    for (std::size_t i = 0; i < sz; ++i)
    {
        buttons[i].setSize({220, 50});
        buttons[i].setPosition({10.0f, 60.0f * i + 10});
        buttons[i].setFillColor(sf::Color::White);

        sf::Text text(font, DifficultyConfig::DiffToString(levels[i]).data(), 22);
        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin({textRect.size.y + textRect.size.x / 2.0f, textRect.position.y + textRect.size.y / 2.0f});
        text.setPosition({buttons[i].getPosition().x + buttons[i].getSize().x / 2.0f,
                         buttons[i].getPosition().y + buttons[i].getSize().y / 2.0f});
        text.setFillColor(sf::Color::Black);
        labels.push_back(std::move(text));
    }

    std::optional<DifficultyConfig> chosen;

    // Main input loop
    while (prompt.isOpen())
    {
        prompt.clear(sf::Color::Black);

        while (const auto event = prompt.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                prompt.close();
                break;
            }
            else if (const auto mbr = event->getIf<sf::Event::MouseButtonReleased>())
            {
                const auto mousePos = sf::Vector2f(mbr->position);
                for (std::size_t i = 0; i < sz; ++i)
                {
                    if (buttons[i].getGlobalBounds().contains(mousePos))
                    {
                        if (i < sz - 1)
                        {
                            chosen = DifficultyConfig::From(levels[i]);
                        }
                        else
                        {
                            // Custom selection — prompt user
                            prompt.close();
                            auto [rows, cols, mines] = PromptCustomDifficulty();
                            chosen.emplace(Difficulty::CUSTOM, rows, cols, mines);
                        }
                        prompt.close();
                        break;
                    }
                }
            }
        }

        for (auto &btn : buttons) prompt.draw(btn);
        for (auto &label : labels) prompt.draw(label);
        prompt.display();
    }

    if (!chosen.has_value())
        throw std::runtime_error("No difficulty selected.");

    return chosen.value();
}


std::array<unsigned, 3> DifficultySelector::PromptCustomDifficulty()
{
    constexpr unsigned nfields = 3;
    sf::RenderWindow &inputWindow = prompt;
    prompt.create(sf::VideoMode({300, 300}), "Custom Difficulty", sf::Style::Close);

    sf::Text title(font, "Press Enter to Advance", 16);
    title.setPosition({10.f, 10.f});
    title.setFillColor(sf::Color::White);

    std::array<std::string, nfields> labels = {"Rows:", "Cols:", "Mines:"};
    std::array<std::string, nfields> inputs = {"", "", ""};
    std::vector<sf::Text> inputTexts;
    std::array<sf::RectangleShape, nfields> inputBoxes;

    for (int i = 0; i < nfields; ++i)
    {
        inputTexts.emplace_back(font);
        inputBoxes[i].setOutlineColor(sf::Color::Cyan);
        inputTexts[i].setCharacterSize(18);
        inputTexts[i].setFillColor(sf::Color::Black);
        inputTexts[i].setPosition({110.f, 50.f + i * 60});

        inputBoxes[i].setSize({150.f, 30.f});
        inputBoxes[i].setPosition({100.f, 50.f + i * 60});
        inputBoxes[i].setFillColor(sf::Color::White);
    }

    int currentBox = 0;
    sf::Text labelText(font,"" , 18);
    labelText.setFillColor(sf::Color::White);

    sf::RectangleShape okButton({80.f, 40.f});
    okButton.setPosition({110.f, 240.f});
    okButton.setFillColor(sf::Color::Green);
    sf::Text okText(font ,"OK" , 18);
    okText.setPosition({135.f, 250.f});
    okText.setFillColor(sf::Color::Black);

    while (inputWindow.isOpen())
    {
        while (const auto event = inputWindow.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                inputWindow.close();
            else if (event->is<sf::Event::TextEntered>())
            {
                auto e = *event->getIf<sf::Event::TextEntered>();
                if (std::isdigit(e.unicode) && inputs[currentBox].size() < 5)
                    inputs[currentBox] += static_cast<char>(e.unicode);
                else if (e.unicode == '\b' && !inputs[currentBox].empty())
                    inputs[currentBox].pop_back();

                inputTexts[currentBox].setString(inputs[currentBox]);
            }
            else if (const auto *kp = event->getIf<sf::Event::KeyPressed>())
            {
                const auto &e = *kp;
                if (e.code == sf::Keyboard::Key::Enter)
                {
                    if (currentBox < 2)
                        ++currentBox;
                    else
                        inputWindow.close();
                }
                else if (e.code == sf::Keyboard::Key::Tab)
                {
                    currentBox = (currentBox + 1) % nfields;
                }
            }
            else if (const auto *mbr = event->getIf<sf::Event::MouseButtonReleased>())
            {
                for (int i = 0; i < nfields; ++i)
                {
                    if (inputBoxes[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(mbr->position)))
                        currentBox = i;
                }
                if (okButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(inputWindow))))
                    inputWindow.close();
            }
            for (int i = 0; i < nfields; ++i) inputBoxes[i].setOutlineThickness(0.0f);
            inputBoxes[currentBox].setOutlineThickness(4.0f);
        }

        inputWindow.clear(sf::Color::Black);
        inputWindow.draw(title);
        for (int i = 0; i < 3; ++i)
        {
            labelText.setString(labels[i]);
            labelText.setPosition({20.f, 50.f + i * 60});
            inputWindow.draw(labelText);
            inputWindow.draw(inputBoxes[i]);
            inputWindow.draw(inputTexts[i]);
        }
        inputWindow.draw(okButton);
        inputWindow.draw(okText);
        inputWindow.display();
    }

    std::array<unsigned, 3> ret;
    for (std::size_t i = 0; i < std::size(ret); ++i)
        ret[i] = std::stoi(inputs[i]);
    return ret;
}