#ifndef __RANGED_ATTACK_STRATEGY_H__
#define __RANGED_ATTACK_STRATEGY_H__

#include "AttackStrategy.h"
#include "Character.h"
#include "Weapon.h"
#include "Utils.h"
#include "LogManager.h"

namespace noname
{
    /**
     * @brief Estrategia de ataque para combate a distancia
     * 
     * Maneja la lógica de ataques con armas de combate a distancia
     * como arcos, ballestas, y armas arrojadizas.
     */
    class RangedAttackStrategy : public AttackStrategy {
    public:
        int calculateDamage(const Character& attacker, 
                           const std::shared_ptr<Weapon>& weapon) const override {
            if (!weapon) {
                LM.writeLog(Level::Debug, "Cannot perform ranged attack without weapon");
                return 0; // No se puede atacar a distancia sin arma
            }
            
            // Roll para determinar si golpea (más difícil a distancia)
            int hitRoll = Utils::rollDie(1, 20);
            lastHitRoll = hitRoll;
            
            if (hitRoll <= 2) {
                LM.writeLog(Level::Debug, "Ranged attack missed with roll: " + std::to_string(hitRoll));
                return 0; // Miss más probable que en combate cuerpo a cuerpo
            }
            
            // Calcular daño base del arma
            int baseDamage = Utils::rollDie(1, weapon->getDie());
            
            // Aplicar críticos (más fácil para armas a distancia)
            if (hitRoll >= 19) {
                baseDamage *= getCriticalMultiplier();
                LM.writeLog(Level::Debug, "Critical ranged hit! Damage doubled");
            }
            
            // Agregar bonus de skill
            int skillBonus = attacker.getSkill(SkillType::DISTANCE);
            
            // Agregar bonus de destreza para combate a distancia
            int dexterityBonus = attacker.getHeritable(HeritableType::DEXTERY) / 10;
            
            int totalDamage = baseDamage + skillBonus + dexterityBonus;
            
            LM.writeLog(Level::Debug, "Ranged damage calculation: base(" + std::to_string(baseDamage) + 
                       ") + distance_skill(" + std::to_string(skillBonus) + 
                       ") + dexterity(" + std::to_string(dexterityBonus) + 
                       ") = " + std::to_string(totalDamage));
            
            return totalDamage;
        }
        
        SkillType getRelevantSkill(const std::shared_ptr<Weapon>& weapon) const override {
            return SkillType::DISTANCE;
        }
        
        bool isSuccessfulHit() const override {
            return lastHitRoll > 2;
        }
        
        bool isCriticalHit() const override {
            return lastHitRoll >= 19; // Más fácil hacer crítico a distancia
        }
        
        int getCriticalMultiplier() const override {
            return 3; // Los críticos a distancia son más devastadores
        }
        
    private:
        mutable int lastHitRoll = 0;
    };
}

#endif // __RANGED_ATTACK_STRATEGY_H__
