# Lección 03: Patrón Flyweight para Optimización de Memoria

## 🎯 Objetivo de la Lección
Aprender e implementar el patrón Flyweight para reducir el consumo de memoria cuando manejamos muchos objetos similares, particularmente con los Items del juego.

## 📚 Teoría: ¿Qué es el Patrón Flyweight?

### Definición
El patrón **Flyweight** es un patrón de diseño estructural que permite ahorrar memoria almacenando eficientemente grandes cantidades de objetos similares.

### Problema que Resuelve
- **Consumo excesivo de memoria**: Cuando tenemos muchos objetos similares (ej: miles de items del mismo tipo)
- **Redundancia de datos**: Objetos que comparten la mayor parte de su estado
- **Performance degradada**: Demasiados objetos individuales afectan el rendimiento

### Conceptos Clave

#### Estado Intrínseco vs Extrínseco
- **Intrínseco**: Datos compartidos entre objetos similares (ej: tipo de arma, sprite, estadísticas base)
- **Extrínseco**: Datos únicos por instancia (ej: posición, propietario, durabilidad actual)

#### Componentes del Patrón
1. **Flyweight Interface**: Define métodos que aceptan estado extrínseco
2. **Concrete Flyweight**: Implementa la interfaz y almacena estado intrínseco
3. **Flyweight Factory**: Gestiona y reutiliza instancias de flyweights
4. **Context**: Mantiene estado extrínseco y referencias a flyweights

## 🎮 Aplicación en Nuestro RPG

### Antes: Problema de Memoria
```cpp
// ❌ PROBLEMÁTICO: Cada Item almacena toda su información
class Item {
    ItemType type;           // Repetido en todos los items del mismo tipo
    std::string name;        // Repetido (ej: "Iron Sword", "Iron Sword", ...)
    std::string description; // Repetido
    int baseValue;          // Repetido
    std::string iconPath;   // Repetido
    
    // Estado único por instancia
    int currentDurability;
    Player* owner;
    int position;
};

// Con 1000 "Iron Sword" = 1000 copias de los mismos strings y valores!
```

### Después: Solución con Flyweight
```cpp
// ✅ OPTIMIZADO: Estado intrínseco compartido
class ItemTemplate {  // Flyweight
    ItemType type;
    std::string name;
    std::string description;
    int baseValue;
    std::string iconPath;
    // ... otros datos compartidos
};

class ItemInstance {  // Context
    std::shared_ptr<ItemTemplate> template_;  // Referencia al flyweight
    int currentDurability;  // Estado extrínseco
    Player* owner;         // Estado extrínseco
    int position;          // Estado extrínseco
};

class ItemTemplateFactory {  // Factory
    static std::unordered_map<ItemType, std::shared_ptr<ItemTemplate>> templates_;
public:
    static std::shared_ptr<ItemTemplate> getTemplate(ItemType type);
};
```

## 💡 Ventajas del Flyweight Pattern

### Memoria
- **Reducción drástica**: De N objetos completos a 1 template + N contexts ligeros
- **Cache eficiente**: Templates compartidos mejoran localidad de cache
- **Escalabilidad**: Soporta miles de items sin problema

### Performance  
- **Menos allocations**: Fewer object creations
- **Faster loading**: Templates se cargan una vez
- **Better cache locality**: Shared data stays hot in cache

### Mantenimiento
- **Datos centralizados**: Cambiar stats de un item tipo afecta a todos
- **Consistency**: No hay duplicación de datos maestros
- **Easier updates**: Modificar template actualiza todas las instancias

## 🏗️ Implementación Paso a Paso

### Paso 1: Identificar Estado Intrínseco vs Extrínseco

**Estado Intrínseco (Compartido)**:
- Tipo de item
- Nombre y descripción
- Estadísticas base
- Sprite/icono
- Tipo de slot que ocupa
- Valor base

**Estado Extrínseco (Por Instancia)**:
- Durabilidad actual
- Propietario
- Posición en inventario
- Modificadores temporales
- Estado (equipado, en suelo, etc.)

### Paso 2: Crear la Interfaz Flyweight
```cpp
class ItemTemplate {
public:
    virtual ~ItemTemplate() = default;
    
    // Métodos que operan con estado extrínseco
    virtual int calculateCurrentValue(int durability) const = 0;
    virtual bool canBeUsedBy(const Character& character) const = 0;
    virtual std::string getDisplayName(int enhancement = 0) const = 0;
    
    // Getters para estado intrínseco
    virtual ItemType getType() const = 0;
    virtual const std::string& getName() const = 0;
    virtual int getBaseValue() const = 0;
    // ... más getters
};
```

### Paso 3: Implementar Flyweights Concretos
```cpp
class WeaponTemplate : public ItemTemplate {
private:
    ItemType type_;
    std::string name_;
    std::string description_;
    int baseValue_;
    int baseDamage_;
    SkillType requiredSkill_;
    
public:
    WeaponTemplate(ItemType type, std::string name, int damage, SkillType skill)
        : type_(type), name_(std::move(name)), baseDamage_(damage), requiredSkill_(skill) {}
    
    int calculateCurrentValue(int durability) const override {
        return baseValue_ * (durability / 100.0);
    }
    
    bool canBeUsedBy(const Character& character) const override {
        return character.getSkill(requiredSkill_) >= getRequiredSkill();
    }
};
```

### Paso 4: Crear Factory para Flyweights
```cpp
class ItemTemplateFactory {
private:
    static std::unordered_map<ItemType, std::shared_ptr<ItemTemplate>> templates_;
    static bool initialized_;
    
    static void initialize();
    
public:
    static std::shared_ptr<ItemTemplate> getTemplate(ItemType type) {
        if (!initialized_) initialize();
        
        auto it = templates_.find(type);
        if (it != templates_.end()) {
            return it->second;
        }
        
        // Create new template if not exists
        return createTemplate(type);
    }
    
private:
    static std::shared_ptr<ItemTemplate> createTemplate(ItemType type);
};
```

### Paso 5: Refactorizar Item para usar Context
```cpp
class Item {
private:
    std::shared_ptr<ItemTemplate> template_;  // Flyweight reference
    
    // Estado extrínseco
    int currentDurability_;
    Character* owner_;
    ItemSlotType currentSlot_;
    std::vector<Modifier> temporaryModifiers_;
    
public:
    Item(ItemType type) 
        : template_(ItemTemplateFactory::getTemplate(type))
        , currentDurability_(100)
        , owner_(nullptr) {}
    
    // Delegar a template + pasar estado extrínseco
    int getCurrentValue() const {
        return template_->calculateCurrentValue(currentDurability_);
    }
    
    const std::string& getName() const {
        return template_->getName();
    }
    
    // Métodos para estado extrínseco
    int getCurrentDurability() const { return currentDurability_; }
    void setDurability(int durability) { currentDurability_ = durability; }
};
```

## 📊 Análisis de Impacto

### Uso de Memoria: Antes vs Después

**Antes (sin Flyweight)**:
```
1000 Iron Swords = 1000 × sizeof(Item completo)
                 = 1000 × ~200 bytes = 200KB
```

**Después (con Flyweight)**:
```
1000 Iron Swords = 1 × ItemTemplate + 1000 × Context
                 = 150 bytes + 1000 × 50 bytes = 50KB
```

**Ahorro**: ~75% menos memoria!

### Comparación de Performance

| Aspecto | Sin Flyweight | Con Flyweight | Mejora |
|---------|---------------|---------------|---------|
| Memoria por 1000 items | 200KB | 50KB | 75% menos |
| Tiempo de carga | O(n) | O(1) después de init | Mucho más rápido |
| Cache misses | Alto | Bajo | Mejor locality |
| Consistency | Manual | Automática | Mantenimiento fácil |

## 🎯 Casos de Uso Ideales

### ✅ Cuándo Usar Flyweight
- **Muchos objetos similares**: Miles de items del mismo tipo
- **Estado compartido significativo**: Gran parte de los datos son comunes
- **Memoria limitada**: Aplicaciones con restricciones de memoria
- **Performance crítica**: Cache locality es importante

### ❌ Cuándo NO Usar Flyweight
- **Pocos objetos**: Overhead no justificado
- **Estado único**: Cada objeto es completamente diferente  
- **Complejidad innecesaria**: Simple applications don't need it
- **Modificaciones frecuentes**: Si templates cambian mucho

## 🔄 Extensiones y Variaciones

### Template Inheritance
```cpp
class ArmorTemplate : public ItemTemplate {
    int baseDefense_;
    ArmorType armorType_;
    // Specific armor logic
};

class ConsumableTemplate : public ItemTemplate {
    EffectType effect_;
    int effectDuration_;
    // Consumable-specific logic
};
```

### Enhanced Factory
```cpp
class ItemTemplateFactory {
public:
    // Support for modding/configuration
    static void loadTemplatesFromFile(const std::string& filename);
    
    // Runtime template creation
    static void registerTemplate(ItemType type, std::unique_ptr<ItemTemplate> template);
    
    // Memory management
    static void clearUnusedTemplates();
    static size_t getMemoryUsage();
};
```

## 🚀 Próximos Pasos

1. **Implementar**: Crear ItemTemplate hierarchy
2. **Refactorizar**: Migrar Item actual a usar flyweights
3. **Optimizar**: Medir impacto en memoria y performance
4. **Extender**: Aplicar a otros sistemas (spells, skills, etc.)

## 📝 Resumen de la Lección

El patrón Flyweight es esencial para optimizar memoria en juegos y aplicaciones que manejan muchos objetos similares. Al separar estado intrínseco (compartido) del extrínseco (único), podemos:

- **Reducir memoria**: Hasta 75% menos en casos típicos
- **Mejorar performance**: Mejor cache locality
- **Simplificar mantenimiento**: Datos centralizados
- **Facilitar escalabilidad**: Soportar miles de objetos

**Recordatorio clave**: Flyweight es sobre compartir datos comunes, no sobre reducir la cantidad de objetos. Cada item sigue existiendo, pero ahora es más ligero!

---

*Esta implementación del patrón Flyweight transformará significativamente el manejo de memoria de nuestro sistema de items, preparándolo para escalar a un RPG completo con miles de objetos simultáneos.*
