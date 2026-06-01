#include "pch.h"
#include "plantuml-to-cpp/file_pu.h"
#include "plantuml-to-cpp/class_diagram.h"

namespace pu2cpp
{

bool ClassNameParser::Parse(const uint8_t *binary_data, uint32_t size, uint8_t*& iterator, ClassTree& class_tree) const
{
    // Check it is a class declaration
    const char* class_keyword = "class ";
    for (size_t i = 0; i < strlen(class_keyword); ++i)
    {
        if (iterator[i] != class_keyword[i])
            return false; // Not a class declaration
    }

    // Move the iterator past the "class " keyword
    iterator += strlen(class_keyword);

    // Collect the class name until we hit a space
    std::string class_name;
    while (*iterator != ' ')
    {
        class_name += static_cast<char>(*iterator);
        ++iterator;
        if (iterator - binary_data >= size)
            return false; // Reached the end of the data without finding a space
    }

    // Move the iterator to the { character
    while (*iterator != '{')
    {
        ++iterator;
        if (iterator - binary_data >= size)
            return false; // Reached the end of the data without finding '{'
    }

    return true;
}

bool MemberParser::Parse(const uint8_t *binary_data, uint32_t size, uint8_t*& iterator, ClassTree& class_tree) const
{
    return false;
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