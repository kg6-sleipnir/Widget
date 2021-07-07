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

	std::vector<float> averageAccuracy(100, 0);

	for (int i = 0; i < 4200; i++)
	{
		//check if next line in tag file and next line in word file have same length
		try
		{
			file.nextLine();
		}
		catch (...)
		{
			continue;
		}
		


		//get features and create dataset from them
		features = test.getFeatures(file.tokens2);
		test.createDataset(features);



		//number of correct predictions for accuracy
		int numCorrect = 0;



		//predict all tags and output them
		for (int i = 0; i < file.tokens2.size(); i++)
		{
			std::pair<int, int> tags = test.predictTag(i, 48, 49);

			std::cout << tags.first << " " << tags.second << "   " << test.tags[tags.second] << "   " << file.tokens2[i] << "             ";

			if (test.tags[tags.second] == file.tokens1[i])
			{
				std::cout << "Correct";
				numCorrect++;
			}
			
			std::cout << "\n";
		}


		//print ending tag
		std::pair<int, int> tags = test.predictTag(file.tokens2.size(), 48, 49);
		std::cout << tags.first << " " << tags.second << "\n";

		
		//print number correct and incorrect
		std::cout << "Number Correct: " << numCorrect << "\nNumber Incorrect: " << file.tokens1.size() - numCorrect << "\n\n";


		//print accuracy of current iteration
		std::cout << "Accuracy: " << (float)numCorrect / file.tokens1.size() << "\n";
		
		
		
		//get accuracy over past 10 iterations
		averageAccuracy.insert(averageAccuracy.begin(), (float)numCorrect / file.tokens1.size());
		averageAccuracy.pop_back();



		//print accuracy over past 10 iterations
		float temp = 0;

		for (int i = 0; i < 10; i++)
		{
			temp += averageAccuracy[i];
		}

		std::cout << "Accuracy over past 10: " << temp / 10 << "\n";

		
		//print accuracy over past 100 iterations
		float temp2 = 0;

		for (int i = 0; i < averageAccuracy.size(); i++)
		{
			temp2 += averageAccuracy[i];
		}

		std::cout << "Accuracy over past 100: " << temp2 / 100 << "\n";


		//push back starting feature
		tagFeatures.push_back(std::pair(features[0], std::string("START")));

		//get features and pair them with correct tags to update weights
		for (int i = 0; i < file.tokens1.size(); i++)
		{
			tagFeatures.push_back(std::pair(features[i + 1], file.tokens1[i]));
		}

		tagFeatures.push_back(std::pair(features[features.size() - 1], std::string("STOP")));
		

		/*std::thread t1(&CRF::updateWeights, &test, tagFeatures, &test.tags, test.transitionMatrix, 1 - temp / 10);
		std::thread t2(&CRF::updateTransitionMatrix, &test, tagFeatures, &test.tags, test.transitionMatrix, 1 - temp / 10);
		t1.join();
		t2.join();*/


		test.updateTransitionMatrix(tagFeatures, &test.tags, test.transitionMatrix, (1.0f - (temp / 10)));
		test.updateWeights(tagFeatures, &test.tags, (1.0f - (temp / 10)));

		tagFeatures.clear();

		std::cout << "\n\n";
	}
	
}