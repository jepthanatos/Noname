#ifndef __RELATIONSHIPS_H__
#define __RELATIONSHIPS_H__

// System includes
#include <string>
#include <vector>
#include <tuple>

namespace noname
{
    enum class Relationship
    {
        PARENT,
        CHILD,
        SIBLING,
        MARRIED,
        ENEMY
    };

    template <class T>
    class Relationships
    {
    private:
        std::vector<std::tuple<T, Relationship, T>> _familyRelations;
        std::vector<std::tuple<T, Relationship, T>> _predatoryRelations;

    public:
        void add_parent_and_child(const T &parent, const T &child);
        void add_husband_and_wife(const T &husband, const T &wife);
        void add_predator_and_prey(const T &predator, const T &prey);

        std::vector<T> find_all_children_of(const std::string &name);
        std::vector<T> find_parents_of(const std::string &name);
        std::vector<T> find_husband_of(const std::string &name);
        std::vector<T> find_wife_of(const std::string &name);
        std::vector<T> find_prey_of(const std::string &name);
        std::vector<T> find_predator_of(const std::string &name);
    };
}

namespace noname
{
    template <class T>
    void Relationships<T>::add_parent_and_child(const T &parent, const T &child)
    {
        _familyRelations.push_back({parent, Relationship::PARENT, child});
        _familyRelations.push_back({child, Relationship::CHILD, parent});
    }

    template <class T>
    void Relationships<T>::add_husband_and_wife(const T &husband, const T &wife)
    {
        _familyRelations.push_back({husband, Relationship::MARRIED, wife});
    }

    template <class T>
    void Relationships<T>::add_predator_and_prey(const T &predator, const T &prey)
    {
        _predatoryRelations.push_back({predator, Relationship::ENEMY, prey});
    }

    template <class T>
    std::vector<T> Relationships<T>::find_all_children_of(const std::string &name)
    {
        std::vector<T> result;
        for (auto &&[first, rel, second] : _familyRelations)
        {
            if (first.getName() == name && rel == Relationship::PARENT)
            {
                result.push_back(second);
            }
        }
        return result;
    }

    template <class T>
    std::vector<T> Relationships<T>::find_parents_of(const std::string &name)
    {
        std::vector<T> result;
        for (auto &&[first, rel, second] : _familyRelations)
        {
            if (first.getName() == name && rel == Relationship::CHILD)
            {
                result.push_back(second);
            }
        }
        return result;
    }

    template <class T>
    std::vector<T> Relationships<T>::find_husband_of(const std::string &name)
    {
        std::vector<T> result;
        for (auto &&[first, rel, second] : _familyRelations)
        {
            if (second.getName() == name && rel == Relationship::MARRIED)
            {
                result.push_back(first);
            }
        }
        return result;
    }

    template <class T>
    std::vector<T> Relationships<T>::find_wife_of(const std::string &name)
    {
        std::vector<T> result;
        for (auto &&[first, rel, second] : _familyRelations)
        {
            if (first.getName() == name && rel == Relationship::MARRIED)
            {
                result.push_back(second);
            }
        }
        return result;
    }

    template <class T>
    std::vector<T> Relationships<T>::find_prey_of(const std::string &name)
    {
        std::vector<T> result;
        for (auto &&[first, rel, second] : _predatoryRelations)
        {
            if (first.getName() == name && rel == Relationship::ENEMY)
            {
                result.push_back(second);
            }
        }
        return result;
    }

    template <class T>
    std::vector<T> Relationships<T>::find_predator_of(const std::string &name)
    {
        std::vector<T> result;
        for (auto &&[first, rel, second] : _predatoryRelations)
        {
            if (second.getName() == name && rel == Relationship::ENEMY)
            {
                result.push_back(first);
            }
        }
        return result;
    }
}

#endif // __RELATIONSHIPS_H__