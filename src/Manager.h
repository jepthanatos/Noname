#pragma once

#include <string>
#include <string_view>

namespace noname
{
    class Manager
    {
    private:
        std::string _type{"None"};

    protected:
        bool _started{false};

        Manager() = default;

        void setType(std::string_view value) noexcept { _type = value; }

    public:
        virtual ~Manager() = default;

        [[nodiscard]] std::string_view getType() const noexcept { return _type; }

        virtual void startUp() noexcept { _started = true; }
        virtual void shutDown() noexcept { _started = false; }

        [[nodiscard]] bool isStarted() const noexcept { return _started; }
    };
}
