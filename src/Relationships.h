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
        MARRIED,
        ENEMY
    };

    class RelationshipBrowser
    {
    public:
        virtual std::vector<Character> find_all_children_of(const std::string &name) = 0;
        virtual std::vector<Character> find_parents_of(const std::string &name) = 0;
        virtual std::vector<Character> find_husband_of(const std::string &name) = 0;
        virtual std::vector<Character> find_wife_of(const std::string &name) = 0;
        virtual std::vector<Character> find_prey_of(const std::string &name) = 0;
        virtual std::vector<Character> find_predator_of(const std::string &name) = 0;
    };

    class Relationships : RelationshipBrowser
    {
    private:
        std::vector<std::tuple<Character, Relationship, Character>> _familyRelations;
        std::vector<std::tuple<Character, Relationship, Character>> _predatoryRelations;

    public:
        void add_parent_and_child(const Character &parent, const Character &child);
        void add_husband_and_wife(const Character &husband, const Character &wife);
        void add_predator_and_prey(const Character &predator, const Character &prey);

        std::vector<Character> find_all_children_of(const std::string &name) override;
        std::vector<Character> find_parents_of(const std::string &name) override;
        std::vector<Character> find_husband_of(const std::string &name) override;
        std::vector<Character> find_wife_of(const std::string &name) override;
        std::vector<Character> find_prey_of(const std::string &name) override;
        std::vector<Character> find_predator_of(const std::string &name) override;
    };
}

#endif // __RELATIONSHIPS_H__