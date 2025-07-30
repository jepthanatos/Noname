# Lección: Strategy Pattern - Sistema de Combate Flexible

## 🎯 **¿Qué es el Strategy Pattern?**

El **Strategy Pattern** es un patrón de diseño comportamental que permite seleccionar algoritmos sobre la marcha. Define una familia de algoritmos, los encapsula y los hace intercambiables.

### **Problema en el Código Actual**

En tu código actual, toda la lógica de combate está hardcodeada en `Character::attack()`:

```cpp
// PROBLEMA: Lógica de ataque rígida y no extensible
short Character::getAttackDamage() noexcept {
    auto weapon = _inventory.getWeapon();
    if (!weapon) return 0;
    
    short doubleDamage{1};
    auto d20{Utils::rollDie(1, 20)};
    if (d20 > 1) {
        if (d20 == 20) doubleDamage = 2;  // Critical hit
        short baseDamage = Utils::rollDie(1, weapon->getDie()) * doubleDamage;
        short skillBonus = skills.at(static_cast<size_t>(weapon->getSkillType()));
        return baseDamage + skillBonus;
    }
    return 0; // Miss
}
```

### **¿Por qué es problemático?**

1. **No extensible**: Para agregar nuevos tipos de ataque necesitas modificar la clase Character
2. **Viola SRP**: Character maneja tanto estado como lógica de combate
3. **Difícil testing**: No puedes probar diferentes estrategias por separado
4. **Acoplamiento**: Diferentes tipos de armas requieren lógica diferente

## 🎯 **Solución: Strategy Pattern**

### **Paso 1: Definir la Interfaz Strategy**

```cpp
// AttackStrategy.h
class AttackStrategy {
public:
    virtual ~AttackStrategy() = default;
    
    // Método principal para calcular daño
    virtual int calculateDamage(const Character& attacker, 
                               const std::shared_ptr<Weapon>& weapon) const = 0;
    
    // Obtener el skill relevante para este tipo de ataque
    virtual SkillType getRelevantSkill(const std::shared_ptr<Weapon>& weapon) const = 0;
    
    // Verificar si el ataque fue exitoso (no falló)
    virtual bool isSuccessfulHit() const = 0;
    
    // Verificar si fue un golpe crítico
    virtual bool isCriticalHit() const = 0;
    
    // Obtener multiplicador de daño crítico
    virtual int getCriticalMultiplier() const { return 2; }
};
```

### **Paso 2: Implementar Estrategias Concretas**

```cpp
// Estrategia para combate cuerpo a cuerpo
class MeleeAttackStrategy : public AttackStrategy {
public:
    int calculateDamage(const Character& attacker, 
                       const std::shared_ptr<Weapon>& weapon) const override {
        if (!weapon) return calculateUnarmedDamage(attacker);
        
        // Roll para determinar si golpea
        int hitRoll = Utils::rollDie(1, 20);
        if (hitRoll <= 1) return 0; // Miss
        
        // Calcular daño base
        int baseDamage = Utils::rollDie(1, weapon->getDie());
        
        // Aplicar críticos
        if (hitRoll >= 20) {
            baseDamage *= getCriticalMultiplier();
        }
        
        // Agregar bonus de skill
        SkillType skill = getRelevantSkill(weapon);
        int skillBonus = attacker.getSkill(skill);
        
        // Agregar bonus de fuerza para combate cuerpo a cuerpo
        int strengthBonus = attacker.getHeritable(HeritableType::STRENGTH) / 10;
        
        return baseDamage + skillBonus + strengthBonus;
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
    
    int calculateUnarmedDamage(const Character& attacker) const {
        int roll = Utils::rollDie(1, 20);
        lastHitRoll = roll;
        if (roll <= 1) return 0;
        
        int baseDamage = Utils::rollDie(1, 4); // Puños hacen 1d4
        int skillBonus = attacker.getSkill(SkillType::FIST);
        int strengthBonus = attacker.getHeritable(HeritableType::STRENGTH) / 10;
        
        return baseDamage + skillBonus + strengthBonus;
    }
};

// Estrategia para combate a distancia
class RangedAttackStrategy : public AttackStrategy {
public:
    int calculateDamage(const Character& attacker, 
                       const std::shared_ptr<Weapon>& weapon) const override {
        if (!weapon) return 0; // No se puede atacar a distancia sin arma
        
        // Roll para determinar si golpea (más difícil a distancia)
        int hitRoll = Utils::rollDie(1, 20);
        lastHitRoll = hitRoll;
        if (hitRoll <= 2) return 0; // Miss más probable
        
        // Calcular daño base
        int baseDamage = Utils::rollDie(1, weapon->getDie());
        
        // Aplicar críticos
        if (hitRoll >= 19) { // Crítico más fácil para armas a distancia
            baseDamage *= getCriticalMultiplier();
        }
        
        // Agregar bonus de skill
        int skillBonus = attacker.getSkill(SkillType::DISTANCE);
        
        // Agregar bonus de destreza para combate a distancia
        int dexterityBonus = attacker.getHeritable(HeritableType::DEXTERY) / 10;
        
        return baseDamage + skillBonus + dexterityBonus;
    }
    
    SkillType getRelevantSkill(const std::shared_ptr<Weapon>& weapon) const override {
        return SkillType::DISTANCE;
    }
    
    bool isSuccessfulHit() const override {
        return lastHitRoll > 2;
    }
    
    bool isCriticalHit() const override {
        return lastHitRoll >= 19;
    }
    
private:
    mutable int lastHitRoll = 0;
};

// Estrategia para ataques mágicos
class MagicAttackStrategy : public AttackStrategy {
public:
    int calculateDamage(const Character& attacker, 
                       const std::shared_ptr<Weapon>& weapon) const override {
        // Los ataques mágicos siempre golpean, pero consumen mana
        if (attacker.getCurrentMana() < getManaCost()) return 0;
        
        lastHitRoll = 20; // Siempre golpea
        
        // Daño basado en nivel mágico e inteligencia
        int baseDamage = Utils::rollDie(attacker.getMagicLevel(), 6);
        int intelligenceBonus = attacker.getHeritable(HeritableType::INTELLIGENCE) / 5;
        
        return baseDamage + intelligenceBonus;
    }
    
    SkillType getRelevantSkill(const std::shared_ptr<Weapon>& weapon) const override {
        return SkillType::LAST_SKILL; // Los hechizos no usan skills de armas
    }
    
    bool isSuccessfulHit() const override {
        return true; // Los hechizos siempre golpean
    }
    
    bool isCriticalHit() const override {
        return false; // Los hechizos no hacen críticos
    }
    
    int getManaCost() const { return 10; }
    
private:
    mutable int lastHitRoll = 20;
};
```

### **Paso 3: Modificar la Clase Character**

```cpp
// En Character.h
class Character {
private:
    std::unique_ptr<AttackStrategy> attackStrategy;
    
public:
    // Cambiar estrategia de ataque
    void setAttackStrategy(std::unique_ptr<AttackStrategy> strategy) {
        attackStrategy = std::move(strategy);
    }
    
    // Obtener estrategia actual
    AttackStrategy* getAttackStrategy() const {
        return attackStrategy.get();
    }
    
    // Método de ataque usando estrategia
    virtual void attack(Character &defender) noexcept;
};

// En Character.cpp
void Character::attack(Character &defender) noexcept {
    if (!attackStrategy) {
        // Estrategia por defecto basada en el arma equipada
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
        // Solo actualizar tries si el ataque fue exitoso y usa un arma
        if (weapon && attackStrategy->getRelevantSkill(weapon) != SkillType::LAST_SKILL) {
            updateTries(attackStrategy->getRelevantSkill(weapon));
        }
        
        // Aplicar daño al defensor
        defender.defense(damage);
        
        // Log del ataque
        std::string attackType = attackStrategy->isCriticalHit() ? " CRITICAL" : "";
        LM.writeLog(Level::Debug, getName() + " attacks " + defender.getName() + 
                   " for " + std::to_string(damage) + attackType + " damage");
    } else {
        LM.writeLog(Level::Debug, getName() + " missed attack against " + defender.getName());
    }
}
```

## 📊 **Beneficios del Strategy Pattern**

### **1. Extensibilidad**
```cpp
// Fácil agregar nuevos tipos de ataque
class PoisonAttackStrategy : public AttackStrategy {
    // Implementación para ataques con veneno
};

class BerserkerAttackStrategy : public AttackStrategy {
    // Ataques más poderosos pero menos precisos
};
```

### **2. Flexibilidad en Runtime**
```cpp
Character warrior("Warrior");

// Cambiar estrategia dinámicamente
if (enemy.getType() == "Undead") {
    warrior.setAttackStrategy(std::make_unique<HolyAttackStrategy>());
} else if (warrior.getCurrentHealth() < 20) {
    warrior.setAttackStrategy(std::make_unique<DesperationAttackStrategy>());
}
```

### **3. Testing Mejorado**
```cpp
// Puedes testear estrategias por separado
TEST(MeleeAttackStrategyTest, CalculatesDamageCorrectly) {
    MeleeAttackStrategy strategy;
    Character attacker("Test");
    auto weapon = std::make_shared<Weapon>("Sword", SkillType::SWORD);
    
    int damage = strategy.calculateDamage(attacker, weapon);
    EXPECT_GT(damage, 0);
}
```

### **4. Configuración por Tipo de Personaje**
```cpp
class Warrior : public Character {
public:
    Warrior(const std::string& name) : Character(name) {
        setAttackStrategy(std::make_unique<MeleeAttackStrategy>());
    }
};

class Archer : public Character {
public:
    Archer(const std::string& name) : Character(name) {
        setAttackStrategy(std::make_unique<RangedAttackStrategy>());
    }
};

class Mage : public Character {
public:
    Mage(const std::string& name) : Character(name) {
        setAttackStrategy(std::make_unique<MagicAttackStrategy>());
    }
};
```

## 🔧 **Pasos de Implementación**

1. **Crear AttackStrategy.h** con la interfaz base
2. **Implementar estrategias concretas** en archivos separados
3. **Modificar Character** para usar estrategias
4. **Crear factory** para estrategias por defecto
5. **Actualizar tests** para probar cada estrategia

## 🎯 **Ejemplo de Uso Después de la Implementación**

```cpp
Character knight("Sir Lancelot");
Character archer("Robin Hood");
Character wizard("Gandalf");

// Configuración automática basada en equipo
knight.equipWeapon(swordWeapon);     // Auto-selecciona MeleeAttackStrategy
archer.equipWeapon(bowWeapon);       // Auto-selecciona RangedAttackStrategy
wizard.setAttackStrategy(std::make_unique<MagicAttackStrategy>());

// Combate flexible
knight.attack(dragon);    // Ataque cuerpo a cuerpo con bonus de fuerza
archer.attack(dragon);    // Ataque a distancia con bonus de destreza
wizard.attack(dragon);    // Ataque mágico que consume mana

// Cambio dinámico de estrategia
if (knight.getHealthData().getPercentage() < 25) {
    knight.setAttackStrategy(std::make_unique<DesperationAttackStrategy>());
}
```

## 📚 **Lecciones Aprendidas**

1. **Encapsulación de algoritmos**: Cada estrategia maneja su propia lógica
2. **Polimorfismo**: Mismo interface, diferentes implementaciones
3. **Composición sobre herencia**: Character usa estrategias, no hereda de ellas
4. **Responsabilidad única**: Character gestiona estado, Strategy gestiona combate
5. **Extensibilidad**: Agregar nuevas estrategias sin modificar código existente

Este patrón transforma tu sistema de combate de rígido y acoplado a flexible y extensible, permitiendo fácilmente agregar nuevos tipos de ataque, personajes especializados y mecánicas de combate complejas.
