#pragma once
#ifdef _DEBUG

#include <iostream>
#include <string>

#include "Decision Tree.h"
#include "Logistic Regression.h"


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

}

#endif