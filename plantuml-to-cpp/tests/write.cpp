#include <gtest/gtest.h>
#include "plantuml-to-cpp/file_io.h"
#include "plantuml-to-cpp/file_cpp.h"

namespace pu2cpp
{

class TempFile : public File
{
public:
    TempFile(std::unique_ptr<ClassTree> class_tree) : class_tree_(std::move(class_tree)) {}
    ~TempFile() override = default;

    std::string GetFileExt() const override { return "cpp"; }
    const ClassTree& GetClassTree() const override { return *class_tree_; }

private:
    std::unique_ptr<ClassTree> class_tree_;
};

}

TEST(plantuml_to_cpp, write_cpp_file)
{
    const std::string output_file_path = "../../../data/test_output.cpp";

    // Create a simple ClassTree with one class, one method, and one variable
    std::unique_ptr<pu2cpp::ClassTree> class_tree = std::make_unique<pu2cpp::ClassTree>();

    pu2cpp::ClassNode class_node("TestClass");
    class_node.AddMethod(pu2cpp::ClassMember("void TestMethod()", {"public"}, {"@brief : This is a test method."}));
    class_node.AddVariable(pu2cpp::ClassMember("int test_variable", {"private"}, {"This is a test variable."}));
    class_tree->AddNode(std::make_unique<pu2cpp::ClassNode>(class_node));

    pu2cpp::ClassNode child_class_node("ChildClass", class_tree->GetNode("TestClass"));
    child_class_node.AddMethod(pu2cpp::ClassMember("void ChildMethod()", {"public"}, {"@brief : This is a child method."}));
    child_class_node.AddMethod(pu2cpp::ClassMember("void StaticChildMethod()", {"public", "static"}, {"@brief : This is a static child method."}));
    child_class_node.AddMethod(pu2cpp::ClassMember("void ProtectedChildMethod()", {"protected"}, {"@brief : This is a protected child method."}));
    child_class_node.AddVariable(pu2cpp::ClassMember("int child_variable", {"private"}, {"This is a child variable."}));
    class_tree->AddNode(std::make_unique<pu2cpp::ClassNode>(child_class_node));

    // Create a TempFile with the ClassTree
    std::unique_ptr<pu2cpp::File> file = std::make_unique<pu2cpp::TempFile>(std::move(class_tree));

    // Create a CppFileWriter and write the file to binary data
    pu2cpp::CppFileWriter writer;
    uint32_t size = 0;
    std::unique_ptr<uint8_t[]> buffer = writer.Write(std::move(file), size);
    ASSERT_NE(buffer, nullptr);

    // Write the binary data to the output file
    ASSERT_TRUE(pu2cpp::WriteBufferToFile(output_file_path, buffer.get(), size));
}