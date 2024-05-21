#ifndef __RELATIONSHIPS_H__
#define __RELATIONSHIPS_H__

// System includes
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
        SIBLING,
        MARRIED
    };

    class RelationshipBrowser
    {
    public:
        virtual std::vector<Character> find_all_children_of(const std::string &name) = 0;
        virtual std::vector<Character> find_parents_of(const std::string &name) = 0;
        virtual std::vector<Character> find_husband_of(const std::string &name) = 0;
        virtual std::vector<Character> find_wife_of(const std::string &name) = 0;
    };

    class Relationships : RelationshipBrowser
    {
    private:
        std::vector<std::tuple<Character, Relationship, Character>> relations;

    public:
        void add_parent_and_child(const Character &parent, const Character &child);
        void add_husband_and_wife(const Character &husband, const Character &wife);

        std::vector<Character> find_all_children_of(const std::string &name) override;
        std::vector<Character> find_parents_of(const std::string &name) override;
        std::vector<Character> find_husband_of(const std::string &name) override;
        std::vector<Character> find_wife_of(const std::string &name) override;
    };
}

#endif // __RELATIONSHIPS_H__