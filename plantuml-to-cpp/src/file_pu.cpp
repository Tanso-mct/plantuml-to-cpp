#include "pch.h"
#include "plantuml-to-cpp/file_pu.h"
#include "plantuml-to-cpp/class_diagram.h"

namespace pu2cpp
{

PuFile::PuFile(ClassTree class_tree)
    : class_tree_(std::move(class_tree))
{
}

std::string PuFile::GetFileExt() const
{
    return "pu";
}

const ClassTree& PuFile::GetClassTree() const
{
    return class_tree_;
}

std::unique_ptr<File> PuFileReader::Read(std::unique_ptr<uint8_t[]> binary_data, uint32_t size) const
{
    // Create a ClassTree object
    ClassTree class_tree;
}

} // namespace pu2cpp