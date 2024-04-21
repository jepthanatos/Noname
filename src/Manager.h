//==============================================================================
// Name        : Manager.h
// Author      : Jorge
// Copyright   : https://creativecommons.org/licenses/by/4.0/
// Description : Base class for managers.
//==============================================================================

#ifndef __MANAGER_H__
#define __MANAGER_H__

// System includes.
#include <string>

//==============================================================================
/* Manager */
//==============================================================================

/**
 * Base class for managers.
 */
namespace noname
{
    class Manager
    {
    private:
        std ::string type; // Manager type identifier.

    protected:
        bool started; // True when started sucessfully.

        // Default constructor.
        Manager();

        // Set type identifier of Manager.
        void setType(std ::string value);

    public:
        virtual ~Manager(){};

        // Get type identifier of Manager.
        std::string getType() const;

        // Startup Manager.
        virtual void startUp()
        {
            started = true;
        }

        // Shutdown Manager.
        virtual void shutDown()
        {
            started = false;
        }

        // Return true when startUp() was executed ok, else false.
        bool isStarted() const;
    };
}
//------------------------------------------------------------------------------
#endif /* __MANAGER_H__ */
