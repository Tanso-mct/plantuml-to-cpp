#pragma once

#include "plantuml-to-cpp/file.h"
#include "plantuml-to-cpp/class_diagram.h"

namespace pu2cpp
{

class PuFile 
    : public File
{
public:
    /**
     * @brief : Constructor for PuFile with a class tree
     * @param class_tree : The class tree representing the structure of the PlantUML file
     */
    PuFile(ClassTree class_tree);

    ~PuFile() override = default;

    /**
     * @brief : Get the file extension for PuFile
     * @return : The file extension "pu"
     */
    std::string GetFileExt() const override;

    /**
     * @brief : Get the class tree representing the structure of the PlantUML file
     * @return : The class tree
     */
    const ClassTree& GetClassTree() const;

private:
    // The class tree representing the structure of the PlantUML file
    ClassTree class_tree_;
};

class PuFileReader :
    public FileReader
{
public:
    PuFileReader() = default;
    ~PuFileReader() override = default;

    /**
     * @brief : Read binary data and create a PuFile object
     * @param binary_data : The binary data to read
     * @param size : The size of the binary data
     * @return : A unique pointer to the created PuFile object
     */
    std::unique_ptr<File> Read(std::unique_ptr<uint8_t[]> binary_data, uint32_t size) const override;
};

} // namespace pu2cpp