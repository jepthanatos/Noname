#include "ItemTemplate.h"
#include "Character.h"

namespace noname
{
    bool WeaponTemplate::canBeUsedBy(const Character& character) const 
    {
        // Verificar nivel de skill requerido
        if (character.getSkill(requiredSkill_) < minRequiredSkillLevel_) {
            return false;
        }
        
        // Verificar si el character no está muerto
        if (character.isDead()) {
            return false;
        }
        
        return true;
    }
    
    bool ArmorTemplate::canBeUsedBy(const Character& character) const 
    {
        // Verificar nivel requerido
        if (character.getLevel() < minRequiredLevel_) {
            return false;
        }
        
        // Verificar si el character no está muerto
        if (character.isDead()) {
            return false;
        }
        
        return true;
    }
}
