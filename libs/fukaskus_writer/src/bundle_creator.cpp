// #include "../include/bundle_creator.hpp"

// #include <iostream>

// #include "../include/bundle_writer_32.hpp"

// namespace FukaskusWriter
// {
    
//     BundleCreator::BundleCreator(std::string specFilePath, std::string outputFilePath, bool is64bit)
//     {
//         if (!is64bit)
//         {
//             _writer = std::make_unique<BundleWriter32>(specFilePath, outputFilePath);
//         }
//     }

//     void BundleCreator::write()
//     {
//         _writer->write();
//     }
// }
