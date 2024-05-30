#ifndef __ITEMS_MANAGER_H__
#define __ITEMS_MANAGER_H__

// Local includes
#include "Singleton.h"
#include "Manager.h"
#include "LogManager.h"
#include "Item.h"

// System includes
#include <vector>
#include <unordered_map>

// Two-letter acronym for easier access to manager
#define IM noname::ItemsManager::getInstance()

namespace noname
{
    class ItemsManager : public Manager, public Singleton<ItemsManager>
    {

    private:
        std::unordered_map<std::string, Item> _items;

    public:
        void startUp()
        {
            Manager::setType("ItemsManager");
            LM.writeLog(Level::Debug, "ItemsManager::startUp");
            Manager::startUp();
            initializeItems();
        }

        void shutDown()
        {
            _items.clear();
            Manager::shutDown();
            LM.writeLog(Level::Debug, "ItemsManager::shutDown");
        }

        void initializeItems()
        {
            _items.insert({"Rune", {"Rune", ItemType::USABLE, 20, 10, 5}});
        }

        std::unordered_map<std::string, Item> getItemsList() const { return _items; }

        Item getItem(const std::string &name) { return _items.find(name)->second; }
    };

}
#endif // __ITEMS_MANAGER_H__
