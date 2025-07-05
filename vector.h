
#ifndef VECTOR_H
#define VECTOR_H
#include <initializer_list>
#include <iostream>
#include <stdexcept>

template <typename T>
class Vector
{
public:
    class ConstIterator;
    class Iterator;
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type &;
    using const_reference = const value_type &;
    using pointer = value_type *;
    using const_pointer = const value_type *;
    using iterator = Vector::Iterator;
    using const_iterator = Vector::ConstIterator;

private:
    size_type sz;
    size_type max_sz;
    pointer values;
    static constexpr size_type DEFAULT_RESERVATION_SCALE = 2;
    static constexpr size_type DEFAULT_MAX_SZ = 0;
    static constexpr size_type DEFAULT_RESERVATION_SHIFT = 1;

    void realloc(size_type new_max_sz)
    {
        if (new_max_sz < DEFAULT_MAX_SZ)
            throw std::runtime_error("Capacity of the vector cannot be smaller than the default capacity");
        if (new_max_sz < sz)
            throw std::runtime_error("New capacity cannot be smaller than the actual size");

        max_sz = new_max_sz;
        pointer new_memarea_vals{nullptr};
        if (max_sz)
        {
            new_memarea_vals = new value_type[max_sz];
            for (size_type i{0}; i < sz; i++)
                new_memarea_vals[i] = values[i];
        }
        if (values)
            delete[] values;
        values = new_memarea_vals;
    }

public:
    Vector() : Vector(DEFAULT_MAX_SZ)
    {
    }

    Vector(size_type n) : sz{0}, max_sz{n}, values{n ? new value_type[n] : nullptr}
    {
    }
    Vector(std::initializer_list<value_type> il) : Vector(il.size() * DEFAULT_RESERVATION_SCALE)
    {
        for (const auto &elem : il)
            values[sz++] = elem;
    }
    ~Vector()
    {
        sz = 0;
        max_sz = 0;
        if (values)
        {
            delete[] values;
            values = nullptr;
        }
    }
    Vector(const Vector &rop) : sz{rop.sz}, max_sz{rop.max_sz}
    {
        values = new value_type[max_sz];
        for (size_type i{0}; i < sz; i++)
            values[i] = rop.values[i];
    }

    Vector &operator=(Vector rop)
    {
        std::swap(rop.sz, sz);
        std::swap(rop.max_sz, max_sz);
        std::swap(rop.values, values);

        return *this;
    }

    size_type size() const
    {
        return sz;
    }
    bool empty() const
    {
        return !sz;
    }
    void clear()
    {
        sz = 0;
    }
    void reserve(size_t n)
    {
        if (max_sz < n)
        {
            max_sz = n;
            realloc(max_sz);
        }
    }
    void shrink_to_fit()
    {
        if (sz < max_sz)
        {
            max_sz = sz;
            realloc(max_sz);
        }
    }
    void push_back(value_type x)
    {
        if (sz >= max_sz)
        {
            max_sz = (sz + DEFAULT_RESERVATION_SHIFT) * DEFAULT_RESERVATION_SCALE;
            realloc(max_sz);
        }
        values[sz++] = x;
    }
    void pop_back()
    {
        if (sz == 0)
            throw std::runtime_error("Attempt to extract an element from the empty vector");
        sz--;
    }
    reference operator[](size_type index)
    {
        if (index >= sz)
            throw std::runtime_error("Vector index is out of bounds");
        return values[index];
    }
    const_reference operator[](size_type index) const
    {
        if (index >= sz)
            throw std::runtime_error("Vector index is out of bounds");
        return values[index];
    }
    size_type capacity() const
    {
        return max_sz;
    }
    friend std::ostream &operator<<(std::ostream &o, const Vector &v)
    {
        o << '[';

        for (size_type i{0}; i < v.size(); i++)
        {
            if (i)
                o << ", " << v.values[i];
            else
                o << v.values[i];
        }
        o << ']';
        return o;
    }
    iterator begin()
    {
        return iterator(&values[0], 0, this);
    }
    iterator end()
    {
        return iterator(&values[sz], sz, this);
    }

    const_iterator begin() const
    {
        return const_iterator(&values[0], 0, this);
    }
    const_iterator end() const
    {
        return const_iterator(&values[sz], sz, this);
    }

    iterator insert(const_iterator pos, const_reference val)
    {
        auto diff = pos - begin();
        if (diff < 0 || static_cast<size_type>(diff) > sz)
            throw std::runtime_error("Iterator is out of bounds");
        size_type current{static_cast<size_type>(diff)};
        if (sz >= max_sz)
        {
            reserve((max_sz + DEFAULT_RESERVATION_SHIFT) * DEFAULT_RESERVATION_SCALE);
        }
        for (auto i{sz}; i-- > current;)
            values[i + 1] = values[i];
        values[current] = val;
        ++sz;
        return iterator(values + current, current, this);
    }
    iterator erase(const_iterator pos)
    {
        auto diff = pos - begin();
        if (diff < 0 || static_cast<size_type>(diff) >= sz)
            throw std::runtime_error("Iterator is out of bounds");
        size_type current{static_cast<size_type>(diff)};
        for (auto i{current}; i < sz - 1; ++i)
            values[i] = values[i + 1];
        --sz;
        return iterator{values + current, current, this};
    }

    friend Iterator;
    friend ConstIterator;

public:
    class Iterator
    {
    public:
        using value_type = Vector::value_type;
        using reference = Vector::reference;
        using pointer = Vector::pointer;
        using difference_type = Vector::difference_type;
        using iterator_category = std::forward_iterator_tag;

    private:
        pointer ptr;
        size_type shift;
        const Vector *tied_vec;
        bool check_validity() const
        {
            return tied_vec && tied_vec->values && ptr && shift < tied_vec->size() &&
                   (tied_vec->values + shift == ptr);
        }

    public:
        Iterator(pointer ptr = nullptr,
                 size_type shift = 0,
                 const Vector *tied_vec = nullptr)
            : ptr{ptr}, shift{shift}, tied_vec{tied_vec}
        {
        }

        reference operator*() const
        {
            if (check_validity())
                return *ptr;
            throw std::runtime_error("Bad Iterator Access");
        }
        pointer operator->() const
        {
            if (check_validity())
                return ptr;
            throw std::runtime_error("Bad Iterator Access");
        };
        iterator &operator++()
        {
            if (check_validity())
            {
                shift++;
                ptr++;
            }
            return *this;
        }
        iterator operator++(int)
        {
            iterator tmp = *this;
            if (check_validity())
            {
                shift++;
                ptr++;
            }

            return tmp;
        }
        operator const_iterator() const
        {
            return ConstIterator(ptr, shift, tied_vec);
        }

        bool operator!=(const const_iterator &rop) const
        {
            return const_iterator(*this) != rop;
        }
        bool operator==(const const_iterator &rop) const
        {
            return const_iterator(*this) == rop;
        }
    };
    class ConstIterator
    {
    public:
        using value_type = Vector::value_type;
        using reference = Vector::const_reference;
        using pointer = Vector::const_pointer;
        using difference_type = Vector::difference_type;
        using iterator_category = std::forward_iterator_tag;

    private:
        pointer ptr;
        size_type shift;
        const Vector *tied_vec;
        bool check_validity() const
        {
            return tied_vec && ptr && shift < tied_vec->size() &&
                   (tied_vec->values + shift == ptr);
        }

    public:
        ConstIterator(pointer ptr = nullptr,
                      size_type shift = 0,
                      const Vector *tied_vec = nullptr)
            : ptr{ptr}, shift{shift}, tied_vec{tied_vec}
        {
        }
        reference operator*() const
        {
            if (check_validity())
                return *ptr;
            throw std::runtime_error("Bad ConstIterator Access");
        }

        pointer operator->() const
        {
            if (check_validity())
                return ptr;
            throw std::runtime_error("Bad ConstIterator Access");
        };
        const_iterator operator++()
        {
            if (check_validity())
            {
                shift++;
                ptr++;
            }
            return *this;
        }
        const_iterator operator++(int)
        {
            const_iterator tmp = *this;
            if (check_validity())
            {
                shift++;
                ptr++;
            }
            return tmp;
        }
        bool operator!=(const const_iterator &rop) const
        {
            return ptr != rop.ptr;
        }
        bool operator==(const const_iterator &rop) const
        {
            return ptr == rop.ptr;
        }
        Vector::difference_type operator-(const const_iterator &rop) const
        {
            return ptr - rop.ptr;
        }
    };
};

#endif
