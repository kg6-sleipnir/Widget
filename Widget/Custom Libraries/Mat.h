#pragma once

#include <array>
#include <exception>
#include <iostream>
#include <vector>

namespace Mat
{

	class MATRIX_ERROR : public std::exception
	{
	public:
		//construct an error with a message
		MATRIX_ERROR(char const* const message) : std::exception(message) {}

		//get error message
		char const* what() const noexcept
		{
			return exception::what();
		}
	};

	//two dimensional matrix of floats
	class V2f
	{
	private:

		std::vector<std::vector<float>> matrix;

		int h;

		int w;

	public:

		V2f();

		V2f(int height, int width);

		V2f(std::vector<std::vector<float>> construct);

		std::vector<float>& operator[](int pos);

		V2f operator+(V2f mat2);
		
		V2f operator+(float val);

		V2f operator-(V2f mat2);

		V2f operator-(float val);
		
		V2f operator*(V2f mat2);

		V2f operator*(float val);

		V2f transpose();

		std::vector<float> sumRows();

		std::vector<float> sumCols();

		void ex();

		void printMatrix();
		
		int height();
		
		int width();
		
	};

	class V2i
	{
	private:

		std::vector<std::vector<int>> matrix;

		int h;

		int w;

	public:

		V2i();

		V2i(int height, int width);

		V2i(std::vector<std::vector<int>> construct);
		

		std::vector<int>& operator[](int pos);

		V2i operator+(V2i mat2);

		V2i operator+(int val);

		V2i operator-(V2i mat2);

		V2i operator-(int val);

		V2i operator*(V2i mat2);

		V2i operator*(int val);

		V2i transpose();

		std::vector<int> sumRows();

		std::vector<int> sumCols();

		void ex();

		void printMatrix();

		int height();

		int width();

	};

	class V2d
	{
	private:

		std::vector<std::vector<double>> matrix;

		int h;

		int w;

	public:

		V2d();

		V2d(int height, int width);

		V2d(std::vector<std::vector<double>> construct);

		std::vector<double>& operator[](int pos);

		V2d operator+(V2d mat2);

		V2d operator+(double val);

		V2d operator-(V2d mat2);

		V2d operator-(double val);

		V2d operator*(V2d mat2);

		V2d operator*(double val);

		V2d transpose();

		std::vector<double> sumRows();

		std::vector<double> sumCols();

		void ex();

		void printMatrix();

		int height();

		int width();

	};

	V2f addOuterf(std::vector<float> v1, std::vector<float> v2);

	V2i addOuteri(std::vector<int> v1, std::vector<int> v2);

	V2d addOuterd(std::vector<double> v1, std::vector<double> v2);

}