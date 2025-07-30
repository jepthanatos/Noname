#ifndef __EVENT_SUBJECT_H__
#define __EVENT_SUBJECT_H__

#include "EventObserver.h"
#include "LogManager.h"
#include <vector>
#include <memory>
#include <algorithm>
#include <functional>

namespace noname
{
    /**
     * @brief Clase EventSubject - Subject del Observer Pattern
     * 
     * Permite que objetos se suscriban para recibir notificaciones
     * de eventos. Maneja una lista de observers y los notifica cuando
     * ocurren eventos.
     */
    class EventSubject {
    private:
        std::vector<std::weak_ptr<EventObserver>> observers_;
        bool notificationInProgress_;
        
        /**
         * @brief Limpia observers que ya no existen (weak_ptr expirados)
         */
        void cleanupObservers() {
            if (notificationInProgress_) {
                return; // No limpiar durante notificación para evitar problemas
            }
            
            observers_.erase(
                std::remove_if(observers_.begin(), observers_.end(),
                    [](const std::weak_ptr<EventObserver>& obs) {
                        return obs.expired();
                    }),
                observers_.end()
            );
        }
        
        /**
         * @brief Ordena observers por prioridad
         */
        void sortObserversByPriority() {
            std::sort(observers_.begin(), observers_.end(),
                [](const std::weak_ptr<EventObserver>& a, const std::weak_ptr<EventObserver>& b) {
                    auto lockedA = a.lock();
                    auto lockedB = b.lock();
                    
                    if (!lockedA && !lockedB) return false;
                    if (!lockedA) return false; // B tiene prioridad
                    if (!lockedB) return true;  // A tiene prioridad
                    
                    return lockedA->getPriority() < lockedB->getPriority();
                });
        }
        
    public:
        EventSubject() : notificationInProgress_(false) {}
        
        virtual ~EventSubject() = default;
        
        /**
         * @brief Suscribe un observer para recibir eventos
         * @param observer Observer a suscribir
         */
        void subscribe(std::shared_ptr<EventObserver> observer) {
            if (!observer) {
                LM.writeLog(Level::Warning, "Attempted to subscribe null observer");
                return;
            }
            
            // Verificar que no esté ya suscrito
            for (const auto& existingObs : observers_) {
                if (auto locked = existingObs.lock()) {
                    if (locked.get() == observer.get()) {
                        LM.writeLog(Level::Debug, "Observer " + observer->getObserverId() + " already subscribed");
                        return;
                    }
                }
            }
            
            observers_.push_back(observer);
            sortObserversByPriority();
            
            LM.writeLog(Level::Debug, "Observer " + observer->getObserverId() + " subscribed");
        }
        
        /**
         * @brief Desuscribe un observer
         * @param observer Observer a desuscribir
         */
        void unsubscribe(std::shared_ptr<EventObserver> observer) {
            if (!observer) {
                return;
            }
            
            auto removed = std::remove_if(observers_.begin(), observers_.end(),
                [&observer](const std::weak_ptr<EventObserver>& obs) {
                    auto locked = obs.lock();
                    return !locked || locked.get() == observer.get();
                });
            
            bool wasRemoved = (removed != observers_.end());
            observers_.erase(removed, observers_.end());
            
            if (wasRemoved) {
                LM.writeLog(Level::Debug, "Observer " + observer->getObserverId() + " unsubscribed");
            }
        }
        
        /**
         * @brief Desuscribe todos los observers
         */
        void unsubscribeAll() {
            size_t count = observers_.size();
            observers_.clear();
            LM.writeLog(Level::Debug, "All observers unsubscribed (" + std::to_string(count) + " removed)");
        }
        
        /**
         * @brief Notifica a todos los observers sobre un evento
         * @param event Evento a notificar
         */
        void notifyObservers(const Event& event) {
            notificationInProgress_ = true;
            
            // Crear una copia de la lista para evitar problemas si se modifican observers durante notificación
            auto observersCopy = observers_;
            
            int notifiedCount = 0;
            int errorCount = 0;
            
            for (auto& weakObs : observersCopy) {
                if (auto obs = weakObs.lock()) {
                    // Verificar si el observer está activo y maneja este tipo de evento
                    if (!obs->isActive() || !obs->handlesEventType(event.getType())) {
                        continue;
                    }
                    
                    try {
                        obs->onEvent(event);
                        notifiedCount++;
                    } catch (const std::exception& e) {
                        errorCount++;
                        LM.writeLog(Level::Error, 
                            "Observer " + obs->getObserverId() + " error: " + std::string(e.what()));
                    } catch (...) {
                        errorCount++;
                        LM.writeLog(Level::Error, 
                            "Observer " + obs->getObserverId() + " unknown error");
                    }
                }
            }
            
            notificationInProgress_ = false;
            
            // Limpiar observers expirados después de la notificación
            cleanupObservers();
            
            if (notifiedCount > 0 || errorCount > 0) {
                LM.writeLog(Level::Debug, 
                    "Event " + std::string(eventTypeToString(event.getType())) + 
                    " notified to " + std::to_string(notifiedCount) + " observers" +
                    (errorCount > 0 ? " (" + std::to_string(errorCount) + " errors)" : ""));
            }
        }
        
        /**
         * @brief Obtiene el número de observers activos
         * @return Número de observers que no han expirado
         */
        size_t getObserverCount() const {
            return std::count_if(observers_.begin(), observers_.end(),
                [](const std::weak_ptr<EventObserver>& obs) {
                    return !obs.expired();
                });
        }
        
        /**
         * @brief Obtiene el número de observers activos de un tipo específico
         * @param eventType Tipo de evento
         * @return Número de observers que manejan este tipo de evento
         */
        size_t getObserverCountForEventType(EventType eventType) const {
            return std::count_if(observers_.begin(), observers_.end(),
                [eventType](const std::weak_ptr<EventObserver>& obs) {
                    auto locked = obs.lock();
                    return locked && locked->isActive() && locked->handlesEventType(eventType);
                });
        }
        
        /**
         * @brief Obtiene lista de IDs de observers activos
         * @return Vector con IDs de observers
         */
        std::vector<std::string> getObserverIds() const {
            std::vector<std::string> ids;
            for (const auto& weakObs : observers_) {
                if (auto obs = weakObs.lock()) {
                    ids.push_back(obs->getObserverId());
                }
            }
            return ids;
        }
        
        /**
         * @brief Verifica si hay observers suscritos
         */
        bool hasObservers() const {
            return getObserverCount() > 0;
        }
    };
}

#endif // __EVENT_SUBJECT_H__
