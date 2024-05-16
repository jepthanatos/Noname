#include "Manager.h"

namespace noname
{
    Manager::Manager()
    {
        type = "None";
        started = false;
    }

    void Manager::setType(std ::string value)
    {
        type = value;
    }

    std::string Manager::getType() const
    {
        return type;
    }

    bool Manager::isStarted() const
    {
        return started;
    }
}
