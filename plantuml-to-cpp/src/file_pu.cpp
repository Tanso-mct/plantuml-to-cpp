#include "pch.h"
#include "plantuml-to-cpp/file_pu.h"
#include "plantuml-to-cpp/class_diagram.h"

namespace pu2cpp
{

bool ClassNameParser::Parse(const uint8_t *binary_data, uint32_t size, uint8_t*& iterator, ClassTree& class_tree) const
{
    // Find the next occurrence of class keyword in the binary data
    bool found_class_keyword = false;
    while (iterator - binary_data < size)
    {
        bool keyword_found = false;
        for (const std::string& keyword : CLASS_KEYWORDS)
        {
            if (std::equal(keyword.begin(), keyword.end(), iterator))
            {
                found_class_keyword = true;
                keyword_found = true;
                iterator += keyword.size(); // Move the iterator past the keyword
                break;
            }
        }

        if (keyword_found)
            break; // Found a class keyword, exit the loop

        ++iterator; // Move to the next byte
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

    // Store the last parsed class name in the parser
    last_parsed_class_name_ = class_name;

    return true;
}

std::string_view ClassNameParser::GetLastParsedClassName() const
{
    return last_parsed_class_name_;
}

MembersParser::MembersParser(std::string_view class_name)
    : class_name_(std::move(class_name))
{
}

bool MembersParser::Parse(const uint8_t *binary_data, uint32_t size, uint8_t*& iterator, ClassTree& class_tree) const
{
    uint8_t* current = iterator;

    // Find the next occurrence of member declaration start in the binary data
    bool found_members_declaration_start = false;
    uint8_t* members_declaration_start = nullptr;
    while (current - binary_data < size)
    {
        if (std::equal(MEMBER_DECLARATION_START.begin(), MEMBER_DECLARATION_START.end(), current))
        {
            found_members_declaration_start = true;
            current += MEMBER_DECLARATION_START.size(); // Move the current past the member declaration start
            members_declaration_start = current; // Mark the start of the member declaration
            break;
        }

        ++current; // Move to the next byte
    }

    if (!found_members_declaration_start)
        return false; // No more member declarations found, parsing is complete

    // Find the next occurrence of member declaration end in the binary data
    bool found_members_declaration_end = false;
    uint8_t* members_declaration_end = nullptr;
    while (current - binary_data < size)
    {
        if (std::equal(MEMBER_DECLARATION_END.begin(), MEMBER_DECLARATION_END.end(), current))
        {
            found_members_declaration_end = true;
            current += MEMBER_DECLARATION_END.size(); // Move the current past the member declaration end
            members_declaration_end = current; // Mark the end of the member declaration
            break;
        }

        ++current; // Move to the next byte
    }

    if (!found_members_declaration_end)
        return false; // No member declaration end found, parsing is incomplete

    // Find the next occurrence of member divide line in the binary data
    bool found_member_divide_line = false;
    uint8_t* member_divide_line = members_declaration_start;
    while (member_divide_line < members_declaration_end)
    {
        if (std::equal(MEMBER_DIVIDE_LINE.begin(), MEMBER_DIVIDE_LINE.end(), member_divide_line))
        {
            found_member_divide_line = true;
            break;
        }

        ++member_divide_line; // Move to the next byte
    }

    if (!found_member_divide_line)
        return false; // No member divide line found, parsing is incomplete

    // Move the current pointer back to the start of the member declaration to begin parsing member declarations
    current = members_declaration_start;

    while (current < members_declaration_end)
    {
        // Find the next occurrence of a member modifier in the binary data
        bool found_member_modifier = false;
        std::vector<std::string> member_modifiers; // To store all found member modifiers
        while (current < members_declaration_end)
        {
            for (int i = 0; i < sizeof(MEMBER_MODIFIERS) / sizeof(MEMBER_MODIFIERS[0]); ++i)
            {
                if (std::equal(MEMBER_MODIFIERS[i].begin(), MEMBER_MODIFIERS[i].end(), current))
                {
                    found_member_modifier = true;

                    // Store the stripped member modifier
                    member_modifiers.push_back(MEMBER_MODIFIERS_STRIPPED[i]);

                    // If the member modifier is static, also add the static modifier to the list of modifiers
                    if (IS_STATIC_MODIFIER[i])
                        member_modifiers.push_back(STATIC_MODIFIER_STRIPPED);

                    current += MEMBER_MODIFIERS[i].size(); // Move the current pointer past the member modifier
                    break;
                }
            }

            if (found_member_modifier)
                break; // Found a member modifier, exit the loop

            ++current; // Move to the next byte
        }

        if (current >= members_declaration_end)
            break; // Reached the end of member declarations, exit the loop

        if (!found_member_modifier)
            return false; // No member modifier found, parsing is incomplete

        bool is_method = false;
        if (current < member_divide_line)
            is_method = true; // If the current pointer is still before the member divide line, it is a method declaration

        // Extract the member declaration between the member modifier and end of line
        std::string member_declaration;
        while (*current != '\r' && *current != '\n')
        {
            member_declaration += static_cast<char>(*current);
            ++current;
        }

        // Move the current pointer past the end of line characters
        while (*current == '\r' || *current == '\n')
            ++current;

        // Check it has comments
        uint8_t* temp = current;
        std::vector<std::string> comments;
        while (temp < members_declaration_end)
        {
            if (std::equal(COMMENT_PREFIX.begin(), COMMENT_PREFIX.end(), temp))
            {
                // Move the temp pointer past the comment prefix
                temp += COMMENT_PREFIX.size();

                // Extract the comment text until the end of the line
                std::string comment;
                while (*temp != '\r' && *temp != '\n')
                {
                    comment += static_cast<char>(*temp);
                    ++temp;
                }

                // Add the extracted comment to the list of comments for the member declaration
                comments.push_back(std::move(comment));

                // Move to the next line after the comment
                while (*temp == '\r' || *temp == '\n')
                    ++temp;

                // Move the current pointer to the next line after the comment for the next iteration
                current = temp;

                continue;
            }

            if (*temp == '\r' || *temp == '\n')
            {
                // If we encounter a line that does not start with the comment prefix, 
                // it means we have reached the end of the comments for the member declaration
                break;
            }

            ++temp; // Move to the next byte
        }

        // Create the ClassMember object with the extracted member declaration, modifiers, and comments
        ClassMember member(std::move(member_declaration), std::move(member_modifiers), std::move(comments));
        
        // Get the class node from the class tree using the class name
        ClassNode* class_node = class_tree.GetNode(class_name_);
        if (!class_node)
            return false; // Class node not found, parsing is incomplete

        // Add the member to the class node
        if (is_method)
            class_node->AddMethod(std::move(member));
        else
            class_node->AddVariable(std::move(member));
    }

    // Move the iterator to the end of the member declarations for the current class
    iterator = members_declaration_end;

    return true;
}

bool ClassRelationshipParser::Parse(const uint8_t *binary_data, uint32_t size, uint8_t*& iterator, ClassTree& class_tree) const
{
    // Find the next occurrence of class keyword in the binary data
    bool found_class_keyword = false;
    uint8_t* next_class_start_pos = iterator;
    while (next_class_start_pos - binary_data < size)
    {
        for (const std::string& keyword : CLASS_KEYWORDS)
        {
            if (std::equal(keyword.begin(), keyword.end(), next_class_start_pos))
            {
                found_class_keyword = true;
                break;
            }
        }

        if (found_class_keyword)
            break; // Found the start of the next class, exit the loop

        ++next_class_start_pos; // Move to the next byte
    }

    // Find the next occurrence of inheritance keyword in the binary data
    uint8_t* inheritance_keyword_pos = iterator;
    while (inheritance_keyword_pos < next_class_start_pos)
    {
        if (std::equal(INHERITANCE_KEYWORD.begin(), INHERITANCE_KEYWORD.end(), inheritance_keyword_pos))
        {
            // Move backwards from the inheritance keyword to find the derived class name
            uint8_t* temp = inheritance_keyword_pos;
            while (temp > binary_data && *temp != '\r' && *temp != '\n')
                --temp;

            // Extract the derived class name
            std::string derived_class_name = std::string(temp + 1, inheritance_keyword_pos);

            // Move forwards from the inheritance keyword to find the base class name
            temp = inheritance_keyword_pos + INHERITANCE_KEYWORD.size();
            while (temp < next_class_start_pos && *temp != '\r' && *temp != '\n')
                ++temp;

            // Extract the base class name
            std::string base_class_name = std::string(inheritance_keyword_pos + INHERITANCE_KEYWORD.size(), temp);

            // Get the class nodes for the derived and base classes from the class tree
            ClassNode* derived_class_node = class_tree.GetNode(derived_class_name);
            if (!derived_class_node)
            {
                // Add the derived class node to the class tree if it does not exist
                class_tree.AddNode(std::make_unique<ClassNode>(derived_class_name));
                derived_class_node = class_tree.GetNode(derived_class_name);
            }

            ClassNode* base_class_node = class_tree.GetNode(base_class_name);
            if (!base_class_node)
            {
                // Add the base class node to the class tree if it does not exist
                class_tree.AddNode(std::make_unique<ClassNode>(base_class_name));
                base_class_node = class_tree.GetNode(base_class_name);
            }

            // Set the parent of the derived class node to be the base class node
            derived_class_node->SetParent(base_class_node);
            
            // Move the inheritance keyword position past the current inheritance relationship to find the next one
            inheritance_keyword_pos = temp;
        }

        ++inheritance_keyword_pos; // Move to the next byte
    }

    // Move the iterator to the start of the next class for the next iteration
    iterator = next_class_start_pos;

    return true;
}

std::unique_ptr<ClassTree> ParsePuFile(const uint8_t* binary_data, uint32_t size)
{
    // Create parsers for class names and members
    std::unique_ptr<pu2cpp::ClassNameParser> class_name_parser = std::make_unique<pu2cpp::ClassNameParser>();

    // Create a ClassTree to hold the parsed class diagram
    std::unique_ptr<ClassTree> class_tree = std::make_unique<ClassTree>();

    uint8_t* iterator = const_cast<uint8_t*>(binary_data);
    while (class_name_parser->Parse(binary_data, size, iterator, *class_tree))
    {
        // Successfully parsed a class name, create a members parser for the parsed class name
        std::unique_ptr<pu2cpp::MembersParser> member_parser 
            = std::make_unique<pu2cpp::MembersParser>(class_name_parser->GetLastParsedClassName());
        
        if (!member_parser->Parse(binary_data, size, iterator, *class_tree))
        {
            // Failed to parse members for the class, return nullptr to indicate parsing failure
            return nullptr;
        }

        // After parsing members, should parse relationships
        std::unique_ptr<pu2cpp::ClassRelationshipParser> relationship_parser = std::make_unique<pu2cpp::ClassRelationshipParser>();
        relationship_parser->Parse(binary_data, size, iterator, *class_tree);
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