#include <iostream>

#include <algorithm>
#include <fstream>
#include <numeric>
#include <string>


#include "./Machine Learning/Logistic Regression.h"
#include "./Input Processing/Tokenizer.h"
#include "./Machine Learning/CRF.h"
#include "./Machine Learning/POS Tagger CRF.h"
#include "./Data Handling/Dual File Reader.h"

//debugging headers
#ifdef _DEBUG

#include "Machine Learning/MLearn Testing.h"

#endif


using namespace MLearn;

int main()
{
	srand(time(NULL));
	
	PosTagCRF test;

	Data::File::DualFileRead file("./Machine Learning/Training Data/TOS data/Original Data/tags.txt", "./Machine Learning/Training Data/TOS data/Original Data/Words.txt");

	std::vector<std::pair<std::vector<std::string>, std::string>> tagFeatures;

	std::vector<std::vector<std::string>> features;

	for (int i = 0; i < 4200; i++)
	{
		try
		{
			file.nextLine();
		}
		catch (...)
		{
			continue;
		}
		
		features = test.getFeatures(file.tokens2);

		test.createDataset(features);

		for (int i = 0; i < file.tokens2.size(); i++)
		{
			std::pair<int, int> tags = test.predictTag(i, 48, 49);

			std::cout << tags.first << " " << tags.second << " " << file.tokens2[i] << "              ";

			if (test.tags[tags.second] == file.tokens1[i])
			{
				std::cout << "Correct";
			}
			
			std::cout << "\n";
		}

		for (int i = 0; i < file.tokens1.size(); i++)
		{
			tagFeatures.push_back(std::pair(features[i + 1], file.tokens1[i]));
		}

		test.updateWeights(tagFeatures, &test.tags, 0.00001f);

		

		std::cout << "\n\n";
	}
	
}