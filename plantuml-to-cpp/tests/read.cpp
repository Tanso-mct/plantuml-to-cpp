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

    // Output the class information for verification
    for (const std::string_view class_name : class_tree->GetNodeNames())
    {
        const pu2cpp::ClassNode* class_node = class_tree->GetNode(class_name);
        if (class_node)
        {
            std::cout << "Class: " << class_name << std::endl;

            for (const pu2cpp::ClassMember& method : class_node->GetMethods())
            {
                // Modifiers are printed before the declaration for better readability
                for (const std::string& modifier : method.GetModifiers())
                    std::cout << modifier << " ";
                
                // Print the method declaration
                std::cout << method.GetDeclaration() << std::endl;

                // Print the comments for the method, if any
                for (const std::string& comment : method.GetComments())
                    std::cout << comment << std::endl;

                std::cout << std::endl;
            }

            for (const pu2cpp::ClassMember& variable : class_node->GetVariables())
            {
                // Modifiers are printed before the declaration for better readability
                for (const std::string& modifier : variable.GetModifiers())
                    std::cout << modifier << " ";
                
                // Print the variable declaration
                std::cout << variable.GetDeclaration() << std::endl;

                // Print the comments for the variable, if any
                for (const std::string& comment : variable.GetComments())
                    std::cout << comment << std::endl;
            }

            std::cout << "-----------------------------" << std::endl;
        }
    }
}