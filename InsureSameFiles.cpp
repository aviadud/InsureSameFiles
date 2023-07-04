// InsureSameFiles.cpp : This small program takes 2 directories and print to output file the files paths that don't appear in the other directory.
//

#include <iostream>
#include <string>
#include <filesystem>
#include <unordered_map>
#include <algorithm>
#include <fstream>
#include <queue>


namespace fs = std::filesystem;

static const std::string OUTPUT_FILE_NAME = "output.txt";

std::unordered_map<fs::path, fs::path> parseDirectory(const fs::path& directoryPath) {
	std::unordered_map<fs::path, fs::path> filesMap;
	if (fs::exists(directoryPath) && fs::is_directory(directoryPath)) {
		for (const auto& dirEntry : fs::recursive_directory_iterator(directoryPath))
			{
				filesMap[fs::relative(dirEntry.path(), directoryPath)] = dirEntry.path();
			}
	}
	else {
		std::cerr << "Invalid directory path: " << directoryPath << std::endl;
	}
	return filesMap;
}

/**
* 1's argument: Source directory path.
* 2's argument: Target directory path.
* 3's argument: The directory path to save output file at.
*/
int main(int argc, char* argv[])
{
	if (argc == 4) {
		const fs::path sourceDirectoryPath = argv[1], targetDirectoryPath = argv[2];
		const fs::path outputFilePath = fs::path(argv[3]) / OUTPUT_FILE_NAME;
		std::cout << "Hello World!\n" << "I hate debugging" << std::endl;
		std::unordered_map sourceFilesMap = parseDirectory(sourceDirectoryPath);
		std::unordered_map targetFilesMap = parseDirectory(targetDirectoryPath);
		std::priority_queue<fs::path> sourceListOutput, targetListOutput;
		std::ofstream outputFile(outputFilePath);
		int counter = 0;
		outputFile << "Files in " << targetDirectoryPath << " and not in " << sourceDirectoryPath << ":\n";
		for (const auto& it : targetFilesMap) {
			if (!sourceFilesMap.contains(it.first)) {
				counter++;
				targetListOutput.push(it.second);
			}
		}
		while (!targetListOutput.empty())
		{
			outputFile << targetListOutput.top() << std::endl;
			targetListOutput.pop();
		}
		outputFile << "Number of files found in target and not in source is: " << counter << std::endl;
		counter = 0;
		outputFile << "Files in " << sourceDirectoryPath << " and not in " << targetDirectoryPath << ":\n";
		for (const auto& it : sourceFilesMap) {
			if (!targetFilesMap.contains(it.first)) {
				counter++;
				sourceListOutput.push(it.second);
			}
		}
		while (!sourceListOutput.empty())
		{
			outputFile << sourceListOutput.top() << std::endl;
			sourceListOutput.pop();
		}
		outputFile << "Number of files found in source and not in target is: " << counter << std::endl;
		
	}
	else
	{
		std::cerr << "Error - wrong usage of script" << std::endl;
	}

}
