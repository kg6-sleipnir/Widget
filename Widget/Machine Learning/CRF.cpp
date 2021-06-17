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



void MLearn::CRF::createFeatureFunctionWeights()
{
	//seed random
	srand(time(NULL));
	
	//iterate over tags and features, adding them to featureFunctions map
	for (const auto& i : tags)
	{
		for (const auto& j : features)
		{
			featureFunctionWeights[std::pair(i, j)] = 0.01f;
		}
	}


	//clear features vector to save memory
	features.clear();
}

void MLearn::CRF::createProbabilityMatrix(std::vector<std::pair<std::string, std::string>>& featureFunctions, int position)
{
	std::vector<std::vector<float>> matrix;

	if (position != 1)
	{
		//get probabilities of previous tags by summing the columns in the previous probability matrix
		std::vector<float> prevTagProbs = Custom::Matrix::sumColumns<float, Custom::Matrix::Fmatrix>(probabilityMatrices[position - 1]);


		for (int i = 0; i < tags.size(); i++)
		{
			std::vector<float> temp;

			for (int j = 0; j < tags.size(); j++)
			{
				temp.push_back(prevTagProbs[i]);
			}

			matrix.push_back(temp);
		}
	}
	else
	{
		for (int i = 0; i < tags.size(); i++)
		{
			std::vector<float> temp;

			for (int j = 0; j < tags.size(); j++)
			{
				temp.push_back(0);
			}

			matrix.push_back(temp);
		}
	}


	if (position != 1)
	{
		//total value for normalizing the probabilities
		//sum of probabilities of previous tag should always add up to 1
		//so we start with that and add the new values
		float totalValue = 1;


		//push back weights for feature functions to get probabilities at position 1 since position 0 is always NUL tag
		for (const auto& i : featureFunctions)
		{
			for (int j = 0; j < matrix.size(); j++)
			{
				//push back probabilities with previous tag being NUL
				matrix[j][std::distance(tags.begin(), std::find(tags.begin(), tags.end(), i.first))] += featureFunctionWeights[i];
				
				//add to total value
				totalValue += featureFunctionWeights[i];
			}
		}


		//normalize probabilities
		for (auto& i : matrix)
		{
			for (auto& j : i)
			{
				j /= totalValue;
			}
		}
	}
	else
	{
		//total value for normalizing the probabilities
		float totalValue = 0;


		//push back weights for feature functions to get probabilities at position 1 since position 0 is always NUL tag
		for (const auto& i : featureFunctions)
		{
			//push back probabilities with previous tag being NUL
			matrix[0][std::distance(tags.begin(), std::find(tags.begin(), tags.end(), i.first))] += featureFunctionWeights[i];

			//add to total value
			totalValue += featureFunctionWeights[i];
		}


		//normalize the probabilities
		for (auto& i : matrix[0])
		{
			i /= totalValue;
		}
	}


	//add matrix to probability matrices
	probabilityMatrices.push_back(matrix);
}
