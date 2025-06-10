#pragma once

#include <vector>
#include <type_traits>

/*
template <typename T, class C>
struct SubscriptProxy
{
    using Host = FlatVector<T, C>;

    T& operator[](std::size_t index);
    const T& operator[](std::size_t index);

    typename Host::size_type size() const noexcept;

    Host &context;
};
*/

template <typename T, class C = std::vector<T>>
class FlatVector
{
    friend struct SubscriptProxy;
    using Type = typename std::remove_const<typename std::remove_reference<T>::type>::type;
    using Container = C;
    
public:
    using size_type = typename Container::size_type;
    using value_type = typename Container::value_type;
    struct SubscriptProxy
    {
        using Host = FlatVector;

        SubscriptProxy(const Host *host, size_type row_number) noexcept;

        const value_type& operator[](std::size_t col) const;
        value_type& operator[](std::size_t col);

        size_type size() const noexcept;

        const Host * const context;
        size_type row;
    };

    FlatVector() noexcept;
    FlatVector(size_type rows, size_type cols);
    FlatVector(const FlatVector&) = default;

    // const value_type& operator()(size_type row, size_type col) const;
    // value_type& operator()(size_type row, size_type col);

    const SubscriptProxy operator[](size_type index) const;
    SubscriptProxy operator[](size_type index);
    // const value_type& operator[](size_type index) const;
    // value_type& operator[](size_type index);

    size_type size() const noexcept;
    size_type rows_count() const noexcept;
    size_type cols_count() const noexcept;

    void resize(size_type rows, size_type cols);
    void clear() noexcept;
    bool empty() const noexcept;
    void reserve(size_type rows, size_type cols);
    void reserve(size_type size);

private:
    Container storage;
    size_type rows, cols;
};

// friend struct SubscriptProxy<T>;
// template <typename T, template <typename> class C = std::vector>


template <typename T, class C>
FlatVector<T, C>::SubscriptProxy::SubscriptProxy(const Host *host, size_type row_number) noexcept : context(host), row(row_number)
{
}

template <typename T, class C>
const typename FlatVector<T,C>::value_type& FlatVector<T, C>::SubscriptProxy::operator[](std::size_t col) const
{
    // return (*context)(row, col);
    return context->storage[row * context->cols + col];
}

template <typename T, class C>
typename FlatVector<T,C>::value_type& FlatVector<T, C>::SubscriptProxy::operator[](std::size_t col)
{
    return const_cast<value_type&>(static_cast<const SubscriptProxy*>(this)->operator[](col));
}

template <typename T, class C>
typename FlatVector<T, C>::size_type FlatVector<T, C>::SubscriptProxy::size() const noexcept
{
    return context->cols_count();
}

template <typename T, class C>
FlatVector<T, C>::FlatVector() noexcept : rows(0), cols(0)
{
}

template <typename T, class C>
FlatVector<T, C>::FlatVector(size_type rows, size_type cols) : rows(rows), cols(cols)
{
    storage.resize(rows * cols);
}

/*
template<typename T, class C>
const typename FlatVector<T, C>::value_type& FlatVector<T, C>::operator()(size_type row, size_type col) const
{
    return storage[row * cols + col];
}

template<typename T, class C>
typename FlatVector<T, C>::value_type &FlatVector<T, C>::operator()(size_type row, size_type col)
{
    return storage[row * cols + col];
}
*/

template<typename T, class C>
typename FlatVector<T, C>::SubscriptProxy FlatVector<T, C>::operator[](size_type index)
{
    return SubscriptProxy(this, index);
}

template<typename T, class C>
const typename FlatVector<T, C>::SubscriptProxy FlatVector<T, C>::operator[](size_type index) const
{
    return SubscriptProxy(this, index);
}

/*
template<typename T, class C>
const typename FlatVector<T, C>::value_type &FlatVector<T, C>::operator[](size_type index) const
{
    return storage[index];
}

template<typename T, class C>
typename FlatVector<T, C>::value_type &FlatVector<T, C>::operator[](size_type index)
{
    return storage[index];
}
*/

template <typename T, class C>
typename FlatVector<T, C>::size_type FlatVector<T, C>::size() const noexcept
{
    return rows_count();
}

template <typename T, class C>
typename FlatVector<T, C>::size_type FlatVector<T, C>::rows_count() const noexcept
{
    return rows;
}

template<typename T, class C>
typename FlatVector<T, C>::size_type FlatVector<T, C>::cols_count() const noexcept
{
    return cols;
}

template <typename T, class C>
void FlatVector<T, C>::resize(size_type rows, size_type cols)
{
    this->rows = rows;
    this->cols = cols;

    storage.resize(rows * cols);
}

template <typename T, class C>
void FlatVector<T, C>::clear() noexcept
{
    storage.clear();
}

template <typename T, class C>
bool FlatVector<T, C>::empty() const noexcept
{
    return storage.empty();
}

template <typename T, class C>
void FlatVector<T, C>::reserve(size_type rows, size_type cols)
{
    reserve(rows, cols);
}

template <typename T, class C>
void FlatVector<T, C>::reserve(size_type size)
{
    storage.reserve(size);
}