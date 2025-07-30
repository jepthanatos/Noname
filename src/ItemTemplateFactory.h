#ifndef __ITEM_TEMPLATE_FACTORY_H__
#define __ITEM_TEMPLATE_FACTORY_H__

#include "ItemTemplate.h"
#include "ItemEnumTypes.h"
#include <memory>
#include <unordered_map>

namespace noname
{
    /**
     * @brief Factory para crear y gestionar ItemTemplates (Flyweight Factory)
     * 
     * Garantiza que solo existe una instancia de template por ItemType,
     * implementando el patrón Flyweight para optimizar memoria.
     */
    class ItemTemplateFactory 
    {
    private:
        static std::unordered_map<ItemType, std::shared_ptr<ItemTemplate>> templates_;
        static bool initialized_;
        
        // Inicializar templates predefinidos
        static void initialize();
        
        // Crear templates específicos
        static std::shared_ptr<ItemTemplate> createWeaponTemplate(ItemType type);
        static std::shared_ptr<ItemTemplate> createArmorTemplate(ItemType type);
        static std::shared_ptr<ItemTemplate> createConsumableTemplate(ItemType type);
        
        // Constructor privado para patrón Singleton
        ItemTemplateFactory() = default;
        
    public:
        /**
         * @brief Obtiene el template para un tipo de item específico
         * @param type Tipo de item
         * @return Shared pointer al template (flyweight)
         */
        static std::shared_ptr<ItemTemplate> getTemplate(ItemType type) {
            if (!initialized_) {
                initialize();
            }
            
            auto it = templates_.find(type);
            if (it != templates_.end()) {
                return it->second;
            }
            
            // Si no existe, crear nuevo template
            auto newTemplate = createTemplate(type);
            if (newTemplate) {
                templates_[type] = newTemplate;
            }
            
            return newTemplate;
        }
        
        /**
         * @brief Registra un template personalizado
         * @param type Tipo de item
         * @param template Template a registrar
         */
        static void registerTemplate(ItemType type, std::shared_ptr<ItemTemplate> templatePtr) {
            templates_[type] = std::move(templatePtr);
        }
        
        /**
         * @brief Obtiene el número de templates cargados
         * @return Cantidad de templates únicos en memoria
         */
        static size_t getTemplateCount() {
            return templates_.size();
        }
        
        /**
         * @brief Libera templates no utilizados (optimización de memoria)
         */
        static void clearUnusedTemplates() {
            auto it = templates_.begin();
            while (it != templates_.end()) {
                if (it->second.use_count() == 1) { // Solo la factory tiene referencia
                    it = templates_.erase(it);
                } else {
                    ++it;
                }
            }
        }
        
        /**
         * @brief Limpia todos los templates (para shutdown)
         */
        static void clear() {
            templates_.clear();
            initialized_ = false;
        }
        
        /**
         * @brief Obtiene estadísticas de uso de memoria
         * @return Información sobre templates cargados
         */
        static std::string getMemoryStats() {
            size_t totalTemplates = templates_.size();
            size_t totalReferences = 0;
            
            for (const auto& pair : templates_) {
                totalReferences += pair.second.use_count();
            }
            
            return "Templates loaded: " + std::to_string(totalTemplates) + 
                   ", Total references: " + std::to_string(totalReferences);
        }
        
    private:
        /**
         * @brief Crea un template para el tipo especificado
         * @param type Tipo de item
         * @return Template creado o nullptr si el tipo no es válido
         */
        static std::shared_ptr<ItemTemplate> createTemplate(ItemType type);
    };
}

#endif // __ITEM_TEMPLATE_FACTORY_H__
