#include "plantuml-to-cpp/plantuml-to-cpp.h"

#include <iostream>

namespace pu2cpp
{

// Error codes
constexpr const int ERROR_INVALID_ARGUMENTS = 1;
constexpr const int ERROR_FILE_READ = 2;
constexpr const int ERROR_FILE_CONVERT = 3;
constexpr const int ERROR_FILE_WRITE = 4;

/**
 * @brief Checks if the command-line arguments are valid
 * @param argc : The number of command-line arguments
 * @param argv : The array of command-line arguments
 * @return bool : true if the arguments are valid, false otherwise
 */
bool CheckArguments(int argc, char* argv[])
{
	if (argc != 3)
	{
		std::cerr << "You must provide exactly 2 arguments: <input_file> <output_file>" << std::endl;
		std::cerr << "Usage: plantuml-to-cpp.exe input_file_path output_file_path" << std::endl;
		return false;
	}

	return true;
};

// Indices for command-line arguments
constexpr const uint32_t ARGUMENT_INPUT_FILE_PATH_INDEX = 1;
constexpr const uint32_t ARGUMENT_OUTPUT_FILE_PATH_INDEX = 2;

/**
 * @brief Collects the input and output file paths from the command-line arguments
 * @param argv : The array of command-line arguments
 * @param input_file_path : Output parameter that will hold the input file path
 * @param output_file_path : Output parameter that will hold the output file path
 */
void CollectFilePaths(char* argv[], std::string& input_file_path, std::string& output_file_path)
{
	input_file_path = argv[ARGUMENT_INPUT_FILE_PATH_INDEX];
	output_file_path = argv[ARGUMENT_OUTPUT_FILE_PATH_INDEX];
}

} // namespace pu2cpp

int main(int argc, char* argv[])
{
	// Check command-line arguments
	if (!pu2cpp::CheckArguments(argc, argv))
		return pu2cpp::ERROR_INVALID_ARGUMENTS;

	// Collect input and output file paths from command-line arguments
	std::string input_file_path, output_file_path;
	pu2cpp::CollectFilePaths(argv, input_file_path, output_file_path);

	// Create a converter with a PuFileReader and CppFileWriter
	std::unique_ptr<pu2cpp::Converter> converter = nullptr;
	{
		// Create a file reader
		std::unique_ptr<pu2cpp::FileReader> reader = std::make_unique<pu2cpp::PuFileReader>();

		// Create a file writer
		std::unique_ptr<pu2cpp::FileWriter> writer = std::make_unique<pu2cpp::CppFileWriter>();

		// Create a converter with the reader and writer
		converter = std::make_unique<pu2cpp::Converter>(std::move(reader), std::move(writer));
	}

	// Read the input file into a buffer
	fpos_t input_file_size = 0;
	std::unique_ptr<uint8_t[]> input_file_buffer = pu2cpp::ReadFileToBuffer(input_file_path, input_file_size);
	if (!input_file_buffer)
	{
		std::cerr << "Failed to read input file: " << input_file_path << std::endl;
		return pu2cpp::ERROR_FILE_READ;
	}

	// Convert the input file buffer to an output file buffer
	uint32_t output_file_size = 0;
	std::unique_ptr<uint8_t[]> output_file_buffer 
		= converter->Convert(std::move(input_file_buffer), input_file_size, output_file_size);
	if (!output_file_buffer)
	{
		std::cerr << "Failed to convert input file buffer" << std::endl;
		return pu2cpp::ERROR_FILE_CONVERT;
	}

	// Write the output file buffer to the output file
	if (!pu2cpp::WriteBufferToFile(output_file_path, output_file_buffer.get(), output_file_size))
	{
		std::cerr << "Failed to write output file: " << output_file_path << std::endl;
		return pu2cpp::ERROR_FILE_WRITE;
	}

	return 0;
}

