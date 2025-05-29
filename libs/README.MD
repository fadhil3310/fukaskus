# libfukaskus

A C++ library to pack files into a single binary file, and to read the file inside the binary file. 

**Target C++17**

# How to Use

> Note: Fukaskus will throw an exception if there's something wrong happened when writing/reading (it only use exception from the `std::exception`).

## Bit Mode

Fukaskus allows you to write/read file in 32-bit or 64-bit mode. This is useful if you want to write to a filesystem that doesn't support 64-bit file (like FAT32).

**If you want to pack files that are larger than 4GB, then you must use 64-bit mode**

The writer/reader are seperated for 32-bit and 64-bit.

## Write

Use `libfukaskus-writer` to pack files into a binary file. 

To start writing, use `bundle_writer_32.hpp` for 32-bit or `bundle_writer_64.hpp` for 64-bit.

You just have to write a 2-line code like this:
```
FukaskusWriter::BundleWriter32 bundleWriter(inputPath, outputPath);
bundleWriter.write();
```

The `inputPath` is the path of the specification file. To see how the specification format is, read the `fukaskus` readme file.

The `outputPath` is the path of the binary file that will be generated.


## Read

Use `libfukaskus-reader` to read file from the generated binary file.

To start reading, use `bundle_reader_32.hpp` for 32-bit binary or `bundle_reader_64.hpp` for 64-bit binary.

There are 3 main functions that you can use inside a `BundleReader`:

- `std::optional<std::vector<char>> readFile(const std::string &filePath)` used to read a file inside the binary file, will return the data buffer if the file exist, otherwise `std::nullopt` if doesn't exist.
- `bool isExist(const std::string &filePath)` check if the file exist.
- `bool isFile(const std::string &filePath)` check if the provided path is a file.


To start reading a file, you can write a code like this:
```
FukaskusReader::BundleReader32 bundleReader(binaryPath);
std::optional<std::vector<char>> buffer = bundleReader.readFile(readPath);
if (!buffer)
{
    std::cout << "File not found\n";
}
```

> Note: If you reading a 64-bit file by using a 32-bit reader, or reading a 32-bit file by using a 64-bit reader, Fukaskus will fail to read the file and throws an exception.