#pragma once

#include <any>
#include <eh.h>
#include <iostream>
#include <map>
#include <stdarg.h>
#include <string>
#include <typeinfo>
#include <vector>

#include "../Custom Libraries/Switch Type.h"
#include "../Custom Libraries/Vector Converter.h"

/// <summary>
/// turns a string into a set of tokens of a specified type
/// </summary>
/// <param name="input">- input string</param>
/// <param name="...delimiters">- list of delimiter characters</param>
/// <returns>returns vector of an expected variable type - valid return types are [ INT, DOUBLE, CHAR, FLOAT, STD::STRING ]</returns>
template<class type, class ... delim> 
std::vector<type> tokenize(std::string input, delim... delimiters)
{
	//temporary values to store current and created tokens
	std::vector<std::string> tokens;
	std::string token;

	std::vector<wchar_t> delims{ static_cast<wchar_t>(delimiters)... };
	
	if (delims.size() == 0)
	{
		delims.push_back(' ');
	}

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


	switch (switchType[typeid(type).name()])
	{
	case str:
		
		finTokens = tokens;
		break;

	case i:

		finTokens = convertToInt(tokens);
		break;

	case d:

		finTokens = convertToDouble(tokens);
		break;

	case f:
	
		finTokens = convertToFloat(tokens);
		break;

	case c:

		finTokens = convertToChar(tokens);
		break;

	default:
		throw Tokenizer_Error("Invalid return type in tokenizer");
	}

	return std::any_cast<std::vector<type>>(finTokens);;
}