//==============================================================================
// Name        : Manager.cpp
// Author      : Jorge
// Copyright   : https://creativecommons.org/licenses/by/4.0/
// Description : Base class for managers.
//==============================================================================

#include "Manager.h"

//==============================================================================
/* Manager */
//==============================================================================

namespace noname
{
    /**
     * Default constuctor.
     */
    Manager::Manager()
    {
        type = "None";
        started = false;
    }

    /**
     * Set type identifier of Manager.
     */
    void Manager::setType(std ::string value)
    {
        type = value;
    }

    /**
     * Get type identifier of Manager.
     */
    std::string Manager::getType() const
    {
        return type;
    }

    /**
     * Return true when startUp() was executed ok, else false.
     */
    bool Manager::isStarted() const
    {
        return started;
    }
}
