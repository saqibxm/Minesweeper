#pragma once

#include <random>
#include <type_traits>

#include "common.h"

namespace mines
{
    template <typename T>
    class NumberGenerator
    {
        using Type = std::remove_const_t<std::remove_reference_t<T>>;
        static_assert(std::is_integral_v<Type>, "Type must be integral");

    public:
        NumberGenerator() = default;
        NumberGenerator(Type lo, Type hi);
        
        void Range(Type lo, Type hi);
        Type Generate();

        void Seed(unsigned val);

    private:
        std::default_random_engine eng;
        std::uniform_int_distribution<T> dist;

    public:
        decltype(eng)& UnderlyingEngine() { return eng; }
    };

    template <typename T>
    NumberGenerator<T>::NumberGenerator(Type lo, Type hi)
    : dist(lo, hi)
    {
        if constexpr (debug)
        {
            Seed(std::random_device{}());
        }
        else Seed(777);
    }

    template <typename T>
    void NumberGenerator<T>::Range(Type lo, Type hi)
    {
        dist = decltype(dist)(lo, hi);
    }

    template <typename T>
    typename NumberGenerator<T>::Type NumberGenerator<T>::Generate()
    {
        return dist(eng);
    }

    template <typename T>
    void NumberGenerator<T>::Seed(unsigned val)
    {
        eng.seed(val);
    }
}