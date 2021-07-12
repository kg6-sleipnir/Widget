#include "Logistic Regression.h"


using namespace MLearn;

LogNet::LogNet(int numInputs, std::vector<int> layers, int numOutputs)
{
	//create temporary layer of neurons
	std::vector<Neuron> temp;

	

	//create 2D vector of neurons
	for (int i = 0; i < layers.size(); i++)
	{
		//add neurons to temp with extra neuron to act as weight
		for (int j = 0; j < layers[i] + 1; j++)
		{
			//setup neuron with layer number and number of weights equal to
			//number of neurons in previous layer or number of inputs if first layer
			Neuron n([=](int) {
				if (j == layers[i]) //return 0 if last neuron to set bias neuron
				{
					return 0;
				}
				else if (i == 0) 
				{
					return numInputs + 1;
				}
				else
				{
					return layers[i - 1] + 1;
				}
				}(i));

			//push back neuron to temporary layer
			temp.push_back(n);
		}

		//add temp neuron layer to neuron vector
		neurons.push_back(temp);
		temp.clear();
	}


	//add last layer of neurons
	for (int j = 0; j < numOutputs; j++)
	{
		//set temporary neuron
		Neuron n(neurons[neurons.size() - 1].size());

		//push back neuron to temporary layer
		temp.push_back(n);
	}

	//add temp neuron layer to neuron vector
	neurons.push_back(temp);
}

MLearn::LogNet::LogNet(std::string fileName)
{
	std::fstream file;
	file.open(fileName, std::ios::in);

	std::string curLine;
	std::vector<Neuron> temp;


	while (std::getline(file, curLine))
	{
		

		if (curLine == "")
		{
			temp.push_back(Neuron(0));
			
			neurons.push_back(temp);

			temp.clear();
		}
		else
		{
			std::vector<double> tokens = tokenize<double, ' '>(curLine);

			Neuron n;

			for (const auto& i : tokens)
			{
				n.weights.push_back(i);
			}

			temp.push_back(n);
		}
	}

	neurons.push_back(temp);
}

double LogNet::totalError(std::vector<double> correctOut)
{
	//temporary value to hold sum
	double temp = 0;


	//check if size of correctOut is equal to number of neurons
	if (correctOut.size() != neurons[neurons.size() - 1].size())
	{
		throw LogNetError("Output Size Not Equal To Number of Output Neurons");
	}


	//calculate sum of square error in each output neuron
	for (int i = 0; i < correctOut.size(); i++)
	{
		temp += pow(correctOut[i] - neurons[neurons.size() - 1][i].getSigValue(), 2);
	}


	//average error
	temp /= correctOut.size();

	return temp;
}

void LogNet::calculateOutput(std::vector<double> input)
{
	//push back 1 to add bias to first layer
	input.push_back(1);

	
	//check if there is the correct number of inputs
	if (input.size() != neurons[0][0].weights.size())
	{
		throw LogNetError("Input Size is not Equal to Expected Input Size");
	}
	
	//iterate over each neuron layer
	for (int i = 0; i < neurons.size() - 1; i++)
	{
		//iterate over each neuron in layer and calculate output
		for (int j = 0; j < neurons[i].size() - 1; j++)
		{
			neurons[i][j].sigmoidOutput(neurons[i][j].calcNetOutput(input));
		}
		
		//clear current input
		input.clear();
		
		//iterate over neurons to get input of next layer
		for (int j = 0; j < neurons[i].size(); j++)
		{
			input.push_back(neurons[i][j].getSigValue());
		}
	}

	//calculate output of output layer
	for (int j = 0; j < neurons[neurons.size() - 1].size(); j++)
	{
		neurons[neurons.size() - 1][j].sigmoidOutput(neurons[neurons.size() - 1][j].calcNetOutput(input));
	}
}

int MLearn::LogNet::getMaxOut()
{
	double max = 0;
	int maxIndex = 0;

	for (int i = 0; i < neurons.back().size(); i++)
	{
		if (neurons.back()[i].getSigValue() > max)
		{
			max = neurons.back()[i].getSigValue();
			maxIndex = i;
		}
	}

	return maxIndex;
}

double LogNet::DErrDNet(int layer, int neuronNum, double correctOut)
{
	//partial derivative of error total with respect to sigmoid output
	double dETdSig = -1 * (correctOut - neurons[layer][neuronNum].getSigValue());

	//partial derivative of sigmoid output with respect to net output
	double dSigdNet = neurons[layer][neuronNum].getSigValue() * (1 - neurons[layer][neuronNum].getSigValue());

	//partial derivative of net output with respect to curWeight is just sigmoid output of the neuron assosiated with the weight


	//put it all together
	return dETdSig * dSigdNet;
}

void LogNet::updateWeights(std::vector<double> input, std::vector<double> correctAns, float learningRate)
{
	
	
	//get input with extra 1 for bias
	std::vector<double> biasInput(input);
	biasInput.push_back(1);

	//total error of system
	double error = learningRate;

	
	//check if size of correct answers is equal to number of output neurons
	if (correctAns.size() != neurons[neurons.size() - 1].size())
	{
		throw LogNetError("Output Size Not Equal To Number of Output Neurons");
	}


	//sum of error and weight of each neuron in next layer
	//each index contains the sum of the error multiplied by the weight
	//corresponding to that neuron in each of the neurons in the next layer
	//index 0 would be the sum of error and weights of each neuron that correspond to the neuron 0 in previous layer
	std::vector<double> sumErrWeight(neurons[neurons.size() - 2].size(), 0);


	//iterate through neurons in last layer to update their weights
	for (int j = 0; j < neurons[neurons.size() - 1].size(); j++)
	{
		//partial derivative of error over net output
		double ErrNet = DErrDNet(neurons.size() - 1, j, correctAns[j]);


		//iterate through weights in neuron
		for (int curWeight = 0; curWeight < neurons[neurons.size() - 1][j].weights.size() - 1; curWeight++)
		{
			sumErrWeight[curWeight] += ErrNet * neurons[neurons.size() - 1][j].weights[curWeight];
			
			
			//update the last layers weights
			neurons[neurons.size() - 1][j].weights[curWeight] -= ErrNet * neurons[neurons.size() - 2][curWeight].getSigValue() * error;
//			---------update current neuron's weights---------   DerrDnet  ---------previous neuron's sigmoid output-------  learning rate
		}
	}
	
	

	//reverse iterate through other layers except first
	for (int i = neurons.size() - 2; i >= 1; i--)
	{
		//create new vector to hold previous values
		std::vector<double> prevSumErrWeight(sumErrWeight);

		//clear and resize sumErrWeight to get new values as we iterate
		sumErrWeight.clear();
		sumErrWeight.resize(neurons[i - 1].size());


		//iterate through neurons in layer
		for (int j = 0; j < neurons[i].size() - 1; j++)
		{
			//get the partial derivative of the error over the net output
			double ErrNet = prevSumErrWeight[j] * (neurons[i][j].getSigValue() * (1 - neurons[i][j].getSigValue()));
//						   sum of error * weight  |-------------partial derivative current neuron's-------
//						  assosiated with neuron  |                sigmoid output over net output
//												  |_______________________________________________________
//						partial derivative error
//						over net output previous neruon * 
//						pd net output of previous neruon
//						over sigmoid output of current neuron


			//iterate through weights in neuron
			for (int curWeight = 0; curWeight < neurons[i][j].weights.size(); curWeight++)
			{
				//add to sumErrWeight for next layer's calculations
				sumErrWeight[curWeight] += ErrNet * neurons[i][j].weights[curWeight];


				//update current weight
				neurons[i][j].weights[curWeight] -= ErrNet * neurons[i - 1][curWeight].getSigValue() * error;

			}
			
		}
		
	}
	
	

	//iterate through neurons in first layer
	for (int j = 0; j < neurons[0].size() - 1; j++)
	{
		//get the partial derivative of the error over the net output
		double ErrNet = sumErrWeight[j] * neurons[0][j].getSigValue() * (1 - neurons[0][j].getSigValue());


		//iterate through weights in neuron
		for (int curWeight = 0; curWeight < neurons[0][j].weights.size(); curWeight++)
		{
			//update current weight
			neurons[0][j].weights[curWeight] -= ErrNet * biasInput[curWeight] * error;

		}
		
	}

	
}


std::string MLearn::LogNet::weightText()
{
	std::string text;

	for (int i = 0; i < neurons.size(); i++)
	{
		for (int j = 0; j < neurons[i].size(); j++)
		{
			for (int curWeight = 0; curWeight < neurons[i][j].weights.size(); curWeight++)
			{
				text.append(std::to_string(neurons[i][j].weights[curWeight]));
				text.append("\t");
			}

			text.append("\n");
		}
	}

	return text;
}

void MLearn::LogNet::saveWeights(std::string fileName)
{
	std::fstream file;
	file.open(fileName, std::ios::out);

	file << weightText();
}


LogNet::Neuron::Neuron(int numWeights)
{
	if (numWeights != 0)
	{
		//setup value and bias with arbitrary values
		netOutput = 0;
		sigOutput = 0.5;

		//create set of weights
		for (int i = 0; i < numWeights; i++)
		{
			weights.push_back((double)rand() / RAND_MAX);
		}
	}
	else
	{
		//set output to 1 if neuron is the bias controlling neuron
		netOutput = 1;
		sigOutput = 1;
	}
}

//calculate net ouput before the sigmoid function
double LogNet::Neuron::calcNetOutput(std::vector<double> input)
{
	//initialize temporary value
	double temp = 0;


	//iterate through each weight and input
	//and create a linear function using them
	//using the equation
	//NetOutput = (Σ 𝓌ₙ * 𝒾ₙ) + 𝘣
	//where 𝓌ₙ is the nth weight, 𝒾ₙ is the nth input, and 𝘣 is the bias
	//since the last input is always 1 it will be the bias and will update itself
	for (int i = 0; i < input.size(); i++)
	{
		temp += weights[i] * input[i];
	}


	netOutput = temp;
	return temp;
}

//run net output through a sigmoid function
double LogNet::Neuron::sigmoidOutput(double netOutput)
{
	//sigmoid function
	/*
			1
		---------
		      (-netOutput)
		1 - e
	*/
	
	double temp = 1 / (1 + exp(-netOutput));

	sigOutput = temp;
	return temp;
}

double LogNet::Neuron::getSigValue()
{
	return sigOutput;
}

MLearn::LogNetError::LogNetError(char const* const message) : std::exception(message) {}

char const* MLearn::LogNetError::what() const noexcept
{
	return exception::what();
}
