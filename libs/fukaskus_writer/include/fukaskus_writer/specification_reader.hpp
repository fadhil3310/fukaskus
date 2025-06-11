#pragma once

#include <string>
#include <vector>
#include <nlohmann/json.hpp>

#include "specification.hpp"


namespace FukaskusWriter
{      
    class SpecificationReader
    {
    private:


    public:
        std::vector<Specification> read(const std::string &specFilePath);

    private:
        Specification readElement(const nlohmann::json &jsonData/* , const std::string &parentPath */);
    };
}