#pragma once

#include <string>
#include <memory>

namespace pu2cpp
{

class File
{
public:
    virtual ~File() = default;

    /**
     * @brief : Get the file extension
     * @return : The file extension
     */
    virtual std::string GetFileExt() const = 0;
};

class FileReader
{
public:
    virtual ~FileReader() = default;

    /**
     * @brief : Read binary data and create a File object
     * @param binary_data : The binary data to read
     * @param size : The size of the binary data
     * @return : A unique pointer to the created File object
     */
    virtual std::unique_ptr<File> Read(std::unique_ptr<uint8_t[]> binary_data, uint32_t size) const = 0;
};

class FileWriter
{
public:
    virtual ~FileWriter() = default;

    /**
     * @brief : Write a File object to binary data
     * @param file : The File object to write
     * @return std::unique_ptr<uint8_t[]> : Vector of bytes representing the binary data
     */
    virtual std::unique_ptr<uint8_t[]> Write(std::unique_ptr<File> file) const = 0;
};

} // namespace pu2cpp