#ifndef __ATTACK_STRATEGY_H__
#define __ATTACK_STRATEGY_H__

#include <memory>
#include "Skill.h"

namespace noname
{
    class Character;
    class Weapon;

    /**
     * @brief Interfaz base para estrategias de ataque
     * 
     * Define el contrato que deben cumplir todas las estrategias de combate.
     * Permite intercambiar algoritmos de ataque dinámicamente.
     */
    class AttackStrategy {
    public:
        virtual ~AttackStrategy() = default;
        
        /**
         * @brief Calcula el daño de un ataque
         * @param attacker El personaje que ataca
         * @param weapon El arma utilizada (puede ser nullptr)
         * @return Daño calculado (0 si el ataque falla)
         */
        virtual int calculateDamage(const Character& attacker, 
                                   const std::shared_ptr<Weapon>& weapon) const = 0;
        
        /**
         * @brief Obtiene el skill relevante para este tipo de ataque
         * @param weapon El arma utilizada
         * @return El tipo de skill que se debe actualizar
         */
        virtual SkillType getRelevantSkill(const std::shared_ptr<Weapon>& weapon) const = 0;
        
        /**
         * @brief Verifica si el último ataque fue exitoso
         * @return true si el ataque golpeó, false si falló
         */
        virtual bool isSuccessfulHit() const = 0;
        
        /**
         * @brief Verifica si el último ataque fue crítico
         * @return true si fue crítico, false en caso contrario
         */
        virtual bool isCriticalHit() const = 0;
        
        /**
         * @brief Obtiene el multiplicador de daño crítico
         * @return Multiplicador aplicado en golpes críticos
         */
        virtual int getCriticalMultiplier() const { return 2; }
        
        /**
         * @brief Obtiene el costo en mana de este tipo de ataque
         * @return Costo en mana (0 para ataques físicos)
         */
        virtual int getManaCost() const { return 0; }
    };
}

#endif // __ATTACK_STRATEGY_H__
