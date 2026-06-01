#include <gtest/gtest.h>
#include "plantuml-to-cpp/file_io.h"
#include "plantuml-to-cpp/file_pu.h"

TEST(plantuml_to_cpp, read_pu_file)
{
    const std::string test_file_path = "../../../data/test_file.pu";

    // Read data from file
    fpos_t out_size = 0;
    std::unique_ptr<uint8_t[]> buffer = pu2cpp::ReadFileToBuffer(test_file_path, out_size);
    ASSERT_NE(buffer, nullptr);

    // Parse the buffer into a ClassTree
    std::unique_ptr<pu2cpp::ClassTree> class_tree = pu2cpp::ParsePuFile(buffer.get(), out_size);
    ASSERT_NE(class_tree, nullptr);
}