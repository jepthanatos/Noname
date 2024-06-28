#ifndef __PROPERTY_H__
#define __PROPERTY_H__

// Local includes
#include "HtmlBuilder.h"

// System includes
#include <string>
#include <string_view>
#include <type_traits>
#include <algorithm> // Para std::min y std::max

namespace noname
{
    template <typename T>
    class Property
    {
    private:
        T value;

    public:
        Property(const T initialValue)
            : value(initialValue)
        {
        }

        operator T() const
        {
            return value;
        }

        Property<T> &operator=(T newValue)
        {
            value = newValue;
            return *this;
        }

        bool operator>(const T &rValue) const
        {
            return value > rValue;
        }

        bool operator<(const T &rValue) const
        {
            return value < rValue;
        }

        bool operator>=(const T &rValue) const
        {
            return value >= rValue;
        }

        bool operator<=(const T &rValue) const
        {
            return value <= rValue;
        }

        bool operator==(const T &rValue) const
        {
            return value == rValue;
        }

        bool operator!=(const T &rValue) const
        {
            return value != rValue;
        }

        Property<T> &operator++()
        {
            ++value;
            return *this;
        }

        Property<T> operator++(int)
        {
            Property<T> temp = *this;
            ++value;
            return temp;
        }

        Property<T> &operator--()
        {
            --value;
            return *this;
        }

        Property<T> operator--(int)
        {
            Property<T> temp = *this;
            --value;
            return temp;
        }

        Property<T> &operator+=(const T &rightValue)
        {
            value += rightValue;
            return *this;
        }

        Property<T> &operator-=(const T &rightValue)
        {
            value -= rightValue;
            return *this;
        }

        std::string toString() const
        {
            if constexpr (std::is_convertible_v<T, std::string> || std::is_convertible_v<T, std::string_view>)
            {
                return value;
            }
            else
            {
                return std::to_string(value);
            }
        }

        HtmlBuilder toHtmlBuilder(const std::string &title) const
        {
            if constexpr (std::is_convertible_v<T, std::string> || std::is_convertible_v<T, std::string_view>)
            {
                return HtmlBuilder{"tr"}.add_child("td", title).add_child("td", value);
            }
            else
            {
                return HtmlBuilder{"tr"}.add_child("td", title).add_child("td", std::to_string(value));
            }
        }
    };
}

#endif // __PROPERTY_H__
