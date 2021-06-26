#pragma once

#include <fstream>
#include <string>
#include <vector>

#include "../Input Processing/Tokenizer.h"


namespace Data
{
	namespace File
	{

		//takes two files as input, matches elements in both separated by spaces, and outputs them as pairs
		//ex.
		//file1: "Hello World"
		//file2: "greeting planet"
		//output: ("Hello", "greeting"), ("World", "planet")
		class DualFileRead
		{
		private:

			int lineNum = 0;

			std::fstream file1;
			std::fstream file2;
		

		public:

			std::string curLine1;
			std::string curLine2;

			std::vector<std::string> tokens1;
			std::vector<std::string> tokens2;

			std::vector<std::pair<std::string, std::string>> tokenPairs;

			DualFileRead(std::string file1, std::string file2);

			std::pair<std::string, std::string> operator[](int subscript);

			std::vector<std::pair<std::string, std::string>> getLine();

			void nextLine();

			//check to see if all lines in files are of equal size
			//returns vector of line indexes if lines of unequal sizes are found
			std::vector<int> checkFiles();

		};

		struct DualFile_Error : public std::exception
		{
			//construct an error with a message
			DualFile_Error(char const* const message) : std::exception(message) {}

			//get error message
			virtual char const* what() const noexcept
			{
				return std::exception::what();
			}
		};

	}
}