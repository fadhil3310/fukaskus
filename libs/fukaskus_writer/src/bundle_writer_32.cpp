#include "../include/fukaskus_writer/bundle_writer_32.hpp"

#include <filesystem>
#include <iostream>

#include "../include/fukaskus_writer/specification_reader.hpp"

namespace fs = std::filesystem;

namespace FukaskusWriter
{
    //
    // ============================
    // ===== PUBLIC FUNCTIONS =====
    // ============================
    //

    BundleWriter32::BundleWriter32(std::string specFilePath, std::string outputFilePath, uint32_t bufferSize)
    {
        loadSpecList(specFilePath);
        _outputFilePath = outputFilePath;
        _bufferSize = bufferSize;
    }

    BundleWriter32::BundleWriter32(std::vector<Specification> specList, std::string outputFilePath, uint32_t bufferSize)
    {
        _specList = std::move(specList);
        _outputFilePath = outputFilePath;
        _bufferSize = bufferSize;
    }

    /// @brief Start bundling provided files into a binary file in the provided output file path
    void BundleWriter32::write()
    {
        _outputFile.open(_outputFilePath, std::ios::out | std::ios::binary);
        if (!_outputFile.is_open())
        {
            throw std::runtime_error("Output file (" + _outputFilePath + ") failed to open");
        }

        _writtenBytes = 0;
        // uint8_t emptyByte = 0;
        // _outputFile.write(reinterpret_cast<char*>(&emptyByte), sizeof(uint8_t));
        // _outputFile.seekp(1);

        bool flag64bit = 0;
        _outputFile.write(reinterpret_cast<char*>(&flag64bit), sizeof(bool));
        _writtenBytes += sizeof(bool);

        writeTable(_writtenBytes, _specList, "");

        _outputFile.close();
    }

    //
    // ==========================
    // ========= SETTER =========
    // ==========================
    //

    void BundleWriter32::setSpecFilePath(std::string specFilePath)
    {
        loadSpecList(specFilePath);
    }
    void BundleWriter32::setSpecList(std::vector<Specification> specList)
    {
        _specList = std::move(specList);
    }
    void BundleWriter32::setOutputFilePath(std::string outputFilePath)
    {
        _outputFilePath = outputFilePath;
    }
    void BundleWriter32::setBufferSize(uint32_t bufferSize)
    {
        _bufferSize = bufferSize;
    }

    //
    // ===========================
    // ==== PRIVATE FUNCTIONS ====
    // ===========================
    //

    uint32_t BundleWriter32::writeTable(const uint32_t pos, const std::vector<Specification> &specList, const std::string &parentPath)
    {
        _outputFile.seekp(pos);

        std::vector<SpecificationQueue> afterWriteQueue;

        // Write how much item is in the table
        uint16_t specListSize = specList.size();
        checkOverflow(sizeof specListSize);
        _outputFile.write(reinterpret_cast<char *>(&specListSize), sizeof(uint16_t));
        _writtenBytes += sizeof(uint16_t);

        for (auto &spec : specList)
        {
            std::string rawPath;
            if (!parentPath.empty())
            {
                rawPath.append(parentPath + "/");
            }
            rawPath.append(spec.name);

            fs::path path(rawPath);
            if (!fs::exists(path))
            {
                throw std::invalid_argument(rawPath + " doesn't exist");
            }

            SpecificationQueue queue{
                .spec = &spec,
                .pos = (uint32_t)_outputFile.tellp(),
            };

            // isFile flag
            bool flagIsFile = !fs::is_directory(path);

            // std::cout << "Name: " << spec.name.c_str() << "\n";

            // nameSize and name
            uint8_t nameSize = spec.name.length();

            // Check first if output file exceeds 32-bit
            uint32_t tableSize =
                sizeof flagIsFile +
                sizeof nameSize +
                nameSize +
                sizeof(uint32_t) * (flagIsFile ? 2 : 1); // startOffset/size
            checkOverflow(tableSize);

            // Write isFile flag
            _outputFile.write(reinterpret_cast<char *>(&flagIsFile), sizeof(bool));
            queue.isFile = flagIsFile;

            // Write nameSize and name
            _outputFile.write(reinterpret_cast<char *>(&nameSize), sizeof(nameSize));
            _outputFile.write(spec.name.c_str(), nameSize);

            // Skip writing startOffset and size,
            // because it will be written after writing file (if the spec is a file).
            // For directory, size will always be skipped as its not part of the directory table attribute.
            queue.posStartPos = _outputFile.tellp();
            if (flagIsFile)
            {
                uint32_t skipWriteByte = 0;
                _outputFile.write(reinterpret_cast<char *>(&skipWriteByte), sizeof(uint32_t));
                _outputFile.write(reinterpret_cast<char *>(&skipWriteByte), sizeof(uint32_t));
            }
            else
            {
                uint32_t skipWriteByte = 0;
                _outputFile.write(reinterpret_cast<char *>(&skipWriteByte), sizeof(uint32_t));
            }

            _writtenBytes += tableSize;
            afterWriteQueue.push_back(std::move(queue));
        }

        uint32_t afterTablesPos = _outputFile.tellp();

        // Write files
        for (auto &queue : afterWriteQueue)
        {
            if (!queue.isFile)
                continue;

            std::string filePath;
            if (!parentPath.empty())
            {
                filePath.append(parentPath + "/");
            }
            filePath.append(queue.spec->name);

            uint32_t startPos = _outputFile.tellp();
            uint32_t size = writeFile(startPos, filePath);
            uint32_t endPos = _outputFile.tellp();

            // Check if writing spec file to the output file will exceeds 32-bit limit
            // std::cout << "Written file size: " << size << "\n";
            // checkOverflow(size);

            _outputFile.seekp(queue.posStartPos);
            _outputFile.write(reinterpret_cast<char *>(&startPos), sizeof(uint32_t));
            _outputFile.write(reinterpret_cast<char *>(&size), sizeof(uint32_t));
            _outputFile.seekp(endPos);
            _writtenBytes += sizeof(uint32_t) * 2;
        }

        // Write directories (by creating table again)
        for (auto &queue : afterWriteQueue)
        {
            if (queue.isFile)
                continue;

            std::string childParentPath;
            if (!parentPath.empty())
            {
                childParentPath.append(parentPath + "/");
            }
            childParentPath.append(queue.spec->name);

            uint32_t startPos = _outputFile.tellp();
            uint32_t size = writeTable(startPos, queue.spec->children, childParentPath);
            uint32_t endPos = _outputFile.tellp();

            // Check if writing table to the output file will exceeds 32-bit limit
            checkOverflow(size);

            _outputFile.seekp(queue.posStartPos);
            _outputFile.write(reinterpret_cast<char *>(&startPos), sizeof(uint32_t));
            _outputFile.seekp(endPos);
            _writtenBytes += sizeof(uint32_t);
        }

        uint32_t accumulatedSize = (uint32_t)_outputFile.tellp() - pos - 1;
        return accumulatedSize;
    }

    uint32_t BundleWriter32::writeFile(const uint32_t pos, const std::string &filePath)
    {
        std::ifstream inputFile(filePath, std::ifstream::binary);
        if (!inputFile.is_open())
        {
            throw std::runtime_error("File (" + filePath + ") failed to open");
        }

        uint32_t fileSize = 0;

        _outputFile.seekp(pos);
        std::vector<char> buffer(_bufferSize, 0);

        while (!inputFile.eof())
        {
            inputFile.read(buffer.data(), buffer.size());

            std::streamsize size = inputFile.gcount();
            // std::cout << size << " | " << (uint32_t)(fileSize + size) << " | " << (uint32_t)fileSize << "\n";
            checkOverflow(size);
            // if ((uint32_t)(fileSize + size) < (uint32_t)fileSize)
            // {
            //     throw std::invalid_argument("File (" + filePath + ") is larger than 4GB, which exceeds 32-bit limit");
            // }

            _outputFile.write(buffer.data(), size);
            fileSize += size;
            _writtenBytes += size;
        }

        inputFile.close();
        return (uint32_t)fileSize;
    }

    void BundleWriter32::loadSpecList(const std::string &specFilePath)
    {
        SpecificationReader specReader;
        _specList = specReader.read(specFilePath);
    }

    void BundleWriter32::checkOverflow(const uint32_t size)
    {
        if ((uint32_t)(_writtenBytes + size) < (uint32_t)_writtenBytes)
        {
            throw std::runtime_error("Written output file exceeds 32-bit limit");
        }
    }
}