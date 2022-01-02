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
	
	PosTagCRF test;

	test.loadWeights("./Machine Learning/Saved Weights/POS Weights/PosCRFweights.sav");

	while (true)
	{

		std::string in;
		std::getline(std::cin, in);
	
		std::vector<std::string> tokens;

		tokens = tokenize<std::string>(in);

		test.createDataset(tokens);

		std::vector<std::pair<int, int>> predictions = test.viterbi();

		for (int i = 0; i < tokens.size(); i++)
		{

			std::cout << predictions.at(i).second << " " << test.tagList->at(predictions.at(i).second) << " " << tokens.at(i) << "\n";

		}
	}

}