#pragma once
#ifdef _DEBUG

#include <iostream>
#include <string>

#include "./Desicion Trees/Decision Tree.h"
#include "../Data Handling/Dual File Reader.h"
#include "./Logistic Regression/Logistic Regression.h"
#include "../Sentence Processing/Part of Speech Tagging/POS Tagger CRF.h"


namespace MLearn
{
	//test function for decision tree
	//shows accuracy of second file after learning from first
	//leave predictFile as "" if not making predictions
	void DTreeTest(std::string learnFile, std::string predictFile, int targetClass, int maxRecursions);

	//test function for logistic regression
	//uses 2 values of 1 or 0 as input and outputs 1 value of inputs using XOR
	//also shows the error of the network
	void LogRegTest();

	//test function for logistic regression
	//reads data from "./Machine Learning/Training Data/Power Plant Data TRAIN.txt"
	//and calculates the predicted output in the 5th column based on the
	//data from the first 4
	void LogRegTest2();

	//test function for PosTagCRF object using
	//conditional random fields to predict the 
	//part of speech tag of a word in a sequence
	//reads data from "./Machine Learning/Training Data/TOS data/Original Data/tags.txt"
	//and "./Machine Learning/Training Data/TOS data/Original Data/Words.txt"
	void CRFTest();


}

#endif