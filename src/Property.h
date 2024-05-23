#ifndef __PROPERTY_H__
#define __PROPERTY_H__

// System includes
#include <string>

// Local includes
namespace noname
{
    template <typename T>
    struct Property
    {
        T value;
        // Property() {}
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
            return std::to_string(value);
        }
    };
}
#endif // __PROPERTY_H__
