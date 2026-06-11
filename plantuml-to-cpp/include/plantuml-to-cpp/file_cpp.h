#pragma once

#include "plantuml-to-cpp/file.h"

namespace pu2cpp
{

const std::string CPP_FILE_INDENT = "    ";
const std::string CPP_FILE_DOXYGEN_START_SYMBOL = "/**\n";
const std::string CPP_FILE_DOXYGEN_END_SYMBOL = " */\n";
const std::string CPP_FILE_DOXYGEN_COMMENT_SYMBOL = " * ";
const std::string CPP_FILE_COMMENT_SYMBOL = "// ";

class CppFileWriter :
    public FileWriter
{
public:
    CppFileWriter() = default;
    ~CppFileWriter() override = default;

    /**
     * @brief : Write a File object to binary data
     * @param file : The File object to write
     * @param size : Output parameter that will hold the size of the binary data
     * @return std::unique_ptr<uint8_t[]> : Vector of bytes representing the binary data
     */
    std::unique_ptr<uint8_t[]> Write(std::unique_ptr<File> file, uint32_t& size) const override;
};

} // namespace pu2cpp