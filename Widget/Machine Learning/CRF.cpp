#include "CRF.h"

void MLearn::CRF::addFeature(std::string feature)
{
	//check if feature is not in features vector
	if (std::find(features.begin(), features.end(), feature) == tags.end())
	{
		//add feature to features vector
		features.push_back(feature);
	}
}

void MLearn::CRF::addTag(std::string tag)
{
	//check if tag is not in tags vector
	if (std::find(tags.begin(), tags.end(), tag) == tags.end())
	{
		//add tag to tags vector
		tags.push_back(tag);
	}
}

void MLearn::CRF::getFeaturesFromFile(std::string inputFile)
{
	//initialize and open file
	std::fstream file;
	file.open(inputFile, std::ios::in);

	//create temporary value to hold current line of text
	std::string curLine;


	//iterate over lines in file
	while (std::getline(file, curLine))
	{
		//create vector for holding tokens
		std::vector<std::string> tokens;


		//tokenize current line with exception handling
		try
		{
			tokens = tokenize<std::string>(curLine);
		}
		catch (std::exception& e)
		{
			std::cout << e.what() << "\n";
			continue;
		}


		//iterate over current tokens
		for (const auto& i : tokens)
		{
			//check if current token is already in the features vector
			if (std::find(features.begin(), features.end(), i) != features.end())
			{
				continue;
			}
			else
			{
				//add token to features
				features.push_back(i);
			}
		}
	}

	file.close();
}

void MLearn::CRF::getTagsFromFile(std::string inputFile)
{
	//initialize and open file
	std::fstream file;
	file.open(inputFile, std::ios::in);

	//temporary value to hold current line in file
	std::string curLine;


	//iterate over lines in file
	while (std::getline(file, curLine))
	{
		//temporary value to hold current tokens
		std::vector<std::string> tokens;


		//tokenize current line with exception handling
		try
		{
			tokens = tokenize<std::string>(curLine);
		}
		catch (std::exception& e)
		{
			std::cout << e.what() << "\n";
			continue;
		}


		//iterate over tokens
		for (const auto& i : tokens)
		{
			//check if token is already in tags vector
			if (std::find(tags.begin(), tags.end(), i) != tags.end())
			{
				continue;
			}
			else
			{
				//add token to tags vector
				tags.push_back(i);
			}
		}
	}
	file.close();
}

std::vector<std::vector<std::vector<float>>> MLearn::CRF::createProbabilityMatrices()
{
	
}

