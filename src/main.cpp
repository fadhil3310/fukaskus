
#include <iostream>
#include <fstream>
#include <iterator>
#include <cxxopts.hpp>

#include "../libs/fukaskus_writer/include/bundle_writer_32.hpp"
#include "../libs/fukaskus_reader/include/bundle_reader_32.hpp"

int main(int argc, char *argv[])
{
    cxxopts::Options options("FuKaskus Resource System", "Bundle your resource files into a single binary");
    options.add_options()                                                                                                                                                                                                                                                        //
        ("w,write", "Run in write mode, and the path of the generated binary file", cxxopts::value<std::string>())                                                                                                                                                               // Write mode, and the output file path
        ("r,read", "Run in read mode, and the file path in the binary file to be read", cxxopts::value<std::string>())                                                                                                                                                           // Read mode, and path in the binary file to be read
        ("i,input", "Path of the input file (the json specification if in write mode)", cxxopts::value<std::string>())                                                                                                                                                           // File input
        ("b,bit", "Write/read the binary file in 32-bit or 64-bit (32 for 32-bit, 64 for 64-bit). If in write mode, use 64-bit if you want to bundle files larger than 4GB as long as your filesystem supports 64-bit file", cxxopts::value<std::string>()->default_value("32")) // Write bit mode
        ("o,read-output-path", "Path of the output file if you want to output the bytes read from the binary file to disk, not to stdout (used in read mode)", cxxopts::value<std::string>())                                                                                    // Write bytes read from the binary file to a file
        ("h,help", "Print usage");                                                                                                                                                                                                                                               // Help

    cxxopts::ParseResult result;
    try
    {
        result = options.parse(argc, argv);
    }
    catch (const cxxopts::exceptions::parsing& e)
    {
        std::cerr << "Please provide appropriate options and arguments, see --help for details\n";
        exit(1);
    }

    if (result.count("help"))
    {
        std::cout << options.help() << "\n";
        exit(0);
    }

    bool isWriteMode;
    std::string writePath;
    std::string readPath;

    // Tell the user to run the program in write or read mode
    if (result.count("write") == 0 && result.count("read") == 0)
    {
        std::cerr << "Please run in either write or read mode (-w for write and -r for read, see --help for more information)";
        exit(1);
    }

    if (result.count("write")) // If the user wants to run in write mode
    {
        // Check first if the inputted write path is empty
        writePath = result["write"].as<std::string>();
        if (writePath.empty())
        {
            std::cerr << "Output path is empty, please provide the path after the 'write' parameter\n";
            exit(1);
        }
        isWriteMode = true;
    }
    if (result.count("read")) // If the user wants to run in read mode
    {
        if (isWriteMode)
        {
            std::cerr << "Don't run in write and read mode simultaneously\n";
            exit(1);
        }

        readPath = result["read"].as<std::string>();
        if (readPath.empty())
        {
            std::cerr << "Path of the file to be read inside the inputted binary file is empty, please provide the path after the 'read' parameter\n";
            exit(1);
        }
    }

    // Get the path of the file
    std::string inputPath;
    if (result.count("input"))
    {
        inputPath = result["input"].as<std::string>();
        if (inputPath.empty())
        {
            std::cerr << "No input file given\n";
            exit(1);
        }
    }
    else
    {
        std::cerr << "No input file given\n";
        exit(1);
    }

    // Check if the user wants to write/read in 64-bit
    bool is64bit;
    if (result["bit"].as<std::string>() == "64")
    {
        is64bit = true;
    }
    else if (result["bit"].as<std::string>() != "32")
    {
        std::cerr << "Set either 32 for 32-bit, or 64 for 64-bit\n";
        exit(1);
    }

    if (isWriteMode) // Write mode
    {
        try
        {
            // Start writing..
            if (!is64bit) // 32-bit
            {
                FukaskusWriter::BundleWriter32 bundleWriter(inputPath, writePath);
                bundleWriter.write();
            }
            else // 64-bit
            {
                std::cerr << "Sorry, writing in 64-bit isn't supported for now\n";
                exit(1);
            }
        }
        catch (const std::exception& e)
        {
            std::cerr << "Failed writing binary file\nReason: " << e.what() << "\n";
            exit(2);
        }
    }
    else // Read mode
    {
        // Check if the user wants to output the file after being read to disk
        bool isOutputToFile;
        std::string readOutputPath;
        if (result.count("read-output-path"))
        {
            readOutputPath = result["read-output-path"].as<std::string>();
            if (readOutputPath.empty())
            {
                std::cerr << "Output path to write to disk is empty\n";
                exit(1);
            }
            isOutputToFile = true;
        }

        // Start reading..
        std::optional<std::vector<char>> buffer;
        try
        {
            if (!is64bit) // 32-bit
            {
                FukaskusReader::BundleReader32 bundleReader(inputPath);
                buffer = bundleReader.readFile(readPath);
                if (!buffer)
                {
                    std::cout << "File not found\n";
                    exit(1);
                }
            }
            else // 64-bit
            {
                std::cerr << "Sorry, reading in 64-bit isn't supported for now\n";
                exit(1);
            }
        }
        catch (const std::exception& e)
        {
            std::cerr << "Failed reading file inside the binary file\nReason: " << e.what() << "\n";
            exit(2);
        }

        // Start outputting..
        try
        {
            if (isOutputToFile) // Write to disk
            {
                std::ofstream outputFile(readOutputPath, std::ios::binary);
                std::ostream_iterator<char> outputIterator(outputFile, "");
                std::copy(buffer->begin(), buffer->end(), outputIterator);
            }
            else // Write to stdout
            {
                for (auto &byte : buffer.value())
                {
                    std::cout << byte;
                }
            }
        }
        catch (const std::exception& e)
        {
            std::cerr << "Failed outputting bytes\nReason: " << e.what() << "\n";
            exit(2);
        }
    }
}