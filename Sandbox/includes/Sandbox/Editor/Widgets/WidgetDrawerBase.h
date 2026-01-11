#pragma once

#include <string_view>

template <typename Derived>
class WidgetDrawerBase
{
public:
    template <typename T>
    void Draw(T& object)
    {
        static_cast<Derived*>(this)->Draw(object);
    }

protected:
    WidgetDrawerBase() = default;
};
