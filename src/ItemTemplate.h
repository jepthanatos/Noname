#ifndef __ITEM_TEMPLATE_H__
#define __ITEM_TEMPLATE_H__

#include "ItemEnumTypes.h"
#include "Skill.h"
#include <string>

namespace noname
{
    // Forward declaration
    class Character;

    /**
     * @brief Interfaz base para templates de items (Flyweight pattern)
     * 
     * Los templates almacenan el estado intrínseco (compartido entre items del mismo tipo).
     * El estado extrínseco (único por instancia) se pasa como parámetros a los métodos.
     */
    class ItemTemplate 
    {
    protected:
        ItemType type_;
        std::string name_;
        std::string description_;
        int baseValue_;
        ItemSlotType slotType_;
        
    public:
        ItemTemplate(ItemType type, std::string name, std::string description, 
                    int baseValue, ItemSlotType slotType)
            : type_(type), name_(std::move(name)), description_(std::move(description))
            , baseValue_(baseValue), slotType_(slotType) {}
        
        virtual ~ItemTemplate() = default;
        
        // Getters para estado intrínseco
        ItemType getType() const { return type_; }
        const std::string& getName() const { return name_; }
        const std::string& getDescription() const { return description_; }
        int getBaseValue() const { return baseValue_; }
        ItemSlotType getSlotType() const { return slotType_; }
        
        // Métodos virtuales que operan con estado extrínseco
        virtual int calculateCurrentValue(int durability) const {
            return static_cast<int>(baseValue_ * (durability / 100.0));
        }
        
        virtual bool canBeUsedBy(const Character& character) const = 0;
        
        virtual std::string getDisplayName(int enhancement = 0) const {
            if (enhancement > 0) {
                return name_ + " +" + std::to_string(enhancement);
            }
            return name_;
        }
        
        // Método para obtener información detallada
        virtual std::string getDetailedInfo(int durability, int enhancement = 0) const {
            std::string info = getDisplayName(enhancement) + "\n";
            info += description_ + "\n";
            info += "Value: " + std::to_string(calculateCurrentValue(durability)) + " gold\n";
            info += "Durability: " + std::to_string(durability) + "%";
            return info;
        }
    };

    /**
     * @brief Template para armas (Concrete Flyweight)
     */
    class WeaponTemplate : public ItemTemplate 
    {
    private:
        int baseDamage_;
        int dieSize_;
        SkillType requiredSkill_;
        int minRequiredSkillLevel_;
        
    public:
        WeaponTemplate(ItemType type, std::string name, std::string description,
                      int baseValue, int baseDamage, int dieSize, SkillType requiredSkill,
                      int minSkillLevel = 1)
            : ItemTemplate(type, std::move(name), std::move(description), baseValue, ItemSlotType::WEAPON)
            , baseDamage_(baseDamage), dieSize_(dieSize), requiredSkill_(requiredSkill)
            , minRequiredSkillLevel_(minSkillLevel) {}
        
        // Getters específicos de armas
        int getBaseDamage() const { return baseDamage_; }
        int getDieSize() const { return dieSize_; }
        SkillType getRequiredSkill() const { return requiredSkill_; }
        int getMinRequiredSkillLevel() const { return minRequiredSkillLevel_; }
        
        // Cálculos que dependen del estado extrínseco
        int calculateDamage(int enhancement = 0, int skillLevel = 1) const {
            int damage = baseDamage_ + enhancement;
            // Bonus por skill level
            damage += skillLevel / 10; // +1 damage por cada 10 levels de skill
            return damage;
        }
        
        bool canBeUsedBy(const Character& character) const override;
        
        std::string getDetailedInfo(int durability, int enhancement = 0) const override {
            std::string info = ItemTemplate::getDetailedInfo(durability, enhancement);
            info += "\nDamage: " + std::to_string(baseDamage_);
            if (enhancement > 0) {
                info += " (+" + std::to_string(enhancement) + " enhanced)";
            }
            info += "\nDie: d" + std::to_string(dieSize_);
            info += "\nRequired Skill: " + std::to_string(static_cast<int>(requiredSkill_));
            return info;
        }
    };

    /**
     * @brief Template para armaduras (Concrete Flyweight)
     */
    class ArmorTemplate : public ItemTemplate 
    {
    private:
        int baseDefense_;
        int minRequiredLevel_;
        
    public:
        ArmorTemplate(ItemType type, std::string name, std::string description,
                     int baseValue, ItemSlotType slotType, int baseDefense, int minLevel = 1)
            : ItemTemplate(type, std::move(name), std::move(description), baseValue, slotType)
            , baseDefense_(baseDefense), minRequiredLevel_(minLevel) {}
        
        // Getters específicos de armadura
        int getBaseDefense() const { return baseDefense_; }
        int getMinRequiredLevel() const { return minRequiredLevel_; }
        
        // Cálculos que dependen del estado extrínseco
        int calculateDefense(int enhancement = 0) const {
            return baseDefense_ + enhancement;
        }
        
        bool canBeUsedBy(const Character& character) const override;
        
        std::string getDetailedInfo(int durability, int enhancement = 0) const override {
            std::string info = ItemTemplate::getDetailedInfo(durability, enhancement);
            info += "\nDefense: " + std::to_string(baseDefense_);
            if (enhancement > 0) {
                info += " (+" + std::to_string(enhancement) + " enhanced)";
            }
            info += "\nRequired Level: " + std::to_string(minRequiredLevel_);
            return info;
        }
    };

    /**
     * @brief Template para consumibles (Concrete Flyweight)
     */
    class ConsumableTemplate : public ItemTemplate 
    {
    private:
        int healingAmount_;
        int manaAmount_;
        bool isSingleUse_;
        
    public:
        ConsumableTemplate(ItemType type, std::string name, std::string description,
                          int baseValue, int healingAmount = 0, int manaAmount = 0, 
                          bool singleUse = true)
            : ItemTemplate(type, std::move(name), std::move(description), baseValue, ItemSlotType::AMMUNITION)
            , healingAmount_(healingAmount), manaAmount_(manaAmount), isSingleUse_(singleUse) {}
        
        // Getters específicos de consumibles
        int getHealingAmount() const { return healingAmount_; }
        int getManaAmount() const { return manaAmount_; }
        bool isSingleUse() const { return isSingleUse_; }
        
        bool canBeUsedBy(const Character& character) const override {
            // Los consumibles generalmente pueden ser usados por cualquiera
            return true;
        }
        
        std::string getDetailedInfo(int durability, int enhancement = 0) const override {
            std::string info = ItemTemplate::getDetailedInfo(durability, enhancement);
            if (healingAmount_ > 0) {
                info += "\nHealing: +" + std::to_string(healingAmount_) + " HP";
            }
            if (manaAmount_ > 0) {
                info += "\nMana: +" + std::to_string(manaAmount_) + " MP";
            }
            info += isSingleUse_ ? "\nSingle use" : "\nReusable";
            return info;
        }
    };
}

#endif // __ITEM_TEMPLATE_H__
