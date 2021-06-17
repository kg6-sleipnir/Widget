#pragma once

#include <any>
#include <eh.h>
#include <iostream>
#include <vector>

namespace Custom
{
	typedef std::vector<std::vector<int>> Imatrix;
	typedef std::vector<std::vector<float>> Fmatrix;
	typedef std::vector<std::vector<double>> Dmatrix;


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

	template<typename matrixType>
	matrixType multiplyMatrix(matrixType mat1, matrixType mat2)
	{
		//new matrix for return value
		matrixType matrix;


		//check if matrix type is supported
		if (typeid(matrixType) != typeid(Imatrix) and typeid(matrixType) != typeid(Dmatrix) and typeid(matrixType) != typeid(Fmatrix))
		{
			throw MATRIX_ERROR("Invalid Matrix Data Type");
		}

		//check if matrices are of equal size
		if (mat1.size() != mat2[0].size())
		{
			throw MATRIX_ERROR("Cannot Multiply Matrices of Unequal Size");
		}

		
		//create new matrix for return value
		for (int y1 = 0; y1 < mat1.size(); y1++)
		{
			typename matrixType::value_type temp;

			for (int x2 = 0; x2 < mat2[0].size(); x2++)
			{
				temp.push_back(0);
			}

			matrix.push_back(temp);
		}

		//iterate over rows in first matrix
		for (int y1 = 0; y1 < mat1.size(); y1++)
		{
			//iterate over columns in second matrix
			for (int x2 = 0; x2 < mat2[0].size(); x2++)
			{
				//iterate over elements in current column in second matrix and current row in first
				for (int i = 0; i < mat1[0].size(); i++)
				{
					matrix[y1][x2] += mat1[y1][i] * mat2[i][x2];
				}
			}
		}


		return matrix;
	}

	template<typename matrixType>
	matrixType addMatrix(matrixType mat1, matrixType mat2)
	{
		//check if matrix type is supported
		if (typeid(matrixType) != typeid(Imatrix) and typeid(matrixType) != typeid(Dmatrix) and typeid(matrixType) != typeid(Fmatrix))
		{
			throw MATRIX_ERROR("Invalid Matrix Data Type");
		}

		//check if matrices are correct size
		if (mat1.size() != mat2.size() or mat1[0].size() != mat2[0].size())
		{
			throw MATRIX_ERROR("Cannot Add Matrices of Unequal Size");
		}


		//add elements in mat2 to mat1
		for (int y = 0; y < mat1.size(); y++)
		{
			for (int x = 0; x < mat2[0].size(); y++)
			{
				mat1[y][x] += mat2[y][x];
			}
		}


		return mat1;
	}

	template<typename matrixType>
	matrixType subtractMatrix(matrixType mat1, matrixType mat2)
	{
		//check if matrix type is supported
		if (typeid(matrixType) != typeid(Imatrix) and typeid(matrixType) != typeid(Dmatrix) and typeid(matrixType) != typeid(Fmatrix))
		{
			throw MATRIX_ERROR("Invalid Matrix Data Type");
		}

		//check if matrices are correct size
		if (mat1.size() != mat2.size() or mat1[0].size() != mat2[0].size())
		{
			throw MATRIX_ERROR("Cannot Subtract Matrices of Unequal Size");
		}


		//add elements in mat2 to mat1
		for (int y = 0; y < mat1.size(); y++)
		{
			for (int x = 0; x < mat2[0].size(); y++)
			{
				mat1[y][x] -= mat2[y][x];
			}
		}


		return mat1;
	}


	//get single element from final matrix after multiplying two matrices
	template<typename T, typename matrixType>
	T getMultipliedElement(matrixType mat1, matrixType mat2, int row, int column)
	{
		T returnValue = 0;


		//check if matrix type is supported
		if (typeid(matrixType) != typeid(Imatrix) and typeid(matrixType) != typeid(Dmatrix) and typeid(matrixType) != typeid(Fmatrix))
		{
			throw MATRIX_ERROR("Invalid Matrix Data Type");
		}

		//check if matrices are of equal size
		if (mat1.size() != mat2[0].size())
		{
			throw MATRIX_ERROR("Cannot Multiply Matrices of Unequal Size");
		}

		//check if index of wanted element from final matrix is outside of the matrix's range
		if (row >= mat1.size() or column >= mat2[0].size())
		{
			throw MATRIX_ERROR("Index Out Of Range");
		}

		
		//multiply and sum the row of the first matrix with the column of the second
		for (int i = 0; i < mat1[0].size(); i++)
		{
			returnValue += mat1[row][i] * mat2[i][column];
		}


		return returnValue;
	}

	//sum rows of matrix to 1 dimensional vector
	template<typename T, typename matrixType>
	std::vector<T> sumRows(matrixType mat)
	{
		std::vector<T> returnVector;


		//check if matrix type is supported
		if (typeid(matrixType) != typeid(Imatrix) and typeid(matrixType) != typeid(Dmatrix) and typeid(matrixType) != typeid(Fmatrix))
		{
			throw MATRIX_ERROR("Invalid Matrix Data Type");
		}


		for (const auto& i : mat)
		{
			T temp = 0;

			for (const auto& j : i)
			{
				temp += j;
			}

			returnVector.push_back(temp);
		}

		return returnVector;
	}

	template<typename T, typename matrixType>
	matrixType multiplyMatrixBy(matrixType mat, T value)
	{
		matrixType returnValue = mat;
		
		//check if matrix type is supported
		if (typeid(matrixType) != typeid(Imatrix) and typeid(matrixType) != typeid(Dmatrix) and typeid(matrixType) != typeid(Fmatrix))
		{
			throw MATRIX_ERROR("Invalid Matrix Data Type");
		}

		
		for (auto& i : returnValue)
		{
			for (auto& j : i)
			{
				j *= value;
			}
		}


		return returnValue;
	}


	template<typename matrixType>
	void printMatrix(matrixType mat)
	{
		//check if matrix type is supported
		if (typeid(matrixType) != typeid(Imatrix) and typeid(matrixType) != typeid(Dmatrix) and typeid(matrixType) != typeid(Fmatrix))
		{
			throw MATRIX_ERROR("Invalid Matrix Data Type");
		}
		
		for (int y = 0; y < mat.size(); y++)
		{
			for (int x = 0; x < mat[0].size(); x++)
			{
				std::cout << mat[y][x] << " ";
			}

			std::cout << "\n";
		}
	}
}

