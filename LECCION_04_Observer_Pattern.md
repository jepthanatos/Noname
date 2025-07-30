# Lección: Observer Pattern - Sistema de Eventos y Notificaciones

## 🎯 **¿Qué es el Observer Pattern?**

El **Observer Pattern** es un patrón de diseño comportamental que define una dependencia uno-a-muchos entre objetos, de manera que cuando un objeto cambia su estado, todos sus dependientes son notificados automáticamente.

### **Problema en el Código Actual**

En tu código actual, los cambios de estado no notifican a otros sistemas que podrían estar interesados:

```cpp
// PROBLEMA: Cambios aislados sin notificaciones
void Character::takeDamage(short damage) noexcept {
    healthData.setCurrent(healthData.getCurrent() - damage);
    
    if (healthData.getCurrent() <= 0) {
        // Solo maneja la muerte localmente
        healthData.setCurrent(0);
        die();
    }
    // ❌ No notifica a: UI, Achievement System, Combat Log, etc.
}

void Character::gainExperience(int exp) {
    experience.setCurrent(experience.getCurrent() + exp);
    
    if (experience.getCurrent() >= experience.getMaximum()) {
        levelUp();
    }
    // ❌ No notifica a: UI, Sound System, Achievement System, etc.
}
```

### **¿Por qué es problemático?**

1. **Acoplamiento fuerte**: Para agregar nueva funcionalidad necesitas modificar Character
2. **Responsabilidad múltiple**: Character maneja estado Y notificaciones
3. **Difícil testing**: No puedes probar listeners por separado
4. **Mantenimiento**: Cambios en un sistema requieren modificar muchas clases

## 🎯 **Solución: Observer Pattern**

### **Paso 1: Definir Event System**

```cpp
// EventTypes.h
enum class EventType {
    DAMAGE_TAKEN,
    DAMAGE_DEALT,
    HEALTH_CHANGED,
    MANA_CHANGED,
    EXPERIENCE_GAINED,
    LEVEL_UP,
    CHARACTER_DIED,
    CHARACTER_RESPAWNED,
    ITEM_EQUIPPED,
    ITEM_UNEQUIPPED,
    SKILL_IMPROVED,
    COMBAT_STARTED,
    COMBAT_ENDED
};

// Event.h
#include <any>
#include <unordered_map>
#include <string>
#include <memory>

class Event {
private:
    EventType type_;
    std::unordered_map<std::string, std::any> data_;
    
public:
    Event(EventType type) : type_(type) {}
    
    EventType getType() const { return type_; }
    
    // Template para agregar datos tipados al evento
    template<typename T>
    void setData(const std::string& key, const T& value) {
        data_[key] = value;
    }
    
    // Template para obtener datos tipados del evento
    template<typename T>
    T getData(const std::string& key) const {
        auto it = data_.find(key);
        if (it != data_.end()) {
            try {
                return std::any_cast<T>(it->second);
            } catch (const std::bad_any_cast& e) {
                throw std::runtime_error("Invalid data type for key: " + key);
            }
        }
        throw std::runtime_error("Key not found: " + key);
    }
    
    // Verificar si existe una clave
    bool hasData(const std::string& key) const {
        return data_.find(key) != data_.end();
    }
    
    // Obtener todas las claves
    std::vector<std::string> getKeys() const {
        std::vector<std::string> keys;
        for (const auto& pair : data_) {
            keys.push_back(pair.first);
        }
        return keys;
    }
};
```

### **Paso 2: Crear Observer Interface**

```cpp
// EventObserver.h
class EventObserver {
public:
    virtual ~EventObserver() = default;
    
    // Método que será llamado cuando ocurra un evento
    virtual void onEvent(const Event& event) = 0;
    
    // Opcional: obtener ID único del observer
    virtual std::string getObserverId() const {
        return "Observer_" + std::to_string(reinterpret_cast<uintptr_t>(this));
    }
};

// EventSubject.h (Observable)
#include <vector>
#include <memory>
#include <algorithm>

class EventSubject {
private:
    std::vector<std::weak_ptr<EventObserver>> observers_;
    
    // Limpiar observers que ya no existen
    void cleanupObservers() {
        observers_.erase(
            std::remove_if(observers_.begin(), observers_.end(),
                [](const std::weak_ptr<EventObserver>& obs) {
                    return obs.expired();
                }),
            observers_.end()
        );
    }
    
public:
    virtual ~EventSubject() = default;
    
    // Agregar observer
    void subscribe(std::shared_ptr<EventObserver> observer) {
        if (observer) {
            observers_.push_back(observer);
        }
    }
    
    // Remover observer
    void unsubscribe(std::shared_ptr<EventObserver> observer) {
        observers_.erase(
            std::remove_if(observers_.begin(), observers_.end(),
                [&observer](const std::weak_ptr<EventObserver>& obs) {
                    auto locked = obs.lock();
                    return !locked || locked == observer;
                }),
            observers_.end()
        );
    }
    
    // Notificar a todos los observers
    void notifyObservers(const Event& event) {
        cleanupObservers();
        
        for (auto& weakObs : observers_) {
            if (auto obs = weakObs.lock()) {
                try {
                    obs->onEvent(event);
                } catch (const std::exception& e) {
                    // Log error pero continúa con otros observers
                    LM.writeLog(Level::Error, 
                        "Observer error: " + std::string(e.what()));
                }
            }
        }
    }
    
    // Obtener número de observers activos
    size_t getObserverCount() const {
        return std::count_if(observers_.begin(), observers_.end(),
            [](const std::weak_ptr<EventObserver>& obs) {
                return !obs.expired();
            });
    }
};
```

### **Paso 3: Modificar Character para ser Observable**

```cpp
// En Character.h
class Character : public EventSubject {
    // ... resto de la clase
    
private:
    // Helper para crear eventos comunes
    Event createHealthEvent(EventType type, int oldValue, int newValue) const {
        Event event(type);
        event.setData<std::string>("character_name", getName());
        event.setData<int>("old_value", oldValue);
        event.setData<int>("new_value", newValue);
        event.setData<int>("character_level", getLevel());
        return event;
    }
    
    Event createCombatEvent(EventType type, const std::string& target, int damage = 0) const {
        Event event(type);
        event.setData<std::string>("attacker", getName());
        event.setData<std::string>("target", target);
        if (damage > 0) {
            event.setData<int>("damage", damage);
        }
        return event;
    }
};

// En Character.cpp - Métodos modificados para notificar
void Character::takeDamage(short damage) noexcept {
    int oldHealth = healthData.getCurrent();
    healthData.setCurrent(healthData.getCurrent() - damage);
    
    // Notificar daño recibido
    Event damageEvent = createCombatEvent(EventType::DAMAGE_TAKEN, "self", damage);
    notifyObservers(damageEvent);
    
    // Notificar cambio de salud
    Event healthEvent = createHealthEvent(EventType::HEALTH_CHANGED, 
                                        oldHealth, healthData.getCurrent());
    notifyObservers(healthEvent);
    
    if (healthData.getCurrent() <= 0) {
        healthData.setCurrent(0);
        die();
        
        // Notificar muerte
        Event deathEvent(EventType::CHARACTER_DIED);
        deathEvent.setData<std::string>("character_name", getName());
        deathEvent.setData<int>("final_level", getLevel());
        notifyObservers(deathEvent);
    }
}

void Character::gainExperience(int exp) {
    int oldExp = experience.getCurrent();
    int oldLevel = getLevel();
    
    experience.setCurrent(experience.getCurrent() + exp);
    
    // Notificar experiencia ganada
    Event expEvent(EventType::EXPERIENCE_GAINED);
    expEvent.setData<std::string>("character_name", getName());
    expEvent.setData<int>("experience_gained", exp);
    expEvent.setData<int>("total_experience", experience.getCurrent());
    notifyObservers(expEvent);
    
    if (experience.getCurrent() >= experience.getMaximum()) {
        levelUp();
        
        // Notificar subida de nivel
        Event levelEvent(EventType::LEVEL_UP);
        levelEvent.setData<std::string>("character_name", getName());
        levelEvent.setData<int>("old_level", oldLevel);
        levelEvent.setData<int>("new_level", getLevel());
        notifyObservers(levelEvent);
    }
}

void Character::attack(Character &defender) noexcept {
    // Notificar inicio de combate
    Event combatStartEvent(EventType::COMBAT_STARTED);
    combatStartEvent.setData<std::string>("attacker", getName());
    combatStartEvent.setData<std::string>("defender", defender.getName());
    notifyObservers(combatStartEvent);
    
    // Lógica de ataque existente...
    if (!attackStrategy) {
        auto weapon = _inventory.getWeapon();
        if (weapon && weapon->getSkillType() == SkillType::DISTANCE) {
            setAttackStrategy(std::make_unique<RangedAttackStrategy>());
        } else {
            setAttackStrategy(std::make_unique<MeleeAttackStrategy>());
        }
    }
    
    auto weapon = _inventory.getWeapon();
    int damage = attackStrategy->calculateDamage(*this, weapon);
    
    if (damage > 0) {
        // Notificar daño infligido
        Event damageDealtEvent = createCombatEvent(EventType::DAMAGE_DEALT, 
                                                 defender.getName(), damage);
        notifyObservers(damageDealtEvent);
        
        // Actualizar tries si corresponde
        if (weapon && attackStrategy->getRelevantSkill(weapon) != SkillType::LAST_SKILL) {
            updateTries(attackStrategy->getRelevantSkill(weapon));
        }
        
        defender.defense(damage);
    }
    
    // Notificar fin de combate
    Event combatEndEvent(EventType::COMBAT_ENDED);
    combatEndEvent.setData<std::string>("attacker", getName());
    combatEndEvent.setData<std::string>("defender", defender.getName());
    combatEndEvent.setData<bool>("hit", damage > 0);
    if (damage > 0) {
        combatEndEvent.setData<int>("damage", damage);
    }
    notifyObservers(combatEndEvent);
}
```

### **Paso 4: Implementar Observers Concretos**

```cpp
// CombatLogObserver.h
class CombatLogObserver : public EventObserver {
public:
    void onEvent(const Event& event) override {
        switch (event.getType()) {
            case EventType::DAMAGE_DEALT: {
                auto attacker = event.getData<std::string>("attacker");
                auto target = event.getData<std::string>("target");
                auto damage = event.getData<int>("damage");
                
                LM.writeLog(Level::Info, 
                    attacker + " deals " + std::to_string(damage) + 
                    " damage to " + target);
                break;
            }
            
            case EventType::CHARACTER_DIED: {
                auto characterName = event.getData<std::string>("character_name");
                auto level = event.getData<int>("final_level");
                
                LM.writeLog(Level::Important, 
                    characterName + " (level " + std::to_string(level) + ") has died!");
                break;
            }
            
            case EventType::LEVEL_UP: {
                auto characterName = event.getData<std::string>("character_name");
                auto newLevel = event.getData<int>("new_level");
                
                LM.writeLog(Level::Important, 
                    characterName + " reached level " + std::to_string(newLevel) + "!");
                break;
            }
        }
    }
    
    std::string getObserverId() const override {
        return "CombatLogObserver";
    }
};

// AchievementSystemObserver.h
class AchievementSystemObserver : public EventObserver {
private:
    std::unordered_map<std::string, int> damageDealt_;
    std::unordered_map<std::string, int> levelsGained_;
    
public:
    void onEvent(const Event& event) override {
        switch (event.getType()) {
            case EventType::DAMAGE_DEALT: {
                auto attacker = event.getData<std::string>("attacker");
                auto damage = event.getData<int>("damage");
                
                damageDealt_[attacker] += damage;
                
                // Verificar achievements de daño
                if (damageDealt_[attacker] >= 1000) {
                    LM.writeLog(Level::Important, 
                        attacker + " unlocked achievement: Damage Dealer (1000+ damage dealt)");
                }
                break;
            }
            
            case EventType::LEVEL_UP: {
                auto characterName = event.getData<std::string>("character_name");
                auto newLevel = event.getData<int>("new_level");
                
                levelsGained_[characterName]++;
                
                // Verificar achievements de nivel
                if (newLevel >= 10) {
                    LM.writeLog(Level::Important, 
                        characterName + " unlocked achievement: Veteran (reached level 10)");
                }
                break;
            }
        }
    }
    
    std::string getObserverId() const override {
        return "AchievementSystemObserver";
    }
    
    // Métodos para obtener estadísticas
    int getTotalDamageDealt(const std::string& character) const {
        auto it = damageDealt_.find(character);
        return it != damageDealt_.end() ? it->second : 0;
    }
    
    int getLevelsGained(const std::string& character) const {
        auto it = levelsGained_.find(character);
        return it != levelsGained_.end() ? it->second : 0;
    }
};

// UIUpdateObserver.h
class UIUpdateObserver : public EventObserver {
public:
    void onEvent(const Event& event) override {
        switch (event.getType()) {
            case EventType::HEALTH_CHANGED: {
                auto characterName = event.getData<std::string>("character_name");
                auto newValue = event.getData<int>("new_value");
                
                // Simular actualización de UI
                LM.writeLog(Level::Debug, 
                    "UI: Updating health bar for " + characterName + 
                    " to " + std::to_string(newValue));
                break;
            }
            
            case EventType::EXPERIENCE_GAINED: {
                auto characterName = event.getData<std::string>("character_name");
                auto totalExp = event.getData<int>("total_experience");
                
                LM.writeLog(Level::Debug, 
                    "UI: Updating experience bar for " + characterName + 
                    " to " + std::to_string(totalExp));
                break;
            }
            
            case EventType::LEVEL_UP: {
                auto characterName = event.getData<std::string>("character_name");
                auto newLevel = event.getData<int>("new_level");
                
                LM.writeLog(Level::Debug, 
                    "UI: Showing level up animation for " + characterName + 
                    " (level " + std::to_string(newLevel) + ")");
                break;
            }
        }
    }
    
    std::string getObserverId() const override {
        return "UIUpdateObserver";
    }
};
```

### **Paso 5: Event Manager Global**

```cpp
// EventManager.h - Singleton para gestión global de eventos
class EventManager {
private:
    static std::unique_ptr<EventManager> instance_;
    std::vector<std::shared_ptr<EventObserver>> globalObservers_;
    
    EventManager() = default;
    
public:
    static EventManager& getInstance() {
        if (!instance_) {
            instance_ = std::unique_ptr<EventManager>(new EventManager());
        }
        return *instance_;
    }
    
    // Registrar observer global
    void registerObserver(std::shared_ptr<EventObserver> observer) {
        globalObservers_.push_back(observer);
    }
    
    // Desregistrar observer global
    void unregisterObserver(std::shared_ptr<EventObserver> observer) {
        globalObservers_.erase(
            std::remove(globalObservers_.begin(), globalObservers_.end(), observer),
            globalObservers_.end()
        );
    }
    
    // Enviar evento a todos los observers globales
    void broadcastEvent(const Event& event) {
        for (auto& observer : globalObservers_) {
            if (observer) {
                try {
                    observer->onEvent(event);
                } catch (const std::exception& e) {
                    LM.writeLog(Level::Error, 
                        "Global observer error: " + std::string(e.what()));
                }
            }
        }
    }
    
    // Obtener estadísticas
    size_t getGlobalObserverCount() const {
        return globalObservers_.size();
    }
};

// Inicialización del singleton
std::unique_ptr<EventManager> EventManager::instance_ = nullptr;
```

## 📊 **Beneficios del Observer Pattern**

### **1. Desacoplamiento**
```cpp
// Character no necesita saber quién está escuchando
character.takeDamage(50);  // Automáticamente notifica a UI, Combat Log, etc.
```

### **2. Extensibilidad**
```cpp
// Agregar nuevos sistemas sin modificar Character
auto soundObserver = std::make_shared<SoundEffectObserver>();
character.subscribe(soundObserver);
```

### **3. Configuración Dinámica**
```cpp
// Activar/desactivar sistemas en runtime
if (settings.combatLogging) {
    character.subscribe(combatLogObserver);
} else {
    character.unsubscribe(combatLogObserver);
}
```

### **4. Testing Mejorado**
```cpp
// Test observer para verificar eventos
class TestObserver : public EventObserver {
    std::vector<Event> receivedEvents_;
public:
    void onEvent(const Event& event) override {
        receivedEvents_.push_back(event);
    }
    
    bool receivedEventOfType(EventType type) const {
        return std::any_of(receivedEvents_.begin(), receivedEvents_.end(),
            [type](const Event& e) { return e.getType() == type; });
    }
};
```

## 🎯 **Ejemplo de Uso Completo**

```cpp
int main() {
    // Crear character
    Character hero("Hero");
    
    // Crear observers
    auto combatLog = std::make_shared<CombatLogObserver>();
    auto achievements = std::make_shared<AchievementSystemObserver>();
    auto ui = std::make_shared<UIUpdateObserver>();
    
    // Suscribir observers
    hero.subscribe(combatLog);
    hero.subscribe(achievements);
    hero.subscribe(ui);
    
    // Registrar observers globales
    EventManager::getInstance().registerObserver(combatLog);
    
    // Simular gameplay - todos los eventos se notifican automáticamente
    hero.gainExperience(100);    // → UI actualiza XP bar, log registra
    hero.takeDamage(25);         // → UI actualiza health bar, log registra
    hero.gainExperience(900);    // → Level up! → Achievement desbloqueado
    
    // Verificar estadísticas
    std::cout << "Damage dealt by Hero: " 
              << achievements->getTotalDamageDealt("Hero") << std::endl;
    
    return 0;
}
```

## 📚 **Lecciones Aprendidas**

1. **Separación de responsabilidades**: Character maneja estado, observers manejan reacciones
2. **Event-driven architecture**: Los cambios de estado desencadenan automáticamente acciones
3. **Flexibilidad**: Fácil agregar/remover sistemas de notificación
4. **Robustez**: Errores en un observer no afectan a otros
5. **Testing**: Cada observer se puede probar por separado

Este patrón transforma tu arquitectura de acoplada y rígida a flexible y modular, permitiendo fácilmente agregar sistemas como UI, sonido, achievements, analytics, etc., sin modificar el código core del juego.
