#include "pch.h"
#include "plantuml-to-cpp/file_pu.h"
#include "plantuml-to-cpp/class_diagram.h"

namespace pu2cpp
{

bool ClassNameParser::Parse(const uint8_t *binary_data, uint32_t size, uint8_t*& iterator, ClassTree& class_tree) const
{
    // Find the next occurrence of "class " in the binary data
    const std::string class_keyword = "class ";
    bool found_class_keyword = false;
    while (iterator - binary_data < size)
    {
        if (std::equal(class_keyword.begin(), class_keyword.end(), iterator))
        {
            // Found the "class " keyword, move the iterator past it
            iterator += class_keyword.size();
            found_class_keyword = true;
            break;
        }
        ++iterator;
    }

    if (!found_class_keyword)
        return false; // No more class keywords found, parsing is complete

    // Collect the class name until we hit a space or the end of the binary data
    std::string class_name;
    while (iterator - binary_data < size && *iterator != ' ' && *iterator != '\r' && *iterator != '\n')
    {
        class_name += static_cast<char>(*iterator);
        ++iterator;
    }

    // Add the class name to the class tree
    class_tree.AddNode(std::make_unique<ClassNode>(class_name));

    return true;
}

bool MemberParser::Parse(const uint8_t *binary_data, uint32_t size, uint8_t*& iterator, ClassTree& class_tree) const
{
    return false;
}

bool ClassRelationshipParser::Parse(const uint8_t *binary_data, uint32_t size, uint8_t*& iterator, ClassTree& class_tree) const
{
    return false;
}

std::unique_ptr<ClassTree> ParsePuFile(const uint8_t* binary_data, uint32_t size)
{
    // Create parsers for class names and members
    std::unique_ptr<pu2cpp::PuFileParser> class_name_parser = std::make_unique<pu2cpp::ClassNameParser>();
    std::unique_ptr<pu2cpp::PuFileParser> member_parser = std::make_unique<pu2cpp::MemberParser>();
    std::unique_ptr<pu2cpp::PuFileParser> relationship_parser = std::make_unique<pu2cpp::ClassRelationshipParser>();

    // Create a ClassTree to hold the parsed class diagram
    std::unique_ptr<ClassTree> class_tree = std::make_unique<ClassTree>();

    uint8_t* iterator = const_cast<uint8_t*>(binary_data);
    while (class_name_parser->Parse(binary_data, size, iterator, *class_tree))
    {
        // Successfully parsed a class name, now try to parse members
        
        while (member_parser->Parse(binary_data, size, iterator, *class_tree))
        {
            // Successfully parsed a member, continue parsing members until we can't parse anymore
        }

        // After parsing members, should parse relationships

        while (relationship_parser->Parse(binary_data, size, iterator, *class_tree))
        {
            // Successfully parsed a relationship, continue parsing relationships until we can't parse anymore
        }
    }

    return class_tree;
}

PuFile::PuFile(std::unique_ptr<ClassTree> class_tree)
    : class_tree_(std::move(class_tree))
{
}

std::string PuFile::GetFileExt() const
{
    return "pu";
}

const ClassTree& PuFile::GetClassTree() const
{
    assert(class_tree_ != nullptr && "Class tree should not be null");
    return *class_tree_;
}



std::unique_ptr<File> PuFileReader::Read(std::unique_ptr<uint8_t[]> binary_data, uint32_t size) const
{
    // Create a ClassTree object
    ClassTree class_tree;

    // Parse the binary data
    for (uint32_t i = 0; i < size; ++i)
    {
        
    }

    return nullptr;
}

} // namespace pu2cpp