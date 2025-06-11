#include "../include/fukaskus_writer/specification_reader.hpp"

#include <iostream>
#include <fstream>

using json = nlohmann::json;

namespace FukaskusWriter
{
    std::vector<Specification> SpecificationReader::read(const std::string &specFilePath)
    {
        std::ifstream file(specFilePath);
        if (!file.is_open())
        {
            throw std::runtime_error("Specification file (" + specFilePath + ") failed to open");
        }
        json data = json::parse(file);

        std::vector<Specification> specList;

        // std::string parentPathChild = "";
        for (auto &element : data)
        {
            auto spec = readElement(element /* , parentPathChild */);
            specList.push_back(spec);
        }

        file.close();
        return specList;
    }

    Specification SpecificationReader::readElement(const json &jsonData /* , const std::string &parentPath */)
    {
        Specification spec;

        // Set spec name
        // Limit name to 255 characters
        if (((std::string)jsonData["name"]).length() > 254)
        {
            throw std::invalid_argument("File name must not be longer than 255 characters: " + (std::string)jsonData["name"]);
        }
        spec.name = jsonData["name"];

        // Set spec children
        std::vector<Specification> specList;
        spec.children = std::move(specList);

        // std::string childParentPath = spec.name + "/";
        if (jsonData.contains("children"))
        {
            for (auto &element : jsonData["children"])
            {
                auto childSpec = readElement(element /* , childParentPath */);
                spec.children.push_back(childSpec);
            }
        }

        return spec;
    }
}