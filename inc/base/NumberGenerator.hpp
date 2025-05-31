#pragma once

#include <random>
#include <type_traits>

namespace mines
{
    template <typename T>
    class NumberGenerator
    {
        using Type = std::remove_const_t<std::remove_reference_t<T>>;
        static_assert(std::is_integral_v<Type>, "Type must be integral");
    public:
        NumberGenerator(Type lo, Type hi);
        
        void Range(Type lo, Type hi);
        Type Generate();

        void Seed(unsigned val);

    private:
        std::default_random_engine eng;
        std::uniform_int_distribution<T> dist;
    };
}