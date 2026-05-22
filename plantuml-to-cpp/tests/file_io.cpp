#include <gtest/gtest.h>
#include "plantuml-to-cpp/file_io.h"

TEST(plantuml_to_cpp, read_write_file)
{
    const std::string test_file_path = "test_file.bin";
    const std::string test_data = "Hello, PlantUML to C++!";

    // Write test data to file
    bool write_success = pu2cpp::WriteBufferToFile(
        test_file_path, reinterpret_cast<const uint8_t*>(test_data.data()), static_cast<uint32_t>(test_data.size()));
    EXPECT_TRUE(write_success);

    // Read data back from file
    fpos_t out_size = 0;
    std::unique_ptr<uint8_t[]> buffer = pu2cpp::ReadFileToBuffer(test_file_path, out_size);
    EXPECT_NE(buffer, nullptr);
    EXPECT_EQ(out_size, test_data.size());

    // Verify the read data matches the original data
    std::string read_data(reinterpret_cast<char*>(buffer.get()), out_size);
    EXPECT_EQ(read_data, test_data);
}