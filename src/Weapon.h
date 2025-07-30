#ifndef __WEAPON_H__
#define __WEAPON_H__

// System includes
#include <string>
#include <memory>

// Local includes
#include "Skill.h"
#include "Item.h"
#include "ItemEnumTypes.h"
#include "Property.h"

namespace noname
{
    class Weapon : public Item
    {
    private:
        SkillType _skillType;
        Property<short> _die;
        WeaponHandedness _handedness;

    public:
        // Constructor que mantiene compatibilidad hacia atrás
        Weapon(const std::string &name,
               SkillType skillType,
               ItemRank rank = ItemRank::NORMAL,
               short die = 0,
               std::optional<short> value = std::nullopt,
               std::optional<short> uses = std::nullopt,
               std::optional<short> weight = std::nullopt)
            : Item{name, skillTypeToItemType(skillType), rank, value, uses, weight}, 
              _skillType{skillType}, _die{die}, _handedness{WeaponHandedness::ONE_HAND} {}

        // Constructor con especificación de handedness
        Weapon(const std::string &name,
               SkillType skillType,
               ItemRank rank,
               short die,
               WeaponHandedness handedness,
               std::optional<short> value = std::nullopt,
               std::optional<short> uses = std::nullopt,
               std::optional<short> weight = std::nullopt)
            : Item{name, skillTypeToItemType(skillType), rank, value, uses, weight}, 
              _skillType{skillType}, _die{die}, _handedness{handedness} {}

        SkillType getSkillType() const { return _skillType; }
        short getDie() const { return _die; }
        WeaponHandedness getHandedness() const { return _handedness; }
        bool isTwoHanded() const { return _handedness == WeaponHandedness::TWO_HAND; }
    };

    class NullWeapon
    {
    public:
        static std::shared_ptr<Weapon> getInstance()
        {
            static std::shared_ptr<Weapon> instance{new Weapon("Null Weapon", SkillType::AXE, ItemRank::NORMAL, 0, WeaponHandedness::ONE_HAND)};
            return instance;
        }

    private:
        NullWeapon(const NullWeapon &) = delete;
        NullWeapon(NullWeapon &&) = delete;
        NullWeapon &operator=(const NullWeapon &) = delete;
        NullWeapon &operator=(NullWeapon &&) = delete;
    };
}

#endif // __WEAPON_H__
