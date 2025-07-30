#ifndef __ACHIEVEMENT_OBSERVER_H__
#define __ACHIEVEMENT_OBSERVER_H__

#include "EventObserver.h"
#include "LogManager.h"
#include <unordered_map>
#include <string>
#include <vector>

namespace noname
{
    /**
     * @brief Observer que maneja achievements del juego
     * 
     * Rastrea estadísticas de personajes y desbloquea achievements
     * basados en eventos del juego.
     */
    class AchievementObserver : public EventObserver {
    private:
        // Estadísticas por personaje
        std::unordered_map<std::string, int> damageDealt_;
        std::unordered_map<std::string, int> damageTaken_;
        std::unordered_map<std::string, int> experienceGained_;
        std::unordered_map<std::string, int> levelsGained_;
        std::unordered_map<std::string, int> combatsWon_;
        std::unordered_map<std::string, int> deaths_;
        
        // Achievements desbloqueados por personaje
        std::unordered_map<std::string, std::vector<std::string>> unlockedAchievements_;
        
        // Configuración de achievements
        struct AchievementThreshold {
            int value;
            std::string name;
            std::string description;
        };
        
        std::vector<AchievementThreshold> damageThresholds_ = {
            {100, "First Blood", "Deal 100 damage total"},
            {1000, "Warrior", "Deal 1000 damage total"},
            {5000, "Veteran Fighter", "Deal 5000 damage total"},
            {10000, "Damage Dealer", "Deal 10000 damage total"}
        };
        
        std::vector<AchievementThreshold> levelThresholds_ = {
            {5, "Apprentice", "Reach level 5"},
            {10, "Journeyman", "Reach level 10"},
            {20, "Expert", "Reach level 20"},
            {50, "Master", "Reach level 50"},
            {100, "Legendary", "Reach level 100"}
        };
        
    public:
        AchievementObserver() = default;
        
        void onEvent(const Event& event) override {
            switch (event.getType()) {
                case EventType::DAMAGE_DEALT:
                    handleDamageDealt(event);
                    break;
                    
                case EventType::DAMAGE_TAKEN:
                    handleDamageTaken(event);
                    break;
                    
                case EventType::EXPERIENCE_GAINED:
                    handleExperienceGained(event);
                    break;
                    
                case EventType::LEVEL_UP:
                    handleLevelUp(event);
                    break;
                    
                case EventType::CHARACTER_DIED:
                    handleCharacterDied(event);
                    break;
                    
                case EventType::COMBAT_ENDED:
                    handleCombatEnded(event);
                    break;
                    
                default:
                    break;
            }
        }
        
        std::string getObserverId() const override {
            return "AchievementObserver";
        }
        
        bool handlesEventType(EventType eventType) const override {
            switch (eventType) {
                case EventType::DAMAGE_DEALT:
                case EventType::DAMAGE_TAKEN:
                case EventType::EXPERIENCE_GAINED:
                case EventType::LEVEL_UP:
                case EventType::CHARACTER_DIED:
                case EventType::COMBAT_ENDED:
                    return true;
                default:
                    return false;
            }
        }
        
        int getPriority() const override {
            return 75; // Prioridad media-baja
        }
        
        // Métodos para obtener estadísticas
        int getTotalDamageDealt(const std::string& character) const {
            auto it = damageDealt_.find(character);
            return it != damageDealt_.end() ? it->second : 0;
        }
        
        int getTotalDamageTaken(const std::string& character) const {
            auto it = damageTaken_.find(character);
            return it != damageTaken_.end() ? it->second : 0;
        }
        
        int getTotalExperienceGained(const std::string& character) const {
            auto it = experienceGained_.find(character);
            return it != experienceGained_.end() ? it->second : 0;
        }
        
        int getLevelsGained(const std::string& character) const {
            auto it = levelsGained_.find(character);
            return it != levelsGained_.end() ? it->second : 0;
        }
        
        int getCombatsWon(const std::string& character) const {
            auto it = combatsWon_.find(character);
            return it != combatsWon_.end() ? it->second : 0;
        }
        
        int getDeaths(const std::string& character) const {
            auto it = deaths_.find(character);
            return it != deaths_.end() ? it->second : 0;
        }
        
        std::vector<std::string> getUnlockedAchievements(const std::string& character) const {
            auto it = unlockedAchievements_.find(character);
            return it != unlockedAchievements_.end() ? it->second : std::vector<std::string>{};
        }
        
        /**
         * @brief Obtiene estadísticas completas de un personaje
         */
        std::string getCharacterStats(const std::string& character) const {
            std::string stats = "=== " + character + " Statistics ===\n";
            stats += "Damage Dealt: " + std::to_string(getTotalDamageDealt(character)) + "\n";
            stats += "Damage Taken: " + std::to_string(getTotalDamageTaken(character)) + "\n";
            stats += "Experience Gained: " + std::to_string(getTotalExperienceGained(character)) + "\n";
            stats += "Levels Gained: " + std::to_string(getLevelsGained(character)) + "\n";
            stats += "Combats Won: " + std::to_string(getCombatsWon(character)) + "\n";
            stats += "Deaths: " + std::to_string(getDeaths(character)) + "\n";
            
            auto achievements = getUnlockedAchievements(character);
            stats += "Achievements: " + std::to_string(achievements.size()) + "\n";
            for (const auto& achievement : achievements) {
                stats += "  - " + achievement + "\n";
            }
            
            return stats;
        }
        
    private:
        void handleDamageDealt(const Event& event) {
            try {
                auto attacker = event.getData<std::string>("attacker");
                auto damage = event.getData<int>("damage");
                
                damageDealt_[attacker] += damage;
                
                // Verificar achievements de daño
                checkDamageAchievements(attacker);
                
            } catch (const std::exception& e) {
                LM.writeLog(Level::Error, "AchievementObserver: Error processing DAMAGE_DEALT: " + std::string(e.what()));
            }
        }
        
        void handleDamageTaken(const Event& event) {
            try {
                auto character = event.getData<std::string>("character_name");
                auto damage = event.getData<int>("damage");
                
                damageTaken_[character] += damage;
                
                // Podrías agregar achievements por resistencia aquí
                
            } catch (const std::exception& e) {
                LM.writeLog(Level::Error, "AchievementObserver: Error processing DAMAGE_TAKEN: " + std::string(e.what()));
            }
        }
        
        void handleExperienceGained(const Event& event) {
            try {
                auto character = event.getData<std::string>("character_name");
                auto expGained = event.getData<int>("experience_gained");
                
                experienceGained_[character] += expGained;
                
            } catch (const std::exception& e) {
                LM.writeLog(Level::Error, "AchievementObserver: Error processing EXPERIENCE_GAINED: " + std::string(e.what()));
            }
        }
        
        void handleLevelUp(const Event& event) {
            try {
                auto character = event.getData<std::string>("character_name");
                auto newLevel = event.getData<int>("new_level");
                
                levelsGained_[character]++;
                
                // Verificar achievements de nivel
                checkLevelAchievements(character, newLevel);
                
            } catch (const std::exception& e) {
                LM.writeLog(Level::Error, "AchievementObserver: Error processing LEVEL_UP: " + std::string(e.what()));
            }
        }
        
        void handleCharacterDied(const Event& event) {
            try {
                auto character = event.getData<std::string>("character_name");
                
                deaths_[character]++;
                
                // Achievement por morirse muchas veces (humor negro)
                if (deaths_[character] == 5) {
                    unlockAchievement(character, "Persistent", "Die 5 times but keep trying");
                }
                
            } catch (const std::exception& e) {
                LM.writeLog(Level::Error, "AchievementObserver: Error processing CHARACTER_DIED: " + std::string(e.what()));
            }
        }
        
        void handleCombatEnded(const Event& event) {
            try {
                auto attacker = event.getData<std::string>("attacker");
                auto hit = event.getDataOrDefault<bool>("hit", false);
                
                if (hit) {
                    combatsWon_[attacker]++;
                    
                    // Achievements por combates ganados
                    int wins = combatsWon_[attacker];
                    if (wins == 1) {
                        unlockAchievement(attacker, "First Victory", "Win your first combat");
                    } else if (wins == 10) {
                        unlockAchievement(attacker, "Skilled Fighter", "Win 10 combats");
                    } else if (wins == 50) {
                        unlockAchievement(attacker, "Combat Master", "Win 50 combats");
                    }
                }
                
            } catch (const std::exception& e) {
                LM.writeLog(Level::Error, "AchievementObserver: Error processing COMBAT_ENDED: " + std::string(e.what()));
            }
        }
        
        void checkDamageAchievements(const std::string& character) {
            int totalDamage = damageDealt_[character];
            
            for (const auto& threshold : damageThresholds_) {
                if (totalDamage >= threshold.value) {
                    unlockAchievement(character, threshold.name, threshold.description);
                }
            }
        }
        
        void checkLevelAchievements(const std::string& character, int level) {
            for (const auto& threshold : levelThresholds_) {
                if (level >= threshold.value) {
                    unlockAchievement(character, threshold.name, threshold.description);
                }
            }
        }
        
        void unlockAchievement(const std::string& character, const std::string& name, const std::string& description) {
            auto& achievements = unlockedAchievements_[character];
            
            // Verificar si ya está desbloqueado
            if (std::find(achievements.begin(), achievements.end(), name) != achievements.end()) {
                return; // Ya desbloqueado
            }
            
            achievements.push_back(name);
            
            LM.writeLog(Level::Info, 
                "🏆 ACHIEVEMENT UNLOCKED: " + character + " earned '" + name + "' - " + description);
        }
    };
}

#endif // __ACHIEVEMENT_OBSERVER_H__
