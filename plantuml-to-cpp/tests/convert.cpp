#include <gtest/gtest.h>
#include "plantuml-to-cpp/file_io.h"
#include "plantuml-to-cpp/converter.h"
#include "plantuml-to-cpp/file_pu.h"
#include "plantuml-to-cpp/file_cpp.h"

TEST(plantuml_to_cpp, convert_pu_to_cpp)
{
    const std::string test_file_path = "../../../data/test_file.pu";
    const std::string output_file_path = "../../../data/test_converted_output.cpp";

    // Read data from file
    fpos_t out_size = 0;
    std::unique_ptr<uint8_t[]> buffer = pu2cpp::ReadFileToBuffer(test_file_path, out_size);
    ASSERT_NE(buffer, nullptr);

    // Create a converter with a PuFileReader and CppFileWriter
	std::unique_ptr<pu2cpp::Converter> converter = nullptr;
	{
		// Create a file reader
		std::unique_ptr<pu2cpp::FileReader> reader = std::make_unique<pu2cpp::PuFileReader>();

		// Create a file writer
		std::unique_ptr<pu2cpp::FileWriter> writer = std::make_unique<pu2cpp::CppFileWriter>();

		// Create a converter with the reader and writer
		converter = std::make_unique<pu2cpp::Converter>(std::move(reader), std::move(writer));
	}

    // Convert the input file buffer to an output file buffer
    uint32_t output_file_size = 0;
    std::unique_ptr<uint8_t[]> output_buffer 
        = converter->Convert(std::move(buffer), out_size, output_file_size);
    ASSERT_NE(output_buffer, nullptr);

    // Write the output file buffer to the output file
    ASSERT_TRUE(pu2cpp::WriteBufferToFile(output_file_path, output_buffer.get(), output_file_size));
}