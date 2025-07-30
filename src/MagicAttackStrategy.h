#ifndef __MAGIC_ATTACK_STRATEGY_H__
#define __MAGIC_ATTACK_STRATEGY_H__

#include "AttackStrategy.h"
#include "Character.h"
#include "Weapon.h"
#include "Utils.h"
#include "LogManager.h"

namespace noname
{
    /**
     * @brief Estrategia de ataque para combate mágico
     * 
     * Maneja la lógica de ataques mágicos que consumen mana
     * y se basan en el nivel mágico e inteligencia del personaje.
     */
    class MagicAttackStrategy : public AttackStrategy {
    public:
        explicit MagicAttackStrategy(int manaCost = 10) : manaCost_(manaCost) {}
        
        int calculateDamage(const Character& attacker, 
                           const std::shared_ptr<Weapon>& weapon) const override {
            // Verificar si hay suficiente mana
            if (attacker.getCurrentMana() < manaCost_) {
                LM.writeLog(Level::Debug, "Not enough mana for magic attack. Required: " + 
                           std::to_string(manaCost_) + ", Available: " + 
                           std::to_string(attacker.getCurrentMana()));
                return 0;
            }
            
            lastHitRoll = 20; // Los ataques mágicos siempre golpean
            
            // Daño basado en nivel mágico (más niveles = más dados)
            int magicLevel = attacker.getMagicLevel();
            int baseDamage = 0;
            
            // Tirar múltiples dados basados en el nivel mágico
            for (int i = 0; i < magicLevel; ++i) {
                baseDamage += Utils::rollDie(1, 6);
            }
            
            // Agregar bonus de inteligencia
            int intelligenceBonus = attacker.getHeritable(HeritableType::INTELLIGENCE) / 5;
            
            // Bonus adicional si se usa un arma mágica
            int weaponBonus = 0;
            if (weapon && weapon->getItemType() == ItemType::WEAPON) {
                // Las armas mágicas pueden tener rank que afecte el daño
                weaponBonus = static_cast<int>(weapon->getItemType()) * 2;
            }
            
            int totalDamage = baseDamage + intelligenceBonus + weaponBonus;
            
            LM.writeLog(Level::Debug, "Magic damage calculation: base(" + std::to_string(baseDamage) + 
                       ") + intelligence(" + std::to_string(intelligenceBonus) + 
                       ") + weapon_bonus(" + std::to_string(weaponBonus) + 
                       ") = " + std::to_string(totalDamage) + 
                       " [Mana cost: " + std::to_string(manaCost_) + "]");
            
            return totalDamage;
        }
        
        SkillType getRelevantSkill(const std::shared_ptr<Weapon>& weapon) const override {
            return SkillType::LAST_SKILL; // Los hechizos no usan skills de armas
        }
        
        bool isSuccessfulHit() const override {
            return true; // Los hechizos siempre golpean si hay mana suficiente
        }
        
        bool isCriticalHit() const override {
            return false; // Los hechizos no hacen críticos por defecto
        }
        
        int getCriticalMultiplier() const override {
            return 1; // Sin críticos
        }
        
        int getManaCost() const override {
            return manaCost_;
        }
        
        /**
         * @brief Establece el costo en mana de este hechizo
         * @param cost Nuevo costo en mana
         */
        void setManaCost(int cost) {
            manaCost_ = cost;
        }
        
    private:
        int manaCost_;
        mutable int lastHitRoll = 20; // Los hechizos siempre "golpean"
    };
    
    /**
     * @brief Estrategia de ataque mágico con críticos
     * 
     * Variante de ataque mágico que puede hacer críticos basados en inteligencia
     */
    class CriticalMagicAttackStrategy : public MagicAttackStrategy {
    public:
        explicit CriticalMagicAttackStrategy(int manaCost = 15) : MagicAttackStrategy(manaCost) {}
        
        int calculateDamage(const Character& attacker, 
                           const std::shared_ptr<Weapon>& weapon) const override {
            // Usar lógica base de MagicAttackStrategy
            int baseDamage = MagicAttackStrategy::calculateDamage(attacker, weapon);
            
            if (baseDamage > 0) {
                // Posibilidad de crítico basada en inteligencia
                int intelligence = attacker.getHeritable(HeritableType::INTELLIGENCE);
                int criticalChance = intelligence / 10; // 1% por cada 10 puntos de inteligencia
                
                int roll = Utils::rollDie(1, 100);
                if (roll <= criticalChance) {
                    lastWasCritical = true;
                    baseDamage *= 2;
                    LM.writeLog(Level::Debug, "Critical magic attack! Damage doubled (roll: " + 
                               std::to_string(roll) + " vs " + std::to_string(criticalChance) + "%)");
                } else {
                    lastWasCritical = false;
                }
            }
            
            return baseDamage;
        }
        
        bool isCriticalHit() const override {
            return lastWasCritical;
        }
        
        int getCriticalMultiplier() const override {
            return 2;
        }
        
    private:
        mutable bool lastWasCritical = false;
    };
}

#endif // __MAGIC_ATTACK_STRATEGY_H__
