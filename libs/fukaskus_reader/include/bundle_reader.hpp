#pragma once

#include <string>
#include <vector>
#include <optional>

namespace FukaskusReader
{
    class IBundleReader
    {
    public:
        virtual void initialize() = 0;
        virtual std::optional<std::vector<char>> readFile(const std::string &filePath) = 0;

        virtual bool isExist(const std::string &filePath) = 0;
        virtual bool isFile(const std::string &filePath) = 0;
    };
}