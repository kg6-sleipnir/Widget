#pragma once

#include <eh.h>
#include <fstream>
#include <map>
#include <string>
#include <vector>

#include "../Input Processing/Tokenizer.h"

namespace Data
{

	class dataTable
	{
	private:

		std::vector<std::map<int, int>> data;

		int targetVar = NULL;

	public:

		//create table from file
		dataTable(std::string dataFile, int target);

		//create table from scratch
		dataTable(int target);


		//get target variable in table
		int getTargetClass();

		//gets size of table
		int getTableSize();

		//check if variable in table is able to be operated on
		bool checkOperate(int varNum);

		//get counts of each unique value from table in the variable varNum
		std::map<int, int> getCounts(int varNum);

		//remove column from table
		void removeCol(int varNum);

		//add row of variables to table
		void addRow(std::map<int, int> row);

		//remove row of variables from table
		void removeRow(int lineNum);

		//get row of variables from table
		std::map<int, int> getRow(int lineNum);

		//create new table from rows that contain the value varVal in variable varNum
		dataTable splitData(int varNum, int varVal);

		//removes all rows that contain the value varVal in variable varNum from table
		void removeData(int varNum, int varVal);

	};

	struct TABLE_ERROR : public std::exception
	{

		//construct an error with a message
		TABLE_ERROR(char const* const message);

		//get error message
		virtual char const* what() const noexcept;

	};
	

}