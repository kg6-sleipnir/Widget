#include "Dual File Reader.h"

using namespace Data;

File::DualFileRead::DualFileRead(std::string inputFile1, std::string inputFile2)
{
	//open files
	file1.open(inputFile1, std::ios::in);
	file2.open(inputFile2, std::ios::in);

	//check if files were not opened
	if (!file1)
	{
		throw DualFile_Error("Unable to Open File1");
	}
	else if (!file2)
	{
		throw DualFile_Error("Unable to Open File2");
	}

	//call for next line
	nextLine();
}

std::vector<std::pair<std::string, std::string>> Data::File::DualFileRead::getLine()
{
	//get the lines as pairs
	return tokenPairs;
}

void File::DualFileRead::nextLine()
{
	lineNum++;

	//get the lines from the files
	std::getline(file1, curLine1);
	std::getline(file2, curLine2);


	//check for errors while tokenizing
	try
	{
		tokens1 = tokenize<std::string, ' '>(curLine1);
	}
	catch (Tokenizer_Error& e)
	{
		throw DualFile_Error(std::string("Error Tokenizing Line in File1: ").append(e.what()).c_str()); //string shinanigans
	}

	//check for more errors while tokenizing
	try
	{
		tokens2 = tokenize<std::string, ' '>(curLine2);
	}
	catch (Tokenizer_Error& e)
	{
		throw DualFile_Error(std::string("Error Tokenizing Line in File2: ").append(e.what()).c_str());
	}


	//check for inconsistency in amount of tokens
	if (tokens1.size() != tokens2.size())
	{
		throw DualFile_Error("Line Size in Files are Unequal");
	}


	//clear previous tokens
	tokenPairs.clear();

	//set token pairs
	for (int i = 0; i < tokens1.size(); i++)
	{
		tokenPairs.push_back(std::pair<std::string, std::string>(tokens1[i], tokens2[i]));
	}
}

std::vector<int> Data::File::DualFileRead::checkFiles()
{
	//vector to hold index of lines of unequal size
	std::vector<int> unequalLineIndexes;
	

	//temporary strings
	std::string tempLine1;
	std::string tempLine2;

	//temporary tokens
	std::vector<std::string> tempTokens1;
	std::vector<std::string> tempTokens2;

	//line index
	int line = 0;


	//iterate over lines
	while (std::getline(file1, tempLine1) and std::getline(file2, tempLine2))
	{
		line++;

		
		//check for errors while tokenizing
		try
		{
			tempTokens1 = tokenize<std::string, ' '>(tempLine1);
		}
		catch (Tokenizer_Error& e)
		{
			std::cout << "Error Tokenizing Line in File1: " << e.what() << ", skipping line: " << line << "\n";
			continue;
		}

		//check for more errors while tokenizing
		try
		{
			tempTokens2 = tokenize<std::string, ' '>(tempLine2);
		}
		catch (Tokenizer_Error& e)
		{
			std::cout << "Error Tokenizing Line in File2: " << e.what() << ", skipping line: " << line << "\n";
			continue;
		}


		//check if lines are of unequal sizes
		if (tempTokens1.size() != tempTokens2.size())
		{
			unequalLineIndexes.push_back(line);
		}
	}

	return unequalLineIndexes;
}

std::pair<std::string, std::string> Data::File::DualFileRead::operator[](int subscript)
{
	if (subscript < 0 or subscript >= tokenPairs.size())
	{
		throw DualFile_Error("Array Index Out of Bounds");
	}

	return tokenPairs[subscript];
}
