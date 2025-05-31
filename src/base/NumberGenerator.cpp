#include "NumberGenerator.hpp"

using namespace mines;

template <typename T>
NumberGenerator<T>::NumberGenerator(Type lo, Type hi)
    : dist(lo, hi)
{
}

template <typename T>
void NumberGenerator<T>::Range(Type lo, Type hi)
{
    dist = decltype(dist)(lo, hi);
}

template <typename T>
NumberGenerator<T>::Type NumberGenerator<T>::Generate()
{
    return dist(eng);
}

template <typename T>
void NumberGenerator<T>::Seed(unsigned val)
{
    eng.seed(val);
}