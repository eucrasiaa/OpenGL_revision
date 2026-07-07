#pragma once
#include <string>

class ICommandSource {
public:
    virtual ~ICommandSource() = default;
    virtual bool popCommand(std::string& out_cmd) = 0;
};
