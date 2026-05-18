#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

namespace pu2cpp
{

class ClassMember
{
public:
    /**
     * @brief : Constructor for ClassMember with a declaration and modifiers
     * @param declaration : The declaration (e.g., "void foo(int x), "int bar")
     * @param modifiers : A vector of modifiers [e.g., "public", "static"]
     * @param comments : A vector of comments
     */
    ClassMember(std::string declaration, std::vector<std::string> modifiers, std::vector<std::string> comments);

    ~ClassMember() = default;

    /**
     * @brief : Get the declaration
     * @return : The declaration
     */
    std::string_view GetDeclaration() const;

    /**
     * @brief : Get the vector of modifiers
     * @return : The vector of modifiers
     */
    const std::vector<std::string>& GetModifiers() const;

    /**
     * @brief : Get the vector of comments
     * @return : The vector of comments
     */
    const std::vector<std::string>& GetComments() const;

private:
    // The declaration (e.g., "void foo(int x), "int bar")
    std::string declaration_;

    // A vector of modifiers (e.g., ["public", "static"])
    std::vector<std::string> modifiers_;

    // A vector of comments
    std::vector<std::string> comments_;
};

class ClassNode
{
public:
    /**
     * @brief : Constructor for ClassNode with a name and an optional parent
     * @param name : The name of the class
     * @param parent : Pointer to the parent class node (nullptr if it's a root node)
     */
    ClassNode(std::string name, const ClassNode* parent = nullptr);

    ~ClassNode() = default;

    /**
     * @brief : Get the name of the class
     * @return : The name of the class
     */
    std::string_view GetName() const;

    /**
     * @brief : Add a method to the class node
     * @param method : The method to add
     */
    void AddMethod(ClassMember method);

    /**
     * @brief : Add a variable to the class node
     * @param variable : The variable to add
     */
    void AddVariable(ClassMember variable);

    /**
     * @brief : Get a pointer to the parent class node
     * @return : Pointer to the parent class node (nullptr if it's a root node)
     */
    const ClassNode* GetParent() const;

private:
    // The name of the class
    std::string name_;

    // Pointer to the parent class node (nullptr if it's a root node)
    // Parent is the base class, and Child is the derived class
    const ClassNode* parent_;

    // A vector of methods belonging to the class
    std::vector<ClassMember> methods_;

    // A vector of variables belonging to the class
    std::vector<ClassMember> variables_;
};

class ClassTree
{
public:
    ClassTree() = default;
    ~ClassTree() = default;

    /**
     * @brief : Add a class node to the class tree
     * @param node : The class node to add
     */
    void AddNode(std::unique_ptr<ClassNode> node);

    /**
     * @brief : Get a pointer to a class node by name
     * @param name : The name of the class node to retrieve
     * @return : Pointer to the class node with the specified name (nullptr if not found)
     */
    const ClassNode* GetNode(std::string_view name) const;

    /**
     * @brief : Get a vector of class node names in the class tree
     * @return : A vector of class node names in the class tree
     */
    std::vector<std::string_view> GetNodeNames() const;

    /**
     * @brief : Get a vector of pointers to the root class nodes in the class tree
     * @return : A vector of pointers to the root class nodes in the class tree
     */
    const std::vector<const ClassNode*>& GetRootNodes() const;

private:
    // A map of class nodes in the class tree, keyed by class name
    std::unordered_map<std::string, std::unique_ptr<ClassNode>> nodes;

    // A vector of pointers to the root class nodes in the class tree
    // Root nodes are those that do not have a parent
    std::vector<const ClassNode*> root_nodes;
};

} // namespace pu2cpp