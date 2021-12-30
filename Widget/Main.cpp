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



using namespace Mat;

using namespace MLearn;

int main()
{
	srand(5);
	
	PosTagCRF test;

	//vector to hold accuracy of each iteration over past 100 iterations
	std::vector<float> averageAccuracy(100, 0);

	//vector to hold number of 100% accuracy iterations - the sum over this vector over 100 is the ratio of 100% accuracies
	std::vector<bool> ratio100Prcnt(100, 0);

	//amount of 100% predictions
	int num100prcnt = 0;


	//learn over dataset multiple times to become more accurate
	for (int k = 0; k < 5; k++)
	{

		test.unfreezeWeights();

		//load the file containing tags and file containing words to predict
		Data::File::DualFileRead file(TAG_FILE, WORD_FILE);

		//reset amount of 100% predictions
		num100prcnt = 0;

		//number of lines in words and tags files
		for (int i = 0; i < 4200; i++)
		{


			//check if next line in tag file and next line in word file have same length
			try
			{
				file.nextLine();
				std::cout << "Iteration: " << i + 1 << "\n\n";
			}
			catch (...)
			{
				std::cout << "Iteration  " << i + 1 << " Failed, Continuing to Next Iteration\n\n";
				continue;
			}



			//create dataset on current words
			test.createDataset(file.tokens2);


			//clear f1 scores every 1000 iterations
			if (i % 1000 == 0)
			{
				test.clearF1Scores();
			}


			//number of correct predictions in current iteration to calculate accuracy
			int numCorrect = 0;

			
			//set of predictions using the Viterbi algorithm
			std::vector<std::pair<int, int>> predictions = test.viterbi();


			//output the predicted tags
			for (int i = 0; i < predictions.size() - 1; i++)
			{
				//current predicted tags
				std::pair<int, int> tags = predictions[i];


				//output the predicted tags and the current word
				std::cout << tags.first << " " << tags.second << "   " << test.tagList->at(tags.second) << "   " << file.tokens2[i] << "             ";


				//check if the prediction is a null tag
				if (tags.second == 0)
				{
					std::cout << "Prediction Failed";
				}

				//check if the prediction is correct
				if (test.tagList->at(tags.second) == file.tokens1[i])
				{
					std::cout << "Correct";
					numCorrect++;
				}

				//add current predictions to the f1 scores
				test.addToF1Scores(file.tokens1[i], test.tagList->at(tags.second));

				std::cout << "\n";
			}


			//print number correct and incorrect
			std::cout << "Number Correct: " << numCorrect << "\nNumber Incorrect: " << file.tokens1.size() - numCorrect << "\n\n";


			//print accuracy of current iteration
			std::cout << "Accuracy: " << (float)numCorrect / file.tokens1.size() << "\n";


			//add to the number of 100% predictions if entire prediction is correct
			if ((float)numCorrect / (predictions.size() - 1) == 1)
			{
				num100prcnt++;
			}

			//print number of 100% predictions
			std::cout << "\nNumber of 100% predictions: " << num100prcnt << "\n\n";

			//add accuracy of current iteration to average accuracy vector
			averageAccuracy.insert(averageAccuracy.begin(), (float)numCorrect / file.tokens1.size());
			averageAccuracy.pop_back();


			//add to ratio of 100% predictions over past 100 iterations
			if ((float)numCorrect / (predictions.size() - 1) == 1.0f)
			{
				ratio100Prcnt.insert(ratio100Prcnt.begin(), 1);
			}
			else
			{
				ratio100Prcnt.insert(ratio100Prcnt.begin(), 0);
			}

			//delete last number in vector holding ratio of 100% predictions to keep amount at 100
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


			//print current f1 scores
			test.printF1Scores();


			//iterate the weights during training period but not testing period
			if (i < 4000)
			{

#define learnRate 0.1f

				test.iterateWeights(file.tokens1, learnRate / (sqrt(k) + 1));

			}
			else if (i == 4000) //clear f1 scores going into testing period
			{
				test.clearF1Scores();
				test.removeWeights(-0.2f, 0.2f);
				test.saveWeights("output.sav");
				test.freezeWeights();
			}


			std::cout << "\n\n";

			

		}
	}
}

