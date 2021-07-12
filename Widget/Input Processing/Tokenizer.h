#pragma once

#include <any>
#include <eh.h>
#include <iostream>
#include <string>
#include <typeinfo>
#include <vector>


//"the root of all evil is the tokenizer" -me
//"and periods" -also me


//I swear if this code breaks again I will go crazy
//im going crazy, can you guess why?
//i think its finally done now... maybe... hopefully...
//nope, i had to fix it many... MANY more times

//rewrote the whole thing for a second time :(

//and now a third time cause I'm an idiot for not just using delimiters
//now I'm going to have to go back and update everything that uses the tokenizer

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


//turns a string into a set of tokens of a specified type
//valid return types are [ INT, DOUBLE, CHAR, STD::STRING ]
template<class type, char ...delimiters> 
std::vector<type> tokenize(std::string input)
{
	//temporary values to store current and created tokens
	std::vector<std::string> tokens;
	std::string token;

	std::vector<wchar_t> delims = { { delimiters... } };
	

	//return vector of wanted datatype
	std::any finTokens;


	//check if input is empty
	if (input == "")
	{
		throw Tokenizer_Error("Tokenizer Input Cannot be Empty");
	}

	//itterates over input string with i being the current character
	for (const auto& i : input)
	{
		if (std::find(delims.begin(), delims.end(), i) == delims.end())
		{
			token.push_back(i);
		}
		else
		{
			if (token != "")
			{
				tokens.push_back(token);
			}
			token.clear();
		}

		if (&i == &input.back()) //push back current token to tokens if current character is last in string
		{
			if (token != "")
			{
				tokens.push_back(token);
			}
		}
	}

	
	if (!std::is_same<std::string, type>::value) //check if wanted datatype is not a string
	{
		if (std::is_same<int, type>::value) //wanted datatype is int
		{
			std::vector<int> iTokens;

			try
			{
				for (const auto& i : tokens) //convert string tokens to int
				{
					iTokens.push_back(std::stoi(i));
				}
			}
			catch (...)
			{
				throw Tokenizer_Error("Type: \"std::string\" could not be converted to type: \"int\")");
			}

			finTokens = iTokens; //set return value
		}
		else if (std::is_same<double, type>::value) //wanted datatype is double
		{
			std::vector<double> dTokens;

			try
			{
				for (const auto& i : tokens) //convert string tokens to double
				{
					dTokens.push_back(std::stod(i));
				}
			}
			catch (...)
			{
				throw Tokenizer_Error("Type: \"std::string\" could not be converted to type: \"double\")");
			}

			finTokens = dTokens; //set return value
		}
		else if (std::is_same<char, type>::value) //wanted datatype is char
		{
			std::vector<char> cTokens;

				
			for (int i = 0; i < tokens.size(); i++) //convert string tokens to char
			{
				cTokens.push_back(tokens[i][0]);

				if (tokens[i][1]) //check if tokens contain more than one character
				{
					throw Tokenizer_Error("Type: \"std::string\" could not be converted to type: \"char\")");
				}
			}
				

			finTokens = cTokens; //set return value
		}
		else
		{
			finTokens = "";


			//throw error if wanted datatype is not supported with some string trickery
			throw Tokenizer_Error("Invalid Tokenizer Data Type");
		}
	}
	else //if wanted datatype is string set return value with string tokens
	{
		finTokens = tokens;
	}
	

	return std::any_cast<std::vector<type>>(finTokens);;
}