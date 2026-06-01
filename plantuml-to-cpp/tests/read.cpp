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

    // Create parsers for class names and members
    std::unique_ptr<pu2cpp::PuFileParser> class_name_parser = std::make_unique<pu2cpp::ClassNameParser>();
    std::unique_ptr<pu2cpp::PuFileParser> member_parser = std::make_unique<pu2cpp::MemberParser>();

    // Create a ClassTree to hold the parsed class diagram
    std::unique_ptr<pu2cpp::ClassTree> class_tree = std::make_unique<pu2cpp::ClassTree>();

    uint8_t* iterator = buffer.get();
    uint8_t* methods_start = nullptr;
    uint8_t* variables_start = nullptr;

    std::string str;
    for (size_t i = 0; i < out_size; ++i)
    {
        

        
    }
}