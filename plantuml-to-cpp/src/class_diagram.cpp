#include "pch.h"
#include "plantuml-to-cpp/class_diagram.h"

namespace pu2cpp
{

ClassMember::ClassMember(
    std::string declaration, std::vector<std::string> modifiers, std::vector<std::string> comments) :
    declaration_(std::move(declaration)),
    modifiers_(std::move(modifiers)),
    comments_(std::move(comments))
{
}

std::string_view ClassMember::GetDeclaration() const
{
    return declaration_;
}

const std::vector<std::string>& ClassMember::GetModifiers() const
{
    return modifiers_;
}

const std::vector<std::string>& ClassMember::GetComments() const
{
    return comments_;
}

ClassNode::ClassNode(std::string name, const ClassNode *parent) :
    name_(std::move(name)),
    parent_(parent)
{
}

std::string_view ClassNode::GetName() const
{
    return name_;
}

void ClassNode::AddMethod(ClassMember method)
{
    methods_.emplace_back(std::move(method));
}

const std::vector<ClassMember>& ClassNode::GetMethods() const
{
    return methods_;
}

void ClassNode::AddVariable(ClassMember variable)
{
    variables_.emplace_back(std::move(variable));
}

const std::vector<ClassMember>& ClassNode::GetVariables() const
{
    return variables_;
}

const ClassNode* ClassNode::GetParent() const
{
    return parent_;
}

void ClassTree::AddNode(std::unique_ptr<ClassNode> node)
{
    // Get the name of the node to be added
    const std::string_view node_name = node->GetName();

    // Check if the node already exists in the class tree
    if (nodes_.find(node_name.data()) != nodes_.end())
        return; // Node already exists, do not add it again

    // Add the node to the nodes map, keyed by its name
    nodes_[node_name.data()] = std::move(node);
}

ClassNode* ClassTree::GetNode(std::string_view name)
{
    auto it = nodes_.find(name.data());
    if (it != nodes_.end())
        return it->second.get(); // Node found, return pointer to the node
    return nullptr; // Node not found, return nullptr
}

const ClassNode* ClassTree::GetNode(std::string_view name) const
{
    auto it = nodes_.find(name.data());
    if (it != nodes_.end())
        return it->second.get(); // Node found, return pointer to the node
    return nullptr; // Node not found, return nullptr
}

std::vector<std::string_view> ClassTree::GetNodeNames() const
{
    // Collect the names of all class nodes in the class tree into a vector
    std::vector<std::string_view> node_names;
    for (const auto& pair : nodes_)
        node_names.push_back(pair.first);

    // Return the vector of class node names
    return node_names;
}

std::vector<const ClassNode*> ClassTree::GetRootNodes() const
{
    std::vector<const ClassNode*> root_nodes;
    for (const auto& pair : nodes_)
    {
        const ClassNode* node = pair.second.get();

        // If the node does not have a parent, it is a root node, so add it to the root nodes vector
        if (node->GetParent() == nullptr)
            root_nodes.push_back(node);
    }

    return root_nodes;
}

} // namespace pu2cpp