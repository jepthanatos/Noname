#ifndef __PROPERTY_H__
#define __PROPERTY_H__

// Local includes
#include "HtmlBuilder.h"

// System includes
#include <string>
#include <string_view>
#include <type_traits>

namespace noname
{
    template <typename T>
    class Property
    {
    private:
        T value;

    public:
        Property(const T initialValue)
        {
            *this = initialValue;
        }
        operator T() const
        {
            return value;
        }
        T operator=(T newValue)
        {
            return value = newValue;
        }
        bool operator>(T rValue)
        {
            return value > rValue;
        }
        T operator++()
        {
            return value++;
        }
        T operator--()
        {
            return value--;
        }
        T operator+=(T &rightValue)
        {
            return value += rightValue;
        }
        T operator-=(T &rightValue)
        {
            return value -= rightValue;
        }
        std::string toString() const
        {
            if constexpr (std::is_convertible_v<T, std::string> or
                          std::is_convertible_v<T, std::string_view>)
                return value;
            else
                return std::to_string(value);
        }
        HtmlBuilder toHtmlBuilder(const std::string &title)
        {
            if constexpr (std::is_convertible_v<T, std::string> or
                          std::is_convertible_v<T, std::string_view>)
                return HtmlBuilder{"tr"}.add_child("td", title).add_child("td", value);
            else
                return HtmlBuilder{"tr"}.add_child("td", title).add_child("td", std::to_string(value));
        }
    };
}
#endif // __PROPERTY_H__
