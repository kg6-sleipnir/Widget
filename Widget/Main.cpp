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
	
	

}