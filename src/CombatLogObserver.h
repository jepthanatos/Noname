#ifndef __COMBAT_LOG_OBSERVER_H__
#define __COMBAT_LOG_OBSERVER_H__

#include "EventObserver.h"
#include "LogManager.h"
#include <string>

namespace noname
{
    /**
     * @brief Observer que registra eventos de combate en el log
     * 
     * Se encarga de escribir mensajes descriptivos en el log
     * para eventos relacionados con combate y estado de personajes.
     */
    class CombatLogObserver : public EventObserver {
    private:
        bool verboseLogging_;
        
    public:
        /**
         * @brief Constructor
         * @param verboseLogging Si true, registra más detalles
         */
        explicit CombatLogObserver(bool verboseLogging = false) 
            : verboseLogging_(verboseLogging) {}
        
        void onEvent(const Event& event) override {
            switch (event.getType()) {
                case EventType::DAMAGE_DEALT: {
                    logDamageDealt(event);
                    break;
                }
                
                case EventType::DAMAGE_TAKEN: {
                    logDamageTaken(event);
                    break;
                }
                
                case EventType::COMBAT_STARTED: {
                    logCombatStarted(event);
                    break;
                }
                
                case EventType::COMBAT_ENDED: {
                    logCombatEnded(event);
                    break;
                }
                
                case EventType::CHARACTER_DIED: {
                    logCharacterDied(event);
                    break;
                }
                
                case EventType::LEVEL_UP: {
                    logLevelUp(event);
                    break;
                }
                
                case EventType::MAGIC_LEVEL_UP: {
                    logMagicLevelUp(event);
                    break;
                }
                
                case EventType::EXPERIENCE_GAINED: {
                    if (verboseLogging_) {
                        logExperienceGained(event);
                    }
                    break;
                }
                
                case EventType::HEALTH_CHANGED: {
                    if (verboseLogging_) {
                        logHealthChanged(event);
                    }
                    break;
                }
                
                default:
                    // Este observer no maneja otros tipos de eventos
                    break;
            }
        }
        
        std::string getObserverId() const override {
            return "CombatLogObserver";
        }
        
        bool handlesEventType(EventType eventType) const override {
            switch (eventType) {
                case EventType::DAMAGE_DEALT:
                case EventType::DAMAGE_TAKEN:
                case EventType::COMBAT_STARTED:
                case EventType::COMBAT_ENDED:
                case EventType::CHARACTER_DIED:
                case EventType::LEVEL_UP:
                case EventType::MAGIC_LEVEL_UP:
                    return true;
                    
                case EventType::EXPERIENCE_GAINED:
                case EventType::HEALTH_CHANGED:
                    return verboseLogging_;
                    
                default:
                    return false;
            }
        }
        
        int getPriority() const override {
            return 50; // Prioridad alta para logging
        }
        
        /**
         * @brief Activa/desactiva logging verboso
         */
        void setVerboseLogging(bool verbose) {
            verboseLogging_ = verbose;
        }
        
        bool isVerboseLoggingEnabled() const {
            return verboseLogging_;
        }
        
    private:
        void logDamageDealt(const Event& event) {
            try {
                auto attacker = event.getData<std::string>("attacker");
                auto target = event.getData<std::string>("target");
                auto damage = event.getData<int>("damage");
                
                std::string message = attacker + " deals " + std::to_string(damage) + 
                                    " damage to " + target;
                
                // Agregar información adicional si está disponible
                if (event.hasData("critical") && event.getData<bool>("critical")) {
                    message += " (CRITICAL HIT!)";
                }
                
                if (event.hasData("weapon_type")) {
                    auto weapon = event.getData<std::string>("weapon_type");
                    message += " with " + weapon;
                }
                
                LM.writeLog(Level::Info, message);
                
            } catch (const std::exception& e) {
                LM.writeLog(Level::Error, "CombatLogObserver: Error processing DAMAGE_DEALT event: " + std::string(e.what()));
            }
        }
        
        void logDamageTaken(const Event& event) {
            try {
                auto character = event.getData<std::string>("character_name");
                auto damage = event.getData<int>("damage");
                
                std::string message = character + " takes " + std::to_string(damage) + " damage";
                
                if (event.hasData("remaining_health")) {
                    auto health = event.getData<int>("remaining_health");
                    message += " (" + std::to_string(health) + " HP remaining)";
                }
                
                LM.writeLog(Level::Info, message);
                
            } catch (const std::exception& e) {
                LM.writeLog(Level::Error, "CombatLogObserver: Error processing DAMAGE_TAKEN event: " + std::string(e.what()));
            }
        }
        
        void logCombatStarted(const Event& event) {
            try {
                auto attacker = event.getData<std::string>("attacker");
                auto defender = event.getData<std::string>("defender");
                
                LM.writeLog(Level::Info, "=== COMBAT: " + attacker + " vs " + defender + " ===");
                
            } catch (const std::exception& e) {
                LM.writeLog(Level::Error, "CombatLogObserver: Error processing COMBAT_STARTED event: " + std::string(e.what()));
            }
        }
        
        void logCombatEnded(const Event& event) {
            try {
                auto attacker = event.getData<std::string>("attacker");
                auto defender = event.getData<std::string>("defender");
                auto hit = event.getDataOrDefault<bool>("hit", false);
                
                std::string message = "=== COMBAT END: " + attacker;
                if (hit) {
                    if (event.hasData("damage")) {
                        auto damage = event.getData<int>("damage");
                        message += " hit " + defender + " for " + std::to_string(damage) + " damage";
                    } else {
                        message += " hit " + defender;
                    }
                } else {
                    message += " missed " + defender;
                }
                message += " ===";
                
                LM.writeLog(Level::Info, message);
                
            } catch (const std::exception& e) {
                LM.writeLog(Level::Error, "CombatLogObserver: Error processing COMBAT_ENDED event: " + std::string(e.what()));
            }
        }
        
        void logCharacterDied(const Event& event) {
            try {
                auto characterName = event.getData<std::string>("character_name");
                auto level = event.getDataOrDefault<int>("final_level", 0);
                
                std::string message = "💀 " + characterName + " has died!";
                if (level > 0) {
                    message += " (Level " + std::to_string(level) + ")";
                }
                
                LM.writeLog(Level::Info, message);
                
            } catch (const std::exception& e) {
                LM.writeLog(Level::Error, "CombatLogObserver: Error processing CHARACTER_DIED event: " + std::string(e.what()));
            }
        }
        
        void logLevelUp(const Event& event) {
            try {
                auto characterName = event.getData<std::string>("character_name");
                auto newLevel = event.getData<int>("new_level");
                
                std::string message = "🎉 " + characterName + " reached level " + 
                                    std::to_string(newLevel) + "!";
                
                if (event.hasData("old_level")) {
                    auto oldLevel = event.getData<int>("old_level");
                    message += " (from level " + std::to_string(oldLevel) + ")";
                }
                
                LM.writeLog(Level::Info, message);
                
            } catch (const std::exception& e) {
                LM.writeLog(Level::Error, "CombatLogObserver: Error processing LEVEL_UP event: " + std::string(e.what()));
            }
        }
        
        void logMagicLevelUp(const Event& event) {
            try {
                auto characterName = event.getData<std::string>("character_name");
                auto newLevel = event.getData<int>("new_magic_level");
                
                std::string message = "✨ " + characterName + " magic level increased to " + 
                                    std::to_string(newLevel) + "!";
                
                LM.writeLog(Level::Info, message);
                
            } catch (const std::exception& e) {
                LM.writeLog(Level::Error, "CombatLogObserver: Error processing MAGIC_LEVEL_UP event: " + std::string(e.what()));
            }
        }
        
        void logExperienceGained(const Event& event) {
            try {
                auto characterName = event.getData<std::string>("character_name");
                auto expGained = event.getData<int>("experience_gained");
                auto totalExp = event.getDataOrDefault<int>("total_experience", 0);
                
                std::string message = characterName + " gains " + std::to_string(expGained) + " experience";
                if (totalExp > 0) {
                    message += " (total: " + std::to_string(totalExp) + ")";
                }
                
                LM.writeLog(Level::Debug, message);
                
            } catch (const std::exception& e) {
                LM.writeLog(Level::Error, "CombatLogObserver: Error processing EXPERIENCE_GAINED event: " + std::string(e.what()));
            }
        }
        
        void logHealthChanged(const Event& event) {
            try {
                auto characterName = event.getData<std::string>("character_name");
                auto oldValue = event.getData<int>("old_value");
                auto newValue = event.getData<int>("new_value");
                
                std::string change = (newValue > oldValue) ? "gained" : "lost";
                int amount = std::abs(newValue - oldValue);
                
                std::string message = characterName + " " + change + " " + 
                                    std::to_string(amount) + " health (" + 
                                    std::to_string(newValue) + " HP)";
                
                LM.writeLog(Level::Debug, message);
                
            } catch (const std::exception& e) {
                LM.writeLog(Level::Error, "CombatLogObserver: Error processing HEALTH_CHANGED event: " + std::string(e.what()));
            }
        }
    };
}

#endif // __COMBAT_LOG_OBSERVER_H__
