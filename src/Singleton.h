#ifndef __SINGLETON_H__
#define __SINGLETON_H__

template <class T>
class Singleton
{
public:
    // Delete the copy and move assignment operators
    Singleton(const Singleton &) = delete;
    Singleton(Singleton &&) = delete;
    Singleton &operator=(const Singleton &) = delete;
    Singleton &operator=(Singleton &&) = delete;

    // Get the instance of the singleton
    static T &getInstance()
    {
        static T instance;
        return instance;
    }

protected:
    // Provide a default implementation of the default constructor
    Singleton() = default;
};
#endif /* __SINGLETON_H__ */