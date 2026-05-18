#pragma once

#include "plantuml-to-cpp/file.h"

namespace pu2cpp
{

class CppFileWriter :
    public FileWriter
{
public:
    CppFileWriter() = default;
    ~CppFileWriter() override = default;

    /**
     * @brief : Write a File object to binary data
     * @param file : The File object to write
     * @return std::unique_ptr<uint8_t[]> : Vector of bytes representing the binary data
     */
    std::unique_ptr<uint8_t[]> Write(std::unique_ptr<File> file) const override;
};

} // namespace pu2cpp