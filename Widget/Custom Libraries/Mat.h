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

		size_t h;

		size_t w;

	public:

		V2f() { h = 0; w = 0; };

		V2f(int height, int width)
		{

			h = height;
			w = width;

			matrix = std::vector<std::vector<float>>(height, std::vector<float>(width, 0.0f));
		}

		V2f(std::vector<std::vector<float>> construct)
		{
			if (construct.size() != h)
			{
				throw MATRIX_ERROR("Error constructing V2f, constructor size not equal to matrix size");
			}

			matrix = construct;
		}

		std::vector<float>& operator[](size_t pos)
		{
			return matrix[pos];
		}

		V2f operator+(V2f mat2)
		{
			if (mat2.height() != this->height() or mat2.width() != this->width())
			{
				throw MATRIX_ERROR("Error adding matrices, sizes are unequal");
			}


			V2f returnMatrix(this->height(), this->width());


			for (int i = 0; i < this->height(); i++)
			{
				for (int j = 0; j < this->width(); j++)
				{
					returnMatrix[i][j] = matrix[i][j] + mat2[i][j];
				}
			}

			return returnMatrix;
		}


		V2f operator+(float val)
		{
			V2f returnMatrix(this->height(), this->width());

			for (int i = 0; i < returnMatrix.height(); i++)
			{
				for (int j = 0; j < returnMatrix.width(); j++)
				{
					returnMatrix[i][j] = matrix[i][j] + val;
				}
			}

			return returnMatrix;
		}

		V2f operator-(V2f mat2)
		{
			if (mat2.height() != this->height() or mat2.width() != this->width())
			{
				throw MATRIX_ERROR("Error subtracting matrices, sizes are unequal");
			}


			V2f returnMatrix(this->height(), this->width());


			for (int i = 0; i < this->height(); i++)
			{
				for (int j = 0; j < this->width(); j++)
				{
					returnMatrix[i][j] = matrix[i][j] - mat2[i][j];
				}
			}

			return returnMatrix;
		}

		V2f operator-(float val)
		{
			V2f returnMatrix(this->height(), this->width());

			for (int i = 0; i < returnMatrix.height(); i++)
			{
				for (int j = 0; j < returnMatrix.width(); j++)
				{
					returnMatrix[i][j] = matrix[i][j] - val;
				}
			}

			return returnMatrix;
		}

		V2f operator*(V2f mat2)
		{
			if (this->width() != mat2.height())
			{
				throw MATRIX_ERROR("Error multiplying matrices, width of first must equal height of second");
			}

			V2f returnMatrix(this->height(), mat2.width());

			for (int y1 = 0; y1 < this->height(); y1++)
			{
				for (int x2 = 0; x2 < mat2.width(); x2++)
				{
					for (int i = 0; i < this->width(); i++)
					{
						returnMatrix[y1][x2] += matrix[y1][i] * mat2[i][x2];
					}
				}
			}

			return returnMatrix;
		}

		V2f operator*(float val)
		{
			V2f returnMatrix(this->height(), this->width());


			for (int i = 0; i < this->height(); i++)
			{
				for (int j = 0; j < this->width(); j++)
				{
					returnMatrix[i][j] = matrix[i][j] * val;
				}
			}

			return returnMatrix;
		}

		V2f transpose()
		{
			V2f tempMatrix(this->width(), this->height());

			for (int i = 0; i < this->height(); i++)
			{
				for (int j = 0; j < this->width(); j++)
				{
					tempMatrix[j][i] = matrix[i][j];
				}
			}

			return tempMatrix;
		}

		std::vector<float> sumRows()
		{
			std::vector<float> returnVector(this->width(), 0.0f);
			
			for (int i = 0; i < this->height(); i++)
			{
				for (int j = 0; j < this->width(); j++)
				{
					returnVector[j] += matrix[i][j];
				}
			}

			return returnVector;
		}

		std::vector<float> sumCols()
		{
			std::vector<float> returnVector(this->height(), 0.0f);

			for (int i = 0; i < this->height(); i++)
			{
				for (int j = 0; j < this->width(); j++)
				{
					returnVector[i] += matrix[i][j];
				}
			}

			return returnVector;
		}

		void ex()
		{
			for (int i = 0; i < this->height(); i++)
			{
				for (int j = 0; j < this->width(); j++)
				{
					matrix[i][j] = exp(matrix[i][j]);
				}
			}
		}

		void printMatrix()
		{
			for (int i = 0; i < this->height(); i++)
			{
				for (int j = 0; j < this->width(); j++)
				{
					std::cout << matrix[i][j] << " ";
				}
				std::cout << "\n";
			}
		}

		size_t height()
		{
			return h;
		}

		size_t width()
		{
			return w;
		}

	};

	class V2i
	{
	private:

		std::vector<std::vector<int>> matrix;

		size_t h;

		size_t w;

	public:

		V2i() { h = 0; w = 0; };

		V2i(int height, int width)
		{

			h = height;
			w = width;

			matrix = std::vector<std::vector<int>>(height, std::vector<int>(width, 0.0f));
		}

		V2i(std::vector<std::vector<int>> construct)
		{
			if (construct.size() != h)
			{
				throw MATRIX_ERROR("Error constructing V2i, constructor size not equal to matrix size");
			}

			matrix = construct;
		}

		std::vector<int>& operator[](size_t pos)
		{
			return matrix[pos];
		}

		V2i operator+(V2i mat2)
		{
			if (mat2.height() != this->height() or mat2.width() != this->width())
			{
				throw MATRIX_ERROR("Error adding matrices, sizes are unequal");
			}


			V2i returnMatrix(this->height(), this->width());


			for (int i = 0; i < this->height(); i++)
			{
				for (int j = 0; j < this->width(); j++)
				{
					returnMatrix[i][j] = matrix[i][j] + mat2[i][j];
				}
			}

			return returnMatrix;
		}


		V2i operator+(int val)
		{
			V2i returnMatrix(this->height(), this->width());

			for (int i = 0; i < returnMatrix.height(); i++)
			{
				for (int j = 0; j < returnMatrix.width(); j++)
				{
					returnMatrix[i][j] = matrix[i][j] + val;
				}
			}

			return returnMatrix;
		}

		V2i operator-(V2i mat2)
		{
			if (mat2.height() != this->height() or mat2.width() != this->width())
			{
				throw MATRIX_ERROR("Error subtracting matrices, sizes are unequal");
			}


			V2i returnMatrix(this->height(), this->width());


			for (int i = 0; i < this->height(); i++)
			{
				for (int j = 0; j < this->width(); j++)
				{
					returnMatrix[i][j] = matrix[i][j] - mat2[i][j];
				}
			}

			return returnMatrix;
		}

		V2i operator-(int val)
		{
			V2i returnMatrix(this->height(), this->width());

			for (int i = 0; i < returnMatrix.height(); i++)
			{
				for (int j = 0; j < returnMatrix.width(); j++)
				{
					returnMatrix[i][j] = matrix[i][j] - val;
				}
			}

			return returnMatrix;
		}

		V2i operator*(V2i mat2)
		{
			if (this->width() != mat2.height())
			{
				throw MATRIX_ERROR("Error multiplying matrices, width of first must equal height of second");
			}

			V2i returnMatrix(this->height(), mat2.width());

			for (int y1 = 0; y1 < this->height(); y1++)
			{
				for (int x2 = 0; x2 < mat2.width(); x2++)
				{
					for (int i = 0; i < this->width(); i++)
					{
						returnMatrix[y1][x2] += matrix[y1][i] * mat2[i][x2];
					}
				}
			}

			return returnMatrix;
		}

		V2i operator*(int val)
		{
			V2i returnMatrix(this->height(), this->width());


			for (int i = 0; i < this->height(); i++)
			{
				for (int j = 0; j < this->width(); j++)
				{
					returnMatrix[i][j] = matrix[i][j] * val;
				}
			}

			return returnMatrix;
		}

		V2i transpose()
		{
			V2i tempMatrix(this->width(), this->height());

			for (int i = 0; i < this->height(); i++)
			{
				for (int j = 0; j < this->width(); j++)
				{
					tempMatrix[j][i] = matrix[i][j];
				}
			}

			return tempMatrix;
		}

		std::vector<int> sumRows()
		{
			std::vector<int> returnVector(this->width(), 0.0f);

			for (int i = 0; i < this->height(); i++)
			{
				for (int j = 0; j < this->width(); j++)
				{
					returnVector[j] += matrix[i][j];
				}
			}

			return returnVector;
		}

		std::vector<int> sumCols()
		{
			std::vector<int> returnVector(this->height(), 0.0f);

			for (int i = 0; i < this->height(); i++)
			{
				for (int j = 0; j < this->width(); j++)
				{
					returnVector[i] += matrix[i][j];
				}
			}

			return returnVector;
		}

		void ex()
		{
			for (int i = 0; i < this->height(); i++)
			{
				for (int j = 0; j < this->width(); j++)
				{
					matrix[i][j] = exp(matrix[i][j]);
				}
			}
		}

		void printMatrix()
		{
			for (int i = 0; i < this->height(); i++)
			{
				for (int j = 0; j < this->width(); j++)
				{
					std::cout << matrix[i][j] << " ";
				}
				std::cout << "\n";
			}
		}

		size_t height()
		{
			return h;
		}

		size_t width()
		{
			return w;
		}

	};

	class V2d
	{
	private:

		std::vector<std::vector<double>> matrix;

		size_t h;

		size_t w;

	public:

		V2d() { h = 0; w = 0; };

		V2d(int height, int width)
		{

			h = height;
			w = width;

			matrix = std::vector<std::vector<double>>(height, std::vector<double>(width, 0.0f));
		}

		V2d(std::vector<std::vector<double>> construct)
		{
			if (construct.size() != h)
			{
				throw MATRIX_ERROR("Error constructing V2d, constructor size not equal to matrix size");
			}

			matrix = construct;
		}

		std::vector<double>& operator[](size_t pos)
		{
			return matrix[pos];
		}

		V2d operator+(V2d mat2)
		{
			if (mat2.height() != this->height() or mat2.width() != this->width())
			{
				throw MATRIX_ERROR("Error adding matrices, sizes are unequal");
			}


			V2d returnMatrix(this->height(), this->width());


			for (int i = 0; i < this->height(); i++)
			{
				for (int j = 0; j < this->width(); j++)
				{
					returnMatrix[i][j] = matrix[i][j] + mat2[i][j];
				}
			}

			return returnMatrix;
		}


		V2d operator+(double val)
		{
			V2d returnMatrix(this->height(), this->width());

			for (int i = 0; i < returnMatrix.height(); i++)
			{
				for (int j = 0; j < returnMatrix.width(); j++)
				{
					returnMatrix[i][j] = matrix[i][j] + val;
				}
			}

			return returnMatrix;
		}

		V2d operator-(V2d mat2)
		{
			if (mat2.height() != this->height() or mat2.width() != this->width())
			{
				throw MATRIX_ERROR("Error subtracting matrices, sizes are unequal");
			}


			V2d returnMatrix(this->height(), this->width());


			for (int i = 0; i < this->height(); i++)
			{
				for (int j = 0; j < this->width(); j++)
				{
					returnMatrix[i][j] = matrix[i][j] - mat2[i][j];
				}
			}

			return returnMatrix;
		}

		V2d operator-(double val)
		{
			V2d returnMatrix(this->height(), this->width());

			for (int i = 0; i < returnMatrix.height(); i++)
			{
				for (int j = 0; j < returnMatrix.width(); j++)
				{
					returnMatrix[i][j] = matrix[i][j] - val;
				}
			}

			return returnMatrix;
		}

		V2d operator*(V2d mat2)
		{
			if (this->width() != mat2.height())
			{
				throw MATRIX_ERROR("Error multiplying matrices, width of first must equal height of second");
			}

			V2d returnMatrix(this->height(), mat2.width());

			for (int y1 = 0; y1 < this->height(); y1++)
			{
				for (int x2 = 0; x2 < mat2.width(); x2++)
				{
					for (int i = 0; i < this->width(); i++)
					{
						returnMatrix[y1][x2] += matrix[y1][i] * mat2[i][x2];
					}
				}
			}

			return returnMatrix;
		}

		V2d operator*(double val)
		{
			V2d returnMatrix(this->height(), this->width());


			for (int i = 0; i < this->height(); i++)
			{
				for (int j = 0; j < this->width(); j++)
				{
					returnMatrix[i][j] = matrix[i][j] * val;
				}
			}

			return returnMatrix;
		}

		V2d transpose()
		{
			V2d tempMatrix(this->width(), this->height());

			for (int i = 0; i < this->height(); i++)
			{
				for (int j = 0; j < this->width(); j++)
				{
					tempMatrix[j][i] = matrix[i][j];
				}
			}

			return tempMatrix;
		}

		std::vector<double> sumRows()
		{
			std::vector<double> returnVector(this->width(), 0.0f);

			for (int i = 0; i < this->height(); i++)
			{
				for (int j = 0; j < this->width(); j++)
				{
					returnVector[j] += matrix[i][j];
				}
			}

			return returnVector;
		}

		std::vector<double> sumCols()
		{
			std::vector<double> returnVector(this->height(), 0.0f);

			for (int i = 0; i < this->height(); i++)
			{
				for (int j = 0; j < this->width(); j++)
				{
					returnVector[i] += matrix[i][j];
				}
			}

			return returnVector;
		}

		void ex()
		{
			for (int i = 0; i < this->height(); i++)
			{
				for (int j = 0; j < this->width(); j++)
				{
					matrix[i][j] = exp(matrix[i][j]);
				}
			}
		}

		void printMatrix()
		{
			for (int i = 0; i < this->height(); i++)
			{
				for (int j = 0; j < this->width(); j++)
				{
					std::cout << matrix[i][j] << " ";
				}
				std::cout << "\n";
			}
		}

		size_t height()
		{
			return h;
		}

		size_t width()
		{
			return w;
		}

	};

	V2f addOuterf(std::vector<float> v1, std::vector<float> v2);

	V2i addOuteri(std::vector<int> v1, std::vector<int> v2);

	V2d addOuterd(std::vector<double> v1, std::vector<double> v2);

}