#pragma once

#include <map>
#include <string>

//enum for switch statements using data types
static enum switchTypeEnum
{
	str,
	i,
	d,
	f,
	c
};

//map that contains data type as string from typeid(type).name() and an enum
//use this in a switch statement using switch(returnTypeMap[typeid(type).name])
static std::map<std::string, int> switchType =
{
	{"class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >", str},
	{"int", i},
	{"double", d},
	{"float", f},
	{"char", c}
};