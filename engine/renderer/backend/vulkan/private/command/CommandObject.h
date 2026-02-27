#pragma once

namespace ly::renderer
{

class CommandObject
{
public:
    CommandObject() = default;

    CommandObject(CommandObject const&) = delete;
    CommandObject& operator=(CommandObject const&) = delete;

    CommandObject(CommandObject&&) = default;
    CommandObject& operator=(CommandObject&&) = default;

private:
};

} // namespace ly::renderer
