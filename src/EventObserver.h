#ifndef __EVENT_OBSERVER_H__
#define __EVENT_OBSERVER_H__

#include "Event.h"
#include <string>

namespace noname
{
    /**
     * @brief Interfaz EventObserver - Observer Pattern
     * 
     * Define la interfaz que deben implementar todos los observers
     * que quieren recibir notificaciones de eventos.
     */
    class EventObserver {
    public:
        virtual ~EventObserver() = default;
        
        /**
         * @brief Método llamado cuando ocurre un evento
         * @param event El evento que ha ocurrido
         */
        virtual void onEvent(const Event& event) = 0;
        
        /**
         * @brief Obtiene un identificador único del observer
         * @return String que identifica únicamente este observer
         */
        virtual std::string getObserverId() const {
            return "Observer_" + std::to_string(reinterpret_cast<uintptr_t>(this));
        }
        
        /**
         * @brief Verifica si este observer maneja un tipo específico de evento
         * @param eventType Tipo de evento a verificar
         * @return true si maneja este tipo de evento
         * 
         * Implementación por defecto: maneja todos los eventos.
         * Los observers pueden sobrescribir este método para filtrar eventos.
         */
        virtual bool handlesEventType(EventType eventType) const {
            return true; // Por defecto maneja todos los eventos
        }
        
        /**
         * @brief Obtiene la prioridad del observer
         * @return Prioridad (menor número = mayor prioridad)
         * 
         * Los observers con mayor prioridad reciben eventos primero.
         * Útil para casos donde el orden de procesamiento importa.
         */
        virtual int getPriority() const {
            return 100; // Prioridad media por defecto
        }
        
        /**
         * @brief Verifica si el observer está activo
         * @return true si debe recibir eventos
         */
        virtual bool isActive() const {
            return true; // Activo por defecto
        }
    };
}

#endif // __EVENT_OBSERVER_H__
