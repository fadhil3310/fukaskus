#pragma once

#include <string>
#include <vector>

namespace FukaskusWriter
{
    struct Specification
    {
        std::string name;
        std::vector<Specification> children;
    };
}
