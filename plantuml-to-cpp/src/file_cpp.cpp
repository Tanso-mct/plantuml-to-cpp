#include "pch.h"
#include "plantuml-to-cpp/file_cpp.h"
#include "plantuml-to-cpp/class_diagram.h"
#include "plantuml-to-cpp/file_pu.h"

namespace pu2cpp
{

void WriteMethod(std::string& output_data, const ClassMember* member)
{
    if (member->GetComments().size() > 0) // If the member has comments
    {
        // Write the comments for the member using Doxygen comment style
        output_data += CPP_FILE_INDENT + CPP_FILE_DOXYGEN_START_SYMBOL;

        for (const std::string& comment : member->GetComments())
            output_data += CPP_FILE_INDENT + CPP_FILE_DOXYGEN_COMMENT_SYMBOL + comment + "\n";
        
        output_data += CPP_FILE_INDENT + CPP_FILE_DOXYGEN_END_SYMBOL;
    }

    // Check if the member is static by looking for the "static" modifier
    bool is_static = false;
    for (const std::string& modifier : member->GetModifiers())
    {
        if (modifier == "static")
        {
            is_static = true;
            break;
        }
    }

    // Write the member declaration
    if (is_static) output_data += CPP_FILE_INDENT + "static " + member->GetDeclaration().data() + ";\n";
    else output_data += CPP_FILE_INDENT + member->GetDeclaration().data() + ";\n";

    output_data += "\n"; // Add an extra newline after the method declaration for better readability
}

void WriteVariable(std::string& output_data, const ClassMember* member)
{
    if (member->GetComments().size() > 0) // If the member has comments
    {
        // Write the comments for the member
        for (const std::string& comment : member->GetComments())
            output_data += CPP_FILE_INDENT + CPP_FILE_COMMENT_SYMBOL + comment + "\n";
    }

    // Check if the member is static by looking for the "static" modifier
    bool is_static = false;
    for (const std::string& modifier : member->GetModifiers())
    {
        if (modifier == "static")
        {
            is_static = true;
            break;
        }
    }

    // Write the member declaration
    if (is_static) output_data += CPP_FILE_INDENT + "static " + member->GetDeclaration().data() + ";\n";
    else output_data += CPP_FILE_INDENT + member->GetDeclaration().data() + ";\n";

    output_data += "\n"; // Add an extra newline after the method declaration for better readability
}

std::unique_ptr<uint8_t[]> CppFileWriter::Write(std::unique_ptr<File> file, uint32_t& size) const
{
    // Get the class tree from the File object
    const ClassTree& class_tree = file->GetClassTree();

    std::string output_data;

    // Write the class declarations for each class in the class tree
    for (const std::string_view& class_name : class_tree.GetNodeNames())
    {
        // Get the class node for the class name
        const ClassNode* class_node = class_tree.GetNode(class_name);

        // Write the class declaration
        if (class_node->GetParent()) // If the class has a parent, write the inheritance relationship
        {
            output_data += "class " + std::string(class_node->GetName()) + "\n";
            output_data += CPP_FILE_INDENT + ": public " + std::string(class_node->GetParent()->GetName()) + "\n";
        }
        else // If the class does not have a parent, write the class declaration without inheritance
        {
            output_data += "class " + std::string(class_node->GetName()) + "\n";
        }

        // Write the opening brace for the class declaration
        output_data += "{\n";

        // Get the methods
        const std::vector<ClassMember>& methods = class_node->GetMethods();

        // Separate the methods into different categories based on their modifiers
        std::vector<const ClassMember*> public_methods;
        std::vector<const ClassMember*> protected_methods;
        std::vector<const ClassMember*> private_methods;
        for (int i = 0; i < methods.size(); ++i)
        {
            const ClassMember* method = &methods[i];
            for (const std::string& modifier : method->GetModifiers())
            {
                if (modifier == "public")
                    public_methods.push_back(method);
                else if (modifier == "protected")
                    protected_methods.push_back(method);
                else if (modifier == "private")
                    private_methods.push_back(method);
            }
        }

        // Write the public methods
        if (!public_methods.empty())
            output_data += "public:\n";
        for (const ClassMember* method : public_methods)
            WriteMethod(output_data, method);

        // Write the protected methods
        if (!protected_methods.empty())
            output_data += "protected:\n";
        for (const ClassMember* method : protected_methods)
            WriteMethod(output_data, method);

        // Write the private methods
        if (!private_methods.empty())
            output_data += "private:\n";
        for (const ClassMember* method : private_methods)
            WriteMethod(output_data, method);
            
        // Get the variables
        const std::vector<ClassMember>& variables = class_node->GetVariables();
        
        // Separate the variables into different categories based on their modifiers
        std::vector<const ClassMember*> public_variables;
        std::vector<const ClassMember*> protected_variables;
        std::vector<const ClassMember*> private_variables;
        for (int i = 0; i < variables.size(); ++i)
        {
            const ClassMember* variable = &variables[i];
            for (const std::string& modifier : variable->GetModifiers())
            {
                if (modifier == "public")
                    public_variables.push_back(variable);
                else if (modifier == "protected")
                    protected_variables.push_back(variable);
                else if (modifier == "private")
                    private_variables.push_back(variable);
            }
        }

        // Write the public variables
        if (!public_variables.empty())
            output_data += "public:\n";
        for (const ClassMember* variable : public_variables)
            WriteVariable(output_data, variable);

        // Write the protected variables
        if (!protected_variables.empty())
            output_data += "protected:\n";
        for (const ClassMember* variable : protected_variables)
            WriteVariable(output_data, variable);

        // Write the private variables
        if (!private_variables.empty())
            output_data += "private:\n";
        for (const ClassMember* variable : private_variables)
            WriteVariable(output_data, variable);

        // Write the closing brace for the class declaration
        output_data += "};\n\n";
    }

    // Convert the output data string to a vector of bytes
    size = static_cast<uint32_t>(output_data.size());
    std::unique_ptr<uint8_t[]> output_buffer = std::make_unique<uint8_t[]>(size);
    std::copy(output_data.begin(), output_data.end(), output_buffer.get());

    return output_buffer;
}

} // namespace pu2cpp