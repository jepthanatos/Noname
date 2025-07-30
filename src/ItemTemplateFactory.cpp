#include "ItemTemplateFactory.h"
#include "LogManager.h"
#include <cassert>

namespace noname
{
    // Inicialización de variables estáticas
    std::unordered_map<ItemType, std::shared_ptr<ItemTemplate>> ItemTemplateFactory::templates_;
    bool ItemTemplateFactory::initialized_ = false;

    void ItemTemplateFactory::initialize() 
    {
        if (initialized_) return;
        
        LM.writeLog(Level::Info, "Initializing ItemTemplateFactory with predefined templates");
        
        // Pre-cargar algunos templates comunes para mejor performance
        // Esto reduce la latencia en runtime ya que los templates más usados
        // ya están disponibles
        
        // Armas básicas
        templates_[ItemType::WEAPON] = createWeaponTemplate(ItemType::WEAPON);
        
        // Armaduras básicas  
        templates_[ItemType::ARMOR] = createArmorTemplate(ItemType::ARMOR);
        
        // Consumibles básicos
        templates_[ItemType::USABLE] = createConsumableTemplate(ItemType::USABLE);
        
        initialized_ = true;
        
        LM.writeLog(Level::Info, "ItemTemplateFactory initialized with " + 
                   std::to_string(templates_.size()) + " predefined templates");
    }

    std::shared_ptr<ItemTemplate> ItemTemplateFactory::createTemplate(ItemType type) 
    {
        switch (type) {
            case ItemType::WEAPON:
                return createWeaponTemplate(type);
                
            case ItemType::ARMOR:
                return createArmorTemplate(type);
                
            case ItemType::USABLE:
                return createConsumableTemplate(type);
                
            default:
                LM.writeLog(Level::Warning, "Unknown ItemType requested: " + 
                           std::to_string(static_cast<int>(type)));
                return nullptr;
        }
    }

    std::shared_ptr<ItemTemplate> ItemTemplateFactory::createWeaponTemplate(ItemType type) 
    {
        // Datos base para diferentes tipos de armas
        // En un juego real, estos podrían cargarse desde archivos de configuración
        
        switch (type) {
            case ItemType::WEAPON:
                // Arma genérica - podría ser personalizada más tarde
                return std::make_shared<WeaponTemplate>(
                    ItemType::WEAPON,
                    "Basic Weapon",
                    "A simple weapon for basic combat.",
                    50,        // baseValue
                    8,         // baseDamage
                    6,         // dieSize (d6)
                    SkillType::CLUB,  // requiredSkill
                    1          // minRequiredSkillLevel
                );
            
            default:
                // Para extensibilidad futura - diferentes subtipos de armas
                return std::make_shared<WeaponTemplate>(
                    type,
                    "Unknown Weapon",
                    "A mysterious weapon of unknown origin.",
                    10, 5, 4, SkillType::CLUB, 1
                );
        }
    }

    std::shared_ptr<ItemTemplate> ItemTemplateFactory::createArmorTemplate(ItemType type) 
    {
        switch (type) {
            case ItemType::ARMOR:
                // Armadura genérica
                return std::make_shared<ArmorTemplate>(
                    ItemType::ARMOR,
                    "Basic Armor",
                    "Simple protective gear.",
                    100,       // baseValue
                    ItemSlotType::ARMOR,  // slotType
                    5,         // baseDefense
                    1          // minRequiredLevel
                );
            
            default:
                return std::make_shared<ArmorTemplate>(
                    type,
                    "Unknown Armor",
                    "Protective gear of unknown make.",
                    50, ItemSlotType::ARMOR, 2, 1
                );
        }
    }

    std::shared_ptr<ItemTemplate> ItemTemplateFactory::createConsumableTemplate(ItemType type) 
    {
        switch (type) {
            case ItemType::USABLE:
                // Consumible genérico - poción de vida básica
                return std::make_shared<ConsumableTemplate>(
                    ItemType::USABLE,
                    "Health Potion",
                    "Restores health when consumed.",
                    25,        // baseValue
                    50,        // healingAmount
                    0,         // manaAmount
                    true       // isSingleUse
                );
            
            default:
                return std::make_shared<ConsumableTemplate>(
                    type,
                    "Unknown Potion",
                    "A mysterious liquid with unknown effects.",
                    10, 10, 0, true
                );
        }
    }
}
