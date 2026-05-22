#include <gtest/gtest.h>
#include "plantuml-to-cpp/class_diagram.h"

TEST(plantuml_to_cpp, class_member)
{
    // Sample declaration, modifiers, and comments for testing
    std::vector<std::string> modifiers = {"public"};
    std::vector<std::string> comments = {"@brief : This is a test method", "@param x : An integer parameter"};
    std::string declaration = "void foo(int x)";

    // Create a ClassMember instance with the declaration, modifiers, and comments
    pu2cpp::ClassMember member(declaration, modifiers, comments);

    // Verify that the declaration, modifiers, and comments are correctly stored in the ClassMember instance
    EXPECT_EQ(member.GetDeclaration(), declaration);
    EXPECT_EQ(member.GetModifiers(), modifiers);
    EXPECT_EQ(member.GetComments(), comments);
}

TEST(plantuml_to_cpp, class_node)
{
    // Sample class name and parent class name for testing
    std::string class_name = "TestClass";
    std::string parent_class_name = "BaseClass";

    // Create a parent ClassNode instance
    pu2cpp::ClassNode parent_node(parent_class_name);

    // Create a ClassNode instance with the class name and parent node
    pu2cpp::ClassNode node(class_name, &parent_node);

    // Verify that the class name and parent node are correctly stored in the ClassNode instance
    EXPECT_EQ(node.GetName(), class_name);
    EXPECT_EQ(node.GetParent(), &parent_node);

    // Create a ClassMember instance for a method
    std::vector<std::string> method_modifiers = {"public"};
    std::vector<std::string> method_comments = {"@brief : This is a test method", "@param x : An integer parameter"};
    std::string method_declaration = "void foo(int x)";
    pu2cpp::ClassMember method(method_declaration, method_modifiers, method_comments);

    // Add the method to the class node
    node.AddMethod(std::move(method));

    // Verify that the method was added to the class node
    EXPECT_EQ(node.GetMethods().size(), 1);
    EXPECT_EQ(node.GetMethods()[0].GetDeclaration(), method_declaration);
    EXPECT_EQ(node.GetMethods()[0].GetModifiers(), method_modifiers);
    EXPECT_EQ(node.GetMethods()[0].GetComments(), method_comments);

    // Create a ClassMember instance for a variable
    std::vector<std::string> variable_modifiers = {"private"};
    std::vector<std::string> variable_comments = {"@brief : This is a test variable"};
    std::string variable_declaration = "int bar";
    pu2cpp::ClassMember variable(variable_declaration, variable_modifiers, variable_comments);

    // Add the variable to the class node
    node.AddVariable(std::move(variable));

    // Verify that the variable was added to the class node
    EXPECT_EQ(node.GetVariables().size(), 1);
    EXPECT_EQ(node.GetVariables()[0].GetDeclaration(), variable_declaration);
    EXPECT_EQ(node.GetVariables()[0].GetModifiers(), variable_modifiers);
    EXPECT_EQ(node.GetVariables()[0].GetComments(), variable_comments);
}

TEST(plantuml_to_cpp, class_tree)
{
    // Create a ClassTree instance
    pu2cpp::ClassTree tree;

    const uint32_t NUM_NODES = 2;
    const uint32_t NUM_ROOT_NODES = 1;

    // Create a root ClassNode instance
    std::string root_class_name = "RootClass";
    std::unique_ptr<pu2cpp::ClassNode> root_node = std::make_unique<pu2cpp::ClassNode>(root_class_name);

    // Create a child ClassNode instance with the root node as its parent
    std::string child_class_name = "ChildClass";
    std::unique_ptr<pu2cpp::ClassNode> child_node = std::make_unique<pu2cpp::ClassNode>(child_class_name, root_node.get());

    // Add the root and child nodes to the class tree
    tree.AddNode(std::move(root_node));
    tree.AddNode(std::move(child_node));

    // Verify that the root and child nodes are correctly stored in the class tree
    EXPECT_EQ(tree.GetNode(root_class_name)->GetName(), root_class_name);
    EXPECT_EQ(tree.GetNode(child_class_name)->GetName(), child_class_name);

    // Verify that the root node is correctly identified as a root node in the class tree
    const std::vector<const pu2cpp::ClassNode*>& root_nodes = tree.GetRootNodes();
    EXPECT_EQ(root_nodes.size(), NUM_ROOT_NODES);
    EXPECT_EQ(root_nodes[0]->GetName(), root_class_name);

    // Verify that the amount of nodes in the class tree is correct
    std::vector<std::string_view> node_names = tree.GetNodeNames();
    EXPECT_EQ(node_names.size(), NUM_NODES);
}