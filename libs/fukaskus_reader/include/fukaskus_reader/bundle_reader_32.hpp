#pragma once

#include <string>
#include <fstream>
#include <cstdint>
#include <optional>

#include "bundle_reader.hpp"

namespace FukaskusReader
{
    class BundleReader32 : public IBundleReader
    {
    private:
        bool _hasInitialized;
        std::string _binFilePath;
        std::ifstream _fileInput;

        uint32_t _fileSize;
        // uint32_t _bytesRead;
        
        struct TableEntry
        {
            uint32_t entryPos;

            bool isFile;

            uint8_t nameSize;
            std::string name;

            uint32_t targetPos;
            uint32_t targetSize;
        };

    public:
        BundleReader32(const std::string binFilePath);

        void initialize() override;
        std::optional<std::vector<char>> readFile(const std::string &filePath) override;

        bool isExist(const std::string &filePath) override;
        bool isFile(const std::string &filePath) override;

    private:
        std::optional<TableEntry> findEntry(const std::string &filePath);
        std::optional<TableEntry> findEntry(const std::vector<std::string> filePath, bool isFirstSearching);
        // uint32_t findFileByEntry(const uint32_t entryPos);
    };
}