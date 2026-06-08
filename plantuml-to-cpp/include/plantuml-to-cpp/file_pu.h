#pragma once

#include <memory>

#include "plantuml-to-cpp/file.h"
#include "plantuml-to-cpp/class_diagram.h"

namespace pu2cpp
{

class PuFileParser
{
public:
    virtual ~PuFileParser() = default;

    /**
     * @brief : Parse binary data and create a ClassTree object
     * @param binary_data : The binary data to parse
     * @param size : The size of the binary data
     * @param iterator : A pointer to the current position in the binary data (used for parsing)
     * @param class_tree : Output parameter that will hold the created ClassTree object
     * @return bool : true if parsing was successful, false otherwise
     */
    virtual bool Parse(const uint8_t* binary_data, uint32_t size, uint8_t*& iterator, ClassTree& class_tree) const = 0;
};

// Define specific parsers for class names, members, and relationships
const std::string CLASS_KEYWORDS[] = {"class ", "interface ", "abstract class "};
const std::string MEMBER_DECLARATION_START = "{";
const std::string MEMBER_DECLARATION_END = "}\r\n";
const std::string MEMBER_MODIFIERS[] = {"+ ", "- ", "# ", "{static} + ", "{static} - ", "{static} # "};
const std::string MEMBER_MODIFIERS_STRIPPED[] = {"public ", "private ", "protected ", "public", "private ", "protected "};
const std::string STATIC_MODIFIER_STRIPPED = "static";
const bool IS_STATIC_MODIFIER[] = {false, false, false, true, true, true};
const std::string COMMENT_PREFIX = " * ";
const std::string MEMBER_DIVIDE_LINE = "---";

class ClassNameParser : public PuFileParser
{
public:
    ClassNameParser() = default;
    ~ClassNameParser() override = default;
    bool Parse(const uint8_t* binary_data, uint32_t size, uint8_t*& iterator, ClassTree& class_tree) const override;

    // Get the name of the last parsed class
    std::string_view GetLastParsedClassName() const;

private:
    // The name of the last parsed class
    mutable std::string last_parsed_class_name_;
};

class MembersParser : public PuFileParser
{
public:
    MembersParser(std::string_view class_name);
    ~MembersParser() override = default;
    bool Parse(const uint8_t* binary_data, uint32_t size, uint8_t*& iterator, ClassTree& class_tree) const override;

private:
    // The name of the class whose members we are parsing
    std::string_view class_name_;
};

class ClassRelationshipParser : public PuFileParser
{
public:
    ClassRelationshipParser() = default;
    ~ClassRelationshipParser() override = default;
    bool Parse(const uint8_t* binary_data, uint32_t size, uint8_t*& iterator, ClassTree& class_tree) const override;
};

/**
 * @brief : Parse binary data from a PlantUML file and create a ClassTree object representing the structure of the file
 * @param binary_data : The binary data to parse
 * @param size : The size of the binary data
 * @return std::unique_ptr<ClassTree> : A unique pointer to the created ClassTree object (nullptr if parsing failed)
 */
std::unique_ptr<ClassTree> ParsePuFile(const uint8_t* binary_data, uint32_t size);

class PuFile 
    : public File
{
public:
    /**
     * @brief : Constructor for PuFile with a class tree
     * @param class_tree : The class tree representing the structure of the PlantUML file
     */
    PuFile(std::unique_ptr<ClassTree> class_tree);

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
    std::unique_ptr<ClassTree> class_tree_;
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