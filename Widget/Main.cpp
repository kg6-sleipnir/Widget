#include <iostream>

#include <algorithm>
#include <fstream>
#include <numeric>
#include <string>
#include <thread>

#include "./Machine Learning/Logistic Regression.h"
#include "./Input Processing/Tokenizer.h"
#include "./Machine Learning/CRF.h"
#include "./Machine Learning/POS Tagger CRF.h"
#include "./Data Handling/Dual File Reader.h"

//debugging headers
#ifdef _DEBUG

#include "Machine Learning/MLearn Testing.h"

#endif


#define TAG_FILE "./Machine Learning/Training Data/TOS data/Original Data/tags.txt"
#define WORD_FILE "./Machine Learning/Training Data/TOS data/Original Data/Words.txt"


using namespace MLearn;

int main()
{
	srand(5);
	
	PosTagCRF test;

	Data::File::DualFileRead file(TAG_FILE, WORD_FILE);

	std::vector<std::pair<std::vector<std::string>, std::string>> tagFeatures;

	std::vector<std::vector<std::string>> features;

	std::vector<float> averageAccuracy(100, 0);

	int iteration = 0;

	int num100prcnt = 0;

	//number of lines in words and tags files
	for (int i = 0; i < 4200; i++)
	{
		iteration++;

		
		//check if next line in tag file and next line in word file have same length
		try
		{
			file.nextLine();
			std::cout << "Iteration: " << iteration << "\n\n";
		}
		catch (...)
		{
			std::cout << "Iteration  " << iteration << " Failed, Continuing to Next Iteration\n\n";
			continue;
		}
		


		//get features and create dataset from them
		features = test.getFeatures(file.tokens2);
		
		test.createDataset(features);


		

		if (file.tokens2.size() > 27)
		{
			std::cout << "\n\n";

			tagFeatures.clear();

			continue;
		}
			
			
		if (iteration % 500 == 0)
		{
			test.tagF1Frequencies.clear();
		}


		//number of correct predictions for accuracy
		int numCorrect = 0;



		//predict all tags and output them
		for (int i = 0; i < file.tokens2.size(); i++)
		{
			std::pair<int, int> tags = test.predictTag(i, 45, 46);

			std::cout << tags.first << " " << tags.second << "   " << test.tags[tags.second] << "   " << file.tokens2[i] << "             ";

			if (tags.first == 0 or tags.second == 0)
			{
				std::cout << "Prediction Failed";
			}


			if (test.tags[tags.second] == file.tokens1[i])
			{
				std::cout << "Correct";
				numCorrect++;
				test.tagF1Frequencies[file.tokens1[i]][0]++;
			}
			else
			{
				test.tagF1Frequencies[test.tags[tags.second]][1]++;
				test.tagF1Frequencies[file.tokens1[i]][2]++;
			}

			std::cout << "\n";
		}


		//print ending tag
		std::pair<int, int> tags = test.predictTag(file.tokens2.size(), 45, 46);
		std::cout << tags.first << " " << tags.second << "\n";

		{
			//print ending tag
			/*std::pair<int, int> tags = test.predictTag(file.tokens2.size() + 1, 45, 46);
			std::cout << tags.first << " " << tags.second << "\n";*/
		}

		//print number correct and incorrect
		std::cout << "Number Correct: " << numCorrect << "\nNumber Incorrect: " << file.tokens1.size() - numCorrect << "\n\n";


		//print accuracy of current iteration
		std::cout << "Accuracy: " << (float)numCorrect / file.tokens1.size() << "\n";

		if ((float)numCorrect / file.tokens1.size() == 1)
		{
			num100prcnt++;
		}

		std::cout << "\nNumber of 100% predictions: " << num100prcnt << "\n\n";

		//get accuracy over past 10 iterations
		averageAccuracy.insert(averageAccuracy.begin(), (float)numCorrect / file.tokens1.size());
		averageAccuracy.pop_back();



		//print accuracy over past 10 iterations
		float past10Accrcy = 0;

		for (int i = 0; i < 10; i++)
		{
			past10Accrcy += averageAccuracy[i];
		}

		std::cout << "Accuracy over past 10: " << past10Accrcy / 10 << "\n";
		


		//print accuracy over past 100 iterations
		float past100Accrcy = 0;

		for (int i = 0; i < averageAccuracy.size(); i++)
		{
			past100Accrcy += averageAccuracy[i];
		}

		std::cout << "Accuracy over past 100: " << past100Accrcy / 100 << "\n";
		

		std::cout << "\n\n";

			
			
		test.printF1Scores();
			
		


		

		//push back starting feature
		//tagFeatures.push_back(std::pair(features[0], std::string("START")));

		//get features and pair them with correct tags to update weights
		for (int i = 0; i < file.tokens1.size(); i++)
		{
			tagFeatures.push_back(std::pair(features[i], file.tokens1[i]));
		}

		tagFeatures.push_back(std::pair(features[features.size() - 1], std::string("STOP")));
		

	
		
		//update the weights and transition matrix using multithreading
		
		if (true) //apparently, for some stupid reason, you can't skip initialization of variables using goto, so this gets rid of that error
		{
			//std::thread t1(&PosTagCRF::updateWeights, &test, std::ref(tagFeatures), &test.tags, std::array<float, 7>({ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 5.0f, 10.0f }));
			std::thread t1(&PosTagCRF::updateWeightsCLL, &test, tagFeatures, &test.tags, 10.0f - (past100Accrcy * 10.0f / 100), 45, 46);

			//std::thread t2(&PosTagCRF::updateTransitionMatrix, &test, std::ref(tagFeatures), &test.tags, std::ref(test.transitionMatrix), std::array<float, 2>({ 0.005f, 0.05f }));
			std::thread t2(&PosTagCRF::updatetransitionMatrixCLL, &test, tagFeatures, &test.tags, std::ref(test.transitionMatrix), 1.0f - (past100Accrcy * 1.0f / 100), 45, 46);


			t1.join();
			t2.join();


			test.tMatrix = test.transitionMatrix;
			
			/*test.updateWeightsCLL(tagFeatures, &test.tags, 5 - (past100Accrcy * 5), 45, 46);
			
			test.updatetransitionMatrixCLL(tagFeatures, &test.tags, test.transitionMatrix, 2.5f - (past100Accrcy * 2.5f), 45, 46);*/
			
		}
		//test.updateWeights(tagFeatures, &test.tags, std::array<float, 7>({ 4.6f, 4.6f, 4.4f, 4.5f, 4.5f, 4.0f, 4.3f }));
	
	

		std::cout << "\n\n";

		tagFeatures.clear();
	
	
	
		

	}
}