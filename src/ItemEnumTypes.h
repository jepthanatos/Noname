#ifndef __ITEM_ENUM_TYPES_H__
#define __ITEM_ENUM_TYPES_H__

#include <string>
#include "Skill.h"

namespace noname
{
    enum class ItemType
    {
        NO_TYPE,
        AMULET,
        HELMET,
        CONTAINER,
        WEAPON,
        RING,
        ARMOR,
        SHIELD,
        LEGS_ARMOR,
        BOOTS,
        AMMUNITION,
        USABLE,
        LAST_ITEM_TYPE
    };

    enum class ItemRank
    {
        NO_RANK,
        NORMAL,
        MAGIC,
        RARE,
        EPIC,
        LEGENDARY,
        LAST_ITEM_RANK
    };

    enum class ItemSlotType
    {
        AMULET,
        HELMET,
        CONTAINER,
        WEAPON,
        RING_LEFT,
        ARMOR,
        SHIELD,
        RING_RIGHT,
        LEGS_ARMOR,
        BOOTS,
        AMMUNITION,
        LAST_SLOT_TYPE
    };

    static std::string ItemTypeToString(ItemType type)
    {
        switch (type)
        {
        case ItemType::NO_TYPE:
            return "NO_TYPE";
        case ItemType::AMULET:
            return "AMULET";
        case ItemType::HELMET:
            return "HELMET";
        case ItemType::CONTAINER:
            return "CONTAINER";
        case ItemType::WEAPON:
            return "WEAPON";
        case ItemType::RING:
            return "RING";
        case ItemType::ARMOR:
            return "ARMOR";
        case ItemType::SHIELD:
            return "SHIELD";
        case ItemType::LEGS_ARMOR:
            return "LEGS_ARMOR";
        case ItemType::BOOTS:
            return "BOOTS";
        case ItemType::AMMUNITION:
            return "AMMUNITION";
        case ItemType::USABLE:
            return "USABLE";
        default:
            return "UNKNOWN";
        }
    }

    static ItemType slotTypeToItemType(ItemSlotType slot)
    {
        switch (slot)
        {
        case ItemSlotType::AMULET:
            return ItemType::AMULET;
        case ItemSlotType::HELMET:
            return ItemType::HELMET;
        case ItemSlotType::CONTAINER:
            return ItemType::CONTAINER;
        case ItemSlotType::WEAPON:
            return ItemType::WEAPON;
        case ItemSlotType::RING_LEFT:
        case ItemSlotType::RING_RIGHT:
            return ItemType::RING;
        case ItemSlotType::ARMOR:
            return ItemType::ARMOR;
        case ItemSlotType::SHIELD:
            return ItemType::SHIELD;
        case ItemSlotType::LEGS_ARMOR:
            return ItemType::LEGS_ARMOR;
        case ItemSlotType::BOOTS:
            return ItemType::BOOTS;
        case ItemSlotType::AMMUNITION:
            return ItemType::AMMUNITION;
        default:
            return ItemType::NO_TYPE;
        }
    }

    inline ItemType skillTypeToItemType(SkillType skillType)
    {
        switch (skillType)
        {
        case SkillType::FIST:
        case SkillType::SWORD:
        case SkillType::AXE:
        case SkillType::CLUB:
        case SkillType::DISTANCE:
            return ItemType::WEAPON;
        case SkillType::SHIELDING:
            return ItemType::SHIELD;
        default:
            return ItemType::NO_TYPE;
        }
    }
}

#endif // __ITEM_ENUM_TYPES_H__