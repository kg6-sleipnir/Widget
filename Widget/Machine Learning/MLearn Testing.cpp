#ifdef _DEBUG


#include "MLearn Testing.h"

using namespace MLearn;

void MLearn::DTreeTest(std::string learnFile, std::string predictFile, int targetClass, int maxRecursions)
{	
	try
	{
		//create decision tree
		DecisionTree tree(learnFile, targetClass, maxRecursions);

		
		std::cout << "\n\n";

		//display entropy of each variable
		for (int i = 0; i < targetClass; i++)
		{
			std::cout << tree.entropy(*tree.table, i) << " ";
		}

		std::cout << "\n\n" << tree.entropy(*tree.table) << "\n\n";


		//check if there is a file to predict
		if (predictFile != "")
		{
			//temp values to hold number of correct and incorrect predictions
			int correct = 0;
			int incorrect = 0;

			std::fstream file;
			file.open(predictFile, std::ios::in);

			if (!file)
			{
				throw "Prediction File Not Found";
			}

			//temp value to hold current line as string before tokenizing
			std::string line;

			while (getline(file, line))
			{
				//tokenize line into variables
				std::vector<int> V = tokenize<int>(line);
				
				//make prediction
				int answer = tree.query(V);

				//check if prediction is correct by comparing with target class
				if (answer == V[targetClass])
				{
					std::cout << answer << "\n";
					correct++;
				}
				else
				{
					std::cout << answer << " Is Incorrect\n";
					incorrect++;
				}
			}

			//output number of correct and incorrect predictions and total accuracy
			std::cout << "\nCorrect Answers: " << correct << "\nIncorrect Answers: " << incorrect << "\n\nAccuracy: " << (double)correct / ((double)correct + incorrect) * 100 << "%";
		}

		std::cout << tree.treeText(tree.tree);

		tree.saveTree("./Machine Learning/Training Data/tables/saved tables/saved test table.txt");
	}
	catch (Data::TABLE_ERROR& e)
	{
		std::cout << "Table_Error(" << e.what() << ")\n";
	}
	catch (const char* s)
	{
		std::cout << s << "\n";
	}
	catch (...)
	{
		std::cout << "Unknown Error\n";
	}
}


void MLearn::LogRegTest()
{
	//create test object
	LogNet test(2, std::vector<int>({ 4 }), 1);

	//seed random to arbitrary value to make every learning cycle the same
	srand(5);


	//learn weights for taking two inputs that are 1 or 0 and XOR them
	for (int i = 0; i < 10000; i++)
	{
		//create input vector of two numbers of either 1 or 0
		std::vector<double> input = { (double)(rand() % 2), (double)(rand() % 2) };

		//XOR the inputs together and convert it to a double
		std::vector<double> output = { (double)((int)input[0] xor (int)input[1]) };


		//calculate the output to get prediction for updateWeights() function
		test.calculateOutput(input);

		std::cout << test.neurons[test.neurons.size() - 1][0].getSigValue() << "\t";

		//update the weights
		test.updateCurWeights(input, output, 15);


		//it is optional but you can call calculateOutput here
		//to better see what the prediction is after updating the weights
		test.calculateOutput(input);


		//output total error
		std::cout << i << " " << test.totalError(output);

		//output the input and output vector
		std::cout << "\t{" << input[0] << ", " << input[1] << "}\t" << output[0] << "\t";

		//output the predicted answer
		std::cout << test.neurons[test.neurons.size() - 1][0].getSigValue() << "\n";
	}


	//print space between learning and prediction cycles
	for (int i = 0; i < 100; i++)
	{
		std::cout << "\n";
	}


	//output prediction when input is { 0, 0 }
	{
		std::vector<double> input = { 0.0, 0.0 };
		std::vector<double> output = { (double)((int)input[0] xor (int)input[1]) };

		test.calculateOutput(input);

		std::cout << test.totalError(output);

		std::cout << "\t{" << input[0] << ", " << input[1] << "}\t" << output[0] << "\t";
		std::cout << test.neurons[test.neurons.size() - 1][0].getSigValue() << "\n";
	}

	//output prediction when input is { 1, 0 }
	{
		std::vector<double> input = { 1.0, 0.0 };
		std::vector<double> output = { (double)((int)input[0] xor (int)input[1]) };

		test.calculateOutput(input);

		std::cout << test.totalError(output);

		std::cout << "\t{" << input[0] << ", " << input[1] << "}\t" << output[0] << "\t";
		std::cout << test.neurons[test.neurons.size() - 1][0].getSigValue() << "\n";
	}

	//output prediction when input is { 0, 1 }
	{
		std::vector<double> input = { 0.0, 1.0 };
		std::vector<double> output = { (double)((int)input[0] xor (int)input[1]) };

		test.calculateOutput(input);

		std::cout << test.totalError(output);

		std::cout << "\t{" << input[0] << ", " << input[1] << "}\t" << output[0] << "\t";
		std::cout << test.neurons[test.neurons.size() - 1][0].getSigValue() << "\n";
	}

	//output prediction when input is { 1, 1 }
	{
		std::vector<double> input = { 1.0, 1.0 };
		std::vector<double> output = { (double)((int)input[0] xor (int)input[1]) };

		test.calculateOutput(input);

		std::cout << test.totalError(output);

		std::cout << "\t{" << input[0] << ", " << input[1] << "}\t" << output[0] << "\t";
		std::cout << test.neurons[test.neurons.size() - 1][0].getSigValue() << "\n";
	}


	//print learned weights
	std::cout << "\n\n\n" << test.weightText();
}

void MLearn::LogRegTest2()
{
	//logistic regression object
	LogNet test(4, std::vector<int>({ 2 }), 1);

	//temporary file object
	std::fstream file;

	//temporary string for current line in file
	std::string curLine;
	
	//vector that will hold expected output
	std::vector<double> output(1, 0);

	//vector that holds the error of the past 50 itterations
	std::vector<double> error(50, 1);

	//learning rate
	double learningRate = 15;

	//vector that holds error of every itteration
	std::vector<double> tError;

	//iterate over the file several times for greater accuracy (hopefully)
	for (int k = 0; k < 1; k++)
	{
		//open the file
		file.open("./Machine Learning/Training Data/Power Plant Data TRAIN.txt", std::ios::in);

		//iterate over the lines in the file and get line as a string
		for (int j = 0; std::getline(file, curLine); j++)
		{
			//get input vector by tokenizing the current line
			std::vector<double> input = tokenize<double>(curLine);
			
			//get the last element in the line as an answer variable
			double ans = input.back();
			input.pop_back();


			//normalize the input by subtracting the minimum value and dividing by the
			//maximum minus the minimum so that every value is between 0 and 1
			input[0] = (input[0] - 1.81) / 35.3;
			input[1] = (input[1] - 25.36) / 56.2;
			input[2] = (input[2] - 992.89) / 40.41;
			input[3] = (input[3] - 25.56) / 74.6;

			//normalize the output in the same way
			output[0] = (ans - 420.26) / 75.5;


			//calculate the mean error over the past 50 itterations
			double ME = [=] {
				double temp = 0;
				for (const auto& i : error)
				{
					temp += i;
				}

				return temp / error.size();
			}();


			//decrease the learning rate
			//as the learning rate decreases, the rate at which it decrements also decreases
			learningRate *= 0.99999;


			//calculate output before updating weights for better calculations
			test.calculateOutput(input);

			//update the weights using the input, expected output, and the learning rate
			test.updateCurWeights(input, output, ME * learningRate);

			//calculate the output so we can get current error 
			test.calculateOutput(input);


			//push back to the error over the entire training cycle
			tError.push_back(abs(output[0] - test.neurons[test.neurons.size() - 1][0].getSigValue()));

			//push back to the error over the past 50 itterations
			error.insert(error.begin(), tError.back());
			error.pop_back();
			

			//output data to console
			std::cout << tError.back() << "\t" << learningRate << "\t" << ME << "\t" << ans << "\t" << test.neurons[test.neurons.size() - 1][0].getSigValue() * 75.5 + 420.26 << "\n";
		}
		//close the file
		file.close();
	}


	//get error over entire learning cycle
	{
		double temp = 0;
		for (const auto& i : tError)
		{
			temp += i;
		}
		temp /= tError.size();
		std::cout << "\n\n" << temp << "\n\n";
	}

	
	//make predictions on prediction file
	{
		file.open("./Machine Learning/Training Data/Power Plant Data PREDICT.txt", std::ios::in);


		//create empty space in console to separate learning and predictions
		for (int j = 0; j < 100; j++)
		{
			std::cout << "\n";
		}


		//reset the error so it's not contaminated from the learning cycle
		error.clear();
		

		//get current line while itterating over file
		while (std::getline(file, curLine))
		{
			//tokenize the current line to get inputs and output
			std::vector<double> input = tokenize<double>(curLine);

			//get expected output from 5th element
			double ans = input.back();
			input.pop_back();

			//normalize all inputs to values between 0 and 1
			input[0] = (input[0] - 1.81) / 35.3;
			input[1] = (input[1] - 25.36) / 56.2;
			input[2] = (input[2] - 992.89) / 40.41;
			input[3] = (input[3] - 25.56) / 74.6;

			//do the same with the expected output
			output[0] = (ans - 420.26) / 75.5;


			//calculate the predicted output
			test.calculateOutput(input);

			//insert the error of the current iterration into error vector
			error.push_back(test.totalError(output));


			//calculate the mean error over the entire testing cycle
			double MSE = [=] {
				double temp = 0;
				for (const auto& i : error)
				{
					temp += i;
				}
				return temp / error.size();
			}();


			//output data to console
			std::cout << test.totalError(output) << "\t" << MSE << "\t" << ans << "\t" << test.neurons[test.neurons.size() - 1][0].getSigValue() * 75.5 + 420.26 << "\n";
		}
	}
}

void MLearn::CRFTest()
{

#define TAG_FILE "./Machine Learning/Training Data/TOS data/Original Data/tags.txt"
#define WORD_FILE "./Machine Learning/Training Data/TOS data/Original Data/Words.txt"

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
				test.removeWeights(-0.15f, 0.15f);
				test.saveWeights("./CRFtestOutput.sav");
				test.freezeWeights();
			}

			std::cout << "\n\n";

		}
	}
}

#endif