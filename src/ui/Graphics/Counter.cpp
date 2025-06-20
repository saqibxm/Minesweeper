#include <map>
#include <cmath>
#include <cassert>

#include "Graphics/Counter.hpp"

using namespace mines;

Counter::Counter(TextureManager &texman, int val)
    : manager(texman), digits()
    , left(manager.Placeholder()), frame(manager.Placeholder()), right(manager.Placeholder())
{
    for (decltype(digits)::size_type i = 0; i < digits.size(); ++i)
    {
        digits[i]
            .UpdateSize(25, 50)
        .UpdatePosition(0 + i*25, 10)
        .UpdateTexture(manager.FetchPtr("counter0"));
    }

    left.setTexture(manager.Fetch("counterleft"), true);
    frame.setTexture(manager.Fetch("countermiddle"), true);
    right.setTexture(manager.Fetch("counterright"), true);
}

void Counter::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    for (auto rit = digits.rbegin(); rit != digits.rend(); ++rit)
        rit->draw(target, states);

    target.draw(left, states);
    // target.draw(frame, states);
    target.draw(right, states);
}

void Counter::UpdatePosition(float x, float y)
{
    position = {x, y};

    for (decltype(digits)::size_type i = 0; i < digits.size(); ++i)
    {
        auto [width, height] = digits[i].RetrieveSize();
        digits[i].UpdatePosition(x + i * width, y);
    }

    left.setPosition(position);
    right.setPosition({position.x + size.x * digits.size(), position.y});
    frame.setOrigin(position + (RetrieveSize().componentWiseDiv({2.0f, 2.0f})));
}

sf::Vector2f Counter::RetrieveSize() const
{
    auto sz = size;
    sz.x *= digits.size();
    return sz;
}


const sf::Vector2f& Counter::RetrievePosition() const
{
    return position;
}


void Counter::UpdateSize(float w, float h)
{
    size.x = w;
    size.y = h;
    //  A check to ensure a certain ratio
    for (decltype(digits)::size_type i = 0; i < digits.size(); ++i)
    {
        digits[i].UpdateSize(w, h);
    }
    UpdatePosition(position.x, position.y);

    const auto &leftBorder = manager.Fetch("counterleft");
    const auto &frameBorder = manager.Fetch("countermiddle");
    const auto &rightBorder = manager.Fetch("counterright");

    left.setScale({ 1.0f, h / leftBorder.getSize().y});
    right.setScale({ 1.0f, h / rightBorder.getSize().y});
    frame.setScale({ w / frameBorder.getSize().x, h / frameBorder.getSize().y});
}


void Counter::SetNumber(int val)
{
    value = val;
    UpdateAllFaces();
}

int Counter::GetNumber() const
{
    return value;
}

void Counter::ResetNumber()
{
    SetNumber(000);
}


void Counter::UpdateAllFaces()
{
    std::size_t index = 0;
    bool negative = value < 0;
    int val = value;

    for(int i = 0; i < digits.size(); ++i)
        UpdateFace(i, Face::ZERO);

    if(negative) val *= -1;
    while(val && index < digits.size()) {
        int digit = (val % 10);
        UpdateFace(digits.size() - index - 1, static_cast<Face>(digit));
        val /= 10;
        ++index;
    }

    if(negative)
        UpdateFace(index < digits.size() ? digits.size() - index - 1 : 0, Face::MINUS);
}


void Counter::UpdateFace(std::size_t index, Face face)
{
    constexpr const char prefix[] = "counter";

    static const std::map<Face, char> texmap
    {
        { MINUS, '-' },
        { ZERO, '0' },
        { ONE, '1' },
        { TWO, '2' },
        { THREE, '3' },
        { FOUR, '4' },
        { FIVE, '5' },
        { SIX, '6' },
        { SEVEN, '7' },
        { EIGHT, '8' },
        { NINE, '9' }
    };

    digits[index].UpdateTexture(manager.FetchPtr(std::string(prefix) + texmap.at(face)));
}
