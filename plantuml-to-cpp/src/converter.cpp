#include "pch.h"
#include "plantuml-to-cpp/converter.h"

namespace pu2cpp
{

Converter::Converter(std::unique_ptr<FileReader> reader, std::unique_ptr<FileWriter> writer)
    : reader_(std::move(reader)), writer_(std::move(writer))
{
}

std::unique_ptr<uint8_t[]> Converter::Convert(
    std::unique_ptr<uint8_t[]> binary_data, uint32_t input_size, uint32_t& output_size) const
{
    // Use the reader to read the input binary data and create a File object
    std::unique_ptr<File> file = reader_->Read(std::move(binary_data), input_size);
    if (!file)
    {
        std::cerr << "Failed to read input binary data" << std::endl;
        return nullptr;
    }

    // Use the writer to write the File object to output binary data
    std::unique_ptr<uint8_t[]> output_binary_data = writer_->Write(std::move(file));
    if (!output_binary_data)
    {
        std::cerr << "Failed to write output binary data" << std::endl;
        return nullptr;
    }

    // Set the output size (this is just an example, you would need to determine the actual size)
    output_size = input_size; // This is a placeholder, you should set this to the actual size of the output binary data

    return output_binary_data;
}

} // namespace pu2cpp