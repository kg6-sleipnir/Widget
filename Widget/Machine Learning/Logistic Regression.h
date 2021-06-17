#pragma once
#include <eh.h>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <vector>

#include "../Input Processing/Tokenizer.h"

#define _USE_MATH_DEFINES
#include <math.h>


namespace MLearn
{

	//neural network using logistic regression
	class LogNet
	{
#ifdef _DEBUG
	public:
#else
	private:
#endif

		class Neuron
		{
		public:

			//net value of neuron before sigmoid function
			double netOutput;

			//value of neuron after sigmoid function
			double sigOutput;

		public:

			//weights between previous layer and current neuron
			std::vector<double> weights;

			Neuron() { netOutput = 0; sigOutput = 0.5; }

			Neuron(int numWeights);

			double calcNetOutput(std::vector<double> input);

			double sigmoidOutput(double netOutput);

			double getSigValue();

		};

		//calculate the partial derivative of the error over the net output of a neuron in the output layer
		double DErrDNet(int layer, int neuronNum, double correctOut);

	public:

		//list of neurons in the network
		//first dimension is the layer of the neuron
		//second dimension is neurons index in that layer
		std::vector<std::vector<Neuron>> neurons;


		//construct logistic regression model
		//numInputs = number of inputs in model
		//layers = number of neurons in each layer - every element in vector is new layer with an integer amount of neurons
		//numOutputs = number of different possible output features
		LogNet(int numInputs, std::vector<int> layers, int numOutputs);

		//create logistic regression network from saved weights file
		LogNet(std::string fileName);

		//get mean square error of output neurons
		double totalError(std::vector<double> correctOut);

		//calculate output of network based on input
		void calculateOutput(std::vector<double> input);

		//get the index of the output neuron with the highest output
		int getMaxOut();

		//update the weights of the entire model
		//this function calls calculateOutput() so no need to call it before this
		void updateWeights(std::vector<double> input, std::vector<double> correctAns, float learningRate);

		//creates string from weights in each layer for either output or saving to file
		std::string weightText();

		//save weights to file
		void saveWeights(std::string fileName);

	};


	struct LogNetError : public std::exception
	{

		//construct an error with a message
		LogNetError(char const* const message);

		//get error message
		virtual char const* what() const noexcept;

	};
}