#ifndef __EVENT_TYPES_H__
#define __EVENT_TYPES_H__

namespace noname
{
    /**
     * @brief Enumeración de tipos de eventos del sistema
     * 
     * Define todos los tipos de eventos que pueden ocurrir en el juego
     * y que pueden ser observados por diferentes sistemas.
     */
    enum class EventType {
        // Eventos de combate
        DAMAGE_TAKEN,
        DAMAGE_DEALT,
        COMBAT_STARTED,
        COMBAT_ENDED,
        
        // Eventos de salud y mana
        HEALTH_CHANGED,
        MANA_CHANGED,
        
        // Eventos de experiencia y niveles
        EXPERIENCE_GAINED,
        LEVEL_UP,
        MAGIC_LEVEL_UP,
        
        // Eventos de estado del personaje
        CHARACTER_DIED,
        CHARACTER_RESPAWNED,
        
        // Eventos de inventario
        ITEM_EQUIPPED,
        ITEM_UNEQUIPPED,
        ITEM_PICKED_UP,
        ITEM_DROPPED,
        
        // Eventos de habilidades
        SKILL_IMPROVED,
        SKILL_USED,
        
        // Eventos de propiedades heredables
        STRENGTH_CHANGED,
        DEXTERITY_CHANGED,
        INTELLIGENCE_CHANGED,
        CONSTITUTION_CHANGED
    };
    
    /**
     * @brief Convierte un EventType a string para logging
     */
    inline const char* eventTypeToString(EventType type) {
        switch (type) {
            case EventType::DAMAGE_TAKEN: return "DAMAGE_TAKEN";
            case EventType::DAMAGE_DEALT: return "DAMAGE_DEALT";
            case EventType::COMBAT_STARTED: return "COMBAT_STARTED";
            case EventType::COMBAT_ENDED: return "COMBAT_ENDED";
            case EventType::HEALTH_CHANGED: return "HEALTH_CHANGED";
            case EventType::MANA_CHANGED: return "MANA_CHANGED";
            case EventType::EXPERIENCE_GAINED: return "EXPERIENCE_GAINED";
            case EventType::LEVEL_UP: return "LEVEL_UP";
            case EventType::MAGIC_LEVEL_UP: return "MAGIC_LEVEL_UP";
            case EventType::CHARACTER_DIED: return "CHARACTER_DIED";
            case EventType::CHARACTER_RESPAWNED: return "CHARACTER_RESPAWNED";
            case EventType::ITEM_EQUIPPED: return "ITEM_EQUIPPED";
            case EventType::ITEM_UNEQUIPPED: return "ITEM_UNEQUIPPED";
            case EventType::ITEM_PICKED_UP: return "ITEM_PICKED_UP";
            case EventType::ITEM_DROPPED: return "ITEM_DROPPED";
            case EventType::SKILL_IMPROVED: return "SKILL_IMPROVED";
            case EventType::SKILL_USED: return "SKILL_USED";
            case EventType::STRENGTH_CHANGED: return "STRENGTH_CHANGED";
            case EventType::DEXTERITY_CHANGED: return "DEXTERITY_CHANGED";
            case EventType::INTELLIGENCE_CHANGED: return "INTELLIGENCE_CHANGED";
            case EventType::CONSTITUTION_CHANGED: return "CONSTITUTION_CHANGED";
            default: return "UNKNOWN_EVENT";
        }
    }
}

#endif // __EVENT_TYPES_H__
