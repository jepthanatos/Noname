#ifndef __EVENT_H__
#define __EVENT_H__

#include "EventTypes.h"
#include <any>
#include <unordered_map>
#include <string>
#include <vector>
#include <stdexcept>

namespace noname
{
    /**
     * @brief Clase Event - Representa un evento del sistema
     * 
     * Encapsula información sobre eventos que ocurren en el juego.
     * Usa std::any para permitir datos tipados flexibles.
     */
    class Event {
    private:
        EventType type_;
        std::unordered_map<std::string, std::any> data_;
        
    public:
        /**
         * @brief Constructor
         * @param type Tipo de evento
         */
        explicit Event(EventType type) : type_(type) {}
        
        /**
         * @brief Obtiene el tipo de evento
         */
        EventType getType() const { return type_; }
        
        /**
         * @brief Agrega datos tipados al evento
         * @tparam T Tipo de dato
         * @param key Clave para el dato
         * @param value Valor del dato
         */
        template<typename T>
        void setData(const std::string& key, const T& value) {
            data_[key] = value;
        }
        
        /**
         * @brief Obtiene datos tipados del evento
         * @tparam T Tipo esperado del dato
         * @param key Clave del dato
         * @return Valor del dato
         * @throws std::runtime_error si la clave no existe o el tipo es incorrecto
         */
        template<typename T>
        T getData(const std::string& key) const {
            auto it = data_.find(key);
            if (it != data_.end()) {
                try {
                    return std::any_cast<T>(it->second);
                } catch (const std::bad_any_cast& e) {
                    throw std::runtime_error("Invalid data type for key '" + key + "': " + e.what());
                }
            }
            throw std::runtime_error("Key not found: " + key);
        }
        
        /**
         * @brief Obtiene datos con valor por defecto
         * @tparam T Tipo esperado del dato
         * @param key Clave del dato
         * @param defaultValue Valor por defecto si la clave no existe
         * @return Valor del dato o valor por defecto
         */
        template<typename T>
        T getDataOrDefault(const std::string& key, const T& defaultValue) const {
            try {
                return getData<T>(key);
            } catch (const std::runtime_error&) {
                return defaultValue;
            }
        }
        
        /**
         * @brief Verifica si existe una clave
         * @param key Clave a verificar
         * @return true si la clave existe
         */
        bool hasData(const std::string& key) const {
            return data_.find(key) != data_.end();
        }
        
        /**
         * @brief Obtiene todas las claves disponibles
         * @return Vector con todas las claves
         */
        std::vector<std::string> getKeys() const {
            std::vector<std::string> keys;
            keys.reserve(data_.size());
            for (const auto& pair : data_) {
                keys.push_back(pair.first);
            }
            return keys;
        }
        
        /**
         * @brief Obtiene el número de datos almacenados
         */
        size_t getDataCount() const {
            return data_.size();
        }
        
        /**
         * @brief Limpia todos los datos del evento
         */
        void clearData() {
            data_.clear();
        }
        
        /**
         * @brief Convierte el evento a string para debugging
         */
        std::string toString() const {
            std::string result = "Event[" + std::string(eventTypeToString(type_)) + "]";
            if (!data_.empty()) {
                result += " with data: ";
                for (const auto& pair : data_) {
                    result += pair.first + ", ";
                }
                // Remover la última coma y espacio
                if (result.size() >= 2) {
                    result.erase(result.size() - 2);
                }
            }
            return result;
        }
    };
}

#endif // __EVENT_H__
