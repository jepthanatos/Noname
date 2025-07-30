#ifndef __MELEE_ATTACK_STRATEGY_H__
#define __MELEE_ATTACK_STRATEGY_H__

#include "AttackStrategy.h"
#include "Character.h"
#include "Weapon.h"
#include "Utils.h"
#include "LogManager.h"

namespace noname
{
    /**
     * @brief Estrategia de ataque para combate cuerpo a cuerpo
     * 
     * Maneja la lógica de ataques con armas de combate cuerpo a cuerpo
     * como espadas, hachas, mazas, y combate desarmado.
     */
    class MeleeAttackStrategy : public AttackStrategy {
    public:
        int calculateDamage(const Character& attacker, 
                           const std::shared_ptr<Weapon>& weapon) const override {
            if (!weapon) {
                return calculateUnarmedDamage(attacker);
            }
            
            // Roll para determinar si golpea
            int hitRoll = Utils::rollDie(1, 20);
            lastHitRoll = hitRoll;
            
            if (hitRoll <= 1) {
                LM.writeLog(Level::Debug, "Attack missed with roll: " + std::to_string(hitRoll));
                return 0; // Miss
            }
            
            // Calcular daño base del arma
            int baseDamage = Utils::rollDie(1, weapon->getDie());
            
            // Aplicar críticos
            if (hitRoll >= 20) {
                baseDamage *= getCriticalMultiplier();
                LM.writeLog(Level::Debug, "Critical hit! Damage doubled");
            }
            
            // Agregar bonus de skill
            SkillType skill = getRelevantSkill(weapon);
            int skillBonus = attacker.getSkill(skill);
            
            // Agregar bonus de fuerza para combate cuerpo a cuerpo
            int strengthBonus = attacker.getHeritable(HeritableType::STRENGTH) / 10;
            
            int totalDamage = baseDamage + skillBonus + strengthBonus;
            
            LM.writeLog(Level::Debug, "Melee damage calculation: base(" + std::to_string(baseDamage) + 
                       ") + skill(" + std::to_string(skillBonus) + 
                       ") + strength(" + std::to_string(strengthBonus) + 
                       ") = " + std::to_string(totalDamage));
            
            return totalDamage;
        }
        
        SkillType getRelevantSkill(const std::shared_ptr<Weapon>& weapon) const override {
            if (!weapon) return SkillType::FIST;
            return weapon->getSkillType();
        }
        
        bool isSuccessfulHit() const override {
            return lastHitRoll > 1;
        }
        
        bool isCriticalHit() const override {
            return lastHitRoll >= 20;
        }
        
    private:
        mutable int lastHitRoll = 0; // Para recordar el último roll
        
        /**
         * @brief Calcula daño para combate desarmado
         * @param attacker El personaje que ataca
         * @return Daño calculado
         */
        int calculateUnarmedDamage(const Character& attacker) const {
            int roll = Utils::rollDie(1, 20);
            lastHitRoll = roll;
            
            if (roll <= 1) {
                LM.writeLog(Level::Debug, "Unarmed attack missed with roll: " + std::to_string(roll));
                return 0;
            }
            
            // Puños hacen 1d4 de daño base
            int baseDamage = Utils::rollDie(1, 4);
            
            // Aplicar críticos
            if (roll >= 20) {
                baseDamage *= getCriticalMultiplier();
                LM.writeLog(Level::Debug, "Critical unarmed hit!");
            }
            
            int skillBonus = attacker.getSkill(SkillType::FIST);
            int strengthBonus = attacker.getHeritable(HeritableType::STRENGTH) / 10;
            
            int totalDamage = baseDamage + skillBonus + strengthBonus;
            
            LM.writeLog(Level::Debug, "Unarmed damage: base(" + std::to_string(baseDamage) + 
                       ") + fist_skill(" + std::to_string(skillBonus) + 
                       ") + strength(" + std::to_string(strengthBonus) + 
                       ") = " + std::to_string(totalDamage));
            
            return totalDamage;
        }
    };
}

#endif // __MELEE_ATTACK_STRATEGY_H__
