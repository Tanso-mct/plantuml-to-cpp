#pragma once

#include <memory>

#include "plantuml-to-cpp/file.h"

namespace pu2cpp
{

class Converter
{
public:
    /**
     * @brief : Constructor for Converter with a FileReader and FileWriter
     * @param reader : The FileReader to use for reading files
     * @param writer : The FileWriter to use for writing files
     */
    Converter(std::unique_ptr<FileReader> reader, std::unique_ptr<FileWriter> writer);

    ~Converter() = default;

    /**
     * @brief : Convert binary data from one file format to another
     * @param binary_data : The binary data to convert
     * @param input_size : The size of the input binary data
     * @param output_size : Reference to a variable to store the size of the output binary
     * @return std::unique_ptr<uint8_t[]> : Vector of bytes representing the converted binary data
     */
    std::unique_ptr<uint8_t[]> Convert(
        std::unique_ptr<uint8_t[]> binary_data, uint32_t input_size, uint32_t& output_size) const;

private:
    // The FileReader to use for reading files
    std::unique_ptr<FileReader> reader_;

    // The FileWriter to use for writing files
    std::unique_ptr<FileWriter> writer_;
};

} // namespace pu2cpp