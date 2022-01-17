#pragma once

#include <exception>
#include <string>
#include <vector>


struct Conversion_Error : public std::exception
{
	//construct an error with a message
	Conversion_Error(char const* const message) : std::exception(message) {}

	//get error message
	virtual char const* what() const noexcept
	{
		return std::exception::what();
	}
};


/// <summary>
/// Convert vector of strings to vector of int
/// </summary>
/// <param name="tokens">- input vector</param>
/// <returns>vector of int</returns>
static std::vector<int> convertToInt(std::vector<std::string> tokens)
{

	std::vector<int> returnVector;

	for (int i = 0; i < tokens.size(); i++)
	{

		if (std::find_if(tokens[i].begin(), tokens[i].end(), [](unsigned char c) { return !std::isdigit(c); }) == tokens[i].end())
		{
			returnVector.push_back(std::stoi(tokens[i]));
		}
		else
		{
			throw Conversion_Error("Could not convert string to int");
		}
	}

	return returnVector;
}

/// <summary>
/// Convert vector of string to vector of doubles
/// </summary>
/// <param name="tokens">- input vector</param>
/// <returns>vector of doubles</returns>
static std::vector<double> convertToDouble(std::vector<std::string> tokens)
{

	std::vector<double> returnVector;

	for (int i = 0; i < tokens.size(); i++)
	{
		try
		{
			returnVector.push_back(std::stod(tokens[i]));
		}
		catch (...)
		{
			throw Conversion_Error("Could not convert string to double");
		}
	}

	return returnVector;
}


/// <summary>
/// Convert vector of strings to vector of floats
/// </summary>
/// <param name="tokens">- input vector</param>
/// <returns>vector of floats</returns>
static std::vector<float> convertToFloat(std::vector<std::string> tokens)
{

	std::vector<float> returnVector;

	for (int i = 0; i < tokens.size(); i++)
	{
		try
		{
			returnVector.push_back(std::stof(tokens[i]));
		}
		catch (...)
		{
			throw Conversion_Error("Could not convert string to double");
		}
	}

	return returnVector;
}


/// <summary>
/// Convert vector of strings to vector of chars
/// </summary>
/// <param name="tokens">- input vector</param>
/// <returns>vector of chars</returns>
static std::vector<char> convertToChar(std::vector<std::string> tokens)
{

	std::vector<char> returnVector;

	for (int i = 0; i < tokens.size(); i++)
	{
		if (tokens[i].size() == 1)
		{
			returnVector.push_back(tokens[i][0]);
		}
		else
		{
			throw Conversion_Error("Could not convert string to double");
		}
	}

	return returnVector;
}

//error object for tokenizer
struct Tokenizer_Error : public std::exception
{
	//construct an error with a message
	Tokenizer_Error(char const* const message) : std::exception(message) {}

	//get error message
	virtual char const* what() const noexcept
	{
		return std::exception::what();
	}
};

