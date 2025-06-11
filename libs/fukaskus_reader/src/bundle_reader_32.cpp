#include "../include/fukaskus_reader/bundle_reader_32.hpp"

#include <iostream>
#include <filesystem>
#include <vector>

namespace FukaskusReader
{
    BundleReader32::BundleReader32(const std::string binFilePath)
    {
        _binFilePath = binFilePath;
    }

    void BundleReader32::initialize()
    {
        _fileInput.open(_binFilePath, std::ios_base::binary);
        if (!_fileInput.is_open())
        {
            throw std::runtime_error("The provided binary file can't be opened");
        }

        // Check file size
        auto fileSize = std::filesystem::file_size(_binFilePath);
        if (fileSize == 0)
            throw std::invalid_argument("The provided binary file is empty");
        if (sizeof fileSize > std::numeric_limits<uint32_t>::max())
            throw std::invalid_argument("The provided binary file exceeds 32-bit limit");
        _fileSize = (uint32_t)fileSize;

        // Make sure input file is a 32-bit file
        bool flagIs64;
        if (!_fileInput.read(reinterpret_cast<char *>(&flagIs64), 1))
        {
            throw std::runtime_error("Can't read 64-bit flag");
        }
        // std::cout << "Is 64 bit: " << flagIs64 << "\n";
        if (flagIs64)
        {
            throw std::invalid_argument("The provided binary file is a 64-bit file");
        }

        _hasInitialized = true;
    }

    std::optional<std::vector<char>> BundleReader32::readFile(const std::string &filePath)
    {
        if (!_hasInitialized)
            initialize();

        std::optional<TableEntry> fileEntry = findEntry(filePath);
        if (!fileEntry)
            return std::nullopt;

        std::vector<char> buffer(fileEntry->targetSize);

        _fileInput.seekg(fileEntry->targetPos);
        _fileInput.read(buffer.data(), buffer.size());

        return buffer;
    }

    bool BundleReader32::isExist(const std::string &filePath)
    {
        if (!_hasInitialized)
            initialize();

        std::optional<TableEntry> fileEntry = findEntry(filePath);
        return (bool)fileEntry;
    }

    bool BundleReader32::isFile(const std::string &filePath)
    {
         if (!_hasInitialized)
            initialize();

        std::optional<TableEntry> fileEntry = findEntry(filePath);
        if (!fileEntry)
            return false;
            
        return fileEntry->isFile;
    }

    //
    // ===========================
    // ==== PRIVATE FUNCTIONS ====
    // ===========================
    //

    std::optional<BundleReader32::TableEntry> BundleReader32::findEntry(const std::string &filePath)
    {
        std::vector<std::string> splittedFilePath;
        size_t start = 0;
        size_t end = filePath.find("/");
        while (true)
        {
            std::string segment = filePath.substr(start, end - start);
            splittedFilePath.push_back(segment);

            if (end == std::string::npos)
                break;

            start += (end - start) + 1;
            end = filePath.find("/", start);
        }

        return findEntry(std::move(splittedFilePath), true);
    }

    std::optional<BundleReader32::TableEntry> BundleReader32::findEntry(const std::vector<std::string> filePath, bool isFirstSearching)
    {
        // Seek to first table position if this is the first time searching for entry
        if (isFirstSearching)
            _fileInput.seekg(1);

        uint16_t tableLength;
        _fileInput.read(reinterpret_cast<char *>(&tableLength), sizeof(uint16_t));

        for (int i = 0; i < tableLength; i++)
        {
            uint32_t entryPos = _fileInput.tellg();

            // Read isFile flag
            bool isFile;
            _fileInput.read(reinterpret_cast<char *>(&isFile), sizeof(bool));

            // Read nameSize
            uint8_t nameSize;
            _fileInput.read(reinterpret_cast<char *>(&nameSize), sizeof(nameSize));

            // Read name, size is based on nameSize
            std::string name(nameSize, '\0');
            _fileInput.read(reinterpret_cast<char *>(&name[0]), nameSize);

            if (filePath[0] == name)
            {
                if (filePath.size() == 1)
                {
                    // Read targetPos
                    uint32_t targetPos;
                    _fileInput.read(reinterpret_cast<char *>(&targetPos), sizeof(uint32_t));

                    TableEntry entryData{
                        .entryPos = entryPos,
                        .isFile = isFile,
                        .nameSize = nameSize,
                        .name = name,
                        .targetPos = targetPos};

                    if (isFile)
                    {
                        // Read targetSize
                        uint32_t targetSize;
                        _fileInput.read(reinterpret_cast<char *>(&targetSize), sizeof(uint32_t));
                        entryData.targetSize = targetSize;
                    }

                    return std::move(entryData);
                }
                else
                {
                    // Go to entry position
                    uint32_t targetEntryPos;
                    _fileInput.read(reinterpret_cast<char *>(&targetEntryPos), sizeof(uint32_t));
                    _fileInput.seekg(targetEntryPos);

                    // Cut file path to navigate to next path
                    std::vector splittedFilePath(filePath.begin() + 1, filePath.end());
                    return findEntry(std::move(splittedFilePath), false);
                }
            }

            // Skip pos, and also size (if entry is a file)
            _fileInput.seekg(_fileInput.tellg() + (sizeof(uint32_t) * (isFile ? 2 : 1)));
        }

        return std::nullopt;
    }

    // uint32_t BundleReader32::findFileByEntry(const uint32_t entryPos)
    // {
    // }
}
