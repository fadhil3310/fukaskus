#pragma once

#include <memory>
#include <string>
#include <vector>
#include <fstream>
#include <cstdint>

#include "bundle_writer.hpp"
#include "specification.hpp"

namespace FukaskusWriter
{    
    class BundleWriter32 : public IBundleWriter
    {
    private:
        std::vector<Specification> _specList;
        std::string _outputFilePath;
        uint32_t _bufferSize;

        std::ofstream _outputFile;
        uint32_t _writtenBytes;

        struct SpecificationQueue
        {
            const Specification *spec;
            bool isFile;
            uint32_t pos;
            uint32_t posStartPos; // Yes, its a terrible name
        };

    public:
        BundleWriter32(std::string specFilePath, std::string outputFilePath, uint32_t bufferSize = 5120 * 5120);
        BundleWriter32(std::vector<Specification> specList, std::string outputFilePath, uint32_t bufferSize = 5120 * 5120);

        void write() override;
        
        void setSpecFilePath(std::string specFilePath);
        void setSpecList(std::vector<Specification> specList);
        void setOutputFilePath(std::string outputFilePath);
        void setBufferSize(uint32_t bufferSize);

        std::vector<Specification> getSpecList() { return _specList; }
        std::string getOutputFilePath() { return _outputFilePath; }
        uint32_t getBufferSize() { return _bufferSize; }

    private:
        uint32_t writeTable(const uint32_t pos, const std::vector<Specification> &specList, const std::string &parentPath);
        uint32_t writeFile(const uint32_t pos, const std::string &filePath);

        void loadSpecList(const std::string &specFilePath);
        void checkOverflow(const uint32_t size);
    };
}