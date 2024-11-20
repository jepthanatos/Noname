#ifndef __ITEM_ENUM_TYPES_H__
#define __ITEM_ENUM_TYPES_H__

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
        ARMOR,
        SHIELD,
        RING,
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
        RIGHT_RING,
        ARMOR,
        SHIELD,
        LEFT_RING,
        LEGS_ARMOR,
        BOOTS,
        AMMUNITION,
        LAST_SLOT_TYPE
    };

    inline ItemType slotTypeToItemType(ItemSlotType itemSlotType)
    {
        switch (itemSlotType)
        {
        case ItemSlotType::AMULET:
            return ItemType::AMULET;
        case ItemSlotType::HELMET:
            return ItemType::HELMET;
        case ItemSlotType::CONTAINER:
            return ItemType::CONTAINER;
        case ItemSlotType::WEAPON:
            return ItemType::WEAPON;
        case ItemSlotType::RIGHT_RING:
            return ItemType::RING;
        case ItemSlotType::LEFT_RING:
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