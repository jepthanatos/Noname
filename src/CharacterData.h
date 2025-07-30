#ifndef __CHARACTER_DATA_H__
#define __CHARACTER_DATA_H__

#include <algorithm>

namespace noname
{
    /**
     * @brief Estructura que agrupa datos relacionados con la vida del personaje
     */
    struct HealthData {
        int current;
        int maximum;
        
        HealthData(int max = 100) : current(max), maximum(max) {}
        
        int getPercentage() const { 
            return maximum > 0 ? (current * 100) / maximum : 0; 
        }
        
        bool isDead() const { return current <= 0; }
        bool isFullHealth() const { return current >= maximum; }
        
        void heal(int amount) {
            if (amount > 0) {
                current = std::min(current + amount, maximum);
            }
        }
        
        void takeDamage(int damage) {
            if (damage > 0) {
                current = std::max(0, current - damage);
            }
        }
        
        void setMaximum(int newMax) {
            maximum = newMax;
            current = std::min(current, maximum); // Ajusta current si es necesario
        }
    };

    /**
     * @brief Estructura que agrupa datos relacionados con el maná del personaje
     */
    struct ManaData {
        int current;
        int maximum;
        
        ManaData(int max = 50) : current(max), maximum(max) {}
        
        int getPercentage() const { 
            return maximum > 0 ? (current * 100) / maximum : 0; 
        }
        
        bool isEmpty() const { return current <= 0; }
        bool isFull() const { return current >= maximum; }
        
        void restore(int amount) {
            if (amount > 0) {
                current = std::min(current + amount, maximum);
            }
        }
        
        bool consume(int cost) {
            if (current >= cost && cost > 0) {
                current -= cost;
                return true;
            }
            return false;
        }
        
        void setMaximum(int newMax) {
            maximum = newMax;
            current = std::min(current, maximum); // Ajusta current si es necesario
        }
    };

    /**
     * @brief Estructura que agrupa datos relacionados con la experiencia
     */
    struct ExperienceData {
        unsigned long long current;
        unsigned long long nextLevel;
        
        ExperienceData() : current(0), nextLevel(1000) {}
        
        int getPercentageToNextLevel() const {
            if (nextLevel <= current) return 100;
            return static_cast<int>((current * 100) / nextLevel);
        }
        
        bool hasLeveledUp() const { return current >= nextLevel; }
        
        void gain(unsigned long long amount) {
            current += amount;
        }
        
        void lose(unsigned long long amount) {
            current = (current > amount) ? current - amount : 0;
        }
        
        void setNextLevelRequirement(unsigned long long requirement) {
            nextLevel = requirement;
        }
    };

    /**
     * @brief Estructura que agrupa datos relacionados con la capacidad de carga
     */
    struct CapacityData {
        int current;
        int maximum;
        
        CapacityData(int max = 100) : current(0), maximum(max) {}
        
        int getAvailable() const { return maximum - current; }
        int getPercentageUsed() const { 
            return maximum > 0 ? (current * 100) / maximum : 0; 
        }
        
        bool canCarry(int weight) const { return current + weight <= maximum; }
        bool isOverloaded() const { return current > maximum; }
        
        void updateCurrent(int newCurrent) {
            current = std::max(0, newCurrent);
        }
        
        void setMaximum(int newMax) {
            maximum = newMax;
        }
    };

    /**
     * @brief Estructura que agrupa datos relacionados con la velocidad
     */
    struct SpeedData {
        int base;
        int current;
        
        SpeedData(int baseSpeed = 100) : base(baseSpeed), current(baseSpeed) {}
        
        void updateCurrent(int weightPenalty = 0) {
            current = std::max(1, base - weightPenalty); // Mínimo 1 de velocidad
        }
        
        void setBase(int newBase) {
            base = newBase;
            updateCurrent(); // Recalcula current basado en el nuevo base
        }
    };
}

#endif // __CHARACTER_DATA_H__
