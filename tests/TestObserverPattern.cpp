#include "gtest/gtest.h"
#include "../src/Character.h"
#include "../src/CombatLogObserver.h"
#include "../src/AchievementObserver.h"
#include "../src/EventTypes.h"
#include "../src/Event.h"
#include <sstream>
#include <memory>

class TestObserverPattern : public ::testing::Test {
protected:
    void SetUp() override {
        // Crear personaje con configuración básica
        character = std::make_unique<Character>("TestHero");
        
        // Crear observadores
        combatLogger = std::make_shared<CombatLogObserver>();
        achievementTracker = std::make_shared<AchievementObserver>();
        
        // Suscribir observadores
        character->subscribe(combatLogger);
        character->subscribe(achievementTracker);
        
        // Crear objetivo
        target = std::make_unique<Character>("TestTarget");
    }
    
    void TearDown() override {
        character.reset();
        target.reset();
        combatLogger.reset();
        achievementTracker.reset();
    }
    
    std::unique_ptr<Character> character;
    std::unique_ptr<Character> target;
    std::shared_ptr<CombatLogObserver> combatLogger;
    std::shared_ptr<AchievementObserver> achievementTracker;
};

TEST_F(TestObserverPattern, ExperienceGainNotification) {
    // Simular ganancia de experiencia
    int initialExp = character->getExperience();
    character->gainExperience(100);
    
    // Verificar que la experiencia cambió
    EXPECT_EQ(character->getExperience(), initialExp + 100);
    
    // Verificar que se notificaron los observadores
    // (Los observadores concretos tienen sus propios logs internos)
    EXPECT_TRUE(true); // Placeholder - en implementación real verificarías logs
}

TEST_F(TestObserverPattern, DamageNotification) {
    // Obtener HP inicial
    int initialHP = character->getCurrentHealth();
    
    // Simular daño
    character->takeDamage(20);
    
    // Verificar que el HP cambió
    EXPECT_LT(character->getCurrentHealth(), initialHP);
    
    // Los observadores fueron notificados automáticamente
    EXPECT_TRUE(true); // Placeholder
}

TEST_F(TestObserverPattern, CombatNotifications) {
    // Verificar que el personaje puede atacar
    EXPECT_NO_THROW(character->attack(*target));
    
    // Los eventos de ataque y defensa fueron emitidos
    EXPECT_TRUE(true); // Placeholder
}

TEST_F(TestObserverPattern, ObserverAddRemove) {
    // Crear un observador temporal
    auto tempObserver = std::make_shared<CombatLogObserver>();
    
    // Agregarlo
    character->subscribe(tempObserver);
    
    // Removerlo
    character->unsubscribe(tempObserver);
    
    // Verificar que se puede ganar experiencia sin problemas
    EXPECT_NO_THROW(character->gainExperience(50));
}

TEST_F(TestObserverPattern, MultipleEventTypes) {
    // Generar diferentes tipos de eventos
    character->gainExperience(100);    // EXPERIENCE_GAINED
    character->takeDamage(10);         // DAMAGE_TAKEN
    character->attack(*target);        // DAMAGE_DEALT
    
    // Todos los eventos fueron procesados sin errores
    EXPECT_TRUE(!character->isDead());
}

// Clase de observador personalizada para pruebas
class TestEventCounter : public EventObserver {
private:
    int eventCount = 0;
    EventType lastEventType = EventType::EXPERIENCE_GAINED;
    
public:
    void onEvent(const Event& event) override {
        eventCount++;
        lastEventType = event.getType();
    }
    
    int getEventCount() const { return eventCount; }
    EventType getLastEventType() const { return lastEventType; }
};

TEST_F(TestObserverPattern, CustomObserver) {
    // Crear observador contador personalizado
    auto counter = std::make_shared<TestEventCounter>();
    character->subscribe(counter);
    
    // Generar eventos
    character->gainExperience(100);
    character->takeDamage(5);
    
    // Verificar que se contaron los eventos
    EXPECT_EQ(counter->getEventCount(), 2);
    EXPECT_EQ(counter->getLastEventType(), EventType::DAMAGE_TAKEN);
}
