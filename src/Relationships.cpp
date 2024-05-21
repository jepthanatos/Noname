// Local includes
#include "Relationships.h"

namespace noname
{
    void Relationships::add_parent_and_child(const Character &parent, const Character &child)
    {
        relations.push_back({parent, Relationship::PARENT, child});
        relations.push_back({child, Relationship::CHILD, parent});
    }

    void Relationships::add_husband_and_wife(const Character &husband, const Character &wife)
    {
        relations.push_back({husband, Relationship::MARRIED, wife});
    }

    std::vector<Character> Relationships::find_all_children_of(const std::string &name)
    {
        std::vector<Character> result;
        for (auto &&[first, rel, second] : relations)
        {
            if (first.getName() == name && rel == Relationship::PARENT)
            {
                result.push_back(second);
            }
        }
        return result;
    }

    std::vector<Character> Relationships::find_parents_of(const std::string &name)
    {
        std::vector<Character> result;
        for (auto &&[first, rel, second] : relations)
        {
            if (first.getName() == name && rel == Relationship::CHILD)
            {
                result.push_back(second);
            }
        }
        return result;
    }

    std::vector<Character> Relationships::find_husband_of(const std::string &name)
    {
        std::vector<Character> result;
        for (auto &&[first, rel, second] : relations)
        {
            if (second.getName() == name && rel == Relationship::MARRIED)
            {
                result.push_back(first);
            }
        }
        return result;
    }

    std::vector<Character> Relationships::find_wife_of(const std::string &name)
    {
        std::vector<Character> result;
        for (auto &&[first, rel, second] : relations)
        {
            if (first.getName() == name && rel == Relationship::MARRIED)
            {
                result.push_back(second);
            }
        }
        return result;
    }
}