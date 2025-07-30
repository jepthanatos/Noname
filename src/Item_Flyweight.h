#ifndef __ITEM_H__
#define __ITEM_H__

// System includes
#include <string>
#include <memory>
#include <atomic>

// Local includes
#include "ItemEnumTypes.h"
#include "ItemTemplate.h"
#include "ItemTemplateFactory.h"

#define ID_ITEM int

namespace noname
{
    inline constexpr int NULL_ITEM = -1;

    /**
     * @brief Clase Item refactorizada usando el patrón Flyweight
     * 
     * Ahora Item actúa como Context en el patrón Flyweight:
     * - template_: Referencia al flyweight (estado intrínseco compartido)
     * - Estado extrínseco: ID, durabilidad, owner, etc. (único por instancia)
     */
    class Item
    {
    protected:
        ID_ITEM _id;
        std::shared_ptr<ItemTemplate> template_;  // Flyweight reference
        
        // Estado extrínseco (único por instancia)
        int currentDurability_;     // 0-100
        int enhancement_;           // +0, +1, +2, etc.
        int currentUses_;          // Para items con usos limitados
        
        // Generar ID único por instancia
        auto generateId()
        {
            static std::atomic_int cont;
            return cont++;
        }

    public:
        /**
         * @brief Constructor principal usando ItemType (recomendado)
         * @param type Tipo de item - el template se obtiene automáticamente
         * @param durability Durabilidad inicial (0-100)
         * @param enhancement Nivel de mejora (+0, +1, +2, etc.)
         */
        Item(ItemType type, int durability = 100, int enhancement = 0)
            : _id{generateId()}
            , template_{ItemTemplateFactory::getTemplate(type)}
            , currentDurability_{durability}
            , enhancement_{enhancement}
            , currentUses_{-1}  // -1 = unlimited uses
        {
            if (!template_) {
                throw std::runtime_error("Failed to create template for ItemType: " + 
                                       std::to_string(static_cast<int>(type)));
            }
        }

        /**
         * @brief Constructor avanzado con template específico
         * @param templatePtr Template específico a usar
         * @param durability Durabilidad inicial
         * @param enhancement Nivel de mejora
         */
        Item(std::shared_ptr<ItemTemplate> templatePtr, int durability = 100, int enhancement = 0)
            : _id{generateId()}
            , template_{std::move(templatePtr)}
            , currentDurability_{durability}
            , enhancement_{enhancement}
            , currentUses_{-1}
        {
            if (!template_) {
                throw std::runtime_error("Template pointer cannot be null");
            }
        }

        /**
         * @brief Constructor legacy para compatibilidad (DEPRECATED)
         * @deprecated Use Item(ItemType) instead
         */
        [[deprecated("Use Item(ItemType) constructor instead")]]
        Item(const std::string &name, ItemType type, int durability = 100)
            : Item(type, durability, 0) 
        {
            // Este constructor mantiene compatibilidad pero usa el nuevo sistema
        }

        virtual ~Item() = default;

        // Getters básicos
        int getId() const { return _id; }
        
        // Delegar al template para estado intrínseco
        virtual std::string getName() const { 
            return template_->getDisplayName(enhancement_); 
        }
        
        ItemType getItemType() const { 
            return template_->getType(); 
        }
        
        const std::string& getDescription() const {
            return template_->getDescription();
        }
        
        ItemSlotType getSlotType() const {
            return template_->getSlotType();
        }
        
        // Métodos que combinan estado intrínseco + extrínseco
        int getCurrentValue() const { 
            return template_->calculateCurrentValue(currentDurability_); 
        }
        
        int getBaseValue() const {
            return template_->getBaseValue();
        }
        
        virtual short getWeight() const { 
            // Peso podría verse afectado por enhancement
            return 10; // Valor por defecto - podría mejorarse en el futuro
        }

        // Getters/Setters para estado extrínseco
        int getCurrentDurability() const { return currentDurability_; }
        void setCurrentDurability(int durability) { 
            currentDurability_ = std::max(0, std::min(100, durability)); 
        }
        
        int getEnhancement() const { return enhancement_; }
        void setEnhancement(int enhancement) { 
            enhancement_ = std::max(0, enhancement); 
        }
        
        int getCurrentUses() const { return currentUses_; }
        void setCurrentUses(int uses) { currentUses_ = uses; }
        
        // Métodos de acción
        bool canBeUsedBy(const Character& character) const {
            return template_->canBeUsedBy(character);
        }
        
        void useItem() {
            if (currentUses_ > 0) {
                --currentUses_;
            }
            // También podría reducir durabilidad
            if (currentDurability_ > 0) {
                --currentDurability_;
            }
        }
        
        void repair(int amount = 100) {
            currentDurability_ = std::min(100, currentDurability_ + amount);
        }
        
        void enhance(int levels = 1) {
            enhancement_ += levels;
        }
        
        // Información detallada
        std::string getDetailedInfo() const {
            return template_->getDetailedInfo(currentDurability_, enhancement_);
        }
        
        // Operators
        virtual bool operator==(const Item &item) const {
            return _id == item._id;
        }
        
        // Acceso al template (para casos especiales)
        std::shared_ptr<ItemTemplate> getTemplate() const {
            return template_;
        }
        
        // Métodos de debug/statistics
        std::string getMemoryInfo() const {
            return "Item ID: " + std::to_string(_id) + 
                   ", Template shared with " + std::to_string(template_.use_count() - 1) + 
                   " other items";
        }
    };

    /**
     * @brief NullItem refactorizado usando Flyweight
     */
    class NullItem
    {
    public:
        static std::shared_ptr<Item> getInstance()
        {
            static std::shared_ptr<Item> instance{nullptr};
            if (!instance) {
                // Crear un template especial para null items
                auto nullTemplate = std::make_shared<ConsumableTemplate>(
                    ItemType::NO_TYPE, "Null Item", "An empty slot.", 0, 0, 0, false
                );
                ItemTemplateFactory::registerTemplate(ItemType::NO_TYPE, nullTemplate);
                instance = std::make_shared<Item>(ItemType::NO_TYPE, 0, 0);
            }
            return instance;
        }

    private:
        NullItem(const NullItem &) = delete;
        NullItem(NullItem &&) = delete;
        NullItem &operator=(const NullItem &) = delete;
        NullItem &operator=(NullItem &&) = delete;
    };
}

#endif // __ITEM_H__
