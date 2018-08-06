#pragma once

#include <iostream>
#include <cstring>
#include <string>   // char_traits
#include <utility>


namespace targem {


/*
    Basic character type independent string class.
    NOT using SSO, NOT allocator aware.
*/
template <
    class CharT,
    class Traits = std::char_traits<CharT>
>
class basic_string {
public:
    using traits_type = Traits;
    using value_type = CharT;


    basic_string() noexcept {
        m_ptr = nullptr;
        m_size = m_capacity = 0;
    }
    
    basic_string(CharT const* data)
        : basic_string(data, std::strlen(data)) {}

    basic_string(CharT const* data, std::size_t size) {
        m_ptr  = new CharT[size + 1];
        m_size = m_capacity = size;

        Traits::copy(m_ptr, data, size);
        Traits::assign(m_ptr[size], static_cast<CharT>(0));
    }

    // Copy cstr
    basic_string(basic_string const& other)
        : basic_string(other.data(), other.size()) {}

    // Move cstr
    basic_string(basic_string&& bs) noexcept {
        m_ptr      = bs.m_ptr;
        m_size     = bs.m_size;
        m_capacity = bs.m_capacity;

        bs.m_ptr      = nullptr;
        bs.m_size     = 0;
        bs.m_capacity = 0;
    }

    // Copy assignment operator
    basic_string& operator=(basic_string const& other) {
        if (this != &other) {
            this->~basic_string();

            new (this) basic_string(other);
        }

        return *this;
    }

    // Move assignment operator
    basic_string& operator=(basic_string&& other) {
        if (this != &other) {
            this->~basic_string();

            new (this) basic_string(std::forward<basic_string>(other));
        }

        return *this;
    }

    ~basic_string() {
        if (m_ptr != nullptr) {
            delete[] m_ptr;
        }
    }

    CharT const* data() const noexcept {
        return m_ptr;
    }

    std::size_t size() const noexcept {
        return m_size;
    }

    std::size_t capacity() const noexcept {
        return m_capacity;
    }

    void reserve(std::size_t capacity) {
        if (m_capacity < capacity) {
            auto ptr = new CharT[capacity + 1];

            Traits::copy(ptr, m_ptr, m_size);
            Traits::assign(ptr[m_size], static_cast<CharT>(0));

            this->~basic_string();

            m_ptr = ptr;
            m_capacity = capacity;
        }
    }

    basic_string& append(CharT const* data, std::size_t size) {
        if (m_capacity < m_size + size) {
            reserve(static_cast<std::size_t>((m_size + size) * 1.5f));
        }

        Traits::copy(m_ptr + m_size, data, size);
        m_size += size;

        Traits::assign(m_ptr[m_size], static_cast<CharT>(0));

        return *this;
    }

    basic_string& append(basic_string const& other) {
        return append(other.data(), other.size());
    }

    void clear() noexcept {
        if (m_size > 0) {
            Traits::assign(m_ptr[0], static_cast<CharT>(0));
            m_size = 0;
        }
    }

private:
    CharT* m_ptr;
    std::size_t m_size;
    std::size_t m_capacity;
};



template <class CharT, class Traits>
int compare(
    basic_string<CharT, Traits> const& lhs,
    basic_string<CharT, Traits> const& rhs
) noexcept {
    std::size_t ls = lhs.size();
    std::size_t rs = rhs.size();

    int res = Traits::compare(lhs.data(), rhs.data(), std::min(ls, rs));

    return (res == 0 ? (ls == rs ? 0 : (ls < rs ? -1 : 1)) : res);
}

template <class CharT, class Traits>
bool operator==(
    basic_string<CharT, Traits> const& lhs,
    basic_string<CharT, Traits> const& rhs
) noexcept {
    return compare(lhs, rhs) == 0;
}

template <class CharT, class Traits>
bool operator<(
    basic_string<CharT, Traits> const& lhs,
    basic_string<CharT, Traits> const& rhs
) noexcept {
    return compare(lhs, rhs) < 0;
}

template <class CharT, class Traits>
bool operator>(
    basic_string<CharT, Traits> const& lhs,
    basic_string<CharT, Traits> const& rhs
) noexcept {
    return compare(lhs, rhs) > 0;
}



template <class CharT, class Traits>
basic_string<CharT, Traits> operator+(
    basic_string<CharT, Traits> const& lhs,
    basic_string<CharT, Traits> const& rhs
) {
    basic_string<CharT, Traits> s;
    s.reserve(lhs.size() + rhs.size());

    return s.append(lhs).append(rhs);
}

template <class CharT, class Traits>
basic_string<CharT, Traits> operator+(
    CharT const* lhs,
    basic_string<CharT, Traits> const& rhs
) {
    basic_string<CharT, Traits> s;
    s.reserve(std::strlen(lhs) + rhs.size());

    return s.append(lhs).append(rhs);
}

template <class CharT, class Traits>
basic_string<CharT, Traits> operator+(
    basic_string<CharT, Traits> const& lhs,
    CharT const* rhs
) {
    basic_string<CharT, Traits> s;
    s.reserve(lhs.size() + std::strlen(rhs));

    return s.append(lhs).append(rhs);
}


template <class CharT, class Traits>
basic_string<CharT, Traits> operator+=(
    basic_string<CharT, Traits> const& lhs,
    basic_string<CharT, Traits> const& rhs
) {
    basic_string<CharT, Traits> s;
    s.reserve(lhs.size() + rhs.size());

    return s.append(lhs).append(rhs);
}

template <class CharT, class Traits>
basic_string<CharT, Traits> operator+=(
    basic_string<CharT, Traits> const& lhs,
    CharT const* rhs
) {
    basic_string<CharT, Traits> s;
    s.reserve(lhs.size() + std::strlen(rhs));

    return s.append(lhs).append(rhs);
}



template <class CharT, class Traits>
std::ostream& operator<<(
    std::ostream& os, basic_string<CharT, Traits> const& bs
) {
    return (os << (bs.size() == 0 ? "" : bs.data()));
}

// Very slow istream operator.
template <class CharT, class Traits>
std::istream& operator>>(
    std::istream& is, basic_string<CharT, Traits>& bs
) {
    auto nl = static_cast<CharT>('\n');

    CharT ch = nl;

    // Read until new line character
    for (is.read(&ch, 1); ch != nl; is.read(&ch, 1)) {
        bs.append(&ch, 1);
    }

    return is;
}

} // namespace targem
