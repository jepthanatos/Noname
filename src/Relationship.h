#ifndef __RELATIONSHIP_H__
#define __RELATIONSHIP_H__

// System includes
#include <iostream>
#include <string>
#include <vector>
#include <tuple>

// Local includes
#include "Character.h"

namespace noname
{
    enum class Relationship
    {
        PARENT,
        CHILD,
        SIBLING
    };

    class RelationshipBrowser
    {
    public:
        virtual std::vector<Character> find_all_children_of(const std::string &name) = 0;
    };

    class Relationships : RelationshipBrowser
    {
    private:
        std::vector<tuple<Character, Relationship, Character>> relations;

    public:
        void add_parent_and_child(const Character &parent, const Character &child)
        {
            relations.push_back({parent, Relationship::PARENT, child});
            relations.push_back({child, Relationship::CHILD, parent});
        }

        std::vector<Character> find_all_children_of(const std::string &name) override
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
    };
}

#endif // __RELATIONSHIP_H__