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

	

	std::vector<std::pair<std::vector<std::string>, std::string>> tagFeatures;

	std::vector<std::vector<std::string>> features;

	//vector to hold accuracy of each iteration over past 100 iterations
	std::vector<float> averageAccuracy(100, 0);

	//vector to hold number of 100% accuracy iterations - the sum over this vector over 100 is the ratio of 100% accuracies
	std::vector<bool> ratio100Prcnt(100, 0);

	int iteration = 0;

	int num100prcnt = 0;

	for (int k = 0; k < 5; k++)
	{

		Data::File::DualFileRead file(TAG_FILE, WORD_FILE);

		//iteration = 0;

		num100prcnt = 0;

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




			/*if (file.tokens2.size() > 27)
			{
				std::cout << "\n\n";

				tagFeatures.clear();

				continue;
			}*/


			if (iteration % 1000 == 0)
			{
				test.tagF1Frequencies.clear();
			}


			//number of correct predictions for accuracy
			int numCorrect = 0;


			std::vector<std::pair<int, int>> tagProbs = test.viterbi();


			//predict all tags and output them
			for (int i = 0; i < file.tokens2.size(); i++)
			{
				std::pair<int, int> tags = tagProbs[i];

				std::cout << tags.first << " " << tags.second << "   " << test.tags[tags.second] << "   " << file.tokens2[i] << "             ";

				if (tags.second == 0)
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
			//std::pair<int, int> tags = test.predictTag(file.tokens2.size());
			//std::cout << tags.first << " " << tags.second << "\n";


			//print number correct and incorrect
			std::cout << "Number Correct: " << numCorrect << "\nNumber Incorrect: " << file.tokens1.size() - numCorrect << "\n\n";


			//print accuracy of current iteration
			std::cout << "Accuracy: " << (float)numCorrect / file.tokens1.size() << "\n";

			if ((float)numCorrect / file.tokens1.size() == 1)
			{
				num100prcnt++;
			}

			std::cout << "\nNumber of 100% predictions: " << num100prcnt << "\n\n";

			//add accuracy of current iteration to average accuracy vector
			averageAccuracy.insert(averageAccuracy.begin(), (float)numCorrect / file.tokens1.size());
			averageAccuracy.pop_back();


			//add to ratio of 100% predictions over past 100 iterations
			if ((float)numCorrect / file.tokens1.size() == 1.0f)
			{
				ratio100Prcnt.insert(ratio100Prcnt.begin(), 1);
			}
			else
			{
				ratio100Prcnt.insert(ratio100Prcnt.begin(), 0);
			}

			ratio100Prcnt.pop_back();



			//print ratio of 100% predictions
			int ratio = 0;

			for (int i = 0; i < 100; i++)
			{
				ratio += ratio100Prcnt[i];
			}

			std::cout << "Ratio of 100% predictions: " << (float)ratio / 100 << "\n\n";


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

			//for (int i = 0; i < 1; i++)
			if (i < 4000)
			{

#define learnRate 0.1f

				//std::thread t1(&PosTagCRF::updateWeights, &test, std::ref(tagFeatures), &test.tags, std::array<float, 7>({ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 5.0f, 10.0f }));
				std::thread t1(&PosTagCRF::updateWeights, &test, tagFeatures, learnRate / (sqrt(k) + 1)/*- (float)(iteration / 4200 * learnRate)*//*- (past100Accrcy * learnRate / 100)*/);

				std::thread t2(&PosTagCRF::updatePrimeWeights, &test, tagFeatures, learnRate / (sqrt(k) + 1)/*- (float)(iteration / 4200 * learnRate))*//*- (past100Accrcy * learnRate / 100)*/);

				//std::thread t3(&PosTagCRF::updatetransitionMatrix, &test, tagFeatures, &test.tags, std::ref(test.transitionMatrix), 0.01f - (past100Accrcy * 0.01f / 100));


				t1.join();
				t2.join();
				//t3.join();


				test.tMatrix = test.transitionMatrix;

				/*test.updateWeightsCLL(tagFeatures, &test.tags, 5 - (past100Accrcy * 5));

				test.updatetransitionMatrixCLL(tagFeatures, &test.tags, test.transitionMatrix, 2.5f - (past100Accrcy * 2.5f));*/

			}
			else if (i == 4000)
			{
				test.tagF1Frequencies.clear();
			}

			//test.updateWeights(tagFeatures, &test.tags, std::array<float, 7>({ 4.6f, 4.6f, 4.4f, 4.5f, 4.5f, 4.0f, 4.3f }));



			std::cout << "\n\n";

			tagFeatures.clear();





		}
	}
}
